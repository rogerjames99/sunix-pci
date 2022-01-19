#include "snx_common.h"


#define SNX_PP_IOCTL	'p'

struct snx_ppdev_frob_struct {
	unsigned char mask;
	unsigned char val;
};


#define SNX_PPSETMODE		_IOW(SNX_PP_IOCTL, 0x80, int)
#define SNX_PPRSTATUS		_IOR(SNX_PP_IOCTL, 0x81, unsigned char)
#define SNX_PPRCONTROL		_IOR(SNX_PP_IOCTL, 0x83, unsigned char)
#define SNX_PPWCONTROL		_IOW(SNX_PP_IOCTL, 0x84, unsigned char)
#define SNX_PPFCONTROL      _IOW(SNX_PP_IOCTL, 0x8e, struct snx_ppdev_frob_struct)
#define SNX_PPRDATA			_IOR(SNX_PP_IOCTL, 0x85, unsigned char)
#define SNX_PPWDATA			_IOW(SNX_PP_IOCTL, 0x86, unsigned char)
#define SNX_PPCLAIM			_IO(SNX_PP_IOCTL, 0x8b)
#define SNX_PPRELEASE		_IO(SNX_PP_IOCTL, 0x8c)
#define SNX_PPYIELD			_IO(SNX_PP_IOCTL, 0x8d)
#define SNX_PPEXCL			_IO(SNX_PP_IOCTL, 0x8f)
#define SNX_PPDATADIR		_IOW(SNX_PP_IOCTL, 0x90, int)
#define SNX_PPNEGOT			_IOW(SNX_PP_IOCTL, 0x91, int)
#define SNX_PPWCTLONIRQ		_IOW(SNX_PP_IOCTL, 0x92, unsigned char)
#define SNX_PPCLRIRQ		_IOR(SNX_PP_IOCTL, 0x93, int)
#define SNX_PPSETPHASE		_IOW(SNX_PP_IOCTL, 0x94, int)
#define SNX_PPGETTIME		_IOR(SNX_PP_IOCTL, 0x95, struct timeval)
#define SNX_PPSETTIME		_IOW(SNX_PP_IOCTL, 0x96, struct timeval)
#define SNX_PPGETMODES		_IOR(SNX_PP_IOCTL, 0x97, unsigned int)
#define SNX_PPGETMODE		_IOR(SNX_PP_IOCTL, 0x98, int)
#define SNX_PPGETPHASE		_IOR(SNX_PP_IOCTL, 0x99, int)
#define SNX_PPGETFLAGS		_IOR(SNX_PP_IOCTL, 0x9a, int)
#define SNX_PPSETFLAGS		_IOW(SNX_PP_IOCTL, 0x9b, int)

#define SNX_PP_FASTWRITE	(1<<2)
#define SNX_PP_FASTREAD		(1<<3)
#define SNX_PP_W91284PIC	(1<<4)

#define SNX_PP_FLAGMASK		(SNX_PP_FASTWRITE | SNX_PP_FASTREAD | SNX_PP_W91284PIC)

