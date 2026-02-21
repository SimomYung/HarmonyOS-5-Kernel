/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vpipe implementation
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 08 11:20:09 2021
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libvpipe/vpipe.h>
#include <libvpipe/vnotify_impl.h>
#include <hmasm/lsyscall.h>

#ifdef CONFIG_VNOTIFY_DRIVER_EXTENSION

#define VPIPE_NO_RECEIVE 0x01U

struct vpipe_pool_s {
	struct rbt_tree rbt;
	struct raw_mutex mutex;
};

static struct vpipe_pool_s g_vpipe_pool = {
	.rbt = RBT_ROOT,
	.mutex = RAW_MUTEX_INITIALIZER,
};

struct vpipe_s {
	int fd;
	unsigned int flags;
	struct raw_refcnt_nr refcnt;
	struct vnotify_s vnotify;
	struct rbt_node node;
};

static int vpipe_get(struct vpipe_s *vp)
{
	int rc = E_HM_INVAL;

	if (vp != NULL) {
		rc = raw_refcnt_nr_get(&vp->refcnt);
	}

	return rc;
}

static void vpipe_put(struct vpipe_s *vp)
{
	int cnt;
	int ret;

	if (vp != NULL) {
		cnt = raw_refcnt_nr_put(&vp->refcnt);
		if (cnt == 0) {
			ret = lsyscall_vfs_close(vp->fd, 0);
			if (ret != E_HM_OK) {
				hm_debug("close vpipe fd ret=%s\n", hmstrerror(ret));
			}
			ret = hm_vnotify_detach_chn(&vp->vnotify, false, NULL);
			if (ret != E_HM_OK) {
				hm_debug("detach chn ret=%s\n", hmstrerror(ret));
			}
			free(vp);
		}
	}
}

static struct vpipe_pool_s *vpipe_pool(void)
{
	return &g_vpipe_pool;
}

static struct raw_mutex *vpipe_pool_mutex(void)
{
	return &vpipe_pool()->mutex;
}

static struct rbt_tree *vpipe_pool_rbt(void)
{
	return &vpipe_pool()->rbt;
}

static struct vpipe_s *vpipe_entry(const struct rbt_node *p)
{
	return container_of(p, struct vpipe_s, node);
}

void hm_vpipe_lock_parent(void)
{
	raw_mutex_lock(vpipe_pool_mutex());
}

void hm_vpipe_unlock_parent(void)
{
	raw_mutex_unlock(vpipe_pool_mutex());
}

void hm_vpipe_fork_reinit(void)
{
	struct rbt_node *vp_node = NULL;
	struct vpipe_s *vp = NULL;
	struct rbt_tree *tree = NULL;
	int rc;

	tree = vpipe_pool_rbt();
	/* the old vpipe fd cannot be used in child process after fork */
	rbt_for_each_entry_safe(vp, vp_node, tree, struct vpipe_s, node) {
		rbt_remove(tree, &vp->node);
		rc = lsyscall_vfs_close(vp->fd, 0);
		if (rc != 0) {
			hm_debug("close vpipe fd, rc=%s\n", hmstrerror(rc));
		}
		free(vp);
	}
	raw_mutex_unlock(vpipe_pool_mutex());
}

static int do_insert_vpipe_rbt(struct rbt_tree *tree,
			       struct vpipe_s *vp)
{
	int err = E_HM_OK;
	bool is_rchild = false;
	int fd = vp->fd;
	struct rbt_node *p = tree->root;
	struct rbt_node *parent = NULL;
	struct vpipe_s *curr = NULL;

	while (p != NULL) {
		parent = p;
		curr = vpipe_entry(p);
		if (fd > curr->fd) {
			p = p->rbt_right;
			if (p == NULL) {
				is_rchild = true;
			}
		} else if (fd < curr->fd) {
			p = p->rbt_left;
			if (p == NULL) {
				is_rchild = false;
			}
		} else {
			err = E_HM_OBJEXIST;
			break;
		}
	}

	if (err == E_HM_OK) {
		rbt_insert(tree, &vp->node, parent, is_rchild);
	}
	return err;
}

static int vpipe_pool_insert_vpipe(struct vpipe_s *vp)
{
	RAW_MUTEX_GUARD(_, vpipe_pool_mutex());
	return do_insert_vpipe_rbt(vpipe_pool_rbt(), vp);
}

static struct vpipe_s *search_vpipe_rbt(struct rbt_tree *tree, int fd)
{
	struct rbt_node *p = tree->root;
	struct vpipe_s *curr = NULL;
	struct vpipe_s *vp = NULL;

	while (p != NULL) {
		curr = vpipe_entry(p);
		if (fd > curr->fd) {
			p = p->rbt_right;
		} else if (fd < curr->fd) {
			p = p->rbt_left;
		} else {
			vp = curr;
			break;
		}
	}
	return vp;
}

static struct vpipe_s *vpipe_pool_get_vpipe(int fd)
{
	struct vpipe_s *vp;
	int rc;

	RAW_MUTEX_GUARD(_, vpipe_pool_mutex());
	vp = search_vpipe_rbt(vpipe_pool_rbt(), fd);
	if (vp != NULL) {
		rc = vpipe_get(vp);
		if (rc <= 0) {
			vp = NULL;
		}
	}
	return vp;
}

static struct vpipe_s *vpipe_pool_delete_vpipe(int fd)
{
	struct vpipe_s *vp;

	RAW_MUTEX_GUARD(_, vpipe_pool_mutex());
	vp = search_vpipe_rbt(vpipe_pool_rbt(), fd);
	if (vp != NULL) {
		rbt_remove(vpipe_pool_rbt(), &vp->node);
	}
	return vp;
}

static void init_vpipe_fd(struct vpipe_s *vp, int fd)
{
	raw_refcnt_nr_init(&vp->refcnt, 1);
	vp->fd = fd;
	vp->flags = 0U;
}

int hm_vpipe_attach_chn(const char *name, unsigned int flags)
{
	int err = E_HM_OK;
	int fd = -1;
	struct vpipe_s *vp = NULL;
	vp = (struct vpipe_s *)malloc(sizeof(struct vpipe_s));
	if (vp == NULL) {
		err = E_HM_NOMEM;
	} else {
		mem_zero_b(vp, sizeof(struct vpipe_s));
	}

	if (err == E_HM_OK) {
		err = hm_vnotify_attach_chn(&vp->vnotify, name, flags);
		if (err != E_HM_OK) {
			free(vp);
		}
	}

	if (err == E_HM_OK) {
		fd = hm_vnotify_attach_fd(&vp->vnotify);
		if (fd < 0) {
			(void)hm_vnotify_detach_chn(&vp->vnotify, false, NULL);
			free(vp);
			err = fd;
		}
	}

	if (err == E_HM_OK) {
		init_vpipe_fd(vp, fd);
		err = vpipe_pool_insert_vpipe(vp);
		if (err != E_HM_OK) {
			(void)lsyscall_vfs_close(fd, 0);
			(void)hm_vnotify_detach_chn(&vp->vnotify, false, NULL);
			free(vp);
		}
	}

	if (err == E_HM_OK) {
		err = fd;
	}
	return err;
}

int hm_vpipe_detach_chn(int fd)
{
	int err = E_HM_OK;
	struct vpipe_s *vp = vpipe_pool_delete_vpipe(fd);
	if (vp == NULL) {
		err = E_HM_INVAL;
	} else {
		/* put for init_vpipe_fd */
		vpipe_put(vp);
	}

	return err;
}

int hm_vpipe_chnid_of(int vpfd)
{
	int err = E_HM_INVAL;
	struct vpipe_s *vp = vpipe_pool_get_vpipe(vpfd);
	if (vp != NULL) {
		err = hm_vnotify_chnid_of(&vp->vnotify);
		vpipe_put(vp);
	}
	return err;
}

int hm_vpipe_notify(int vpfd)
{
	int err = E_HM_INVAL;
	struct vpipe_s *vp = vpipe_pool_get_vpipe(vpfd);

	if (vp != NULL) {
		err = hm_vnotify_notify(&vp->vnotify);
		vpipe_put(vp);
	}
	return err;
}

int hm_vpipe_fast_notify(int vpfd)
{
	int err = E_HM_INVAL;
	struct vpipe_s *vp = vpipe_pool_get_vpipe(vpfd);
	if (vp != NULL) {
		err = hm_vnotify_fast_notify(&vp->vnotify);
		vpipe_put(vp);
	}
	return err;
}

static int do_hm_vpipe_receive(int vpfd, const struct timespec *ts, unsigned int flags, bool fast)
{
	int err = E_HM_OK;
	struct vpipe_s *vp = vpipe_pool_get_vpipe(vpfd);
	if (vp == NULL) {
		err = E_HM_INVAL;
	}

	if (vp != NULL) {
		/* hm_vpipe_receive can't be used together with epoll. */
		if ((vp->flags & VPIPE_NO_RECEIVE) != 0U) {
			err = E_HM_BUSY;
		}

		if (err == E_HM_OK) {
			if (fast == true) {
				err = hm_vnotify_fast_receive(&vp->vnotify, ts, flags);
			} else {
				err = hm_vnotify_receive(&vp->vnotify, ts, flags);
			}
		}

		vpipe_put(vp);
	}
	return err;
}

int hm_vpipe_receive(int vpfd, const struct timespec *ts, unsigned int flags)
{
	return do_hm_vpipe_receive(vpfd, ts, flags, false);
}

int hm_vpipe_fast_receive(int vpfd, const struct timespec *ts, unsigned int flags)
{
	return do_hm_vpipe_receive(vpfd, ts, flags, true);
}

int hm_vpipe_update_futex(int vpfd, int *futex, bool shared)
{
	int err = E_HM_OK;
	struct vpipe_s *vp = vpipe_pool_get_vpipe(vpfd);

	if (vp == NULL) {
		err = E_HM_INVAL;
	} else {
		/* Not allow to update the vpipe that has been added to epoll. */
		if ((vp->flags & VPIPE_NO_RECEIVE) != 0U) {
			err = E_HM_BUSY;
		} else {
			err = hm_vnotify_update_futex(&vp->vnotify, futex, shared);
		}

		vpipe_put(vp);
	}
	return err;
}

int hm_vpipe_inject_event(int vpfd, unsigned int event)
{
	int err = E_HM_INVAL;
	struct vpipe_s *vp = vpipe_pool_get_vpipe(vpfd);

	if (vp != NULL) {
		err = hm_vnotify_inject_event(&vp->vnotify, event);
		vpipe_put(vp);
	}

	return err;
}

void post_epoll_ctl_handler(int fd, int op, int fd2, const struct epoll_event *ev)
{
	struct vpipe_s *vp = vpipe_pool_get_vpipe(fd2);
	int ret;
	UNUSED(fd, op, ev);

	if (vp != NULL) {
		if (op == EPOLL_CTL_ADD) {
			vp->flags |= VPIPE_NO_RECEIVE;
		} else if (op == EPOLL_CTL_DEL) {
			vp->flags &= ~VPIPE_NO_RECEIVE;
			/*
			 * For vpipe fd, microkernel will clear listener after epoll ctl del.
			 * we need to update the next listener address to prevent message loss.
			 */
			ret = hm_vnotify_update_futex(&vp->vnotify, &vp->vnotify.futex, false);
			if (ret != E_HM_OK) {
				hm_debug("vpipe update futex ret=%s\n", hmstrerror(ret));
			}
		} else {
			/* do nothing */
		}
		vpipe_put(vp);
	}
}

#else    /* #ifdef CONFIG_VNOTIFY_DRIVER_EXTENSION */

void hm_vpipe_fork_reinit(void)
{
	/* do-nothing */
}

void hm_vpipe_lock_parent(void)
{
	/* do-nothing */
}

void hm_vpipe_unlock_parent(void)
{
	/* do-nothing */
}

int hm_vpipe_notify(int vpfd)
{
	UNUSED(vpfd);
	return E_HM_NOSYS;
}

int hm_vpipe_fast_notify(int vpfd)
{
	UNUSED(vpfd);
	return E_HM_NOSYS;
}

int hm_vpipe_receive(int vpfd, const struct timespec *ts,
		     unsigned int flags)
{
	UNUSED(vpfd, ts, flags);
	return E_HM_NOSYS;
}

int hm_vpipe_fast_receive(int vpfd, const struct timespec *ts, unsigned int flags)
{
	UNUSED(vpfd, ts, flags);
	return E_HM_NOSYS;
}

int hm_vpipe_inject_event(int vpfd, unsigned int event)
{
	UNUSED(vpfd, event);
	return E_HM_NOSYS;
}

int hm_vpipe_chnid_of(int vpfd)
{
	UNUSED(vpfd);
	return E_HM_NOSYS;
}

int hm_vpipe_attach_chn(const char *name, unsigned int flags)
{
	UNUSED(name, flags);
	return E_HM_NOSYS;
}

int hm_vpipe_detach_chn(int vpfd)
{
	UNUSED(vpfd);
	return E_HM_NOSYS;
}

int hm_vpipe_update_futex(int vpfd, int *futex, bool shared)
{
	UNUSED(vpfd, futex, shared);
	return E_HM_NOSYS;
}

void post_epoll_ctl_handler(int fd, int op, int fd2, const struct epoll_event *ev)
{
	UNUSED(fd, op, fd2, ev);
}

#endif   /* #ifdef CONFIG_VNOTIFY_DRIVER_EXTENSION */
