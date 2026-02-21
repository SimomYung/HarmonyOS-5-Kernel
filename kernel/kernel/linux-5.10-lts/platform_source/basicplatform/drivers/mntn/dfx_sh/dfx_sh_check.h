#ifndef __DFX_SH_CHECK_H__
#define __DFX_SH_CHECK_H__

#include <linux/types.h>

bool sh_check_panic(enum sh_type type);
bool is_sh_gpu_pim_exc(void);
int is_sh_vdec_exc(void);
bool sh_peri_need_fast_pv(void);
bool is_sh_hfrc_panic(void);
#endif /* __DFX_SH_CHECK_H__ */
