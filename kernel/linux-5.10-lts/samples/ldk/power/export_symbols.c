// SPDX-License-Identifier: GPL-2.0
#include <linux/export.h>
#include <linux/compat.h>
#include <platform_include/cee/linux/hw_vote.h>

#ifdef CONFIG_HW_VOTE
/* hw_vote APIs used in hmtpp_dal.ko */
EXPORT_SYMBOL(hv_set_freq);
EXPORT_SYMBOL(hv_get_freq);
EXPORT_SYMBOL(hv_get_result);
EXPORT_SYMBOL(hvdev_register);
EXPORT_SYMBOL(hvdev_remove);
#endif /* CONFIG_HW_VOTE */
