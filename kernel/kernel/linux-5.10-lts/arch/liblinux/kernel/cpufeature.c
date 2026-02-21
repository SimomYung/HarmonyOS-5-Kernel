// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/types.h>
#include <linux/export.h>

bool cpu_have_feature(unsigned int num)
{
	return true;
}
EXPORT_SYMBOL_GPL(cpu_have_feature);
