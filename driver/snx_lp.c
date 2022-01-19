#include "snx_common.h"
#include "snx_lp.h"


#undef SNX_LP_STATS

static int SNX_PAL_MAJOR ;

#define SNX_LP_NO SNX_PAR_TOTAL_MAX
//#define SNX_CONFIG_LP_CONSOLE
#undef SNX_CONFIG_LP_CONSOLE

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
static devfs_handle_t snx_devfs_handle;
#endif

static struct snx_lp_struct snx_lp_table[SNX_LP_NO];

static unsigned int snx_lp_count;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
static struct class *snx_lp_class;
#else
#endif

#ifdef SNX_CONFIG_LP_CONSOLE
static struct snx_parport *console_registered;
#endif


#define SNX_LP_PREEMPT_REQUEST 1
#define SNX_LP_PARPORT_CLAIMED 2


#define r_dtr(x)		(sunix_parport_read_data(snx_lp_table[(x)].dev->port))
#define r_str(x)		(sunix_parport_read_status(snx_lp_table[(x)].dev->port))
#define w_ctr(x, y)	do { sunix_parport_write_control(snx_lp_table[(x)].dev->port, (y)); } while (0)
#define w_dtr(x, y)	do { sunix_parport_write_data(snx_lp_table[(x)].dev->port, (y)); } while (0)



static void snx_lp_claim_parport_or_block(struct snx_lp_struct *this_lp)
{
	if (!test_and_set_bit(SNX_LP_PARPORT_CLAIMED, &this_lp->bits)) {
		sunix_parport_claim_or_block(this_lp->dev);
	}
}


static void snx_lp_release_parport(struct snx_lp_struct *this_lp)
{
	if (test_and_clear_bit(SNX_LP_PARPORT_CLAIMED, &this_lp->bits)) {
		sunix_parport_release(this_lp->dev);
	}
}


static int snx_lp_preempt(void *handle)
{
	struct snx_lp_struct *this_lp = (struct snx_lp_struct *)handle;
	set_bit(SNX_LP_PREEMPT_REQUEST, &this_lp->bits);
	return 1;
}


static int snx_lp_negotiate(struct snx_parport *port, int mode)
{
	if (sunix_parport_negotiate(port, mode) != 0) {
		mode = IEEE1284_MODE_COMPAT;
		sunix_parport_negotiate(port, mode);
	}
	return (mode);
}


static int snx_lp_reset(int minor)
{
	int retval;
	snx_lp_claim_parport_or_block(&snx_lp_table[minor]);

	w_ctr(minor, SNX_LP_PSELECP);

	udelay(SNX_LP_DELAY);

	w_ctr(minor, SNX_LP_PSELECP | SNX_LP_PINITP);

	retval = r_str(minor);

	snx_lp_release_parport(&snx_lp_table[minor]);
	return retval;
}


static void snx_lp_error(int minor)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	DEFINE_WAIT(wait);
#endif
	int polling;

	if (SNX_LP_F(minor) & SNX_LP_ABORT) {
		return;
	}

	polling = snx_lp_table[minor].dev->port->irq == PARPORT_IRQ_NONE;

	if (polling) {
		snx_lp_release_parport(&snx_lp_table[minor]);
	}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	prepare_to_wait(&snx_lp_table[minor].waitq, &wait, TASK_INTERRUPTIBLE);

	schedule_timeout(SNX_LP_TIMEOUT_POLLED);
	finish_wait(&snx_lp_table[minor].waitq, &wait);
#else
	interruptible_sleep_on_timeout(&snx_lp_table[minor].waitq, SNX_LP_TIMEOUT_POLLED);
#endif

	if (polling) {
		snx_lp_claim_parport_or_block(&snx_lp_table[minor]);
	} else {
		sunix_parport_yield_blocking(snx_lp_table[minor].dev);
	}
}


static int snx_lp_check_status(int minor)
{
	int error = 0;
	unsigned int last = snx_lp_table[minor].last_error;
	unsigned char status = r_str(minor);

	if ((status & SNX_LP_PERRORP) && !(SNX_LP_F(minor) & SNX_LP_CAREFUL)) {
		last = 0;
	} else if ((status & SNX_LP_POUTPA)) {
		if (last != SNX_LP_POUTPA) {
			last = SNX_LP_POUTPA;
			printk("SNX Info : lp%d port out of paper.\n", minor);
		}
		error = -ENOSPC;
	} else if (!(status & SNX_LP_PSELECD)) {
		if (last != SNX_LP_PSELECD) {
			last = SNX_LP_PSELECD;
			printk("SNX Info : lp%d port off-line.\n", minor);
		}
		error = -EIO;
	} else if (!(status & SNX_LP_PERRORP)) {
		if (last != SNX_LP_PERRORP) {
			last = SNX_LP_PERRORP;
			printk("SNX Info : lp%d port on fire.\n", minor);
		}
		error = -EIO;
	} else {
		last = 0;
	}

	snx_lp_table[minor].last_error = last;

	if (last != 0) {
		snx_lp_error(minor);
	}
	return error;
}


static int snx_lp_wait_ready(int minor, int nonblock)
{
	int error = 0;

	if (snx_lp_table[minor].current_mode != IEEE1284_MODE_COMPAT) {
		return 0;
	}

	do {
		error = snx_lp_check_status(minor);

		if (error && (nonblock || (SNX_LP_F(minor) & SNX_LP_ABORT))) {
			break;
		}

		if (signal_pending(current)) {
			error = -EINTR;
			break;
		}
	} while (error);

	return error;
}


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
static ssize_t snx_lp_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
#else
static ssize_t snx_lp_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 19))
    unsigned int minor = iminor(file->f_path.dentry->d_inode);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
    unsigned int minor = iminor(file->f_dentry->d_inode);
#else
    unsigned int minor = MINOR (file->f_dentry->d_inode->i_rdev);
#endif


	struct snx_parport *port = snx_lp_table[minor].dev->port;
	char *kbuf = snx_lp_table[minor].lp_buffer;

	ssize_t retv = 0;
	ssize_t written;
	size_t copy_size = count;
	int nonblock = ((file->f_flags & O_NONBLOCK) || (SNX_LP_F(minor) & SNX_LP_ABORT));


#ifdef SNX_LP_STATS
	if (time_after(jiffies, snx_lp_table[minor].lastcall + SNX_LP_TIME(minor))) {
		snx_lp_table[minor].runchars = 0;
	}

	snx_lp_table[minor].lastcall = jiffies;
#endif

	if (copy_size > SNX_LP_BUFFER_SIZE) {
		copy_size = SNX_LP_BUFFER_SIZE;
	}
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24))
	if (down_interruptible(&snx_lp_table[minor].port_mutex))
#else
	if (mutex_lock_interruptible(&snx_lp_table[minor].port_mutex))
#endif
	{
		return -EINTR;
	}

	if (copy_from_user(kbuf, buf, copy_size)) {
		retv = -EFAULT;
		goto out_unlock;
	}

	snx_lp_claim_parport_or_block(&snx_lp_table[minor]);

	snx_lp_table[minor].current_mode = snx_lp_negotiate(port, snx_lp_table[minor].best_mode);

	sunix_parport_set_timeout(snx_lp_table[minor].dev, (nonblock ? SNX_PARPORT_INACTIVITY_O_NONBLOCK : snx_lp_table[minor].timeout));

	retv = snx_lp_wait_ready(minor, nonblock);

	//if ((retv = snx_lp_wait_ready(minor, nonblock)) == 0)

	do {
		written = sunix_parport_write(port, kbuf, copy_size);
		if (written > 0) {
			copy_size -= written;
			count -= written;
			buf  += written;
			retv += written;
		}

		if (signal_pending(current)) {
			if (retv == 0) {
				retv = -EINTR;
			}
			break;
		}

		if (copy_size > 0) {
			int error;

			sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);
			snx_lp_table[minor].current_mode = IEEE1284_MODE_COMPAT;

			error = snx_lp_wait_ready(minor, nonblock);

			if (error) {
				if (retv == 0) {
					retv = error;
				}
				break;
			} else if (nonblock) {
				if (retv == 0) {
					retv = -EAGAIN;
				}
				break;
			}

			sunix_parport_yield_blocking(snx_lp_table[minor].dev);
			snx_lp_table[minor].current_mode = snx_lp_negotiate(port, snx_lp_table[minor].best_mode);

		}
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 19))
		else if (need_resched()) {
			schedule ();
		}
#else
		else if (current->need_resched) {
			schedule ();
		}
#endif

		if (count) {
			copy_size = count;
			if (copy_size > SNX_LP_BUFFER_SIZE) {
				copy_size = SNX_LP_BUFFER_SIZE;
			}

			if (copy_from_user(kbuf, buf, copy_size)) {
				if (retv == 0) {
					retv = -EFAULT;
				}
				break;
			}
		}
	} while (count > 0);

	if (test_and_clear_bit(SNX_LP_PREEMPT_REQUEST, &snx_lp_table[minor].bits)) {
		printk("SNX Info : lp%d releasing parport.\n", minor);
		sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);

		snx_lp_table[minor].current_mode = IEEE1284_MODE_COMPAT;

		snx_lp_release_parport(&snx_lp_table[minor]);
	}

out_unlock:
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24))
	up(&snx_lp_table[minor].port_mutex);
#else
	mutex_unlock(&snx_lp_table[minor].port_mutex);
#endif
	return retv;
}


#ifdef SNX_CONFIG_PARPORT_1284

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
static ssize_t snx_lp_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
#else
static ssize_t snx_lp_read(struct file *file, char *buf, size_t count, loff_t *ppos)
#endif
{

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 19))
    unsigned int minor = iminor(file->f_path.dentry->d_inode);
    DEFINE_WAIT(wait);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
    unsigned int minor = iminor(file->f_dentry->d_inode);
    DEFINE_WAIT(wait);
#else
    unsigned int minor = MINOR(file->f_dentry->d_inode->i_rdev);
#endif

    struct snx_parport *port = snx_lp_table[minor].dev->port;
	ssize_t retval = 0;
	char *kbuf = snx_lp_table[minor].lp_buffer;
	int nonblock = ((file->f_flags & O_NONBLOCK) || (SNX_LP_F(minor) & SNX_LP_ABORT));

	if (count > SNX_LP_BUFFER_SIZE) {
		count = SNX_LP_BUFFER_SIZE;
	}

	if (down_interruptible(&snx_lp_table[minor].port_mutex)) {
		return -EINTR;
	}

	snx_lp_claim_parport_or_block(&snx_lp_table[minor]);

	sunix_parport_set_timeout(snx_lp_table[minor].dev, (nonblock ? PARPORT_INACTIVITY_O_NONBLOCK : snx_lp_table[minor].timeout));

	sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);

	if (sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_NIBBLE)) {
		retval = -EIO;
		goto out;
	}

	while (retval == 0) {
		retval = sunix_parport_read(port, kbuf, count);

		if (retval > 0) {
			break;
		}

		if (nonblock) {
			retval = -EAGAIN;
			break;
		}

		if (snx_lp_table[minor].dev->port->irq == PARPORT_IRQ_NONE) {
			sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);
			snx_lp_error(minor);

			if (sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_NIBBLE)) {
				retval = -EIO;
				goto out;
			}
		} else {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
			prepare_to_wait(&snx_lp_table[minor].waitq, &wait, TASK_INTERRUPTIBLE);
			schedule_timeout(SNX_LP_TIMEOUT_POLLED);
			finish_wait(&snx_lp_table[minor].waitq, &wait);
#else
			interruptible_sleep_on_timeout(&snx_lp_table[minor].waitq, SNX_LP_TIMEOUT_POLLED);
#endif
		}

		if (signal_pending(current)) {
			retval = -ERESTARTSYS;
			break;
		}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		cond_resched();
#else
		if (current->need_resched) {
			schedule();
		}
#endif
	}

	sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);

out:
	snx_lp_release_parport(&snx_lp_table[minor]);

	if (retval > 0 && copy_to_user(buf, kbuf, retval)) {
		retval = -EFAULT;
	}

	up(&snx_lp_table[minor].port_mutex);

	return retval;
}
#endif


static int snx_lp_open(struct inode *inode, struct file *file)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
    unsigned int minor = iminor(inode);
#else
	unsigned int minor = MINOR(inode->i_rdev);
#endif

	if (minor >= SNX_LP_NO) {
		return -ENXIO;
	}

	if ((SNX_LP_F(minor) & SNX_LP_EXIST) == 0) {
		return -ENXIO;
	}

	if (test_and_set_bit(SNX_LP_BUSY_BIT_POS, &SNX_LP_F(minor))) {
		return -EBUSY;
	}

	if ((SNX_LP_F(minor) & SNX_LP_ABORTOPEN) && !(file->f_flags & O_NONBLOCK)) {
		int status;
		snx_lp_claim_parport_or_block(&snx_lp_table[minor]);
		status = r_str(minor);
		snx_lp_release_parport(&snx_lp_table[minor]);

		if (status & SNX_LP_POUTPA) {
			printk("SNX Error: lp%d out of paper.\n", minor);
			SNX_LP_F(minor) &= ~SNX_LP_BUSY;
			return -ENOSPC;
		} else if (!(status & SNX_LP_PSELECD)) {
			printk("SNX Error: lp%d off-line.\n", minor);
			SNX_LP_F(minor) &= ~SNX_LP_BUSY;
			return -EIO;
		} else if (!(status & SNX_LP_PERRORP)) {
			printk("SNX Error: lp%d printer error.\n", minor);
			SNX_LP_F(minor) &= ~SNX_LP_BUSY;
			return -EIO;
		}
	}

	snx_lp_table[minor].lp_buffer = kmalloc(SNX_LP_BUFFER_SIZE, GFP_KERNEL);

	if (!snx_lp_table[minor].lp_buffer) {
		SNX_LP_F(minor) &= ~SNX_LP_BUSY;
		return -ENOMEM;
	}

	snx_lp_claim_parport_or_block(&snx_lp_table[minor]);

	if ((snx_lp_table[minor].dev->port->modes & PARPORT_MODE_ECP) && !sunix_parport_negotiate (snx_lp_table[minor].dev->port, IEEE1284_MODE_ECP)) {
		printk("SNX Info : lp%d ECP mode.\n", minor);
		snx_lp_table[minor].best_mode = IEEE1284_MODE_ECP;
	} else {
		snx_lp_table[minor].best_mode = IEEE1284_MODE_COMPAT;
	}

	sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);

	snx_lp_release_parport(&snx_lp_table[minor]);
	snx_lp_table[minor].current_mode = IEEE1284_MODE_COMPAT;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		try_module_get(THIS_MODULE);
#else
		MOD_INC_USE_COUNT;
#endif

	return 0;
}


static int snx_lp_release(struct inode *inode, struct file *file)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
    unsigned int minor = iminor(inode);
#else
	unsigned int minor = MINOR(inode->i_rdev);
#endif

	snx_lp_claim_parport_or_block(&snx_lp_table[minor]);
	sunix_parport_negotiate(snx_lp_table[minor].dev->port, IEEE1284_MODE_COMPAT);

	snx_lp_table[minor].current_mode = IEEE1284_MODE_COMPAT;
	snx_lp_release_parport(&snx_lp_table[minor]);
	kfree(snx_lp_table[minor].lp_buffer);
	snx_lp_table[minor].lp_buffer = NULL;
	SNX_LP_F(minor) &= ~SNX_LP_BUSY;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		module_put(THIS_MODULE);
#else
		MOD_DEC_USE_COUNT;
#endif

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int snx_lp_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
    unsigned int minor = iminor(inode);
#else
	unsigned int minor = MINOR(inode->i_rdev);
#endif

	int status;
	int retval = 0;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
	void __user *argp = (void __user *)arg;
#endif


	if (minor >= SNX_LP_NO) {
		return -ENODEV;
	}

	if ((SNX_LP_F(minor) & SNX_LP_EXIST) == 0) {
		return -ENODEV;
	}

		switch (cmd) {
		struct timeval par_timeout;
		long to_jiffies;

		case SNX_LPTIME:
			SNX_LP_TIME(minor) = arg * HZ/100;
			break;


		case SNX_LPCHAR:
			SNX_LP_CHAR(minor) = arg;
			break;


		case SNX_LPABORT:
			if (arg) {
				SNX_LP_F(minor) |= SNX_LP_ABORT;
			} else {
				SNX_LP_F(minor) &= ~SNX_LP_ABORT;
			}
			break;


		case SNX_LPABORTOPEN:
			if (arg) {
				SNX_LP_F(minor) |= SNX_LP_ABORTOPEN;
			} else {
				SNX_LP_F(minor) &= ~SNX_LP_ABORTOPEN;
			}
			break;


		case SNX_LPCAREFUL:
			if (arg) {
				SNX_LP_F(minor) |= SNX_LP_CAREFUL;
			} else {
				SNX_LP_F(minor) &= ~SNX_LP_CAREFUL;
			}
			break;

		case SNX_LPWAIT:
			SNX_LP_WAIT(minor) = arg;
			break;


		case SNX_LPSETIRQ:
			return -EINVAL;
			break;


		case SNX_LPGETIRQ:
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
			if (copy_to_user(argp, &SNX_LP_IRQ(minor), sizeof(int)))
#else
			if (copy_to_user((int *) arg, &SNX_LP_IRQ(minor), sizeof(int)))
#endif
			{
				return -EFAULT;
			}
			break;


		case SNX_LPGETSTATUS:
			snx_lp_claim_parport_or_block(&snx_lp_table[minor]);
			status = r_str(minor);
			snx_lp_release_parport(&snx_lp_table[minor]);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
			if (copy_to_user(argp, &status, sizeof(int)))
#else
			if (copy_to_user((int *) arg, &status, sizeof(int)))
#endif
			{
				return -EFAULT;
			}
			break;


		case SNX_LPRESET:
			snx_lp_reset(minor);
			break;


#ifdef SNX_LP_STATS
		case SNX_LPGETSTATS:

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
			if (copy_to_user(argp, &SNX_LP_STAT(minor),	sizeof(struct snx_lp_stats)))
#else
			if (copy_to_user((int *) arg, &SNX_LP_STAT(minor), sizeof(struct snx_lp_stats)))
#endif
			{
				return -EFAULT;
			}

			if (capable(CAP_SYS_ADMIN)) {
				memset(&SNX_LP_STAT(minor), 0, sizeof(struct snx_lp_stats));
			}
			break;
#endif


		case SNX_LPGETFLAGS:
			status = SNX_LP_F(minor);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
			if (copy_to_user(argp, &status, sizeof(int)))
#else
			if (copy_to_user((int *) arg, &status, sizeof(int)))
#endif
			{
				return -EFAULT;
			}
			break;


		case SNX_LPSETTIMEOUT:
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
			if (copy_from_user(&par_timeout, argp, sizeof (struct timeval)))
#else
			if (copy_from_user(&par_timeout, (struct timeval *) arg, sizeof (struct timeval)))
#endif
			{
				return -EFAULT;
			}

			if ((par_timeout.tv_sec < 0) || (par_timeout.tv_usec < 0)) {
				return -EINVAL;
			}

			to_jiffies = SNX_ROUND_UP(par_timeout.tv_usec, 1000000/HZ);
			to_jiffies += par_timeout.tv_sec * (long) HZ;
			if (to_jiffies <= 0) {
				return -EINVAL;
			}
			snx_lp_table[minor].timeout = to_jiffies;
			break;


		default:
			retval = -EINVAL;
	}

	return retval;
}
#endif

static struct file_operations snx_lp_fops = {
	.owner		= THIS_MODULE,
	.write		= snx_lp_write,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
  .ioctl    = snx_lp_ioctl,
#endif
	.open		= snx_lp_open,
	.release	= snx_lp_release,
#ifdef SNX_CONFIG_PARPORT_1284
	.read		= snx_lp_read,
#endif
};


#ifdef SNX_CONFIG_LP_CONSOLE
#define SNX_CONSOLE_LP 0

#define SNX_CONSOLE_LP_STRICT 1

static void snx_lp_console_write(struct console *co, const char *s, unsigned count)
{
	struct snx_pardevice *dev = snx_lp_table[SNX_CONSOLE_LP].dev;
	struct snx_parport *port = dev->port;
	ssize_t written;

	if (sunix_parport_claim(dev)) {
		return;
	}

	sunix_parport_set_timeout(dev, 0);

	sunix_parport_negotiate(port, IEEE1284_MODE_COMPAT);

	do {
		ssize_t canwrite = count;
		char *lf = memchr(s, '\n', count);

		if (lf) {
			canwrite = lf - s;
		}

		if (canwrite > 0) {
			written = sunix_parport_write(port, s, canwrite);

			if (written <= 0) {
				continue;
			}

			s += written;
			count -= written;
			canwrite -= written;
		}

		if (lf && canwrite <= 0) {
			const char *crlf = "\r\n";
			int i = 2;

			s++;
			count--;
			do {
				written = sunix_parport_write(port, crlf, i);
				if (written > 0) {
					i -= written, crlf += written;
				}
			} while (i > 0 && (SNX_CONSOLE_LP_STRICT || written > 0));
		}
	} while (count > 0 && (SNX_CONSOLE_LP_STRICT || written > 0));

	sunix_parport_release(dev);
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
static struct console snx_lpcons = {
	.name		= "lx",
	.write		= snx_lp_console_write,
	.flags		= CON_PRINTBUFFER,
};
#else
static kdev_t snx_lp_console_device(struct console *c)
{
	return MKDEV(SNX_PAL_MAJOR, SNX_CONSOLE_LP);
}

static struct console snx_lpcons = {
	name:		"lx",
	write :		snx_lp_console_write,
	device :		snx_lp_console_device,
	flags :		CON_PRINTBUFFER,
};
#endif
#endif


static int snx_parport_nr[SNX_LP_NO] = {0, 1, 2, 3};
static int reset;


static int snx_lp_register(int nr, struct snx_parport *port)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
	char name[8];
#endif

	snx_lp_table[nr].dev = sunix_parport_register_device(port, "lx", snx_lp_preempt, NULL, NULL, 0, (void *) &snx_lp_table[nr]);

	if (snx_lp_table[nr].dev == NULL) {
		return 1;
	}

	snx_lp_table[nr].flags |= SNX_LP_EXIST;

	if (reset) {
		snx_lp_reset(nr);
	}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
	sprintf(name, "%d", nr);
	devfs_register(snx_devfs_handle, name, DEVFS_FL_DEFAULT, SNX_PAL_MAJOR, nr, S_IFCHR | S_IRUGO | S_IWUGO,	&snx_lp_fops, NULL);

#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
	devfs_mk_cdev(MKDEV(SNX_PAL_MAJOR, nr), S_IFCHR | S_IRUGO | S_IWUGO, "printer/%d", nr);
#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 23))
	class_device_create(snx_lp_class, NULL, MKDEV(SNX_PAL_MAJOR, nr), NULL, "lp%d", nr);
#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 26))
	device_create(snx_lp_class, NULL, MKDEV(SNX_PAL_MAJOR, nr), "lp%d", nr);
#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 27))
	device_create_drvdata(snx_lp_class, NULL, MKDEV(SNX_PAL_MAJOR, nr), NULL, "lp%d", nr);
#else
	device_create(snx_lp_class, NULL, MKDEV(SNX_PAL_MAJOR, nr), NULL, "lp%d", nr);
#endif

	printk("SNX Info : lp%d port using %s (%s).\n", nr, port->name, (port->irq == PARPORT_IRQ_NONE)?"polling":"interrupt-driven");

#ifdef SNX_CONFIG_LP_CONSOLE

	if (!nr) {
		if (port->modes & PARPORT_MODE_SAFEININT) {
			register_console(&snx_lpcons);
			console_registered = port;
			printk("SNX Info : lp%d port console ready.\n", SNX_CONSOLE_LP);
		} else {
			printk("SNX Info : lp%d port cannot run console on %s.\n", SNX_CONSOLE_LP, port->name);
		}
	}
#endif
	return 0;
}


static void snx_lp_attach(struct snx_parport *port)
{
	unsigned int i;

	for (i = 0; i < SNX_LP_NO; i++) {
		if (port->number == snx_parport_nr[i]) {
			if (!snx_lp_register(i, port)) {
				snx_lp_count++;
			}
			break;
		}
	}
}


static void snx_lp_detach(struct snx_parport *port)
{
#ifdef SNX_CONFIG_LP_CONSOLE
	if (console_registered == port) {
		unregister_console (&snx_lpcons);
		console_registered = NULL;
	}

#endif
}


static struct snx_parport_driver snx_lp_driver = {
	.name = "lx",
	.attach = snx_lp_attach,
	.detach = snx_lp_detach,
};


static int snx_lp_init(void)
{
	int i, err = 0;

	for (i = 0; i < SNX_LP_NO; i++) {
		snx_lp_table[i].dev = NULL;
		snx_lp_table[i].flags = 0;
		snx_lp_table[i].chars = SNX_LP_INIT_CHAR;
		snx_lp_table[i].time = SNX_LP_INIT_TIME;
		snx_lp_table[i].wait = SNX_LP_INIT_WAIT;
		snx_lp_table[i].lp_buffer = NULL;

#ifdef SNX_LP_STATS
		snx_lp_table[i].lastcall = 0;
		snx_lp_table[i].runchars = 0;
		memset(&snx_lp_table[i].stats, 0, sizeof (struct snx_lp_stats));
#endif
		snx_lp_table[i].last_error = 0;
		init_waitqueue_head (&snx_lp_table[i].waitq);
		init_waitqueue_head (&snx_lp_table[i].dataq);
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24))
		init_MUTEX(&snx_lp_table[i].port_mutex);
#else
		mutex_init(&snx_lp_table[i].port_mutex);
#endif
		snx_lp_table[i].timeout = 10 * HZ;
	}

	SNX_PAL_MAJOR = register_chrdev(0, "lx", &snx_lp_fops);

	if (SNX_PAL_MAJOR < 0) {
		printk("SNX Error: lp unable to get major \n");
		return -EIO;
	}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
    snx_devfs_handle = devfs_mk_dir(NULL, "sprinter", NULL);
#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
	devfs_mk_dir("sprinter");
#else
	snx_lp_class = class_create(THIS_MODULE, "sprinter");

	if (IS_ERR(snx_lp_class)) {
		err = PTR_ERR(snx_lp_class);
		goto out_reg;
	}
#endif

	if (sunix_parport_register_driver(&snx_lp_driver)) {
		printk("SNX Error: lp unable to register with parport.\n");
		err = -EIO;
		goto out_class;
	}

	if (!snx_lp_count) {
		printk("SNX Warng: lp driver loaded but no devices found.\n");
	}

	return 0;

out_class:

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))

#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
	devfs_remove("sprinter");
#else
	class_destroy(snx_lp_class);

out_reg:
#endif

	unregister_chrdev(SNX_PAL_MAJOR, "lx");
	return err;
}


int sunix_par_lp_init(void)
{
	int status = 0;
	status = snx_lp_init();
	return status;
}


void sunix_par_lp_exit(void)
{
	unsigned int offset;
	sunix_parport_unregister_driver(&snx_lp_driver);

#ifdef SNX_CONFIG_LP_CONSOLE
	unregister_console(&snx_lpcons);
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
    devfs_unregister(snx_devfs_handle);
#endif

	unregister_chrdev(SNX_PAL_MAJOR, "lx");


	for (offset = 0; offset < SNX_LP_NO; offset++) {
		if (snx_lp_table[offset].dev == NULL) {
			continue;
		}

		sunix_parport_unregister_device(snx_lp_table[offset].dev);

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))

#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 23))
		class_device_destroy(snx_lp_class, MKDEV(SNX_PAL_MAJOR, offset));
#else
		device_destroy(snx_lp_class, MKDEV(SNX_PAL_MAJOR, offset));
#endif
	}


#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 0))

#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
	devfs_remove("sprinter");
#else
	class_destroy(snx_lp_class);
#endif
}

