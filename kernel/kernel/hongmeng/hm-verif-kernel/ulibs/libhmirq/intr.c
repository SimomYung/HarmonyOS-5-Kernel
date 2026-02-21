/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Native irq api implementation for user app usage
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 26 10:32:19 2019
 */
#include <libhmirq/irq.h>
#include <libhmirq/intr.h>
#include <libhmactv/actv.h>
#include <libpreempt/preempt.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_sysctrl.h>

#include <hmkernel/trigger.h>
#include <hmkernel/sysconf.h>

#define LIBHMIRQ_VERSION_MAJOR	1U
#define LIBHMIRQ_VERSION_MINOR	0U
#define LIBHMIRQ_VERSION_PATCH	1U
#define LIBHMIRQ_VERSION_BUILD	0U

void hm_irq_version_get(struct hm_irq_version *version)
{
	if (version != NULL) {
		version->major = LIBHMIRQ_VERSION_MAJOR;
		version->minor = LIBHMIRQ_VERSION_MINOR;
		version->patch = LIBHMIRQ_VERSION_PATCH;
		version->build = LIBHMIRQ_VERSION_BUILD;
	}
}

int hm_irq_version_check(const struct hm_irq_version *expected)
{
	int ret = E_HM_OK;

	if (expected == NULL) {
		ret = E_HM_INVAL;
	}

	if ((ret == E_HM_OK) && (expected->major > LIBHMIRQ_VERSION_MAJOR)) {
		ret = E_HM_NOSYS;
	}

	/* According to semantic versioning, only major will affect compatibility */
	return ret;
}

int hm_irq_default_ctrl_id(enum hm_irq_ctrl_type type)
{
	int ret = E_HM_OK;

	switch (type) {
	case HM_IRQ_CTRL_TYPE_PHYSICAL:
		ret = actvcall_hmcall_irq_default_phyctrl_id();
		break;
	case HM_IRQ_CTRL_TYPE_SMC:
		ret = E_HM_NOSYS;
		break;
	case HM_NR_IRQ_CTRL_TYPE:	/* fall through, allowed empty */
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

/* desc: Require irq grant, config, register and enable this irq on
 *       current cpu. Note that after invoking this function and before
 *       it returns the irq maybe triggered.
 * @in:  ctrl_id:  id of irqctrl
 *       config:   config of this irq
 * @out: irq_rref: rref of this irq
 * ret:  < 0: error code; >= 0: requested irq
 */
static int
hm_irq_request_arg_check(const int ctrl_id, const struct hm_irq_request_config *config,
			 const uint32_t *logic_irq, const rref_t *irq_rref)
{
	int ret = E_HM_OK;

	if (ctrl_id < 0) {
		ret = E_HM_INVAL;
	}

	if ((ret == E_HM_OK) && (config == NULL || logic_irq == NULL || irq_rref == NULL)) {
		ret = E_HM_INVAL;
	}

	if ((ret == E_HM_OK) && (config->conf == NULL)) {
		ret = E_HM_INVAL;
	}

	return ret;
}

static int
hm_irq_request_prepare(int ctrl_id, const struct hm_irq_request_config *config,
		       uint32_t *logic_irq, rref_t *irq_rref)
{
	uint32_t logic;
	rref_t rref;
	int ret;

	/* NOTE kernel should support user get logic_irq on current cpu,
	 * using cpuid 0 as a temporary method */
	ret = hm_irq_conf_to_irq(ctrl_id, config->conf, config->conf_len, 0);
	if (ret >= 0) {
		/* if ret is not less than 0, means logic irq */
		logic = (uint32_t)ret;

		rref = hm_irq_require_grant(ctrl_id, logic);
		if (IS_REF_ERR(rref)) {
			ret = REF_TO_ERR(rref);
		} else {
			*logic_irq = logic;
			*irq_rref = rref;
			ret = E_HM_OK;
		}
	}

	return ret;
}

static int
hm_irq_request_implement(rref_t rref, uint32_t irq,
			 struct hm_irq_request_config *config)
{
	struct hm_irq_register_setting reg_setting;
	int err;

	/* config irq */
	err = hm_irq_config(rref, irq, config->conf,
			    config->conf_len);
	if (err >= 0) {
		/* register irq */
		reg_setting.name = config->name;
		/* using sync message by default */
		reg_setting.flags = config->flags;
		/* register to all cpus */
		reg_setting.cpu_mask = __cpuset_full;
		/* not use irq actv fastpath */
		reg_setting.irq_fastpath_ap = (cref_t)0ULL;
		/* not bypass curr cnode when use fastpath */
		reg_setting.is_fp_bypass = 0U;

		err = hm_irq_register(rref, irq, &reg_setting,
				      config->tcb_cref);
	}

	if (err >= 0) {
		/* enable irq */
		err = hm_irq_enable(rref, irq);
	}

	return err;
}

int hm_irq_request_current_cpu(int ctrl_id,
			       struct hm_irq_request_config *config,
			       rref_t *irq_rref)
{
	int err;
	uint32_t irq = 0U;
	rref_t rref = 0ULL;

	err = hm_irq_request_arg_check(ctrl_id, config, &irq, &rref);
	if (err >= 0) {
		err = hm_irq_request_prepare(ctrl_id, config, &irq, &rref);
	}

	if (err >= 0) {
		err = hm_irq_request_implement(rref, irq, config);
		if (err < 0) {
			(void)hm_irq_reject_grant(rref);
		}
	}

	if (err >= 0) {
		/* irq is successfully request & enabled */
		if (irq_rref != NULL) {
			*irq_rref = rref;
			err = (int)irq;
		}
	}

	return err;
}

int hm_irq_free(rref_t irq_rref)
{
	return hm_irq_reject_grant(irq_rref);
}

int hm_irq_enable(rref_t irq_rref, uint32_t irq)
{
	return syscap_IrqCtrlEnable(irq_rref, irq);
}

int hm_irq_disable(rref_t irq_rref, uint32_t irq)
{
	return syscap_IrqCtrlDisable(irq_rref, irq);
}

int hm_irq_set_wake(rref_t irq_rref, uint32_t irq, uint32_t flag)
{
	return syscap_IrqCtrlSetWake(irq_rref, irq, flag);
}

static int irq_stat_actv(struct bunch_actv_attr *attr, void *ctrl_name)
{
	struct irqmgr_stat_arg arg;
	char *name = void_to_ptr(ctrl_name, char);

	NOFAIL(strncpy_s(arg.name, sizeof(arg.name), name, strlen(name)));
	arg.attr = *attr;
	return actvcall_hmcall_irq_stat(arg);
}

int hm_irq_stat(const char *name, struct bunch *bunch)
{
	int err = E_HM_OK;
	size_t len;
	char ctrl_name[IRQCTRL_NAME_LEN];
	struct bunch_actv_reader_ctx ctx;

	if (bunch == NULL || name == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		len = strnlen(name, IRQCTRL_NAME_LEN);
		if (len >= IRQCTRL_NAME_LEN || len == 0UL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(ctrl_name, sizeof(ctrl_name), name, strlen(name)));

		ctx.actvcall = irq_stat_actv;
		ctx.private_data = ptr_to_void(ctrl_name);

		err = bunch_actv_request(bunch, &ctx);
		if (err < 0) {
			hm_info("irq_stat request failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}

int hm_irq_wait_for_irq(void)
{
	return hm_trigger_wait(__TRIGGER_WAIT_DONE, NULL);
}

/*
 * The system services (UDK/LDK/Test) do not destroy actv pools
 * except when the process exits.
 * Reclaim resources on process exit
 */
static void __destroy_fastpath(struct actvpool_configs *irq_ap_cfg,
			       struct irq_fastpath_res_s *irq_fp_res)
{
	UNUSED(irq_fp_res);
	(void)hm_apconfig_private_destroy(irq_ap_cfg);
}

void hm_irq_destroy_fastpath(struct actvpool_configs *irq_ap_cfg,
			     struct irq_fastpath_res_s *irq_fp_res)
{
	int err = E_HM_OK;

	if (irq_ap_cfg == NULL || irq_fp_res == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		__destroy_fastpath(irq_ap_cfg, irq_fp_res);
	}
}

static int __create_fastpath(struct actvpool_configs *irq_ap_cfg,
			     struct irq_fastpath_res_s *irq_fp_res,
			     int (*irq_fastpath_init)(void))
{
	int err = E_HM_OK;
	cref_t irq_ap = (cref_t)0UL;

	NOFAIL(hm_apconfig_private_init(irq_ap_cfg));
	NOFAIL(hm_apconfig_private_set_actvinitfunc(irq_ap_cfg, irq_fastpath_init));
	/* set preinitentry for no page fault */
	NOFAIL(hm_apconfig_private_enable_preinitentry(irq_ap_cfg));
	NOFAIL(hm_apconfig_private_set_poolid(irq_ap_cfg, IRQ_ACTV_POOLID));

	if (preempt_process_feature_enabled()) {
		NOFAIL(hm_apconfig_private_enable_alak(irq_ap_cfg));
	}

	err = hm_setup_actvpool(irq_ap_cfg, &irq_ap);
	if (err == E_HM_OK) {
		irq_fp_res->irq_actv_pool = irq_ap;
	} else {
		__destroy_fastpath(irq_ap_cfg, irq_fp_res);
	}

	return err;
}

/*
 * Only use for system services(LDK/UDK/Test) to create irq fastpath resource;
 * The rest of the use needs to control the resource managemet by itself.
 * Only an even nr of actvs are supported.
 */
int hm_irq_create_fastpath(struct actvpool_configs *irq_ap_cfg,
			   struct irq_fastpath_res_s *irq_fp_res,
			   int (*irq_fastpath_init)(void))
{
	int err = E_HM_OK;
	int rc = 0;
	unsigned int irq_mode;
	struct irq_fastpath_res_s tmp_irq_fp_res;
	bool do_create = __false;

	if (irq_ap_cfg == NULL || irq_fp_res == NULL ||
	    irq_fastpath_init == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		rc = hm_sysctrl_read_conf(__SYSCONF_IRQ_MODE, &irq_mode,
					  sizeof(irq_mode));
		if (rc >= 0) {
			/* only SUI & SUI_NG mode need create irq-fastpath res */
			if (____IS(irq_mode & IRQ_MODE_SUI) &&
			    ____IS(irq_mode & IRQ_MODE_SUI_NG)) {
				do_create = __true;
			}
		}
	}

	if (err == E_HM_OK && do_create) {
		mem_zero_s(tmp_irq_fp_res);
		err = __create_fastpath(irq_ap_cfg, &tmp_irq_fp_res, irq_fastpath_init);
	}

	if (err == E_HM_OK && do_create) {
		mem_zero_s(*irq_fp_res);
		*irq_fp_res = tmp_irq_fp_res;
	}

	return err;
}
