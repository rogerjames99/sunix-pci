# sunix-pci

This repository contains a modified version of the official open source pci
parallel port driver from Sunix Microsystems. The modifications allow the driver
to be built on more recent linux kernel versions.

If you are not using the driver for driving a printer then I recommend you do not
install either this driver or the the official sunix one and access the port via the
standard linux ppd driver. This allows you to bit bang the lines individually. I have
provided instructions for this and sample files in the ppd directory in this
repository.

