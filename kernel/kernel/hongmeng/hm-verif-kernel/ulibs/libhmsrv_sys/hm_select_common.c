/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Supply epoll server common interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 12 21:00:44 2020
 */

#include <libhmsrv_sys/hm_select_common.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <vfs_epoll.h>

static inline void __init_eventpoll_node(struct eventpoll_node_s *node,
					 int fd, uint32_t events, uint64_t key,
					 struct eventpoll_ops_t *ops)
{
	node->events = events;
	node->fd = fd;
	node->key = key;
	node->ops = ops;
	dlist_init(&node->link);
}

static void try_fdtable_wakeup_poll(struct ppoll_node *node, uint32_t revents)
{
	int err;

	if (node->info.revents != revents) {
		err = syscap_FDTableWakeupPoll(node->info.fdtable_rref,
					       node->info.tcb_key,
					       node->node.fd, node->info.idx,
					       (short)revents);
		if (err != E_HM_OK) {
			hm_error("wakeup poll or select failed: %s\n", hmstrerror(err));
		} else {
			node->info.revents = revents;
		}
	}
}

static void __wakeup_poll_node(struct eventpoll_node_s *node, uint32_t revents)
{
	struct ppoll_node *entry = container_of(node, struct ppoll_node, node);
	uint32_t new_revents = node->events & revents;
	try_fdtable_wakeup_poll(entry, new_revents);
}

static void __cleanup_poll_node(struct eventpoll_node_s *node, uint32_t revents)
{
	struct ppoll_node *entry = container_of(node, struct ppoll_node, node);
	uint32_t new_revents = entry->info.revents & ~revents;
	try_fdtable_wakeup_poll(entry, new_revents);
}

static void __add_event_poll_node(struct eventpoll_node_s *node, uint32_t revents)
{
	struct ppoll_node *entry = container_of(node, struct ppoll_node, node);
	uint32_t new_revents = entry->info.revents | revents;
	try_fdtable_wakeup_poll(entry, new_revents);
}

static void __close_poll_node(struct eventpoll_node_s *node, uint32_t revents)
{
	struct ppoll_node *entry = container_of(node, struct ppoll_node, node);

	if (revents != 0) {
		try_fdtable_wakeup_poll(entry, revents);
	}
	dlist_delete(&node->link);
	free(entry);
}

static void __remove_poll_node(struct eventpoll_node_s *node, uint64_t private, void **mem)
{
	struct ppoll_node *entry = container_of(node, struct ppoll_node, node);
	if (entry->info.tcb_key == private) {
		dlist_delete(&node->link);
		if (mem != NULL) {
			*mem = entry;
		} else {
			free(entry);
		}
	}
}

/*
 * If E_HM_CNODE_LOOKUP and E_HM_CNODE_LOOKUP_R are returned, cap_epoll has been deleted.
 * If E_HM_CNODE_TYPE is returned, the cap_epoll is released and the cref is multiplexed by other types of caps.
 * In this scenario, the corresponding cap_epoll needs to be deleted from epoll_list.
 */
#define convert_epoll_syscap_err(_ret) \
	do { \
		if ((_ret) == E_HM_CNODE_TYPE || (_ret) == E_HM_CNODE_LOOKUP || (_ret) == E_HM_CNODE_LOOKUP_R) { \
			(_ret) = E_HM_NON_UNIFORM; \
		} \
	} while (0) \

static void __close_ep_node(struct eventpoll_node_s *node, uint32_t revents)
{
	struct __epoll_ctl_info_s ctl_info = {0};
	struct epoll_node *entry = container_of(node, struct epoll_node, node);

	/* delete epitem */
	ctl_info.op = EPOLL_CTL_DEL;
	ctl_info.events_triggered = revents;
	ctl_info.fd = entry->node.fd;
	ctl_info.key = entry->node.key;
	(void)syscap_EpollCtl(entry->cap.rref, &ctl_info, entry->cap.tock);

	/* delete ep node */
	dlist_delete(&node->link);
	free(entry);
}

static void __wakeup_ep_node(struct eventpoll_node_s *node, uint32_t revents)
{
	int ret;

	if (node == NULL) {
		hm_error("eventpoll wakeup failed: invalid arguments\n");
		return;
	}

	if (retrive_valid_events(node->events, revents) == 0U) {
		return;
	}

	struct epoll_node *entry = container_of(node, struct epoll_node, node);
	ret = syscap_EpollWakeup(entry->cap.rref, node->fd, revents, node->key, entry->cap.tock);
	convert_epoll_syscap_err(ret);
	if (ret == E_HM_NON_UNIFORM) {
		hm_debug("the cref does not avaliable anymore, release it\n");
		dlist_delete(&node->link);
		free(entry);
	} else if (ret != E_HM_OK) {
		hm_warn("eventpoll wakeup failed: notify cap epoll returns %s\n", hmstrerror(ret));
	}
}

static void __cleanup_ep_node(struct eventpoll_node_s *node, uint32_t revents)
{
	struct epoll_node *entry = container_of(node, struct epoll_node, node);
	int ret = syscap_EpollCleanup(entry->cap.rref, node->fd, revents, node->key, entry->cap.tock);
	convert_epoll_syscap_err(ret);
	if (ret == E_HM_NON_UNIFORM) {
		hm_debug("the cref does not avaliable anymore, release it\n");
		dlist_delete(&node->link);
		free(entry);
	}
}

static void __remove_ep_node(struct eventpoll_node_s *node, uint64_t private, void **mem)
{
	struct epoll_node *entry = container_of(node, struct epoll_node, node);
	if (entry->cap.cref == (cref_t)private) {
		dlist_delete(&node->link);
		if (mem != NULL) {
			*mem = entry;
		} else {
			free(entry);
		}
	}
}

static void __add_event_ep_node(struct eventpoll_node_s *node, uint32_t revents)
{
	UNUSED(node, revents);
}

static struct eventpoll_ops_t g_eventpoll_ops[] __read_mostly = {
	[EVENTPOLL_TYPE_EPOLL] = {
		.wakeup  = __wakeup_ep_node,
		.cleanup = __cleanup_ep_node,
		.close   = __close_ep_node,
		.remove  = __remove_ep_node,
		.add_event = __add_event_ep_node,
	},
	[EVENTPOLL_TYPE_PPOLL] = {
		.wakeup  = __wakeup_poll_node,
		.cleanup = __cleanup_poll_node,
		.close   = __close_poll_node,
		.remove  = __remove_poll_node,
		.add_event = __add_event_poll_node,
	}
};

static int __insert_ep_node(struct dlist_node *epoll_list, const struct epoll_info_arg *arg,
			    struct epoll_node **epnode)
{
	int err = E_HM_OK;
	struct epoll_node *entry = NULL;
	size_t entry_size = sizeof(struct epoll_node);
	const struct epoll_cap_info_s *cap_info = &arg->cap_info;
	const struct __epoll_ctl_info_s *ctl_info = &arg->ctl_info;

	if (epnode != NULL && *epnode != NULL) {
		entry = *epnode;
	} else {
		entry = malloc(sizeof(struct epoll_node));
		if (entry == NULL) {
			hm_warn("no memory for epoll.\n");
			err = E_HM_NOMEM;
		} else if (epnode != NULL) {
			*epnode = entry;
		}
	}

	if (err == E_HM_OK) {
		NOFAIL(memset_s(entry, entry_size, 0, entry_size));
		NOFAIL(memcpy_s(&entry->cap, sizeof(struct epoll_cap_grant_s),
				&cap_info->cap, sizeof(struct epoll_cap_grant_s)));
		__init_eventpoll_node(&entry->node, ctl_info->fd, (uint32_t)ctl_info->events_waited,
				      (uint64_t)ctl_info->key,  &g_eventpoll_ops[EVENTPOLL_TYPE_EPOLL]);
		dlist_insert_tail(epoll_list, &entry->node.link);
	}

	return err;
}

static int __delete_ep_node(struct dlist_node *epoll_list,
			    const struct epoll_cap_info_s *cap_info,
			    const struct __epoll_ctl_info_s *ctl_info,
			    struct epoll_node **epnode)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;

	dlist_for_each_entry_safe(pos, n, epoll_list, struct eventpoll_node_s, link) {
		/* In the kmsg scenario, key = ptr_to_u64(file) */
		if (pos->key != ctl_info->key) {
			continue;
		}

		struct epoll_node *entry = container_of(pos, struct epoll_node, node);
		if (entry->cap.cref == cap_info->cap.cref &&
		    entry->cap.tock == cap_info->cap.tock) {
			dlist_delete(&pos->link);
			if (epnode != NULL) {
				*epnode = entry;
			} else {
				free(entry);
			}
			break;
		}
	}

	return E_HM_OK;
}

static int __modify_ep_node(struct dlist_node *epoll_list,
			    const struct epoll_cap_info_s *cap_info,
			    const struct __epoll_ctl_info_s *ctl_info,
			    struct epoll_node **epnode)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;
	struct epoll_node *entry = NULL;

	dlist_for_each_entry_safe(pos, n, epoll_list, struct eventpoll_node_s, link) {
		/* In the kmsg scenario, key = ptr_to_u64(file) and other key = 0 */
		if (pos->key != ctl_info->key) {
			continue;
		}

		entry = container_of(pos, struct epoll_node, node);
		if (entry->cap.cref == cap_info->cap.cref &&
		    entry->cap.tock == cap_info->cap.tock) {
			pos->events = ctl_info->events_waited;
			break;
		}
	}

	if (epnode != NULL) {
		*epnode = entry;
	}

	return E_HM_OK;
}

static int __do_epoll_ctl(struct dlist_node *epoll_list, const struct epoll_info_arg *arg,
			  struct epoll_node **epnode)
{
	int err;
	const struct epoll_cap_info_s *cap_info = &arg->cap_info;
	const struct __epoll_ctl_info_s *ctl_info = &arg->ctl_info;

	switch (ctl_info->op) {
	case EPOLL_CTL_ADD:
		err = __insert_ep_node(epoll_list, arg, epnode);
		break;
	case EPOLL_CTL_DEL:
		err = __delete_ep_node(epoll_list, cap_info, ctl_info, epnode);
		break;
	case EPOLL_CTL_MOD:
		err = __modify_ep_node(epoll_list, cap_info, ctl_info, epnode);
		break;
	default:
		err = E_HM_INVAL;
		break;
	}

	return err;
}

static int __do_cap_epoll_grant(struct server_info *si, int epfd, __u32 cnode, rref_t *rref)
{
	int err = E_HM_OK;

	rref_t rref_tmp = hm_ucap_require(0, si->cnode_idx, CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_RREF);
	if (IS_REF_ERR(rref_tmp)) {
		hm_warn("require temp rref failed, err: %s\n", hmstrerror(REF_TO_ERR(rref_tmp)));
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = vfs_epoll_rref_grant(si, epfd, cnode, rref_tmp, rref);
		if (err == E_HM_OK) {
			*rref = rref_tmp;
		} else if (err == E_HM_EXIST) {
			/*
			 * return E_HM_EXIST, indicates that other threads have authorized the server.
			 * In this case, the temporary rref should be reclaimed.
			 */
			(void)hm_ucap_reject(0, rref_tmp);
			err = E_HM_OK;
		} else {
			(void)hm_ucap_reject(0, rref_tmp);
			hm_warn("epoll rref grant failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}

int eventpoll_ctl_common(struct dlist_node *epoll_list, struct epoll_info_arg *arg, uint32_t revents,
			 struct epoll_node **ep_info)
{
	int err = E_HM_OK;
	struct __epoll_ctl_info_s *ctl_info = NULL;
	struct epoll_cap_info_s *cap_info = NULL;
	struct __epoll_ctl_info_s __info = { 0 };

	if (epoll_list == NULL || arg == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		ctl_info = &arg->ctl_info;
		cap_info = &arg->cap_info;
		ctl_info->events_triggered = revents;
		err = syscap_EpollCtl(cap_info->cap.rref, ctl_info, cap_info->cap.tock);
		if (err != E_HM_OK) {
			hm_warn("Failed to do epoll_ctl on fd %d with error:%s\n",
				ctl_info->fd, hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		err = __do_epoll_ctl(epoll_list, arg, ep_info);
		if (err != E_HM_OK && ctl_info->op == EPOLL_CTL_ADD) {
			hm_warn("Failed to add epoll node, rollback\n");
			__info.op = EPOLL_CTL_DEL;
			__info.fd = ctl_info->fd;
			__info.key = ctl_info->key;
			(void)syscap_EpollCtl(cap_info->cap.rref, &__info, cap_info->cap.tock);
		}
	}

	return err;
}

int eventpoll_cap_epoll_grant(int epfd, unsigned int ap_cnode, struct epoll_cap_grant_s *cap)
{
	int err = E_HM_OK;
	struct server_info *si = NULL;

	if (cap == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = get_server_info("fsmgr", &si);
		if (err != E_HM_OK) {
			hm_warn("get vfs server info failed, err: %s\n", hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		cap->rref = ERR_TO_REF(E_HM_INVAL);

		/*
		 * return E_HM_OK, indicating that the user has been authorized.
		 * return E_HM_NOENT, indicating that the user need to be authorized.
		 * return other ERROR, indicating that the calling is failed.
		 */
		err = vfs_epoll_rref_acquire(si, epfd, (__u32)ap_cnode, cap);
		if (err == E_HM_NOENT) {
			cap->rref = ERR_TO_REF(E_HM_NOENT);
			err = E_HM_OK;
		} else if (err != E_HM_OK) {
			hm_warn("epoll rref acquire failed: %s\n", hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		if (IS_REF_VALID(cap->rref)) {
			/* acquire rref successfully, do nothing */
		} else {
			err = __do_cap_epoll_grant(si, epfd, (__u32)ap_cnode, &cap->rref);
		}
	}

	put_server_info(si);
	return err;
}

int eventpoll_add(struct dlist_node *eventpoll_list,
		  struct ppoll_info_s *info,
		  uint32_t events, int fd, uint64_t key,
		  struct ppoll_node **node)
{
	int err = E_HM_OK;
	struct ppoll_node *entry = NULL;
	size_t size = sizeof(struct ppoll_node);

	if (info == NULL || eventpoll_list == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (node != NULL && *node != NULL) {
			/*
			 * binderfs would malloc memory outside of eventpoll
			 * context lock
			 */
			entry = *node;
		} else {
			entry = malloc(sizeof(struct ppoll_node));
			if (entry == NULL) {
				err = E_HM_NOMEM;
			} else if (node != NULL) {
				*node = entry;
			}
		}
	}

	if (err == E_HM_OK) {
		NOFAIL(memset_s(entry, size, 0, size));
		NOFAIL(memcpy_s(&entry->info, sizeof(struct ppoll_info_s),
				info, sizeof(struct ppoll_info_s)));
		__init_eventpoll_node(&entry->node, fd, events, key,
				      &g_eventpoll_ops[EVENTPOLL_TYPE_PPOLL]);
		dlist_insert_tail(eventpoll_list, &entry->node.link);
	}

	return err;
}

void eventpoll_wakeup_sink(struct dlist_node *eventpoll_list, uint32_t revents)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;

	if (eventpoll_list != NULL) {
		dlist_for_each_entry_safe(pos, n, eventpoll_list,
					  struct eventpoll_node_s, link) {
			eventpoll_ops_exec(wakeup, pos->ops, pos, revents);
		}
	}
}

void eventpoll_cleanup_sink(struct dlist_node *eventpoll_list, uint32_t revents, uint64_t key)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;

	if (eventpoll_list != NULL) {
		dlist_for_each_entry_safe(pos, n, eventpoll_list,
					  struct eventpoll_node_s, link) {
			/*
			 * /dev/kmsg has a global eventpoll_list, but other file has one
			 * eventpoll_list each struct file
			 * In the kmsg scenario, key = ptr_to_u64(file) and other key = 0
			 */
			if (key != 0 && pos->key != key) {
				continue;
			}
			eventpoll_ops_exec(cleanup, pos->ops, pos, revents);
		}
	}
}

void eventpoll_add_event_sink(struct dlist_node *eventpoll_list, uint32_t revents, uint64_t key)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;

	if (eventpoll_list != NULL) {
		dlist_for_each_entry_safe(pos, n, eventpoll_list,
					  struct eventpoll_node_s, link) {
			/*
			 * /dev/kmsg has a global eventpoll_list, but other file has one
			 * eventpoll_list each struct file
			 * In the kmsg scenario, key = ptr_to_u64(file) and other key = 0
			 */
			if (key != 0 && pos->key != key) {
				continue;
			}
			eventpoll_ops_exec(add_event, pos->ops, pos, revents);
		}
	}
}

void eventpoll_close_sink(struct dlist_node *eventpoll_list, uint32_t revents, uint64_t key)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;

	if (eventpoll_list != NULL) {
		dlist_for_each_entry_safe(pos, n, eventpoll_list,
					  struct eventpoll_node_s, link) {
			/*
			 * /dev/kmsg has a global eventpoll_list, but other file has one
			 * eventpoll_list each struct file
			 * In the kmsg scenario, key = ptr_to_u64(file) and other key = 0
			 */
			if (key != 0 && pos->key != key) {
				continue;
			}
			eventpoll_ops_exec(close, pos->ops, pos, revents);
		}
	}
}

void eventpoll_remove_sink(struct dlist_node *eventpoll_list, uint64_t key, uint64_t private, void **mem)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *n = NULL;

	if (eventpoll_list != NULL) {
		dlist_for_each_entry_safe(pos, n, eventpoll_list, struct eventpoll_node_s, link) {
			/*
			 * /dev/kmsg has a global eventpoll_list, but other file has one
			 * eventpoll_list each struct file
			 * In the kmsg scenario, key = ptr_to_u64(file) and other key = 0
			 */
			if (key != 0U && pos->key != key) {
				continue;
			}
			eventpoll_ops_exec(remove, pos->ops, pos, private, mem);
		}
	}
}

int eventpoll_select_common(struct select_ctl_info_s *select, rref_t fdtable, select_handler func)
{
	int err = E_HM_OK;

	if (select == NULL || func == NULL || IS_REF_ERR(fdtable)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		for (int i = FDSET_IDX_READ; i < FDSET_IDX_MAX; i++) {
			FD_ZERO(&select->tmpfds[i]);
		}
	}

	/* fetch the metadata info of select */
	if (err == E_HM_OK) {
		err = syscap_FDTableFetchSelect(fdtable, select->tcb_cref,
						(__u32 *)&select->nfds,
						(void *)select->fdsets,
						(__u64 *)&select->tmpfds[FDSET_IDX_READ],
						(__u64 *)&select->tmpfds[FDSET_IDX_WRITE],
						(__u64 *)&select->tmpfds[FDSET_IDX_EXP]);
		if (err != E_HM_OK) {
			hm_warn("failed to fetch select metadata\n");
		}
	}

	if (err == E_HM_OK) {
		err = func(select, fdtable, select->cnode_idx);
	}

	return err;
}

uint32_t select_waited_events(int fd, const fd_set tmpfds[])
{
	uint32_t events = 0U;
	uint32_t select_events[FDSET_IDX_MAX] = { POLLIN_SET, POLLOUT_SET, POLLEX_SET };

	for (int i = FDSET_IDX_READ; i < FDSET_IDX_MAX; i++) {
		if (FD_ISSET(fd, &tmpfds[i])) {
			events |= select_events[i]; /* POLLIN_SET, POLLOUT_SET, POLLEX_SET */
		}
	}

	return events;
}

int select_triggered_events(int fd, uint32_t revents, fd_set tmpfds[])
{
	int count = 0;
	uint32_t select_events[FDSET_IDX_MAX] = { POLLIN_SET, POLLOUT_SET, POLLEX_SET };

	for (int i = FDSET_IDX_READ; i < FDSET_IDX_MAX; i++) {
		if (!FD_ISSET(fd, &tmpfds[i])) {
			continue;
		}

		/* if the event waited not occur, clear the corresponding fdset */
		if ((revents & select_events[i]) == 0U) { /* POLLIN_SET, POLLOUT_SET, POLLEX_SET */
			FD_CLR(fd, &tmpfds[i]);
		} else {
			count++;
		}
	}

	return count;
}
