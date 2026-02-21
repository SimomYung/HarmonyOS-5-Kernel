/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Native interface of key management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 02 15:08:10 2021
 */
#ifndef LIBHMSRV_KEY_H
#define LIBHMSRV_KEY_H

#include <stddef.h>
#include <stdint.h>
#include <libhmucap/ucap.h>
#ifdef CONFIG_VFS_KEYMGR_DIRECT
#include <libkeymgr/fscrypt_api.h>
#include <libkeymgr/fsverity_api.h>
#endif

/*
 * special keyring's shortcut ID
 * KEY_PROC keyrings can be found by process's relative data structure
 * KEY_PROC keyrings can be found by user namespace's relative data structure
 */
#define KEY_PROC_THREAD_KEYRING		(-1) /* - key ID for thread-specific keyring */
#define KEY_PROC_PROCESS_KEYRING	(-2) /* - key ID for process-specific keyring */
#define KEY_PROC_SESSION_KEYRING	(-3) /* - key ID for session-specific keyring */
#define KEY_USER_KEYRING		(-4) /* - key ID for UID-specific keyring */
#define KEY_USER_SESSION_KEYRING	(-5) /* - key ID for UID-session keyring */
#define KEY_UNKNOWN_KEYRING		(-6) /* - invalid keyring */

/* request-key default keyrings */
#define KEY_REQUEST_KEY_DEFL_NO_CHANGE			(-1) /* make no change to the current settings */
#define KEY_REQUEST_KEY_DEFL_DEFAULT			0 /* set to default behaviour */
#define KEY_REQUEST_KEY_DEFL_THREAD_KEYRING		1 /* use thread-specific keyring the default destination */
#define KEY_REQUEST_KEY_DEFL_PROCESS_KEYRING		2 /* use process-specific keyring the default destination */
#define KEY_REQUEST_KEY_DEFL_SESSION_KEYRING		3 /* use session-specific keyring the default destination */
#define KEY_REQUEST_KEY_DEFL_USER_KEYRING		4 /* use UID-specific keyring the default destination */
#define KEY_REQUEST_KEY_DEFL_USER_SESSION_KEYRING	5 /* use UID-session keyring the default destination */
#define KEY_REQUEST_KEY_DEFL_GROUP_KEYRING		6 /* not implemented yet */
#define KEY_REQUEST_KEY_DEFL_REQUESTOR_KEYRING		7 /* use the requestor keyring */

/* key_control syscall operations */
#define KEY_CONTROL_GET_KEYRING_ID		0 /* ask for a keyring's ID */
#define KEY_CONTROL_JOIN_SESSION_KEYRING	1 /* join or start named session keyring */
#define KEY_CONTROL_UPDATE			2 /* update a key */
#define KEY_CONTROL_REVOKE			3 /* revoke a key */
#define KEY_CONTROL_CHOWN			4 /* set ownership of a key */
#define KEY_CONTROL_SETPERM			5 /* set perms on a key */
#define KEY_CONTROL_DESCRIBE			6 /* describe a key */
#define KEY_CONTROL_CLEAR			7 /* clear contents of a keyring */
#define KEY_CONTROL_LINK			8 /* link a key into a keyring */
#define KEY_CONTROL_UNLINK			9 /* unlink a key from a keyring */
#define KEY_CONTROL_SEARCH			10 /* search for a key in a keyring */
#define KEY_CONTROL_READ			11 /* read a key or keyring's contents */
#define KEY_CONTROL_INSTANTIATE			12 /* instantiate a partially constructed key */
#define KEY_CONTROL_NEGATE			13 /* negate a partially constructed key */
#define KEY_CONTROL_SET_REQKEY_KEYRING		14 /* set default request-key keyring */
#define KEY_CONTROL_SET_TIMEOUT			15 /* set key timeout */
#define KEY_CONTROL_ASSUME_AUTHORITY		16 /* assume request_key() authorisation */
#define KEY_CONTROL_GET_SECURITY		17 /* get key security label */
#define KEY_CONTROL_SESSION_TO_PARENT		18 /* apply session keyring to parent process */
#define KEY_CONTROL_REJECT			19 /* reject a partially constructed key */
#define KEY_CONTROL_INSTANTIATE_IOV		20 /* instantiate a partially constructed key */
#define KEY_CONTROL_INVALIDATE			21 /* invalidate a key */
#define KEY_CONTROL_GET_PERSISTENT		22 /* get a user's persistent keyring */
#define KEY_CONTROL_DH_COMPUTE			23 /* Compute Diffie-Hellman values */
#define KEY_CONTROL_PKEY_QUERY			24 /* Query public key parameters */
#define KEY_CONTROL_PKEY_ENCRYPT		25 /* Encrypt a blob using a public key */
#define KEY_CONTROL_PKEY_DECRYPT		26 /* Decrypt a blob using a public key */
#define KEY_CONTROL_PKEY_SIGN			27 /* Create a public key signature */
#define KEY_CONTROL_PKEY_VERIFY			28 /* Verify a public key signature */
#define KEY_CONTROL_RESTRICT_KEYRING		29 /* Restrict keys allowed to link to a keyring */
#define KEY_CONTROL_MOVE			30 /* Move keys between keyrings */
#define KEY_CONTROL_CAPABILITIES		31 /* Find capabilities of keyrings subsystem */
#define KEY_CONTROL_OP_NUM			32 /* key control syscall operations num */

/* keycall parameter limit */
#define KEY_TYPE_MAX_LEN	32U
#define KEY_NAME_MAX_LEN	4096U
#define KEY_PAYLOAD_MAX_LEN	(1024U * 1024U)

/* KRCTL operations */
#define KRCTL_SETUID		0
#define KRCTL_SETGID		1
#define KRCTL_REFCNT_INC	2
#define KRCTL_REFCNT_DEC	3
#define KRCTL_INVAL		4

struct key_add_param {
	char *type;
	unsigned long tlen;
	char *name;
	unsigned long nlen;
	void *payload;
	unsigned long plen;
	int keyring_id;
	bool user_construct;
	/* whether the hash tree is locked in key_link */
	bool hash_tree_locked;
};

struct key_request_param {
	char *type;
	unsigned long tlen;
	char *name;
	unsigned long nlen;
	char *callout;
	unsigned long clen;
	int keyring_id;
};

/*
 * Brief:	Add or update key in key manangement
 *
 * @param	type [I] key type
 *		name [I] key name
 *		payload [I] payload of key
 *		plen [I] payload length
 *		link_keyring_id [I] the keyring that key will be linked
 *
 * @return	success, key id
 *		failed, return -1 and set errno
 * @note	None
 * @see		"man add_key" in linux
 * @deprecated	None
 */
long hm_key_add(const char *type, const char *name, const void *payload,
		size_t plen, int32_t link_keyring_id);

/*
 * Brief:	key request native interface, search a key by name and type
 *
 * @param	type [I] key type
 *		name [I] key name
 *		callout_info [I] not used now
 *		link_keyring_id [I] the keyring that key will be linked
 *
 * @return	success, return key unique id
 *		failed, return -1 and set errno
 * @note	None
 * @see		man request_key in linux
 * @deprecated	None
 */
long hm_key_request(const char *type, const char *name, const char *callout_info,
		    int32_t link_keyring_id);
/*
 * Brief:	Key Control native interface
 *
 * @param	operation [I] key control operation, just like UPDATE and READ
 *		arg# [I] meaning depends on the operation
 *
 * @return	success, return 0
 *		failed, return -1 and set errno
 * @note	None
 * @see		man 2 keyctl, see and libhmsrv_key/key.h
 * @deprecated	None
 */
long hm_key_control(int operation,
		    unsigned long arg2,
		    unsigned long arg3,
		    unsigned long arg4,
		    unsigned long arg5);

long hm_key_fscrypt_ctrl_actv(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size);
int hm_key_fsverity_ctrl_actv(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size);

#ifdef CONFIG_VFS_KEYMGR_DIRECT
static inline long hm_key_fscrypt_ctrl(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size)
{
	UNUSED(keymgr_rref);
	return keymgr_fscrypt_ctrl(operation, arg_addr, arg_size);
}

static inline int hm_key_fsverity_ctrl(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size)
{
	UNUSED(keymgr_rref);
	return keymgr_fsverity_ctrl(operation, arg_addr, arg_size);
}
#else
static inline long hm_key_fscrypt_ctrl(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size)
{
	return hm_key_fscrypt_ctrl_actv(keymgr_rref, operation, arg_addr, arg_size);
}

static inline int hm_key_fsverity_ctrl(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size)
{
	return hm_key_fsverity_ctrl_actv(keymgr_rref, operation, arg_addr, arg_size);
}
#endif /* CONFIG_VFS_KEYMGR_DIRECT */

int hm_key_krctl_actv(rref_t keymgr_rref, int operation, void *keyring, unsigned long arg);

#ifdef CONFIG_KEYMGR_FUSION
static inline int hm_key_krctl(rref_t keymgr_rref, int operation, void *keyring, unsigned long arg)
{
	UNUSED(keymgr_rref);
	return keymgr_krctl(operation, keyring, arg);
}
#else
static inline int hm_key_krctl(rref_t keymgr_rref, int operation, void *keyring, unsigned long arg)
{
	return hm_key_krctl_actv(keymgr_rref, operation, keyring, arg);
}
#endif /* CONFIG_KEYMGR_FUSION */
#endif
