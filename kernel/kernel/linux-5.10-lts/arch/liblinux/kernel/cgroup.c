// SPDX-License-Identifier: GPL-2.0-only
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/cgroup.h>
#include <linux/bpf.h>
#include <linux/bpf-cgroup.h>
#include <net/bpf_sk_storage.h>

DEFINE_MUTEX(cgroup_mutex);
static struct cgroup hack_cgrp;

#define cgroup_assert_mutex_or_rcu_locked()                             \
        RCU_LOCKDEP_WARN(!rcu_read_lock_held() &&                       \
                           !lockdep_is_held(&cgroup_mutex),             \
                           "cgroup_mutex or RCU read lock required");

struct css_set init_css_set = {
	.refcount               = REFCOUNT_INIT(1),
	.dom_cset               = &init_css_set,
	.tasks                  = LIST_HEAD_INIT(init_css_set.tasks),
	.mg_tasks               = LIST_HEAD_INIT(init_css_set.mg_tasks),
	.dying_tasks            = LIST_HEAD_INIT(init_css_set.dying_tasks),
	.task_iters             = LIST_HEAD_INIT(init_css_set.task_iters),
	.threaded_csets         = LIST_HEAD_INIT(init_css_set.threaded_csets),
	.cgrp_links             = LIST_HEAD_INIT(init_css_set.cgrp_links),
	.mg_preload_node        = LIST_HEAD_INIT(init_css_set.mg_preload_node),
	.mg_node                = LIST_HEAD_INIT(init_css_set.mg_node),
	.dfl_cgrp               = &hack_cgrp,
};

struct cgroup *cgroup_get_from_fd(int fd)
{
	return &hack_cgrp;
}

int __init cgroup_init(void)
{
	memset(&hack_cgrp, 0, sizeof(hack_cgrp));
	hack_cgrp.kn = kzalloc(sizeof(struct kernfs_node), GFP_KERNEL);
	hack_cgrp.kn->id = 1;   // add for cgroup_id()
	hack_cgrp.self.flags |= CSS_NO_REF;
	INIT_LIST_HEAD(&hack_cgrp.self.sibling);
	INIT_LIST_HEAD(&hack_cgrp.self.children);
	hack_cgrp.self.cgroup = &hack_cgrp;

	return cgroup_bpf_inherit(&hack_cgrp);
}

int __init cgroup_init_early(void)
{
	/* Dummy early init */
	return 0;
}

void cgroup_sk_alloc(struct sock_cgroup_data *skcd)
{
	skcd->cgroup = &hack_cgrp;
	cgroup_bpf_get(&hack_cgrp);
}

void cgroup_sk_clone(struct sock_cgroup_data *skcd)
{
	struct cgroup *cgrp = sock_cgroup_ptr(skcd);

	cgroup_bpf_get(cgrp);
}

void cgroup_sk_free(struct sock_cgroup_data *skcd)
{
	struct cgroup *cgrp = sock_cgroup_ptr(skcd);

	cgroup_bpf_put(cgrp);
}

int cgroup_bpf_attach(struct cgroup *cgrp,
		      struct bpf_prog *prog, struct bpf_prog *replace_prog,
		      struct bpf_cgroup_link *link,
		      enum bpf_attach_type type,
		      u32 flags)
{
	int ret;

	mutex_lock(&cgroup_mutex);
	ret = __cgroup_bpf_attach(cgrp, prog, replace_prog, link, type, flags);
	mutex_unlock(&cgroup_mutex);
	return ret;
}

int cgroup_bpf_detach(struct cgroup *cgrp, struct bpf_prog *prog,
		      enum bpf_attach_type type)
{
	int ret;

	mutex_lock(&cgroup_mutex);
	ret = __cgroup_bpf_detach(cgrp, prog, NULL, type);
	mutex_unlock(&cgroup_mutex);
	return ret;
}

int cgroup_bpf_query(struct cgroup *cgrp, const union bpf_attr *attr,
		     union bpf_attr __user *uattr)
{
	int ret;

	mutex_lock(&cgroup_mutex);
	ret = __cgroup_bpf_query(cgrp, attr, uattr);
	mutex_unlock(&cgroup_mutex);
	return ret;
}

struct cgroup_subsys_state *
css_next_descendant_pre(struct cgroup_subsys_state *pos,
			struct cgroup_subsys_state *root)
{
	struct cgroup_subsys_state *next;

	cgroup_assert_mutex_or_rcu_locked();

	/* if first iteration, visit @root */
	if (!pos)
		return root;

	/* visit the first child if exists */
	next = css_next_child(NULL, pos);
	if (next)
		return next;

	/* no child, visit my or the closest ancestor's next sibling */
	while (pos != root) {
		next = css_next_child(pos, pos->parent);
		if (next)
			return next;
		pos = pos->parent;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(css_next_descendant_pre);
