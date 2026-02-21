/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of VM captype
 * Author: IT Software Infrastructure Lab, Munich Research Center
 * Create: Thu Feb 7 16:53:23 2019
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(VM, 12, CAPFINEGRANT(VM)
		CAPNOGRANTMOVE(VM)
		CAPNODEFAULTCREF(VM)
		CAPRAWCREFRLOOKUP(VM)
		CAPDEFAULTSIZE(VM),
	CAPMETHOD(VM, GetVcpuStructSize)
	CAPMETHOD(VM, GetIORegNum)
	CAPMETHOD(VM, GetIORegs)
	CAPMETHOD(VM, RegisterIOReg)
	CAPMETHOD(VM, UnRegisterIOReg)
	CAPMETHOD(VM, BindVcpuActvpool)
	CAPMETHOD(VM, UnBindVcpuActvpool)
	CAPMETHOD(VM, BindDispatcherActvpool)
	CAPMETHOD(VM, CreateVnotify)
	CAPMETHOD(VM, VnotifyCreateChannel)
	CAPMETHOD(VM, VnotifyDestroyChannel)
	CAPMETHOD(VM, VnotifyConfig)
	CAPMETHOD(VM, GetNumVcpus)
	CAPMETHOD(VM, VperfctxConfig)
	CAPMETHOD(VM, VperfctxAttach)
	CAPMETHOD(VM, VperfctxDetach)
	CAPMETHOD(VM, RegisterMMIODispatcher)
	CAPMETHOD(VM, BindVcpuMMIODispatcher)
	CAPMETHOD(VM, AssertIrq)
	CAPMETHOD(VM, AssertLpi)
	CAPMETHOD(VM, AssertFiq)
	CAPMETHOD(VM, ResetVsmmu)
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_VM_API_EXPORTED
#define CAP_VM_API_EXPORTED

struct sysarg_vm_config {
	cref_t vm_vspace;
	__u64 pgd_smmu;
	__u64 vgic_addr;
	__u32 vpmu_mode;
	__u16 dev_group_id;
};

struct sysarg_vm_vnotify_info {
	__u32 irq;
	__u64 addr;
	__u64 size;
};

struct vm_ioreg_s {
	__u64 paddr;
	__u64 vaddr;
	__u64 size;
	__u8 mem_config;
	__u8 mem_prot;
};

enum {
	CRIGHT_VM_OWNER_BIT,
	CRIGHT_VM_USER_BIT,
	CRIGHT_VM_MAX
};

#define CRIGHT_VM_OWNER		(1U << CRIGHT_VM_OWNER_BIT)
#define CRIGHT_VM_USER		(1U << CRIGHT_VM_USER_BIT)

#endif
#endif
