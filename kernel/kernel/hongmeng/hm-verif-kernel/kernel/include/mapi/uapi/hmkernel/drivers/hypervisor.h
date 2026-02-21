/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: General macros and structs of hypervisor
 * Author: IT Software Infrastructure Lab, Munich Research Center
 * Create: Thu May 23 15:57:23 2019
 */

#ifndef UAPI_HMKERNEL_DRIVERS_HYPERVISOR_H
#define UAPI_HMKERNEL_DRIVERS_HYPERVISOR_H

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

#define CAP_VM_SIZE	(PAGE_SIZE * 4U)

#define MAX_VCPUS		16U
#define MAX_DEV_GROUPS		8U
#define MAX_DEV_IO_REGS		128U
#define MAX_REG_SEG_NUM		2U
#define NO_DEV_GROUP_MASK	0x8000U

#define HM_VCPU_EVENT_VMEXIT	0x0U
#define HM_VCPU_EVENT_ERROR	0x1U
#define HM_VCPU_EVENT_SUSPEND	0x2U

#define HYP_VCPU_INVALID		0
#define HYP_VCPU_SETUP			1
#define HYP_VCPU_GETREG			2
#define HYP_VCPU_SETREG			3
#define HYP_VCPU_GET_CTX		4
#define HYP_VCPU_SET_CTX		5
#define HYP_VCPU_GET_SVE_CTX		6
#define HYP_VCPU_SET_SVE_CTX		7
#define HYP_VCPU_RESET			8
#define HYP_VGIC_ASSERT_IRQ		9
#define HYP_VCPU_START			10
#define HYP_VCPU_STOP			11
#define HYP_VCPU_DEBUG_SETUP		12 /* 0: disable, others: enable */
#define HYP_VCPU_GET_DEBUG_INFO		13
#define HYP_VCPU_SETIRQ_ATTRIBUTE	14
#define HYP_VCPU_GET_VIRTUAL_TIME	15
#define HYP_VCPU_SET_VIRTUAL_TIME	16
#define HYP_VCPU_PAUSE			17 /* use START to unpause vcpu */
#define HYP_VCPU_SET_TIMEOUT		18
#define HYP_VCPU_PVTIME_ENABLE		19
#define HYP_VCPU_SET_TRAP_WFI		20
#define HYP_VCPU_GET_VIRTUAL_TIMEOFF	21
#define HYP_VCPU_SET_TRAP_WFE		22

#define HYP_VCPU_ONLY_CHANGE_VCPU_STATE		0ULL
#define HYP_VCPU_CHANGE_VCPU_STATE_WITH_THREAD	1ULL

#define HYP_MEMCONF_NORMAL_MEM	(1UL)
#define HYP_MEMCONF_UVMM_ACCESS	(1UL << 1)
#define HYP_MEMCONF_SHARE_MEM	(1UL << 2)
#define HYP_MEMCONF_RESERVE_MEM (1UL << 3)
#define HYP_MEMCONF_PVTIME_MEM	(1UL << 4)
#define HYP_MEMCONF_SMMU_EXTRA_MEM	(1UL << 5)
#define HYP_MEMCONF_SMMU_EXTRA_MEM_EXEC	(1UL << 6)
#define HYP_MEMCONF_READONLY	(1UL << 7)
#define HYP_MEMCONF_MEM_REGISTER_MASK (0x80)

#define HYP_MEMPROT_READ (1UL)
#define HYP_MEMPROT_WRITE (1UL << 1)
#define HYP_MEMPROT_UEXEC (1UL << 2)
#define HYP_MEMPROT_KEXEC (1UL << 3)
#define HYP_MEMPROT_MASK_ALL (0xF)

/*
 * return value for event channel hvc call
 */
#define __HYP_EC_SUCCESS	(0)
#define __HYP_EC_ERR_INVAL	(-1)
#define __HYP_EC_ERR_AGAIN	(-2)
#define __HYP_EC_ERR_DENIED	(-3)
#define __HYP_EC_ERR_OVERFLOW	(-4)
#define __HYP_EC_ERR_FAULT	(-5)
#define __HYP_EC_ERR_INTERNAL	(-6)

#define HYP_VM_DISABLE_TRAP_WFX 0ULL
#define HYP_VM_ENABLE_TRAP_WFX 1ULL

struct __hyp_vcpu_config_setup {
	cref_t vm;
	__u8 is_aarch32;
	__u8 enable_trap_wfi;
	__u8 enable_trap_wfe;
};

struct __hyp_vcpu_config_setreg {
	__u64 reg;
	__u64 val;
};

struct __hyp_vcpu_debug_info {
	__u32 brp_nr;
	__u32 wrp_nr;
};

struct __vmexit_info {
	__u64 exit_event;
};

enum __hyp_mr_type {
	__MMIO_TYPE_MMIO_TRIGGER,
#ifdef CONFIG_MMIO_DISPATCHER_KICK_TYPE
	__MMIO_TYPE_MMIO_KICK,
#endif
	__MMIO_TYPE_MMIO_ACTV,
#ifdef CONFIG_HYPERVISOR_VIRTUAL_SMMU
	__MMIO_TYPE_MMIO_VSMMU,
#endif
	__MMIO_TYPE_MAX,
};

enum __hyp_disp_type {
	__DISPATCHER_TYPE_MMIO_TRIGGER,
#ifdef CONFIG_MMIO_DISPATCHER_KICK_TYPE
	__DISPATCHER_TYPE_MMIO_KICK,
#endif
	__DISPATCHER_TYPE_MMIO_ACTV,
	__DISPATCHER_TYPE_MAX,
};

enum __hyp_vm_map_iomem_type {
	__VM_MAP_IOMEM_TYPE_HOST,
	__VM_MAP_IOMEM_TYPE_GUEST_MMU,
	__VM_MAP_IOMEM_TYPE_GUEST_SMMU,
	__VM_MAP_IOMEM_TYPE_UNMAP,
	__VM_MAP_IOMEM_TYPE_MAX,
};

/* Keep compatible with the ioctl command format */
#define __HMV_IOC(a, b, c, d)	(((a) << 30) | ((b) << 8) | (c) | ((d) << 16))

#define __HMV_IOC_NONE		0U
#define __HMV_IOC_WRITE		1U
#define __HMV_IOC_READ		2U

#define __HMV_IO(a, b)		__HMV_IOC(__HMV_IOC_NONE, (a), (b), 0)
#define __HMV_IOW(a, b, c)	__HMV_IOC(__HMV_IOC_WRITE, (a), (b), sizeof(c))
#define __HMV_IOR(a, b, c)	__HMV_IOC(__HMV_IOC_READ, (a), (b), sizeof(c))
#define __HMV_IOWR(a, b, c)	__HMV_IOC(__HMV_IOC_READ | \
								__HMV_IOC_WRITE, (a), (b), sizeof(c))

/* We use kvm magic in order to not conflict with other ioctl cmds from linux */
#define HMVIO 0xAE

struct __hmv_new_vcpu {
	struct __hyp_vcpu_config_setup vcpu_conf;
	/* return */
	cref_t thread_cref;
	int thread_tid;
	cref_t vcpu_ctx;
};
#define HMV_NEW_VCPU	__HMV_IOWR(HMVIO, 0, struct __hmv_new_vcpu)

struct __hmv_delete_vcpu {
	cref_t thread_cref;
	cref_t vcpu_ctx;
};
#define HMV_DELETE_VCPU	__HMV_IOW(HMVIO, 1, struct __hmv_delete_vcpu)

struct __hmv_new_vm {
	unsigned short dev_group_id;
	__u32 vpmu_mode;
	__u64 vgic_addr;
	/* return */
	cref_t vm_cref;
};
#define HMV_NEW_VM	__HMV_IOWR(HMVIO, 2, struct __hmv_new_vm)
#define HMV_DELETE_VM	__HMV_IO(HMVIO, 3)

struct __hmv_register_region {
	cref_t vm_cref;
	unsigned long gpa;
	unsigned long hva;
	unsigned long size;
	unsigned long flags;
};
#define HMV_REGISTER_REGION	__HMV_IOW(HMVIO, 4, struct __hmv_register_region)

struct __hmv_unregister_region {
	cref_t vm_cref;
	unsigned long gpa;
	unsigned long size;
};
#define HMV_UNREGISTER_REGION	__HMV_IOW(HMVIO, 5, struct __hmv_unregister_region)

#define HMV_VMEXIT_NEW_ACTVPOOL	__HMV_IOR(HMVIO, 6, cref_t)
#define HMV_VMEXIT_DEL_ACTVPOOL	__HMV_IOW(HMVIO, 7, cref_t)

#define HYP_VM_VPMU_PASSTHROUGH_MODE	0U
#define HYP_VM_VPMU_EMULATE_MODE	1U

#endif
