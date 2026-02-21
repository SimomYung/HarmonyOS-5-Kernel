/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device epoll for fdtable sink
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 26 13:16:18 2023
 */
#include "devhost_poll.h"
#include <devhost/poll.h>

#include <devhost_api.h>
#include <devhost/file.h>
#include <hongmeng/types.h>

#include "types.h"
#include "devhost.h"
#include "utils/utils.h"
#include <devhost/log.h>

struct devhost_epoll_node {
	struct eventpoll_node_s node;
	struct epoll_cap_grant_s cap;
	struct raw_refcnt refcnt;
	struct devhost_poll_wqueues_info pwq_info;
};

static int __dh_do_epoll_ctl(struct epoll_info_arg *arg, unsigned int revents)
{
	int ret;
	struct epoll_cap_info_s *cap_info = &arg->cap_info;
	struct __epoll_ctl_info_s *ctl_info = &arg->ctl_info;

	if (revents != 0U) {
		revents |= EPOLLRECHECK;
	}
	ctl_info->events_triggered = revents;
	ret = syscap_EpollCtl(cap_info->cap.rref, ctl_info, cap_info->cap.tock);

	return ret;
}

static void __dh_do_epoll_ctl_del(struct devhost_epoll_node *enode, uint32_t revents)
{
	struct __epoll_ctl_info_s ctl_info = {0};
	ctl_info.fd = enode->node.fd;
	ctl_info.op = EPOLL_CTL_DEL;
	ctl_info.events_triggered = revents;
	ctl_info.key = enode->node.key;

	(void)syscap_EpollCtl(enode->cap.rref, &ctl_info, enode->cap.tock);
}

static int __devhost_epoll_wakeup(void *target, unsigned int revents)
{
	struct devhost_epoll_node *enode = NULL;

	if (target == NULL) {
		dh_warn("invalid params\n");
		return -hmerrno2posix(E_HM_INVAL);
	}

	enode = (struct devhost_epoll_node *)target;
	revents |= EPOLLRECHECK;
	return syscap_EpollWakeup(enode->cap.rref, enode->node.fd, revents,
				  enode->node.key, enode->cap.tock);
}
static void __dh_release_epoll_node(struct raw_refcnt *ref)
{
	struct devhost_epoll_node *enode = NULL;

	enode = container_of(ref, struct devhost_epoll_node, refcnt);
	if (enode == NULL) {
		return;
	}

	devhost_release_pwq(&enode->pwq_info);
	free(enode);
}

static int __dh_insert_epoll_node(struct filp_node *fnode,
				  const struct epoll_info_arg *arg,
				  struct devhost_epoll_node **ep_node,
				  struct devhost_poll_info *pinfo)
{
	struct dlist_node *head = NULL;
	struct eventpoll_node_s *node = NULL;
	struct devhost_epoll_node *enode = NULL;

	enode = (struct devhost_epoll_node *)malloc(sizeof(*enode));
	if (enode == NULL) {
		dh_warn("malloc failed\n");
		return E_HM_NOMEM;
	}

	node = &enode->node;
	dlist_init(&node->link);
	node->key = arg->ctl_info.key;
	node->fd = arg->ctl_info.fd;
	node->events = arg->ctl_info.events_waited;
	NOFAIL(memcpy_s(&enode->cap, sizeof(struct epoll_cap_grant_s),
			&arg->cap_info.cap, sizeof(struct epoll_cap_grant_s)));
	/* It needs to be held when inserted to ep_list and calling poll() */
	raw_refcnt_init(&enode->refcnt, 2, __dh_release_epoll_node);
	devhost_init_pwq_info(&enode->pwq_info, NULL, NULL);

	pinfo->pwq_info = &enode->pwq_info;
	pinfo->target = (void *)enode;
	pinfo->wakeup = __devhost_epoll_wakeup;

	head = epoll_list_of_filp(fnode);
	filp_epoll_list_lock(fnode);
	dlist_insert_tail(head, &node->link);
	filp_epoll_list_unlock(fnode);

	*ep_node = enode;
	return E_HM_OK;
}

static void __dh_remove_epoll_node(struct filp_node *fnode,
				   struct devhost_epoll_node *enode)
{
	/* maybe aready delete by cmd epoll ctl(DEL) or close() */
	filp_epoll_list_lock(fnode);
	if (!dlist_empty(&enode->node.link)) {
		dlist_delete(&enode->node.link);
		(void)raw_refcnt_put(&enode->refcnt);
	}
	filp_epoll_list_unlock(fnode);

	(void)raw_refcnt_put(&enode->refcnt);
}

static int __dh_epoll_ctl_add(struct filp_node *fnode, struct epoll_info_arg *arg)
{
	int ret;
	unsigned int revents = 0;
	struct device_info *devinfo = fnode->devinfo;
	struct devhost_epoll_node *enode = NULL;
	struct devhost_poll_info pinfo = {0};

	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	ret = __dh_do_epoll_ctl(arg, 0);
	if (ret != E_HM_OK) {
		dh_debug("do epoll ctl add failed, device=%s, err=%s\n",
			  devinfo->devname, hmstrerror(ret));
		return ret;
	}

	ret = __dh_insert_epoll_node(fnode, arg, &enode, &pinfo);
	if (ret != E_HM_OK) {
		goto init_node_err;
	}

	ret = devinfo->p->ex_fops->poll(fnode->filp, &pinfo, &revents);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		dh_debug("fops epoll failed, device=%s, err=%s\n",
			  devinfo->devname, hmstrerror(ret));
		goto poll_err;
	}

	(void)syscap_EpollWakeup(enode->cap.rref, enode->node.fd, revents,
				 enode->node.key, enode->cap.tock);

	(void)raw_refcnt_put(&enode->refcnt);
	return E_HM_OK;

poll_err:
	__dh_remove_epoll_node(fnode, enode);
init_node_err:
	arg->ctl_info.op = EPOLL_CTL_DEL;
	(void)__dh_do_epoll_ctl(arg, 0);
	return ret;
}

static int __dh_epoll_ctl_del(struct filp_node *fnode, struct epoll_info_arg *arg)
{
	int ret;
	int find = 0;
	struct epoll_cap_info_s *cap_info = &arg->cap_info;
	struct __epoll_ctl_info_s *ctl_info = &arg->ctl_info;
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *next = NULL;
	struct devhost_epoll_node *enode = NULL;
	struct dlist_node *head = epoll_list_of_filp(fnode);

	filp_epoll_list_lock(fnode);
	dlist_for_each_entry_safe(pos, next, head, struct eventpoll_node_s, link) {
		enode = container_of(pos, struct devhost_epoll_node, node);
		if ((enode->cap.cref == cap_info->cap.cref) && (enode->cap.tock == cap_info->cap.tock)) {
			dlist_delete(&pos->link);
			find = 1;
			break;
		}
	}
	filp_epoll_list_unlock(fnode);

	if (find != 0) {
		(void)raw_refcnt_put(&enode->refcnt);
	} else {
		return E_HM_NOENT;
	}

	ret = syscap_EpollCtl(cap_info->cap.rref, ctl_info, cap_info->cap.tock);
	if (ret != E_HM_OK) {
		dh_debug("do epoll ctl del failed, device=%s, err=%s\n",
			  fnode->devinfo->devname, hmstrerror(ret));
	}

	return ret;
}

static int __dh_epoll_ctl_mod(struct filp_node *fnode, struct epoll_info_arg *arg)
{
	int ret;
	unsigned int revents;
	struct device_info *devinfo = fnode->devinfo;

	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	ret = devinfo->p->ex_fops->poll(fnode->filp, NULL, &revents);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		dh_debug("fops epoll mod failed, device=%s, err=%s\n",
			  devinfo->devname, hmstrerror(ret));
		return ret;
	}

	ret = __dh_do_epoll_ctl(arg, revents);
	if (ret < 0) {
		dh_debug("do epoll ctl mod failed, device=%s, err=%s\n",
			  devinfo->devname, hmstrerror(ret));
	}

	return ret;
}

/* The caller ensures that the input parameters are valid */
int devhost_epoll_ctl(struct filp_node *fnode, struct epoll_info_arg *arg)
{
	int ret;
	struct epoll_cap_info_s *cap_info = &arg->cap_info;

	ret = eventpoll_cap_epoll_grant(cap_info->epfd, cap_info->ap_cnode, &cap_info->cap);
	if (ret != E_HM_OK) {
		return ret;
	}

	switch (arg->ctl_info.op) {
	case EPOLL_CTL_ADD:
		ret = __dh_epoll_ctl_add(fnode, arg);
		break;
	case EPOLL_CTL_DEL:
		ret = __dh_epoll_ctl_del(fnode, arg);
		break;
	case EPOLL_CTL_MOD:
		ret = __dh_epoll_ctl_mod(fnode, arg);
		break;
	default:
		ret = E_HM_INVAL;
		break;
	}

	if (ret < 0) {
		dh_debug("devhost epoll ctl (op=%d) failed, err=%s\n",
			  arg->ctl_info.op, hmstrerror(ret));
	}

	return ret;
}

/* when epfd closed, need remove the epoll from each fd */
void devhost_epoll_remove(struct filp_node *fnode, cref_t ep_cref)
{
	bool find = false;
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *next = NULL;
	struct devhost_epoll_node *enode = NULL;
	struct dlist_node *head = epoll_list_of_filp(fnode);

	filp_epoll_list_lock(fnode);
	dlist_for_each_entry_safe(pos, next, head, struct eventpoll_node_s, link) {
		enode = container_of(pos, struct devhost_epoll_node, node);
		if (enode->cap.cref == ep_cref) {
			dlist_delete(&pos->link);
			find = true;
			break;
		}
	}
	filp_epoll_list_unlock(fnode);

	if (find) {
		(void)raw_refcnt_put(&enode->refcnt);
	}
}

int devhost_epoll_recheck(struct filp_node *fnode, unsigned int *revents)
{
	int ret;
	struct device_info *devinfo = fnode->devinfo;

	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	ret = devinfo->p->ex_fops->poll(fnode->filp, NULL, revents);

	return posix2hmerrno(-ret);
}

/* filp to be destroyed */
PUBLIC_SYMBOL
void devhost_epoll_close2(struct filp_node *fnode)
{
	struct eventpoll_node_s *pos = NULL;
	struct eventpoll_node_s *next = NULL;
	struct devhost_epoll_node *enode = NULL;
	struct dlist_node *head = epoll_list_of_filp(fnode);

	filp_epoll_list_lock(fnode);
	dlist_for_each_entry_safe(pos, next, head, struct eventpoll_node_s, link) {
		dlist_delete(&pos->link);
		enode = container_of(pos, struct devhost_epoll_node, node);
		__dh_do_epoll_ctl_del(enode, 0);
		(void)raw_refcnt_put(&enode->refcnt);
	}
	filp_epoll_list_unlock(fnode);
}
