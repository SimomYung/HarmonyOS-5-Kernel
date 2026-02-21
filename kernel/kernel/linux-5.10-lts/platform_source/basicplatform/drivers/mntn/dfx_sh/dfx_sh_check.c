/* dfx_sh_check.c
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "dfx_sh.h"
#include "dfx_sh_check.h"
#include "rdr_inner.h"
#include "mntn_subtype_exception.h"
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <platform_include/basicplatform/linux/dfx_hw_diag.h>
#include <platform_include/cee/linux/hisi_ddr.h>
#include <linux/cpu.h>

static bool sh_check_cpu_panic_subtype(void)
{
	u32 rdr_subtype;

	if (rdr_get_reboot_type() == AP_S_PANIC){
		rdr_subtype = rdr_get_exec_subtype_value();
		if (rdr_subtype == HI_APPANIC_RESERVED)
			return true;

#if defined (CONFIG_DFX_EXC_INFO) && defined(CONFIG_DFX_EXPAND_AP_PANIC)
		if (rdr_subtype == HI_APPANIC_DATA_ABORT || rdr_subtype == HI_APPANIC_INSTRU_ABORT ||
			rdr_subtype == HI_APPANIC_SERROR)
			return true;
#endif
		}
#if defined (CONFIG_DFX_OHOS)
	if (rdr_get_reboot_type() == HM_PANIC){
		rdr_subtype = rdr_get_exec_subtype_value();
		if (rdr_subtype == HM_PANIC_KERNEL || rdr_subtype == HM_PANIC_SYSMGR ||
			rdr_subtype == HM_PANIC_DEVHOST || rdr_subtype == HM_PANIC_SERROR)
			return true;
	}
#endif
	return false;
}

static bool sh_check_cpu_panic(enum sh_type type)
{
	int cpunum;

	if (type > sh_type_cluster_max) {
		pr_err("dfx_sh check cpu panic failed, invalid type %u\n", type);
		return false;
	}
		if (sh_check_cpu_panic_subtype()) {
			cpunum = dfx_hw_diag_get_panic_cpunum();
			if (cpunum < 0 ||  cpunum >= nr_cpu_ids) {
				pr_err("dfx_sh check cpu panic failed, invalid cpu num %d\n", cpunum);
				return false;
			}
			pr_info("dfx_sh check cpu panic, cpu num %d, cluster: %d\n",
					cpunum, topology_physical_package_id(cpunum));
			return (topology_physical_package_id(cpunum) == type);
		}
	return false;
}

static bool is_sh_cpu_exception(enum sh_type type)
{
#if defined (CONFIG_DFX_OHOS)
	u32 rdr_subtype;
#endif

	if (rdr_get_reboot_type() == AP_S_AWDT || rdr_get_reboot_type() == LPM3_S_LPMCURST)
		return true;
#if defined (CONFIG_DFX_OHOS)
	if (rdr_get_reboot_type() == HM_PANIC){
		rdr_subtype = rdr_get_exec_subtype_value();
		if (rdr_subtype == HM_PANIC_LOWDOG || rdr_subtype == HM_PANIC_HARDDOG)
			return true;
	}
#endif
	return sh_check_cpu_panic(type);
}

static bool is_sh_l3_panic(void)
{
	enum sh_type type;
	for (type = sh_type_cluster_0; type <= sh_type_cluster_max; type++) {
		if (sh_check_cpu_panic(type))
			return true;
	}
	return false;
}

#define QIC_PERI_BUSKEY		0x400
static bool is_sh_peri_panic(void)
{
	unsigned int reboot_type;
	unsigned int excep_sub_type;
	unsigned int qic_buskey = 0;
	bool qice_intr;

	reboot_type = rdr_get_reboot_type();
	excep_sub_type = rdr_get_exec_subtype_value();
	qice_intr = check_qice_intr();
#ifdef CONFIG_DFX_SEC_QIC
	qic_buskey = dfx_hw_diag_get_qic_buskey();
#endif

	pr_info("dfx_sh check peri panic, reboot_type 0x%x:0x%x, qic_buskey 0x%X, qice_intr %u\n",
			reboot_type, excep_sub_type, qic_buskey, qice_intr);
#ifdef CONFIG_DFX_SEC_QIC_V300
	if (reboot_type == AP_S_NOC && excep_sub_type == NOC_GPU)
		return true;
#endif
	if (qice_intr)
		return true;
#ifdef CONFIG_DFX_SEC_QIC
	if (qic_buskey == QIC_PERI_BUSKEY)
		return true;
#endif
	if (reboot_type == AP_S_AWDT || reboot_type == LPM3_S_LPMCURST)
		return true;
#if defined (CONFIG_DFX_OHOS)
	if (reboot_type == HM_PANIC){
		if (excep_sub_type == HM_PANIC_LOWDOG || excep_sub_type == HM_PANIC_HARDDOG)
            return true;
    }
#endif
	return false;
}

static bool is_sh_type_panic(enum sh_type type) {
	if (type >= sh_type_cluster_0 && type <= sh_type_cluster_max)
		return is_sh_cpu_exception(type);
	switch (type) {
	case sh_type_l3:
		return is_sh_l3_panic();
	case sh_type_peri:
		return is_sh_peri_panic();
	default:
		return false;
	}
}

bool sh_check_panic(enum sh_type type)
{
	bool panic_happen = false;
 
	if (type < 0 || type >= sh_type_max)
		return false;

	panic_happen = is_sh_type_panic(type);
	if (panic_happen)
		pr_info("sh type %d panic happened\n", type);
	return panic_happen;
}

bool sh_peri_need_fast_pv(void)
{
	unsigned int qic_buskey = 0;
	bool qice_intr;

	qice_intr = check_qice_intr();
#ifdef CONFIG_DFX_SEC_QIC
	qic_buskey = dfx_hw_diag_get_qic_buskey();
#endif

	return (qice_intr || qic_buskey == QIC_PERI_BUSKEY);
}
