#include "snx_common.h"


#define SNX_PARPORT_DEFAULT_TIMESLICE	(HZ/5)


unsigned long sunix_parport_default_timeslice = SNX_PARPORT_DEFAULT_TIMESLICE;
int sunix_parport_default_spintime = DEFAULT_SPIN_TIME;


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
static LIST_HEAD(snx_portlist);
static DEFINE_SPINLOCK(snx_full_list_lock);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
static LIST_HEAD(snx_portlist);
static spinlock_t snx_full_list_lock = SPIN_LOCK_UNLOCKED;
#else
//static struct snx_parport *snx_portlist = NULL;
//static struct snx_parport *snx_portlist_tail = NULL;
static struct snx_parport *snx_portlist;
static struct snx_parport *snx_portlist_tail;
static spinlock_t snx_driverlist_lock = SPIN_LOCK_UNLOCKED;
//static struct snx_parport_driver *snx_driver_chain = NULL;
static struct snx_parport_driver *snx_driver_chain;
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
static DEFINE_SPINLOCK(snx_parportlist_lock);
#else
static spinlock_t snx_parportlist_lock = SPIN_LOCK_UNLOCKED;
#endif
static LIST_HEAD(snx_all_ports);
static LIST_HEAD(snx_drivers);


#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)

static DECLARE_MUTEX(snx_registration_lock);

#else

static DEFINE_SEMAPHORE(snx_registration_lock);

#endif


static void sunix_dead_write_lines(struct snx_parport *p, unsigned char b) {}
static unsigned char sunix_dead_read_lines(struct snx_parport *p) { return 0; }
static unsigned char sunix_dead_frob_lines(struct snx_parport *p, unsigned char b,  unsigned char c) {return 0; }
static void sunix_dead_onearg(struct snx_parport *p) {}
static void sunix_dead_initstate(struct snx_pardevice *d, struct snx_parport_state *s) {}
static void sunix_dead_state(struct snx_parport *p, struct snx_parport_state *s) {}
static size_t sunix_dead_write(struct snx_parport *p, const void *b, size_t l, int f) { return 0; }
static size_t sunix_dead_read(struct snx_parport *p, void *b, size_t l, int f) { return 0; }


static struct snx_parport_ops 	sunix_dead_ops = {
	.write_data			= sunix_dead_write_lines,
	.read_data			= sunix_dead_read_lines,
	.write_control		= sunix_dead_write_lines,
	.read_control		= sunix_dead_read_lines,
	.frob_control		= sunix_dead_frob_lines,
	.read_status		= sunix_dead_read_lines,
	.enable_irq			= sunix_dead_onearg,
	.disable_irq		= sunix_dead_onearg,
	.data_forward		= sunix_dead_onearg,
	.data_reverse		= sunix_dead_onearg,
	.init_state			= sunix_dead_initstate,
	.save_state			= sunix_dead_state,
	.restore_state		= sunix_dead_state,
	.epp_write_data		= sunix_dead_write,
	.epp_read_data		= sunix_dead_read,
	.epp_write_addr		= sunix_dead_write,
	.epp_read_addr		= sunix_dead_read,
	.ecp_write_data		= sunix_dead_write,
	.ecp_read_data		= sunix_dead_read,
	.ecp_write_addr		= sunix_dead_write,
	.compat_write_data	= sunix_dead_write,
	.nibble_read_data	= sunix_dead_read,
	.byte_read_data		= sunix_dead_read,
	.owner				= NULL,
};


static void sunix_attach_driver_chain(struct snx_parport *port)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	struct snx_parport_driver *drv;
	list_for_each_entry(drv, &snx_drivers, list) drv->attach(port);
#else
	struct snx_parport_driver *drv;
	void (**attach) (struct snx_parport *);
	int count = 0;
    int i;

	spin_lock(&snx_driverlist_lock);
	for (drv = snx_driver_chain; drv; drv = drv->next) {
		count++;
	}

	spin_unlock(&snx_driverlist_lock);

	attach = kmalloc(sizeof (void(*)(struct snx_parport *)) * count, GFP_KERNEL);

	if (!attach) {
		printk("SNX Warng: not enough memory to attach\n");
		return;
	}

	spin_lock(&snx_driverlist_lock);

	for (i = 0, drv = snx_driver_chain; drv && i < count; drv = drv->next) {
		attach[i++] = drv->attach;
	}

	spin_unlock(&snx_driverlist_lock);

	for (count = 0; count < i; count++) {
		(*attach[count])(port);
	}

	kfree (attach);
#endif
}


static void sunix_detach_driver_chain(struct snx_parport *port)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	struct snx_parport_driver *drv;
	list_for_each_entry(drv, &snx_drivers, list) drv->detach (port);
#else
	struct snx_parport_driver *drv;

	spin_lock(&snx_driverlist_lock);
	for (drv = snx_driver_chain; drv; drv = drv->next) {
		drv->detach(port);
	}

	spin_unlock(&snx_driverlist_lock);
#endif
}


int sunix_parport_register_driver(struct snx_parport_driver *drv)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	struct snx_parport *port;

	down(&snx_registration_lock);

	list_for_each_entry(port, &snx_portlist, list) drv->attach(port);
	list_add(&drv->list, &snx_drivers);

	up(&snx_registration_lock);

	return 0;
#else
	struct snx_parport *port;
	struct snx_parport **ports;
	int count = 0;
	int i;

	spin_lock(&snx_parportlist_lock);

	for (port = snx_portlist; port; port = port->next) {
		count++;
	}

	spin_unlock(&snx_parportlist_lock);

	ports = kmalloc(sizeof(struct snx_parport *) * count, GFP_KERNEL);

	if (!ports) {
		printk("SNX Warng: not enough memory to attach\n");
	} else {
		spin_lock(&snx_parportlist_lock);

		for (i = 0, port = snx_portlist; port && i < count; port = port->next) {
			ports[i++] = port;
		}

		spin_unlock(&snx_parportlist_lock);

		for (count = 0; count < i; count++) {
			drv->attach(ports[count]);
		}

		kfree(ports);
	}

	spin_lock(&snx_driverlist_lock);

	drv->next = snx_driver_chain;
	snx_driver_chain = drv;

	spin_unlock(&snx_driverlist_lock);

	return 0;
#endif
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
void sunix_parport_unregister_driver(struct snx_parport_driver *drv)
{
	struct snx_parport *port;

	down(&snx_registration_lock);

	list_del_init(&drv->list);
	list_for_each_entry(port, &snx_portlist, list) drv->detach(port);

	up(&snx_registration_lock);
}
#else
void sunix_parport_unregister_driver(struct snx_parport_driver *arg)
{
	struct snx_parport_driver *drv = snx_driver_chain;
    struct snx_parport_driver *olddrv = NULL;

	while (drv) {
		if (drv == arg) {
			struct snx_parport *port;

			spin_lock(&snx_driverlist_lock);
			if (olddrv) {
				olddrv->next = drv->next;
			} else {
				snx_driver_chain = drv->next;
			}

			spin_unlock(&snx_driverlist_lock);

			spin_lock(&snx_parportlist_lock);

			for (port = snx_portlist; port; port = port->next) {
				drv->detach(port);
			}

			spin_unlock(&snx_parportlist_lock);

			return;
		}
		olddrv = drv;
		drv = drv->next;
	}
}
#endif


static void sunix_free_port(struct snx_parport *port)
{
	int d;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	spin_lock(&snx_full_list_lock);
	list_del(&port->full_list);
	spin_unlock(&snx_full_list_lock);

	for (d = 0; d < 5; d++) {
		kfree(port->probe_info[d].class_name);
		kfree(port->probe_info[d].mfr);
		kfree(port->probe_info[d].model);
		kfree(port->probe_info[d].cmdset);
		kfree(port->probe_info[d].description);
	}
#else

	for (d = 0; d < 5; d++) {
		if (port->probe_info[d].class_name) {
			kfree (port->probe_info[d].class_name);
		}

		if (port->probe_info[d].mfr) {
			kfree (port->probe_info[d].mfr);
		}

		if (port->probe_info[d].model) {
			kfree (port->probe_info[d].model);
		}

		if (port->probe_info[d].cmdset) {
			kfree (port->probe_info[d].cmdset);
		}

		if (port->probe_info[d].description) {
			kfree (port->probe_info[d].description);
		}
	}
#endif

	kfree(port->name);
	kfree(port);
}


struct snx_parport *sunix_parport_get_port(struct snx_parport *port)
{
	atomic_inc(&port->ref_count);
	return port;
}


void sunix_parport_put_port(struct snx_parport *port)
{
	if (atomic_dec_and_test(&port->ref_count)) {
		sunix_free_port(port);
	}
	return;
}


struct snx_parport *sunix_parport_register_port(struct sunix_par_port *priv,  struct snx_parport_ops *ops)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	struct list_head *l = NULL;
#endif
	struct snx_parport *tmp = NULL;
	int num;
	int device;
	char *name;

	if ((!priv) || (!ops)) {
		return NULL;
	}

	tmp = kmalloc(sizeof(struct snx_parport), GFP_KERNEL);

	if (!tmp) {
		return NULL;
	}


	memset(tmp, 0, sizeof(struct snx_parport));


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	tmp->base = priv->base;
	tmp->irq = priv->irq;

	tmp->base_hi = priv->base_hi;

	tmp->muxport = tmp->daisy = tmp->muxsel = -1;
	tmp->modes = 0;

	INIT_LIST_HEAD(&tmp->list);

	tmp->devices = tmp->cad = NULL;
	tmp->flags = 0;
	tmp->ops = ops;
	tmp->physport = tmp;

	memset(tmp->probe_info, 0, 5 * sizeof (struct snx_parport_device_info));

	rwlock_init(&tmp->cad_lock);
	spin_lock_init(&tmp->waitlist_lock);
	spin_lock_init(&tmp->pardevice_lock);
	tmp->ieee1284.mode = IEEE1284_MODE_COMPAT;
	tmp->ieee1284.phase = IEEE1284_PH_FWD_IDLE;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
			sema_init(&tmp->ieee1284.irq, 0);
#else
			init_MUTEX_LOCKED(&tmp->ieee1284.irq);
#endif


	tmp->spintime = sunix_parport_default_spintime;

	atomic_set(&tmp->ref_count, 1);

	INIT_LIST_HEAD(&tmp->full_list);

	name = kmalloc(20, GFP_KERNEL);
	if (!name) {
		return NULL;
	}

	spin_lock(&snx_full_list_lock);

	for (l = snx_all_ports.next, num = 2; l != &snx_all_ports; l = l->next, num++) {
		struct snx_parport *p = list_entry(l, struct snx_parport, full_list);

		if (p->number != num) {
			break;
		}
	}

	tmp->portnum = tmp->number = num;

	list_add_tail(&tmp->full_list, l);

	spin_unlock(&snx_full_list_lock);


	sprintf(name, "parport%d", tmp->portnum = tmp->number);
	tmp->name = name;

	for (device = 0; device < 5; device++) {
		tmp->probe_info[device].class = PARPORT_CLASS_LEGACY;
	}

	tmp->waithead = tmp->waittail = NULL;
#else

	spin_lock_irq(&snx_parportlist_lock);
	for (num = 2; ; num++) {
		struct snx_parport *itr = snx_portlist;
		while (itr) {
			if (itr->number == num) {
				break;
			} else {
				itr = itr->next;
			}
		}

		if (itr == NULL) {
			break;
		}
	}

	spin_unlock_irq(&snx_parportlist_lock);

	tmp->base = priv->base;
	tmp->irq = priv->irq;
	tmp->base_hi = priv->base_hi;

	tmp->muxport = tmp->daisy = tmp->muxsel = -1;
	tmp->modes = 0;
	tmp->next = NULL;
	tmp->devices = tmp->cad = NULL;
	tmp->flags = 0;
	tmp->ops = ops;
	tmp->portnum = tmp->number = num;
	tmp->physport = tmp;

	memset (tmp->probe_info, 0, 5 * sizeof (struct snx_parport_device_info));

	tmp->cad_lock = RW_LOCK_UNLOCKED;
	spin_lock_init(&tmp->waitlist_lock);
	spin_lock_init(&tmp->pardevice_lock);
	tmp->ieee1284.mode = IEEE1284_MODE_COMPAT;
	tmp->ieee1284.phase = IEEE1284_PH_FWD_IDLE;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
			sema_init(&tmp->ieee1284.irq, 0);
#else
			init_MUTEX_LOCKED (&tmp->ieee1284.irq);
#endif

	tmp->spintime = sunix_parport_default_spintime;

	atomic_set(&tmp->ref_count, 1);

	name = kmalloc(20, GFP_KERNEL);
	if (!name) {
		kfree(tmp);
		return NULL;
	}

	sprintf(name, "parport%d", num);
	tmp->name = name;

	spin_lock(&snx_parportlist_lock);

	if (snx_portlist_tail) {
		snx_portlist_tail->next = tmp;
    }

	snx_portlist_tail = tmp;

	if (!snx_portlist) {
		snx_portlist = tmp;
    }
	spin_unlock(&snx_parportlist_lock);

	for (device = 0; device < 5; device++) {
		tmp->probe_info[device].class = PARPORT_CLASS_LEGACY;
	}

	tmp->waithead = tmp->waittail = NULL;
#endif

	return tmp;
}


void sunix_parport_announce_port(struct snx_parport *port)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	int i;

	down(&snx_registration_lock);

	spin_lock_irq(&snx_parportlist_lock);

	list_add_tail(&port->list, &snx_portlist);

	for (i = 1; i < 3; i++) {
		struct snx_parport *slave = port->slaves[i-1];

		if (slave) {
			list_add_tail(&slave->list, &snx_portlist);
		}
	}
	spin_unlock_irq(&snx_parportlist_lock);

	sunix_attach_driver_chain(port);

	for (i = 1; i < 3; i++) {
		struct snx_parport *slave = port->slaves[i-1];

		if (slave) {
			sunix_attach_driver_chain(slave);
		}
	}

	up(&snx_registration_lock);
#else

   sunix_attach_driver_chain(port);
#endif
}


void sunix_parport_remove_port(struct snx_parport *port)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	int i;

	down(&snx_registration_lock);

	sunix_detach_driver_chain(port);

	port->ops = &sunix_dead_ops;
	spin_lock(&snx_parportlist_lock);
	list_del_init(&port->list);

	for (i = 1; i < 3; i++) {
		struct snx_parport *slave = port->slaves[i-1];
		if (slave) {
			list_del_init(&slave->list);
		}
	}

	spin_unlock(&snx_parportlist_lock);

	up(&snx_registration_lock);

	for (i = 1; i < 3; i++) {
		struct snx_parport *slave = port->slaves[i-1];
		if (slave) {
			sunix_parport_put_port(slave);
		}
	}

#else

	struct snx_parport *p;

	port->ops = &sunix_dead_ops;

	sunix_detach_driver_chain(port);

	spin_lock(&snx_parportlist_lock);

	if (snx_portlist == port) {
		//if ((snx_portlist = port->next) == NULL) {
		snx_portlist = port->next;
		if (snx_portlist == NULL) {
			snx_portlist_tail = NULL;
		}
	} else {
		for (p = snx_portlist; (p != NULL) && (p->next != port); p = p->next)
		;
			if (p) {
				//if ((p->next = port->next) == NULL) {
					p->next = port->next;
					if (p->next == NULL) {
						snx_portlist_tail = p;
				}
			} else {
				printk("SNX Warng: %s not found in port list!\n", port->name);
			}
		}
	spin_unlock(&snx_parportlist_lock);

	sunix_parport_put_port(port);
#endif
}


struct snx_pardevice *sunix_parport_register_device(
	struct snx_parport 	*port,
	const char 			*name,
	int 				(*pf)(void *),
	void 				(*kf)(void *),
	void 				(*irq_func)(int, void *, struct pt_regs *),
	int 				flags,
	void 				*handle
	)
{
	struct snx_pardevice *tmp;

	if (port->physport->flags & PARPORT_FLAG_EXCL) {
		printk("SNX Warng: %s no more devices allowed\n",	port->name);
		return NULL;
	}

	if (flags & PARPORT_DEV_LURK) {
		if (!pf || !kf) {
			printk("SNX Error: %s refused to register lurking device (%s) without callbacks\n", port->name, name);
			return NULL;
		}
	}

	sunix_parport_get_port(port);

	tmp = kmalloc(sizeof(struct snx_pardevice), GFP_KERNEL);
	if (tmp == NULL) {
		printk("SNX Error: %s memory squeeze, couldn't register %s.\n", port->name, name);
		goto out;
	}

	tmp->state = kmalloc(sizeof(struct snx_parport_state), GFP_KERNEL);
	if (tmp->state == NULL) {
		printk("SNX Error: %s memory squeeze, couldn't register %s.\n", port->name, name);
		goto out_free_pardevice;
	}

	tmp->name = name;
	tmp->port = port;
	tmp->daisy = -1;
	tmp->preempt = pf;
	tmp->wakeup = kf;
	tmp->private = handle;
	tmp->flags = flags;
	tmp->irq_func = irq_func;
	tmp->waiting = 0;
	tmp->timeout = 5 * HZ;

	tmp->prev = NULL;

	spin_lock(&port->physport->pardevice_lock);

	if (flags & PARPORT_DEV_EXCL) {
		if (port->physport->devices) {
			spin_unlock(&port->physport->pardevice_lock);
			printk("SNX Error: %s cannot grant exclusive access for device %s\n", port->name, name);
			goto out_free_all;
		}
		port->flags |= PARPORT_FLAG_EXCL;
	}

	tmp->next = port->physport->devices;
	wmb();

	if (port->physport->devices) {
		port->physport->devices->prev = tmp;
	}

	port->physport->devices = tmp;
	spin_unlock(&port->physport->pardevice_lock);

	init_waitqueue_head(&tmp->wait_q);
	tmp->timeslice = sunix_parport_default_timeslice;
	tmp->waitnext = tmp->waitprev = NULL;

	port->ops->init_state(tmp, tmp->state);
	return tmp;

out_free_all:
	kfree(tmp->state);

out_free_pardevice:
	kfree(tmp);

out:
	sunix_parport_put_port(port);

	return NULL;
}


void sunix_parport_unregister_device(struct snx_pardevice *dev)
{
	struct snx_parport *port;

	if (dev == NULL) {
		return;
	}

	port = dev->port->physport;

	if (port->cad == dev) {
		printk("SNX Warng: %s, %s forgot to release port\n", port->name, dev->name);
		sunix_parport_release(dev);
	}

	spin_lock(&port->pardevice_lock);
	if (dev->next) {
		dev->next->prev = dev->prev;
	}

	if (dev->prev) {
		dev->prev->next = dev->next;
	} else {
		port->devices = dev->next;
	}

	if (dev->flags & PARPORT_DEV_EXCL) {
		port->flags &= ~PARPORT_FLAG_EXCL;
	}

	spin_unlock(&port->pardevice_lock);

	spin_lock (&port->waitlist_lock);
	if (dev->waitprev || dev->waitnext || port->waithead == dev) {
		if (dev->waitprev) {
			dev->waitprev->waitnext = dev->waitnext;
		} else {
			port->waithead = dev->waitnext;
		}


		if (dev->waitnext) {
			dev->waitnext->waitprev = dev->waitprev;
		} else {
			port->waittail = dev->waitprev;
		}
	}

	spin_unlock(&port->waitlist_lock);

	kfree(dev->state);
	kfree(dev);

	sunix_parport_put_port(port);
}


struct snx_parport *sunix_parport_find_number(int number)
{
	struct snx_parport *port, *result = NULL;

	spin_lock(&snx_parportlist_lock);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	list_for_each_entry(port, &snx_portlist, list) {
		if (port->number == number) {
			result = sunix_parport_get_port(port);
			break;
		}
	}
#else
	for (port = snx_portlist; port; port = port->next) {
		if (port->number == number) {
			result = sunix_parport_get_port(port);
			break;
		}
    }

#endif
	spin_unlock(&snx_parportlist_lock);
	return result;
}


struct snx_parport *sunix_parport_find_base(unsigned long base)
{
	struct snx_parport *port, *result = NULL;

	spin_lock(&snx_parportlist_lock);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	list_for_each_entry(port, &snx_portlist, list) {
		if (port->base == base) {
			result = sunix_parport_get_port(port);
			break;
		}
	}
#else
	for (port = snx_portlist; port; port = port->next) {
		if (port->base == base) {
			result = sunix_parport_get_port(port);
			break;
		}
    }
#endif
	spin_unlock(&snx_parportlist_lock);

	return result;
}


int sunix_parport_claim(struct snx_pardevice *dev)
{
	struct snx_pardevice *oldcad;
	struct snx_parport *port = dev->port->physport;
	unsigned long flags;

	if (port->cad == dev) {
		printk("SNX Info : %s, %s already owner\n", dev->port->name, dev->name);
		return 0;
	}

	write_lock_irqsave(&port->cad_lock, flags);
	//if ((oldcad = port->cad) != NULL) {
	oldcad = port->cad;
	if (oldcad != NULL) {
		if (oldcad->preempt) {
			if (oldcad->preempt(oldcad->private)) {
				goto blocked;
			}

			port->ops->save_state(port, dev->state);
		} else {
			goto blocked;
		}

		if (port->cad != oldcad) {
			printk("SNX Error: %s, %s released port when preempted!\n", port->name, oldcad->name);
			if (port->cad) {
				goto blocked;
			}
		}
	}

	if (dev->waiting & 1) {
		dev->waiting = 0;

		spin_lock_irq(&port->waitlist_lock);

		if (dev->waitprev) {
			dev->waitprev->waitnext = dev->waitnext;
		} else {
			port->waithead = dev->waitnext;
		}


		if (dev->waitnext) {
			dev->waitnext->waitprev = dev->waitprev;
		} else {
			port->waittail = dev->waitprev;
		}

		spin_unlock_irq(&port->waitlist_lock);
		dev->waitprev = dev->waitnext = NULL;
	}

	port->cad = dev;

	port->ops->restore_state(port, dev->state);
	write_unlock_irqrestore(&port->cad_lock, flags);
	dev->time = jiffies;

	return 0;

blocked:
	if (dev->waiting & 2 || dev->wakeup) {
		spin_lock (&port->waitlist_lock);
		if (test_and_set_bit(0, &dev->waiting) == 0) {
			dev->waitnext = NULL;
			dev->waitprev = port->waittail;
			if (port->waittail) {
				port->waittail->waitnext = dev;
				port->waittail = dev;
			} else {
				port->waithead = port->waittail = dev;
			}
		}

		spin_unlock(&port->waitlist_lock);
	}

	write_unlock_irqrestore(&port->cad_lock, flags);
	return -EAGAIN;
}


int sunix_parport_claim_or_block(struct snx_pardevice *dev)
{
	int r;
	dev->waiting = 2;

	r = sunix_parport_claim(dev);

	if (r == -EAGAIN) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
		unsigned long flags;
#endif
		printk("SNX Warng: %s sunix_parport_claim() returned -EAGAIN\n", dev->name);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
		save_flags (flags);
		cli();
#endif
		if (dev->waiting) {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 15, 0))
					wait_event_interruptible(dev->wait_q, !dev->waiting);
#else
					interruptible_sleep_on(&dev->wait_q);
#endif

			if (signal_pending(current)) {
				return -EINTR;
			}
			r = 1;
		} else {
			r = 0;
			printk("SNX Warng: %s, didn't sleep in sunix_parport_claim_or_block()\n", dev->name);
		}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
		restore_flags(flags);
#endif
		if (dev->port->physport->cad != dev) {
			printk("SNX Warng: %s, exiting sunix_parport_claim_or_block but %s owns port!\n",
					dev->name,
					dev->port->physport->cad ? dev->port->physport->cad->name:"nobody"
					);
		}
	}
	dev->waiting = 0;

	return r;
}


void sunix_parport_release(struct snx_pardevice *dev)
{
	struct snx_parport *port = dev->port->physport;
	struct snx_pardevice *pd;
	unsigned long flags;

	write_lock_irqsave(&port->cad_lock, flags);
	if (port->cad != dev) {
		write_unlock_irqrestore(&port->cad_lock, flags);
		printk("SNX Warng: %s, %s tried to release parport when not owner\n", port->name, dev->name);
		return;
	}

	port->cad = NULL;
	write_unlock_irqrestore(&port->cad_lock, flags);

	port->ops->save_state(port, dev->state);

	for (pd = port->waithead; pd; pd = pd->waitnext) {
		if (pd->waiting & 2) {
			sunix_parport_claim(pd);
			if (waitqueue_active(&pd->wait_q)) {
				wake_up_interruptible(&pd->wait_q);
			}

			return;
		} else if (pd->wakeup) {
			pd->wakeup(pd->private);
			if (dev->port->cad) {
				return;
			}
		} else {
			printk("SNX Warng: %s don't know how to wake %s\n", port->name, pd->name);
		}
	}

	for (pd = port->devices; (port->cad == NULL) && pd; pd = pd->next) {
		if (pd->wakeup && pd != dev) {
			pd->wakeup(pd->private);
		}
	}
}

