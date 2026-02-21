/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_SYSFS_BATCH_H__
#define __LIBDH_LINUX_SYSFS_BATCH_H__

#define SYSFS_CREATE_BATCH_MAX 1000
#define SYSFS_PATH_NAME_MAX 256
#define rb_to_kn(X) rb_entry((X), struct kernfs_node, rb)

typedef struct ldk_sysfs_entry {
	char path[SYSFS_PATH_NAME_MAX];
	char linkpath[SYSFS_PATH_NAME_MAX];
	unsigned int mode;
	int type;
	unsigned long long size;
	int id;
	unsigned int nsid;
} ldk_sysfs_entry_t;

bool is_need_create_in_batch(const struct net *ns);
int liblinux_register_netdev(void *data);
int liblinux_kobject_add_uevent(void *data);
#endif /* __LIBDH_LINUX_SYSFS_BATCH_H__ */
