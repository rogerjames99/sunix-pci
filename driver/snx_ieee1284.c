#include "snx_common.h"


static void sunix_parport_ieee1284_wakeup(struct snx_parport *port)
{
	up(&port->physport->ieee1284.irq);
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0))
static struct snx_parport *port_from_cookie[SNX_PAR_TOTAL_MAX];
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))

static void sunix_timeout_waiting_on_port(struct timer_list *t)
{
	struct snx_parport *port = from_timer(port, t, timer);

	sunix_parport_ieee1284_wakeup(port);
}
#else

static void sunix_timeout_waiting_on_port(unsigned long cookie)
{
	sunix_parport_ieee1284_wakeup(port_from_cookie[cookie % SNX_PAR_TOTAL_MAX]);
}
#endif



int sunix_parport_wait_event(struct snx_parport *port, signed long timeout)
{
	int ret;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
#else
	struct timer_list timer;
#endif

	if (!port->physport->cad->timeout) {
		return 1;
	}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))

	timer_setup(&port->timer, sunix_timeout_waiting_on_port, 0);
	mod_timer(&port->timer, jiffies + timeout);
#else
	init_timer (&timer);
	timer.expires = jiffies + timeout;
	timer.function = sunix_timeout_waiting_on_port;
	port_from_cookie[port->number % PARPORT_MAX] = port;
	timer.data = port->number;

	add_timer (&timer);

#endif
	ret = down_interruptible(&port->physport->ieee1284.irq);


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
	if (!del_timer(&port->timer) && !ret)
#else
	if (!del_timer(&timer) && !ret)
#endif

	{
		ret = 1;
	}

	return ret;
}


int sunix_parport_poll_peripheral(struct snx_parport *port, unsigned char mask, unsigned char result, int usec)
{
	int count = usec / 5 + 2;
	int i;
	unsigned char status;

	for (i = 0; i < count; i++) {
		status = sunix_parport_read_status(port);

		if ((status & mask) == result) {
			return 0;
		}

		if (signal_pending(current)) {
			return -EINTR;
		}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 19))
		if (need_resched()) {
			break;
		}
#else
		if (current->need_resched) {
			break;
		}
#endif
		if (i >= 2) {
			udelay (5);
		}
	}

	return 1;
}


int sunix_parport_wait_peripheral(struct snx_parport *port, unsigned char mask, unsigned char result)
{
	int ret;
	int usec;
	unsigned long deadline;
	unsigned char status;

	usec = port->physport->spintime;// usecs of fast polling

	if (!port->physport->cad->timeout) {
		usec = 35000;
	}

	ret = sunix_parport_poll_peripheral(port, mask, result, usec);

	if (ret != 1) {
		return ret;
	}

	if (!port->physport->cad->timeout) {
		return 1;
	}

	deadline = jiffies + (HZ + 24) / 25;

	while (time_before(jiffies, deadline)) {
		int ret;

		if (signal_pending(current)) {
			return -EINTR;
		}

		//if ((ret = sunix_parport_wait_event(port, (HZ + 99) / 100)) < 0) {
		ret = sunix_parport_wait_event (port, (HZ + 99) / 100);
		if (ret < 0) {
			return ret;
		}

		status = sunix_parport_read_status(port);
		if ((status & mask) == result) {
			return 0;
		}

		if (!ret) {
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 13))
			__set_current_state (TASK_INTERRUPTIBLE);
			schedule_timeout ((HZ + 99) / 100);
#else
			schedule_timeout_interruptible(msecs_to_jiffies(10));
#endif
		}
	}

	return 1;
}


int sunix_parport_negotiate(struct snx_parport *port, int mode)
{
	if (mode == IEEE1284_MODE_COMPAT) {
		return 0;
	}

	return -1;
}


ssize_t sunix_parport_write(struct snx_parport *port, const void *buffer, size_t len)
{
	ssize_t ret;
	ret = port->ops->compat_write_data(port, buffer, len, 0);

	return ret;
}


ssize_t sunix_parport_read(struct snx_parport *port, void *buffer, size_t len)
{
	printk("SNX Warng: parport%d IEEE1284 not supported in this driver module.\n", port->portnum);
	return -ENODEV;
}


long sunix_parport_set_timeout(struct snx_pardevice *dev, long inactivity)
{
	long int old = dev->timeout;

	dev->timeout = inactivity;

	if (dev->port->physport->cad == dev) {
		sunix_parport_ieee1284_wakeup(dev->port);
	}

	return old;
}

