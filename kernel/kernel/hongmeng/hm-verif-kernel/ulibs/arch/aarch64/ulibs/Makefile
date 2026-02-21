hostprogs += gen_cap_numbers
HOSTCFLAGS_gen_cap_numbers.o := -I$(srctree)/kernel/include/mapi/uapi -D__aarch64__

ifneq ($(CONFIG_COMPAT),y)
subdir-asflags-y += -D__sysif_server_devhost_no_compat32_handlers__ \
		    -D__sysif_server_hmtracemgr_no_compat32_handlers__
subdir-ccflags-y += -D__sysif_server_devhost_no_compat32_handlers__ \
		    -D__sysif_server_hmtracemgr_no_compat32_handlers__
endif

ifneq ($(CONFIG_ILP32),y)
subdir-asflags-y += -D__sysif_server_devhost_no_ilp32_handlers__ \
		    -D__sysif_server_hmtracemgr_no_ilp32_handlers__
subdir-ccflags-y += -D__sysif_server_devhost_no_ilp32_handlers__ \
		    -D__sysif_server_hmtracemgr_no_ilp32_handlers__
endif

obj-y := syscall.o actv_rpc.o actv_pure_rpc.o actv_emrg_entry.o sigcontext.o lsyscall.o libhmtrace/
$(obj)/syscall.o: $(obj)/cap_numbers.h

clean-files := cap_numbers.h

quiet_cmd_build_cap_numbers = CAP_NUMBER $@
      cmd_build_cap_numbers = $(obj)/gen_cap_numbers > $@

$(obj)/cap_numbers.h: $(obj)/gen_cap_numbers
	$(call cmd,build_cap_numbers)

.PHONY: $(obj)/cap_numbers.h

targets += cap_numbers.h
