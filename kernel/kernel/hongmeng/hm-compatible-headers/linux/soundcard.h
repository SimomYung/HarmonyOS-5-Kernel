/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/soundcard.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 25 16:37:19 2021
 */

#ifndef _UAPISOUNDCARD_H
#define _UAPISOUNDCARD_H

#include <linux/ioctl.h>

#ifndef _SIOWR
#if defined(_IOWR) && (defined(_AIX) || (!defined(sun) && !defined(sparc) \
	&& !defined(__sparc__) && !defined(__INCioctlh) && !defined(__Lynx__)))
#define _SIOR	_IOR
#define _SIOWR	_IOWR
#else
#define SIOCPARM_MASK   0x1fff
#define SIOC_OUT    0x20000000
#define SIOC_IN     0x40000000
#define SIOC_INOUT  (SIOC_IN|SIOC_OUT)
#define _SIOR(x,y,t)    ((int)(SIOC_OUT|((sizeof(t)&SIOCPARM_MASK)<<16)|(x<<8)|y))
#define _SIOWR(x,y,t)   ((int)(SIOC_INOUT|((sizeof(t)&SIOCPARM_MASK)<<16)|(x<<8)|y))
#endif /* _IOWR */
#endif /* _SIOWR */

#define SOUND_MIXER_RECSRC  0xff
#define SOUND_MIXER_DEVMASK 0xfe
#define SOUND_MIXER_RECMASK 0xfd

#define SOUND_MIXER_NRDEVICES   25
#define SOUND_MIXER_VOLUME  0
#define SOUND_MIXER_LINE    6
#define SOUND_MIXER_MIC     7
#define SOUND_MIXER_RECLEV  11

#define SOUND_DEVICE_NAMES  {"vol", "bass", "treble", "synth", "pcm", "speaker", "line", \
	"mic", "cd", "mix", "pcm2", "rec", "igain", "ogain", \
	"line1", "line2", "line3", "dig1", "dig2", "dig3", \
	"phin", "phout", "video", "radio", "monitor"}

#define MIXER_READ(dev)		_SIOR('M', dev, int)
#define SOUND_MIXER_READ_RECSRC     MIXER_READ(SOUND_MIXER_RECSRC)
#define SOUND_MIXER_READ_DEVMASK    MIXER_READ(SOUND_MIXER_DEVMASK)
#define SOUND_MIXER_READ_RECMASK    MIXER_READ(SOUND_MIXER_RECMASK)

#define MIXER_WRITE(dev)        _SIOWR('M', dev, int)
#define SOUND_MIXER_WRITE_VOLUME    MIXER_WRITE(SOUND_MIXER_VOLUME)
#define SOUND_MIXER_WRITE_LINE      MIXER_WRITE(SOUND_MIXER_LINE)
#define SOUND_MIXER_WRITE_MIC       MIXER_WRITE(SOUND_MIXER_MIC)

#define SOUND_MIXER_WRITE_RECLEV    MIXER_WRITE(SOUND_MIXER_RECLEV)

#define SOUND_MIXER_WRITE_RECSRC    MIXER_WRITE(SOUND_MIXER_RECSRC)

#endif
