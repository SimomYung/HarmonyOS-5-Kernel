/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __SIGNAL_H__
#define __SIGNAL_H__

extern void __init_sighand(struct task_struct *tsk);
extern int __init_sighand_kthread(struct task_struct *tsk);

#endif /* __SIGNAL_H__ */
