subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/pwrmgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/freqmgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/hguard
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/auditmgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/srvmgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/devmgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/crypt
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/uvmm
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/fs
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/sysmgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/keymgr
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/devhost
subdir-ccflags-y += -I$(HMSDKSYSIFSERVER)/devhost
subdir-ccflags-y += -I$(HMSDKSYSIFCLIENT)/hmtracemgr
subdir-ccflags-y += -I$(HMSDKSYSIFSERVER)/hmtracemgr
subdir-ccflags-y += -I$(HMSDKSYSIFBASE)
subdir-ccflags-y += -I$(HMSDKINCLUDE)
subdir-ccflags-$(CONFIG_FS_SAFETY_READ) += -I$(HMSDKINCLUDE)/crypto
subdir-ccflags-$(CONFIG_FS_SAFETY_READ) += -I$(HMSDKINCLUDE)/bsl
subdir-ccflags-y += -D_XOPEN_SOURCE=700
subdir-ccflags-y += $(SECURITY_CFLAGS)
subdir-ccflags-$(CONFIG_AARCH64) += -funwind-tables
subdir-ccflags-$(CONFIG_DLIST_DEBUG) += -DDLIST_DEBUG_ENABLE

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

obj-y += libvsyscall/ libhmlog/ libpanic/ libmem/ \
	 libhmsync/ libsysif/ libcrt/ libhmucap/ \
	 libalgo/ libperf/ klibs/ libprocfs/ \
	 libhmsrv_sys/ libhmshmalloc/ libbunch/ libhmirq/ \
	 libhmpm/ librb/ libself_rel/ libhmactv/ libhmtrace/ libhmtrace_ng \
	 libhmuev/ libhmelf/ libkbox/ libkev/ libtrack_info/ \
	 libhguard/ libhmsrv_crypt/ libhmebr/ libpreempt/ libfdtable/

obj-y += libvpipe/ libhmiov/ libvhost_user/ \
	 libhmfaultevt/ libsysevent/
obj-$(CONFIG_KEYMGR) += libhmsrv_key/
obj-$(CONFIG_FS_SAFETY_READ) += libhmcert/
obj-$(CONFIG_HMVSYSCALL_TRACE) += libstrace/
obj-$(CONFIG_UCFI) += libucfi/
obj-$(CONFIG_HMPSF) += libhmpsf/
obj-$(CONFIG_HMPROBE) += libhmprobe/
