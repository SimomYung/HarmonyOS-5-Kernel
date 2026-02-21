#ifndef _LINUX_PAC_H
#define _LINUX_PAC_H

#ifdef PAC_FWCFI_SYSSERVICE
#define PAC_FECFI_SIGN(_addr, _type) \
	asm volatile ("pacia %0, %1 \n": "=r" (_addr): "r"(__builtin_get_modifier_bytype(_type)), "0"(_addr))
#define PAC_FECFI_XPAC(_addr) \
({ \
	void *__addr = (_addr); \
\
	asm volatile ("xpaci %0\n" : "+r" (__addr)); \
	(typeof (_addr))__addr; \
})
#define PAC_FECFI_XPAC_SIGN(_addr, _type) do { \
        _addr = PAC_FECFI_XPAC(_addr); \
        if ((_addr) != NULL) { \
                PAC_FECFI_SIGN((_addr), _type); \
        } \
} while (0)
#endif

#endif
