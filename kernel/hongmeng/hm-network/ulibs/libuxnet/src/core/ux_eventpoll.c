/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network eventpoll
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 30 14:29:17 2019
 */
#include "ux_netdev.h"
#include "ux_eventpoll.h"
#include "ux_libldk_pub.h"

#include <vfs.h>
#include <libstrict/strict.h>
#include <libhmsrv_io/net_socket.h>
#include <libhmsrv_io/dev.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

unsigned int do_epoll_getevents(ux_socket_t *sock)
{
	unsigned int rcvev = 0;
	unsigned int sendev = 0;
	unsigned int errev = 0;
	unsigned int events = 0;
	int sock_error = 0;
	int ret;

	ret = socket_getsockevent(sock, &rcvev, &sendev, &errev, &sock_error);
	if (ret != E_HM_OK) {
		return EPOLLERR;
	}
	events = rcvev | sendev | errev;

	/*
	 * Currently, the network has a cache on the uapp side. If uapp doesn't
	 * read the recv data for the first time, the remaining data will be stored
	 * in the cache, and this event will be kept in sock->events. So when the
	 * user calls the select/poll/epoll interface, it also needs to return the
	 * event retained in sock->events.
	 */
	events |= sock->events;

	return events;
}

static int do_sync_polllist_revents(ux_epoll_context_t *ctx, unsigned int revents)
{
	int ret = E_HM_INVAL;

	if (ctx == NULL) {
		return ret;
	}

	if (ctx->kobj_polllist != NULL) {
		ret = hm_update_polllist_events(ctx->kobj_polllist, revents);
	} else if (ctx->polllist != NULL) {
		ret = sysctrlmem_kobjctnr_sync_polllist_revents(ctx->polllist, revents);
	}

	return ret;
}

static int do_clean_polllist_revents(ux_epoll_context_t *ctx, unsigned int revents)
{
	int ret = E_HM_INVAL;

	if (ctx == NULL) {
		return ret;
	}

	if (ctx->kobj_polllist != NULL) {
		ret = hm_cleanup_polllist_events(ctx->kobj_polllist, revents);
	} else if (ctx->polllist != NULL) {
		ret = sysctrlmem_kobjctnr_clean_polllist_revents(ctx->polllist, revents);
	}

	return ret;
}

int ux_spawn_polllist(ux_socket_t *sock, ux_epoll_context_t *epoll_ctx, int use_polllist)
{
	int ret = E_HM_OK;

	/*
	 * If use_polllist is set to SHARE_POLLLIST, the readable and writable ppoll shared memory solution is used.
	 * If the use_polllist value is others, the non-writable ppoll kernel object scheme is used.
	 */
	if (use_polllist == SHARE_POLLLIST) {
		epoll_ctx->kobj_polllist = hm_create_polllist((uintptr_t)sock, 0);
	} else {
		epoll_ctx->polllist = hm_ctrlmem_kobjpool_spawn_polllist((uintptr_t)sock, 0, NULL);
	}

	if (epoll_ctx->polllist != NULL || epoll_ctx->kobj_polllist != NULL) {
		ux_epoll_ctx_set_socket(sock, epoll_ctx);
	} else {
		ret = E_HM_NOMEM;
	}

	return ret;
}

void ux_epoll_wakeup_clean(ux_epoll_context_t *ctx, uint32_t events)
{
	unsigned int revents = 0;
	int ret = E_HM_OK;

	if (ctx == NULL) {
		return;
	}

	raw_mutex_lock(&ctx->epi_mutex);
	if (ctx->polllist != NULL || ctx->kobj_polllist != NULL) {
		if ((events & WAKEUP_MASK) != 0U) {
			revents = do_epoll_getevents(ux_epoll_ctx_get_socket(ctx));
			ret = do_sync_polllist_revents(ctx, revents | (events & WAKEUP_MASK));
			if (ret != E_HM_OK) {
				net_warn(HMNET_CORE, "failed to sync polllist revents when wakeup %s\n", hmstrerror(ret));
			}
		} else {
			revents = (events & CLEAN_MASK) >> CLEAN_EVENT_SHIFT;
			ret = do_clean_polllist_revents(ctx, revents);
			if (ret != E_HM_OK) {
				net_warn(HMNET_CORE, "failed to cleanup revents 0x%x, %s\n", revents, hmstrerror(ret));
			}
		}
	}
	raw_mutex_unlock(&ctx->epi_mutex);
}

int ux_epoll_recheck(ux_socket_t *sock, uint32_t events, uint32_t *revents)
{
	if (sock == NULL) {
		return E_HM_INVAL;
	}

	if (sock->ops != NULL && sock->ops->epoll_recheck_revents != NULL) {
		(void)sock->ops->epoll_recheck_revents(sock, events, revents);
	} else {
		uint32_t ret = do_epoll_getevents(sock);
		if (ret != 0) {
			*revents = ret & events;
		}
	}

	return E_HM_OK;
}

void ux_clear_eventpoll_list(ux_socket_t *sock)
{
	ux_epoll_context_t *epoll_ctx = ux_socket_get_epoll_ctx(sock);
	int ret = E_HM_OK;

	if (epoll_ctx != NULL) {
		raw_mutex_lock(&epoll_ctx->epi_mutex);
		if (epoll_ctx->kobj_polllist != NULL) {
			ret = hm_destroy_polllist(epoll_ctx->kobj_polllist);
		} else if (epoll_ctx->polllist != NULL) {
			ret = hm_ctrlmem_kobjpool_destroy(epoll_ctx->polllist);
		}
		if (ret != E_HM_OK) {
			net_warn(HMNET_CORE, "failed to destroy polllist revents %s\n", hmstrerror(ret));
		}
		epoll_ctx->kobj_polllist = NULL;
		epoll_ctx->polllist = NULL;
		epoll_ctx->socket = NULL;

		raw_mutex_unlock(&epoll_ctx->epi_mutex);
	} else {
		net_error(HMNET_CORE, "failed to clear eventpoll list\n");
	}
}

void ux_sync_polllist_revents(ux_socket_t *sock)
{
	if (sock == NULL) {
		return;
	}

	if (sock->ops != NULL && sock->ops->sync_ctrlmem_polllist != NULL) {
		sock->ops->sync_ctrlmem_polllist(sock);
	} else {
		ux_epoll_context_t *epoll_ctx = NULL;
		uint32_t revents = 0;
		int ret = 0;

		epoll_ctx = ux_socket_get_epoll_ctx(sock);
		if (epoll_ctx != NULL) {
			raw_mutex_lock(&epoll_ctx->epi_mutex);
			if (epoll_ctx->polllist != NULL || epoll_ctx->kobj_polllist != NULL) {
				revents = do_epoll_getevents(sock);
				ret = do_sync_polllist_revents(epoll_ctx, revents);
			}
			raw_mutex_unlock(&epoll_ctx->epi_mutex);
			if (ret != E_HM_OK) {
				net_warn(HMNET_CORE, "failed to sync polllist revents %s\n", hmstrerror(ret));
			}
		}
	}
}

void ux_set_fd_config(ux_socket_t *sock, struct sysarg_fdtable_fd_install *config)
{
	if (sock == NULL) {
		return;
	}

	if (sock->ops != NULL && sock->ops->fd_config != NULL) {
		sock->ops->fd_config(sock, config);
	} else {
		ux_epoll_context_t *epoll_ctx = ux_socket_get_epoll_ctx(sock);
		if (epoll_ctx != NULL && config != NULL) {
			config->common_file = epoll_ctx->polllist;
			config->flags |= FDTABLE_FD_POLLABLE;
		}
	}
}
