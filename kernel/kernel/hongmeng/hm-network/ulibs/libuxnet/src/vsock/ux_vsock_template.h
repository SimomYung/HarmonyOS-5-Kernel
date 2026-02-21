/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for vsock templates
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#ifndef __UX_VSOCK_TEMPLATE_H_
#define __UX_VSOCK_TEMPLATE_H_

/**
 * Define a code template for binded list and connected list
 */
#define DEFINE_CONTAINER_LIST(name) \
	static DLIST_HEAD(name##_vsock_list); \
	static struct raw_mutex name##_vsock_list_lock = RAW_MUTEX_INITIALIZER; \
	static inline void lock_##name##_vsock_list(void) \
	{ \
		raw_mutex_lock(&name##_vsock_list_lock); \
	} \
	static inline void unlock_##name##_vsock_list(void) \
	{\
		raw_mutex_unlock(&name##_vsock_list_lock); \
	} \
	static inline void add_vsock_to_##name##_list_nolock(ux_vsock_sock_t *vsock) \
	{ \
		vsock_get(vsock); \
		dlist_insert(&name##_vsock_list, &vsock->node_for_##name); \
	} \
	static inline void add_vsock_to_##name##_list(ux_vsock_sock_t *vsock) \
	{ \
		lock_##name##_vsock_list(); \
		vsock_get(vsock); \
		dlist_insert(&name##_vsock_list, &vsock->node_for_##name); \
		unlock_##name##_vsock_list(); \
	} \
	static inline void rm_vsock_from_##name##_list_nolock(ux_vsock_sock_t *vsock) \
	{ \
		if (!dlist_empty(&vsock->node_for_##name)) { \
			dlist_delete(&vsock->node_for_##name); \
			vsock_put(vsock); \
		} \
	} \
	static inline void rm_vsock_from_##name##_list(ux_vsock_sock_t *vsock) \
	{ \
		lock_##name##_vsock_list(); \
		rm_vsock_from_##name##_list_nolock(vsock); \
		unlock_##name##_vsock_list(); \
	}

#define DECLARE_VSOCK_STATE(st_name) \
	void vsock_set_##st_name(ux_vsock_sock_t *vsock)

#define DEFINE_VSOCK_STATE(st_name, e_state, f_connect, f_recv_msg) \
	static struct vsock_state vsock_state_##st_name = { \
		.state_name = #st_name, \
		.state = (e_state), \
		.connect = (f_connect), \
		.recv_msg = (f_recv_msg) \
	}; \
	void vsock_set_##st_name(ux_vsock_sock_t *vsock) \
	{ \
		vsock->pstate = &vsock_state_##st_name; \
	}

#endif
