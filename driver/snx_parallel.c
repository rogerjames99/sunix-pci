
#include "snx_common.h"

static LIST_HEAD(snx_ports_list);


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
static DEFINE_SPINLOCK(snx_ports_lock);
#else
static spinlock_t snx_ports_lock = SPIN_LOCK_UNLOCKED;
#endif
static void sunix_frob_econtrol(struct snx_parport *, unsigned char, unsigned char);

// ECR modes
#define ECR_SPP 00
#define ECR_PS2 01
#define ECR_PPF 02
#define ECR_ECP 03
#define ECR_EPP 04
#define ECR_VND 05
#define ECR_TST 06
#define ECR_CNF 07
#define ECR_MODE_MASK 0xe0
#define ECR_WRITE(p, v) sunix_frob_econtrol((p), 0xff, (v))


static void sunix_frob_econtrol(struct snx_parport *pb, unsigned char m, unsigned char v)
{
	unsigned char ectr = 0;

	if (m != 0xff) {
		ectr = inb(SNX_ECR (pb));
	}

	outb((ectr & ~m) ^ v, SNX_ECR (pb));
}


static __inline__ void sunix_frob_set_mode(struct snx_parport *p, int mode)
{
	sunix_frob_econtrol(p, ECR_MODE_MASK, mode << 5);
}


static int sunix_clear_epp_timeout(struct snx_parport *pb)
{
	unsigned char dsr;
	dsr = sunix_parport_pc_read_status(pb);

	if (!(dsr & 0x01)) {
		return 1;
	}

	sunix_parport_pc_read_status(pb);
	dsr = sunix_parport_pc_read_status(pb);

	outb(dsr | 0x01, SNX_DSR (pb));
	outb(dsr & 0xfe, SNX_DSR (pb));

	dsr = sunix_parport_pc_read_status(pb);

	return !(dsr & 0x01);
}


static void sunix_parport_pc_init_state(struct snx_pardevice *dev, struct snx_parport_state *s)
{
	s->u.pc.ctr = 0xc;
	if (dev->irq_func && dev->port->irq != PARPORT_IRQ_NONE) {
		s->u.pc.ctr |= 0x10;
	}

	s->u.pc.ecr = 0x34;
}


static void sunix_parport_pc_save_state(struct snx_parport *p, struct snx_parport_state *s)
{
	const struct sunix_par_port *priv = p->physport->private_data;

	s->u.pc.ctr = priv->ctr;

	if (priv->ecr) {
		s->u.pc.ecr = inb(SNX_ECR (p));
	}
}


static void sunix_parport_pc_restore_state(struct snx_parport *p, struct snx_parport_state *s)
{
	struct sunix_par_port *priv = p->physport->private_data;
	register unsigned char c = s->u.pc.ctr & priv->ctr_writable;

	outb(c, SNX_DCR (p));
	priv->ctr = c;

	if (priv->ecr) {
		ECR_WRITE(p, s->u.pc.ecr);
	}
}


static const struct snx_parport_ops sunix_parport_pc_ops = {
	.write_data			= sunix_parport_pc_write_data,
	.read_data			= sunix_parport_pc_read_data,
	.write_control		= sunix_parport_pc_write_control,
	.read_control		= sunix_parport_pc_read_control,
	.frob_control		= sunix_parport_pc_frob_control,
	.read_status		= sunix_parport_pc_read_status,
	.enable_irq			= sunix_parport_pc_enable_irq,
	.disable_irq		= sunix_parport_pc_disable_irq,
	.data_forward		= sunix_parport_pc_data_forward,
	.data_reverse		= sunix_parport_pc_data_reverse,
	.init_state			= sunix_parport_pc_init_state,
	.save_state			= sunix_parport_pc_save_state,
	.restore_state		= sunix_parport_pc_restore_state,
	.epp_write_data		= sunix_parport_ieee1284_epp_write_data,
	.epp_read_data		= sunix_parport_ieee1284_epp_read_data,
	.epp_write_addr		= sunix_parport_ieee1284_epp_write_addr,
	.epp_read_addr		= sunix_parport_ieee1284_epp_read_addr,
	.ecp_write_data		= sunix_parport_ieee1284_ecp_write_data,
	.ecp_read_data		= sunix_parport_ieee1284_ecp_read_data,
	.ecp_write_addr		= sunix_parport_ieee1284_ecp_write_addr,
	.compat_write_data	= sunix_parport_ieee1284_write_compat,
	.nibble_read_data	= sunix_parport_ieee1284_read_nibble,
	.byte_read_data		= sunix_parport_ieee1284_read_byte,
	.owner				= THIS_MODULE,
};


static int sunix_parport_SPP_supported(struct snx_parport *pb)
{
	unsigned char dcr, w;

	sunix_clear_epp_timeout(pb);

	w = 0xc;
	outb(w, SNX_DCR (pb));

	dcr = inb(SNX_DCR (pb));

	if ((dcr & 0xf) == w) {
		w = 0xe;
		outb(w, SNX_DCR (pb));
		dcr = inb (SNX_DCR (pb));
		outb(0xc, SNX_DCR (pb));

		if ((dcr & 0xf) == w) {
			return PARPORT_MODE_PCSPP;
		}
	}

	w = 0xaa;
	sunix_parport_pc_write_data(pb, w);

	dcr = sunix_parport_pc_read_data(pb);

	if (dcr == w) {
		w = 0x55;
		sunix_parport_pc_write_data(pb, w);
		dcr = sunix_parport_pc_read_data(pb);

		if (dcr == w) {
			return PARPORT_MODE_PCSPP;
		}
	}

	return 0;
}


static int sunix_parport_ECR_present(struct snx_parport *pb)
{
	struct sunix_par_port *priv = pb->private_data;
	unsigned char r = 0xc;

	outb(r, SNX_DCR (pb));

	if ((inb(SNX_ECR (pb)) & 0x3) == (r & 0x3)) {
		outb(r ^ 0x2, SNX_DCR (pb));

		r = inb(SNX_DCR (pb));

		if ((inb(SNX_ECR (pb)) & 0x2) == (r & 0x2)) {
			goto no_reg;
		}
	}

	if ((inb(SNX_ECR (pb)) & 0x3) != 0x1) {
		goto no_reg;
	}

	ECR_WRITE(pb, 0x34);

	if (inb(SNX_ECR (pb)) != 0x35) {
		goto no_reg;
	}

	priv->ecr = 1;
	outb(0xc, SNX_DCR (pb));


	sunix_frob_set_mode(pb, ECR_SPP);

	return 1;

no_reg:
	outb(0xc, SNX_DCR (pb));
	return 0;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))

static int  sunix_parport_PS2_supported(struct snx_parport *pb)
{
	return 0;
}


static int  sunix_parport_EPP_supported(struct snx_parport *pb)
{
	return 0;
}


static int  sunix_parport_ECPEPP_supported(struct snx_parport *pb)
{
	return 0;
}


static int  sunix_parport_ECPPS2_supported(struct snx_parport *pb)
{
	return 0;
}

#else

static int __devinit sunix_parport_PS2_supported(struct snx_parport *pb)
{
	return 0;
}


static int __devinit sunix_parport_EPP_supported(struct snx_parport *pb)
{
	return 0;
}


static int __devinit sunix_parport_ECPEPP_supported(struct snx_parport *pb)
{
	return 0;
}


static int __devinit sunix_parport_ECPPS2_supported(struct snx_parport *pb)
{
	return 0;
}

#endif







struct snx_parport *sunix_parport_pc_probe_port(struct sunix_par_port *priv)
{
	struct snx_parport_ops *ops = NULL;
	struct snx_parport *p = NULL;
	struct resource *base_res;
	struct resource	*ecr_res = NULL;

	if (!priv) {
		goto out1;
	}

	ops = kmalloc(sizeof(struct snx_parport_ops), GFP_KERNEL);
	if (!ops) {
		goto out1;
	}

	p = sunix_parport_register_port(priv, ops);
	if (!p) {
		goto out2;
	}

	base_res = request_region(p->base, SNX_PAR_ADDRESS_LENGTH, "snx_par_base");
	if (!base_res) {
		goto out3;
	}

	memcpy(ops, &sunix_parport_pc_ops, sizeof(struct snx_parport_ops));

	priv->ctr = 0xc;
	priv->ctr_writable = ~0x10;
	priv->ecr = 0;
	priv->fifo_depth = 0;
	priv->dma_buf = NULL;
	priv->dma_handle = 0;
	INIT_LIST_HEAD(&priv->list);

	p->modes = PARPORT_MODE_PCSPP | PARPORT_MODE_SAFEININT;
	p->private_data = priv;

	if (p->base_hi) {
		ecr_res = request_region(p->base_hi, SNX_PAR_ADDRESS_LENGTH, "snx_par_ehan");
		if (ecr_res) {
			sunix_parport_ECR_present(p);
		}

		if (!sunix_parport_EPP_supported(p)) {
			sunix_parport_ECPEPP_supported(p);
		}
	}

	if (!sunix_parport_SPP_supported(p)) {
		goto out4;
	}

	if (priv->ecr) {
		sunix_parport_ECPPS2_supported(p);
	} else {
		sunix_parport_PS2_supported(p);
	}

	p->size = (p->modes & PARPORT_MODE_EPP)?8:3;

	printk("SNX Info : %s - PC-style at 0x%lx", p->name, p->base);
	if (p->base_hi && priv->ecr) {
		printk(" (0x%lx)\n", p->base_hi);
	}

	if (priv->ecr) {
		ECR_WRITE(p, 0x34);
	}

	sunix_parport_pc_write_data(p, 0);

	sunix_parport_pc_data_forward(p);


	spin_lock(&snx_ports_lock);
	list_add(&priv->list, &snx_ports_list);
	spin_unlock(&snx_ports_lock);

	sunix_parport_announce_port(p);

	return p;

out4:
	if (ecr_res) {
		release_region(p->base_hi, SNX_PAR_ADDRESS_LENGTH);
	}

	release_region(p->base, SNX_PAR_ADDRESS_LENGTH);

out3:
	sunix_parport_put_port(p);

out2:
	kfree (ops);

out1:
	return NULL;
}


void sunix_parport_pc_unregister_port(struct snx_parport *p)
{
	struct sunix_par_port *priv = p->private_data;
	struct snx_parport_ops *ops = p->ops;

	sunix_parport_remove_port(p);

	spin_lock(&snx_ports_lock);
	list_del_init(&priv->list);
	spin_unlock(&snx_ports_lock);

	release_region(p->base, SNX_PAR_ADDRESS_LENGTH);

	if (p->base_hi) {
		release_region(p->base_hi, SNX_PAR_ADDRESS_LENGTH);
	}

	sunix_parport_put_port(p);

	kfree (ops);
}


int sunix_par_parport_init(void)
{
	struct sunix_par_port *pp = NULL;
    int status = 0;
	int i;

	for (i = 0; i < SNX_PAR_TOTAL_MAX; i++) {
		pp = &sunix_par_table[i];

		if ((pp->base > 0) && (pp->chip_flag != SUNNONE_HWID)) {
			pp->port = sunix_parport_pc_probe_port(pp);
			if (!pp->port) {
				status = -ENODEV;
				break;
			}
		}

		if (status != 0) {
			break;
		}
	}

	return status;
}


void sunix_par_parport_exit(void)
{
	spin_lock(&snx_ports_lock);
	while (!list_empty(&snx_ports_list)) {
		struct sunix_par_port *priv;
		struct snx_parport *port;

		priv = list_entry(snx_ports_list.next, struct sunix_par_port, list);

		port = priv->port;
		spin_unlock(&snx_ports_lock);
		sunix_parport_pc_unregister_port(port);
		spin_lock(&snx_ports_lock);
	}
	spin_unlock(&snx_ports_lock);
}

