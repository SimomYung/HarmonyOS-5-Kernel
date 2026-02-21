/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for pwrmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/pwrmgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/pwrmgr/libsysif/pwrmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/pwrmgr/libsysif/pwrmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/pwrmgr/libsysif/pwrmgr/l2h_map.h --legacy-mode
* Create: Tue Sep 16 04:31:10 2025
*/
#ifndef LIBSYSIF_PWRMGR_API_H
#define LIBSYSIF_PWRMGR_API_H
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
#define __pwrmgr_method_thermmgr_stat (__ACTV_HIGHTABLE_START + 35)
#define __pwrmgr_method_thermmgr_rsvd_3 (__ACTV_HIGHTABLE_START + 36)
#define __pwrmgr_method_thermmgr_rsvd_2 (__ACTV_HIGHTABLE_START + 37)
#define __pwrmgr_method_thermmgr_rsvd_1 (__ACTV_HIGHTABLE_START + 38)
#define __pwrmgr_method_idlemgr_stat (__ACTV_HIGHTABLE_START + 39)
#define __pwrmgr_method_pm_init_notify (__ACTV_HIGHTABLE_START + 40)
#define __pwrmgr_method_MAX (__ACTV_HIGHTABLE_START + 41)
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

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_pmcall_pwrmgr_system_state(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_pmcall_pwrmgr_system_state(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_pwrmgr_system_state(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_pwrmgr_system_state(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_pwrmgr_system_state(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_pwrmgr_system_state(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_shutdown_enter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_shutdown_enter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_shutdown_enter(unsigned long long __rpc_info, void *__buf, long timeout_ms)
{
	int __ret;
	long __maybe_unused __actv_local_var_timeout_ms = timeout_ms;
	__ret = ____actvcall_pmcall_shutdown_enter(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(timeout_ms, __actv_local_var_timeout_ms));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_shutdown_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf, long timeout_ms)
{
	int __ret;
	long __maybe_unused __actv_local_var_timeout_ms = timeout_ms;
	__ret = ____actvcapcall_pmcall_shutdown_enter(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(timeout_ms, __actv_local_var_timeout_ms));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_shutdown_softreboot_enter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_shutdown_softreboot_enter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_shutdown_softreboot_enter(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_shutdown_softreboot_enter(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_shutdown_softreboot_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_shutdown_softreboot_enter(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_shutdown_reboot_enter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_shutdown_reboot_enter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_shutdown_reboot_enter(unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd, long timeout_ms)
{
	int __ret;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	long __maybe_unused __actv_local_var_timeout_ms = timeout_ms;
	__ret = ____actvcall_pmcall_shutdown_reboot_enter(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(timeout_ms, __actv_local_var_timeout_ms));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_shutdown_reboot_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd, long timeout_ms)
{
	int __ret;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	long __maybe_unused __actv_local_var_timeout_ms = timeout_ms;
	__ret = ____actvcapcall_pmcall_shutdown_reboot_enter(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(timeout_ms, __actv_local_var_timeout_ms));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_shutdown_kexec_enter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_shutdown_kexec_enter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_shutdown_kexec_enter(unsigned long long __rpc_info, void *__buf, long timeout_ms)
{
	int __ret;
	long __maybe_unused __actv_local_var_timeout_ms = timeout_ms;
	__ret = ____actvcall_pmcall_shutdown_kexec_enter(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(timeout_ms, __actv_local_var_timeout_ms));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_shutdown_kexec_enter(rref_t __rref, unsigned long long __rpc_info, void *__buf, long timeout_ms)
{
	int __ret;
	long __maybe_unused __actv_local_var_timeout_ms = timeout_ms;
	__ret = ____actvcapcall_pmcall_shutdown_kexec_enter(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(timeout_ms, __actv_local_var_timeout_ms));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_suspend_set_state(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_suspend_set_state(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_suspend_set_state(unsigned long long __rpc_info, void *__buf, int state)
{
	int __ret;
	int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcall_pmcall_suspend_set_state(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_suspend_set_state(rref_t __rref, unsigned long long __rpc_info, void *__buf, int state)
{
	int __ret;
	int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcapcall_pmcall_suspend_set_state(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_suspend_disk_restore(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_suspend_disk_restore(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_suspend_disk_restore(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_suspend_disk_restore(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_suspend_disk_restore(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_suspend_disk_restore(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_sysfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_sysfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_sysfs_read(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_buf = actv_buf;
	__ret = ____actvcall_pmcall_sysfs_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(actv_buf, __actv_local_var_actv_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf actv_buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_buf = actv_buf;
	__ret = ____actvcapcall_pmcall_sysfs_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(actv_buf, __actv_local_var_actv_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_pmcall_sysfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_pmcall_sysfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_sysfs_write(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_sysfs_write(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_sysfs_write(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_wakelock_action(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_wakelock_action(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_wakelock_action(unsigned long long __rpc_info, void *__buf, unsigned int action, struct wake_lock_name name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_action = action;
	struct wake_lock_name __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcall_pmcall_wakelock_action(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_wakelock_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int action, struct wake_lock_name name)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_action = action;
	struct wake_lock_name __maybe_unused __actv_local_var_name = name;
	__ret = ____actvcapcall_pmcall_wakelock_action(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_wakelock_acquire_timeout(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_wakelock_acquire_timeout(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_wakelock_acquire_timeout(unsigned long long __rpc_info, void *__buf, struct wake_lock_name wakelock, unsigned long msec)
{
	int __ret;
	struct wake_lock_name __maybe_unused __actv_local_var_wakelock = wakelock;
	unsigned long __maybe_unused __actv_local_var_msec = msec;
	__ret = ____actvcall_pmcall_wakelock_acquire_timeout(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(wakelock, __actv_local_var_wakelock), __ARCH_ACTV_RPC_PASS_ARG(msec, __actv_local_var_msec));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_wakelock_acquire_timeout(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct wake_lock_name wakelock, unsigned long msec)
{
	int __ret;
	struct wake_lock_name __maybe_unused __actv_local_var_wakelock = wakelock;
	unsigned long __maybe_unused __actv_local_var_msec = msec;
	__ret = ____actvcapcall_pmcall_wakelock_acquire_timeout(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(wakelock, __actv_local_var_wakelock), __ARCH_ACTV_RPC_PASS_ARG(msec, __actv_local_var_msec));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_pmcall_wakelock_get_count(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_pmcall_wakelock_get_count(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_wakelock_get_count(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_wakelock_get_count(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_wakelock_get_count(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_wakelock_get_count(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_wakelock_save_count(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_wakelock_save_count(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_wakelock_save_count(unsigned long long __rpc_info, void *__buf, unsigned int count)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_count = count;
	__ret = ____actvcall_pmcall_wakelock_save_count(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_wakelock_save_count(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int count)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_count = count;
	__ret = ____actvcapcall_pmcall_wakelock_save_count(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_wakelock_stat(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_wakelock_stat(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_wakelock_stat(unsigned long long __rpc_info, void *__buf, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	int __ret;
	struct wake_lock_name __maybe_unused __actv_local_var_name = name;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_pmcall_wakelock_stat(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_wakelock_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct wake_lock_name name, struct bunch_ipc_attr attr)
{
	int __ret;
	struct wake_lock_name __maybe_unused __actv_local_var_name = name;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_pmcall_wakelock_stat(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_pmcall_cpuhp_add_notifier(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_pmcall_cpuhp_add_notifier(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_cpuhp_add_notifier(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_cpuhp_add_notifier(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_cpuhp_add_notifier(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_cpuhp_add_notifier(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_pmcall_cpuhp_remove_notifier(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_pmcall_cpuhp_remove_notifier(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_cpuhp_remove_notifier(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_pmcall_cpuhp_remove_notifier(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_cpuhp_remove_notifier(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_pmcall_cpuhp_remove_notifier(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_cpuhp_action(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_cpuhp_action(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_cpuhp_action(unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int event)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpu = cpu;
	unsigned int __maybe_unused __actv_local_var_event = event;
	__ret = ____actvcall_pmcall_cpuhp_action(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpu, __actv_local_var_cpu), __ARCH_ACTV_RPC_PASS_ARG(event, __actv_local_var_event));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_cpuhp_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int event)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpu = cpu;
	unsigned int __maybe_unused __actv_local_var_event = event;
	__ret = ____actvcapcall_pmcall_cpuhp_action(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpu, __actv_local_var_cpu), __ARCH_ACTV_RPC_PASS_ARG(event, __actv_local_var_event));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_pmcall_cpuhp_revoke_listener(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_pmcall_cpuhp_revoke_listener(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_cpuhp_revoke_listener(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcall_pmcall_cpuhp_revoke_listener(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_cpuhp_revoke_listener(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcapcall_pmcall_cpuhp_revoke_listener(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_pmcall_pmqos_request_add(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_pmcall_pmqos_request_add(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_pmqos_request_add(unsigned long long __rpc_info, void *__buf, unsigned int type, int value)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	int __maybe_unused __actv_local_var_value = value;
	__ret = ____actvcall_pmcall_pmqos_request_add(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_pmqos_request_add(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type, int value)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	int __maybe_unused __actv_local_var_value = value;
	__ret = ____actvcapcall_pmcall_pmqos_request_add(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_pmqos_request_remove(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_pmqos_request_remove(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_pmqos_request_remove(unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request)
{
	int __ret;
	struct hm_pmqos_request __maybe_unused __actv_local_var_request = request;
	__ret = ____actvcall_pmcall_pmqos_request_remove(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_pmqos_request_remove(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request)
{
	int __ret;
	struct hm_pmqos_request __maybe_unused __actv_local_var_request = request;
	__ret = ____actvcapcall_pmcall_pmqos_request_remove(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_pmqos_request_update(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_pmqos_request_update(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_pmqos_request_update(unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request, int value)
{
	int __ret;
	struct hm_pmqos_request __maybe_unused __actv_local_var_request = request;
	int __maybe_unused __actv_local_var_value = value;
	__ret = ____actvcall_pmcall_pmqos_request_update(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_pmqos_request_update(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_pmqos_request request, int value)
{
	int __ret;
	struct hm_pmqos_request __maybe_unused __actv_local_var_request = request;
	int __maybe_unused __actv_local_var_value = value;
	__ret = ____actvcapcall_pmcall_pmqos_request_update(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_dh_register(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_dh_register(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_dh_register(unsigned long long __rpc_info, void *__buf, unsigned long long dh_ap_uref, unsigned int type)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_dh_ap_uref = dh_ap_uref;
	unsigned int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcall_pmcall_dh_register(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(dh_ap_uref, __actv_local_var_dh_ap_uref), __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_dh_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long dh_ap_uref, unsigned int type)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_dh_ap_uref = dh_ap_uref;
	unsigned int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcapcall_pmcall_dh_register(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(dh_ap_uref, __actv_local_var_dh_ap_uref), __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_pmcall_sec_mgmt(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_pmcall_sec_mgmt(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcall_pmcall_sec_mgmt(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcapcall_pmcall_sec_mgmt(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_thermmgr_stat(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_thermmgr_stat(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_thermmgr_stat(unsigned long long __rpc_info, void *__buf, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	int __ret;
	struct thermal_zone_type __maybe_unused __actv_local_var_type = type;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_pmcall_thermmgr_stat(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_thermmgr_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct thermal_zone_type type, struct bunch_ipc_attr attr)
{
	int __ret;
	struct thermal_zone_type __maybe_unused __actv_local_var_type = type;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_pmcall_thermmgr_stat(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_idlemgr_stat(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_idlemgr_stat(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_idlemgr_stat(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_pmcall_idlemgr_stat(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_idlemgr_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_pmcall_idlemgr_stat(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_pmcall_pm_init_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_pmcall_pm_init_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_pmcall_pm_init_notify(unsigned long long __rpc_info, void *__buf, enum pm_init_event type)
{
	int __ret;
	enum pm_init_event __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcall_pmcall_pm_init_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_pmcall_pm_init_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum pm_init_event type)
{
	int __ret;
	enum pm_init_event __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcapcall_pmcall_pm_init_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

#endif
#endif
