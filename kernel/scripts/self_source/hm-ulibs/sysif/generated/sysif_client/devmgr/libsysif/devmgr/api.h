/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for devmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/devmgr/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/devmgr/libsysif/devmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/devmgr/libsysif/devmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/devmgr/libsysif/devmgr/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:27:09 2025
*/
#ifndef LIBSYSIF_DEVMGR_API_H
#define LIBSYSIF_DEVMGR_API_H
#ifndef LIBSYSIF_DEVMGR_COMMON_H
#define LIBSYSIF_DEVMGR_COMMON_H
#include <libsysif/utils_common.h>
#include <hongmeng/compiler.h>
#include <hongmeng/types.h>
#include <hmasm/actv_rpc.h>
#ifndef LIBSYSIF_BASE_API_COMMON_H
#define LIBSYSIF_BASE_API_COMMON_H
#include <stddef.h>
#include <stdint.h>
#include <hmkernel/capability.h>
struct sysif_reply_header {
	int32_t retval;
	uint32_t msgsz;
};

#define SYSIF_REPLY_LEN_MAX 256UL
extern void* hm_actvpool_prepare_ret(unsigned long long sender, unsigned long credential, unsigned long size);


extern struct arch_actv_local* hm_actv_current_arch_actv_local(void);


#endif
#include <libhmpm/shutdown.h>
#include <libhmpm/wl_stat.h>
#include <libbunch/bunch_ipc.h>
#include <libsysif/devmgr/types.h>

#define DEVMGR_PATH	"devmgr.actv"


#define RESOURCE_COUNT_MAX    128

struct devhost_boot_resource {
	unsigned long long bootfdt_shm_id;
	unsigned int bootfdt_fwtype;
};

raw_static_assert(sizeof(uref_t) <= 96,
                    drvcall_devmgr_core_register_devhost_arg_ap_uref_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_core_register_devhost_arg_group_is_too_large);
raw_static_assert(sizeof(uref_t) <= 96,
                    drvcall_devmgr_core_register_devhost_arg_devmgr_ap_uref_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_core_device_add_arg_devname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_core_device_add_arg_devnode_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_core_device_add_arg_group_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_core_request_module_arg_modname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_core_request_module_arg_args_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_chrdev_register_region_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_blkdev_register_region_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_blkdev_unregister_region_arg_name_is_too_large);
raw_static_assert(sizeof(rref_t) <= 96,
                    drvcall_devmgr_fops_open_arg_tgt_rref_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_mgmt_load_module_arg_modname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_mgmt_load_module_arg_modargs_is_too_large);
raw_static_assert(sizeof(struct devmgr_mgmt_target) <= 96,
                    drvcall_devmgr_mgmt_load_module_arg_target_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_mgmt_unload_module_arg_modname_is_too_large);
raw_static_assert(sizeof(struct devmgr_mgmt_target) <= 96,
                    drvcall_devmgr_mgmt_unload_module_arg_target_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    drvcall_devmgr_mgmt_dump_arg_attr_is_too_large);
raw_static_assert(sizeof(struct drvcall_devmgr_irq_map_arg) <= 96,
                    drvcall_devmgr_irq_map_arg_arg_is_too_large);
raw_static_assert(sizeof(struct drvcall_devmgr_irq_map_arg) <= 96,
                    drvcall_devmgr_irq_translate_arg_arg_is_too_large);
raw_static_assert(sizeof(struct drvcall_devmgr_irq_fetch_arg) <= 96,
                    drvcall_devmgr_irq_fetch_arg_arg_is_too_large);
raw_static_assert(sizeof(struct drvcall_devmgr_irq_return_arg) <= 96,
                    drvcall_devmgr_irq_return_arg_arg_is_too_large);
raw_static_assert(sizeof(struct drvcall_devmgr_irq_thread_arg) <= 96,
                    drvcall_devmgr_irq_thread_setaffinity_arg_arg_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_net_register_arg_name_is_too_large);
raw_static_assert(sizeof(rref_t) <= 96,
                    drvcall_devmgr_net_open_arg_tgt_rref_is_too_large);
raw_static_assert(sizeof(struct reboot_cmd) <= 96,
                    drvcall_devmgr_pm_reboot_arg_cmd_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_procfs_create_data_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_procfs_create_data_arg_ops_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_procfs_create_data_arg_linkname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_procfs_unlink_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_create_dir_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_create_file_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_create_file_ex_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_create_symlink_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_create_symlink_arg_linkname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_delete_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_notify_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_read_arg_actv_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_write_arg_actv_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_rename_link_arg_old_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devmgr_sysfs_rename_link_arg_new_name_is_too_large);
struct __actvret_drvcall_devmgr_core_register_devhost {
	struct devhost_boot_resource resource;
};

struct __actvret_drvcall_devmgr_core_devhost_init_done {
};

struct __actvret_drvcall_devmgr_core_device_add {
	int devid;
	unsigned int dev_flags;
};

struct __actvret_drvcall_devmgr_core_device_del {
};

struct __actvret_drvcall_devmgr_core_request_module {
};

struct __actvret_drvcall_devmgr_chrdev_register_region {
	unsigned int major;
};

struct __actvret_drvcall_devmgr_chrdev_unregister_region {
};

struct __actvret_drvcall_devmgr_blkdev_register_region {
	unsigned int major;
};

struct __actvret_drvcall_devmgr_blkdev_unregister_region {
};

struct __actvret_drvcall_devmgr_fops_open {
	unsigned int fastpath;
	unsigned long long filp;
	unsigned long long kshm_key;
	unsigned long kobj_offset;
	unsigned int nonvapre;
	unsigned int dev_fops_flags;
};

struct __actvret_drvcall_devmgr_device_mgmt_index {
	int index;
};

struct __actvret_drvcall_devmgr_mgmt_load_module {
};

struct __actvret_drvcall_devmgr_mgmt_unload_module {
};

struct __actvret_drvcall_devmgr_mgmt_sync {
};

struct __actvret_drvcall_devmgr_mgmt_dump {
	unsigned int pos_end;
};

struct __actvret_drvcall_devmgr_mgmt_init_module {
};

struct __actvret_drvcall_devmgr_mgmt_finit_module {
};

struct __actvret_drvcall_devmgr_mgmt_delete_module {
};

struct __actvret_drvcall_devmgr_irq_map {
	unsigned int virt_irq;
};

struct __actvret_drvcall_devmgr_irq_translate {
	unsigned int virt_irq;
};

struct __actvret_drvcall_devmgr_irq_fetch {
	struct drvcall_devmgr_irq_fetch_rlt rlt;
};

struct __actvret_drvcall_devmgr_irq_return {
};

struct __actvret_drvcall_devmgr_irq_thread_setaffinity {
};

struct __actvret_drvcall_devmgr_net_alloc_ifindex {
	int ifindex;
};

struct __actvret_drvcall_devmgr_net_register {
	int ifindex;
};

struct __actvret_drvcall_devmgr_net_unregister {
};

struct __actvret_drvcall_devmgr_net_hmnetd_ready {
};

struct __actvret_drvcall_devmgr_net_open {
};

struct __actvret_drvcall_devmgr_pm_action {
};

struct __actvret_drvcall_devmgr_pm_power_off {
};

struct __actvret_drvcall_devmgr_pm_reboot {
};

struct __actvret_drvcall_devmgr_pm_cpuhp_action {
};

struct __actvret_drvcall_devmgr_procfs_create_data {
};

struct __actvret_drvcall_devmgr_procfs_unlink {
};

struct __actvret_drvcall_devmgr_procfs_read {
};

struct __actvret_drvcall_devmgr_procfs_write {
};

struct __actvret_drvcall_devmgr_sysfs_create_dir {
};

struct __actvret_drvcall_devmgr_sysfs_create_file {
};

struct __actvret_drvcall_devmgr_sysfs_create_file_ex {
};

struct __actvret_drvcall_devmgr_sysfs_create_symlink {
};

struct __actvret_drvcall_devmgr_sysfs_delete {
	int tagid;
};

struct __actvret_drvcall_devmgr_sysfs_notify {
};

struct __actvret_drvcall_devmgr_sysfs_create_batch {
};

struct __actvret_drvcall_devmgr_sysfs_read {
};

struct __actvret_drvcall_devmgr_sysfs_write {
};

struct __actvret_drvcall_devmgr_sysfs_poll {
};

struct __actvret_drvcall_devmgr_sysfs_rename_link {
};

struct __actvret_drvcall_devmgr_sysfs_open {
	unsigned long long fd;
};

struct __actvret_drvcall_devmgr_sysfs_close {
};

struct __actvret_drvcall_devmgr_fw_acquire_contents {
	unsigned int rsize;
};

struct __actvret_drvcall_devmgr_fw_cms_verify {
};

struct __actvret_drvcall_devmgr_sec_mgmt {
};

struct __actvret_drvcall_devmgr_sec_sync_fastpath_dev {
};

struct __actvret_drvcall_devmgr_pac_cfi {
};

#ifdef __aarch64__
union __actvret_drvcall_devmgr {
	struct __actvret_drvcall_devmgr_core_register_devhost core_register_devhost;
	struct __actvret_drvcall_devmgr_core_devhost_init_done core_devhost_init_done;
	struct __actvret_drvcall_devmgr_core_device_add core_device_add;
	struct __actvret_drvcall_devmgr_core_device_del core_device_del;
	struct __actvret_drvcall_devmgr_core_request_module core_request_module;
	struct __actvret_drvcall_devmgr_chrdev_register_region chrdev_register_region;
	struct __actvret_drvcall_devmgr_chrdev_unregister_region chrdev_unregister_region;
	struct __actvret_drvcall_devmgr_blkdev_register_region blkdev_register_region;
	struct __actvret_drvcall_devmgr_blkdev_unregister_region blkdev_unregister_region;
	struct __actvret_drvcall_devmgr_fops_open fops_open;
	struct __actvret_drvcall_devmgr_device_mgmt_index device_mgmt_index;
	struct __actvret_drvcall_devmgr_mgmt_load_module mgmt_load_module;
	struct __actvret_drvcall_devmgr_mgmt_unload_module mgmt_unload_module;
	struct __actvret_drvcall_devmgr_mgmt_sync mgmt_sync;
	struct __actvret_drvcall_devmgr_mgmt_dump mgmt_dump;
	struct __actvret_drvcall_devmgr_mgmt_init_module mgmt_init_module;
	struct __actvret_drvcall_devmgr_mgmt_finit_module mgmt_finit_module;
	struct __actvret_drvcall_devmgr_mgmt_delete_module mgmt_delete_module;
	struct __actvret_drvcall_devmgr_irq_map irq_map;
	struct __actvret_drvcall_devmgr_irq_translate irq_translate;
	struct __actvret_drvcall_devmgr_irq_fetch irq_fetch;
	struct __actvret_drvcall_devmgr_irq_return irq_return;
	struct __actvret_drvcall_devmgr_irq_thread_setaffinity irq_thread_setaffinity;
	struct __actvret_drvcall_devmgr_net_alloc_ifindex net_alloc_ifindex;
	struct __actvret_drvcall_devmgr_net_register net_register;
	struct __actvret_drvcall_devmgr_net_unregister net_unregister;
	struct __actvret_drvcall_devmgr_net_hmnetd_ready net_hmnetd_ready;
	struct __actvret_drvcall_devmgr_net_open net_open;
	struct __actvret_drvcall_devmgr_pm_action pm_action;
	struct __actvret_drvcall_devmgr_pm_power_off pm_power_off;
	struct __actvret_drvcall_devmgr_pm_reboot pm_reboot;
	struct __actvret_drvcall_devmgr_pm_cpuhp_action pm_cpuhp_action;
	struct __actvret_drvcall_devmgr_procfs_create_data procfs_create_data;
	struct __actvret_drvcall_devmgr_procfs_unlink procfs_unlink;
	struct __actvret_drvcall_devmgr_procfs_read procfs_read;
	struct __actvret_drvcall_devmgr_procfs_write procfs_write;
	struct __actvret_drvcall_devmgr_sysfs_create_dir sysfs_create_dir;
	struct __actvret_drvcall_devmgr_sysfs_create_file sysfs_create_file;
	struct __actvret_drvcall_devmgr_sysfs_create_file_ex sysfs_create_file_ex;
	struct __actvret_drvcall_devmgr_sysfs_create_symlink sysfs_create_symlink;
	struct __actvret_drvcall_devmgr_sysfs_delete sysfs_delete;
	struct __actvret_drvcall_devmgr_sysfs_notify sysfs_notify;
	struct __actvret_drvcall_devmgr_sysfs_create_batch sysfs_create_batch;
	struct __actvret_drvcall_devmgr_sysfs_read sysfs_read;
	struct __actvret_drvcall_devmgr_sysfs_write sysfs_write;
	struct __actvret_drvcall_devmgr_sysfs_poll sysfs_poll;
	struct __actvret_drvcall_devmgr_sysfs_rename_link sysfs_rename_link;
	struct __actvret_drvcall_devmgr_sysfs_open sysfs_open;
	struct __actvret_drvcall_devmgr_sysfs_close sysfs_close;
	struct __actvret_drvcall_devmgr_fw_acquire_contents fw_acquire_contents;
	struct __actvret_drvcall_devmgr_fw_cms_verify fw_cms_verify;
	struct __actvret_drvcall_devmgr_sec_mgmt sec_mgmt;
	struct __actvret_drvcall_devmgr_sec_sync_fastpath_dev sec_sync_fastpath_dev;
	struct __actvret_drvcall_devmgr_pac_cfi pac_cfi;
};

#elif defined(__arm__)
union __actvret_drvcall_devmgr {
	struct __actvret_drvcall_devmgr_core_register_devhost core_register_devhost;
	struct __actvret_drvcall_devmgr_core_devhost_init_done core_devhost_init_done;
	struct __actvret_drvcall_devmgr_core_device_add core_device_add;
	struct __actvret_drvcall_devmgr_core_device_del core_device_del;
	struct __actvret_drvcall_devmgr_core_request_module core_request_module;
	struct __actvret_drvcall_devmgr_chrdev_register_region chrdev_register_region;
	struct __actvret_drvcall_devmgr_chrdev_unregister_region chrdev_unregister_region;
	struct __actvret_drvcall_devmgr_blkdev_register_region blkdev_register_region;
	struct __actvret_drvcall_devmgr_blkdev_unregister_region blkdev_unregister_region;
	struct __actvret_drvcall_devmgr_fops_open fops_open;
	struct __actvret_drvcall_devmgr_device_mgmt_index device_mgmt_index;
	struct __actvret_drvcall_devmgr_mgmt_load_module mgmt_load_module;
	struct __actvret_drvcall_devmgr_mgmt_unload_module mgmt_unload_module;
	struct __actvret_drvcall_devmgr_mgmt_sync mgmt_sync;
	struct __actvret_drvcall_devmgr_mgmt_dump mgmt_dump;
	struct __actvret_drvcall_devmgr_mgmt_init_module mgmt_init_module;
	struct __actvret_drvcall_devmgr_mgmt_finit_module mgmt_finit_module;
	struct __actvret_drvcall_devmgr_mgmt_delete_module mgmt_delete_module;
	struct __actvret_drvcall_devmgr_irq_map irq_map;
	struct __actvret_drvcall_devmgr_irq_translate irq_translate;
	struct __actvret_drvcall_devmgr_irq_fetch irq_fetch;
	struct __actvret_drvcall_devmgr_irq_return irq_return;
	struct __actvret_drvcall_devmgr_irq_thread_setaffinity irq_thread_setaffinity;
	struct __actvret_drvcall_devmgr_net_alloc_ifindex net_alloc_ifindex;
	struct __actvret_drvcall_devmgr_net_register net_register;
	struct __actvret_drvcall_devmgr_net_unregister net_unregister;
	struct __actvret_drvcall_devmgr_net_hmnetd_ready net_hmnetd_ready;
	struct __actvret_drvcall_devmgr_net_open net_open;
	struct __actvret_drvcall_devmgr_pm_action pm_action;
	struct __actvret_drvcall_devmgr_pm_power_off pm_power_off;
	struct __actvret_drvcall_devmgr_pm_reboot pm_reboot;
	struct __actvret_drvcall_devmgr_pm_cpuhp_action pm_cpuhp_action;
	struct __actvret_drvcall_devmgr_procfs_create_data procfs_create_data;
	struct __actvret_drvcall_devmgr_procfs_unlink procfs_unlink;
	struct __actvret_drvcall_devmgr_procfs_read procfs_read;
	struct __actvret_drvcall_devmgr_procfs_write procfs_write;
	struct __actvret_drvcall_devmgr_sysfs_create_dir sysfs_create_dir;
	struct __actvret_drvcall_devmgr_sysfs_create_file sysfs_create_file;
	struct __actvret_drvcall_devmgr_sysfs_create_file_ex sysfs_create_file_ex;
	struct __actvret_drvcall_devmgr_sysfs_create_symlink sysfs_create_symlink;
	struct __actvret_drvcall_devmgr_sysfs_delete sysfs_delete;
	struct __actvret_drvcall_devmgr_sysfs_notify sysfs_notify;
	struct __actvret_drvcall_devmgr_sysfs_create_batch sysfs_create_batch;
	struct __actvret_drvcall_devmgr_sysfs_read sysfs_read;
	struct __actvret_drvcall_devmgr_sysfs_write sysfs_write;
	struct __actvret_drvcall_devmgr_sysfs_poll sysfs_poll;
	struct __actvret_drvcall_devmgr_sysfs_rename_link sysfs_rename_link;
	struct __actvret_drvcall_devmgr_sysfs_open sysfs_open;
	struct __actvret_drvcall_devmgr_sysfs_close sysfs_close;
	struct __actvret_drvcall_devmgr_fw_acquire_contents fw_acquire_contents;
	struct __actvret_drvcall_devmgr_fw_cms_verify fw_cms_verify;
	struct __actvret_drvcall_devmgr_sec_mgmt sec_mgmt;
	struct __actvret_drvcall_devmgr_sec_sync_fastpath_dev sec_sync_fastpath_dev;
	struct __actvret_drvcall_devmgr_pac_cfi pac_cfi;
};

#else
union __actvret_drvcall_devmgr {
	struct __actvret_drvcall_devmgr_core_register_devhost core_register_devhost;
	struct __actvret_drvcall_devmgr_core_devhost_init_done core_devhost_init_done;
	struct __actvret_drvcall_devmgr_core_device_add core_device_add;
	struct __actvret_drvcall_devmgr_core_device_del core_device_del;
	struct __actvret_drvcall_devmgr_core_request_module core_request_module;
	struct __actvret_drvcall_devmgr_chrdev_register_region chrdev_register_region;
	struct __actvret_drvcall_devmgr_chrdev_unregister_region chrdev_unregister_region;
	struct __actvret_drvcall_devmgr_blkdev_register_region blkdev_register_region;
	struct __actvret_drvcall_devmgr_blkdev_unregister_region blkdev_unregister_region;
	struct __actvret_drvcall_devmgr_fops_open fops_open;
	struct __actvret_drvcall_devmgr_device_mgmt_index device_mgmt_index;
	struct __actvret_drvcall_devmgr_mgmt_load_module mgmt_load_module;
	struct __actvret_drvcall_devmgr_mgmt_unload_module mgmt_unload_module;
	struct __actvret_drvcall_devmgr_mgmt_sync mgmt_sync;
	struct __actvret_drvcall_devmgr_mgmt_dump mgmt_dump;
	struct __actvret_drvcall_devmgr_mgmt_init_module mgmt_init_module;
	struct __actvret_drvcall_devmgr_mgmt_finit_module mgmt_finit_module;
	struct __actvret_drvcall_devmgr_mgmt_delete_module mgmt_delete_module;
	struct __actvret_drvcall_devmgr_irq_map irq_map;
	struct __actvret_drvcall_devmgr_irq_translate irq_translate;
	struct __actvret_drvcall_devmgr_irq_fetch irq_fetch;
	struct __actvret_drvcall_devmgr_irq_return irq_return;
	struct __actvret_drvcall_devmgr_irq_thread_setaffinity irq_thread_setaffinity;
	struct __actvret_drvcall_devmgr_net_alloc_ifindex net_alloc_ifindex;
	struct __actvret_drvcall_devmgr_net_register net_register;
	struct __actvret_drvcall_devmgr_net_unregister net_unregister;
	struct __actvret_drvcall_devmgr_net_hmnetd_ready net_hmnetd_ready;
	struct __actvret_drvcall_devmgr_net_open net_open;
	struct __actvret_drvcall_devmgr_pm_action pm_action;
	struct __actvret_drvcall_devmgr_pm_power_off pm_power_off;
	struct __actvret_drvcall_devmgr_pm_reboot pm_reboot;
	struct __actvret_drvcall_devmgr_pm_cpuhp_action pm_cpuhp_action;
	struct __actvret_drvcall_devmgr_procfs_create_data procfs_create_data;
	struct __actvret_drvcall_devmgr_procfs_unlink procfs_unlink;
	struct __actvret_drvcall_devmgr_procfs_read procfs_read;
	struct __actvret_drvcall_devmgr_procfs_write procfs_write;
	struct __actvret_drvcall_devmgr_sysfs_create_dir sysfs_create_dir;
	struct __actvret_drvcall_devmgr_sysfs_create_file sysfs_create_file;
	struct __actvret_drvcall_devmgr_sysfs_create_file_ex sysfs_create_file_ex;
	struct __actvret_drvcall_devmgr_sysfs_create_symlink sysfs_create_symlink;
	struct __actvret_drvcall_devmgr_sysfs_delete sysfs_delete;
	struct __actvret_drvcall_devmgr_sysfs_notify sysfs_notify;
	struct __actvret_drvcall_devmgr_sysfs_create_batch sysfs_create_batch;
	struct __actvret_drvcall_devmgr_sysfs_read sysfs_read;
	struct __actvret_drvcall_devmgr_sysfs_write sysfs_write;
	struct __actvret_drvcall_devmgr_sysfs_poll sysfs_poll;
	struct __actvret_drvcall_devmgr_sysfs_rename_link sysfs_rename_link;
	struct __actvret_drvcall_devmgr_sysfs_open sysfs_open;
	struct __actvret_drvcall_devmgr_sysfs_close sysfs_close;
	struct __actvret_drvcall_devmgr_fw_acquire_contents fw_acquire_contents;
	struct __actvret_drvcall_devmgr_fw_cms_verify fw_cms_verify;
	struct __actvret_drvcall_devmgr_sec_mgmt sec_mgmt;
	struct __actvret_drvcall_devmgr_sec_sync_fastpath_dev sec_sync_fastpath_dev;
	struct __actvret_drvcall_devmgr_pac_cfi pac_cfi;
};

#endif
enum __devmgr_mgr {
	__devmgr_INVAL_mgr=0,
	__devmgr_core_mgr,
	__devmgr_chrdev_mgr,
	__devmgr_blkdev_mgr,
	__devmgr_fops_mgr,
	__devmgr_device_mgmt_mgr,
	__devmgr_mgmt_mgr,
	__devmgr_irq_mgr,
	__devmgr_net_mgr,
	__devmgr_pm_mgr,
	__devmgr_procfs_mgr,
	__devmgr_sysfs_mgr,
	__devmgr_fw_mgr,
	__devmgr_sec_mgr,
	__devmgr_MAX_mgr,
};

#define __devmgr_method_MIN __ACTV_HIGHTABLE_START
#define __devmgr_method_mgmt_init_module (__ACTV_HIGHTABLE_START + 1)
#define __devmgr_method_mgmt_finit_module (__ACTV_HIGHTABLE_START + 2)
#define __devmgr_method_mgmt_delete_module (__ACTV_HIGHTABLE_START + 3)
#define __devmgr_method_core_register_devhost (__ACTV_HIGHTABLE_START + 4)
#define __devmgr_method_core_devhost_init_done (__ACTV_HIGHTABLE_START + 5)
#define __devmgr_method_core_device_add (__ACTV_HIGHTABLE_START + 6)
#define __devmgr_method_core_device_del (__ACTV_HIGHTABLE_START + 7)
#define __devmgr_method_core_request_module (__ACTV_HIGHTABLE_START + 8)
#define __devmgr_method_core_rsvd_3 (__ACTV_HIGHTABLE_START + 9)
#define __devmgr_method_core_rsvd_2 (__ACTV_HIGHTABLE_START + 10)
#define __devmgr_method_core_rsvd_1 (__ACTV_HIGHTABLE_START + 11)
#define __devmgr_method_chrdev_register_region (__ACTV_HIGHTABLE_START + 12)
#define __devmgr_method_chrdev_unregister_region (__ACTV_HIGHTABLE_START + 13)
#define __devmgr_method_chrdev_rsvd_2 (__ACTV_HIGHTABLE_START + 14)
#define __devmgr_method_chrdev_rsvd_1 (__ACTV_HIGHTABLE_START + 15)
#define __devmgr_method_blkdev_register_region (__ACTV_HIGHTABLE_START + 16)
#define __devmgr_method_blkdev_unregister_region (__ACTV_HIGHTABLE_START + 17)
#define __devmgr_method_blkdev_rsvd_2 (__ACTV_HIGHTABLE_START + 18)
#define __devmgr_method_blkdev_rsvd_1 (__ACTV_HIGHTABLE_START + 19)
#define __devmgr_method_fops_open (__ACTV_HIGHTABLE_START + 20)
#define __devmgr_method_fops_rsvd_1 (__ACTV_HIGHTABLE_START + 21)
#define __devmgr_method_device_mgmt_index (__ACTV_HIGHTABLE_START + 22)
#define __devmgr_method_mgmt_load_module (__ACTV_HIGHTABLE_START + 23)
#define __devmgr_method_mgmt_unload_module (__ACTV_HIGHTABLE_START + 24)
#define __devmgr_method_mgmt_sync (__ACTV_HIGHTABLE_START + 25)
#define __devmgr_method_mgmt_dump (__ACTV_HIGHTABLE_START + 26)
#define __devmgr_method_mgmt_rsvd_1 (__ACTV_HIGHTABLE_START + 27)
#define __devmgr_method_irq_map (__ACTV_HIGHTABLE_START + 28)
#define __devmgr_method_irq_translate (__ACTV_HIGHTABLE_START + 29)
#define __devmgr_method_irq_fetch (__ACTV_HIGHTABLE_START + 30)
#define __devmgr_method_irq_return (__ACTV_HIGHTABLE_START + 31)
#define __devmgr_method_irq_thread_setaffinity (__ACTV_HIGHTABLE_START + 32)
#define __devmgr_method_net_alloc_ifindex (__ACTV_HIGHTABLE_START + 33)
#define __devmgr_method_net_register (__ACTV_HIGHTABLE_START + 34)
#define __devmgr_method_net_unregister (__ACTV_HIGHTABLE_START + 35)
#define __devmgr_method_net_hmnetd_ready (__ACTV_HIGHTABLE_START + 36)
#define __devmgr_method_net_open (__ACTV_HIGHTABLE_START + 37)
#define __devmgr_method_net_rsvd_3 (__ACTV_HIGHTABLE_START + 38)
#define __devmgr_method_net_rsvd_2 (__ACTV_HIGHTABLE_START + 39)
#define __devmgr_method_net_rsvd_1 (__ACTV_HIGHTABLE_START + 40)
#define __devmgr_method_pm_action (__ACTV_HIGHTABLE_START + 41)
#define __devmgr_method_pm_power_off (__ACTV_HIGHTABLE_START + 42)
#define __devmgr_method_pm_reboot (__ACTV_HIGHTABLE_START + 43)
#define __devmgr_method_pm_cpuhp_action (__ACTV_HIGHTABLE_START + 44)
#define __devmgr_method_procfs_create_data (__ACTV_HIGHTABLE_START + 45)
#define __devmgr_method_procfs_unlink (__ACTV_HIGHTABLE_START + 46)
#define __devmgr_method_procfs_read (__ACTV_HIGHTABLE_START + 47)
#define __devmgr_method_procfs_write (__ACTV_HIGHTABLE_START + 48)
#define __devmgr_method_sysfs_create_dir (__ACTV_HIGHTABLE_START + 49)
#define __devmgr_method_sysfs_create_file (__ACTV_HIGHTABLE_START + 50)
#define __devmgr_method_sysfs_create_file_ex (__ACTV_HIGHTABLE_START + 51)
#define __devmgr_method_sysfs_create_symlink (__ACTV_HIGHTABLE_START + 52)
#define __devmgr_method_sysfs_delete (__ACTV_HIGHTABLE_START + 53)
#define __devmgr_method_sysfs_notify (__ACTV_HIGHTABLE_START + 54)
#define __devmgr_method_sysfs_create_batch (__ACTV_HIGHTABLE_START + 55)
#define __devmgr_method_sysfs_read (__ACTV_HIGHTABLE_START + 56)
#define __devmgr_method_sysfs_write (__ACTV_HIGHTABLE_START + 57)
#define __devmgr_method_sysfs_poll (__ACTV_HIGHTABLE_START + 58)
#define __devmgr_method_sysfs_rename_link (__ACTV_HIGHTABLE_START + 59)
#define __devmgr_method_sysfs_open (__ACTV_HIGHTABLE_START + 60)
#define __devmgr_method_sysfs_close (__ACTV_HIGHTABLE_START + 61)
#define __devmgr_method_sysfs_rsvd_3 (__ACTV_HIGHTABLE_START + 62)
#define __devmgr_method_sysfs_rsvd_2 (__ACTV_HIGHTABLE_START + 63)
#define __devmgr_method_sysfs_rsvd_1 (__ACTV_HIGHTABLE_START + 64)
#define __devmgr_method_fw_acquire_contents (__ACTV_HIGHTABLE_START + 65)
#define __devmgr_method_fw_cms_verify (__ACTV_HIGHTABLE_START + 66)
#define __devmgr_method_sec_mgmt (__ACTV_HIGHTABLE_START + 67)
#define __devmgr_method_sec_sync_fastpath_dev (__ACTV_HIGHTABLE_START + 68)
#define __devmgr_method_pac_cfi (__ACTV_HIGHTABLE_START + 69)
#define __devmgr_method_MAX (__ACTV_HIGHTABLE_START + 70)
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_core_register_devhost) < 512,
            drvcall_devmgr_core_register_devhost_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_core_devhost_init_done) < 512,
            drvcall_devmgr_core_devhost_init_done_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_core_device_add) < 512,
            drvcall_devmgr_core_device_add_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_core_device_del) < 512,
            drvcall_devmgr_core_device_del_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_core_request_module) < 512,
            drvcall_devmgr_core_request_module_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_chrdev_register_region) < 512,
            drvcall_devmgr_chrdev_register_region_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_chrdev_unregister_region) < 512,
            drvcall_devmgr_chrdev_unregister_region_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_blkdev_register_region) < 512,
            drvcall_devmgr_blkdev_register_region_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_blkdev_unregister_region) < 512,
            drvcall_devmgr_blkdev_unregister_region_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_fops_open) < 512,
            drvcall_devmgr_fops_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_device_mgmt_index) < 512,
            drvcall_devmgr_device_mgmt_index_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_load_module) < 512,
            drvcall_devmgr_mgmt_load_module_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_unload_module) < 512,
            drvcall_devmgr_mgmt_unload_module_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_sync) < 512,
            drvcall_devmgr_mgmt_sync_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_dump) < 512,
            drvcall_devmgr_mgmt_dump_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_init_module) < 512,
            drvcall_devmgr_mgmt_init_module_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_finit_module) < 512,
            drvcall_devmgr_mgmt_finit_module_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_mgmt_delete_module) < 512,
            drvcall_devmgr_mgmt_delete_module_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_irq_map) < 512,
            drvcall_devmgr_irq_map_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_irq_translate) < 512,
            drvcall_devmgr_irq_translate_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_irq_fetch) < 512,
            drvcall_devmgr_irq_fetch_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_irq_return) < 512,
            drvcall_devmgr_irq_return_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_irq_thread_setaffinity) < 512,
            drvcall_devmgr_irq_thread_setaffinity_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_net_alloc_ifindex) < 512,
            drvcall_devmgr_net_alloc_ifindex_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_net_register) < 512,
            drvcall_devmgr_net_register_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_net_unregister) < 512,
            drvcall_devmgr_net_unregister_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_net_hmnetd_ready) < 512,
            drvcall_devmgr_net_hmnetd_ready_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_net_open) < 512,
            drvcall_devmgr_net_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_pm_action) < 512,
            drvcall_devmgr_pm_action_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_pm_power_off) < 512,
            drvcall_devmgr_pm_power_off_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_pm_reboot) < 512,
            drvcall_devmgr_pm_reboot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_pm_cpuhp_action) < 512,
            drvcall_devmgr_pm_cpuhp_action_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_procfs_create_data) < 512,
            drvcall_devmgr_procfs_create_data_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_procfs_unlink) < 512,
            drvcall_devmgr_procfs_unlink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_procfs_read) < 512,
            drvcall_devmgr_procfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_procfs_write) < 512,
            drvcall_devmgr_procfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_create_dir) < 512,
            drvcall_devmgr_sysfs_create_dir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_create_file) < 512,
            drvcall_devmgr_sysfs_create_file_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_create_file_ex) < 512,
            drvcall_devmgr_sysfs_create_file_ex_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_create_symlink) < 512,
            drvcall_devmgr_sysfs_create_symlink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_delete) < 512,
            drvcall_devmgr_sysfs_delete_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_notify) < 512,
            drvcall_devmgr_sysfs_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_create_batch) < 512,
            drvcall_devmgr_sysfs_create_batch_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_read) < 512,
            drvcall_devmgr_sysfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_write) < 512,
            drvcall_devmgr_sysfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_poll) < 512,
            drvcall_devmgr_sysfs_poll_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_rename_link) < 512,
            drvcall_devmgr_sysfs_rename_link_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_open) < 512,
            drvcall_devmgr_sysfs_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sysfs_close) < 512,
            drvcall_devmgr_sysfs_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_fw_acquire_contents) < 512,
            drvcall_devmgr_fw_acquire_contents_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_fw_cms_verify) < 512,
            drvcall_devmgr_fw_cms_verify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sec_mgmt) < 512,
            drvcall_devmgr_sec_mgmt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_sec_sync_fastpath_dev) < 512,
            drvcall_devmgr_sec_sync_fastpath_dev_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devmgr_pac_cfi) < 512,
            drvcall_devmgr_pac_cfi_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_drvcall_devmgr) < 512,
        drvcall_devmgr_too_many_actvret_data);
extern int __actvcall_drvcall_devmgr_core_register_devhost(unsigned long long __rpc_info, void *__buf, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref);


extern int __actvcapcall_drvcall_devmgr_core_register_devhost(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_core_register_devhost(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref, struct __actvret_drvcall_devmgr_core_register_devhost *__ret)
{
	const unsigned long __callno = __devmgr_method_core_register_devhost;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_core_register_devhost(rpc_info, __ret, flags, ap_uref, group, devmgr_ap_uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_core_register_devhost(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref, struct __actvret_drvcall_devmgr_core_register_devhost *__ret)
{
	const unsigned long __callno = __devmgr_method_core_register_devhost;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_core_register_devhost(__rref, rpc_info, __ret, flags, ap_uref, group, devmgr_ap_uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_core_register_devhost(unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref, struct __actvret_drvcall_devmgr_core_register_devhost *__ret)
{
	return actvxactcall_drvcall_devmgr_core_register_devhost(false, false, flags, ap_uref, group, devmgr_ap_uref, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_core_register_devhost(rref_t __rref, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref, struct __actvret_drvcall_devmgr_core_register_devhost *__ret)
{
	return actvxactcapcall_drvcall_devmgr_core_register_devhost(false, false, __rref, flags, ap_uref, group, devmgr_ap_uref, __ret);
}

extern int __actvcall_drvcall_devmgr_core_devhost_init_done(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_core_devhost_init_done(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_core_devhost_init_done(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devmgr_method_core_devhost_init_done;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_core_devhost_init_done(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_core_devhost_init_done(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devmgr_method_core_devhost_init_done;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_core_devhost_init_done(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_core_devhost_init_done(void)
{
	return actvxactcall_drvcall_devmgr_core_devhost_init_done(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_core_devhost_init_done(rref_t __rref)
{
	return actvxactcapcall_drvcall_devmgr_core_devhost_init_done(false, false, __rref);
}

extern int __actvcall_drvcall_devmgr_core_device_add(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group);


extern int __actvcapcall_drvcall_devmgr_core_device_add(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_core_device_add(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devmgr_core_device_add *__ret)
{
	const unsigned long __callno = __devmgr_method_core_device_add;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_core_device_add(rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), devname, devnode_name, parent, flags, devt, vfs_mode, group);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_core_device_add(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devmgr_core_device_add *__ret)
{
	const unsigned long __callno = __devmgr_method_core_device_add;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_core_device_add(__rref, rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), devname, devnode_name, parent, flags, devt, vfs_mode, group);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_core_device_add(struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devmgr_core_device_add *__ret)
{
	return actvxactcall_drvcall_devmgr_core_device_add(false, false, devname, devnode_name, parent, flags, devt, vfs_mode, group, ___void_buf, ___void_buf_sz, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_core_device_add(rref_t __rref, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devmgr_core_device_add *__ret)
{
	return actvxactcapcall_drvcall_devmgr_core_device_add(false, false, __rref, devname, devnode_name, parent, flags, devt, vfs_mode, group, ___void_buf, ___void_buf_sz, __ret);
}

extern int __actvcall_drvcall_devmgr_core_device_del(unsigned long long __rpc_info, void *__buf, int devid);


extern int __actvcapcall_drvcall_devmgr_core_device_del(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_core_device_del(bool is_xact_fwd, bool is_xact_allow_refwd, int devid)
{
	const unsigned long __callno = __devmgr_method_core_device_del;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_core_device_del(rpc_info, NULL, devid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_core_device_del(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int devid)
{
	const unsigned long __callno = __devmgr_method_core_device_del;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_core_device_del(__rref, rpc_info, NULL, devid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_core_device_del(int devid)
{
	return actvxactcall_drvcall_devmgr_core_device_del(false, false, devid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_core_device_del(rref_t __rref, int devid)
{
	return actvxactcapcall_drvcall_devmgr_core_device_del(false, false, __rref, devid);
}

extern int __actvcall_drvcall_devmgr_core_request_module(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct hm_actv_buf args);


extern int __actvcapcall_drvcall_devmgr_core_request_module(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct hm_actv_buf args);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_core_request_module(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf modname, struct hm_actv_buf args)
{
	const unsigned long __callno = __devmgr_method_core_request_module;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_core_request_module(rpc_info, NULL, modname, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_core_request_module(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf modname, struct hm_actv_buf args)
{
	const unsigned long __callno = __devmgr_method_core_request_module;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_core_request_module(__rref, rpc_info, NULL, modname, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_core_request_module(struct hm_actv_buf modname, struct hm_actv_buf args)
{
	return actvxactcall_drvcall_devmgr_core_request_module(false, false, modname, args);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_core_request_module(rref_t __rref, struct hm_actv_buf modname, struct hm_actv_buf args)
{
	return actvxactcapcall_drvcall_devmgr_core_request_module(false, false, __rref, modname, args);
}

extern int __actvcall_drvcall_devmgr_chrdev_register_region(unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name);


extern int __actvcapcall_drvcall_devmgr_chrdev_register_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_chrdev_register_region(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_chrdev_register_region *__ret)
{
	const unsigned long __callno = __devmgr_method_chrdev_register_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_chrdev_register_region(rpc_info, __ret, major, baseminor, minorct, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_chrdev_register_region(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_chrdev_register_region *__ret)
{
	const unsigned long __callno = __devmgr_method_chrdev_register_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_chrdev_register_region(__rref, rpc_info, __ret, major, baseminor, minorct, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_chrdev_register_region(unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_chrdev_register_region *__ret)
{
	return actvxactcall_drvcall_devmgr_chrdev_register_region(false, false, major, baseminor, minorct, name, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_chrdev_register_region(rref_t __rref, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_chrdev_register_region *__ret)
{
	return actvxactcapcall_drvcall_devmgr_chrdev_register_region(false, false, __rref, major, baseminor, minorct, name, __ret);
}

extern int __actvcall_drvcall_devmgr_chrdev_unregister_region(unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct);


extern int __actvcapcall_drvcall_devmgr_chrdev_unregister_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_chrdev_unregister_region(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int major, unsigned int baseminor, int minorct)
{
	const unsigned long __callno = __devmgr_method_chrdev_unregister_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_chrdev_unregister_region(rpc_info, NULL, major, baseminor, minorct);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_chrdev_unregister_region(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int major, unsigned int baseminor, int minorct)
{
	const unsigned long __callno = __devmgr_method_chrdev_unregister_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_chrdev_unregister_region(__rref, rpc_info, NULL, major, baseminor, minorct);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_chrdev_unregister_region(unsigned int major, unsigned int baseminor, int minorct)
{
	return actvxactcall_drvcall_devmgr_chrdev_unregister_region(false, false, major, baseminor, minorct);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_chrdev_unregister_region(rref_t __rref, unsigned int major, unsigned int baseminor, int minorct)
{
	return actvxactcapcall_drvcall_devmgr_chrdev_unregister_region(false, false, __rref, major, baseminor, minorct);
}

extern int __actvcall_drvcall_devmgr_blkdev_register_region(unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name);


extern int __actvcapcall_drvcall_devmgr_blkdev_register_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_blkdev_register_region(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int major, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_blkdev_register_region *__ret)
{
	const unsigned long __callno = __devmgr_method_blkdev_register_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_blkdev_register_region(rpc_info, __ret, major, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_blkdev_register_region(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int major, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_blkdev_register_region *__ret)
{
	const unsigned long __callno = __devmgr_method_blkdev_register_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_blkdev_register_region(__rref, rpc_info, __ret, major, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_blkdev_register_region(unsigned int major, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_blkdev_register_region *__ret)
{
	return actvxactcall_drvcall_devmgr_blkdev_register_region(false, false, major, name, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_blkdev_register_region(rref_t __rref, unsigned int major, struct hm_actv_buf name, struct __actvret_drvcall_devmgr_blkdev_register_region *__ret)
{
	return actvxactcapcall_drvcall_devmgr_blkdev_register_region(false, false, __rref, major, name, __ret);
}

extern int __actvcall_drvcall_devmgr_blkdev_unregister_region(unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name);


extern int __actvcapcall_drvcall_devmgr_blkdev_unregister_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_blkdev_unregister_region(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int major, struct hm_actv_buf name)
{
	const unsigned long __callno = __devmgr_method_blkdev_unregister_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_blkdev_unregister_region(rpc_info, NULL, major, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_blkdev_unregister_region(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int major, struct hm_actv_buf name)
{
	const unsigned long __callno = __devmgr_method_blkdev_unregister_region;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_blkdev_unregister_region(__rref, rpc_info, NULL, major, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_blkdev_unregister_region(unsigned int major, struct hm_actv_buf name)
{
	return actvxactcall_drvcall_devmgr_blkdev_unregister_region(false, false, major, name);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_blkdev_unregister_region(rref_t __rref, unsigned int major, struct hm_actv_buf name)
{
	return actvxactcapcall_drvcall_devmgr_blkdev_unregister_region(false, false, __rref, major, name);
}

extern int __actvcall_drvcall_devmgr_fops_open(unsigned long long __rpc_info, void *__buf, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref);


extern int __actvcapcall_drvcall_devmgr_fops_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_fops_open(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref, struct __actvret_drvcall_devmgr_fops_open *__ret)
{
	const unsigned long __callno = __devmgr_method_fops_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_fops_open(rpc_info, __ret, devno, vfs_flags, vfs_mode, tgt_rref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_fops_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref, struct __actvret_drvcall_devmgr_fops_open *__ret)
{
	const unsigned long __callno = __devmgr_method_fops_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_fops_open(__rref, rpc_info, __ret, devno, vfs_flags, vfs_mode, tgt_rref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_fops_open(unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref, struct __actvret_drvcall_devmgr_fops_open *__ret)
{
	return actvxactcall_drvcall_devmgr_fops_open(false, false, devno, vfs_flags, vfs_mode, tgt_rref, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_fops_open(rref_t __rref, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref, struct __actvret_drvcall_devmgr_fops_open *__ret)
{
	return actvxactcapcall_drvcall_devmgr_fops_open(false, false, __rref, devno, vfs_flags, vfs_mode, tgt_rref, __ret);
}

extern int __actvcall_drvcall_devmgr_device_mgmt_index(unsigned long long __rpc_info, void *__buf, unsigned int type, unsigned int cmd, int index);


extern int __actvcapcall_drvcall_devmgr_device_mgmt_index(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type, unsigned int cmd, int index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_device_mgmt_index(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int type, unsigned int cmd, int index, struct __actvret_drvcall_devmgr_device_mgmt_index *__ret)
{
	const unsigned long __callno = __devmgr_method_device_mgmt_index;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_device_mgmt_index(rpc_info, __ret, type, cmd, index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_device_mgmt_index(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int type, unsigned int cmd, int index, struct __actvret_drvcall_devmgr_device_mgmt_index *__ret)
{
	const unsigned long __callno = __devmgr_method_device_mgmt_index;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_device_mgmt_index(__rref, rpc_info, __ret, type, cmd, index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_device_mgmt_index(unsigned int type, unsigned int cmd, int index, struct __actvret_drvcall_devmgr_device_mgmt_index *__ret)
{
	return actvxactcall_drvcall_devmgr_device_mgmt_index(false, false, type, cmd, index, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_device_mgmt_index(rref_t __rref, unsigned int type, unsigned int cmd, int index, struct __actvret_drvcall_devmgr_device_mgmt_index *__ret)
{
	return actvxactcapcall_drvcall_devmgr_device_mgmt_index(false, false, __rref, type, cmd, index, __ret);
}

extern int __actvcall_drvcall_devmgr_mgmt_load_module(unsigned long long __rpc_info, void *__buf, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target);


extern int __actvcapcall_drvcall_devmgr_mgmt_load_module(rref_t __rref, unsigned long long __rpc_info, void *__buf, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_mgmt_load_module(bool is_xact_fwd, bool is_xact_allow_refwd, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_mgmt_load_module;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct devmgr_mgmt_target), __arch_actv_rpc_stksz_alignment(struct devmgr_mgmt_target));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_mgmt_load_module(rpc_info, ___void_buf, load_type, modname, modargs, target);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_mgmt_load_module(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_mgmt_load_module;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct devmgr_mgmt_target), __arch_actv_rpc_stksz_alignment(struct devmgr_mgmt_target));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_mgmt_load_module(__rref, rpc_info, ___void_buf, load_type, modname, modargs, target);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_mgmt_load_module(int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_mgmt_load_module(false, false, load_type, modname, modargs, target, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_mgmt_load_module(rref_t __rref, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_mgmt_load_module(false, false, __rref, load_type, modname, modargs, target, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_mgmt_unload_module(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct devmgr_mgmt_target target);


extern int __actvcapcall_drvcall_devmgr_mgmt_unload_module(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct devmgr_mgmt_target target);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_mgmt_unload_module(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf modname, struct devmgr_mgmt_target target)
{
	const unsigned long __callno = __devmgr_method_mgmt_unload_module;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct devmgr_mgmt_target), __arch_actv_rpc_stksz_alignment(struct devmgr_mgmt_target));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_mgmt_unload_module(rpc_info, NULL, modname, target);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_mgmt_unload_module(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf modname, struct devmgr_mgmt_target target)
{
	const unsigned long __callno = __devmgr_method_mgmt_unload_module;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct devmgr_mgmt_target), __arch_actv_rpc_stksz_alignment(struct devmgr_mgmt_target));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_mgmt_unload_module(__rref, rpc_info, NULL, modname, target);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_mgmt_unload_module(struct hm_actv_buf modname, struct devmgr_mgmt_target target)
{
	return actvxactcall_drvcall_devmgr_mgmt_unload_module(false, false, modname, target);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_mgmt_unload_module(rref_t __rref, struct hm_actv_buf modname, struct devmgr_mgmt_target target)
{
	return actvxactcapcall_drvcall_devmgr_mgmt_unload_module(false, false, __rref, modname, target);
}

extern int __actvcall_drvcall_devmgr_mgmt_sync(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_mgmt_sync(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_mgmt_sync(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devmgr_method_mgmt_sync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_mgmt_sync(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_mgmt_sync(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devmgr_method_mgmt_sync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_mgmt_sync(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_mgmt_sync(void)
{
	return actvxactcall_drvcall_devmgr_mgmt_sync(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_mgmt_sync(rref_t __rref)
{
	return actvxactcapcall_drvcall_devmgr_mgmt_sync(false, false, __rref);
}

extern int __actvcall_drvcall_devmgr_mgmt_dump(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


extern int __actvcapcall_drvcall_devmgr_mgmt_dump(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_mgmt_dump(bool is_xact_fwd, bool is_xact_allow_refwd, struct bunch_ipc_attr attr, struct __actvret_drvcall_devmgr_mgmt_dump *__ret)
{
	const unsigned long __callno = __devmgr_method_mgmt_dump;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_mgmt_dump(rpc_info, __ret, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_mgmt_dump(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct bunch_ipc_attr attr, struct __actvret_drvcall_devmgr_mgmt_dump *__ret)
{
	const unsigned long __callno = __devmgr_method_mgmt_dump;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_mgmt_dump(__rref, rpc_info, __ret, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_mgmt_dump(struct bunch_ipc_attr attr, struct __actvret_drvcall_devmgr_mgmt_dump *__ret)
{
	return actvxactcall_drvcall_devmgr_mgmt_dump(false, false, attr, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_mgmt_dump(rref_t __rref, struct bunch_ipc_attr attr, struct __actvret_drvcall_devmgr_mgmt_dump *__ret)
{
	return actvxactcapcall_drvcall_devmgr_mgmt_dump(false, false, __rref, attr, __ret);
}

extern int __actvcall_drvcall_devmgr_irq_map(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg);


extern int __actvcapcall_drvcall_devmgr_irq_map(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_irq_map(bool is_xact_fwd, bool is_xact_allow_refwd, struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_map *__ret)
{
	const unsigned long __callno = __devmgr_method_irq_map;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_map_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_map_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_irq_map(rpc_info, __ret, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_irq_map(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_map *__ret)
{
	const unsigned long __callno = __devmgr_method_irq_map;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_map_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_map_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_irq_map(__rref, rpc_info, __ret, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_irq_map(struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_map *__ret)
{
	return actvxactcall_drvcall_devmgr_irq_map(false, false, arg, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_irq_map(rref_t __rref, struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_map *__ret)
{
	return actvxactcapcall_drvcall_devmgr_irq_map(false, false, __rref, arg, __ret);
}

extern int __actvcall_drvcall_devmgr_irq_translate(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg);


extern int __actvcapcall_drvcall_devmgr_irq_translate(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_irq_translate(bool is_xact_fwd, bool is_xact_allow_refwd, struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_translate *__ret)
{
	const unsigned long __callno = __devmgr_method_irq_translate;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_map_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_map_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_irq_translate(rpc_info, __ret, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_irq_translate(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_translate *__ret)
{
	const unsigned long __callno = __devmgr_method_irq_translate;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_map_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_map_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_irq_translate(__rref, rpc_info, __ret, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_irq_translate(struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_translate *__ret)
{
	return actvxactcall_drvcall_devmgr_irq_translate(false, false, arg, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_irq_translate(rref_t __rref, struct drvcall_devmgr_irq_map_arg arg, struct __actvret_drvcall_devmgr_irq_translate *__ret)
{
	return actvxactcapcall_drvcall_devmgr_irq_translate(false, false, __rref, arg, __ret);
}

extern int __actvcall_drvcall_devmgr_irq_fetch(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_fetch_arg arg);


extern int __actvcapcall_drvcall_devmgr_irq_fetch(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_fetch_arg arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_irq_fetch(bool is_xact_fwd, bool is_xact_allow_refwd, struct drvcall_devmgr_irq_fetch_arg arg, struct __actvret_drvcall_devmgr_irq_fetch *__ret)
{
	const unsigned long __callno = __devmgr_method_irq_fetch;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_fetch_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_fetch_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_irq_fetch(rpc_info, __ret, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_irq_fetch(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct drvcall_devmgr_irq_fetch_arg arg, struct __actvret_drvcall_devmgr_irq_fetch *__ret)
{
	const unsigned long __callno = __devmgr_method_irq_fetch;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_fetch_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_fetch_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_irq_fetch(__rref, rpc_info, __ret, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_irq_fetch(struct drvcall_devmgr_irq_fetch_arg arg, struct __actvret_drvcall_devmgr_irq_fetch *__ret)
{
	return actvxactcall_drvcall_devmgr_irq_fetch(false, false, arg, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_irq_fetch(rref_t __rref, struct drvcall_devmgr_irq_fetch_arg arg, struct __actvret_drvcall_devmgr_irq_fetch *__ret)
{
	return actvxactcapcall_drvcall_devmgr_irq_fetch(false, false, __rref, arg, __ret);
}

extern int __actvcall_drvcall_devmgr_irq_return(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_return_arg arg);


extern int __actvcapcall_drvcall_devmgr_irq_return(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_return_arg arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_irq_return(bool is_xact_fwd, bool is_xact_allow_refwd, struct drvcall_devmgr_irq_return_arg arg)
{
	const unsigned long __callno = __devmgr_method_irq_return;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_return_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_return_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_irq_return(rpc_info, NULL, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_irq_return(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct drvcall_devmgr_irq_return_arg arg)
{
	const unsigned long __callno = __devmgr_method_irq_return;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_return_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_return_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_irq_return(__rref, rpc_info, NULL, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_irq_return(struct drvcall_devmgr_irq_return_arg arg)
{
	return actvxactcall_drvcall_devmgr_irq_return(false, false, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_irq_return(rref_t __rref, struct drvcall_devmgr_irq_return_arg arg)
{
	return actvxactcapcall_drvcall_devmgr_irq_return(false, false, __rref, arg);
}

extern int __actvcall_drvcall_devmgr_irq_thread_setaffinity(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_thread_arg arg);


extern int __actvcapcall_drvcall_devmgr_irq_thread_setaffinity(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_thread_arg arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_irq_thread_setaffinity(bool is_xact_fwd, bool is_xact_allow_refwd, struct drvcall_devmgr_irq_thread_arg arg)
{
	const unsigned long __callno = __devmgr_method_irq_thread_setaffinity;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_thread_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_thread_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_irq_thread_setaffinity(rpc_info, NULL, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_irq_thread_setaffinity(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct drvcall_devmgr_irq_thread_arg arg)
{
	const unsigned long __callno = __devmgr_method_irq_thread_setaffinity;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct drvcall_devmgr_irq_thread_arg), __arch_actv_rpc_stksz_alignment(struct drvcall_devmgr_irq_thread_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_irq_thread_setaffinity(__rref, rpc_info, NULL, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_irq_thread_setaffinity(struct drvcall_devmgr_irq_thread_arg arg)
{
	return actvxactcall_drvcall_devmgr_irq_thread_setaffinity(false, false, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_irq_thread_setaffinity(rref_t __rref, struct drvcall_devmgr_irq_thread_arg arg)
{
	return actvxactcapcall_drvcall_devmgr_irq_thread_setaffinity(false, false, __rref, arg);
}

extern int __actvcall_drvcall_devmgr_net_alloc_ifindex(unsigned long long __rpc_info, void *__buf, int ifindex);


extern int __actvcapcall_drvcall_devmgr_net_alloc_ifindex(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_net_alloc_ifindex(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex, struct __actvret_drvcall_devmgr_net_alloc_ifindex *__ret)
{
	const unsigned long __callno = __devmgr_method_net_alloc_ifindex;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_net_alloc_ifindex(rpc_info, __ret, ifindex);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_net_alloc_ifindex(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex, struct __actvret_drvcall_devmgr_net_alloc_ifindex *__ret)
{
	const unsigned long __callno = __devmgr_method_net_alloc_ifindex;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_net_alloc_ifindex(__rref, rpc_info, __ret, ifindex);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_net_alloc_ifindex(int ifindex, struct __actvret_drvcall_devmgr_net_alloc_ifindex *__ret)
{
	return actvxactcall_drvcall_devmgr_net_alloc_ifindex(false, false, ifindex, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_net_alloc_ifindex(rref_t __rref, int ifindex, struct __actvret_drvcall_devmgr_net_alloc_ifindex *__ret)
{
	return actvxactcapcall_drvcall_devmgr_net_alloc_ifindex(false, false, __rref, ifindex, __ret);
}

extern int __actvcall_drvcall_devmgr_net_register(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, int ifindex, unsigned long long features);


extern int __actvcapcall_drvcall_devmgr_net_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, int ifindex, unsigned long long features);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_net_register(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, int ifindex, unsigned long long features, struct __actvret_drvcall_devmgr_net_register *__ret)
{
	const unsigned long __callno = __devmgr_method_net_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_net_register(rpc_info, __ret, name, ifindex, features);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_net_register(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, int ifindex, unsigned long long features, struct __actvret_drvcall_devmgr_net_register *__ret)
{
	const unsigned long __callno = __devmgr_method_net_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_net_register(__rref, rpc_info, __ret, name, ifindex, features);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_net_register(struct hm_actv_buf name, int ifindex, unsigned long long features, struct __actvret_drvcall_devmgr_net_register *__ret)
{
	return actvxactcall_drvcall_devmgr_net_register(false, false, name, ifindex, features, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_net_register(rref_t __rref, struct hm_actv_buf name, int ifindex, unsigned long long features, struct __actvret_drvcall_devmgr_net_register *__ret)
{
	return actvxactcapcall_drvcall_devmgr_net_register(false, false, __rref, name, ifindex, features, __ret);
}

extern int __actvcall_drvcall_devmgr_net_unregister(unsigned long long __rpc_info, void *__buf, int ifindex);


extern int __actvcapcall_drvcall_devmgr_net_unregister(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_net_unregister(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex)
{
	const unsigned long __callno = __devmgr_method_net_unregister;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_net_unregister(rpc_info, NULL, ifindex);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_net_unregister(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex)
{
	const unsigned long __callno = __devmgr_method_net_unregister;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_net_unregister(__rref, rpc_info, NULL, ifindex);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_net_unregister(int ifindex)
{
	return actvxactcall_drvcall_devmgr_net_unregister(false, false, ifindex);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_net_unregister(rref_t __rref, int ifindex)
{
	return actvxactcapcall_drvcall_devmgr_net_unregister(false, false, __rref, ifindex);
}

extern int __actvcall_drvcall_devmgr_net_hmnetd_ready(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_net_hmnetd_ready(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_net_hmnetd_ready(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devmgr_method_net_hmnetd_ready;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_net_hmnetd_ready(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_net_hmnetd_ready(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devmgr_method_net_hmnetd_ready;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_net_hmnetd_ready(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_net_hmnetd_ready(void)
{
	return actvxactcall_drvcall_devmgr_net_hmnetd_ready(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_net_hmnetd_ready(rref_t __rref)
{
	return actvxactcapcall_drvcall_devmgr_net_hmnetd_ready(false, false, __rref);
}

extern int __actvcall_drvcall_devmgr_net_open(unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref);


extern int __actvcapcall_drvcall_devmgr_net_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_net_open(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex, rref_t tgt_rref)
{
	const unsigned long __callno = __devmgr_method_net_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_net_open(rpc_info, NULL, ifindex, tgt_rref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_net_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex, rref_t tgt_rref)
{
	const unsigned long __callno = __devmgr_method_net_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_net_open(__rref, rpc_info, NULL, ifindex, tgt_rref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_net_open(int ifindex, rref_t tgt_rref)
{
	return actvxactcall_drvcall_devmgr_net_open(false, false, ifindex, tgt_rref);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_net_open(rref_t __rref, int ifindex, rref_t tgt_rref)
{
	return actvxactcapcall_drvcall_devmgr_net_open(false, false, __rref, ifindex, tgt_rref);
}

extern int __actvcall_drvcall_devmgr_pm_action(unsigned long long __rpc_info, void *__buf, unsigned int action);


extern int __actvcapcall_drvcall_devmgr_pm_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int action);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_pm_action(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int action)
{
	const unsigned long __callno = __devmgr_method_pm_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_pm_action(rpc_info, NULL, action);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_pm_action(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int action)
{
	const unsigned long __callno = __devmgr_method_pm_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_pm_action(__rref, rpc_info, NULL, action);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_pm_action(unsigned int action)
{
	return actvxactcall_drvcall_devmgr_pm_action(false, false, action);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_pm_action(rref_t __rref, unsigned int action)
{
	return actvxactcapcall_drvcall_devmgr_pm_action(false, false, __rref, action);
}

extern int __actvcall_drvcall_devmgr_pm_power_off(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_pm_power_off(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_pm_power_off(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devmgr_method_pm_power_off;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_pm_power_off(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_pm_power_off(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devmgr_method_pm_power_off;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_pm_power_off(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_pm_power_off(void)
{
	return actvxactcall_drvcall_devmgr_pm_power_off(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_pm_power_off(rref_t __rref)
{
	return actvxactcapcall_drvcall_devmgr_pm_power_off(false, false, __rref);
}

extern int __actvcall_drvcall_devmgr_pm_reboot(unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd);


extern int __actvcapcall_drvcall_devmgr_pm_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_pm_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, struct reboot_cmd cmd)
{
	const unsigned long __callno = __devmgr_method_pm_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_pm_reboot(rpc_info, NULL, cmd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_pm_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct reboot_cmd cmd)
{
	const unsigned long __callno = __devmgr_method_pm_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_pm_reboot(__rref, rpc_info, NULL, cmd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_pm_reboot(struct reboot_cmd cmd)
{
	return actvxactcall_drvcall_devmgr_pm_reboot(false, false, cmd);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_pm_reboot(rref_t __rref, struct reboot_cmd cmd)
{
	return actvxactcapcall_drvcall_devmgr_pm_reboot(false, false, __rref, cmd);
}

extern int __actvcall_drvcall_devmgr_pm_cpuhp_action(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_pm_cpuhp_action(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_pm_cpuhp_action(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_pm_cpuhp_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_pm_cpuhp_action(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_pm_cpuhp_action(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_pm_cpuhp_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_pm_cpuhp_action(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_pm_cpuhp_action(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_pm_cpuhp_action(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_pm_cpuhp_action(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_pm_cpuhp_action(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_procfs_create_data(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname);


extern int __actvcapcall_drvcall_devmgr_procfs_create_data(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_procfs_create_data(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname)
{
	const unsigned long __callno = __devmgr_method_procfs_create_data;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_procfs_create_data(rpc_info, NULL, name, mode, ops, linkname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_procfs_create_data(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname)
{
	const unsigned long __callno = __devmgr_method_procfs_create_data;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_procfs_create_data(__rref, rpc_info, NULL, name, mode, ops, linkname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_procfs_create_data(struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname)
{
	return actvxactcall_drvcall_devmgr_procfs_create_data(false, false, name, mode, ops, linkname);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_procfs_create_data(rref_t __rref, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname)
{
	return actvxactcapcall_drvcall_devmgr_procfs_create_data(false, false, __rref, name, mode, ops, linkname);
}

extern int __actvcall_drvcall_devmgr_procfs_unlink(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name);


extern int __actvcapcall_drvcall_devmgr_procfs_unlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_procfs_unlink(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name)
{
	const unsigned long __callno = __devmgr_method_procfs_unlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_procfs_unlink(rpc_info, NULL, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_procfs_unlink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name)
{
	const unsigned long __callno = __devmgr_method_procfs_unlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_procfs_unlink(__rref, rpc_info, NULL, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_procfs_unlink(struct hm_actv_buf name)
{
	return actvxactcall_drvcall_devmgr_procfs_unlink(false, false, name);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_procfs_unlink(rref_t __rref, struct hm_actv_buf name)
{
	return actvxactcapcall_drvcall_devmgr_procfs_unlink(false, false, __rref, name);
}

extern int __actvcall_drvcall_devmgr_procfs_read(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_procfs_read(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_procfs_read(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_procfs_read(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_procfs_read(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_procfs_read(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_procfs_read(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_procfs_write(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_procfs_write(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_procfs_write(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_procfs_write(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_procfs_write(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_procfs_write(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_procfs_write(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_sysfs_create_dir(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned int nsid);


extern int __actvcapcall_drvcall_devmgr_sysfs_create_dir(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_create_dir(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, unsigned int mode, unsigned int nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_dir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_create_dir(rpc_info, NULL, name, mode, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_create_dir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, unsigned int mode, unsigned int nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_dir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_create_dir(__rref, rpc_info, NULL, name, mode, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_create_dir(struct hm_actv_buf name, unsigned int mode, unsigned int nsid)
{
	return actvxactcall_drvcall_devmgr_sysfs_create_dir(false, false, name, mode, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_create_dir(rref_t __rref, struct hm_actv_buf name, unsigned int mode, unsigned int nsid)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_create_dir(false, false, __rref, name, mode, nsid);
}

extern int __actvcall_drvcall_devmgr_sysfs_create_file(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, int tagid);


extern int __actvcapcall_drvcall_devmgr_sysfs_create_file(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, int tagid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_create_file(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, unsigned int mode, int tagid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_file;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_create_file(rpc_info, NULL, name, mode, tagid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_create_file(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, unsigned int mode, int tagid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_file;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_create_file(__rref, rpc_info, NULL, name, mode, tagid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_create_file(struct hm_actv_buf name, unsigned int mode, int tagid)
{
	return actvxactcall_drvcall_devmgr_sysfs_create_file(false, false, name, mode, tagid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_create_file(rref_t __rref, struct hm_actv_buf name, unsigned int mode, int tagid)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_create_file(false, false, __rref, name, mode, tagid);
}

extern int __actvcall_drvcall_devmgr_sysfs_create_file_ex(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid);


extern int __actvcapcall_drvcall_devmgr_sysfs_create_file_ex(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_create_file_ex(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_file_ex;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_create_file_ex(rpc_info, NULL, name, mode, size, tagid, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_create_file_ex(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_file_ex;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_create_file_ex(__rref, rpc_info, NULL, name, mode, size, tagid, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_create_file_ex(struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid)
{
	return actvxactcall_drvcall_devmgr_sysfs_create_file_ex(false, false, name, mode, size, tagid, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_create_file_ex(rref_t __rref, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_create_file_ex(false, false, __rref, name, mode, size, tagid, nsid);
}

extern int __actvcall_drvcall_devmgr_sysfs_create_symlink(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid);


extern int __actvcapcall_drvcall_devmgr_sysfs_create_symlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_create_symlink(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_symlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_create_symlink(rpc_info, NULL, name, mode, linkname, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_create_symlink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_symlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_create_symlink(__rref, rpc_info, NULL, name, mode, linkname, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_create_symlink(struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid)
{
	return actvxactcall_drvcall_devmgr_sysfs_create_symlink(false, false, name, mode, linkname, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_create_symlink(rref_t __rref, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_create_symlink(false, false, __rref, name, mode, linkname, nsid);
}

extern int __actvcall_drvcall_devmgr_sysfs_delete(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int nsid);


extern int __actvcapcall_drvcall_devmgr_sysfs_delete(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_delete(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, unsigned int nsid, struct __actvret_drvcall_devmgr_sysfs_delete *__ret)
{
	const unsigned long __callno = __devmgr_method_sysfs_delete;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_delete(rpc_info, __ret, name, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_delete(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, unsigned int nsid, struct __actvret_drvcall_devmgr_sysfs_delete *__ret)
{
	const unsigned long __callno = __devmgr_method_sysfs_delete;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_delete(__rref, rpc_info, __ret, name, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_delete(struct hm_actv_buf name, unsigned int nsid, struct __actvret_drvcall_devmgr_sysfs_delete *__ret)
{
	return actvxactcall_drvcall_devmgr_sysfs_delete(false, false, name, nsid, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_delete(rref_t __rref, struct hm_actv_buf name, unsigned int nsid, struct __actvret_drvcall_devmgr_sysfs_delete *__ret)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_delete(false, false, __rref, name, nsid, __ret);
}

extern int __actvcall_drvcall_devmgr_sysfs_notify(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name);


extern int __actvcapcall_drvcall_devmgr_sysfs_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_notify(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name)
{
	const unsigned long __callno = __devmgr_method_sysfs_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_notify(rpc_info, NULL, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name)
{
	const unsigned long __callno = __devmgr_method_sysfs_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_notify(__rref, rpc_info, NULL, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_notify(struct hm_actv_buf name)
{
	return actvxactcall_drvcall_devmgr_sysfs_notify(false, false, name);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_notify(rref_t __rref, struct hm_actv_buf name)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_notify(false, false, __rref, name);
}

extern int __actvcall_drvcall_devmgr_sysfs_create_batch(unsigned long long __rpc_info, void *__buf, void *ldk_entry, int cnt);


extern int __actvcapcall_drvcall_devmgr_sysfs_create_batch(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *ldk_entry, int cnt);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_create_batch(bool is_xact_fwd, bool is_xact_allow_refwd, void *ldk_entry, int cnt)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_batch;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_create_batch(rpc_info, NULL, ldk_entry, cnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_create_batch(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *ldk_entry, int cnt)
{
	const unsigned long __callno = __devmgr_method_sysfs_create_batch;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_create_batch(__rref, rpc_info, NULL, ldk_entry, cnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_create_batch(void *ldk_entry, int cnt)
{
	return actvxactcall_drvcall_devmgr_sysfs_create_batch(false, false, ldk_entry, cnt);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_create_batch(rref_t __rref, void *ldk_entry, int cnt)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_create_batch(false, false, __rref, ldk_entry, cnt);
}

extern int __actvcall_drvcall_devmgr_sysfs_read(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf);


extern int __actvcapcall_drvcall_devmgr_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf actv_buf)
{
	const unsigned long __callno = __devmgr_method_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_read(rpc_info, NULL, actv_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf actv_buf)
{
	const unsigned long __callno = __devmgr_method_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_read(__rref, rpc_info, NULL, actv_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_read(struct hm_actv_buf actv_buf)
{
	return actvxactcall_drvcall_devmgr_sysfs_read(false, false, actv_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_read(rref_t __rref, struct hm_actv_buf actv_buf)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_read(false, false, __rref, actv_buf);
}

extern int __actvcall_drvcall_devmgr_sysfs_write(unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf);


extern int __actvcapcall_drvcall_devmgr_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf)
{
	const unsigned long __callno = __devmgr_method_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_write(rpc_info, NULL, tagid, fd, ctx, pos, actv_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf)
{
	const unsigned long __callno = __devmgr_method_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_write(__rref, rpc_info, NULL, tagid, fd, ctx, pos, actv_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_write(int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf)
{
	return actvxactcall_drvcall_devmgr_sysfs_write(false, false, tagid, fd, ctx, pos, actv_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_write(rref_t __rref, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_write(false, false, __rref, tagid, fd, ctx, pos, actv_buf);
}

extern int __actvcall_drvcall_devmgr_sysfs_poll(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_sysfs_poll(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_poll(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_sysfs_poll;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_poll(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_poll(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_sysfs_poll;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_poll(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_poll(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_sysfs_poll(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_poll(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_poll(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_sysfs_rename_link(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid);


extern int __actvcapcall_drvcall_devmgr_sysfs_rename_link(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_rename_link(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_rename_link;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_rename_link(rpc_info, NULL, old_name, new_name, old_nsid, new_nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_rename_link(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid)
{
	const unsigned long __callno = __devmgr_method_sysfs_rename_link;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_rename_link(__rref, rpc_info, NULL, old_name, new_name, old_nsid, new_nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_rename_link(struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid)
{
	return actvxactcall_drvcall_devmgr_sysfs_rename_link(false, false, old_name, new_name, old_nsid, new_nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_rename_link(rref_t __rref, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_rename_link(false, false, __rref, old_name, new_name, old_nsid, new_nsid);
}

extern int __actvcall_drvcall_devmgr_sysfs_open(unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode);


extern int __actvcapcall_drvcall_devmgr_sysfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode, struct __actvret_drvcall_devmgr_sysfs_open *__ret)
{
	const unsigned long __callno = __devmgr_method_sysfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_open(rpc_info, __ret, tagid, ctx, flags, mode);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode, struct __actvret_drvcall_devmgr_sysfs_open *__ret)
{
	const unsigned long __callno = __devmgr_method_sysfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_open(__rref, rpc_info, __ret, tagid, ctx, flags, mode);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_open(int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode, struct __actvret_drvcall_devmgr_sysfs_open *__ret)
{
	return actvxactcall_drvcall_devmgr_sysfs_open(false, false, tagid, ctx, flags, mode, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_open(rref_t __rref, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode, struct __actvret_drvcall_devmgr_sysfs_open *__ret)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_open(false, false, __rref, tagid, ctx, flags, mode, __ret);
}

extern int __actvcall_drvcall_devmgr_sysfs_close(unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx);


extern int __actvcapcall_drvcall_devmgr_sysfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sysfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, int tagid, unsigned long long fd, unsigned long long ctx)
{
	const unsigned long __callno = __devmgr_method_sysfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sysfs_close(rpc_info, NULL, tagid, fd, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sysfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int tagid, unsigned long long fd, unsigned long long ctx)
{
	const unsigned long __callno = __devmgr_method_sysfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sysfs_close(__rref, rpc_info, NULL, tagid, fd, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sysfs_close(int tagid, unsigned long long fd, unsigned long long ctx)
{
	return actvxactcall_drvcall_devmgr_sysfs_close(false, false, tagid, fd, ctx);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sysfs_close(rref_t __rref, int tagid, unsigned long long fd, unsigned long long ctx)
{
	return actvxactcapcall_drvcall_devmgr_sysfs_close(false, false, __rref, tagid, fd, ctx);
}

extern int __actvcall_drvcall_devmgr_fw_acquire_contents(unsigned long long __rpc_info, void *__buf, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen);


extern int __actvcapcall_drvcall_devmgr_fw_acquire_contents(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_fw_acquire_contents(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen, struct __actvret_drvcall_devmgr_fw_acquire_contents *__ret)
{
	const unsigned long __callno = __devmgr_method_fw_acquire_contents;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_fw_acquire_contents(rpc_info, __ret, buf, size, name, namelen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_fw_acquire_contents(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen, struct __actvret_drvcall_devmgr_fw_acquire_contents *__ret)
{
	const unsigned long __callno = __devmgr_method_fw_acquire_contents;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_fw_acquire_contents(__rref, rpc_info, __ret, buf, size, name, namelen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_fw_acquire_contents(unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen, struct __actvret_drvcall_devmgr_fw_acquire_contents *__ret)
{
	return actvxactcall_drvcall_devmgr_fw_acquire_contents(false, false, buf, size, name, namelen, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_fw_acquire_contents(rref_t __rref, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen, struct __actvret_drvcall_devmgr_fw_acquire_contents *__ret)
{
	return actvxactcapcall_drvcall_devmgr_fw_acquire_contents(false, false, __rref, buf, size, name, namelen, __ret);
}

extern int __actvcall_drvcall_devmgr_fw_cms_verify(unsigned long long __rpc_info, void *__buf, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size);


extern int __actvcapcall_drvcall_devmgr_fw_cms_verify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_fw_cms_verify(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size)
{
	const unsigned long __callno = __devmgr_method_fw_cms_verify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_fw_cms_verify(rpc_info, NULL, shm_key, data_size, sig_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_fw_cms_verify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size)
{
	const unsigned long __callno = __devmgr_method_fw_cms_verify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_fw_cms_verify(__rref, rpc_info, NULL, shm_key, data_size, sig_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_fw_cms_verify(unsigned long long shm_key, unsigned long data_size, unsigned long sig_size)
{
	return actvxactcall_drvcall_devmgr_fw_cms_verify(false, false, shm_key, data_size, sig_size);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_fw_cms_verify(rref_t __rref, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size)
{
	return actvxactcapcall_drvcall_devmgr_fw_cms_verify(false, false, __rref, shm_key, data_size, sig_size);
}

extern int __actvcall_drvcall_devmgr_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


extern int __actvcapcall_drvcall_devmgr_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sec_mgmt(rpc_info, ___void_buf, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sec_mgmt(__rref, rpc_info, ___void_buf, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sec_mgmt(unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_sec_mgmt(false, false, module, command, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sec_mgmt(rref_t __rref, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_sec_mgmt(false, false, __rref, module, command, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_sec_sync_fastpath_dev(unsigned long long __rpc_info, void *__buf, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state);


extern int __actvcapcall_drvcall_devmgr_sec_sync_fastpath_dev(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_sec_sync_fastpath_dev(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_sec_sync_fastpath_dev;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_sec_sync_fastpath_dev(rpc_info, ___void_buf, devno, vfs_mode, task_sid, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_sec_sync_fastpath_dev(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devmgr_method_sec_sync_fastpath_dev;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_sec_sync_fastpath_dev(__rref, rpc_info, ___void_buf, devno, vfs_mode, task_sid, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_sec_sync_fastpath_dev(unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devmgr_sec_sync_fastpath_dev(false, false, devno, vfs_mode, task_sid, state, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_sec_sync_fastpath_dev(rref_t __rref, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devmgr_sec_sync_fastpath_dev(false, false, __rref, devno, vfs_mode, task_sid, state, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devmgr_pac_cfi(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devmgr_pac_cfi(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devmgr_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devmgr_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devmgr_pac_cfi(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devmgr_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devmgr_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devmgr_pac_cfi(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devmgr_pac_cfi(void)
{
	return actvxactcall_drvcall_devmgr_pac_cfi(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devmgr_pac_cfi(rref_t __rref)
{
	return actvxactcapcall_drvcall_devmgr_pac_cfi(false, false, __rref);
}

#endif
#ifdef SYSIF_GEN_IFACE
#include <hmkernel/compiler.h>
#include <hmasm/actv_rpc.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <libstrict/strict.h>
#include <hmasm/lsyscall.h>
static int __used __noinline __opt_noframe __internal_rpccall_nobuf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_buf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_retbuf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_fallback(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_nobuf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(NoBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_buf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(SendBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_retbuf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(RetBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_fallback(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(, ret);
	return ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_core_register_devhost(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_core_register_devhost(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_core_register_devhost(unsigned long long __rpc_info, void *__buf, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	uref_t __maybe_unused __actv_local_var_ap_uref = ap_uref;
	struct hm_actv_buf __maybe_unused __actv_local_var_group = group;
	uref_t __maybe_unused __actv_local_var_devmgr_ap_uref = devmgr_ap_uref;
	__ret = ____actvcall_drvcall_devmgr_core_register_devhost(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(ap_uref, __actv_local_var_ap_uref), __ARCH_ACTV_RPC_PASS_ARG(group, __actv_local_var_group), __ARCH_ACTV_RPC_PASS_ARG(devmgr_ap_uref, __actv_local_var_devmgr_ap_uref));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_core_register_devhost(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int flags, uref_t ap_uref, struct hm_actv_buf group, uref_t devmgr_ap_uref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	uref_t __maybe_unused __actv_local_var_ap_uref = ap_uref;
	struct hm_actv_buf __maybe_unused __actv_local_var_group = group;
	uref_t __maybe_unused __actv_local_var_devmgr_ap_uref = devmgr_ap_uref;
	__ret = ____actvcapcall_drvcall_devmgr_core_register_devhost(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(ap_uref, __actv_local_var_ap_uref), __ARCH_ACTV_RPC_PASS_ARG(group, __actv_local_var_group), __ARCH_ACTV_RPC_PASS_ARG(devmgr_ap_uref, __actv_local_var_devmgr_ap_uref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_core_devhost_init_done(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_core_devhost_init_done(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_core_devhost_init_done(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_core_devhost_init_done(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_core_devhost_init_done(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_core_devhost_init_done(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_fallback"))) __internal_rpccall ____actvcall_drvcall_devmgr_core_device_add(unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


static int __attribute__((alias("__internal_rpccapcall_fallback"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_core_device_add(rref_t __rref, unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_core_device_add(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_devname = devname;
	struct hm_actv_buf __maybe_unused __actv_local_var_devnode_name = devnode_name;
	int __maybe_unused __actv_local_var_parent = parent;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_devt = devt;
	int __maybe_unused __actv_local_var_vfs_mode = vfs_mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_group = group;
	__ret = ____actvcall_drvcall_devmgr_core_device_add(__rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(devname, __actv_local_var_devname), __ARCH_ACTV_RPC_PASS_ARG(devnode_name, __actv_local_var_devnode_name), __ARCH_ACTV_RPC_PASS_ARG(parent, __actv_local_var_parent), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(devt, __actv_local_var_devt), __ARCH_ACTV_RPC_PASS_ARG(vfs_mode, __actv_local_var_vfs_mode), __ARCH_ACTV_RPC_PASS_ARG(group, __actv_local_var_group));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_core_device_add(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf devnode_name, int parent, unsigned int flags, unsigned int devt, int vfs_mode, struct hm_actv_buf group)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_devname = devname;
	struct hm_actv_buf __maybe_unused __actv_local_var_devnode_name = devnode_name;
	int __maybe_unused __actv_local_var_parent = parent;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_devt = devt;
	int __maybe_unused __actv_local_var_vfs_mode = vfs_mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_group = group;
	__ret = ____actvcapcall_drvcall_devmgr_core_device_add(__rref, __rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(devname, __actv_local_var_devname), __ARCH_ACTV_RPC_PASS_ARG(devnode_name, __actv_local_var_devnode_name), __ARCH_ACTV_RPC_PASS_ARG(parent, __actv_local_var_parent), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(devt, __actv_local_var_devt), __ARCH_ACTV_RPC_PASS_ARG(vfs_mode, __actv_local_var_vfs_mode), __ARCH_ACTV_RPC_PASS_ARG(group, __actv_local_var_group));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_core_device_del(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_core_device_del(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_core_device_del(unsigned long long __rpc_info, void *__buf, int devid)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	__ret = ____actvcall_drvcall_devmgr_core_device_del(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_core_device_del(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	__ret = ____actvcapcall_drvcall_devmgr_core_device_del(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_core_request_module(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_core_request_module(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_core_request_module(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct hm_actv_buf args)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_modname = modname;
	struct hm_actv_buf __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_drvcall_devmgr_core_request_module(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(modname, __actv_local_var_modname), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_core_request_module(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct hm_actv_buf args)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_modname = modname;
	struct hm_actv_buf __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_drvcall_devmgr_core_request_module(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(modname, __actv_local_var_modname), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_chrdev_register_region(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_chrdev_register_region(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_chrdev_register_region(unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	unsigned int __maybe_unused __actv_local_var_baseminor = baseminor;
	int __maybe_unused __actv_local_var_minorct = minorct;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcall_drvcall_devmgr_chrdev_register_region(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(baseminor, __actv_local_var_baseminor), __ARCH_ACTV_RPC_PASS_ARG(minorct, __actv_local_var_minorct), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_chrdev_register_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct, struct hm_actv_buf name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	unsigned int __maybe_unused __actv_local_var_baseminor = baseminor;
	int __maybe_unused __actv_local_var_minorct = minorct;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcapcall_drvcall_devmgr_chrdev_register_region(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(baseminor, __actv_local_var_baseminor), __ARCH_ACTV_RPC_PASS_ARG(minorct, __actv_local_var_minorct), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_chrdev_unregister_region(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_chrdev_unregister_region(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_chrdev_unregister_region(unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	unsigned int __maybe_unused __actv_local_var_baseminor = baseminor;
	int __maybe_unused __actv_local_var_minorct = minorct;
	__ret = ____actvcall_drvcall_devmgr_chrdev_unregister_region(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(baseminor, __actv_local_var_baseminor), __ARCH_ACTV_RPC_PASS_ARG(minorct, __actv_local_var_minorct));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_chrdev_unregister_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, unsigned int baseminor, int minorct)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	unsigned int __maybe_unused __actv_local_var_baseminor = baseminor;
	int __maybe_unused __actv_local_var_minorct = minorct;
	__ret = ____actvcapcall_drvcall_devmgr_chrdev_unregister_region(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(baseminor, __actv_local_var_baseminor), __ARCH_ACTV_RPC_PASS_ARG(minorct, __actv_local_var_minorct));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_blkdev_register_region(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_blkdev_register_region(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_blkdev_register_region(unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcall_drvcall_devmgr_blkdev_register_region(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_blkdev_register_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcapcall_drvcall_devmgr_blkdev_register_region(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_blkdev_unregister_region(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_blkdev_unregister_region(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_blkdev_unregister_region(unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcall_drvcall_devmgr_blkdev_unregister_region(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_blkdev_unregister_region(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int major, struct hm_actv_buf name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_major = major;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcapcall_drvcall_devmgr_blkdev_unregister_region(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(major, __actv_local_var_major), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_fops_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_fops_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_fops_open(unsigned long long __rpc_info, void *__buf, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_devno = devno;
	unsigned int __maybe_unused __actv_local_var_vfs_flags = vfs_flags;
	int __maybe_unused __actv_local_var_vfs_mode = vfs_mode;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	__ret = ____actvcall_drvcall_devmgr_fops_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(vfs_flags, __actv_local_var_vfs_flags), __ARCH_ACTV_RPC_PASS_ARG(vfs_mode, __actv_local_var_vfs_mode), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_fops_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int devno, unsigned int vfs_flags, int vfs_mode, rref_t tgt_rref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_devno = devno;
	unsigned int __maybe_unused __actv_local_var_vfs_flags = vfs_flags;
	int __maybe_unused __actv_local_var_vfs_mode = vfs_mode;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	__ret = ____actvcapcall_drvcall_devmgr_fops_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(vfs_flags, __actv_local_var_vfs_flags), __ARCH_ACTV_RPC_PASS_ARG(vfs_mode, __actv_local_var_vfs_mode), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_device_mgmt_index(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_device_mgmt_index(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_device_mgmt_index(unsigned long long __rpc_info, void *__buf, unsigned int type, unsigned int cmd, int index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	unsigned int __maybe_unused __actv_local_var_cmd = cmd;
	int __maybe_unused __actv_local_var_index = index;
	__ret = ____actvcall_drvcall_devmgr_device_mgmt_index(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(index, __actv_local_var_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_device_mgmt_index(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type, unsigned int cmd, int index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	unsigned int __maybe_unused __actv_local_var_cmd = cmd;
	int __maybe_unused __actv_local_var_index = index;
	__ret = ____actvcapcall_drvcall_devmgr_device_mgmt_index(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(index, __actv_local_var_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_mgmt_load_module(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_mgmt_load_module(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_mgmt_load_module(unsigned long long __rpc_info, void *__buf, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target)
{
	int __ret;
	int __maybe_unused __actv_local_var_load_type = load_type;
	struct hm_actv_buf __maybe_unused __actv_local_var_modname = modname;
	struct hm_actv_buf __maybe_unused __actv_local_var_modargs = modargs;
	struct devmgr_mgmt_target __maybe_unused __actv_local_var_target = target;
	__ret = ____actvcall_drvcall_devmgr_mgmt_load_module(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(load_type, __actv_local_var_load_type), __ARCH_ACTV_RPC_PASS_ARG(modname, __actv_local_var_modname), __ARCH_ACTV_RPC_PASS_ARG(modargs, __actv_local_var_modargs), __ARCH_ACTV_RPC_PASS_ARG(target, __actv_local_var_target));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_mgmt_load_module(rref_t __rref, unsigned long long __rpc_info, void *__buf, int load_type, struct hm_actv_buf modname, struct hm_actv_buf modargs, struct devmgr_mgmt_target target)
{
	int __ret;
	int __maybe_unused __actv_local_var_load_type = load_type;
	struct hm_actv_buf __maybe_unused __actv_local_var_modname = modname;
	struct hm_actv_buf __maybe_unused __actv_local_var_modargs = modargs;
	struct devmgr_mgmt_target __maybe_unused __actv_local_var_target = target;
	__ret = ____actvcapcall_drvcall_devmgr_mgmt_load_module(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(load_type, __actv_local_var_load_type), __ARCH_ACTV_RPC_PASS_ARG(modname, __actv_local_var_modname), __ARCH_ACTV_RPC_PASS_ARG(modargs, __actv_local_var_modargs), __ARCH_ACTV_RPC_PASS_ARG(target, __actv_local_var_target));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_mgmt_unload_module(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_mgmt_unload_module(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_mgmt_unload_module(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct devmgr_mgmt_target target)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_modname = modname;
	struct devmgr_mgmt_target __maybe_unused __actv_local_var_target = target;
	__ret = ____actvcall_drvcall_devmgr_mgmt_unload_module(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(modname, __actv_local_var_modname), __ARCH_ACTV_RPC_PASS_ARG(target, __actv_local_var_target));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_mgmt_unload_module(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf modname, struct devmgr_mgmt_target target)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_modname = modname;
	struct devmgr_mgmt_target __maybe_unused __actv_local_var_target = target;
	__ret = ____actvcapcall_drvcall_devmgr_mgmt_unload_module(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(modname, __actv_local_var_modname), __ARCH_ACTV_RPC_PASS_ARG(target, __actv_local_var_target));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_mgmt_sync(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_mgmt_sync(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_mgmt_sync(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_mgmt_sync(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_mgmt_sync(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_mgmt_sync(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_mgmt_dump(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_mgmt_dump(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_mgmt_dump(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_drvcall_devmgr_mgmt_dump(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_mgmt_dump(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_drvcall_devmgr_mgmt_dump(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_irq_map(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_irq_map(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_irq_map(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_map_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devmgr_irq_map(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_irq_map(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_map_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devmgr_irq_map(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_irq_translate(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_irq_translate(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_irq_translate(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_map_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devmgr_irq_translate(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_irq_translate(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_map_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_map_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devmgr_irq_translate(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_irq_fetch(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_irq_fetch(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_irq_fetch(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_fetch_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_fetch_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devmgr_irq_fetch(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_irq_fetch(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_fetch_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_fetch_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devmgr_irq_fetch(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_irq_return(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_irq_return(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_irq_return(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_return_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_return_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devmgr_irq_return(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_irq_return(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_return_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_return_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devmgr_irq_return(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_irq_thread_setaffinity(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_irq_thread_setaffinity(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_irq_thread_setaffinity(unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_thread_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_thread_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devmgr_irq_thread_setaffinity(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_irq_thread_setaffinity(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct drvcall_devmgr_irq_thread_arg arg)
{
	int __ret;
	struct drvcall_devmgr_irq_thread_arg __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devmgr_irq_thread_setaffinity(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_net_alloc_ifindex(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_net_alloc_ifindex(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_net_alloc_ifindex(unsigned long long __rpc_info, void *__buf, int ifindex)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	__ret = ____actvcall_drvcall_devmgr_net_alloc_ifindex(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_net_alloc_ifindex(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	__ret = ____actvcapcall_drvcall_devmgr_net_alloc_ifindex(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_net_register(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_net_register(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_net_register(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, int ifindex, unsigned long long features)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	unsigned long long __maybe_unused __actv_local_var_features = features;
	__ret = ____actvcall_drvcall_devmgr_net_register(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(features, __actv_local_var_features));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_net_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, int ifindex, unsigned long long features)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	unsigned long long __maybe_unused __actv_local_var_features = features;
	__ret = ____actvcapcall_drvcall_devmgr_net_register(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(features, __actv_local_var_features));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_net_unregister(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_net_unregister(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_net_unregister(unsigned long long __rpc_info, void *__buf, int ifindex)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	__ret = ____actvcall_drvcall_devmgr_net_unregister(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_net_unregister(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	__ret = ____actvcapcall_drvcall_devmgr_net_unregister(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_net_hmnetd_ready(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_net_hmnetd_ready(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_net_hmnetd_ready(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_net_hmnetd_ready(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_net_hmnetd_ready(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_net_hmnetd_ready(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_net_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_net_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_net_open(unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	__ret = ____actvcall_drvcall_devmgr_net_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_net_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	__ret = ____actvcapcall_drvcall_devmgr_net_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_pm_action(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_pm_action(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_pm_action(unsigned long long __rpc_info, void *__buf, unsigned int action)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_action = action;
	__ret = ____actvcall_drvcall_devmgr_pm_action(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_pm_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int action)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_action = action;
	__ret = ____actvcapcall_drvcall_devmgr_pm_action(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_pm_power_off(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_pm_power_off(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_pm_power_off(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_pm_power_off(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_pm_power_off(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_pm_power_off(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_pm_reboot(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_pm_reboot(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_pm_reboot(unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd)
{
	int __ret;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	__ret = ____actvcall_drvcall_devmgr_pm_reboot(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_pm_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd)
{
	int __ret;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	__ret = ____actvcapcall_drvcall_devmgr_pm_reboot(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_pm_cpuhp_action(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_pm_cpuhp_action(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_pm_cpuhp_action(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_pm_cpuhp_action(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_pm_cpuhp_action(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_pm_cpuhp_action(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_procfs_create_data(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_procfs_create_data(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_procfs_create_data(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_ops = ops;
	struct hm_actv_buf __maybe_unused __actv_local_var_linkname = linkname;
	__ret = ____actvcall_drvcall_devmgr_procfs_create_data(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(ops, __actv_local_var_ops), __ARCH_ACTV_RPC_PASS_ARG(linkname, __actv_local_var_linkname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_procfs_create_data(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf ops, struct hm_actv_buf linkname)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_ops = ops;
	struct hm_actv_buf __maybe_unused __actv_local_var_linkname = linkname;
	__ret = ____actvcapcall_drvcall_devmgr_procfs_create_data(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(ops, __actv_local_var_ops), __ARCH_ACTV_RPC_PASS_ARG(linkname, __actv_local_var_linkname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_procfs_unlink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_procfs_unlink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_procfs_unlink(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcall_drvcall_devmgr_procfs_unlink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_procfs_unlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcapcall_drvcall_devmgr_procfs_unlink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_procfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_procfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_procfs_read(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_procfs_read(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_procfs_read(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_procfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_procfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_procfs_write(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_procfs_write(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_procfs_write(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_create_dir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_create_dir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_create_dir(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_drvcall_devmgr_sysfs_create_dir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_create_dir(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_create_dir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_create_file(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_create_file(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_create_file(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, int tagid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	int __maybe_unused __actv_local_var_tagid = tagid;
	__ret = ____actvcall_drvcall_devmgr_sysfs_create_file(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_create_file(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, int tagid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	int __maybe_unused __actv_local_var_tagid = tagid;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_create_file(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_create_file_ex(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_create_file_ex(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_create_file_ex(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	unsigned long long __maybe_unused __actv_local_var_size = size;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_drvcall_devmgr_sysfs_create_file_ex(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_create_file_ex(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, unsigned long long size, int tagid, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	unsigned long long __maybe_unused __actv_local_var_size = size;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_create_file_ex(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_create_symlink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_create_symlink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_create_symlink(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_linkname = linkname;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_drvcall_devmgr_sysfs_create_symlink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(linkname, __actv_local_var_linkname), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_create_symlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int mode, struct hm_actv_buf linkname, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_linkname = linkname;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_create_symlink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(linkname, __actv_local_var_linkname), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_delete(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_delete(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_delete(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_drvcall_devmgr_sysfs_delete(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_delete(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, unsigned int nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_delete(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_notify(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcall_drvcall_devmgr_sysfs_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_create_batch(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_create_batch(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_create_batch(unsigned long long __rpc_info, void *__buf, void *ldk_entry, int cnt)
{
	int __ret;
	__maybe_unused void *__actv_local_var_ldk_entry = ldk_entry;
	int __maybe_unused __actv_local_var_cnt = cnt;
	__ret = ____actvcall_drvcall_devmgr_sysfs_create_batch(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ldk_entry, __actv_local_var_ldk_entry), __ARCH_ACTV_RPC_PASS_ARG(cnt, __actv_local_var_cnt));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_create_batch(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *ldk_entry, int cnt)
{
	int __ret;
	__maybe_unused void *__actv_local_var_ldk_entry = ldk_entry;
	int __maybe_unused __actv_local_var_cnt = cnt;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_create_batch(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ldk_entry, __actv_local_var_ldk_entry), __ARCH_ACTV_RPC_PASS_ARG(cnt, __actv_local_var_cnt));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_read(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_buf = actv_buf;
	__ret = ____actvcall_drvcall_devmgr_sysfs_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(actv_buf, __actv_local_var_actv_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_buf = actv_buf;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(actv_buf, __actv_local_var_actv_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_write(unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf)
{
	int __ret;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_ctx = ctx;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_buf = actv_buf;
	__ret = ____actvcall_drvcall_devmgr_sysfs_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(actv_buf, __actv_local_var_actv_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx, unsigned long long pos, struct hm_actv_buf actv_buf)
{
	int __ret;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_ctx = ctx;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_buf = actv_buf;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(actv_buf, __actv_local_var_actv_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_poll(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_poll(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_poll(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_sysfs_poll(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_poll(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_poll(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_rename_link(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_rename_link(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_rename_link(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_old_name = old_name;
	struct hm_actv_buf __maybe_unused __actv_local_var_new_name = new_name;
	unsigned int __maybe_unused __actv_local_var_old_nsid = old_nsid;
	unsigned int __maybe_unused __actv_local_var_new_nsid = new_nsid;
	__ret = ____actvcall_drvcall_devmgr_sysfs_rename_link(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(old_name, __actv_local_var_old_name), __ARCH_ACTV_RPC_PASS_ARG(new_name, __actv_local_var_new_name), __ARCH_ACTV_RPC_PASS_ARG(old_nsid, __actv_local_var_old_nsid), __ARCH_ACTV_RPC_PASS_ARG(new_nsid, __actv_local_var_new_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_rename_link(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf old_name, struct hm_actv_buf new_name, unsigned int old_nsid, unsigned int new_nsid)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_old_name = old_name;
	struct hm_actv_buf __maybe_unused __actv_local_var_new_name = new_name;
	unsigned int __maybe_unused __actv_local_var_old_nsid = old_nsid;
	unsigned int __maybe_unused __actv_local_var_new_nsid = new_nsid;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_rename_link(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(old_name, __actv_local_var_old_name), __ARCH_ACTV_RPC_PASS_ARG(new_name, __actv_local_var_new_name), __ARCH_ACTV_RPC_PASS_ARG(old_nsid, __actv_local_var_old_nsid), __ARCH_ACTV_RPC_PASS_ARG(new_nsid, __actv_local_var_new_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_open(unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode)
{
	int __ret;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_ctx = ctx;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	__ret = ____actvcall_drvcall_devmgr_sysfs_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long ctx, unsigned int flags, unsigned int mode)
{
	int __ret;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_ctx = ctx;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sysfs_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sysfs_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sysfs_close(unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx)
{
	int __ret;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devmgr_sysfs_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sysfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int tagid, unsigned long long fd, unsigned long long ctx)
{
	int __ret;
	int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devmgr_sysfs_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_fw_acquire_contents(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_fw_acquire_contents(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_fw_acquire_contents(unsigned long long __rpc_info, void *__buf, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_buf = buf;
	unsigned int __maybe_unused __actv_local_var_size = size;
	unsigned long __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_namelen = namelen;
	__ret = ____actvcall_drvcall_devmgr_fw_acquire_contents(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(namelen, __actv_local_var_namelen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_fw_acquire_contents(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long buf, unsigned int size, unsigned long name, unsigned int namelen)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_buf = buf;
	unsigned int __maybe_unused __actv_local_var_size = size;
	unsigned long __maybe_unused __actv_local_var_name = name;
	unsigned int __maybe_unused __actv_local_var_namelen = namelen;
	__ret = ____actvcapcall_drvcall_devmgr_fw_acquire_contents(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(namelen, __actv_local_var_namelen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_fw_cms_verify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_fw_cms_verify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_fw_cms_verify(unsigned long long __rpc_info, void *__buf, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_shm_key = shm_key;
	unsigned long __maybe_unused __actv_local_var_data_size = data_size;
	unsigned long __maybe_unused __actv_local_var_sig_size = sig_size;
	__ret = ____actvcall_drvcall_devmgr_fw_cms_verify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_key, __actv_local_var_shm_key), __ARCH_ACTV_RPC_PASS_ARG(data_size, __actv_local_var_data_size), __ARCH_ACTV_RPC_PASS_ARG(sig_size, __actv_local_var_sig_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_fw_cms_verify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long shm_key, unsigned long data_size, unsigned long sig_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_shm_key = shm_key;
	unsigned long __maybe_unused __actv_local_var_data_size = data_size;
	unsigned long __maybe_unused __actv_local_var_sig_size = sig_size;
	__ret = ____actvcapcall_drvcall_devmgr_fw_cms_verify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_key, __actv_local_var_shm_key), __ARCH_ACTV_RPC_PASS_ARG(data_size, __actv_local_var_data_size), __ARCH_ACTV_RPC_PASS_ARG(sig_size, __actv_local_var_sig_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sec_mgmt(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sec_mgmt(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcall_drvcall_devmgr_sec_mgmt(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcapcall_drvcall_devmgr_sec_mgmt(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devmgr_sec_sync_fastpath_dev(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_sec_sync_fastpath_dev(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_sec_sync_fastpath_dev(unsigned long long __rpc_info, void *__buf, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_devno = devno;
	int __maybe_unused __actv_local_var_vfs_mode = vfs_mode;
	unsigned int __maybe_unused __actv_local_var_task_sid = task_sid;
	unsigned int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcall_drvcall_devmgr_sec_sync_fastpath_dev(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(vfs_mode, __actv_local_var_vfs_mode), __ARCH_ACTV_RPC_PASS_ARG(task_sid, __actv_local_var_task_sid), __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_sec_sync_fastpath_dev(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int devno, int vfs_mode, unsigned int task_sid, unsigned int state)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_devno = devno;
	int __maybe_unused __actv_local_var_vfs_mode = vfs_mode;
	unsigned int __maybe_unused __actv_local_var_task_sid = task_sid;
	unsigned int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcapcall_drvcall_devmgr_sec_sync_fastpath_dev(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(vfs_mode, __actv_local_var_vfs_mode), __ARCH_ACTV_RPC_PASS_ARG(task_sid, __actv_local_var_task_sid), __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devmgr_pac_cfi(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devmgr_pac_cfi(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devmgr_pac_cfi(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devmgr_pac_cfi(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devmgr_pac_cfi(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devmgr_pac_cfi(__rref, __rpc_info, __buf);
	return __ret;
}

#endif
#endif
