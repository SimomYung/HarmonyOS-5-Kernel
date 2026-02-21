// SPDX-License-Identifier: GPL-2.0-only

#include <linux/notifier.h>

int register_reboot_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&reboot_notifier_list, nb);
}
EXPORT_SYMBOL(register_reboot_notifier);

int unregister_reboot_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&reboot_notifier_list, nb);
}
EXPORT_SYMBOL(unregister_reboot_notifier);
