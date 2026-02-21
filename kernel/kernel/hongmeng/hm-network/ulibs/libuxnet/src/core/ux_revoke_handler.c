/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for network pktbuf
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 21 09:55:16 2021
 */

#include <stdlib.h>
#include <lib/dlist.h>
#include <hongmeng/errno.h>

#include "osal.h"
#include "ux_netlog.h"
#include "ux_socket.h"
#include "ux_fdtable.h"
#include "ux_revoke_handler.h"

#define for_each_revoke_handler(iter) \
	dlist_for_each_entry(iter, &revoke_handlers, \
			     struct ux_revoke_handler_holder, node)

struct ux_revoke_handler_holder {
	struct dlist_node node;

	const char *name;
	ux_revoke_handler_t handler;
};

/**
 * Do not use any lock here, for register is during hmnetd starting and
 * unregister is during hmnetd exiting, can be guarantee to be sequence.
 */
static DLIST_HEAD(revoke_handlers);

static struct ux_revoke_handler_holder *
ux_find_revoke_handler(const ux_revoke_handler_t handler)
{
	struct ux_revoke_handler_holder *iter = NULL;
	struct ux_revoke_handler_holder *result = NULL;

	for_each_revoke_handler(iter) {
		if (iter->handler == handler) {
			result = iter;
			break;
		}
	}

	return result;
}

int ux_register_revoke_handler(const char *name, const ux_revoke_handler_t handler)
{
	struct ux_revoke_handler_holder *holder = NULL;
	int ret = E_HM_OK;

	holder = ux_find_revoke_handler(handler);
	if (holder != NULL) {
		ret = E_HM_OBJEXIST;
	} else {
		holder = malloc(sizeof(*holder));
		if (holder == NULL) {
			ret = E_HM_NOMEM;
		} else {
			dlist_init(&holder->node);
			holder->name = name;
			holder->handler = handler;
			dlist_insert(&revoke_handlers, &holder->node);
		}
	}

	return ret;
}

void ux_unregister_revoke_handler(const ux_revoke_handler_t handler)
{
	struct ux_revoke_handler_holder *holder = ux_find_revoke_handler(handler);

	if (holder != NULL) {
		dlist_delete(&holder->node);
		free(holder);
	}
}

void net_do_balance_rpccnt(void *common_file, unsigned int rpccnt)
{
	uint64_t res = 0ULL;
	ux_socket_t *sock = (ux_socket_t *)common_file;

	NOFAIL(raw_compounded_rpccnt_balance(&sock->refcnt, (uint64_t)rpccnt, &res));
	BUG_ON(res == 0ULL);
}

static void balance_rpccnt_of_sockets(uint32_t cnode_idx)
{
	int err;
	ux_socket_t *sock = NULL;
	unsigned int fd_start = 0U;
	size_t num = PAGE_SIZE / sizeof(struct sysarg_fdtable_rpccnt_file);
	struct sysarg_fdtable_rpccnt_file *data = NULL;

	ux_sock_owner_t *owner = ux_sock_owner_get(cnode_idx, 0);
	if (owner == NULL) {
		return;
	}

	data = (struct sysarg_fdtable_rpccnt_file *)osal_malloc(PAGE_SIZE);
	if (data == NULL) {
		net_error(HMNET_CORE, "failed to alloc data\n");
		ux_sock_owner_put(owner);
		return;
	}

	do {
		err = syscap_FDTableGatherCommonFiles(owner->fdtable_rref,
						      data, num, &fd_start,
						      GATHER_FILES_AND_RPCCNT, FD_NET);
		if (err <= 0) {
			break;
		}
		BUG_ON(err > (int)num);
		for (int i = 0; i < err; i++) {
			uint64_t res = 0ULL;
			unsigned int rpccnt = data[i].rpccnt;
			sock = (ux_socket_t *)(uintptr_t)data[i].common_file;

			NOFAIL(raw_compounded_rpccnt_balance(&sock->refcnt,
							     (uint64_t)rpccnt,
							     &res));
			BUG_ON(res == 0ULL);
		}
	} while (err == (int)num);

	osal_free(data);
	ux_sock_owner_put(owner);
}

void net_do_revoke_by_cnode(uint32_t cnode_idx)
{
	struct ux_revoke_handler_holder *iter = NULL;

	for_each_revoke_handler(iter) {
		iter->handler(cnode_idx);
	}
}

void ux_revoke_by_cnode(uint32_t cnode_idx)
{
	struct ux_revoke_handler_holder *iter = NULL;

	for_each_revoke_handler(iter) {
		balance_rpccnt_of_sockets(cnode_idx);
		iter->handler(cnode_idx);
	}
}
