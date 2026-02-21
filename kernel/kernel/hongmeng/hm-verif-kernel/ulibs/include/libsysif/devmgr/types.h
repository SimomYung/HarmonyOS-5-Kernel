/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Type definations for devmgr sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef LIBSYSIF_DEVMGR_TYPES_H
#define LIBSYSIF_DEVMGR_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <hmkernel/const.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/capability.h>

enum devmgr_device_mgmt_id_cmd {
	DEVMGR_DEVICE_ID_ALLOC,
	DEVMGR_DEVICE_ID_FREE,
	DEVMGR_DEVICE_ID_OPS_MAX,
};

/* Global ID resource types related to the device driver */
enum devmgr_device_mgmt_id_type {
	DEVMGR_DEVICE_ID_SD, /* sd index  */
	DEVMGR_DEVICE_ID_MAX,
};

enum devmgr_mgmt_target_type {
	DM_MGMT_TARGET_DEVID,
	DM_MGMT_TARGET_CNODEIDX,
};

struct devmgr_mgmt_target {
	enum devmgr_mgmt_target_type type;
	union {
		unsigned long long reserved;
		unsigned int cnode_idx;
		int devid;
	};
};

struct devmgr_device_data {
	unsigned int umode;
	unsigned int uid;
	unsigned int gid;
	char compatible[];
};

#define DM_PM_SUSPEND				__U(0)
#define DM_PM_RESUME				__U(1)
#define DM_PM_SUSPEND_LATE			__U(2)
#define DM_PM_RESUME_EARLY			__U(3)
#define DM_PM_SUSPEND_DISK_PREPARE		__U(4)
#define DM_PM_SUSPEND_DISK_POST			__U(5)
#define DM_PM_SUSPEND_DISK_RESTORE_PREPARE	__U(6)
#define DM_PM_SUSPEND_DISK_POST_RESTORE		__U(7)
#define DM_PM_SUSPEND_DISK_FREEZE		__U(8)
#define DM_PM_SUSPEND_DISK_QUIESCE		__U(9)
#define DM_PM_SUSPEND_DISK_THAW			__U(10)
#define DM_PM_SUSPEND_DISK_RECOVER		__U(11)
#define DM_PM_SUSPEND_DISK_RESTORE		__U(12)

#define DH_PM_IDLE_MASK				__U(0x10000000)
#define DH_PM_IDLE_PREPARE			(DH_PM_IDLE_MASK | __U(0))
#define DH_PM_IDLE_ENTER			(DH_PM_IDLE_MASK | __U(1))
#define DH_PM_IDLE_EXIT				(DH_PM_IDLE_MASK | __U(2))

#define DH_PM_INVALID				__U(-1)
#define DH_PM_DEV_SHUTDOWN			__U(0)
#define DH_PM_SYSCORE_SHUTDOWN			__U(1)
#define DH_PM_SUSPEND_PREPARE_FORCE		__U(4)
#define DH_PM_SUSPEND_PREPARE_NORMAL		__U(5)
#define DH_PM_SUSPEND				__U(6)
#define DH_PM_SUSPEND_LATE			__U(7)
#define DH_PM_RESUME_EARLY			__U(8)
#define DH_PM_RESUME				__U(9)
#define DH_PM_RESUME_FINISH			__U(10)
#define DH_PM_ULSR_ENTER			__U(11)
#define DH_PM_ULSR_LEAVE			__U(12)
#define DH_PM_SUSPEND_DISK_PREPARE		__U(13)
#define DH_PM_SUSPEND_DISK_POST			__U(14)
#define DH_PM_SUSPEND_DISK_RESTORE_PREPARE	__U(15)
#define DH_PM_SUSPEND_DISK_POST_RESTORE		__U(16)
#define DH_PM_SUSPEND_DISK_FREEZE		__U(17)
#define DH_PM_SUSPEND_DISK_QUIESCE		__U(18)
#define DH_PM_SUSPEND_DISK_THAW			__U(19)
#define DH_PM_SUSPEND_DISK_RECOVER		__U(20)
#define DH_PM_SUSPEND_DISK_RESTORE		__U(21)
#define DH_PM_WAIT_FOR_DEVICE_PROBE		__U(22)
#define DH_PM_PROC_FREEZE			__U(23)
#define DH_PM_PROC_THAW				__U(24)
#define DH_PM_WQ_FREEZE				__U(25)
#define DH_PM_WQ_THAW				__U(26)

#define DH_PM_SHUTDOWN_PREPARE		__U(0)
#define DH_PM_REBOOT_PREPARE		__U(1)

#define DH_PM_CPU_ADD			__U(0)
#define DH_PM_CPU_REMOVE		__U(1)

#define DH_PM_THERMAL_DEVICE_ACQUIRE		__U(0)
#define DH_PM_THERMAL_DEVICE_TEMP_READ		__U(1)

#define DH_PM_THERMAL_DEVICE_TYPE_LEN		20

struct thermal_action_arg {
	union {
		char type[DH_PM_THERMAL_DEVICE_TYPE_LEN];
		int id;
	};
};

#define O_NO_OPEN	0x80000000
#define O_FASTPATH	0x40000000

#define MODULE_NAME_LEN		32U

#define MODULE_ARGS_LEN		128U

#define DRIVER_NAME_LEN		32U

#define DRIVER_ARGS_LEN		128U

#define DEVICE_NAME_LEN		128U

#define MAJOR_NAME_LEN		32U

#define ISOLATE_GROUP_NAME_LEN	32U

#define DEVICE_CAN_PROBE		__U(1)
#define DEVICE_MUST_ISOLATE		(__U(1) << 1)
#define DEVICE_AUTO_LOAD		(__U(1) << 2)
#define DEVICE_CAN_DIRECTMAP_SHM	(__U(1) << 3)
#define DEVICE_FASTPATH			(__U(1) << 4)
/* the driver poll function of this device must be safe in the atomic context */
#define DEVICE_SAFEPOLL			(__U(1) << 5)
#define DEVICE_ANONYMOUS		(__U(1) << 6)
/* for mmap */
#define DEVICE_NONVAPRE			(__U(1) << 7)

#define DEVICE_CTX_PROXY		(__U(1) << 16)
#define DEVICE_STATE_IN_DEVFS		(__U(1) << 31)

static inline bool device_flag_test(unsigned int flags, unsigned int flag)
{
	return (flags & flag) != 0U;
}

/* devhost identity flags */
#define DH_FLAGS_STANDALONE	(__U(1) << 31)
#define DH_FLAGS_ENABLE_BACKEND	(__U(1) << 30)

static inline bool dh_flag_test(unsigned int flags, unsigned int flag)
{
	return (flags & flag) != 0U;
}

/* used in net */
#define MAC_ADDR_LEN		16
struct mac_addr {
	char sa_data[MAC_ADDR_LEN];
};

/* mount info of sb, root dentry and root node */
struct transfs_m_info_t {
	unsigned int sb_index;
	unsigned int root_d_index;
	unsigned int root_v_index;
	unsigned int root_lnkcnt;
	size_t per_write_size;
	unsigned long long filp;
	size_t block_size;
	uint32_t s_time_gran;
	int64_t s_max_time;
	int64_t s_min_time;
	uint32_t trans_dev_id;
	bool s_reverse_query;
};

/* dentry and node info */
struct transfs_dn_info_t {
	unsigned int d_index;
	unsigned int v_index;
	uint32_t ino;
};

struct transfs_attr_t {
	unsigned long long blocks;
	unsigned long blksize;
};

struct netdev_status {
	unsigned long long	total_rx;
	unsigned long long	total_tx;
	unsigned long long	total_alloc;
	unsigned long long	total_free;

	unsigned long long	rx_in;
	unsigned long long	tx_in;
	unsigned long long	drv_alloc;
	unsigned long long	drv_free;
	unsigned long long	devhost_alloc;
	unsigned long long	devhost_free;
	unsigned long long	full_drop;
	unsigned long long	deact_drop;
	unsigned long long	null_drop;
};

/* used in fops_ioctl */
#define DEVHOST_IOCTL_MAX_DATA		256 /* DEPRECATED */

#define DEVMGR_IRQ_HWCONF_LEN_MAX	8
#define DEVMGR_IRQ_MAX_NR_CPUS		8

struct drvcall_devmgr_irq_map_arg {
	unsigned int oirq_offset;
	unsigned int hwconf_len;
	unsigned int hwconf[DEVMGR_IRQ_HWCONF_LEN_MAX];
};

struct drvcall_devmgr_irq_fetch_arg {
	unsigned int virt_irq;
	unsigned int rrefs_nr;
	bool is_sgi;
	unsigned int cpu;
	rref_t irqctrl_rrefs[DEVMGR_IRQ_MAX_NR_CPUS];
};

struct drvcall_devmgr_irq_fetch_rlt {
	unsigned int irqs_nr;
	unsigned int affinity;
	unsigned int hwconf_len;
	unsigned int hwconf[DEVMGR_IRQ_HWCONF_LEN_MAX];
	unsigned int logic_irqs[DEVMGR_IRQ_MAX_NR_CPUS];
};

struct drvcall_devmgr_irq_return_arg {
	unsigned int virt_irq;
	unsigned int rrefs_nr;
	bool is_sgi;
	unsigned int cpu;
	rref_t irqctrl_rrefs[DEVMGR_IRQ_MAX_NR_CPUS];
};

struct drvcall_devmgr_irq_thread_arg {
	int tid;
	__cpuset_t cpu_mask;
};

enum load_module_type {
	DM_MGMT_INTERNAL_MODULE, /* located in /lib/modules directory */
	DM_MGMT_EXTERNAL_MODULE,
	DM_MGMT_SYSCALL_MODULE,
	DM_MGMT_TYPE_INVALID,
};

struct load_module_arg {
	const char *mod_buf;
	size_t mod_size;
};

bool __devmgr_type_is_valid_name(const char *name, unsigned int maxlen);
bool __devmgr_type_is_valid_filename(const char *name, unsigned int maxlen);

static inline bool is_valid_module_name(const char *name)
{
	return __devmgr_type_is_valid_filename(name, MODULE_NAME_LEN);
}

static inline bool is_valid_driver_name(const char *name)
{
	return __devmgr_type_is_valid_filename(name, DRIVER_NAME_LEN);
}

static inline bool is_valid_device_name(const char *name)
{
	return __devmgr_type_is_valid_name(name, DEVICE_NAME_LEN);
}

static inline bool is_valid_major_name(const char *name)
{
	return __devmgr_type_is_valid_name(name, MAJOR_NAME_LEN);
}

static inline bool is_valid_isolate_group_name(const char *name)
{
	return __devmgr_type_is_valid_name(name, ISOLATE_GROUP_NAME_LEN);
}

static inline unsigned int pm_cpuhp_rollback_action(unsigned int action)
{
	return (action == DH_PM_CPU_REMOVE) ? DH_PM_CPU_ADD : DH_PM_CPU_REMOVE;
}
#endif	/* LIBSYSIF_DEVMGR_TYPES_H */
