obj-y := hm_mem.o hm_ashm.o hm_kshm.o hm_procmgr.o hm_signal.o hmsrv_sys.o \
	 hm_mqueue.o hm_path.o hm_thread.o \
	 hm_timer.o hm_timer_kernel_space.o \
	 hm_sysctrl.o hm_syssrv_loadinfo.o hm_timer_utils.o hm_futex.o \
	 hm_security.o hm_hmsignal.o hm_module.o \
	 hm_select_common.o hm_ctrlmem_kobjctnr.o hm_ipc_shm.o hm_log.o \
	 hm_ipc_msg.o hm_ipc_sem.o hm_dump.o hm_atfork.o hm_ct_task.o \
	 hm_init.o hm_watchdog.o hm_timer_utils_public.o hm_proc_affinity.o \
	 hm_idle_state.o hm_vdso.o hm_shmec.o hm_event.o \
	 hm_ctrlmem_syscalls_layer.o hm_livepatch.o hm_hkip.o
obj-$(CONFIG_SYSMGR_FAULT_INJECT) += hm_inject_fault.o
obj-$(CONFIG_HYPERVISOR) += hm_uvmm.o hm_vm.o

CFLAGS_hm_signal.o += -D_GNU_SOURCE -Wno-strict-prototypes
CFLAGS_hm_procmgr.o += -D_GNU_SOURCE -Wno-strict-prototypes
CFLAGS_hm_mem.o += -D_GNU_SOURCE -Wno-strict-prototypes
CFLAGS_hm_ipc_msg.o += -D_GNU_SOURCE -Wno-strict-prototypes
