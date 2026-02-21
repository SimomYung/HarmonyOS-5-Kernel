/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Native irq api for fine-grained usage
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 10:32:19 2019
 */
#include <hongmeng/errno.h>
#include <hmkernel/const.h>
#include <hongmeng/syscall.h>

#include <libhmirq/irq.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hmsrv_sys.h>

#define HM_IRQ_CONF_MAX_LEN		8U

static uint32_t irqmgr_idx = 0;

__thread struct timespec hm_irq_entry_timespec = {0};
__thread struct timespec hm_irq_exit_timespec = {0};

int hm_irq_entry(void)
{
	int err = 0;

	err = hm_clock_gettime(CLOCK_MONOTONIC, &hm_irq_entry_timespec);
	if (err < 0) {
		hm_warn("clock gettime failed, %s\n", strerror(-err));
	}

	return err;
}
 
int hm_irq_exit(void)
{
	int err = 0;

	err = hm_clock_gettime(CLOCK_MONOTONIC, &hm_irq_exit_timespec);
	if (err < 0) {
		hm_warn("clock gettime failed, %s\n", strerror(-err));
	}

	return err;
}

int hm_irq_exec_time(void)
{
	uint64_t sec = 0;
	uint64_t nsec = 0;
	uint64_t usec = 0;

	if (hm_irq_entry_timespec.tv_sec == 0 && hm_irq_entry_timespec.tv_nsec == 0) {
		return E_HM_INVAL;
	}

	if (hm_irq_exit_timespec.tv_sec == 0 && hm_irq_exit_timespec.tv_nsec == 0) {
		return E_HM_INVAL;
	}

	if (hm_irq_exit_timespec.tv_nsec < hm_irq_entry_timespec.tv_nsec) {
		sec = hm_irq_exit_timespec.tv_sec - hm_irq_entry_timespec.tv_sec - 1;
		nsec = hm_irq_exit_timespec.tv_nsec + NSEC_PER_SEC - hm_irq_entry_timespec.tv_nsec;
	} else {
		sec = hm_irq_exit_timespec.tv_sec - hm_irq_entry_timespec.tv_sec;
		nsec = hm_irq_exit_timespec.tv_nsec - hm_irq_entry_timespec.tv_nsec;
	}
	usec = sec * USEC_PER_SEC + nsec / NSEC_PER_USEC;

	return usec;
}

int hm_irq_init_idx(void)
{
	/* Sysmgr should not rely on this func */
	irqmgr_idx = hmsrv_sysmgr_cnode_idx();
	return 0;
}

int hm_irq_conf_to_irq(int ctrl_id, const uint32_t *conf,
		       uint32_t conf_len, uint32_t cpu_id)
{
	struct irqmgr_to_logic_arg arg;
	int err = E_HM_OK;

	if (conf_len > HM_IRQ_CONF_MAX_LEN) {
		err = E_HM_TOOMANY;
	}

	if ((err == E_HM_OK) && (conf == NULL)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(memcpy_s(&arg.hwconf[0], sizeof(arg.hwconf),
				conf, conf_len * sizeof(uint32_t)));
		arg.hwconf_len = conf_len;

		err = actvcall_hmcall_irq_to_logic(ctrl_id, arg, cpu_id);
	}

	return err;
}

rref_t hm_irq_require_grant(int ctrl_id, uint32_t irq)
{
	/* fisrt check if we have a valid irqmgr_idx */
	int err;
	rref_t irq_ret;
	struct irqmgr_grant_arg arg;

	if (irqmgr_idx == 0U) {
		(void)hm_irq_init_idx();
	}

	irq_ret = hm_ucap_require(0, irqmgr_idx, 0);
	if (!IS_REF_ERR(irq_ret)) {
		arg.irqctrl_id = ctrl_id;
		arg.irqctrl_rref = irq_ret;
		arg.logic_irq = irq;

		/* call irqmgr for grant */
		err = actvcall_hmcall_irq_grant_ctrl(arg);
		if (err < 0) {
			int ret;
			/* impossible to failed */
			ret = hm_ucap_reject(0, irq_ret);
			BUG_ON(ret < 0);
			irq_ret = ERR_TO_REF(err);
		}
	}

	return irq_ret;
}

int hm_irq_reject_grant(rref_t irq_rref)
{
	return hm_ucap_reject(0, irq_rref);
}

int hm_irq_config(rref_t irq_rref, uint32_t irq,
		  uint32_t *conf, uint32_t conf_len)
{
	int err = E_HM_OK;

	if (conf_len > HM_IRQ_CONF_MAX_LEN) {
		err = E_HM_TOOMANY;
	}

	if ((err == E_HM_OK) && (conf == NULL)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = syscap_IrqCtrlConfig(irq_rref, irq, conf, conf_len);
	}

	return err;
}

int hm_irq_register(rref_t irq_rref, uint32_t irq,
		    const struct hm_irq_register_setting *setting,
		    cref_t tcb_cref)
{
	int err = E_HM_OK;
	struct sysarg_irqctrl_register arg;
	size_t name_len = 0UL;

	if (setting == NULL
	    || setting->name == NULL
	    || (__cpuset_is_empty(setting->cpu_mask) != 0)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		/* name length should less than IRQCTRL_NAME_LEN */
		name_len = strnlen(setting->name, IRQCTRL_NAME_LEN);
		if (name_len >= IRQCTRL_NAME_LEN || name_len == 0UL) {
			hm_error("name length is incorrect\n");
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		mem_zero_s(arg);
		err = strncpy_s(arg.name, sizeof(arg.name), setting->name,
				name_len);
		if (err != 0) {
			hm_error("strncpy_s failed\n");
			err = E_HM_RESFAULT;
		}
	}

	if (err == E_HM_OK) {
		arg.flags = setting->flags;
		arg.cpu_mask = setting->cpu_mask;
		arg.irq_fastpath_ap = setting->irq_fastpath_ap;
		arg.is_fp_bypass = setting->is_fp_bypass;

		/* currently, irq_register_setting is same with sysarg_irqctrl_register */
		err = syscap_IrqCtrlRegister(irq_rref, irq, &arg, tcb_cref);
	}

	return err;
}

int hm_irq_unregister(rref_t irq_rref, uint32_t irq)
{
	return syscap_IrqCtrlUnRegister(irq_rref, irq);
}

int hm_irq_generate_sgi(rref_t irq_rref, uint32_t irq,
			__cpuset_t target_set, void *arg,
			uint32_t arg_len)
{
	int err = E_HM_OK;

	if (__cpuset_equal(target_set, __cpuset_empty) == 0) {
		/*
		 * NOTE: cpuset has not been supported, use bits directly
		 * parameter *arg allowed to be NULL
		 */
		err = syscap_IrqCtrlSGI(irq_rref, irq, &target_set, arg, arg_len);
	}

	/* no target, return directly */
	return err;
}

int hm_irq_ioctl(rref_t irq_rref, uint32_t irq,
		 uint32_t cmd, const void *arg,
		 uint32_t arg_len)
{
	return syscap_IrqCtrlIoctl(irq_rref, irq, cmd,
				   arg, arg_len);
}

int hm_irq_set_attribute(rref_t irq_rref, uint32_t logic_irq,
			 struct hm_irq_attribute *hm_irq_attr,
			 unsigned int flag)
{
	int err = E_HM_OK;
	struct sysarg_irqctrl_attribute sysarg_attr = {0};

	if (hm_irq_attr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		sysarg_attr.attr_cpu = hm_irq_attr->attr_cpu;
		sysarg_attr.attr_type = hm_irq_attr->attr_type;
		sysarg_attr.attr_affi_hint = hm_irq_attr->attr_affi_hint;

		err = syscap_IrqCtrlSetAttribute(irq_rref, logic_irq,
						 &sysarg_attr, flag);
	}

	return err;
}
