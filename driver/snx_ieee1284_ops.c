#include "snx_common.h"


size_t sunix_parport_ieee1284_write_compat(struct snx_parport *port, const void *buffer, size_t len, int flags)
{
	int no_irq = 1;
	ssize_t count = 0;
	const unsigned char *addr = buffer;
	unsigned char byte;
	struct snx_pardevice *dev = port->physport->cad;
	unsigned char ctl = (PARPORT_CONTROL_SELECT | PARPORT_CONTROL_INIT);

	if (port->irq != PARPORT_IRQ_NONE) {
		sunix_parport_enable_irq(port);
		no_irq = 0;
	}

	port->physport->ieee1284.phase = IEEE1284_PH_FWD_DATA;
	sunix_parport_write_control(port, ctl);
	sunix_parport_data_forward(port);

	while (count < len) {
		unsigned long expire = jiffies + dev->timeout;
		long wait = (HZ + 99) / 100;
		unsigned char mask = (PARPORT_STATUS_ERROR | PARPORT_STATUS_BUSY);
		unsigned char val = (PARPORT_STATUS_ERROR | PARPORT_STATUS_BUSY);

		do {
			if (!sunix_parport_wait_peripheral(port, mask, val)) {
				goto ready;
			}

			if ((sunix_parport_read_status(port) & (PARPORT_STATUS_PAPEROUT | PARPORT_STATUS_SELECT | PARPORT_STATUS_ERROR)) != (PARPORT_STATUS_SELECT | PARPORT_STATUS_ERROR)) {
				goto stop;
			}

			if (!time_before (jiffies, expire)) {
				break;
			}

			if (count && no_irq) {
				sunix_parport_release(dev);

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 13))
				__set_current_state (TASK_INTERRUPTIBLE);
				schedule_timeout (wait);
#else
				schedule_timeout_interruptible(wait);
#endif
				sunix_parport_claim_or_block(dev);
			} else {
				sunix_parport_wait_event(port, wait);
			}

			if (signal_pending (current)) {
				break;
			}

			wait *= 2;
		} while (time_before(jiffies, expire));

		if (signal_pending(current)) {
			break;
		}

		break;

ready:
		byte = *addr++;
		sunix_parport_write_data(port, byte);
		udelay (1);

		sunix_parport_write_control(port, ctl | PARPORT_CONTROL_STROBE);
		udelay (1);

		sunix_parport_write_control(port, ctl);
		udelay (1);

		count++;

		if (time_before(jiffies, expire)) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 19))
			if (!sunix_parport_yield_blocking(dev) && need_resched()) {
				schedule ();
			}
#else
			if (!sunix_parport_yield_blocking(dev) && current->need_resched) {
				schedule ();
			}
#endif
		}
	}
stop:
	port->physport->ieee1284.phase = IEEE1284_PH_FWD_IDLE;

	return count;
}


size_t sunix_parport_ieee1284_read_nibble(struct snx_parport *port, void *buffer, size_t len, int flags)
{
	return 0;
}


size_t sunix_parport_ieee1284_read_byte(struct snx_parport *port, void *buffer, size_t len, int flags)
{
	return 0;
}


size_t sunix_parport_ieee1284_ecp_write_data(struct snx_parport *port, const void *buffer, size_t len, int flags)
{
	return 0;
}


size_t sunix_parport_ieee1284_ecp_read_data(struct snx_parport *port, void *buffer, size_t len, int flags)
{
	return 0;
}


size_t sunix_parport_ieee1284_ecp_write_addr(struct snx_parport *port, const void *buffer, size_t len, int flags)
{
	return 0;
}


size_t sunix_parport_ieee1284_epp_write_data(struct snx_parport *port, const void *buffer, size_t len, int flags)
{
	unsigned char *bp = (unsigned char *) buffer;
	size_t ret = 0;

	sunix_parport_frob_control(port, PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD | PARPORT_CONTROL_SELECT | PARPORT_CONTROL_INIT,
								PARPORT_CONTROL_STROBE | PARPORT_CONTROL_INIT);

	port->ops->data_forward(port);

	for (; len > 0; len--, bp++) {
		sunix_parport_write_data(port, *bp);
		sunix_parport_frob_control(port, PARPORT_CONTROL_AUTOFD, PARPORT_CONTROL_AUTOFD);

		if (sunix_parport_poll_peripheral(port, PARPORT_STATUS_BUSY, 0, 10)) {
			break;
		}

		sunix_parport_frob_control(port, PARPORT_CONTROL_AUTOFD, 0);

		if (sunix_parport_poll_peripheral(port, PARPORT_STATUS_BUSY, PARPORT_STATUS_BUSY, 5)) {
			break;
		}

		ret++;
	}

	sunix_parport_frob_control(port, PARPORT_CONTROL_STROBE, 0);
	return ret;
}


size_t sunix_parport_ieee1284_epp_read_data(struct snx_parport *port, void *buffer, size_t len, int flags)
{
	unsigned char *bp = (unsigned char *) buffer;
	unsigned ret = 0;

	sunix_parport_frob_control(port, PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD | PARPORT_CONTROL_SELECT | PARPORT_CONTROL_INIT,
							PARPORT_CONTROL_INIT);

	port->ops->data_reverse(port);

	for (; len > 0; len--, bp++) {
		sunix_parport_frob_control(port, PARPORT_CONTROL_AUTOFD, PARPORT_CONTROL_AUTOFD);

		if (sunix_parport_wait_peripheral(port, PARPORT_STATUS_BUSY, 0)) {
			break;
		}

		*bp = sunix_parport_read_data(port);

		sunix_parport_frob_control(port, PARPORT_CONTROL_AUTOFD, 0);

		if (sunix_parport_poll_peripheral(port, PARPORT_STATUS_BUSY, PARPORT_STATUS_BUSY, 5)) {
			break;
		}

		ret++;
	}
	port->ops->data_forward(port);
	return ret;
}


size_t sunix_parport_ieee1284_epp_write_addr(struct snx_parport *port, const void *buffer, size_t len, int flags)
{
	unsigned char *bp = (unsigned char *)buffer;
	size_t ret = 0;

	sunix_parport_frob_control(port, PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD | PARPORT_CONTROL_SELECT | PARPORT_CONTROL_INIT,
								PARPORT_CONTROL_STROBE | PARPORT_CONTROL_INIT);

	port->ops->data_forward(port);

	for (; len > 0; len--, bp++) {
		sunix_parport_write_data(port, *bp);
		sunix_parport_frob_control(port, PARPORT_CONTROL_SELECT, PARPORT_CONTROL_SELECT);

		if (sunix_parport_poll_peripheral(port, PARPORT_STATUS_BUSY, 0, 10)) {
			break;
		}

		sunix_parport_frob_control(port, PARPORT_CONTROL_SELECT, 0);

		if (sunix_parport_poll_peripheral(port, PARPORT_STATUS_BUSY, PARPORT_STATUS_BUSY, 5)) {
			break;
		}

		ret++;
	}

	sunix_parport_frob_control(port, PARPORT_CONTROL_STROBE, 0);
	return ret;
}


size_t sunix_parport_ieee1284_epp_read_addr(struct snx_parport *port, void *buffer, size_t len, int flags)
{
	unsigned char *bp = (unsigned char *) buffer;
	unsigned ret = 0;

	sunix_parport_frob_control(port, PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD | PARPORT_CONTROL_SELECT | PARPORT_CONTROL_INIT,
								PARPORT_CONTROL_INIT);

	port->ops->data_reverse(port);

	for (; len > 0; len--, bp++) {
		sunix_parport_frob_control(port, PARPORT_CONTROL_SELECT, PARPORT_CONTROL_SELECT);

		if (sunix_parport_wait_peripheral(port, PARPORT_STATUS_BUSY, 0)) {
			break;
		}

		*bp = sunix_parport_read_data(port);

		sunix_parport_frob_control(port, PARPORT_CONTROL_SELECT, PARPORT_CONTROL_SELECT);

		if (sunix_parport_poll_peripheral(port, PARPORT_STATUS_BUSY, PARPORT_STATUS_BUSY, 5)) {
			break;
		}

		ret++;
	}

	port->ops->data_forward(port);
	return ret;
}

