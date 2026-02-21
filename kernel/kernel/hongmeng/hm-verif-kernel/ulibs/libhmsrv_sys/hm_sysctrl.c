/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Implementation of sysctrl functions for ulibs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 20 11:55:23 2019
 */

#include <string.h>
#include <hmkernel/drivers/pm_cpu.h>
#include <hmkernel/power/idle.h>
#include <hmkernel/iotcmem_common.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libkev/kev_types.h>

#define ACTV_POOL_CONFIG_NR_MAX	10U
static struct __sysctrl_actv_pool_config g_actv_pool_config_large[ACTV_POOL_CONFIG_NR_MAX] = {
	{"SYSMGR-BUF-POOL", 20, 2048, 12, 256},
	{"fsmgr-bind-actv", 700, 4096, 12, 1024},
	{"fsmgr-actv", 20, 2048, 12, 256},
	{"devmgr.actv", 10, 2048, 4, 256},
	{"HMNET-ACTV-POOL-NOBIND", 40, 2048, 8, 256}
};

#ifndef __HOST_LLT__

int hm_sysctrl_read_conf(unsigned int type, void *buf, unsigned int buflen)
{
	return syscap_SysCtrlReadConf(hm_sysctrl_rref_of(), type, buf, buflen);
}

int hm_sysctrl_load_dtb(void *buf, unsigned long size, unsigned long offset)
{
	return syscap_SysCtrlLoadDTB(hm_sysctrl_rref_of(), buf, size, offset);
}

int hm_sysctrl_read_dtb(const char *path, unsigned long path_len, void *buf,
			unsigned long buf_size)
{
	return syscap_SysCtrlReadDTB(hm_sysctrl_rref_of(), path, path_len, buf, buf_size);
}

int hm_sysctrl_attach_cpuctx(void)
{
	return actvcall_hmcall_sysctrl_attach_cpuctx();
}

int hm_sysctrl_detach_cpuctx(void)
{
	return actvcall_hmcall_sysctrl_detach_cpuctx();
}

int hm_sysctrl_check_cpuctx(unsigned int *is_attached)
{
	struct __actvret_hmcall_sysctrl_check_cpuctx hmcall_ret;
	int ret;

	mem_zero_s(hmcall_ret);
	ret = actvcall_hmcall_sysctrl_check_cpuctx(&hmcall_ret);
	if (ret < 0) {
		hm_error("actvcall check cpuctx failed, err=%s\n", hmstrerror(ret));
	} else {
		if (is_attached != NULL) {
			*is_attached = hmcall_ret.is_attached;
		}
		ret = E_HM_OK;
	}
	return ret;
}
#endif

int hm_sysctrl_actv_pool_conf_of(char *pool_name, unsigned int name_len,
				 struct __sysctrl_actv_pool_config *config)
{
	int err = E_HM_OK;
	bool found = false;
	char *tmp_pool_name = NULL;
	char plat[__SYSCONF_STR_LEN_MAX] = "";

	if ((pool_name == NULL) || (config == NULL) || (name_len >= HMCAP_ACTVPOOL_NAME_SIZE)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = hm_sysctrl_read_conf(__SYSCONF_HARDWARE_PLATFORM, &plat[0], __SYSCONF_STR_LEN_MAX);
		if (err >= 0) {
			if (strcmp(&plat[0], "kirin9000") == 0) {
				err = E_HM_OK;
			} else {
				err = E_HM_POSIX_NOENT;
			}
		}
	}

	if (err == E_HM_OK) {
		for (unsigned int idx = 0; idx < ACTV_POOL_CONFIG_NR_MAX; idx++) {
			tmp_pool_name = g_actv_pool_config_large[idx].pool_name;
			if (tmp_pool_name[0] == '\0') {
				break;
			}
			if (!strncmp(pool_name, tmp_pool_name, strlen(tmp_pool_name))) {
				*config = g_actv_pool_config_large[idx];
				found = true;
				break;
			}
		}

		if (!found) {
			err = E_HM_POSIX_NOENT;
		}
	}

	return err;
}

static int cpu_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	unsigned int flags;

	flags = *(unsigned int *)data;
	return actvcall_hmcall_sysctrl_cpustat(flags, *attr);
}

int hm_sysctrl_cpu_stat(unsigned int flags, struct bunch *bunch)
{
	int err = E_HM_OK;
	struct bunch_ipc_reader_ctx ctx;
	uint32_t cnode_idx;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		cnode_idx = hmsrv_sysmgr_cnode_idx();
		ctx.ipc = cpu_stat_ipc;
		ctx.private_data = ptr_to_void(&flags);
		ctx.target_info = (uint64_t)cnode_idx;

		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_info("bunch ipc bunch request data with shm: %s\n",
				hmstrerror(err));
		}
	}

	return err;
}

int hm_sysctl_cpu_stat_of(unsigned int cpu, struct __sysctrl_cpu_stat *stat)
{
	__cpuset_t set = __cpuset_one(cpu);
	return syscap_SysCtrlGetCpuStat(hm_sysctrl_rref_of(), &set, stat);
}

int hm_sysctl_cpuset_stat_of(__cpuset_t cpuset, struct __sysctrl_cpu_stat *stat)
{
	return syscap_SysCtrlGetCpuStat(hm_sysctrl_rref_of(), &cpuset, stat);
}

int hm_sysctrl_power_off(void)
{
	return syscap_SysCtrlPowerOff(hm_sysctrl_rref_of());
}

int hm_sysctrl_get_softreboot_info(unsigned int *val)
{
	if (val == NULL) {
		return E_HM_INVAL;
	}
	return syscap_SysCtrlGetSoftRebootInfo(hm_sysctrl_rref_of(), val);
}

int hm_sysctrl_reboot(const char *reboot_str, unsigned int reboot_str_len,
		      unsigned int reboot_type)
{
	return syscap_SysCtrlReboot(hm_sysctrl_rref_of(), reboot_str, reboot_str_len, reboot_type);
}

int hm_sysctrl_sysmgr_reboot(const char *reboot_reason, unsigned int reboot_flag)
{
	return actvcall_hmcall_sysctrl_sysmgr_reboot(reboot_reason, strlen(reboot_reason), reboot_flag);
}

int hm_sysctrl_kexec(unsigned int type)
{
	return syscap_SysCtrlKexec(hm_sysctrl_rref_of(), type);
}

int hm_sysctrl_kexec_load_files(struct softreboot_info *files)
{
	return syscap_SysCtrlKexecLoadFiles(hm_sysctrl_rref_of(), files);
}

int hm_sysctrl_kexec_get_files(struct softreboot_info *files)
{
	return syscap_SysCtrlKexecGetFiles(hm_sysctrl_rref_of(), files);
}

int hm_sysctrl_kexec_unload(void)
{
	return syscap_SysCtrlKexecUnload(hm_sysctrl_rref_of());
}

int hm_sysctrl_pm_cpudown(unsigned int target_cpu, unsigned int flag)
{
	return syscap_SysCtrlPmCpuDown(hm_sysctrl_rref_of(), target_cpu, flag);
}

int hm_sysctrl_cpuaffinity(unsigned int target_cpu, unsigned int *val)
{
	return syscap_SysCtrlCpuAffinity(hm_sysctrl_rref_of(), target_cpu, val);
}

int hm_sysctrl_cpuup(unsigned int target_cpu)
{
	return syscap_SysCtrlCpuUp(hm_sysctrl_rref_of(), CPU_POWERUP_DEFAULT_MODE, target_cpu, 0UL);
}

int hm_sysctrl_bootaddr_cpuup(unsigned int boot_mode, unsigned int mpidr, unsigned long boot_addr)
{
	return syscap_SysCtrlCpuUp(hm_sysctrl_rref_of(), boot_mode, mpidr, boot_addr);
}

int hm_sysctrl_pm_suspend(unsigned int index, unsigned int *wakeup_irq)
{
	return syscap_SysCtrlPmSuspend(hm_sysctrl_rref_of(), index, wakeup_irq);
}

int hm_sysctrl_pm_idle(unsigned int index, struct __idle_sys_stat *sys_stat)
{
	return sysfast_pm_idle_enter(index, sys_stat);
}

ssize_t hm_sysctrl_kev_read_data(unsigned int ring_buffer_index, void *buf, size_t size, uint64_t *head)
{
	return (ssize_t)actvcall_hmcall_sysctrl_kev_read_data(ring_buffer_index, buf, size, head);
}

int hm_sysctrl_kev_set_water(unsigned int ring_buffer_index, uint64_t water)
{
	return actvcall_hmcall_sysctrl_kev_set_water(ring_buffer_index, water);
}

int hm_sysctrl_kvic_kev_source_bind(unsigned int ring_buffer_index, unsigned int source_id, unsigned int flags)
{
	return actvcall_hmcall_sysctrl_kvic_kev_source_bind(ring_buffer_index, source_id, flags);
}

int hm_sysctrl_kvic_kev_source_unbind(unsigned int ring_buffer_idx, unsigned int source_id)
{
	return actvcall_hmcall_sysctrl_kvic_kev_source_unbind(ring_buffer_idx, source_id);
}

unsigned int hm_sysctrl_respart_read_nodenum(void)
{
	int ret;
	unsigned long node_num;

	ret = syscap_SysCtrlReadConf(hm_sysctrl_rref_of(), __SYSCONF_RESPART_NODE_NUM,
					   &node_num, sizeof(node_num));
	if (ret < 0) {
		node_num = 0UL;
	}
	return (unsigned int)node_num;
}

int hm_sysctrl_respart_read_nodeinfo(unsigned int nodeid,
				     struct __res_node_info_s *node_info)
{
	int ret = E_HM_OK;

	if (nodeid < 1U || nodeid > NR_RES_NODE) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		node_info->nodeid = nodeid;
		ret = syscap_SysCtrlReadConf(hm_sysctrl_rref_of(), __SYSCONF_RESPART_NODE_INFO,
					     node_info, sizeof(struct __res_node_info_s));
		if (ret == (int)sizeof(struct __res_node_info_s)) {
			ret = E_HM_OK;
		}
	}

	return ret;
}

int hm_sysctrl_respart_monitor_enable(struct hm_respart_partition part)
{
	struct __res_partition_info_s p;
	int ret = E_HM_OK;

	if (part.nodeid < 1U || part.nodeid > NR_RES_NODE) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		p.partid = part.partid;
		p.nodeid = part.nodeid;
		ret = syscap_SysCtrlRespartCtrl(hm_sysctrl_rref_of(), &p,
						RES_PARTITION_MONITOR_ENABLE,
						NULL, 0);
	}
	return ret;
}

int hm_sysctrl_respart_monitor_disable(struct hm_respart_partition part)
{
	struct __res_partition_info_s p;
	int ret = E_HM_OK;

	if (part.nodeid < 1U || part.nodeid > NR_RES_NODE) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		p.partid = part.partid;
		p.nodeid = part.nodeid;
		ret = syscap_SysCtrlRespartCtrl(hm_sysctrl_rref_of(), &p,
						RES_PARTITION_MONITOR_DISABLE,
						NULL, 0);
	}
	return ret;
}

int hm_sysctrl_respart_monitor_capture(struct hm_respart_partition part,
				       unsigned int *val)
{
	struct __res_partition_info_s p;
	int ret = E_HM_OK;

	if (part.nodeid < 1U || part.nodeid > NR_RES_NODE) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		p.partid = part.partid;
		p.nodeid = part.nodeid;
		ret = syscap_SysCtrlRespartCtrl(hm_sysctrl_rref_of(), &p,
						RES_PARTITION_MONITOR_CAPTURE,
						val, sizeof(unsigned int));
	}
	return ret;
}

int hm_sysctrl_respart_read_partinfo(struct hm_respart_partition part,
				     struct __res_partition_info_s *part_info)
{
	struct __res_partition_info_s p;
	int ret = E_HM_OK;

	if (part.nodeid < 1U || part.nodeid > NR_RES_NODE) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		p.partid = part.partid;
		p.nodeid = part.nodeid;
		ret = syscap_SysCtrlRespartCtrl(hm_sysctrl_rref_of(), &p,
						RES_PARTITION_GET_INFO,
						part_info, sizeof(struct __res_partition_info_s));
	}
	return ret;
}

int hm_sysctrl_inject_ras_error(unsigned int cmd, unsigned long param1,
				unsigned long param2)
{
	return syscap_SysCtrlInjectRasError(hm_sysctrl_rref_of(), cmd, param1, param2);
}

int hm_sysctrl_kev_set_block_info(enum kev_type type, cref_t vspace_cref, void *uaddr)
{
	int err = E_HM_INVAL;

	err = actvcall_hmcall_sysctrl_kev_set_block_info((unsigned int)type,
							 vspace_cref, uaddr);
	if (err < 0) {
		hm_warn("kev_type: %u, set block info failed: %s\n",
				(unsigned int)type, hmstrerror(err));
	}

	return err;
}

int hm_sysctrl_ekbox_read(void *buf, unsigned int buflen)
{
	return actvcall_hmcall_sysctrl_ekbox_read(buf, buflen);
}

int hm_sysctrl_register_lsyscall_server(unsigned int server, cref_t actvpool)
{
	return syscap_SysCtrlRegisterLsyscallServer(hm_sysctrl_rref_of(), server, actvpool);
}

int hm_sysctrl_unregister_lsyscall_server(unsigned int server)
{
	return syscap_SysCtrlUnRegisterLsyscallServer(hm_sysctrl_rref_of(), server);
}
