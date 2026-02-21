 /*
  * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
  * Description: notifier interface for drop module
  *
  * This software is licensed under the terms of the GNU General Public
  * License, as published by the Free Software Foundation, and
  * may be copied, distributed, and modified under those terms.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  */
#include <linux/export.h>
#include <linux/notifier.h>
#include <contexthub_drop_event.h>

 static ATOMIC_NOTIFIER_HEAD(g_drop_atomic_notifier_list);
 static BLOCKING_NOTIFIER_HEAD(g_drop_blocking_notifier_list);

 int drop_register_atomic_notifier(struct notifier_block *nb)
 {
 	return atomic_notifier_chain_register(
 		&g_drop_atomic_notifier_list, nb);
 }
 EXPORT_SYMBOL_GPL(drop_register_atomic_notifier);

 int drop_unregister_atomic_notifier(struct notifier_block *nb)
 {
 	return atomic_notifier_chain_unregister(
 		&g_drop_atomic_notifier_list, nb);
 }
 EXPORT_SYMBOL_GPL(drop_unregister_atomic_notifier);

 int call_drop_atomic_notifiers(unsigned long val, void *v)
 {
 	return atomic_notifier_call_chain(
 		&g_drop_atomic_notifier_list, val, v);
 }
 EXPORT_SYMBOL_GPL(call_drop_atomic_notifiers);

 int drop_register_blocking_notifier(struct notifier_block *nb)
 {
 	return blocking_notifier_chain_register(
 		&g_drop_blocking_notifier_list, nb);
 }
 EXPORT_SYMBOL_GPL(drop_register_blocking_notifier);

 int drop_unregister_blocking_notifier(struct notifier_block *nb)
 {
 	return blocking_notifier_chain_unregister(
 		&g_drop_blocking_notifier_list, nb);
 }
 EXPORT_SYMBOL_GPL(drop_unregister_blocking_notifier);

 int call_drop_blocking_notifiers(unsigned long val, void *v)
 {
 	return blocking_notifier_call_chain(
 		&g_drop_blocking_notifier_list, val, v);
 }
 EXPORT_SYMBOL_GPL(call_drop_blocking_notifiers);