/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Devhost security module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 23 17:45:16 2020
 */
#include <stdarg.h>
#include <sys/mman.h>
#include <libsysif/devhost/server.h>
#include <devhost/plugin.h>
#include <libstrict/strict.h>
#ifdef CONFIG_AUDIT
#include <libhmaudit/audit.h>
#endif
#include <libsec/service_management.h>
#ifdef CONFIG_SEHARMONY
#include <libhmsrv_sys/hm_security.h>
#endif
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsrv_crypt/entropy_register.h>

#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_hkip.h>
#include <libhmsrv_sys/hm_mem.h>

#include "common.h"
#include "pm.h"
#include "net.h"
#include "net_socket.h"
#include "iaware_notify.h"
#include "kstate.h"
#include "fileguard.h"
#include "rpc.h"
#include "fops.h"
#include "clock.h"
#include "ability.h"
#include "entropy.h"
#include "security.h"
#include "posix_cap.h"
#include "types.h"

#include "devhost.h"
#include <devhost/log.h>
#include <devhost/file.h>
#include <devhost/device.h>
#include <devhost/memory.h>
#ifdef CONFIG_SEHARMONY
#include <devhost/security_seharmony.h>
#include "security_internal.h"
#include <hongmeng/compiler.h>
#endif


#ifdef CONFIG_HKIP
#include <libroafterinit/roafterinit.h>
#endif
#ifdef CONFIG_SEHARMONY
#include <libhmseharmony/flask.h>
#include <libhmseharmony/services.h>
#endif
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/pac.h>
#endif

static int dh_sec_chk_must_from_devmgr(unsigned long long sender)
{
	if (IS_REF_ERR(libdh_get_devmgr_ap()) ||
	    libdh_get_devmgr_cnode_idx() == ~0U ||
	    libdh_get_devmgr_cnode_idx() != __RPC_SENDER_DECODE_SRC_CIDX(sender)) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static int dh_sec_chk_must_from_sysmgr(unsigned long long sender)
{
	if (hmsrv_sysmgr_cnode_idx() != __RPC_SENDER_DECODE_SRC_CIDX(sender)) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static int dh_sec_chk_must_from_fsmgr_devmgr(unsigned long long sender)
{
	/* sec_chk_src_actv_is_vfs failed will print log */
	if (dh_sec_chk_must_from_devmgr(sender) == E_HM_OK ||
			sec_chk_src_actv_is_vfs() == E_HM_OK) {
		return E_HM_OK;
	}

	return E_HM_INVAL;
}

static inline int dh_sec_chk_null(void)
{
	return E_HM_OK;
}

enum checker_arg_type {
	CHK_ARGS_ALL = 1,
	CHK_ARGS_SENDER,
	CHK_ARGS_NONE,
	CHK_NULL
};

union checker_func_ptr {
	int (*sec_chk_all)(unsigned long long sender, unsigned long credential);
	int (*sec_chk_sender)(unsigned long long sender);
	int (*sec_chk_none)(void);
	int (*sec_chk_null)(void);
};

struct one_sec_chk {
	enum checker_arg_type arg_type;
	union checker_func_ptr u;
};

static struct one_sec_chk dh_sec_checkers[__devhost_method_MAX];

#define DH_SEC_CHECKER_VOID(callno, chk_func)	do {		\
	dh_sec_checkers[callno].arg_type = CHK_ARGS_NONE;	\
	dh_sec_checkers[callno].u.sec_chk_none = (chk_func);	\
} while (0)

#define DH_SEC_CHECKER_SENDER(callno, chk_func)	do {		\
	dh_sec_checkers[callno].arg_type = CHK_ARGS_SENDER;	\
	dh_sec_checkers[callno].u.sec_chk_sender = (chk_func);	\
} while (0)

#define DH_SEC_CHECKER_NULL(callno)	do {		\
	dh_sec_checkers[callno].arg_type = CHK_NULL;	\
	dh_sec_checkers[callno].u.sec_chk_null = dh_sec_chk_null;	\
} while (0)

#define DH_SEC_CHECKER_FROM_DEVMGR(callno)	\
	DH_SEC_CHECKER_SENDER(callno, dh_sec_chk_must_from_devmgr)

#define DH_SEC_CHECKER_FROM_SYSMGR(callno)	\
	DH_SEC_CHECKER_SENDER(callno, dh_sec_chk_must_from_sysmgr)

#define DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(callno)	\
	DH_SEC_CHECKER_SENDER(callno, dh_sec_chk_must_from_fsmgr_devmgr)

#ifdef CONFIG_SEHARMONY
/**
 * seharmony checker
*/
#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*((!!(condition)) ? 1 : 0)]))
#endif
#define __check_func_type(func_ptr, type) BUILD_BUG_ON(!__same_type(typeof(func_ptr), type))

typedef int (*sec_chk_devhost_filp)(struct filp_node *filp, ...);
typedef int (*seharmony_checker_func)(sec_chk_devhost_filp func, va_list args);

/**
 * @filp_node: must check the filp_node not NULL before
 * passing it into devhost_seharmony_check as argument, if
 * using the DH_SEHARMONY_CHECKER_{FILP, IOCTL, FCNTL} to
 * define the checker.
 */
static inline struct filp_node *__dh_parse_filp_vargs(va_list *args)
{
	unsigned long long file = va_arg(*args, unsigned long long);
	return u64_to_ptr(file, struct filp_node);
}

/**
 * @param func int (*)(struct filp_node *filp)
 */
static int seharmony_checker_filp(sec_chk_devhost_filp func, va_list args)
{
	struct filp_node *node = __dh_parse_filp_vargs(&args);
	return func(node);
}

/**
 * @param func int (*)(struct filp_node *filp, unsigned int cmd, unsigned long arg)
 */
static int seharmony_checker_ioctl(sec_chk_devhost_filp func, va_list args)
{
	unsigned int ioctl_cmd;
	unsigned long arg;
	struct filp_node *node = __dh_parse_filp_vargs(&args);
	ioctl_cmd = va_arg(args, unsigned int);
	arg = va_arg(args, unsigned long);
	return func(node, ioctl_cmd, arg);
}

/**
 * @param func int (*)(struct filp_node *filp, int cmd, unsigned long arg)
 */
static int seharmony_checker_fcntl(sec_chk_devhost_filp func, va_list args)
{
	int fcntl_cmd;
	unsigned long arg;
	struct filp_node *node = __dh_parse_filp_vargs(&args);
	fcntl_cmd = va_arg(args, int);
	arg = va_arg(args, unsigned long);
	return func(node, fcntl_cmd, arg);
}

/**
 * struct seharmony_check_element - element for dh_seharmony_checkers array
 * @sec_chk_func:	actual seharmony checker function generated by
 * 			security frameworks
 * @seharmony_checker:	template checker function defined according to
 * 			parameters' number and types
 */
struct seharmony_check_element {
	sec_chk_devhost_filp sec_chk_func;
	seharmony_checker_func seharmony_checker;
};

static struct seharmony_check_element dh_seharmony_checkers[__devhost_method_MAX];

#ifdef PAC_FWCFI_SYSSERVICE
#define DH_SEHARMONY_CHECKER_FILP(callno, chk_func)	do {				\
	__check_func_type(&chk_func, int (*)(struct filp_node *filp));			\
	__asm__ __volatile__("\nmov %0, %1": "=r"(dh_seharmony_checkers[callno].sec_chk_func): "r"(chk_func)); \
	PAC_FECFI_SIGN(dh_seharmony_checkers[callno].sec_chk_func, sec_chk_devhost_filp);	\
	dh_seharmony_checkers[callno].seharmony_checker = seharmony_checker_filp;		\
} while (0)

#define DH_SEHARMONY_CHECKER_IOCTL(callno, chk_func)	do {				\
	__check_func_type(&chk_func, int (*)(struct filp_node *filp,			\
					     unsigned int cmd,				\
					     unsigned long arg));			\
	__asm__ __volatile__("\nmov %0, %1": "=r"(dh_seharmony_checkers[callno].sec_chk_func): "r"(chk_func)); \
	PAC_FECFI_SIGN(dh_seharmony_checkers[callno].sec_chk_func, sec_chk_devhost_filp);	\
	dh_seharmony_checkers[callno].seharmony_checker = seharmony_checker_ioctl;		\
} while (0)

#define DH_SEHARMONY_CHECKER_FCNTL(callno, chk_func)	do {				\
	__check_func_type(&chk_func, int (*)(struct filp_node *filp,			\
					     int cmd, unsigned long arg));		\
	__asm__ __volatile__("\nmov %0, %1": "=r"(dh_seharmony_checkers[callno].sec_chk_func): "r"(chk_func)); \
	PAC_FECFI_SIGN(dh_seharmony_checkers[callno].sec_chk_func, sec_chk_devhost_filp);	\
	dh_seharmony_checkers[callno].seharmony_checker = seharmony_checker_fcntl;		\
} while (0)
#else
#define DH_SEHARMONY_CHECKER_FILP(callno, chk_func)	do {				\
	__check_func_type(&chk_func, int (*)(struct filp_node *filp));			\
	dh_seharmony_checkers[callno].sec_chk_func = (sec_chk_devhost_filp)(chk_func);	\
	dh_seharmony_checkers[callno].seharmony_checker = seharmony_checker_filp;		\
} while (0)

#define DH_SEHARMONY_CHECKER_IOCTL(callno, chk_func)	do {				\
	__check_func_type(&chk_func, int (*)(struct filp_node *filp,			\
					     unsigned int cmd,				\
					     unsigned long arg));			\
	dh_seharmony_checkers[callno].sec_chk_func = (sec_chk_devhost_filp)(chk_func);	\
	dh_seharmony_checkers[callno].seharmony_checker = seharmony_checker_ioctl;		\
} while (0)

#define DH_SEHARMONY_CHECKER_FCNTL(callno, chk_func)	do {				\
	__check_func_type(&chk_func, int (*)(struct filp_node *filp,			\
					     int cmd, unsigned long arg));		\
	dh_seharmony_checkers[callno].sec_chk_func = (sec_chk_devhost_filp)(chk_func);	\
	dh_seharmony_checkers[callno].seharmony_checker = seharmony_checker_fcntl;		\
} while (0)
#endif
#endif

static void __dh_security_chk_transfs(void)
{
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_mounted, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_unmounted, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_mkdir, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_rmdir, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_create, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_unlink, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_write, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_read, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_write_iter, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_fsync, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_mknod, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_link, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_symlink, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_statfs, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_rename, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_readdir, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_lookup, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_setattr, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_readlink, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_open, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_fallocate, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_getattr, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_listxattr, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_getxattr, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_setxattr, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_tmpfile, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_sync, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_drop_caches, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_close, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_ioctl, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_direct_IO, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_kill_dentry, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_node_put, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_update_opts, sec_chk_fops_transfs);
	DH_SEC_CHECKER_VOID(__devhost_method_transfs_iommap, sec_chk_fops_transfs);
}

#ifdef CONFIG_SEHARMONY
static void __dh_security_seharmony_chk_init(void)
{
	DH_SEHARMONY_CHECKER_FILP(__devhost_method_fops_open, sec_chk_devhost_file_alloc);
	DH_SEHARMONY_CHECKER_FILP(__devhost_method_devfs_read, sec_chk_devhost_fread);
	DH_SEHARMONY_CHECKER_FILP(__devhost_method_devfs_write, sec_chk_devhost_fwrite);
	DH_SEHARMONY_CHECKER_IOCTL(__devhost_method_devfs_ioctl, sec_chk_devhost_ioctl);
	DH_SEHARMONY_CHECKER_FILP(__devhost_method_devfs_readv, sec_chk_devhost_fread);
	DH_SEHARMONY_CHECKER_FILP(__devhost_method_devfs_writev, sec_chk_devhost_fwrite);
	DH_SEHARMONY_CHECKER_FCNTL(__devhost_method_devfs_fcntl, sec_chk_devhost_fcntl);
}
#endif

static void __dh_security_chk_init(void)
{
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_procfs_read);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_procfs_write);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_procfs_open);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_procfs_close);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_sysfs_open);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_sysfs_close);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_sysfs_read);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_sysfs_write);
	DH_SEC_CHECKER_FROM_FSMGR_DEVMGR(__devhost_method_sysfs_poll);
	DH_SEC_CHECKER_VOID(__devhost_method_fops_sysfs_read, sec_chk_fops_sysfs);
	DH_SEC_CHECKER_VOID(__devhost_method_fops_sysfs_write, sec_chk_fops_sysfs);
	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_rpc_export);

	DH_SEC_CHECKER_VOID(__devhost_method_rpc_invoke, sec_chk_rpc_invoke);
	DH_SEC_CHECKER_VOID(__devhost_method_net_netlink_recv, sec_chk_net_send);
	DH_SEC_CHECKER_VOID(__devhost_method_clock_settime, sec_chk_clock_settime);
	DH_SEC_CHECKER_VOID(__devhost_method_clock_gettime, sec_chk_clock_gettime);
	DH_SEC_CHECKER_VOID(__devhost_method_clock_getres, sec_chk_clock_getres);
	DH_SEC_CHECKER_VOID(__devhost_method_clock_adjtime, sec_chk_clock_adjtime);
	DH_SEC_CHECKER_VOID(__devhost_method_clock_poweroff_alarm, sec_chk_clock_poweroff_alarm);
	DH_SEC_CHECKER_VOID(__devhost_method_net_prepare, sec_chk_net_prepare);
	DH_SEC_CHECKER_VOID(__devhost_method_net_init, sec_chk_net_init);
	DH_SEC_CHECKER_VOID(__devhost_method_net_send, sec_chk_net_send);
	DH_SEC_CHECKER_VOID(__devhost_method_net_ioctl, sec_chk_net_ioctl);
	DH_SEC_CHECKER_VOID(__devhost_method_net_status, sec_chk_net_stat);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_register, sec_chk_net_socket_register);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_close, sec_chk_net_socket_close);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_connect, sec_chk_net_socket_connect);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_bind, sec_chk_net_socket_bind);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_listen, sec_chk_net_socket_listen);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_accept, sec_chk_net_socket_accept);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_shutdown, sec_chk_net_socket_shutdown);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_ioctl, sec_chk_net_socket_ioctl);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_dev_ioctl, sec_chk_net_socket_dev_ioctl);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_fcntl, sec_chk_net_socket_fcntl);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_send, sec_chk_net_socket_send);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_recv, sec_chk_net_socket_recv);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_sendmmsg, sec_chk_net_socket_sendmmsg);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_recvmmsg, sec_chk_net_socket_recvmmsg);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_sendmsg, sec_chk_net_socket_sendmsg);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_recvmsg, sec_chk_net_socket_recvmsg);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_sendto, sec_chk_net_socket_sendto);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_recvfrom, sec_chk_net_socket_recvfrom);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_readv, sec_chk_net_socket_readv);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_writev, sec_chk_net_socket_writev);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_getsockopt, sec_chk_net_socket_getsockopt);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_setsockopt, sec_chk_net_socket_setsockopt);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_getsockname, sec_chk_net_socket_getsockname);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_getpeername, sec_chk_net_socket_getpeername);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_fchown, sec_chk_net_socket_fchown);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_fstat, sec_chk_net_socket_fstat);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_socket, sec_chk_net_socket_socket);
	DH_SEC_CHECKER_VOID(__devhost_method_iaware_notify_thread_msg, sec_chk_iaware_notify_thread_msg);
	DH_SEC_CHECKER_VOID(__devhost_method_kstate_binderinfo_notify, sec_chk_kstate_binderinfo_notify);
	DH_SEC_CHECKER_VOID(__devhost_method_kstate_killinfo_notify, sec_chk_kstate_killinfo_notify);
	DH_SEC_CHECKER_VOID(__devhost_method_kstate_freeze_binder_info_notify, sec_chk_kstate_freeze_binder_info_notify);
	DH_SEC_CHECKER_VOID(__devhost_method_kstate_request_anco_thaw, sec_chk_kstate_request_anco_thaw);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_alloc_new_ns, sec_chk_net_socket_alloc_new_ns);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_destroy_ns, sec_chk_net_socket_destroy_ns);
	DH_SEC_CHECKER_VOID(__devhost_method_net_socket_bpf, sec_chk_net_socket_bpf);
	DH_SEC_CHECKER_VOID(__devhost_method_fileguard_message_notify, sec_chk_fileguard_message_notify);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_action, sec_chk_pm_action);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_dubai_hwlog, sec_chk_pm_action);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_prepare, sec_chk_pm_action);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_wl_stat, sec_chk_pm_action);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_cpuhp_action, sec_chk_cpuhp_action);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_power_off, sec_chk_power_off);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_reboot, sec_chk_power_off);
	DH_SEC_CHECKER_VOID(__devhost_method_pm_thermal_action, sec_chk_pm_action);

	DH_SEC_CHECKER_VOID(__devhost_method_core_entropy_read, sec_chk_entropy_read);

	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_core_load_driver);
	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_core_unload_driver);
	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_core_create_device);
	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_fops_open);
	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_core_bind_device);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_shrinker_query);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_shrinker_shrink);

	DH_SEC_CHECKER_VOID(__devhost_method_fops_close, sec_chk_fops_close);

	DH_SEC_CHECKER_NULL(__devhost_method_fops_read);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_write);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_read_iter);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_write_iter);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_llseek);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_ioctl);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_fcntl);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_flush);
	DH_SEC_CHECKER_VOID(__devhost_method_fops_mmap, sec_chk_fops_mmap);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_munmap);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_fops_do_page_fault);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_fops_mm_release);

	DH_SEC_CHECKER_NULL(__devhost_method_fops_poll);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_poll_remove);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_epoll_ctl);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_epoll_remove);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_epoll_recheck);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_sync_ctrlmem_polllist);
	DH_SEC_CHECKER_NULL(__devhost_method_fops_file_get);
	DH_SEC_CHECKER_NULL(__devhost_method_sec_mgmt);
	DH_SEC_CHECKER_VOID(__devhost_method_sec_sync_fastpath_open, sec_chk_fops_open_sync_sec);
	DH_SEC_CHECKER_FROM_DEVMGR(__devhost_method_sec_sync_fastpath_dev);

	DH_SEC_CHECKER_NULL(__devhost_method_sec_test_hkip_seharmony_prot_policydb);
	DH_SEC_CHECKER_NULL(__devhost_method_sec_test_hkip_roafter_prot);

	DH_SEC_CHECKER_NULL(__devhost_method_devfs_read);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_write);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_ioctl);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_readv);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_writev);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_lseek);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs__llseek);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_fcntl);
	DH_SEC_CHECKER_NULL(__devhost_method_devfs_fcntl64);

	DH_SEC_CHECKER_NULL(__devhost_method_posix_vhangup);

	/* check transfs ops */
	__dh_security_chk_transfs();

	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_process_revoke);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_trace_enable_disable_ldk_event);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_trace_ldk_event_format);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_trace_print_ldk_event);

	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_file_info_query_dmabuf);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_file_info_query_fence);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_file_info_query_ion);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_file_info_query_meminfo);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_file_info_lowmem_debug);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_file_info_query_gpu);

	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_probe_handle_brk);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_probe_handle_ss_brk);

	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_tests_hkip_protection);
	DH_SEC_CHECKER_FROM_SYSMGR(__devhost_method_test_pac_fwcfi_fault_inject);

#ifdef CONFIG_SEHARMONY
	/* init seharmony checker */
	__dh_security_seharmony_chk_init();
#endif
}

static int __dh_security_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	int ret = E_HM_OK;

	UNUSED(plugin);

	/* init security policy */
	ret = devhost_security_init();
	if (ret != E_HM_OK) {
		dh_error("secuirty initialization failed, err=%s\n",
			 hmstrerror(ret));
		return ret;
	}
	__dh_security_chk_init();

#ifdef CONFIG_AUDIT
	ret = audit_module_init();
	if (ret != E_HM_OK) {
		dh_error("audit module init failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
#endif
#ifdef CONFIG_SEHARMONY
	ret = hm_sec_shconf_init();
	if (ret != E_HM_OK) {
		dh_error("sec shconf init failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
#endif

	return E_HM_OK;
}

static int __do_dh_sec_checker(const struct one_sec_chk *checker,
			       unsigned long long sender, unsigned long credential)
{
	switch (checker->arg_type) {
	case CHK_ARGS_ALL:
		return checker->u.sec_chk_all(sender, credential);
	case CHK_ARGS_SENDER:
		return checker->u.sec_chk_sender(sender);
	case CHK_ARGS_NONE:
		return checker->u.sec_chk_none();
	case CHK_NULL:
		return checker->u.sec_chk_null();
	default:
		return E_HM_INVAL;
	}
}

int devhost_sec_chk_dispatch(int callno, unsigned long long sender, unsigned long credential)
{
	if ((callno <= __devhost_method_MIN) ||
	    (callno >= __devhost_method_MAX)) {
		return E_HM_INVAL;
	}

	if (dh_sec_checkers[callno].u.sec_chk_all == NULL) {
		return E_HM_NOSYS;
	}

	return __do_dh_sec_checker(&dh_sec_checkers[callno], sender, credential);
}

#ifdef CONFIG_SEHARMONY
int devhost_seharmony_common_checker(int callno, ...)
{
	int ret = E_HM_OK;
	va_list args;
	struct seharmony_check_element *checker;

	if ((callno <= __devhost_method_MIN) ||
	    (callno >= __devhost_method_MAX)) {
		return E_HM_INVAL;
	}

	checker = &dh_seharmony_checkers[callno];
	if (checker->sec_chk_func != NULL && checker->seharmony_checker != NULL) {
		va_start(args, callno);
		ret = checker->seharmony_checker(checker->sec_chk_func, args);
		va_end(args);
	}

	return ret;
}
#endif

static int dh_sec_register_entropy_source(struct advice_ctx *ctx)
{
#ifdef CONFIG_DEVHOST_ENTROPY_COLLECT
	int ret;

	ret = hm_register_entropy_source(libdh_get_dh_ap());
	if (ret != E_HM_OK) {
		dh_error("register entorpy source failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
#endif
	return ctx->call_next(ctx);
}
DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, dh_sec_register_entropy_source, NULL);

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sec_mgmt, dh_sec_mgmt_ext,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, module, unsigned int, command)
{
#ifdef CONFIG_SEC_SERVICE
	int ret = E_HM_ACCES;
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	if (hmsrv_sysmgr_cnode_idx() == sysif_actv_src_cnode_idx()) {
#ifdef CONFIG_SEC_SHARE_POLICY
		ret = hm_secs_mgmt(module, command, &recv, SEC_SERVICE_ALL);
#else
#ifdef CONFIG_NET_FUSION
		ret = hm_secs_mgmt(module, command, &recv, SEC_SERVICE_DEVHOST | SEC_SERVICE_NETWORK);
#else
		ret = hm_secs_mgmt(module, command, &recv, SEC_SERVICE_DEVHOST);
#endif
#endif
	}
	return ret;
#else
	UNUSED(sender, credential, module, command);
	return E_HM_NOSYS;
#endif
}

#ifdef CONFIG_SEC_SERVICE
int devhost_reg_sec_mgmt(void)
{
	int err;

	err = hm_sec_register_service("devhost", libdh_get_dh_ap(), __devhost_method_sec_mgmt, NULL);
	if (err < 0) {
		hm_error("devhost setup actvpool failed, err=%s\n", hmstrerror(err));
	}

	return err;
}
#endif

#ifdef CONFIG_HKIP_SEHARMONY_PROT_TEST_CASES
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sec_test_hkip_seharmony_prot_policydb,
		    dh_sec_test_hkip_seharmony_prot_policydb_ext,
		    unsigned long long, sender, unsigned long, credential)
{
	UNUSED(sender, credential);
	return hmsel_policydb_test();
}
#endif

#ifdef CONFIG_HKIP_TEST_CASES
static int handler_test_hkip_protect_ro(void)
{
	int ret = E_HM_OK;
	void *vaddr = NULL;
	vaddr = hm_mem_mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE | MPROT_HKIP,
			    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (vaddr == MAP_FAILED) {
		hm_error("mmap failed\n");
		return E_HM_FAULT;
	}
	ret = actvcall_hmcall_test_hkip_write_evil_data(vaddr, 1);
	if (ret != E_HM_OK || *(char *)vaddr != 1) {
		hm_error("hkip: failed to write evil data, value = %d\n", *(int *)vaddr);
		(void)hm_mem_munmap(vaddr, PAGE_SIZE);
		return E_HM_FAULT;
	}
	ret = hm_mem_mprotect(vaddr, PAGE_SIZE, PROT_READ);
	if (ret != E_HM_OK) {
		hm_error("hkip: failed to invoke hkip protection, err = %s\n", hmstrerror(ret));
		return E_HM_FAULT;
	}
	ret = actvcall_hmcall_test_hkip_write_evil_data(vaddr, 0);
	if (ret != E_HM_OK || *(char *)vaddr == 0) {
		hm_error("hkip: failed to protect ro test_hkip_write_data, value = %d\n", *(int *)vaddr);
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

static int handler_test_hkip_protect_rox(void)
{
	int ret = E_HM_OK;
	void *vaddr = NULL;
	vaddr = hm_mem_mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE | MPROT_HKIP,
			    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (vaddr == MAP_FAILED) {
		hm_error("mmap failed\n");
		return E_HM_FAULT;
	}
	ret = actvcall_hmcall_test_hkip_write_evil_data(vaddr, 1);
	if (ret != E_HM_OK || *(char *)vaddr != 1) {
		hm_error("hkip: failed to write evil data, value = %d\n", *(int *)vaddr);
		(void)hm_mem_munmap(vaddr, PAGE_SIZE);
		return E_HM_FAULT;
	}
	ret = hm_mem_mprotect(vaddr, PAGE_SIZE, PROT_READ | PROT_EXEC);
	if (ret != E_HM_OK) {
		hm_error("hkip: failed to invoke hkip protection, err = %s\n", hmstrerror(ret));
		return E_HM_FAULT;
	}
	ret = actvcall_hmcall_test_hkip_write_evil_data(vaddr, 0);
	if (ret != E_HM_OK || *(char *)vaddr == 0) {
		hm_error("hkip: failed to protect rox test_hkip_write_data, value = %d\n", *(int *)vaddr);
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

static int handler_test_hkip_protect_rowm(void)
{
	int ret = E_HM_OK;
	void *vaddr = NULL;
	vaddr = hm_mem_mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE | MPROT_HKIP | MPROT_HKIP_ROWM,
			    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (vaddr == MAP_FAILED) {
		hm_error("mmap failed\n");
		return E_HM_FAULT;
	}
	ret = actvcall_hmcall_test_hkip_write_evil_data(vaddr, 1);
	if (ret != E_HM_OK || *(char *)vaddr != 1) {
		hm_error("hkip: failed to write evil data, value = %d\n", *(int *)vaddr);
		(void)hm_mem_munmap(vaddr, PAGE_SIZE);
		return E_HM_FAULT;
	}
	ret = hm_mem_mprotect(vaddr, PAGE_SIZE, PROT_READ);
	if (ret != E_HM_OK) {
		hm_error("hkip: failed to invoke hkip protection, err = %s\n", hmstrerror(ret));
		return E_HM_FAULT;
	}
	ret = actvcall_hmcall_test_hkip_write_evil_data(vaddr, 0);
	if (ret != E_HM_OK || *(char *)vaddr == 0) {
		hm_error("hkip: failed to protect ro test_hkip_write_data, value = %d\n", *(int *)vaddr);
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

static int handler_test_hkip_rowm_cpy(void)
{
	int ret = E_HM_OK;
	void *vaddr = NULL;
	char *values = "hkip_test_rowm_cpy";

	vaddr = hm_mem_mmap(NULL, PAGE_SIZE, PROT_READ | MPROT_HKIP | MPROT_HKIP_ROWM,
			    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (vaddr == MAP_FAILED) {
		hm_error("mmap failed\n");
		return E_HM_FAULT;
	}
	hm_error("hkip: hkip_test_rowm_cpy, value = %sn", (char *)vaddr);
	ret = hm_hkip_rowm_cpy(vaddr, values, strlen(values)+1);
	if (strcmp(vaddr, values) != 0) {
		hm_error("hkip: failed to protect rowm cpy, value = %s, err = %d\n", (char *)vaddr, ret);
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

static int handler_test_hkip_rowm_set(void)
{
	int ret = E_HM_OK;
	void *vaddr = NULL;
	vaddr = hm_mem_mmap(NULL, PAGE_SIZE, PROT_READ | MPROT_HKIP | MPROT_HKIP_ROWM,
			    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (vaddr == MAP_FAILED) {
		hm_error("mmap failed\n");
		return E_HM_FAULT;
	}
	ret = hm_hkip_rowm_set(vaddr, 1, PAGE_SIZE);
	if ((*(int *)vaddr) != 0x01010101) {
		hm_error("hkip: failed to protect rowm set, value = %d, err = %s\n", (*(int *)vaddr), hmstrerror(ret));
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

#ifdef CONFIG_HKIP
static const char test_const_ro = 1;
static int handler_test_hkip_const_ro(void)
{
	int ret;
	char *test_ro = (char *)(uintptr_t)(&test_const_ro);
	ret = actvcall_hmcall_test_hkip_write_evil_data(test_ro, 0);
	if (ret != E_HM_OK || *test_ro == 0) {
		hm_error("hkip: failed to protect const ro, value = %u\n", (unsigned int)(*test_ro));
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

static int handler_test_hkip_code(void)
{
	int ret;
	char origin_val;
	char *addr;
	origin_val = *(char *)(uintptr_t)(&handler_test_hkip_const_ro);
	addr = (char *)(uintptr_t)(&handler_test_hkip_const_ro);

	ret = actvcall_hmcall_test_hkip_write_evil_data(addr, origin_val + 1);
	if (ret != E_HM_OK || *addr != origin_val) {
		hm_error("hkip: failed to protect code, value = %u\n", (unsigned int)(*addr));
		return E_HM_FAULT;
	}
	return E_HM_OK;
}
#endif

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_tests_hkip_protection, dh_tests_hkip_protection_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long, test_id)
{
	UNUSED(sender, credential);
	int rc = E_HM_OK;
	switch(test_id) {
	case __TEST_HKIP_RO:
		rc = handler_test_hkip_protect_ro();
		break;
	case __TEST_HKIP_ROX:
		rc = handler_test_hkip_protect_rox();
		break;
	case __TEST_HKIP_ROWM:
		rc = handler_test_hkip_protect_rowm();
		break;
	case __TEST_HKIP_ROWM_CPY:
		rc = handler_test_hkip_rowm_cpy();
		break;
	case __TEST_HKIP_ROWM_SET:
		rc = handler_test_hkip_rowm_set();
		break;
#ifdef CONFIG_HKIP
	case __TEST_HKIP_CODE:
		rc = handler_test_hkip_code();
		break;
	case __TEST_HKIP_CONST_RO:
		rc = handler_test_hkip_const_ro();
		break;
#endif
	default:
		rc = E_HM_INVAL;
		break;
	}
	return rc;
}
#endif

#if defined(PAC_FWCFI_SYSSERVICE) && defined(CONFIG_DEVHOST_FAULT_INJECT)
typedef int (*fwcfi_fn)(void);
static __noinline int fwcfi_attack_payload(void)
{
	hm_error("unreachable: fwcfi\n");
	return E_HM_POSIX_FAULT;
}
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_test_pac_fwcfi_fault_inject, dh_test_pac_fwcfi_fault_inject_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	UNUSED(sender, credential);
	fwcfi_fn fn;
	__asm__ __volatile__("\nmov %0, %1": "=r"(fn): "r"(fwcfi_attack_payload));
	return fn();
}
#else
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_test_pac_fwcfi_fault_inject, dh_test_pac_fwcfi_fault_inject_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	UNUSED(sender, credential);
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_HKIP_TEST_CASES
#ifndef OFF_PADDING
#define OFF_PADDING 8
#endif
int roafter_test_result;

enum hkip_seg_ind {
	ZERO = 0,
	ONE,
	TWO
};

static int test_write(uint32_t *target)
{
	volatile uint8_t *addr = (uint8_t*)target;
	int ret;
	uint8_t old, cmp_val;

	old = *addr;
	cmp_val = old;
	cmp_val++;

	hm_info("[! %s] mpva %p before = %x\n", __FUNCTION__, addr, old);
	ret = actvcall_hmcall_test_hkip_write_evil_data(target, cmp_val);
	if(ret != E_HM_OK) {
		hm_info("[! %s] try write %p with %x failed\n", __func__, addr, cmp_val);
		return E_HM_INVAL;
	}
	hm_info("[! %s] mpva %p now = %x (should != %x)\n", __FUNCTION__, addr, *addr, cmp_val);

	if(*addr == cmp_val) {
		ret = actvcall_hmcall_test_hkip_write_evil_data(target, old);
		if(ret != E_HM_OK) {
			hm_info("[! %s] try write %p with %x failed\n", __func__, addr, old);
		}
		return E_HM_INVAL;
	}
	return E_HM_OK;
}

static int lookup_ro_after_init_range(struct dl_phdr_info *info, uint32_t **start, uint32_t **end)
{
	uint64_t *seg_hkip_info = 0;
	for(size_t i = 0; i < info->dlpi_phnum; i++) {
		if(info->dlpi_phdr[i].p_type == PT_HKIP) {
			seg_hkip_info = (uint64_t*)(info->dlpi_phdr[i].p_vaddr + info->dlpi_addr);
			break;
		}
	}
	if(!seg_hkip_info || seg_hkip_info[ZERO] == 0) {
		return E_HM_INVAL;
	}
	if(!seg_hkip_info[ONE] || !seg_hkip_info[TWO]) {
		hm_error("[! %s] library %s do not got roafter segment\n", __FILE__, info->dlpi_name);
		return E_HM_INVAL;
	}
	if(seg_hkip_info[ONE] >= seg_hkip_info[TWO]) {
		hm_error("[! %s] library %s roafter data range error\n", __func__, info->dlpi_name);
		hm_debug("start %lx, end %lx\n", seg_hkip_info[ONE] + info->dlpi_addr, seg_hkip_info[TWO] + info->dlpi_addr);
		return E_HM_INVAL;
	}
	*start = (uint32_t*)(seg_hkip_info[ONE] + info->dlpi_addr);
	*end = (uint32_t*)(seg_hkip_info[TWO] + info->dlpi_addr);
	return E_HM_OK;
}

static int roafterinit_test(struct dl_phdr_info *info, size_t size, void *data)
{
	uint32_t *start_addr = 0;
	uint32_t *end_addr = 0;

	if(lookup_ro_after_init_range(info, &start_addr, &end_addr) == E_HM_OK) {
		hm_info("[! %s] testing library %s\n", __func__, info->dlpi_name);
		roafter_test_result |= test_write(start_addr);
		roafter_test_result |= test_write((uint32_t*)((char*)end_addr - OFF_PADDING));
	}
	UNUSED(size, data);
	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sec_test_hkip_roafter_prot, dh_sec_test_hkip_roafter_prot_ext,
		    unsigned long long, sender, unsigned long, credential)
{
	roafter_test_result = E_HM_OK;
	/* test roafter protection */
	dl_iterate_phdr(roafterinit_test, NULL);

	UNUSED(sender, credential);
	return roafter_test_result;
}
#endif

#ifdef CONFIG_SEHARMONY
static inline int sel_context_to_sid(char *sval, size_t slen, hmsel_sid_t *sid)
{
	struct hmsel_str_con str_con = {
		.con = sval,
		.len = slen
	};
	return hmsel_context_to_sid(str_con, sid, 0);
}

static int devhost_is_root_uts_ns(bool *is_root_uts_ns)
{
	struct __actvret_hmcall_ns_is_current_in_root_uts_ns pret;
	int ret = E_HM_OK;

	mem_zero_s(pret);
	ret = actvxactcall_hmcall_ns_is_current_in_root_uts_ns(true, true, &pret);
	if (ret != E_HM_OK) {
		hm_error("query current thread uts ns failed\n");
		return ret;
	}

	*is_root_uts_ns = pret.is_root_uts_ns;
	return E_HM_OK;
}

static int devhost_seharmony_set_devinfo_sec(struct filp_node *node, struct device_info *devinfo,
						char *sval, size_t slen, unsigned int task_sid, unsigned int state)
{
	struct device_info_sec *devinfo_cred = NULL;
	hmsel_sid_t sid;
	int ret;
	bool is_root_uts_ns;

	if (devinfo == NULL || sval == NULL || state > (unsigned int)DEVICE_INFO_SEL_INITIALIZED) {
		return E_HM_INVAL;
	}

	ret = devhost_is_root_uts_ns(&is_root_uts_ns);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (is_root_uts_ns) {
		devinfo_cred = &devinfo->p->devinfo_cred;
	} else {
		devinfo_cred = &devinfo->p->devinfo_cred_aoco;
	}

	ret = sel_context_to_sid(sval, slen, &sid);
	if (ret != E_HM_OK) {
		return ret;
	}

	devinfo_cred_set_sid(devinfo_cred, sid);
	devinfo_cred->sel.task_sid = (hmsel_sid_t)task_sid;
	devinfo_cred->sel.sclass = HM_SECCLASS_CHR_FILE;
	devinfo_cred->sel.initialized = (enum devinfo_sel_state)state;
	if (node != NULL) {
		node->security.device_info_sec = devinfo_cred;
	}
	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sec_sync_fastpath_open, dh_sec_sync_fastpath_open_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long long, filp, unsigned int, task_sid, unsigned int, state)
{
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	struct device_info *devinfo = NULL;
	struct filp_node *node = NULL;
	int ret = E_HM_OK;

	node = u64_to_ptr(filp, struct filp_node);
	if (node == NULL) {
		hm_error("devhost: seharmony: sec_sync_fastpath_open node invalid.\n");
		return -E_HM_INVAL;
	}
	devinfo = node->devinfo;

	ret = devhost_seharmony_set_devinfo_sec(node, devinfo, recv.data, recv.size, task_sid, state);
	if (ret != E_HM_OK) {
		hm_error("devhost: seharmony: sec_sync_fastpath_open set devinfo failed.\n");
		return ret;
	}

	return sec_chk_devhost_file_opened(node);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sec_sync_fastpath_dev, dh_sec_sync_fastpath_dev_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, devid, unsigned int, task_sid, unsigned int, state)
{
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	ret = devhost_acquire_device(devid, &devinfo);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	ret = devhost_seharmony_set_devinfo_sec(NULL, devinfo, recv.data, recv.size, task_sid, state);
	if (ret != E_HM_OK) {
		hm_error("devhost: seharmony: sec_sync_fastpath_dev set devinfo failed.\n");
	}

	devinfo_put(devinfo);
	return ret;
}
#endif

/*
 * define security builtin plugin, this plugin includes the `devhost_sec_chk_dispatch`.
 * responsible for security check of sysif interface
 *
 * extension point: NULL
 *
 * extension:
 *  - target advice-ep: `devhost_backend_init_done`
 *
 * export symbol: NULL
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(security, __dh_security_plugin_init,
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext),
#ifdef CONFIG_HKIP_SEHARMONY_PROT_TEST_CASES
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sec_test_hkip_seharmony_prot_policydb,
		dh_sec_test_hkip_seharmony_prot_policydb_ext),
#endif

#ifdef CONFIG_HKIP_TEST_CASES
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_tests_hkip_protection, dh_tests_hkip_protection_ext),
#endif

	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_test_pac_fwcfi_fault_inject, dh_test_pac_fwcfi_fault_inject_ext),

#ifdef CONFIG_HKIP_TEST_CASES
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sec_test_hkip_roafter_prot, dh_sec_test_hkip_roafter_prot_ext),
#endif
#ifdef CONFIG_SEHARMONY
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sec_sync_fastpath_open, dh_sec_sync_fastpath_open_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sec_sync_fastpath_dev, dh_sec_sync_fastpath_dev_ext),
#endif
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sec_mgmt, dh_sec_mgmt_ext)
);
