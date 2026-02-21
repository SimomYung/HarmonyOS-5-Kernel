#ifndef __H_PM_SR_H__
#define __H_PM_SR_H__

#include <platform_include/cee/linux/sr_dmd.h>
#ifdef  CONFIG_PM_DEL_PLATFORM_ADDR
#include <lpm_runtime_offset.h>
#else
#include <lpmcu_runtime.h>
#endif

#if defined (DEEPSLEEP_MNTN_TIME)
u64 get_deepsleep_dur_time(void);
#else
static inline u64 get_deepsleep_dur_time(void) { return 0; }
#endif

#endif /* __H_PM_SR_H__ */