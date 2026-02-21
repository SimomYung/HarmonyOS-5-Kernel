/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Module of misc functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 25 15:40:59 CST 2020
 */

#include <lib/string.h>
#include <hmkernel/panic.h>
#include <hmkernel/errno.h>
#include <hmkernel/module.h>
#include <hmkernel/module_ops.h>
#include <hmkernel/capability/cap_cnode.h>
#include <hmkernel/mm/pgtable.h>
#include <uapi/misc/misc_module.h>

struct misc_ioctrl_arg_s {
	const void __user *args;
	unsigned long args_size;
	void __user *result;
	unsigned long result_size;
};

static int vspace_pte_show(unsigned int level, uptr_t vstart, u64 pa,
			   void *pte, bool is_pt, size_t pt_size, void *arg)
{
	UNUSED(level, vstart, pt_size);
	struct __misc_vspace_pte_result *result = (struct __misc_vspace_pte_result *)arg;

	if (result->count < __MAX_PGTABLE_LEVEL) {
		result->pa[result->count] = pa;
		result->is_lastlv[result->count] = is_pt;
		result->pte[result->count++] = *((unsigned long *)pte);
	}

	return E_HM_OK;
}

static int misc_vspace_ioctl(struct capcall_info_s *callinfo, unsigned long cmd,
			     struct misc_ioctrl_arg_s *arg)
{
	int err = E_HM_OK;
	struct capability_s *vspace_cap = NULL;
	struct CNode_s *caller = module_callinfo_caller_of(callinfo);
	struct __misc_vspace_args vspace_args;
	struct __misc_vspace_pte_result pte_result;

	mem_zero_s(vspace_args);
	if (arg == NULL || arg->args == NULL ||
	    (arg->args_size != sizeof(vspace_args))) {
		err = E_HM_INVAL;
	} else {
		err = module_copy_from_user(&vspace_args, sizeof(vspace_args),
					    arg->args, arg->args_size);
	}

	if (err == E_HM_OK) {
		vspace_cap = cnode_lookup_and_get(caller, vspace_args.cref, hmobj_VSpace);
		if (IS_PTR_ERR(vspace_cap)) {
			err = PTR_TO_ERR(vspace_cap);
		}
	}

	if (err == E_HM_OK) {
		switch (cmd) {
		case __MISC_VSPACE_SHOW_PTE:
			if (arg->result == NULL ||
			    arg->result_size != sizeof(pte_result)) {
				err = E_HM_FAULT;
			}
			mem_zero_s(pte_result);
			if (err == E_HM_OK) {
				err = pgtable_visit(vspace_cap, (uptr_t)vspace_args.vaddr,
						    (uptr_t)(vspace_args.vaddr + 1ULL),
						    vspace_pte_show, (void *)&pte_result);
			}
			if (err == E_HM_OK) {
				err = module_copy_to_user(arg->result, arg->result_size,
							  &pte_result, sizeof(pte_result));
			}
			break;
		default:
			err = E_HM_INVAL;
			break;
		}
		NOFAIL(cap_refcnt_dec(vspace_cap));
	}

	return err;
}

static int misc_module_ioctl(struct capcall_info_s *callinfo,
			     unsigned long cmd,
			     const void __user *args, size_t args_size,
			     void __user *result, size_t result_size)
{
	int err = E_HM_OK;
	struct misc_ioctrl_arg_s arg;

	mem_zero_s(arg);
	arg.args = args;
	arg.args_size = args_size;
	arg.result = result;
	arg.result_size = result_size;
	switch (cmd) {
	case __MISC_VSPACE_SHOW_PTE:
		err = misc_vspace_ioctl(callinfo, cmd, &arg);
		break;
	default:
		err = E_HM_INVAL;
		break;
	}

	return err;
}

static int misc_module_kernel_init(const void *addr)
{
	UNUSED(addr);
	klog_info("misc module init....\n");
	return E_HM_OK;
}

static struct module_s misc_module_s;

static struct module_ops_s misc_module_ops __read_mostly = {
	.list		= DLIST_HEAD_INIT(misc_module_ops.list),
	.name		= "misc_module",
	.module_addr	= &misc_module_s,
	.modif_handler	= NULL,
	.module_init	= misc_module_kernel_init,
	.module_destroy	= NULL,
	.module_ioctl	= misc_module_ioctl,
};

__llt_early_device_init __init
static int misc_module_init(void)
{
	module_ops_register(&misc_module_ops);
	return E_HM_OK;
}
early_device_init(misc_module_init)

#ifdef __HOST_LLT__
extern int llt_misc_vspace_ioctl(struct capcall_info_s *callinfo, unsigned long cmd,
				 void *arg);
extern int llt_vspace_pte_show(unsigned int level, uptr_t vstart, u64 pa,
			       void *pte, bool is_pt, size_t pt_size, void *arg);

int llt_misc_vspace_ioctl(struct capcall_info_s *callinfo, unsigned long cmd,
			  void *arg)
{
	return misc_vspace_ioctl(callinfo, cmd, (struct misc_ioctrl_arg_s *)arg);
}

int llt_vspace_pte_show(unsigned int level, uptr_t vstart, u64 pa,
			void *pte, bool is_pt, size_t pt_size, void *arg)
{
	return vspace_pte_show(level, vstart, pa, pte, is_pt, pt_size, arg);
}
#endif /* __HOST_LLT__ */
