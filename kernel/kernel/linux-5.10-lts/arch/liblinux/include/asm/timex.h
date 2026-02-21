/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_ASM_TIMEX_H
#define __LIBLINUX_ASM_TIMEX_H

#include <asm/arch_timer.h>

#define get_cycles()	__arch_counter_get_cntvct()
#define arch_timer_evtstrm_available() false

#define random_get_entropy()	get_cycles()

#include <asm-generic/timex.h>

void arch_time_sync(void);
void arch_time_sync_early(void);
unsigned long liblinux_get_time_us(void);
#endif /* __LIBILNUX_ASM_TIMEX_H */
