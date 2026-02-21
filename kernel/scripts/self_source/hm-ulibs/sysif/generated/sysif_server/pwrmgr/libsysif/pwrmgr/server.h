/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for pwrmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/pwrmgr/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/pwrmgr/libsysif/pwrmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/pwrmgr/libsysif/pwrmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/pwrmgr/libsysif/pwrmgr/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:38:30 2025
*/
#ifndef LIBSYSIF_PWRMGR_SERVER_H
#define LIBSYSIF_PWRMGR_SERVER_H
#ifndef LIBSYSIF_PWRMGR_COMMON_H
#define LIBSYSIF_PWRMGR_COMMON_H
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
#include <libbunch/bunch_ipc.h>
#include <libhmpm/shutdown.h>

#define PWRMGR_PATH	"pwrmgr_actv"



#include <libhmpm/suspend.h>



#include <hmkernel/types.h>



#include <libhmpm/wl_stat.h>

#define WL_ACTION_ADD			__U(0)
#define WL_ACTION_REMOVE		__U(1)
#define WL_ACTION_ACQUIRE		__U(2)
#define WL_ACTION_RELEASE		__U(3)

struct wake_lock_name {
	char string[WAKELOCK_NAME_MAX];
};




#include <libhmnotifier/hm_notifier.h>

#define CPUHP_ADD_EVENT		(1U)
#define CPUHP_REMOVE_EVENT	(2U)




#include <libhmpm/pmqos.h>




#define THERMAL_ZONE_TYPE_MAX			32

struct thermal_zone_type {
	char string[THERMAL_ZONE_TYPE_MAX];
};




#include <libhmpm/pm_init_event.h>


raw_static_assert(sizeof(struct reboot_cmd) <= 96,
                    pmcall_shutdown_reboot_enter_arg_cmd_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    pmcall_sysfs_read_arg_actv_buf_is_too_large);
raw_static_assert(sizeof(struct wake_lock_name) <= 96,
                    pmcall_wakelock_action_arg_name_is_too_large);
raw_static_assert(sizeof(struct wake_lock_name) <= 96,
                    pmcall_wakelock_acquire_timeout_arg_wakelock_is_too_large);
raw_static_assert(sizeof(struct wake_lock_name) <= 96,
                    pmcall_wakelock_stat_arg_name_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    pmcall_wakelock_stat_arg_attr_is_too_large);
raw_static_assert(sizeof(struct hm_pmqos_request) <= 96,
                    pmcall_pmqos_request_remove_arg_request_is_too_large);
raw_static_assert(sizeof(struct hm_pmqos_request) <= 96,
                    pmcall_pmqos_request_update_arg_request_is_too_large);
raw_static_assert(sizeof(struct thermal_zone_type) <= 96,
                    pmcall_thermmgr_stat_arg_type_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    pmcall_thermmgr_stat_arg_attr_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    pmcall_idlemgr_stat_arg_attr_is_too_large);
raw_static_assert(sizeof(enum pm_init_event) <= 96,
                    pmcall_pm_init_notify_arg_type_is_too_large);
struct __actvret_pmcall_pwrmgr_system_state {
	enum hm_system_state state;
};

struct __actvret_pmcall_shutdown_enter {
};

struct __actvret_pmcall_shutdown_softreboot_enter {
};

struct __actvret_pmcall_shutdown_reboot_enter {
};

struct __actvret_pmcall_shutdown_kexec_enter {
};

struct __actvret_pmcall_shutdown_reboot {
};

struct __actvret_pmcall_suspend_set_state {
};

struct __actvret_pmcall_suspend_disk_restore {
};

struct __actvret_pmcall_sysfs_read {
};

struct __actvret_pmcall_sysfs_write {
};

struct __actvret_pmcall_wakelock_action {
};

struct __actvret_pmcall_wakelock_acquire_timeout {
};

struct __actvret_pmcall_wakelock_get_count {
	unsigned int count;
};

struct __actvret_pmcall_wakelock_save_count {
};

struct __actvret_pmcall_wakelock_stat {
};

struct __actvret_pmcall_cpuhp_add_notifier {
};

struct __actvret_pmcall_cpuhp_remove_notifier {
};

struct __actvret_pmcall_cpuhp_action {
};

struct __actvret_pmcall_cpuhp_revoke_listener {
};

struct __actvret_pmcall_pmqos_request_add {
	int id;
};

struct __actvret_pmcall_pmqos_request_remove {
};

struct __actvret_pmcall_pmqos_request_update {
};

struct __actvret_pmcall_dh_register {
};

struct __actvret_pmcall_sec_mgmt {
};

struct __actvret_pmcall_pac_cfi {
};

struct __actvret_pmcall_thermmgr_stat {
};

struct __actvret_pmcall_idlemgr_stat {
};

struct __actvret_pmcall_pm_init_notify {
};

#ifdef __aarch64__
union __actvret_pmcall {
	struct __actvret_pmcall_pwrmgr_system_state pwrmgr_system_state;
	struct __actvret_pmcall_shutdown_enter shutdown_enter;
	struct __actvret_pmcall_shutdown_softreboot_enter shutdown_softreboot_enter;
	struct __actvret_pmcall_shutdown_reboot_enter shutdown_reboot_enter;
	struct __actvret_pmcall_shutdown_kexec_enter shutdown_kexec_enter;
	struct __actvret_pmcall_shutdown_reboot shutdown_reboot;
	struct __actvret_pmcall_suspend_set_state suspend_set_state;
	struct __actvret_pmcall_suspend_disk_restore suspend_disk_restore;
	struct __actvret_pmcall_sysfs_read sysfs_read;
	struct __actvret_pmcall_sysfs_write sysfs_write;
	struct __actvret_pmcall_wakelock_action wakelock_action;
	struct __actvret_pmcall_wakelock_acquire_timeout wakelock_acquire_timeout;
	struct __actvret_pmcall_wakelock_get_count wakelock_get_count;
	struct __actvret_pmcall_wakelock_save_count wakelock_save_count;
	struct __actvret_pmcall_wakelock_stat wakelock_stat;
	struct __actvret_pmcall_cpuhp_add_notifier cpuhp_add_notifier;
	struct __actvret_pmcall_cpuhp_remove_notifier cpuhp_remove_notifier;
	struct __actvret_pmcall_cpuhp_action cpuhp_action;
	struct __actvret_pmcall_cpuhp_revoke_listener cpuhp_revoke_listener;
	struct __actvret_pmcall_pmqos_request_add pmqos_request_add;
	struct __actvret_pmcall_pmqos_request_remove pmqos_request_remove;
	struct __actvret_pmcall_pmqos_request_update pmqos_request_update;
	struct __actvret_pmcall_dh_register dh_register;
	struct __actvret_pmcall_sec_mgmt sec_mgmt;
	struct __actvret_pmcall_pac_cfi pac_cfi;
	struct __actvret_pmcall_thermmgr_stat thermmgr_stat;
	struct __actvret_pmcall_idlemgr_stat idlemgr_stat;
	struct __actvret_pmcall_pm_init_notify pm_init_notify;
};

#elif defined(__arm__)
union __actvret_pmcall {
	struct __actvret_pmcall_pwrmgr_system_state pwrmgr_system_state;
	struct __actvret_pmcall_shutdown_enter shutdown_enter;
	struct __actvret_pmcall_shutdown_softreboot_enter shutdown_softreboot_enter;
	struct __actvret_pmcall_shutdown_reboot_enter shutdown_reboot_enter;
	struct __actvret_pmcall_shutdown_kexec_enter shutdown_kexec_enter;
	struct __actvret_pmcall_shutdown_reboot shutdown_reboot;
	struct __actvret_pmcall_suspend_set_state suspend_set_state;
	struct __actvret_pmcall_suspend_disk_restore suspend_disk_restore;
	struct __actvret_pmcall_sysfs_read sysfs_read;
	struct __actvret_pmcall_sysfs_write sysfs_write;
	struct __actvret_pmcall_wakelock_action wakelock_action;
	struct __actvret_pmcall_wakelock_acquire_timeout wakelock_acquire_timeout;
	struct __actvret_pmcall_wakelock_get_count wakelock_get_count;
	struct __actvret_pmcall_wakelock_save_count wakelock_save_count;
	struct __actvret_pmcall_wakelock_stat wakelock_stat;
	struct __actvret_pmcall_cpuhp_add_notifier cpuhp_add_notifier;
	struct __actvret_pmcall_cpuhp_remove_notifier cpuhp_remove_notifier;
	struct __actvret_pmcall_cpuhp_action cpuhp_action;
	struct __actvret_pmcall_cpuhp_revoke_listener cpuhp_revoke_listener;
	struct __actvret_pmcall_pmqos_request_add pmqos_request_add;
	struct __actvret_pmcall_pmqos_request_remove pmqos_request_remove;
	struct __actvret_pmcall_pmqos_request_update pmqos_request_update;
	struct __actvret_pmcall_dh_register dh_register;
	struct __actvret_pmcall_sec_mgmt sec_mgmt;
	struct __actvret_pmcall_pac_cfi pac_cfi;
	struct __actvret_pmcall_thermmgr_stat thermmgr_stat;
	struct __actvret_pmcall_idlemgr_stat idlemgr_stat;
	struct __actvret_pmcall_pm_init_notify pm_init_notify;
};

#else
union __actvret_pmcall {
	struct __actvret_pmcall_pwrmgr_system_state pwrmgr_system_state;
	struct __actvret_pmcall_shutdown_enter shutdown_enter;
	struct __actvret_pmcall_shutdown_softreboot_enter shutdown_softreboot_enter;
	struct __actvret_pmcall_shutdown_reboot_enter shutdown_reboot_enter;
	struct __actvret_pmcall_shutdown_kexec_enter shutdown_kexec_enter;
	struct __actvret_pmcall_shutdown_reboot shutdown_reboot;
	struct __actvret_pmcall_suspend_set_state suspend_set_state;
	struct __actvret_pmcall_suspend_disk_restore suspend_disk_restore;
	struct __actvret_pmcall_sysfs_read sysfs_read;
	struct __actvret_pmcall_sysfs_write sysfs_write;
	struct __actvret_pmcall_wakelock_action wakelock_action;
	struct __actvret_pmcall_wakelock_acquire_timeout wakelock_acquire_timeout;
	struct __actvret_pmcall_wakelock_get_count wakelock_get_count;
	struct __actvret_pmcall_wakelock_save_count wakelock_save_count;
	struct __actvret_pmcall_wakelock_stat wakelock_stat;
	struct __actvret_pmcall_cpuhp_add_notifier cpuhp_add_notifier;
	struct __actvret_pmcall_cpuhp_remove_notifier cpuhp_remove_notifier;
	struct __actvret_pmcall_cpuhp_action cpuhp_action;
	struct __actvret_pmcall_cpuhp_revoke_listener cpuhp_revoke_listener;
	struct __actvret_pmcall_pmqos_request_add pmqos_request_add;
	struct __actvret_pmcall_pmqos_request_remove pmqos_request_remove;
	struct __actvret_pmcall_pmqos_request_update pmqos_request_update;
	struct __actvret_pmcall_dh_register dh_register;
	struct __actvret_pmcall_sec_mgmt sec_mgmt;
	struct __actvret_pmcall_pac_cfi pac_cfi;
	struct __actvret_pmcall_thermmgr_stat thermmgr_stat;
	struct __actvret_pmcall_idlemgr_stat idlemgr_stat;
	struct __actvret_pmcall_pm_init_notify pm_init_notify;
};

#endif
enum __pwrmgr_mgr {
	__pwrmgr_INVAL_mgr=0,
	__pwrmgr_pwrmgr_mgr,
	__pwrmgr_shutdown_mgr,
	__pwrmgr_suspend_mgr,
	__pwrmgr_sysfs_mgr,
	__pwrmgr_wakelock_mgr,
	__pwrmgr_cpuhp_mgr,
	__pwrmgr_pmqos_mgr,
	__pwrmgr_dh_mgr,
	__pwrmgr_sec_mgr,
	__pwrmgr_thermmgr_mgr,
	__pwrmgr_idlemgr_mgr,
	__pwrmgr_pm_mgr,
	__pwrmgr_MAX_mgr,
};

#define __pwrmgr_method_MIN __ACTV_HIGHTABLE_START
#define __pwrmgr_method_shutdown_reboot (__ACTV_HIGHTABLE_START + 1)
#define __pwrmgr_method_pwrmgr_system_state (__ACTV_HIGHTABLE_START + 2)
#define __pwrmgr_method_shutdown_enter (__ACTV_HIGHTABLE_START + 3)
#define __pwrmgr_method_shutdown_softreboot_enter (__ACTV_HIGHTABLE_START + 4)
#define __pwrmgr_method_shutdown_reboot_enter (__ACTV_HIGHTABLE_START + 5)
#define __pwrmgr_method_shutdown_kexec_enter (__ACTV_HIGHTABLE_START + 6)
#define __pwrmgr_method_suspend_set_state (__ACTV_HIGHTABLE_START + 7)
#define __pwrmgr_method_suspend_disk_restore (__ACTV_HIGHTABLE_START + 8)
#define __pwrmgr_method_suspend_rsvd_1 (__ACTV_HIGHTABLE_START + 9)
#define __pwrmgr_method_sysfs_read (__ACTV_HIGHTABLE_START + 10)
#define __pwrmgr_method_sysfs_write (__ACTV_HIGHTABLE_START + 11)
#define __pwrmgr_method_sysfs_rsvd_2 (__ACTV_HIGHTABLE_START + 12)
#define __pwrmgr_method_sysfs_rsvd_1 (__ACTV_HIGHTABLE_START + 13)
#define __pwrmgr_method_wakelock_action (__ACTV_HIGHTABLE_START + 14)
#define __pwrmgr_method_wakelock_acquire_timeout (__ACTV_HIGHTABLE_START + 15)
#define __pwrmgr_method_wakelock_get_count (__ACTV_HIGHTABLE_START + 16)
#define __pwrmgr_method_wakelock_save_count (__ACTV_HIGHTABLE_START + 17)
#define __pwrmgr_method_wakelock_stat (__ACTV_HIGHTABLE_START + 18)
#define __pwrmgr_method_wakelock_rsvd_3 (__ACTV_HIGHTABLE_START + 19)
#define __pwrmgr_method_wakelock_rsvd_2 (__ACTV_HIGHTABLE_START + 20)
#define __pwrmgr_method_wakelock_rsvd_1 (__ACTV_HIGHTABLE_START + 21)
#define __pwrmgr_method_cpuhp_add_notifier (__ACTV_HIGHTABLE_START + 22)
#define __pwrmgr_method_cpuhp_remove_notifier (__ACTV_HIGHTABLE_START + 23)
#define __pwrmgr_method_cpuhp_action (__ACTV_HIGHTABLE_START + 24)
#define __pwrmgr_method_cpuhp_revoke_listener (__ACTV_HIGHTABLE_START + 25)
#define __pwrmgr_method_cpuhp_rsvd_4 (__ACTV_HIGHTABLE_START + 26)
#define __pwrmgr_method_cpuhp_rsvd_3 (__ACTV_HIGHTABLE_START + 27)
#define __pwrmgr_method_cpuhp_rsvd_2 (__ACTV_HIGHTABLE_START + 28)
#define __pwrmgr_method_cpuhp_rsvd_1 (__ACTV_HIGHTABLE_START + 29)
#define __pwrmgr_method_pmqos_request_add (__ACTV_HIGHTABLE_START + 30)
#define __pwrmgr_method_pmqos_request_remove (__ACTV_HIGHTABLE_START + 31)
#define __pwrmgr_method_pmqos_request_update (__ACTV_HIGHTABLE_START + 32)
#define __pwrmgr_method_dh_register (__ACTV_HIGHTABLE_START + 33)
#define __pwrmgr_method_sec_mgmt (__ACTV_HIGHTABLE_START + 34)
#define __pwrmgr_method_pac_cfi (__ACTV_HIGHTABLE_START + 35)
#define __pwrmgr_method_thermmgr_stat (__ACTV_HIGHTABLE_START + 36)
#define __pwrmgr_method_thermmgr_rsvd_3 (__ACTV_HIGHTABLE_START + 37)
#define __pwrmgr_method_thermmgr_rsvd_2 (__ACTV_HIGHTABLE_START + 38)
#define __pwrmgr_method_thermmgr_rsvd_1 (__ACTV_HIGHTABLE_START + 39)
#define __pwrmgr_method_idlemgr_stat (__ACTV_HIGHTABLE_START + 40)
#define __pwrmgr_method_pm_init_notify (__ACTV_HIGHTABLE_START + 41)
#define __pwrmgr_method_MAX (__ACTV_HIGHTABLE_START + 42)
raw_static_assert(sizeof(struct __actvret_pmcall_pwrmgr_system_state) < 512,
            pmcall_pwrmgr_system_state_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_shutdown_enter) < 512,
            pmcall_shutdown_enter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_shutdown_softreboot_enter) < 512,
            pmcall_shutdown_softreboot_enter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_shutdown_reboot_enter) < 512,
            pmcall_shutdown_reboot_enter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_shutdown_kexec_enter) < 512,
            pmcall_shutdown_kexec_enter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_shutdown_reboot) < 512,
            pmcall_shutdown_reboot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_suspend_set_state) < 512,
            pmcall_suspend_set_state_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_suspend_disk_restore) < 512,
            pmcall_suspend_disk_restore_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_sysfs_read) < 512,
            pmcall_sysfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_sysfs_write) < 512,
            pmcall_sysfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_wakelock_action) < 512,
            pmcall_wakelock_action_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_wakelock_acquire_timeout) < 512,
            pmcall_wakelock_acquire_timeout_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_wakelock_get_count) < 512,
            pmcall_wakelock_get_count_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_wakelock_save_count) < 512,
            pmcall_wakelock_save_count_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_wakelock_stat) < 512,
            pmcall_wakelock_stat_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_cpuhp_add_notifier) < 512,
            pmcall_cpuhp_add_notifier_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_cpuhp_remove_notifier) < 512,
            pmcall_cpuhp_remove_notifier_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_cpuhp_action) < 512,
            pmcall_cpuhp_action_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_cpuhp_revoke_listener) < 512,
            pmcall_cpuhp_revoke_listener_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_pmqos_request_add) < 512,
            pmcall_pmqos_request_add_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_pmqos_request_remove) < 512,
            pmcall_pmqos_request_remove_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_pmqos_request_update) < 512,
            pmcall_pmqos_request_update_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_dh_register) < 512,
            pmcall_dh_register_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_sec_mgmt) < 512,
            pmcall_sec_mgmt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_pac_cfi) < 512,
            pmcall_pac_cfi_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_thermmgr_stat) < 512,
            pmcall_thermmgr_stat_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_idlemgr_stat) < 512,
            pmcall_idlemgr_stat_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_pmcall_pm_init_notify) < 512,
            pmcall_pm_init_notify_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_pmcall) < 512,
        pmcall_too_many_actvret_data);
extern int __actvcall_pmcall_pwrmgr_system_state(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_pwrmgr_system_state(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_pwrmgr_system_state(bool is_xact_fwd, bool is_xact_allow_refwd, struct __actvret_pmcall_pwrmgr_system_state *__ret)
{
	const unsigned long __callno = __pwrmgr_method_pwrmgr_system_state;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_pwrmgr_system_state(rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_pwrmgr_system_state(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct __actvret_pmcall_pwrmgr_system_state *__ret)
{
	const unsigned long __callno = __pwrmgr_method_pwrmgr_system_state;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_pwrmgr_system_state(__rref, rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_pwrmgr_system_state(struct __actvret_pmcall_pwrmgr_system_state *__ret)
{
	return actvxactcall_pmcall_pwrmgr_system_state(false, false, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_pwrmgr_system_state(rref_t __rref, struct __actvret_pmcall_pwrmgr_system_state *__ret)
{
	return actvxactcapcall_pmcall_pwrmgr_system_state(false, false, __rref, __ret);
}

extern int __actvcall_pmcall_shutdown_enter(unsigned long long __rpc_info, void *__buf, long timeout_ms);


extern int __actvcapcall_pmcall_shutdown_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf, long timeout_ms);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_shutdown_enter(bool is_xact_fwd, bool is_xact_allow_refwd, long timeout_ms)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_shutdown_enter(rpc_info, NULL, timeout_ms);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_shutdown_enter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long timeout_ms)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_shutdown_enter(__rref, rpc_info, NULL, timeout_ms);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_shutdown_enter(long timeout_ms)
{
	return actvxactcall_pmcall_shutdown_enter(false, false, timeout_ms);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_shutdown_enter(rref_t __rref, long timeout_ms)
{
	return actvxactcapcall_pmcall_shutdown_enter(false, false, __rref, timeout_ms);
}

extern int __actvcall_pmcall_shutdown_softreboot_enter(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_shutdown_softreboot_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_shutdown_softreboot_enter(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_softreboot_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_shutdown_softreboot_enter(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_shutdown_softreboot_enter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_softreboot_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_shutdown_softreboot_enter(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_shutdown_softreboot_enter(void)
{
	return actvxactcall_pmcall_shutdown_softreboot_enter(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_shutdown_softreboot_enter(rref_t __rref)
{
	return actvxactcapcall_pmcall_shutdown_softreboot_enter(false, false, __rref);
}

extern int __actvcall_pmcall_shutdown_reboot_enter(unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd, long timeout_ms);


extern int __actvcapcall_pmcall_shutdown_reboot_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd, long timeout_ms);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_shutdown_reboot_enter(bool is_xact_fwd, bool is_xact_allow_refwd, struct reboot_cmd cmd, long timeout_ms)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_reboot_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_shutdown_reboot_enter(rpc_info, NULL, cmd, timeout_ms);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_shutdown_reboot_enter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct reboot_cmd cmd, long timeout_ms)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_reboot_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_shutdown_reboot_enter(__rref, rpc_info, NULL, cmd, timeout_ms);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_shutdown_reboot_enter(struct reboot_cmd cmd, long timeout_ms)
{
	return actvxactcall_pmcall_shutdown_reboot_enter(false, false, cmd, timeout_ms);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_shutdown_reboot_enter(rref_t __rref, struct reboot_cmd cmd, long timeout_ms)
{
	return actvxactcapcall_pmcall_shutdown_reboot_enter(false, false, __rref, cmd, timeout_ms);
}

extern int __actvcall_pmcall_shutdown_kexec_enter(unsigned long long __rpc_info, void *__buf, long timeout_ms);


extern int __actvcapcall_pmcall_shutdown_kexec_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf, long timeout_ms);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_shutdown_kexec_enter(bool is_xact_fwd, bool is_xact_allow_refwd, long timeout_ms)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_kexec_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_shutdown_kexec_enter(rpc_info, NULL, timeout_ms);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_shutdown_kexec_enter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long timeout_ms)
{
	const unsigned long __callno = __pwrmgr_method_shutdown_kexec_enter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_shutdown_kexec_enter(__rref, rpc_info, NULL, timeout_ms);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_shutdown_kexec_enter(long timeout_ms)
{
	return actvxactcall_pmcall_shutdown_kexec_enter(false, false, timeout_ms);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_shutdown_kexec_enter(rref_t __rref, long timeout_ms)
{
	return actvxactcapcall_pmcall_shutdown_kexec_enter(false, false, __rref, timeout_ms);
}

extern int __actvcall_pmcall_suspend_set_state(unsigned long long __rpc_info, void *__buf, int state);


extern int __actvcapcall_pmcall_suspend_set_state(rref_t __rref, unsigned long long __rpc_info, void *__buf, int state);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_suspend_set_state(bool is_xact_fwd, bool is_xact_allow_refwd, int state)
{
	const unsigned long __callno = __pwrmgr_method_suspend_set_state;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_suspend_set_state(rpc_info, NULL, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_suspend_set_state(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int state)
{
	const unsigned long __callno = __pwrmgr_method_suspend_set_state;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_suspend_set_state(__rref, rpc_info, NULL, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_suspend_set_state(int state)
{
	return actvxactcall_pmcall_suspend_set_state(false, false, state);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_suspend_set_state(rref_t __rref, int state)
{
	return actvxactcapcall_pmcall_suspend_set_state(false, false, __rref, state);
}

extern int __actvcall_pmcall_suspend_disk_restore(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_suspend_disk_restore(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_suspend_disk_restore(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __pwrmgr_method_suspend_disk_restore;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_suspend_disk_restore(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_suspend_disk_restore(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __pwrmgr_method_suspend_disk_restore;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_suspend_disk_restore(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_suspend_disk_restore(void)
{
	return actvxactcall_pmcall_suspend_disk_restore(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_suspend_disk_restore(rref_t __rref)
{
	return actvxactcapcall_pmcall_suspend_disk_restore(false, false, __rref);
}

extern int __actvcall_pmcall_sysfs_read(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf);


extern int __actvcapcall_pmcall_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf actv_buf)
{
	const unsigned long __callno = __pwrmgr_method_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_sysfs_read(rpc_info, NULL, actv_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf actv_buf)
{
	const unsigned long __callno = __pwrmgr_method_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_sysfs_read(__rref, rpc_info, NULL, actv_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_sysfs_read(struct hm_actv_buf actv_buf)
{
	return actvxactcall_pmcall_sysfs_read(false, false, actv_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_sysfs_read(rref_t __rref, struct hm_actv_buf actv_buf)
{
	return actvxactcapcall_pmcall_sysfs_read(false, false, __rref, actv_buf);
}

extern int __actvcall_pmcall_sysfs_write(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_sysfs_write(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_sysfs_write(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_sysfs_write(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_pmcall_sysfs_write(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_sysfs_write(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_pmcall_sysfs_write(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_pmcall_wakelock_action(unsigned long long __rpc_info, void *__buf, unsigned int action, struct wake_lock_name name);


extern int __actvcapcall_pmcall_wakelock_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int action, struct wake_lock_name name);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_wakelock_action(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int action, struct wake_lock_name name)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_wakelock_action(rpc_info, NULL, action, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_wakelock_action(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int action, struct wake_lock_name name)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_wakelock_action(__rref, rpc_info, NULL, action, name);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_wakelock_action(unsigned int action, struct wake_lock_name name)
{
	return actvxactcall_pmcall_wakelock_action(false, false, action, name);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_wakelock_action(rref_t __rref, unsigned int action, struct wake_lock_name name)
{
	return actvxactcapcall_pmcall_wakelock_action(false, false, __rref, action, name);
}

extern int __actvcall_pmcall_wakelock_acquire_timeout(unsigned long long __rpc_info, void *__buf, struct wake_lock_name wakelock, unsigned long msec);


extern int __actvcapcall_pmcall_wakelock_acquire_timeout(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct wake_lock_name wakelock, unsigned long msec);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_wakelock_acquire_timeout(bool is_xact_fwd, bool is_xact_allow_refwd, struct wake_lock_name wakelock, unsigned long msec)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_acquire_timeout;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_wakelock_acquire_timeout(rpc_info, NULL, wakelock, msec);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_wakelock_acquire_timeout(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct wake_lock_name wakelock, unsigned long msec)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_acquire_timeout;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_wakelock_acquire_timeout(__rref, rpc_info, NULL, wakelock, msec);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_wakelock_acquire_timeout(struct wake_lock_name wakelock, unsigned long msec)
{
	return actvxactcall_pmcall_wakelock_acquire_timeout(false, false, wakelock, msec);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_wakelock_acquire_timeout(rref_t __rref, struct wake_lock_name wakelock, unsigned long msec)
{
	return actvxactcapcall_pmcall_wakelock_acquire_timeout(false, false, __rref, wakelock, msec);
}

extern int __actvcall_pmcall_wakelock_get_count(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_wakelock_get_count(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_wakelock_get_count(bool is_xact_fwd, bool is_xact_allow_refwd, struct __actvret_pmcall_wakelock_get_count *__ret)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_get_count;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_wakelock_get_count(rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_wakelock_get_count(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct __actvret_pmcall_wakelock_get_count *__ret)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_get_count;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_wakelock_get_count(__rref, rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_wakelock_get_count(struct __actvret_pmcall_wakelock_get_count *__ret)
{
	return actvxactcall_pmcall_wakelock_get_count(false, false, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_wakelock_get_count(rref_t __rref, struct __actvret_pmcall_wakelock_get_count *__ret)
{
	return actvxactcapcall_pmcall_wakelock_get_count(false, false, __rref, __ret);
}

extern int __actvcall_pmcall_wakelock_save_count(unsigned long long __rpc_info, void *__buf, unsigned int count);


extern int __actvcapcall_pmcall_wakelock_save_count(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int count);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_wakelock_save_count(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int count)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_save_count;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_wakelock_save_count(rpc_info, NULL, count);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_wakelock_save_count(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int count)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_save_count;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_wakelock_save_count(__rref, rpc_info, NULL, count);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_wakelock_save_count(unsigned int count)
{
	return actvxactcall_pmcall_wakelock_save_count(false, false, count);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_wakelock_save_count(rref_t __rref, unsigned int count)
{
	return actvxactcapcall_pmcall_wakelock_save_count(false, false, __rref, count);
}

extern int __actvcall_pmcall_wakelock_stat(unsigned long long __rpc_info, void *__buf, struct wake_lock_name name, struct bunch_ipc_attr attr);


extern int __actvcapcall_pmcall_wakelock_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct wake_lock_name name, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_wakelock_stat(bool is_xact_fwd, bool is_xact_allow_refwd, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_wakelock_stat(rpc_info, NULL, name, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_wakelock_stat(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __pwrmgr_method_wakelock_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_wakelock_stat(__rref, rpc_info, NULL, name, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_wakelock_stat(struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	return actvxactcall_pmcall_wakelock_stat(false, false, name, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_wakelock_stat(rref_t __rref, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_pmcall_wakelock_stat(false, false, __rref, name, attr);
}

extern int __actvcall_pmcall_cpuhp_add_notifier(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_cpuhp_add_notifier(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_cpuhp_add_notifier(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_add_notifier;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_cpuhp_add_notifier(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_cpuhp_add_notifier(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_add_notifier;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_cpuhp_add_notifier(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_cpuhp_add_notifier(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_pmcall_cpuhp_add_notifier(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_cpuhp_add_notifier(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_pmcall_cpuhp_add_notifier(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_pmcall_cpuhp_remove_notifier(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_cpuhp_remove_notifier(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_cpuhp_remove_notifier(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_remove_notifier;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_cpuhp_remove_notifier(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_cpuhp_remove_notifier(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_remove_notifier;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_cpuhp_remove_notifier(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_cpuhp_remove_notifier(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_pmcall_cpuhp_remove_notifier(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_cpuhp_remove_notifier(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_pmcall_cpuhp_remove_notifier(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_pmcall_cpuhp_action(unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int event);


extern int __actvcapcall_pmcall_cpuhp_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int event);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_cpuhp_action(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cpu, unsigned int event)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_cpuhp_action(rpc_info, NULL, cpu, event);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_cpuhp_action(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cpu, unsigned int event)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_cpuhp_action(__rref, rpc_info, NULL, cpu, event);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_cpuhp_action(unsigned int cpu, unsigned int event)
{
	return actvxactcall_pmcall_cpuhp_action(false, false, cpu, event);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_cpuhp_action(rref_t __rref, unsigned int cpu, unsigned int event)
{
	return actvxactcapcall_pmcall_cpuhp_action(false, false, __rref, cpu, event);
}

extern int __actvcall_pmcall_cpuhp_revoke_listener(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx);


extern int __actvcapcall_pmcall_cpuhp_revoke_listener(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_cpuhp_revoke_listener(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_revoke_listener;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_cpuhp_revoke_listener(rpc_info, ___void_buf, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_cpuhp_revoke_listener(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_cpuhp_revoke_listener;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_cpuhp_revoke_listener(__rref, rpc_info, ___void_buf, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_cpuhp_revoke_listener(unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_pmcall_cpuhp_revoke_listener(false, false, cnode_idx, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_cpuhp_revoke_listener(rref_t __rref, unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_pmcall_cpuhp_revoke_listener(false, false, __rref, cnode_idx, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_pmcall_pmqos_request_add(unsigned long long __rpc_info, void *__buf, unsigned int type, int value);


extern int __actvcapcall_pmcall_pmqos_request_add(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type, int value);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_pmqos_request_add(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int type, int value, struct __actvret_pmcall_pmqos_request_add *__ret)
{
	const unsigned long __callno = __pwrmgr_method_pmqos_request_add;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_pmqos_request_add(rpc_info, __ret, type, value);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_pmqos_request_add(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int type, int value, struct __actvret_pmcall_pmqos_request_add *__ret)
{
	const unsigned long __callno = __pwrmgr_method_pmqos_request_add;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_pmqos_request_add(__rref, rpc_info, __ret, type, value);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_pmqos_request_add(unsigned int type, int value, struct __actvret_pmcall_pmqos_request_add *__ret)
{
	return actvxactcall_pmcall_pmqos_request_add(false, false, type, value, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_pmqos_request_add(rref_t __rref, unsigned int type, int value, struct __actvret_pmcall_pmqos_request_add *__ret)
{
	return actvxactcapcall_pmcall_pmqos_request_add(false, false, __rref, type, value, __ret);
}

extern int __actvcall_pmcall_pmqos_request_remove(unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request);


extern int __actvcapcall_pmcall_pmqos_request_remove(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_pmqos_request_remove(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_pmqos_request request)
{
	const unsigned long __callno = __pwrmgr_method_pmqos_request_remove;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_pmqos_request_remove(rpc_info, NULL, request);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_pmqos_request_remove(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_pmqos_request request)
{
	const unsigned long __callno = __pwrmgr_method_pmqos_request_remove;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_pmqos_request_remove(__rref, rpc_info, NULL, request);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_pmqos_request_remove(struct hm_pmqos_request request)
{
	return actvxactcall_pmcall_pmqos_request_remove(false, false, request);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_pmqos_request_remove(rref_t __rref, struct hm_pmqos_request request)
{
	return actvxactcapcall_pmcall_pmqos_request_remove(false, false, __rref, request);
}

extern int __actvcall_pmcall_pmqos_request_update(unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request, int value);


extern int __actvcapcall_pmcall_pmqos_request_update(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request, int value);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_pmqos_request_update(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_pmqos_request request, int value)
{
	const unsigned long __callno = __pwrmgr_method_pmqos_request_update;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_pmqos_request_update(rpc_info, NULL, request, value);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_pmqos_request_update(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_pmqos_request request, int value)
{
	const unsigned long __callno = __pwrmgr_method_pmqos_request_update;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_pmqos_request_update(__rref, rpc_info, NULL, request, value);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_pmqos_request_update(struct hm_pmqos_request request, int value)
{
	return actvxactcall_pmcall_pmqos_request_update(false, false, request, value);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_pmqos_request_update(rref_t __rref, struct hm_pmqos_request request, int value)
{
	return actvxactcapcall_pmcall_pmqos_request_update(false, false, __rref, request, value);
}

extern int __actvcall_pmcall_dh_register(unsigned long long __rpc_info, void *__buf, unsigned long long dh_ap_uref, unsigned int type);


extern int __actvcapcall_pmcall_dh_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long dh_ap_uref, unsigned int type);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_dh_register(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long dh_ap_uref, unsigned int type)
{
	const unsigned long __callno = __pwrmgr_method_dh_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_dh_register(rpc_info, NULL, dh_ap_uref, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_dh_register(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long dh_ap_uref, unsigned int type)
{
	const unsigned long __callno = __pwrmgr_method_dh_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_dh_register(__rref, rpc_info, NULL, dh_ap_uref, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_dh_register(unsigned long long dh_ap_uref, unsigned int type)
{
	return actvxactcall_pmcall_dh_register(false, false, dh_ap_uref, type);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_dh_register(rref_t __rref, unsigned long long dh_ap_uref, unsigned int type)
{
	return actvxactcapcall_pmcall_dh_register(false, false, __rref, dh_ap_uref, type);
}

extern int __actvcall_pmcall_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


extern int __actvcapcall_pmcall_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_sec_mgmt(rpc_info, ___void_buf, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __pwrmgr_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_sec_mgmt(__rref, rpc_info, ___void_buf, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_sec_mgmt(unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_pmcall_sec_mgmt(false, false, module, command, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_sec_mgmt(rref_t __rref, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_pmcall_sec_mgmt(false, false, __rref, module, command, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_pmcall_pac_cfi(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_pmcall_pac_cfi(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __pwrmgr_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_pac_cfi(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __pwrmgr_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_pac_cfi(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_pac_cfi(void)
{
	return actvxactcall_pmcall_pac_cfi(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_pac_cfi(rref_t __rref)
{
	return actvxactcapcall_pmcall_pac_cfi(false, false, __rref);
}

extern int __actvcall_pmcall_thermmgr_stat(unsigned long long __rpc_info, void *__buf, struct thermal_zone_type type, struct bunch_ipc_attr attr);


extern int __actvcapcall_pmcall_thermmgr_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct thermal_zone_type type, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_thermmgr_stat(bool is_xact_fwd, bool is_xact_allow_refwd, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __pwrmgr_method_thermmgr_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_thermmgr_stat(rpc_info, NULL, type, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_thermmgr_stat(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __pwrmgr_method_thermmgr_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_thermmgr_stat(__rref, rpc_info, NULL, type, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_thermmgr_stat(struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	return actvxactcall_pmcall_thermmgr_stat(false, false, type, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_thermmgr_stat(rref_t __rref, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_pmcall_thermmgr_stat(false, false, __rref, type, attr);
}

extern int __actvcall_pmcall_idlemgr_stat(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


extern int __actvcapcall_pmcall_idlemgr_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_idlemgr_stat(bool is_xact_fwd, bool is_xact_allow_refwd, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __pwrmgr_method_idlemgr_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_idlemgr_stat(rpc_info, NULL, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_idlemgr_stat(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __pwrmgr_method_idlemgr_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_idlemgr_stat(__rref, rpc_info, NULL, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_idlemgr_stat(struct bunch_ipc_attr attr)
{
	return actvxactcall_pmcall_idlemgr_stat(false, false, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_idlemgr_stat(rref_t __rref, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_pmcall_idlemgr_stat(false, false, __rref, attr);
}

extern int __actvcall_pmcall_pm_init_notify(unsigned long long __rpc_info, void *__buf, enum pm_init_event type);


extern int __actvcapcall_pmcall_pm_init_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum pm_init_event type);


static inline int __attribute__((always_inline)) actvxactcall_pmcall_pm_init_notify(bool is_xact_fwd, bool is_xact_allow_refwd, enum pm_init_event type)
{
	const unsigned long __callno = __pwrmgr_method_pm_init_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_pmcall_pm_init_notify(rpc_info, NULL, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_pmcall_pm_init_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, enum pm_init_event type)
{
	const unsigned long __callno = __pwrmgr_method_pm_init_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_pmcall_pm_init_notify(__rref, rpc_info, NULL, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_pmcall_pm_init_notify(enum pm_init_event type)
{
	return actvxactcall_pmcall_pm_init_notify(false, false, type);
}

static inline int __attribute__((always_inline)) actvcapcall_pmcall_pm_init_notify(rref_t __rref, enum pm_init_event type)
{
	return actvxactcapcall_pmcall_pm_init_notify(false, false, __rref, type);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, long timeout_ms);


extern int actvhdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);


extern int actvhdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, long timeout_ms);


extern int actvhdlr_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);


extern int actvhdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, int state);


extern int actvhdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);


extern int actvhdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);


extern int actvhdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);


extern int actvhdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned int count);


extern int actvhdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);


extern int actvhdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);


extern int actvhdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);


extern int actvhdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned int type, int value);


extern int actvhdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);


extern int actvhdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);


extern int actvhdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);


extern int actvhdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);


extern int actvhdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);


extern int actvhdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);


extern int actvhdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, enum pm_init_event type);


#else /* aarch64 */
extern int actvhdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, long timeout_ms);


extern int actvhdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);


extern int actvhdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, long timeout_ms);


extern int __actvshadowhdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);


extern int __actvshadowhdlr_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, int state);


extern int actvhdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);


extern int actvhdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);


extern int actvhdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);


extern int actvhdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned int count);


extern int actvhdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);


extern int __actvshadowhdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential);


extern int actvhdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);


extern int actvhdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);


extern int __actvshadowhdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned int type, int value);


extern int actvhdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);


extern int actvhdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);


extern int __actvshadowhdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);


extern int __actvshadowhdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);


extern int actvhdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);


extern int __actvshadowhdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);


extern int __actvshadowhdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, enum pm_init_event type);


extern int __actvshadowhdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_pmcall_pwrmgr_system_state* actvhdlr_pmhandler_pwrmgr_system_state_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_pwrmgr_system_state));
	return (struct __actvret_pmcall_pwrmgr_system_state*)__ret;
}

static inline struct __actvret_pmcall_shutdown_enter* actvhdlr_pmhandler_shutdown_enter_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_shutdown_enter));
	return (struct __actvret_pmcall_shutdown_enter*)__ret;
}

static inline struct __actvret_pmcall_shutdown_softreboot_enter* actvhdlr_pmhandler_shutdown_softreboot_enter_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_shutdown_softreboot_enter));
	return (struct __actvret_pmcall_shutdown_softreboot_enter*)__ret;
}

static inline struct __actvret_pmcall_shutdown_reboot_enter* actvhdlr_pmhandler_shutdown_reboot_enter_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_shutdown_reboot_enter));
	return (struct __actvret_pmcall_shutdown_reboot_enter*)__ret;
}

static inline struct __actvret_pmcall_shutdown_kexec_enter* actvhdlr_pmhandler_shutdown_kexec_enter_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_shutdown_kexec_enter));
	return (struct __actvret_pmcall_shutdown_kexec_enter*)__ret;
}

static inline struct __actvret_pmcall_shutdown_reboot* actvhdlr_pmhandler_shutdown_reboot_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_shutdown_reboot));
	return (struct __actvret_pmcall_shutdown_reboot*)__ret;
}

static inline struct __actvret_pmcall_suspend_set_state* actvhdlr_pmhandler_suspend_set_state_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_suspend_set_state));
	return (struct __actvret_pmcall_suspend_set_state*)__ret;
}

static inline struct __actvret_pmcall_suspend_disk_restore* actvhdlr_pmhandler_suspend_disk_restore_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_suspend_disk_restore));
	return (struct __actvret_pmcall_suspend_disk_restore*)__ret;
}

static inline struct __actvret_pmcall_sysfs_read* actvhdlr_pmhandler_sysfs_read_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_sysfs_read));
	return (struct __actvret_pmcall_sysfs_read*)__ret;
}

static inline struct __actvret_pmcall_sysfs_write* actvhdlr_pmhandler_sysfs_write_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_sysfs_write));
	return (struct __actvret_pmcall_sysfs_write*)__ret;
}

static inline struct __actvret_pmcall_wakelock_action* actvhdlr_pmhandler_wakelock_action_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_wakelock_action));
	return (struct __actvret_pmcall_wakelock_action*)__ret;
}

static inline struct __actvret_pmcall_wakelock_acquire_timeout* actvhdlr_pmhandler_wakelock_acquire_timeout_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_wakelock_acquire_timeout));
	return (struct __actvret_pmcall_wakelock_acquire_timeout*)__ret;
}

static inline struct __actvret_pmcall_wakelock_get_count* actvhdlr_pmhandler_wakelock_get_count_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_wakelock_get_count));
	return (struct __actvret_pmcall_wakelock_get_count*)__ret;
}

static inline struct __actvret_pmcall_wakelock_save_count* actvhdlr_pmhandler_wakelock_save_count_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_wakelock_save_count));
	return (struct __actvret_pmcall_wakelock_save_count*)__ret;
}

static inline struct __actvret_pmcall_wakelock_stat* actvhdlr_pmhandler_wakelock_stat_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_wakelock_stat));
	return (struct __actvret_pmcall_wakelock_stat*)__ret;
}

static inline struct __actvret_pmcall_cpuhp_add_notifier* actvhdlr_pmhandler_cpuhp_add_notifier_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_cpuhp_add_notifier));
	return (struct __actvret_pmcall_cpuhp_add_notifier*)__ret;
}

static inline struct __actvret_pmcall_cpuhp_remove_notifier* actvhdlr_pmhandler_cpuhp_remove_notifier_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_cpuhp_remove_notifier));
	return (struct __actvret_pmcall_cpuhp_remove_notifier*)__ret;
}

static inline struct __actvret_pmcall_cpuhp_action* actvhdlr_pmhandler_cpuhp_action_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_cpuhp_action));
	return (struct __actvret_pmcall_cpuhp_action*)__ret;
}

static inline struct __actvret_pmcall_cpuhp_revoke_listener* actvhdlr_pmhandler_cpuhp_revoke_listener_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_cpuhp_revoke_listener));
	return (struct __actvret_pmcall_cpuhp_revoke_listener*)__ret;
}

static inline struct __actvret_pmcall_pmqos_request_add* actvhdlr_pmhandler_pmqos_request_add_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_pmqos_request_add));
	return (struct __actvret_pmcall_pmqos_request_add*)__ret;
}

static inline struct __actvret_pmcall_pmqos_request_remove* actvhdlr_pmhandler_pmqos_request_remove_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_pmqos_request_remove));
	return (struct __actvret_pmcall_pmqos_request_remove*)__ret;
}

static inline struct __actvret_pmcall_pmqos_request_update* actvhdlr_pmhandler_pmqos_request_update_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_pmqos_request_update));
	return (struct __actvret_pmcall_pmqos_request_update*)__ret;
}

static inline struct __actvret_pmcall_dh_register* actvhdlr_pmhandler_dh_register_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_dh_register));
	return (struct __actvret_pmcall_dh_register*)__ret;
}

static inline struct __actvret_pmcall_sec_mgmt* actvhdlr_pmhandler_sec_mgmt_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_sec_mgmt));
	return (struct __actvret_pmcall_sec_mgmt*)__ret;
}

static inline struct __actvret_pmcall_pac_cfi* actvhdlr_pmhandler_pac_cfi_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_pac_cfi));
	return (struct __actvret_pmcall_pac_cfi*)__ret;
}

static inline struct __actvret_pmcall_thermmgr_stat* actvhdlr_pmhandler_thermmgr_stat_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_thermmgr_stat));
	return (struct __actvret_pmcall_thermmgr_stat*)__ret;
}

static inline struct __actvret_pmcall_idlemgr_stat* actvhdlr_pmhandler_idlemgr_stat_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_idlemgr_stat));
	return (struct __actvret_pmcall_idlemgr_stat*)__ret;
}

static inline struct __actvret_pmcall_pm_init_notify* actvhdlr_pmhandler_pm_init_notify_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_pmcall_pm_init_notify));
	return (struct __actvret_pmcall_pm_init_notify*)__ret;
}

#ifdef __sysif_server_pwrmgr_no_compat32_handlers__
#ifdef __HOST_LLT__
union pwrmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgehdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, int state);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state);
	int (*p__actvbridgehdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf);
	int (*p__actvbridgehdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgehdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned int count);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count);
	int (*p__actvbridgehdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned int type, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value);
	int (*p__actvbridgehdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgecapcallhdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgehdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);
	int (*p__actvbridgecapcallhdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command);
	int (*p__actvbridgehdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, enum pm_init_event type);
	int (*p__actvbridgecapcallhdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union pwrmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgehdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, int state);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state);
	int (*p__actvbridgehdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf);
	int (*p__actvbridgehdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgehdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned int count);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count);
	int (*p__actvbridgehdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned int type, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value);
	int (*p__actvbridgehdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgecapcallhdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgehdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);
	int (*p__actvbridgecapcallhdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command);
	int (*p__actvbridgehdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, enum pm_init_event type);
	int (*p__actvbridgecapcallhdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union pwrmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgehdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, int state);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state);
	int (*p__actvbridgehdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf);
	int (*p__actvbridgehdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgehdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned int count);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count);
	int (*p__actvbridgehdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned int type, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value);
	int (*p__actvbridgehdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgecapcallhdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgehdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);
	int (*p__actvbridgecapcallhdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command);
	int (*p__actvbridgehdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, enum pm_init_event type);
	int (*p__actvbridgecapcallhdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union pwrmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgehdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, int state);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state);
	int (*p__actvbridgehdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf);
	int (*p__actvbridgehdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgehdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned int count);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count);
	int (*p__actvbridgehdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned int type, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value);
	int (*p__actvbridgehdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgecapcallhdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgehdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);
	int (*p__actvbridgecapcallhdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command);
	int (*p__actvbridgehdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, enum pm_init_event type);
	int (*p__actvbridgecapcallhdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type);
	int (*p__actvbridgehdlr_compat_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union pwrmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, long timeout_ms);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms);
	int (*p__actvbridgehdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg);
	int (*p__actvbridgehdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, int state);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state);
	int (*p__actvbridgehdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf);
	int (*p__actvbridgehdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name);
	int (*p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec);
	int (*p__actvbridgehdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned int count);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count);
	int (*p__actvbridgehdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event);
	int (*p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned int type, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request);
	int (*p__actvbridgehdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value);
	int (*p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value);
	int (*p__actvbridgehdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgecapcallhdlr_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type);
	int (*p__actvbridgehdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command);
	int (*p__actvbridgecapcallhdlr_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command);
	int (*p__actvbridgehdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, enum pm_init_event type);
	int (*p__actvbridgecapcallhdlr_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type);
	int (*p__actvbridgehdlr_compat_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_dh_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_pwrmgr_no_compat32_handlers__) && defined(__sysif_server_pwrmgr_no_ilp32_handlers__)
#define __pwrmgr_method_TIMES 1
#elif !defined(__sysif_server_pwrmgr_no_compat32_handlers__) && !defined(__sysif_server_pwrmgr_no_ilp32_handlers__)
#define __pwrmgr_method_TIMES 3
#else
#define __pwrmgr_method_TIMES 2
#endif
extern union pwrmgr_hdlrs_union sysif_pwrmgr_actvhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES];

extern union pwrmgr_hdlrs_union sysif_pwrmgr_actvcapcallhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES];

extern void sysif_show_pwrmgr_methods(void);


#ifdef SYSIF_GEN_IFACE
#include <stdint.h>
#include <hmasm/actv_rpc.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libsysif/base/server_arg_compat.h>
#ifdef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
static int __opt_nonoframe __actvbridgehdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, long timeout_ms)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_reboot_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_cmd, cmd), __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_reboot_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_cmd, cmd), __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, long timeout_ms)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_kexec_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_kexec_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, magic1), __ARCH_ACTV_RPC_RECV_ARG(int, magic2), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(void*, cmd_arg));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, int state)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_suspend_set_state(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, state));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_suspend_set_state(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, state));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sysfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, actv_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sysfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, actv_buf));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, action), __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, action), __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, wakelock), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, msec));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, wakelock), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, msec));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned int count)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_save_count(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, count));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_save_count(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, count));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpu), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, event));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpu), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, event));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned int type, int value)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_add(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_add(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_remove(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_remove(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_update(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_update(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_dh_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dh_ap_uref), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_dh_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dh_ap_uref), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sec_mgmt(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, module), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, command));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sec_mgmt(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, module), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, command));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_thermmgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct thermal_zone_type, type), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_thermmgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct thermal_zone_type, type), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_idlemgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_idlemgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, enum pm_init_event type)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pm_init_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum pm_init_event, type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pm_init_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum pm_init_event, type));
}

#ifndef __sysif_server_pwrmgr_no_compat32_handlers__
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
struct compat_x3 {
	unsigned int reserverd;
	unsigned int w3;
};

#else
struct compat_x3 {
	unsigned int w3;
	unsigned int reserverd;
};

#endif /* CONFIG_BIG_ENDIAN */
static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	long __maybe_unused _timeout_ms;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long);
		type = __type_of(long);
		__fetch_one_arg((void*)&_timeout_ms, data, 0, arg_sz, model, type, long, _timeout_ms)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_enter(sender, credential, _timeout_ms);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	long __maybe_unused _timeout_ms;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long);
		type = __type_of(long);
		__fetch_one_arg((void*)&_timeout_ms, data, 0, arg_sz, model, type, long, _timeout_ms)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_enter(sender, credential, _timeout_ms);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct reboot_cmd __maybe_unused _cmd;
	long __maybe_unused _timeout_ms;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct reboot_cmd);
		type = __type_of(struct reboot_cmd);
		__fetch_one_arg((void*)&_cmd, data, 0, arg_sz, model, type, struct reboot_cmd, _cmd)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long);
		type = __type_of(long);
		__fetch_one_arg((void*)&_timeout_ms, data, 1, arg_sz, model, type, long, _timeout_ms)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_reboot_enter(sender, credential, _cmd, _timeout_ms);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct reboot_cmd __maybe_unused _cmd;
	long __maybe_unused _timeout_ms;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct reboot_cmd);
		type = __type_of(struct reboot_cmd);
		__fetch_one_arg((void*)&_cmd, data, 0, arg_sz, model, type, struct reboot_cmd, _cmd)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long);
		type = __type_of(long);
		__fetch_one_arg((void*)&_timeout_ms, data, 1, arg_sz, model, type, long, _timeout_ms)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_reboot_enter(sender, credential, _cmd, _timeout_ms);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	long __maybe_unused _timeout_ms;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long);
		type = __type_of(long);
		__fetch_one_arg((void*)&_timeout_ms, data, 0, arg_sz, model, type, long, _timeout_ms)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_kexec_enter(sender, credential, _timeout_ms);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	long __maybe_unused _timeout_ms;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long);
		type = __type_of(long);
		__fetch_one_arg((void*)&_timeout_ms, data, 0, arg_sz, model, type, long, _timeout_ms)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_kexec_enter(sender, credential, _timeout_ms);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	int __maybe_unused _magic1;
	int __maybe_unused _magic2;
	unsigned int __maybe_unused _type;
	__maybe_unused void *_cmd_arg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_magic1, data, 0, arg_sz, model, type, int, _magic1)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_magic2, data, 1, arg_sz, model, type, int, _magic2)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 2, arg_sz, model, type, unsigned int, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_cmd_arg, data, 3, arg_sz, model, type, void*, _cmd_arg)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_shutdown_reboot(sender, credential, _magic1, _magic2, _type, _cmd_arg);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	int __maybe_unused _state;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_state, data, 0, arg_sz, model, type, int, _state)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_suspend_set_state(sender, credential, _state);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	int __maybe_unused _state;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_state, data, 0, arg_sz, model, type, int, _state)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_suspend_set_state(sender, credential, _state);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct hm_actv_buf __maybe_unused _actv_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_actv_buf, data, 0, arg_sz, model, type, struct hm_actv_buf, _actv_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sysfs_read(sender, credential, _actv_buf);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct hm_actv_buf __maybe_unused _actv_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_actv_buf, data, 0, arg_sz, model, type, struct hm_actv_buf, _actv_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sysfs_read(sender, credential, _actv_buf);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _action;
	struct wake_lock_name __maybe_unused _name;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_action, data, 0, arg_sz, model, type, unsigned int, _action)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct wake_lock_name);
		type = __type_of(struct wake_lock_name);
		__fetch_one_arg((void*)&_name, data, 1, arg_sz, model, type, struct wake_lock_name, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_action(sender, credential, _action, _name);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _action;
	struct wake_lock_name __maybe_unused _name;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_action, data, 0, arg_sz, model, type, unsigned int, _action)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct wake_lock_name);
		type = __type_of(struct wake_lock_name);
		__fetch_one_arg((void*)&_name, data, 1, arg_sz, model, type, struct wake_lock_name, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_action(sender, credential, _action, _name);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct wake_lock_name __maybe_unused _wakelock;
	unsigned long __maybe_unused _msec;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct wake_lock_name);
		type = __type_of(struct wake_lock_name);
		__fetch_one_arg((void*)&_wakelock, data, 0, arg_sz, model, type, struct wake_lock_name, _wakelock)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long);
		type = __type_of(unsigned long);
		__fetch_one_arg((void*)&_msec, data, 1, arg_sz, model, type, unsigned long, _msec)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, _wakelock, _msec);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct wake_lock_name __maybe_unused _wakelock;
	unsigned long __maybe_unused _msec;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct wake_lock_name);
		type = __type_of(struct wake_lock_name);
		__fetch_one_arg((void*)&_wakelock, data, 0, arg_sz, model, type, struct wake_lock_name, _wakelock)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long);
		type = __type_of(unsigned long);
		__fetch_one_arg((void*)&_msec, data, 1, arg_sz, model, type, unsigned long, _msec)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, _wakelock, _msec);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _count;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_count, data, 0, arg_sz, model, type, unsigned int, _count)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_save_count(sender, credential, _count);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _count;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_count, data, 0, arg_sz, model, type, unsigned int, _count)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_save_count(sender, credential, _count);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct wake_lock_name __maybe_unused _name;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct wake_lock_name);
		type = __type_of(struct wake_lock_name);
		__fetch_one_arg((void*)&_name, data, 0, arg_sz, model, type, struct wake_lock_name, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 1, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_stat(sender, credential, _name, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct wake_lock_name __maybe_unused _name;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct wake_lock_name);
		type = __type_of(struct wake_lock_name);
		__fetch_one_arg((void*)&_name, data, 0, arg_sz, model, type, struct wake_lock_name, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 1, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_wakelock_stat(sender, credential, _name, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cpu;
	unsigned int __maybe_unused _event;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cpu, data, 0, arg_sz, model, type, unsigned int, _cpu)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_event, data, 1, arg_sz, model, type, unsigned int, _event)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_action(sender, credential, _cpu, _event);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cpu;
	unsigned int __maybe_unused _event;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cpu, data, 0, arg_sz, model, type, unsigned int, _cpu)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_event, data, 1, arg_sz, model, type, unsigned int, _event)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_action(sender, credential, _cpu, _event);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cnode_idx, data, 0, arg_sz, model, type, unsigned int, _cnode_idx)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cnode_idx, data, 0, arg_sz, model, type, unsigned int, _cnode_idx)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _type;
	int __maybe_unused _value;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, unsigned int, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_value, data, 1, arg_sz, model, type, int, _value)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_add(sender, credential, _type, _value);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _type;
	int __maybe_unused _value;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, unsigned int, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_value, data, 1, arg_sz, model, type, int, _value)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_add(sender, credential, _type, _value);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct hm_pmqos_request __maybe_unused _request;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_pmqos_request);
		type = __type_of(struct hm_pmqos_request);
		__fetch_one_arg((void*)&_request, data, 0, arg_sz, model, type, struct hm_pmqos_request, _request)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_remove(sender, credential, _request);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct hm_pmqos_request __maybe_unused _request;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_pmqos_request);
		type = __type_of(struct hm_pmqos_request);
		__fetch_one_arg((void*)&_request, data, 0, arg_sz, model, type, struct hm_pmqos_request, _request)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_remove(sender, credential, _request);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct hm_pmqos_request __maybe_unused _request;
	int __maybe_unused _value;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_pmqos_request);
		type = __type_of(struct hm_pmqos_request);
		__fetch_one_arg((void*)&_request, data, 0, arg_sz, model, type, struct hm_pmqos_request, _request)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_value, data, 1, arg_sz, model, type, int, _value)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_update(sender, credential, _request, _value);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct hm_pmqos_request __maybe_unused _request;
	int __maybe_unused _value;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_pmqos_request);
		type = __type_of(struct hm_pmqos_request);
		__fetch_one_arg((void*)&_request, data, 0, arg_sz, model, type, struct hm_pmqos_request, _request)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_value, data, 1, arg_sz, model, type, int, _value)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pmqos_request_update(sender, credential, _request, _value);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _dh_ap_uref;
	unsigned int __maybe_unused _type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dh_ap_uref, data, 0, arg_sz, model, type, unsigned long long, _dh_ap_uref)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 1, arg_sz, model, type, unsigned int, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_dh_register(sender, credential, _dh_ap_uref, _type);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _dh_ap_uref;
	unsigned int __maybe_unused _type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dh_ap_uref, data, 0, arg_sz, model, type, unsigned long long, _dh_ap_uref)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 1, arg_sz, model, type, unsigned int, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_dh_register(sender, credential, _dh_ap_uref, _type);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _module;
	unsigned int __maybe_unused _command;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_module, data, 0, arg_sz, model, type, unsigned int, _module)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_command, data, 1, arg_sz, model, type, unsigned int, _command)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sec_mgmt(sender, credential, _module, _command);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _module;
	unsigned int __maybe_unused _command;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_module, data, 0, arg_sz, model, type, unsigned int, _module)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_command, data, 1, arg_sz, model, type, unsigned int, _command)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_sec_mgmt(sender, credential, _module, _command);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct thermal_zone_type __maybe_unused _type;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct thermal_zone_type);
		type = __type_of(struct thermal_zone_type);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, struct thermal_zone_type, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 1, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_thermmgr_stat(sender, credential, _type, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct thermal_zone_type __maybe_unused _type;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct thermal_zone_type);
		type = __type_of(struct thermal_zone_type);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, struct thermal_zone_type, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 1, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_thermmgr_stat(sender, credential, _type, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 0, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_idlemgr_stat(sender, credential, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 0, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_idlemgr_stat(sender, credential, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	enum pm_init_event __maybe_unused _type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum pm_init_event);
		type = __type_of(enum pm_init_event);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, enum pm_init_event, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pm_init_notify(sender, credential, _type);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	enum pm_init_event __maybe_unused _type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum pm_init_event);
		type = __type_of(enum pm_init_event);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, enum pm_init_event, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_pmhandler_pm_init_notify(sender, credential, _type);
}

#endif /* compat handlers */
#ifndef __sysif_server_pwrmgr_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, long timeout_ms)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(long, timeout_ms)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(long, timeout_ms)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct reboot_cmd, cmd)
	} while (0);
	do {
		__adjust_one_arg(long, timeout_ms)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_reboot_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_cmd, cmd), __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct reboot_cmd, cmd)
	} while (0);
	do {
		__adjust_one_arg(long, timeout_ms)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_reboot_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_cmd, cmd), __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, long timeout_ms)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(long, timeout_ms)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_kexec_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(long, timeout_ms)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_kexec_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, magic1)
	} while (0);
	do {
		__adjust_one_arg(int, magic2)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, type)
	} while (0);
	do {
		__adjust_one_arg(void*, cmd_arg)
	} while (0);
	return __actvshadowhdlr_pmhandler_shutdown_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, magic1), __ARCH_ACTV_RPC_RECV_ARG(int, magic2), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(void*, cmd_arg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, int state)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, state)
	} while (0);
	return __actvshadowhdlr_pmhandler_suspend_set_state(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, state));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, state)
	} while (0);
	return __actvshadowhdlr_pmhandler_suspend_set_state(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, state));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hm_actv_buf, actv_buf)
	} while (0);
	return __actvshadowhdlr_pmhandler_sysfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, actv_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hm_actv_buf, actv_buf)
	} while (0);
	return __actvshadowhdlr_pmhandler_sysfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, actv_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, action)
	} while (0);
	do {
		__adjust_one_arg(struct wake_lock_name, name)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, action), __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, action)
	} while (0);
	do {
		__adjust_one_arg(struct wake_lock_name, name)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, action), __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct wake_lock_name, wakelock)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, msec)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, wakelock), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, msec));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct wake_lock_name, wakelock)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, msec)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, wakelock), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, msec));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned int count)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, count)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_save_count(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, count));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, count)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_save_count(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, count));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct wake_lock_name, name)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct wake_lock_name, name)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_pmhandler_wakelock_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cpu)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, event)
	} while (0);
	return __actvshadowhdlr_pmhandler_cpuhp_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpu), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, event));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cpu)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, event)
	} while (0);
	return __actvshadowhdlr_pmhandler_cpuhp_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpu), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, event));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cnode_idx)
	} while (0);
	return __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cnode_idx)
	} while (0);
	return __actvshadowhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned int type, int value)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, type)
	} while (0);
	do {
		__adjust_one_arg(int, value)
	} while (0);
	return __actvshadowhdlr_pmhandler_pmqos_request_add(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, type)
	} while (0);
	do {
		__adjust_one_arg(int, value)
	} while (0);
	return __actvshadowhdlr_pmhandler_pmqos_request_add(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hm_pmqos_request, request)
	} while (0);
	return __actvshadowhdlr_pmhandler_pmqos_request_remove(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hm_pmqos_request, request)
	} while (0);
	return __actvshadowhdlr_pmhandler_pmqos_request_remove(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hm_pmqos_request, request)
	} while (0);
	do {
		__adjust_one_arg(int, value)
	} while (0);
	return __actvshadowhdlr_pmhandler_pmqos_request_update(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hm_pmqos_request, request)
	} while (0);
	do {
		__adjust_one_arg(int, value)
	} while (0);
	return __actvshadowhdlr_pmhandler_pmqos_request_update(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, dh_ap_uref)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, type)
	} while (0);
	return __actvshadowhdlr_pmhandler_dh_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dh_ap_uref), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_dh_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, dh_ap_uref)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, type)
	} while (0);
	return __actvshadowhdlr_pmhandler_dh_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dh_ap_uref), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, module)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, command)
	} while (0);
	return __actvshadowhdlr_pmhandler_sec_mgmt(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, module), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, command));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, module)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, command)
	} while (0);
	return __actvshadowhdlr_pmhandler_sec_mgmt(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, module), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, command));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct thermal_zone_type, type)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_pmhandler_thermmgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct thermal_zone_type, type), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct thermal_zone_type, type)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_pmhandler_thermmgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct thermal_zone_type, type), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_pmhandler_idlemgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_pmhandler_idlemgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, enum pm_init_event type)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum pm_init_event, type)
	} while (0);
	return __actvshadowhdlr_pmhandler_pm_init_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum pm_init_event, type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum pm_init_event, type)
	} while (0);
	return __actvshadowhdlr_pmhandler_pm_init_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum pm_init_event, type));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_pwrmgr_system_state(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, long timeout_ms)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_shutdown_softreboot_enter(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, struct reboot_cmd cmd, long timeout_ms)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_reboot_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_cmd, cmd), __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_cmd cmd, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_reboot_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_cmd, cmd), __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, long timeout_ms)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_kexec_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, long timeout_ms)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_kexec_enter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(long, timeout_ms));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_shutdown_reboot(unsigned long long sender, unsigned long credential, int magic1, int magic2, unsigned int type, void *cmd_arg)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_shutdown_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, magic1), __ARCH_ACTV_RPC_RECV_ARG(int, magic2), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(void*, cmd_arg));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, int state)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_suspend_set_state(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, state));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_suspend_set_state(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_suspend_set_state(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, state));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_suspend_disk_restore(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_suspend_disk_restore(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, struct hm_actv_buf actv_buf)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_sysfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, actv_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_sysfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_actv_buf actv_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_sysfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, actv_buf));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_sysfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_sysfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, unsigned int action, struct wake_lock_name name)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, action), __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_action(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int action, struct wake_lock_name name)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, action), __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, struct wake_lock_name wakelock, unsigned long msec)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, wakelock), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, msec));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name wakelock, unsigned long msec)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_acquire_timeout(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, wakelock), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, msec));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_get_count(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_wakelock_get_count(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, unsigned int count)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_save_count(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, count));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_save_count(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int count)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_save_count(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, count));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_wakelock_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wake_lock_name), __arch_actv_rpc_stksz_alignment(struct wake_lock_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_wakelock_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct wake_lock_name, name), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_cpuhp_add_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_cpuhp_remove_notifier(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, unsigned int cpu, unsigned int event)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_cpuhp_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpu), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, event));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_action(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpu, unsigned int event)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_cpuhp_action(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpu), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, event));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_cpuhp_revoke_listener(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, unsigned int type, int value)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pmqos_request_add(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pmqos_request_add(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type, int value)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pmqos_request_add(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pmqos_request_remove(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pmqos_request_remove(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pmqos_request_remove(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, struct hm_pmqos_request request, int value)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pmqos_request_update(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pmqos_request_update(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hm_pmqos_request request, int value)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_pmqos_request), __arch_actv_rpc_stksz_alignment(struct hm_pmqos_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pmqos_request_update(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hm_pmqos_request, request), __ARCH_ACTV_RPC_RECV_ARG(int, value));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, unsigned long long dh_ap_uref, unsigned int type)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_dh_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dh_ap_uref), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_dh_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long dh_ap_uref, unsigned int type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_dh_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dh_ap_uref), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, unsigned int module, unsigned int command)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_sec_mgmt(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, module), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, command));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_sec_mgmt(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int module, unsigned int command)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_sec_mgmt(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, module), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, command));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_pmhandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_thermmgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct thermal_zone_type, type), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_thermmgr_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct thermal_zone_type), __arch_actv_rpc_stksz_alignment(struct thermal_zone_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_thermmgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct thermal_zone_type, type), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_idlemgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_idlemgr_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_idlemgr_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, enum pm_init_event type)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pm_init_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum pm_init_event, type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_pmhandler_pm_init_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum pm_init_event type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum pm_init_event), __arch_actv_rpc_stksz_alignment(enum pm_init_event));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_pmhandler_pm_init_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum pm_init_event, type));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_pwrmgr_no_compat32_handlers__) && defined(__sysif_server_pwrmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_enter = (&__actvbridgehdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_pmhandler_suspend_set_state = (&__actvbridgehdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_pmhandler_sysfs_read = (&__actvbridgehdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_pmhandler_sysfs_write = (&__actvbridgehdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_pmhandler_wakelock_action = (&__actvbridgehdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_get_count = (&__actvbridgehdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_save_count = (&__actvbridgehdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_pmhandler_wakelock_stat = (&__actvbridgehdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_action = (&__actvbridgehdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_add = (&__actvbridgehdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_update = (&__actvbridgehdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_pmhandler_dh_register = (&__actvbridgehdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_pmhandler_sec_mgmt = (&__actvbridgehdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_pmhandler_pac_cfi = (&__actvbridgehdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_pmhandler_thermmgr_stat = (&__actvbridgehdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_pmhandler_idlemgr_stat = (&__actvbridgehdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_pmhandler_pm_init_notify = (&__actvbridgehdlr_pmhandler_pm_init_notify)
	}
};

__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvcapcallhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_pmhandler_dh_register = (&__actvbridgecapcallhdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_pmhandler_pm_init_notify)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_pwrmgr_no_compat32_handlers__) && defined(__sysif_server_pwrmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_enter = (&__actvbridgehdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_pmhandler_suspend_set_state = (&__actvbridgehdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_pmhandler_sysfs_read = (&__actvbridgehdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_pmhandler_sysfs_write = (&__actvbridgehdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_pmhandler_wakelock_action = (&__actvbridgehdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_get_count = (&__actvbridgehdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_save_count = (&__actvbridgehdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_pmhandler_wakelock_stat = (&__actvbridgehdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_action = (&__actvbridgehdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_add = (&__actvbridgehdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_update = (&__actvbridgehdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_pmhandler_dh_register = (&__actvbridgehdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_pmhandler_sec_mgmt = (&__actvbridgehdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_pmhandler_pac_cfi = (&__actvbridgehdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_pmhandler_thermmgr_stat = (&__actvbridgehdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_pmhandler_idlemgr_stat = (&__actvbridgehdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_pmhandler_pm_init_notify = (&__actvbridgehdlr_pmhandler_pm_init_notify)
	},
	[__pwrmgr_method_MAX ... __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_compat_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_compat_pmhandler_pwrmgr_system_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_reboot = (&__actvbridgehdlr_compat_pmhandler_shutdown_reboot)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_compat_pmhandler_suspend_set_state = (&__actvbridgehdlr_compat_pmhandler_suspend_set_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_compat_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_compat_pmhandler_suspend_disk_restore)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_compat_pmhandler_sysfs_read = (&__actvbridgehdlr_compat_pmhandler_sysfs_read)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_compat_pmhandler_sysfs_write = (&__actvbridgehdlr_compat_pmhandler_sysfs_write)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_action = (&__actvbridgehdlr_compat_pmhandler_wakelock_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_get_count = (&__actvbridgehdlr_compat_pmhandler_wakelock_get_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_save_count = (&__actvbridgehdlr_compat_pmhandler_wakelock_save_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_stat = (&__actvbridgehdlr_compat_pmhandler_wakelock_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_action = (&__actvbridgehdlr_compat_pmhandler_cpuhp_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_compat_pmhandler_pmqos_request_add = (&__actvbridgehdlr_compat_pmhandler_pmqos_request_add)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_compat_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_compat_pmhandler_pmqos_request_remove)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_compat_pmhandler_pmqos_request_update = (&__actvbridgehdlr_compat_pmhandler_pmqos_request_update)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_compat_pmhandler_dh_register = (&__actvbridgehdlr_compat_pmhandler_dh_register)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_compat_pmhandler_sec_mgmt = (&__actvbridgehdlr_compat_pmhandler_sec_mgmt)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_compat_pmhandler_pac_cfi = (&__actvbridgehdlr_compat_pmhandler_pac_cfi)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_compat_pmhandler_thermmgr_stat = (&__actvbridgehdlr_compat_pmhandler_thermmgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_compat_pmhandler_idlemgr_stat = (&__actvbridgehdlr_compat_pmhandler_idlemgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_compat_pmhandler_pm_init_notify = (&__actvbridgehdlr_compat_pmhandler_pm_init_notify)
	}
};

__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvcapcallhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_pmhandler_dh_register = (&__actvbridgecapcallhdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_pmhandler_pm_init_notify)
	},
	[__pwrmgr_method_MAX ... __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot = (&__actvbridgehdlr_compat_pmhandler_shutdown_reboot)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_compat_pmhandler_sysfs_read)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_compat_pmhandler_sysfs_write)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_dh_register = (&__actvbridgecapcallhdlr_compat_pmhandler_dh_register)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_compat_pmhandler_pac_cfi)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_pwrmgr_no_compat32_handlers__) && !defined(__sysif_server_pwrmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_enter = (&__actvbridgehdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_pmhandler_suspend_set_state = (&__actvbridgehdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_pmhandler_sysfs_read = (&__actvbridgehdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_pmhandler_sysfs_write = (&__actvbridgehdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_pmhandler_wakelock_action = (&__actvbridgehdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_get_count = (&__actvbridgehdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_save_count = (&__actvbridgehdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_pmhandler_wakelock_stat = (&__actvbridgehdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_action = (&__actvbridgehdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_add = (&__actvbridgehdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_update = (&__actvbridgehdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_pmhandler_dh_register = (&__actvbridgehdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_pmhandler_sec_mgmt = (&__actvbridgehdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_pmhandler_pac_cfi = (&__actvbridgehdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_pmhandler_thermmgr_stat = (&__actvbridgehdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_pmhandler_idlemgr_stat = (&__actvbridgehdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_pmhandler_pm_init_notify = (&__actvbridgehdlr_pmhandler_pm_init_notify)
	},
	[__pwrmgr_method_MAX ... __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_ilp32_pmhandler_pwrmgr_system_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_softreboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_reboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_kexec_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_reboot)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_pmhandler_suspend_set_state = (&__actvbridgehdlr_ilp32_pmhandler_suspend_set_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_ilp32_pmhandler_suspend_disk_restore)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_pmhandler_sysfs_read = (&__actvbridgehdlr_ilp32_pmhandler_sysfs_read)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_pmhandler_sysfs_write = (&__actvbridgehdlr_ilp32_pmhandler_sysfs_write)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_pmhandler_wakelock_action = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_acquire_timeout)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_get_count = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_get_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_save_count = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_save_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_pmhandler_wakelock_stat = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_add_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_remove_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_action = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_revoke_listener)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_add = (&__actvbridgehdlr_ilp32_pmhandler_pmqos_request_add)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_ilp32_pmhandler_pmqos_request_remove)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_update = (&__actvbridgehdlr_ilp32_pmhandler_pmqos_request_update)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_pmhandler_dh_register = (&__actvbridgehdlr_ilp32_pmhandler_dh_register)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_pmhandler_sec_mgmt = (&__actvbridgehdlr_ilp32_pmhandler_sec_mgmt)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_pmhandler_pac_cfi = (&__actvbridgehdlr_ilp32_pmhandler_pac_cfi)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_pmhandler_thermmgr_stat = (&__actvbridgehdlr_ilp32_pmhandler_thermmgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_pmhandler_idlemgr_stat = (&__actvbridgehdlr_ilp32_pmhandler_idlemgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_pmhandler_pm_init_notify = (&__actvbridgehdlr_ilp32_pmhandler_pm_init_notify)
	}
};

__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvcapcallhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_pmhandler_dh_register = (&__actvbridgecapcallhdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_pmhandler_pm_init_notify)
	},
	[__pwrmgr_method_MAX ... __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pwrmgr_system_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_softreboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_reboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_kexec_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_reboot)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_ilp32_pmhandler_suspend_set_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_ilp32_pmhandler_suspend_disk_restore)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_ilp32_pmhandler_sysfs_read)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_ilp32_pmhandler_sysfs_write)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_acquire_timeout)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_get_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_save_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_add_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_remove_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_revoke_listener)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_add)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_remove)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_update)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_pmhandler_dh_register = (&__actvbridgecapcallhdlr_ilp32_pmhandler_dh_register)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_ilp32_pmhandler_sec_mgmt)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pac_cfi)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_ilp32_pmhandler_thermmgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_ilp32_pmhandler_idlemgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pm_init_notify)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_pwrmgr_no_compat32_handlers__) && !defined(__sysif_server_pwrmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_enter = (&__actvbridgehdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_pmhandler_suspend_set_state = (&__actvbridgehdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_pmhandler_sysfs_read = (&__actvbridgehdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_pmhandler_sysfs_write = (&__actvbridgehdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_pmhandler_wakelock_action = (&__actvbridgehdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_get_count = (&__actvbridgehdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_save_count = (&__actvbridgehdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_pmhandler_wakelock_stat = (&__actvbridgehdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_action = (&__actvbridgehdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_add = (&__actvbridgehdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_update = (&__actvbridgehdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_pmhandler_dh_register = (&__actvbridgehdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_pmhandler_sec_mgmt = (&__actvbridgehdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_pmhandler_pac_cfi = (&__actvbridgehdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_pmhandler_thermmgr_stat = (&__actvbridgehdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_pmhandler_idlemgr_stat = (&__actvbridgehdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_pmhandler_pm_init_notify = (&__actvbridgehdlr_pmhandler_pm_init_notify)
	},
	[__pwrmgr_method_MAX ... __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_compat_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_compat_pmhandler_pwrmgr_system_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_softreboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_reboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_compat_pmhandler_shutdown_kexec_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_compat_pmhandler_shutdown_reboot = (&__actvbridgehdlr_compat_pmhandler_shutdown_reboot)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_compat_pmhandler_suspend_set_state = (&__actvbridgehdlr_compat_pmhandler_suspend_set_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_compat_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_compat_pmhandler_suspend_disk_restore)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_compat_pmhandler_sysfs_read = (&__actvbridgehdlr_compat_pmhandler_sysfs_read)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_compat_pmhandler_sysfs_write = (&__actvbridgehdlr_compat_pmhandler_sysfs_write)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_action = (&__actvbridgehdlr_compat_pmhandler_wakelock_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_compat_pmhandler_wakelock_acquire_timeout)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_get_count = (&__actvbridgehdlr_compat_pmhandler_wakelock_get_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_save_count = (&__actvbridgehdlr_compat_pmhandler_wakelock_save_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_compat_pmhandler_wakelock_stat = (&__actvbridgehdlr_compat_pmhandler_wakelock_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_compat_pmhandler_cpuhp_add_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_compat_pmhandler_cpuhp_remove_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_action = (&__actvbridgehdlr_compat_pmhandler_cpuhp_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_compat_pmhandler_cpuhp_revoke_listener)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_compat_pmhandler_pmqos_request_add = (&__actvbridgehdlr_compat_pmhandler_pmqos_request_add)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_compat_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_compat_pmhandler_pmqos_request_remove)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_compat_pmhandler_pmqos_request_update = (&__actvbridgehdlr_compat_pmhandler_pmqos_request_update)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_compat_pmhandler_dh_register = (&__actvbridgehdlr_compat_pmhandler_dh_register)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_compat_pmhandler_sec_mgmt = (&__actvbridgehdlr_compat_pmhandler_sec_mgmt)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_compat_pmhandler_pac_cfi = (&__actvbridgehdlr_compat_pmhandler_pac_cfi)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_compat_pmhandler_thermmgr_stat = (&__actvbridgehdlr_compat_pmhandler_thermmgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_compat_pmhandler_idlemgr_stat = (&__actvbridgehdlr_compat_pmhandler_idlemgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_compat_pmhandler_pm_init_notify = (&__actvbridgehdlr_compat_pmhandler_pm_init_notify)
	},
	[2 * __pwrmgr_method_MAX ... 2 * __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgehdlr_pmhandler_pwrmgr_system_state = (&__actvbridgehdlr_ilp32_pmhandler_pwrmgr_system_state)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_softreboot_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_reboot_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgehdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_kexec_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgehdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_reboot)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgehdlr_pmhandler_suspend_set_state = (&__actvbridgehdlr_ilp32_pmhandler_suspend_set_state)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgehdlr_pmhandler_suspend_disk_restore = (&__actvbridgehdlr_ilp32_pmhandler_suspend_disk_restore)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgehdlr_pmhandler_sysfs_read = (&__actvbridgehdlr_ilp32_pmhandler_sysfs_read)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgehdlr_pmhandler_sysfs_write = (&__actvbridgehdlr_ilp32_pmhandler_sysfs_write)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgehdlr_pmhandler_wakelock_action = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_action)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgehdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_acquire_timeout)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_get_count = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_get_count)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgehdlr_pmhandler_wakelock_save_count = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_save_count)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgehdlr_pmhandler_wakelock_stat = (&__actvbridgehdlr_ilp32_pmhandler_wakelock_stat)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_add_notifier)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_remove_notifier)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_action = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_action)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgehdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgehdlr_ilp32_pmhandler_cpuhp_revoke_listener)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_add = (&__actvbridgehdlr_ilp32_pmhandler_pmqos_request_add)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_remove = (&__actvbridgehdlr_ilp32_pmhandler_pmqos_request_remove)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgehdlr_pmhandler_pmqos_request_update = (&__actvbridgehdlr_ilp32_pmhandler_pmqos_request_update)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgehdlr_pmhandler_dh_register = (&__actvbridgehdlr_ilp32_pmhandler_dh_register)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgehdlr_pmhandler_sec_mgmt = (&__actvbridgehdlr_ilp32_pmhandler_sec_mgmt)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgehdlr_pmhandler_pac_cfi = (&__actvbridgehdlr_ilp32_pmhandler_pac_cfi)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgehdlr_pmhandler_thermmgr_stat = (&__actvbridgehdlr_ilp32_pmhandler_thermmgr_stat)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgehdlr_pmhandler_idlemgr_stat = (&__actvbridgehdlr_ilp32_pmhandler_idlemgr_stat)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgehdlr_pmhandler_pm_init_notify = (&__actvbridgehdlr_ilp32_pmhandler_pm_init_notify)
	}
};

__ro_after_init_hdlr union pwrmgr_hdlrs_union sysif_pwrmgr_actvcapcallhdlr_table[__pwrmgr_method_MAX * __pwrmgr_method_TIMES] = {
	[0 ... __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state)
	},
	[ __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_enter)
	},
	[ __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter)
	},
	[ __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter)
	},
	[ __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter)
	},
	[ __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_pmhandler_shutdown_reboot)
	},
	[ __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_pmhandler_suspend_set_state)
	},
	[ __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore)
	},
	[ __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_pmhandler_sysfs_read)
	},
	[ __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_pmhandler_sysfs_write)
	},
	[ __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_pmhandler_wakelock_action)
	},
	[ __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout)
	},
	[ __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_get_count)
	},
	[ __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_pmhandler_wakelock_save_count)
	},
	[ __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_pmhandler_wakelock_stat)
	},
	[ __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier)
	},
	[ __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier)
	},
	[ __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_action)
	},
	[ __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener)
	},
	[ __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_add)
	},
	[ __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove)
	},
	[ __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_pmhandler_pmqos_request_update)
	},
	[ __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_pmhandler_dh_register = (&__actvbridgecapcallhdlr_pmhandler_dh_register)
	},
	[ __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_pmhandler_sec_mgmt)
	},
	[ __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_pmhandler_pac_cfi)
	},
	[ __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_pmhandler_thermmgr_stat)
	},
	[ __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_pmhandler_idlemgr_stat)
	},
	[ __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_pmhandler_pm_init_notify)
	},
	[__pwrmgr_method_MAX ... __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_compat_pmhandler_pwrmgr_system_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_softreboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_compat_pmhandler_shutdown_kexec_enter)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_shutdown_reboot = (&__actvbridgehdlr_compat_pmhandler_shutdown_reboot)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_compat_pmhandler_suspend_set_state)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_compat_pmhandler_suspend_disk_restore)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_compat_pmhandler_sysfs_read)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_compat_pmhandler_sysfs_write)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_acquire_timeout)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_get_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_save_count)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_compat_pmhandler_wakelock_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_add_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_remove_notifier)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_action)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_compat_pmhandler_cpuhp_revoke_listener)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_add)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_remove)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_compat_pmhandler_pmqos_request_update)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_dh_register = (&__actvbridgecapcallhdlr_compat_pmhandler_dh_register)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_compat_pmhandler_sec_mgmt)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_compat_pmhandler_pac_cfi)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_compat_pmhandler_thermmgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_compat_pmhandler_idlemgr_stat)
	},
	[__pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_compat_pmhandler_pm_init_notify)
	},
	[2 * __pwrmgr_method_MAX ... 2 * __pwrmgr_method_MAX + __pwrmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pwrmgr_system_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_pwrmgr_system_state = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pwrmgr_system_state)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_softreboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_softreboot_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_softreboot_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_reboot_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_kexec_enter] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_kexec_enter = (&__actvbridgecapcallhdlr_ilp32_pmhandler_shutdown_kexec_enter)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_shutdown_reboot] = {
		.p__actvbridgecapcallhdlr_pmhandler_shutdown_reboot = (&__actvbridgehdlr_ilp32_pmhandler_shutdown_reboot)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_suspend_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_suspend_set_state] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_set_state = (&__actvbridgecapcallhdlr_ilp32_pmhandler_suspend_set_state)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_suspend_disk_restore] = {
		.p__actvbridgecapcallhdlr_pmhandler_suspend_disk_restore = (&__actvbridgecapcallhdlr_ilp32_pmhandler_suspend_disk_restore)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_read] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_read = (&__actvbridgecapcallhdlr_ilp32_pmhandler_sysfs_read)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sysfs_write] = {
		.p__actvbridgecapcallhdlr_pmhandler_sysfs_write = (&__actvbridgecapcallhdlr_ilp32_pmhandler_sysfs_write)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_action = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_action)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_acquire_timeout] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_acquire_timeout = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_acquire_timeout)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_get_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_get_count = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_get_count)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_save_count] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_save_count = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_save_count)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_wakelock_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_wakelock_stat = (&__actvbridgecapcallhdlr_ilp32_pmhandler_wakelock_stat)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_add_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_add_notifier = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_add_notifier)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_remove_notifier] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_remove_notifier = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_remove_notifier)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_action] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_action = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_action)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_cpuhp_revoke_listener] = {
		.p__actvbridgecapcallhdlr_pmhandler_cpuhp_revoke_listener = (&__actvbridgecapcallhdlr_ilp32_pmhandler_cpuhp_revoke_listener)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_add] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_add = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_add)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_remove] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_remove = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_remove)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pmqos_request_update] = {
		.p__actvbridgecapcallhdlr_pmhandler_pmqos_request_update = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pmqos_request_update)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_dh_register] = {
		.p__actvbridgecapcallhdlr_pmhandler_dh_register = (&__actvbridgecapcallhdlr_ilp32_pmhandler_dh_register)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_sec_mgmt] = {
		.p__actvbridgecapcallhdlr_pmhandler_sec_mgmt = (&__actvbridgecapcallhdlr_ilp32_pmhandler_sec_mgmt)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_pmhandler_pac_cfi = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pac_cfi)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_thermmgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_thermmgr_stat = (&__actvbridgecapcallhdlr_ilp32_pmhandler_thermmgr_stat)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_idlemgr_stat] = {
		.p__actvbridgecapcallhdlr_pmhandler_idlemgr_stat = (&__actvbridgecapcallhdlr_ilp32_pmhandler_idlemgr_stat)
	},
	[2 * __pwrmgr_method_MAX + __pwrmgr_method_pm_init_notify] = {
		.p__actvbridgecapcallhdlr_pmhandler_pm_init_notify = (&__actvbridgecapcallhdlr_ilp32_pmhandler_pm_init_notify)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_pwrmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pwrmgr", "system_state", __pwrmgr_method_pwrmgr_system_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pwrmgr", "system_state", __pwrmgr_method_pwrmgr_system_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "enter", __pwrmgr_method_shutdown_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "enter", __pwrmgr_method_shutdown_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "softreboot_enter", __pwrmgr_method_shutdown_softreboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "softreboot_enter", __pwrmgr_method_shutdown_softreboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "reboot_enter", __pwrmgr_method_shutdown_reboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "reboot_enter", __pwrmgr_method_shutdown_reboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "kexec_enter", __pwrmgr_method_shutdown_kexec_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "kexec_enter", __pwrmgr_method_shutdown_kexec_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "reboot", __pwrmgr_method_shutdown_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "reboot", __pwrmgr_method_shutdown_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "suspend", "set_state", __pwrmgr_method_suspend_set_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "suspend", "set_state", __pwrmgr_method_suspend_set_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "suspend", "disk_restore", __pwrmgr_method_suspend_disk_restore);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "suspend", "disk_restore", __pwrmgr_method_suspend_disk_restore);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sysfs", "read", __pwrmgr_method_sysfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sysfs", "read", __pwrmgr_method_sysfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sysfs", "write", __pwrmgr_method_sysfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sysfs", "write", __pwrmgr_method_sysfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "action", __pwrmgr_method_wakelock_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "action", __pwrmgr_method_wakelock_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "acquire_timeout", __pwrmgr_method_wakelock_acquire_timeout);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "acquire_timeout", __pwrmgr_method_wakelock_acquire_timeout);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "get_count", __pwrmgr_method_wakelock_get_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "get_count", __pwrmgr_method_wakelock_get_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "save_count", __pwrmgr_method_wakelock_save_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "save_count", __pwrmgr_method_wakelock_save_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "stat", __pwrmgr_method_wakelock_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "stat", __pwrmgr_method_wakelock_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "add_notifier", __pwrmgr_method_cpuhp_add_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "add_notifier", __pwrmgr_method_cpuhp_add_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "remove_notifier", __pwrmgr_method_cpuhp_remove_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "remove_notifier", __pwrmgr_method_cpuhp_remove_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "action", __pwrmgr_method_cpuhp_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "action", __pwrmgr_method_cpuhp_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "revoke_listener", __pwrmgr_method_cpuhp_revoke_listener);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "revoke_listener", __pwrmgr_method_cpuhp_revoke_listener);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_add", __pwrmgr_method_pmqos_request_add);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_add", __pwrmgr_method_pmqos_request_add);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_remove", __pwrmgr_method_pmqos_request_remove);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_remove", __pwrmgr_method_pmqos_request_remove);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_update", __pwrmgr_method_pmqos_request_update);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_update", __pwrmgr_method_pmqos_request_update);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "dh", "register", __pwrmgr_method_dh_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "dh", "register", __pwrmgr_method_dh_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sec", "mgmt", __pwrmgr_method_sec_mgmt);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sec", "mgmt", __pwrmgr_method_sec_mgmt);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pac", "cfi", __pwrmgr_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pac", "cfi", __pwrmgr_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "thermmgr", "stat", __pwrmgr_method_thermmgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "thermmgr", "stat", __pwrmgr_method_thermmgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "idlemgr", "stat", __pwrmgr_method_idlemgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "idlemgr", "stat", __pwrmgr_method_idlemgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pm", "init_notify", __pwrmgr_method_pm_init_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pm", "init_notify", __pwrmgr_method_pm_init_notify);
	hm_info("%s_%s, max method num: %d\n", "pwrmgr", "method", __pwrmgr_method_MAX);
}

#elif defined(__arm__)
void sysif_show_pwrmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pwrmgr", "system_state", __pwrmgr_method_pwrmgr_system_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pwrmgr", "system_state", __pwrmgr_method_pwrmgr_system_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "enter", __pwrmgr_method_shutdown_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "enter", __pwrmgr_method_shutdown_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "softreboot_enter", __pwrmgr_method_shutdown_softreboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "softreboot_enter", __pwrmgr_method_shutdown_softreboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "reboot_enter", __pwrmgr_method_shutdown_reboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "reboot_enter", __pwrmgr_method_shutdown_reboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "kexec_enter", __pwrmgr_method_shutdown_kexec_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "kexec_enter", __pwrmgr_method_shutdown_kexec_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "reboot", __pwrmgr_method_shutdown_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "reboot", __pwrmgr_method_shutdown_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "suspend", "set_state", __pwrmgr_method_suspend_set_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "suspend", "set_state", __pwrmgr_method_suspend_set_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "suspend", "disk_restore", __pwrmgr_method_suspend_disk_restore);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "suspend", "disk_restore", __pwrmgr_method_suspend_disk_restore);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sysfs", "read", __pwrmgr_method_sysfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sysfs", "read", __pwrmgr_method_sysfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sysfs", "write", __pwrmgr_method_sysfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sysfs", "write", __pwrmgr_method_sysfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "action", __pwrmgr_method_wakelock_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "action", __pwrmgr_method_wakelock_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "acquire_timeout", __pwrmgr_method_wakelock_acquire_timeout);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "acquire_timeout", __pwrmgr_method_wakelock_acquire_timeout);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "get_count", __pwrmgr_method_wakelock_get_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "get_count", __pwrmgr_method_wakelock_get_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "save_count", __pwrmgr_method_wakelock_save_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "save_count", __pwrmgr_method_wakelock_save_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "stat", __pwrmgr_method_wakelock_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "stat", __pwrmgr_method_wakelock_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "add_notifier", __pwrmgr_method_cpuhp_add_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "add_notifier", __pwrmgr_method_cpuhp_add_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "remove_notifier", __pwrmgr_method_cpuhp_remove_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "remove_notifier", __pwrmgr_method_cpuhp_remove_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "action", __pwrmgr_method_cpuhp_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "action", __pwrmgr_method_cpuhp_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "revoke_listener", __pwrmgr_method_cpuhp_revoke_listener);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "revoke_listener", __pwrmgr_method_cpuhp_revoke_listener);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_add", __pwrmgr_method_pmqos_request_add);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_add", __pwrmgr_method_pmqos_request_add);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_remove", __pwrmgr_method_pmqos_request_remove);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_remove", __pwrmgr_method_pmqos_request_remove);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_update", __pwrmgr_method_pmqos_request_update);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_update", __pwrmgr_method_pmqos_request_update);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "dh", "register", __pwrmgr_method_dh_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "dh", "register", __pwrmgr_method_dh_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sec", "mgmt", __pwrmgr_method_sec_mgmt);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sec", "mgmt", __pwrmgr_method_sec_mgmt);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pac", "cfi", __pwrmgr_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pac", "cfi", __pwrmgr_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "thermmgr", "stat", __pwrmgr_method_thermmgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "thermmgr", "stat", __pwrmgr_method_thermmgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "idlemgr", "stat", __pwrmgr_method_idlemgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "idlemgr", "stat", __pwrmgr_method_idlemgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pm", "init_notify", __pwrmgr_method_pm_init_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pm", "init_notify", __pwrmgr_method_pm_init_notify);
	hm_info("%s_%s, max method num: %d\n", "pwrmgr", "method", __pwrmgr_method_MAX);
}

#else
void sysif_show_pwrmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pwrmgr", "system_state", __pwrmgr_method_pwrmgr_system_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pwrmgr", "system_state", __pwrmgr_method_pwrmgr_system_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "enter", __pwrmgr_method_shutdown_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "enter", __pwrmgr_method_shutdown_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "softreboot_enter", __pwrmgr_method_shutdown_softreboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "softreboot_enter", __pwrmgr_method_shutdown_softreboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "reboot_enter", __pwrmgr_method_shutdown_reboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "reboot_enter", __pwrmgr_method_shutdown_reboot_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "kexec_enter", __pwrmgr_method_shutdown_kexec_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "kexec_enter", __pwrmgr_method_shutdown_kexec_enter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "shutdown", "reboot", __pwrmgr_method_shutdown_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "shutdown", "reboot", __pwrmgr_method_shutdown_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "suspend", "set_state", __pwrmgr_method_suspend_set_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "suspend", "set_state", __pwrmgr_method_suspend_set_state);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "suspend", "disk_restore", __pwrmgr_method_suspend_disk_restore);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "suspend", "disk_restore", __pwrmgr_method_suspend_disk_restore);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sysfs", "read", __pwrmgr_method_sysfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sysfs", "read", __pwrmgr_method_sysfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sysfs", "write", __pwrmgr_method_sysfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sysfs", "write", __pwrmgr_method_sysfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "action", __pwrmgr_method_wakelock_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "action", __pwrmgr_method_wakelock_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "acquire_timeout", __pwrmgr_method_wakelock_acquire_timeout);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "acquire_timeout", __pwrmgr_method_wakelock_acquire_timeout);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "get_count", __pwrmgr_method_wakelock_get_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "get_count", __pwrmgr_method_wakelock_get_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "save_count", __pwrmgr_method_wakelock_save_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "save_count", __pwrmgr_method_wakelock_save_count);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "wakelock", "stat", __pwrmgr_method_wakelock_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "wakelock", "stat", __pwrmgr_method_wakelock_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "add_notifier", __pwrmgr_method_cpuhp_add_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "add_notifier", __pwrmgr_method_cpuhp_add_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "remove_notifier", __pwrmgr_method_cpuhp_remove_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "remove_notifier", __pwrmgr_method_cpuhp_remove_notifier);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "action", __pwrmgr_method_cpuhp_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "action", __pwrmgr_method_cpuhp_action);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "cpuhp", "revoke_listener", __pwrmgr_method_cpuhp_revoke_listener);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "cpuhp", "revoke_listener", __pwrmgr_method_cpuhp_revoke_listener);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_add", __pwrmgr_method_pmqos_request_add);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_add", __pwrmgr_method_pmqos_request_add);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_remove", __pwrmgr_method_pmqos_request_remove);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_remove", __pwrmgr_method_pmqos_request_remove);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pmqos", "request_update", __pwrmgr_method_pmqos_request_update);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pmqos", "request_update", __pwrmgr_method_pmqos_request_update);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "dh", "register", __pwrmgr_method_dh_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "dh", "register", __pwrmgr_method_dh_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "sec", "mgmt", __pwrmgr_method_sec_mgmt);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "sec", "mgmt", __pwrmgr_method_sec_mgmt);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pac", "cfi", __pwrmgr_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pac", "cfi", __pwrmgr_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "thermmgr", "stat", __pwrmgr_method_thermmgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "thermmgr", "stat", __pwrmgr_method_thermmgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "idlemgr", "stat", __pwrmgr_method_idlemgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "idlemgr", "stat", __pwrmgr_method_idlemgr_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "pmcall", "pm", "init_notify", __pwrmgr_method_pm_init_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "pmhandler", "pm", "init_notify", __pwrmgr_method_pm_init_notify);
	hm_info("%s_%s, max method num: %d\n", "pwrmgr", "method", __pwrmgr_method_MAX);
}

#endif
#endif
#endif
