#include "snx_ppdev.h"
#include <linux/time.h>

#define SNX_PARPORT_MAX 4
#define SNX_CHRDEV "sppdev"

static int SNX_PPD_MAJOR;

struct snx_pp_struct {
        struct snx_pardevice    *pdev;
        wait_queue_head_t irq_wait;
        atomic_t irqc;
        unsigned int flags;
        int irqresponse;
        unsigned char irqctl;
        struct ieee1284_info state;
        struct ieee1284_info saved_state;
        long default_inactivity;
};


#define SNX_PP_CLAIMED    (1<<0)
#define SNX_PP_EXCL       (1<<1)


#define SNX_PP_INTERRUPT_TIMEOUT    (10 * HZ)
#define SNX_PP_BUFFER_SIZE          1024
#define SNX_PARDEVICE_MAX           SNX_PAR_TOTAL_MAX


static inline void snx_pp_enable_irq(struct snx_pp_struct *pp)
{
        struct snx_parport *port = pp->pdev->port;
        port->ops->enable_irq(port);
}


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
static ssize_t snx_pp_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
#else
static ssize_t snx_pp_read(struct file *file, char *buf, size_t count, loff_t *ppos)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 19))
        unsigned int minor = iminor(file->f_path.dentry->d_inode);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
        unsigned int minor = iminor(file->f_dentry->d_inode);
#else
        unsigned int minor = MINOR (file->f_dentry->d_inode->i_rdev);
#endif

        struct snx_pp_struct *pp = file->private_data;
        char *kbuffer;
        ssize_t bytes_read = 0;
        struct snx_parport *pport;
        int mode;

        if (!(pp->flags & SNX_PP_CLAIMED)) {
                printk("snx_pp_read: %x claim the port first\n", minor);
                return -EINVAL;
        }

        if (count == 0) {
                return 0;
        }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 9))
        kbuffer = kmalloc(min_t(size_t, count, SNX_PP_BUFFER_SIZE), GFP_KERNEL);
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 8))
        kbuffer = kmalloc(min(size_t, count, SNX_PP_BUFFER_SIZE), GFP_KERNEL);
#else
        kbuffer = kmalloc(min(count, SNX_PP_BUFFER_SIZE), GFP_KERNEL);
#endif
        if (!kbuffer) {
                return -ENOMEM;
        }

        pport = pp->pdev->port;
        mode = pport->ieee1284.mode & ~(IEEE1284_DEVICEID | IEEE1284_ADDR);

        sunix_parport_set_timeout(pp->pdev, (file->f_flags & O_NONBLOCK) ? SNX_PARPORT_INACTIVITY_O_NONBLOCK : pp->default_inactivity);

        while (bytes_read == 0) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 9))
                ssize_t need = min_t(unsigned long, count, SNX_PP_BUFFER_SIZE);
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 8))
                ssize_t need = min(unsigned long, count, SNX_PP_BUFFER_SIZE);
#else
                ssize_t need = min(count - bytes_read, SNX_PP_BUFFER_SIZE);
#endif
                if (mode == IEEE1284_MODE_EPP) {
                        int flags = 0;
                        size_t (*fn)(struct snx_parport *, void *, size_t, int);

                        if (pp->flags & SNX_PP_W91284PIC) {
                                flags |= PARPORT_W91284PIC;
                        }

                        if (pp->flags & SNX_PP_FASTREAD) {
                                flags |= PARPORT_EPP_FAST;
                        }

                        if (pport->ieee1284.mode & IEEE1284_ADDR) {
                                fn = pport->ops->epp_read_addr;
                        } else {
                                fn = pport->ops->epp_read_data;
                        }

                        bytes_read = (*fn)(pport, kbuffer, need, flags);
                } else {
                        bytes_read = sunix_parport_read(pport, kbuffer, need);
                }

                if (bytes_read != 0) {
                        break;
                }

                if (file->f_flags & O_NONBLOCK) {
                        bytes_read = -EAGAIN;
                        break;
                }

                if (signal_pending(current)) {
                        bytes_read = -ERESTARTSYS;
                        break;
                }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 19))
                cond_resched();
#else
                if (current->need_resched) {
                        schedule();
                }
#endif
        }

        sunix_parport_set_timeout(pp->pdev, pp->default_inactivity);

        if (bytes_read > 0 && copy_to_user(buf, kbuffer, bytes_read)) {
                bytes_read = -EFAULT;
        }

        kfree(kbuffer);
        snx_pp_enable_irq(pp);

        return bytes_read;
}


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
static ssize_t snx_pp_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
#else
static ssize_t snx_pp_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 19))
        unsigned int minor = iminor(file->f_path.dentry->d_inode);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
        unsigned int minor = iminor(file->f_dentry->d_inode);
#else
        unsigned int minor = MINOR (file->f_dentry->d_inode->i_rdev);
#endif

        struct snx_pp_struct *pp = file->private_data;
        char *kbuffer;
        ssize_t bytes_written = 0;
        ssize_t wrote;
        int mode;
        struct snx_parport *pport;

        if (!(pp->flags & SNX_PP_CLAIMED)) {
                printk("snx_pp_write : %x claim the port first\n", minor);
                return -EINVAL;
        }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 9))
        kbuffer = kmalloc(min_t(size_t, count, SNX_PP_BUFFER_SIZE), GFP_KERNEL);
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 8))
        kbuffer = kmalloc(min(size_t, count, SNX_PP_BUFFER_SIZE), GFP_KERNEL);
#else
        kbuffer = kmalloc(min(count, SNX_PP_BUFFER_SIZE), GFP_KERNEL);
#endif

        if (!kbuffer) {
                return -ENOMEM;
        }

        pport = pp->pdev->port;
        mode = pport->ieee1284.mode & ~(IEEE1284_DEVICEID | IEEE1284_ADDR);

        sunix_parport_set_timeout(pp->pdev, (file->f_flags & O_NONBLOCK) ? SNX_PARPORT_INACTIVITY_O_NONBLOCK : pp->default_inactivity);

        while (bytes_written < count) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 9))
                ssize_t n = min_t(unsigned long, count - bytes_written, SNX_PP_BUFFER_SIZE);
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 8))
                ssize_t n = min(unsigned long, count - bytes_written, SNX_PP_BUFFER_SIZE);
#else
                ssize_t n = min(count - bytes_written, SNX_PP_BUFFER_SIZE);
#endif

                if (copy_from_user(kbuffer, buf + bytes_written, n)) {
                        bytes_written = -EFAULT;
                        break;
                }

                if ((pp->flags & SNX_PP_FASTWRITE) && (mode == IEEE1284_MODE_EPP)) {
                        if (pport->ieee1284.mode & IEEE1284_ADDR) {
                                wrote = pport->ops->epp_write_addr(pport, kbuffer, n, PARPORT_EPP_FAST);
                        } else {
                                wrote = pport->ops->epp_write_data(pport, kbuffer, n, PARPORT_EPP_FAST);
                        }
                } else {
                        wrote = sunix_parport_write(pp->pdev->port, kbuffer, n);
                }

                if (wrote <= 0) {
                        if (!bytes_written) {
                                bytes_written = wrote;
                        }
                        break;
                }

                bytes_written += wrote;

                if (file->f_flags & O_NONBLOCK) {
                        if (!bytes_written) {
                                bytes_written = -EAGAIN;
                        }
                        break;
                }

                if (signal_pending(current)) {
                        if (!bytes_written) {
                                bytes_written = -EINTR;
                        }
                        break;
                }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 19))
                cond_resched();
#else
                if (current->need_resched) {
                        schedule();
                }
#endif
        }

        sunix_parport_set_timeout(pp->pdev, pp->default_inactivity);

        kfree(kbuffer);
        snx_pp_enable_irq(pp);

        return bytes_written;
}

//#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static void snx_pp_irq(int irq, void *private, struct pt_regs *nouse)
{
        struct snx_pp_struct *pp = (struct snx_pp_struct *) private;

        if (pp->irqresponse) {
                sunix_parport_write_control(pp->pdev->port, pp->irqctl);
                pp->irqresponse = 0;
        }

        atomic_inc(&pp->irqc);
        wake_up_interruptible(&pp->irq_wait);
}
//#endif


//#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int snx_register_device(int minor, struct snx_pp_struct *pp)
{
        struct snx_parport *port;
        struct snx_pardevice *pdev = NULL;
        char *name;
        int fl;

        name = kmalloc(strlen(SNX_CHRDEV) + 3, GFP_KERNEL);
        if (name == NULL) {
                return -ENOMEM;
        }

        sprintf(name, SNX_CHRDEV "%x", minor);

        port = sunix_parport_find_number(minor);
        if (!port) {
                printk("SNX Error: %s no associated port!\n", name);
                kfree (name);
                return -ENXIO;
        }

        fl = (pp->flags & SNX_PP_EXCL) ? PARPORT_FLAG_EXCL : 0;

        pdev = sunix_parport_register_device(port, name, NULL, NULL, snx_pp_irq, fl, (struct snx_pp_struct *)pp);

        sunix_parport_put_port(port);

        if (!pdev) {
                printk("SNX Error: %s failed to register device!\n", name);
                kfree (name);
                return -ENXIO;
        }

        pp->pdev = pdev;
        return 0;
}
//#endif

static unsigned int get_minor_device(unsigned long arg)
{
        struct snx_par_port_info snx_port_info;
        memset(&snx_port_info, 0, (sizeof(struct snx_par_port_info)));
        if (copy_from_user(&snx_port_info, (void *)arg, (sizeof(struct snx_par_port_info))))
                return -EFAULT;
        return snx_port_info.minor;
}

static enum ieee1284_phase snx_init_phase(int mode)
{
        switch (mode & ~(IEEE1284_DEVICEID | IEEE1284_ADDR)) {
        case IEEE1284_MODE_NIBBLE:
        case IEEE1284_MODE_BYTE:
                return IEEE1284_PH_REV_IDLE;
        }

        return IEEE1284_PH_FWD_IDLE;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
static int snx_pp_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
        unsigned int minor = iminor(inode);
#else
        unsigned int minor = MINOR(inode->i_rdev);
#endif
        struct snx_pp_struct *pp = file->private_data;
        struct snx_parport *port;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
        void __user *argp = (void __user *)arg;
#endif
#else
static long snx_pp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
        struct snx_pp_struct *pp = file->private_data;
        struct snx_parport *port;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
        void __user *argp = (void __user *)arg;
#endif
        unsigned int minor;
        if (0 > (minor = get_minor_device(arg)))
                return minor;
#endif

        printk("snx_pp_ioctl 0x%04x minor %d\n", cmd, minor);
        switch (cmd) {
        case SNX_PAR_DUMP_PORT_INFO:
        {
                struct snx_par_port_info snx_port_info;
                struct sunix_par_port *sdn = NULL;

                memset(&snx_port_info, 0, (sizeof(struct snx_par_port_info)));

                minor = minor - 2;
                if (minor >= 0) {
                        sdn = (struct sunix_par_port *) &sunix_par_table[minor];

                        memcpy(&snx_port_info.board_name_info[0], &sdn->pb_info.board_name[0], SNX_BOARDNAME_LENGTH);

                        snx_port_info.bus_number_info = sdn->bus_number;
                        snx_port_info.dev_number_info = sdn->dev_number;
                        snx_port_info.port_info       = sdn->portnum + 2;
                        snx_port_info.base_info       = sdn->base;
                        snx_port_info.base_hi_info    = sdn->base_hi;
                        snx_port_info.irq_info        = sdn->irq;

                        if (copy_to_user((void *)arg, &snx_port_info, sizeof(struct snx_par_port_info))) {
                                return -EFAULT;
                        } else {
                                return 0;
                        }
                } else {
                        return -ENXIO;
                }
        }

        case SNX_PPCLAIM:
        {
                struct ieee1284_info *info;
                int ret;

                printk("SNX_PPCLAIM\n");

                if (pp->flags & SNX_PP_CLAIMED) {
                        printk("SNX_PPCLAIM: %x you've already got it!\n", minor);
                        return -EINVAL;
                }

                if (!pp->pdev) {
                        int err = snx_register_device(minor, pp);
                        printk("SNX_PPCLAIM: snx_register_device failed: %d\n", err);
                        if (err) {
                                return err;
                        }
                }

                ret = sunix_parport_claim_or_block(pp->pdev);
                if (ret < 0) {
                        printk("SNX_PPCLAIM: sunix_parport_claim_or_block failed: %d\n", ret);
                        return ret;
                }

                pp->flags |= SNX_PP_CLAIMED;

                snx_pp_enable_irq(pp);

                info = &pp->pdev->port->ieee1284;
                pp->saved_state.mode = info->mode;
                pp->saved_state.phase = info->phase;
                info->mode = pp->state.mode;
                info->phase = pp->state.phase;

                pp->default_inactivity = sunix_parport_set_timeout(pp->pdev, 0);

                sunix_parport_set_timeout(pp->pdev, pp->default_inactivity);
                return 0;
        }


        case SNX_PPEXCL:
        {
                if (pp->pdev) {
                        printk("snx_pp_ioctl: %x too late for SNX_PPEXCL; already registered\n", minor);
                        if (pp->flags & SNX_PP_EXCL) {
                                return 0;
                        }
                        return -EINVAL;
                }

                pp->flags |= SNX_PP_EXCL;
                return 0;
        }

        case SNX_PPSETMODE:
        {
                int mode;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&mode, argp, sizeof (mode)))
#else
                if (copy_from_user(&mode, (int *)arg, sizeof (mode)))
#endif
                {
                        return -EFAULT;
                }
                pp->state.mode = mode;
                pp->state.phase = snx_init_phase(mode);

                if (pp->flags & SNX_PP_CLAIMED) {
                        pp->pdev->port->ieee1284.mode = mode;
                        pp->pdev->port->ieee1284.phase = pp->state.phase;
                }

                return 0;
        }


        case SNX_PPGETMODE:
        {
                int mode;

                if (pp->flags & SNX_PP_CLAIMED) {
                        mode = pp->pdev->port->ieee1284.mode;
                } else {
                        mode = pp->state.mode;
                }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &mode, sizeof (mode)))
#else
                if (copy_to_user((int *)arg, &mode, sizeof (mode)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        case SNX_PPSETPHASE:
        {
                int phase;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&phase, argp, sizeof (phase)))
#else
                if (copy_from_user(&phase, (int *) arg, sizeof (phase)))
#endif
                {
                        return -EFAULT;
                }

                pp->state.phase = phase;

                if (pp->flags & SNX_PP_CLAIMED) {
                        pp->pdev->port->ieee1284.phase = phase;
                }
                return 0;
        }

        case SNX_PPGETPHASE:
        {
                int phase;

                if (pp->flags & SNX_PP_CLAIMED) {
                        phase = pp->pdev->port->ieee1284.phase;
                } else {
                        phase = pp->state.phase;
                }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &phase, sizeof (phase)))
#else
                if (copy_to_user((int *)arg, &phase, sizeof (phase)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        case SNX_PPGETMODES:
        {
                unsigned int modes;

                port = sunix_parport_find_number(minor);
                if (!port) {
                        return -ENODEV;
                }

                modes = port->modes;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &modes, sizeof (modes)))
#else
                if (copy_to_user((unsigned int *)arg, &modes, sizeof (port->modes)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        case SNX_PPSETFLAGS:
        {
                int uflags;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&uflags, argp, sizeof (uflags)))
#else
                if (copy_from_user(&uflags, (int *)arg, sizeof (uflags)))
#endif
                {
                        return -EFAULT;
                }
                pp->flags &= ~SNX_PP_FLAGMASK;
                pp->flags |= (uflags & SNX_PP_FLAGMASK);

                return 0;
        }


        case SNX_PPGETFLAGS:
        {
                int uflags;

                uflags = pp->flags & SNX_PP_FLAGMASK;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &uflags, sizeof (uflags)))
#else
                if (copy_to_user((int *)arg, &uflags, sizeof (uflags)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }
        }


        if ((pp->flags & SNX_PP_CLAIMED) == 0) {
                printk("snx_pp_ioctl: %x claim the port first\n", minor);
                return -EINVAL;
        }


        port = pp->pdev->port;
        switch (cmd) {
                struct ieee1284_info *info;
                unsigned char reg;
                unsigned char mask;
                int mode;
                int ret;
                struct __kernel_old_timeval par_timeout;
                long to_jiffies;

        case SNX_PPRSTATUS:
        {
                reg = sunix_parport_read_status(port);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &reg, sizeof (reg)))
#else
                if (copy_to_user((unsigned char *) arg, &reg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        case SNX_PPRDATA:
        {
                reg = sunix_parport_read_data(port);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &reg, sizeof (reg)))
#else
                if (copy_to_user((unsigned char *) arg, &reg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        case SNX_PPRCONTROL:
        {
                reg = sunix_parport_read_control(port);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &reg, sizeof (reg)))
#else
                if (copy_to_user((unsigned char *) arg, &reg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        case SNX_PPYIELD:
        {
                sunix_parport_yield_blocking(pp->pdev);
                return 0;
        }

        case SNX_PPRELEASE:
        {
                info = &pp->pdev->port->ieee1284;
                pp->state.mode = info->mode;
                pp->state.phase = info->phase;
                info->mode = pp->saved_state.mode;
                info->phase = pp->saved_state.phase;

                sunix_parport_release(pp->pdev);

                pp->flags &= ~SNX_PP_CLAIMED;

                return 0;
        }


        case SNX_PPWCONTROL:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&reg, argp, sizeof (reg)))
#else
                if (copy_from_user(&reg, (unsigned char *) arg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                sunix_parport_write_control(port, reg);

                return 0;
        }


        case SNX_PPWDATA:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&reg, argp, sizeof (reg)))
#else
                if (copy_from_user(&reg, (unsigned char *) arg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                sunix_parport_write_data(port, reg);
                return 0;
        }


        case SNX_PPFCONTROL:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&mask, argp, sizeof (mask)))
#else
                if (copy_from_user(&mask, (unsigned char *) arg, sizeof (mask)))
#endif
                {
                        return -EFAULT;
                }

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&reg, 1 + (unsigned char __user *) arg, sizeof (reg)))
#else
                if (copy_from_user(&reg, 1 + (unsigned char *) arg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                sunix_parport_frob_control(port, mask, reg);

                return 0;
        }


        case SNX_PPDATADIR:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&mode, argp, sizeof (mode)))
#else
                if (copy_from_user(&mode, (int *) arg, sizeof (mode)))
#endif
                {
                        return -EFAULT;
                }

                if (mode) {
                        port->ops->data_reverse(port);
                } else {
                        port->ops->data_forward(port);
                }

                return 0;
        }

        case SNX_PPNEGOT:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&mode, argp, sizeof (mode)))
#else
                if (copy_from_user(&mode, (int *) arg, sizeof (mode)))
#endif
                {
                        return -EFAULT;
                }

                switch ((ret = sunix_parport_negotiate(port, mode))) {
                case 0:
                        break;

                case -1:
                        ret = -EIO;
                        break;

                case 1:
                        ret = -ENXIO;
                        break;
                }

                snx_pp_enable_irq(pp);
                return ret;
        }


        case SNX_PPWCTLONIRQ:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&reg, argp, sizeof (reg)))
#else
                if (copy_from_user(&reg, (unsigned char *) arg, sizeof (reg)))
#endif
                {
                        return -EFAULT;
                }

                pp->irqctl = reg;
                pp->irqresponse = 1;
                return 0;
        }


        case SNX_PPCLRIRQ:
        {
                ret = atomic_read(&pp->irqc);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &ret, sizeof (ret)))
#else
                if (copy_to_user((int *) arg, &ret, sizeof (ret)))
#endif
                {
                        return -EFAULT;
                }

                atomic_sub(ret, &pp->irqc);
                return 0;
        }


        case SNX_PPSETTIME:
        {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_from_user(&par_timeout, argp, sizeof(struct __kernel_old_timeval)))
#else
                if (copy_from_user(&par_timeout, (struct __kernel_old_timeval *)arg, sizeof(struct __kernel_old_timeval)))
#endif
                {
                        return -EFAULT;
                }

                if ((par_timeout.tv_sec < 0) || (par_timeout.tv_usec < 0)) {
                        return -EINVAL;
                }

                to_jiffies = SNX_ROUND_UP(par_timeout.tv_usec, 1000000/HZ);
                to_jiffies += par_timeout.tv_sec * (long)HZ;

                if (to_jiffies <= 0) {
                        return -EINVAL;
                }

                pp->pdev->timeout = to_jiffies;
                return 0;
        }


        case SNX_PPGETTIME:
        {
                to_jiffies = pp->pdev->timeout;
                par_timeout.tv_sec = to_jiffies / HZ;
                par_timeout.tv_usec = (to_jiffies % (long)HZ) * (1000000/HZ);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 26))
                if (copy_to_user(argp, &par_timeout, sizeof(struct __kernel_old_timeval)))
#else
                if (copy_to_user((struct __kernel_old_timeval *)arg, &par_timeout, sizeof(struct __kernel_old_timeval)))
#endif
                {
                        return -EFAULT;
                }

                return 0;
        }


        default:
        {
                printk("SNX Error: %x What? (cmd=0x%x)\n", minor, cmd);
                return -EINVAL;
        }
        }

        return 0;
}

static int snx_pp_open(struct inode *inode, struct file *file)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
        unsigned int minor = iminor(inode);
#else
        unsigned int minor = MINOR(inode->i_rdev);
#endif

        struct snx_pp_struct *pp;

        if (minor >= PARPORT_MAX) {
                return -ENXIO;
        }

        pp = kmalloc(sizeof(struct snx_pp_struct), GFP_KERNEL);
        if (!pp) {
                return -ENOMEM;
        }

        pp->state.mode = IEEE1284_MODE_COMPAT;
        pp->state.phase = snx_init_phase(pp->state.mode);
        pp->flags = 0;
        pp->irqresponse = 0;
        atomic_set(&pp->irqc, 0);

        init_waitqueue_head(&pp->irq_wait);


        pp->pdev = NULL;
        file->private_data = pp;

        return 0;
}


static int snx_pp_release(struct inode *inode, struct file *file)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
        unsigned int minor = iminor(inode);
#else
        unsigned int minor = MINOR(inode->i_rdev);
#endif
        struct snx_pp_struct *pp = file->private_data;
        int compat_negot;

        compat_negot = 0;
        if (!(pp->flags & SNX_PP_CLAIMED) && pp->pdev && (pp->state.mode != IEEE1284_MODE_COMPAT)) {
                struct ieee1284_info *info;

                sunix_parport_claim_or_block(pp->pdev);

                pp->flags |= SNX_PP_CLAIMED;
                info = &pp->pdev->port->ieee1284;
                pp->saved_state.mode = info->mode;
                pp->saved_state.phase = info->phase;
                info->mode = pp->state.mode;
                info->phase = pp->state.phase;
                compat_negot = 1;
        } else if ((pp->flags & SNX_PP_CLAIMED) && pp->pdev && (pp->pdev->port->ieee1284.mode != IEEE1284_MODE_COMPAT)) {
                compat_negot = 2;
        }

        if (compat_negot) {
                sunix_parport_negotiate(pp->pdev->port, IEEE1284_MODE_COMPAT);
                printk("snx_pp_release: %x negotiated back to compatibility mode because user-space forgot\n", minor);
        }


        if (pp->flags & SNX_PP_CLAIMED) {
                struct ieee1284_info *info;

                info = &pp->pdev->port->ieee1284;
                pp->state.mode = info->mode;
                pp->state.phase = info->phase;
                info->mode = pp->saved_state.mode;
                info->phase = pp->saved_state.phase;

                sunix_parport_release(pp->pdev);

                if (compat_negot != 1) {
                        printk("snx_pp_release: %x released pardevice because user-space forgot\n", minor);
                }
        }


        if (pp->pdev) {
                const char *name = pp->pdev->name;
                sunix_parport_unregister_device(pp->pdev);
                kfree(name);
                pp->pdev = NULL;
        }

        kfree(pp);
        return 0;
}


static unsigned int snx_pp_poll(struct file *file, poll_table *wait)
{
        struct snx_pp_struct *pp = file->private_data;
        unsigned int mask = 0;

        poll_wait(file, &pp->irq_wait, wait);

        if (atomic_read(&pp->irqc)) {
                mask |= POLLIN | POLLRDNORM;
        }
        return mask;
}


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 9))
static struct file_operations snx_pp_fops = {
        .owner      = THIS_MODULE,
        .llseek     = no_llseek,
        .read       = snx_pp_read,
        .write      = snx_pp_write,
        .poll       = snx_pp_poll,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
        .ioctl    = snx_pp_ioctl,
#else
        .unlocked_ioctl   = snx_pp_ioctl,
#endif
        .open       = snx_pp_open,
        .release    = snx_pp_release,
};
#else
static loff_t snx_pp_lseek(struct file *file, long long offset, int origin)
{
        return -ESPIPE;
}


static struct file_operations snx_pp_fops = {
        .owner      = THIS_MODULE,
        .llseek     = snx_pp_lseek,
        .read       = snx_pp_read,
        .write      = snx_pp_write,
        .poll       = snx_pp_poll,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
        .ioctl    = snx_pp_ioctl,
#else
        .unlocked_ioctl   = snx_dump_par_ioctl,
#endif
        .open       = snx_pp_open,
        .release    = snx_pp_release,
};
#endif

/*
   static struct file_operations snx_pp_fops =
   {
    .owner    = THIS_MODULE,
   .llseek		= snx_pp_lseek,
   .read     = snx_pp_read,
   .write    = snx_pp_write,
   .poll     = snx_pp_poll,
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
   .ioctl    = snx_pp_ioctl,
 #else
   .unlocked_ioctl	= snx_pp_ioctl,
 #endif

 #ifdef CONFIG_COMPAT
   .compat_ioctl   = snx_pp_ioctl,
 #endif

   .open        = snx_pp_open,
   .release    = snx_pp_release
   };
 #endif
 */

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
static struct class *snx_ppdev_class;

static void snx_pp_attach(struct snx_parport *port)
{
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 26))
        device_create(snx_ppdev_class, NULL, MKDEV(SNX_PPD_MAJOR, port->number), "parport%d", port->number);
#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 27))
        device_create_drvdata(snx_ppdev_class, NULL, MKDEV(SNX_PPD_MAJOR, port->number), NULL, "parport%d", port->number);
#else
        device_create(snx_ppdev_class, NULL, MKDEV(SNX_PPD_MAJOR, port->number), NULL, "parport%d", port->number);
#endif
}

static void snx_pp_detach(struct snx_parport *port)
{
        device_destroy(snx_ppdev_class, MKDEV(SNX_PPD_MAJOR, port->number));
}

static struct snx_parport_driver snx_pp_driver = {
        .name       = SNX_CHRDEV,
        .attach     = snx_pp_attach,
        .detach     = snx_pp_detach,
};
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#else
static devfs_handle_t devfs_handle;
#endif


int sunix_par_ppdev_init(void)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17)))
        int i;
#endif
        int err = 0;

        SNX_PPD_MAJOR = register_chrdev(0, SNX_CHRDEV, &snx_pp_fops);

        if (SNX_PPD_MAJOR < 0) {
                printk("SNX Error: unable to get major \n");
                return -EIO;
        }

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
        devfs_handle = devfs_mk_dir(NULL, "parports", NULL);

        devfs_register_series(devfs_handle,
                              "%u",
                              SNX_PARPORT_MAX,
                              DEVFS_FL_DEFAULT,
                              SNX_PPD_MAJOR,
                              0,
                              S_IFCHR | S_IRUGO | S_IWUGO,
                              &snx_pp_fops,
                              NULL
                              );

#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
        for (i = 2; i < SNX_PARPORT_MAX; i++) {
                devfs_mk_cdev(MKDEV(SNX_PPD_MAJOR, i), S_IFCHR | S_IRUGO | S_IWUGO, "parports/%d", i);
        }
#else
        snx_ppdev_class = class_create(THIS_MODULE, SNX_CHRDEV);
        if (IS_ERR(snx_ppdev_class)) {
                err = PTR_ERR(snx_ppdev_class);
                goto out_chrdev;
        }


        if (sunix_parport_register_driver(&snx_pp_driver)) {
                printk("SNX Error: unable to register with parport\n\n");
                goto out_class;
        }

        goto out;

out_class:
        class_destroy(snx_ppdev_class);
out_chrdev:

        unregister_chrdev(SNX_PPD_MAJOR, SNX_CHRDEV);

out:
#endif

        return err;
}


void sunix_par_ppdev_exit(void)
{
#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17)))
        int i;
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
        devfs_unregister(devfs_handle);
        devfs_unregister_chrdev(SNX_PPD_MAJOR, SNX_CHRDEV);

#elif (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
        for (i = 2; i < SNX_PARPORT_MAX; i++) {
                devfs_remove("parports/%d", i);
        }
        devfs_remove("parports");
#else
        sunix_parport_unregister_driver(&snx_pp_driver);
        class_destroy(snx_ppdev_class);
#endif

        unregister_chrdev(SNX_PPD_MAJOR, SNX_CHRDEV);
}
