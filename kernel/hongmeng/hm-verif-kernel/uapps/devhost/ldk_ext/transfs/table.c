/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ops for transfs table operation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 29 10:42:14 2021
 */

#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <devhost/plugin.h>
#include <devhost/transfs.h>

#include "table.h"

static struct raw_mutex g_trans_sb_info_lock = RAW_MUTEX_INITIALIZER;
static struct trans_sb_info g_trans_sb_info = __SB_INFO_INITIALIZER(g_trans_sb_info);
typedef struct table_ctx *(*get_table_func_t)(unsigned int sb_index);

static int transfs_expand_meta_table(void ***table, unsigned int *table_size,
				     size_t element_size)
{
	void **new_table = NULL;
	unsigned int new_size;

	if (*table == NULL) {
		if (element_size == 0) {
			return E_HM_INVAL;
		}
		*table = calloc(TABLE_EXPAND_SIZE, element_size);
		if (*table == NULL) {
			return E_HM_NOMEM;
		}
		*table_size = TABLE_EXPAND_SIZE;
		return E_HM_OK;
	}

	new_size = *table_size + TABLE_EXPAND_SIZE;
	if (new_size == 0 || element_size == 0) {
		return E_HM_INVAL;
	}
	new_table = (void **)calloc(new_size, element_size);
	if (new_table == NULL) {
		return E_HM_NOMEM;
	}

	NOFAIL(memcpy_s(new_table, new_size * element_size, *table, *table_size * element_size));
	free(*table);
	*table = new_table;
	*table_size = new_size;

	return E_HM_OK;
}

static int transfs_expand_sb_table(struct trans_sb_tables **sb_table,
				   unsigned int *table_size, size_t element_size)
{
	struct trans_sb_tables *new_table = NULL;
	unsigned int new_size;

	if (*sb_table != NULL) {
		new_size = *table_size + TABLE_EXPAND_SIZE;
		if (new_size == 0 || element_size == 0) {
			return E_HM_INVAL;
		}

		new_table = (struct trans_sb_tables *)calloc(new_size, element_size);
		if (new_table == NULL) {
			return E_HM_NOMEM;
		}

		NOFAIL(memcpy_s(new_table, new_size * element_size, *sb_table,
				*table_size * element_size));
		free(*sb_table);
		*sb_table = new_table;
		*table_size = new_size;

		return E_HM_OK;
	}

	if (element_size == 0) {
		return E_HM_INVAL;
	}
	*sb_table = calloc(TABLE_EXPAND_SIZE, element_size);
	if (*sb_table == NULL) {
		return E_HM_NOMEM;
	}
	*table_size = TABLE_EXPAND_SIZE;

	return E_HM_OK;
}

static struct table_ctx *trans_table_init(void)
{
	struct table_ctx *table = NULL;

	table = calloc(1, sizeof(struct table_ctx));
	if (table == NULL) {
		return NULL;
	}
	raw_rwlock_init(&table->table_lock, NULL);
	table->table_size = TABLE_EXPAND_SIZE;
	table->table_used_size = 0;
	table->datas = calloc(TABLE_EXPAND_SIZE, sizeof(void *));
	if (table->datas == NULL) {
		free(table);
		return NULL;
	}
	NOFAIL(memset_s(table->datas, sizeof(void *) * TABLE_EXPAND_SIZE, 0, sizeof(void *) * TABLE_EXPAND_SIZE));

	return table;
}

static void trans_table_free(struct table_ctx **table)
{
	if (table == NULL || *table == NULL) {
		return;
	}

	free((*table)->datas);
	(*table)->datas = NULL;
	free(*table);
	*table = NULL;
}

static int do_alloc_sb_to_table(unsigned int _i)
{
	int sb_index = -1;
	unsigned int i = _i;
	struct trans_sb_tables *item = NULL;

	for (; i < g_trans_sb_info.sb_tables_size; i++) {
		item = &g_trans_sb_info.sb_tables[i];
		if (item->sb == NULL) {
			sb_index = (int)i;
			item->inode_table = trans_table_init();
			if (item->inode_table == NULL) {
				return E_HM_NOMEM;
			}
			item->dentry_table = trans_table_init();
			if (item->dentry_table == NULL) {
				trans_table_free(&item->inode_table);
				return E_HM_NOMEM;
			}
			item->file_table = trans_table_init();
			if (item->file_table == NULL) {
				trans_table_free(&item->dentry_table);
				trans_table_free(&item->inode_table);
				return E_HM_NOMEM;
			}
			break;
		}
	}
	if (sb_index >= 0) {
		item->sb = DATA_HOLDER;
		item->vmnt = DATA_HOLDER;
		g_trans_sb_info.sb_tables_used_size++;
	}

	return sb_index;
}

int alloc_sb_to_table(void)
{
	int ret;
	unsigned int i = 0;
	int sb_index = -1;

	if (g_trans_sb_info.sb_tables_size == 0) {
		hm_error("sb info table has not been inited!\n");
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables_lock);
	if (g_trans_sb_info.sb_tables_used_size == g_trans_sb_info.sb_tables_size) {
		ret = transfs_expand_sb_table(&g_trans_sb_info.sb_tables,
					      &g_trans_sb_info.sb_tables_size,
					      sizeof(struct trans_sb_tables));
		if (ret != E_HM_OK) {
			(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
			return ret;
		}
		i = g_trans_sb_info.sb_tables_used_size;
	}
	if (g_trans_sb_info.sb_tables_used_size >= g_trans_sb_info.sb_tables_size) {
		hm_error("sb_tables_used_size[%u] is larger than sb_tables_size[%u]\n",
			 g_trans_sb_info.sb_tables_used_size, g_trans_sb_info.sb_tables_size);
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return E_HM_INVAL;
	}

	sb_index = do_alloc_sb_to_table(i);
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return sb_index;
}

PUBLIC_SYMBOL
void transfs_iterate_supers(void (*f)(struct super_block *sb, void *data))
{
	unsigned int i = 0;
	struct super_block *sb = NULL;
	unsigned int sb_nr;

	if (g_trans_sb_info.sb_tables_size == 0) {
		return;
	}

	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables_lock);
	sb_nr = g_trans_sb_info.sb_tables_used_size;
	for (; i < g_trans_sb_info.sb_tables_size && sb_nr != 0; i++) {
		if (g_trans_sb_info.sb_tables[i].sb != NULL) {
			sb = g_trans_sb_info.sb_tables[i].sb;
			sb_nr--;
			if (sb == DATA_HOLDER) {
				continue;
			}
			f(sb, g_trans_sb_info.sb_tables[i].data);
		}
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
}

static int get_available_table_index(struct table_ctx *table)
{
	int ret;
	int index = -1;
	unsigned int i = 0;

	/* table lock must hold */
	if (table->table_used_size == table->table_size) {
		ret = transfs_expand_meta_table(&table->datas, &table->table_size,
						sizeof(void *));
		if (ret != E_HM_OK) {
			hm_error("expand metadata table failed\n");
			return ret;
		}
		i = table->table_used_size;
	}
	if (table->table_used_size >= table->table_size) {
		hm_error("table_used_size[%u] is larger than table_size[%u]\n",
			 table->table_used_size, table->table_size);
		return E_HM_INVAL;
	}
	for (; i < table->table_size; i++) {
		if (table->datas[i] == NULL) {
			index = (int)i;
			break;
		}
	}

	return index;
}

int trans_del_sb_by_index(unsigned int sb_index, const struct super_block *sb,
			  const struct vfsmount *vmnt,
			  const struct devhost_transfs_ops *ops)
{
	if (g_trans_sb_info.sb_tables_size == 0) {
		hm_error("sb info table has not been inited!\n");
		return E_HM_INVAL;
	}

	if (sb == NULL || vmnt == NULL || ops == NULL) {
		hm_error("input sb or vmnt is NULL\n");
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables_lock);
	if (sb_index >= g_trans_sb_info.sb_tables_size) {
		hm_error("sb_index[%u] is larger than sb_tables_size[%u]\n",
			 sb_index, g_trans_sb_info.sb_tables_size);
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return E_HM_INVAL;
	}
	if (g_trans_sb_info.sb_tables[sb_index].sb != sb ||
	    g_trans_sb_info.sb_tables[sb_index].vmnt != vmnt ||
	    g_trans_sb_info.sb_tables[sb_index].ops != ops) {
		hm_error("input sb_index[%u] has not been mounted\n", sb_index);
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return E_HM_INVAL;
	}
	if (g_trans_sb_info.sb_tables_used_size == 0) {
		hm_error("used_size is 0, sb table error?\n");
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return E_HM_POSIX_FAULT;
	}
	g_trans_sb_info.sb_tables[sb_index].sb = NULL;
	g_trans_sb_info.sb_tables[sb_index].vmnt = NULL;
	g_trans_sb_info.sb_tables[sb_index].ops = NULL;
	g_trans_sb_info.sb_tables[sb_index].data = NULL;

	g_trans_sb_info.sb_tables_used_size--;
	trans_table_free(&g_trans_sb_info.sb_tables[sb_index].inode_table);
	trans_table_free(&g_trans_sb_info.sb_tables[sb_index].dentry_table);
	trans_table_free(&g_trans_sb_info.sb_tables[sb_index].file_table);

	if (g_trans_sb_info.sb_tables_used_size == 0) {
		/* release sb table */
		free(g_trans_sb_info.sb_tables);
		g_trans_sb_info.sb_tables = NULL;
		g_trans_sb_info.sb_tables_size = 0;
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return E_HM_OK;
}

int transfs_sb_table_init(void)
{
	int ret = 0;

	raw_mutex_lock(&g_trans_sb_info_lock);
	if (g_trans_sb_info.sb_tables_size == 0) {
		ret = transfs_expand_sb_table(&g_trans_sb_info.sb_tables,
					      &g_trans_sb_info.sb_tables_size,
					      sizeof(struct trans_sb_tables));
	}
	raw_mutex_unlock(&g_trans_sb_info_lock);

	return ret;
}

static int check_sb_index(unsigned int sb_index)
{
	if (sb_index >= g_trans_sb_info.sb_tables_size) {
		hm_error("sb_index[%u] is larger than sb_tables_size[%u]\n",
			 sb_index, g_trans_sb_info.sb_tables_size);
		return E_HM_INVAL;
	}

	if (g_trans_sb_info.sb_tables[sb_index].sb == NULL) {
		hm_error("find sb is NULL by input sb_index[%u]\n", sb_index);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static struct table_ctx *get_table_common(unsigned int sb_index, const get_table_func_t get_table)
{
	struct table_ctx *table = NULL;
	int ret;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return NULL;
	}

	table = get_table(sb_index);
	if (table == NULL || table->datas == NULL) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		hm_error("find datas is NULL by input sb_index[%u]\n", sb_index);
		return NULL;
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return table;
}

static int alloc_data_to_table(unsigned int sb_index, const get_table_func_t get_table)
{
	struct table_ctx *table = NULL;
	int trans_data_index;

	table = get_table_common(sb_index, get_table);
	if (table == NULL) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&table->table_lock);
	trans_data_index = get_available_table_index(table);
	if (trans_data_index < 0) {
		(void)raw_rwlock_unlock(&table->table_lock);
		return trans_data_index;
	} else if (table->datas[trans_data_index] != NULL) {
		(void)raw_rwlock_unlock(&table->table_lock);
		hm_error("find trans_data_index[%d] data is not NULL\n", trans_data_index);
		return E_HM_POSIX_FAULT;
	}
	table->datas[trans_data_index] = DATA_HOLDER;
	table->table_used_size++;
	(void)raw_rwlock_unlock(&table->table_lock);

	return trans_data_index;
}

static int del_data_by_index(unsigned int sb_index, unsigned int trans_data_index,
			     const get_table_func_t get_table)
{
	struct table_ctx *table = NULL;

	table = get_table_common(sb_index, get_table);
	if (table == NULL) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&table->table_lock);
	if (trans_data_index >= table->table_size) {
		hm_error("invalid input trans_data_index[%u], table_size[%u]\n",
			 trans_data_index, table->table_size);
		(void)raw_rwlock_unlock(&table->table_lock);
		return E_HM_INVAL;
	}
	if (table->datas[trans_data_index] == NULL) {
		hm_debug("data with index[%u] has deleted\n", trans_data_index);
		(void)raw_rwlock_unlock(&table->table_lock);
		return E_HM_OK;
	}
	table->datas[trans_data_index] = DATA_HOLDER;
	(void)raw_rwlock_unlock(&table->table_lock);

	return E_HM_OK;
}

static void *get_data_by_index(unsigned int sb_index, unsigned int trans_data_index,
			       const get_table_func_t get_table)
{
	struct table_ctx *table = NULL;
	void *data = NULL;

	table = get_table_common(sb_index, get_table);
	if (table == NULL) {
		return NULL;
	}

	(void)raw_rwlock_rdlock(&table->table_lock);
	if (trans_data_index >= table->table_size) {
		hm_error("trans_data_index[%u] is larger than table_size[%u]\n",
			 trans_data_index, table->table_size);
		(void)raw_rwlock_unlock(&table->table_lock);
		return NULL;
	}

	data = (void *)table->datas[trans_data_index];
	(void)raw_rwlock_unlock(&table->table_lock);

	return data;
}

struct vfsmount *get_vmnt_by_index(unsigned int sb_index)
{
	struct vfsmount *vmnt = NULL;
	int ret;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return NULL;
	}
	if (g_trans_sb_info.sb_tables[sb_index].vmnt == NULL) {
		hm_error("find vmnt is NULL by input sb_index[%u]\n", sb_index);
	} else {
		vmnt = g_trans_sb_info.sb_tables[sb_index].vmnt;
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return vmnt;
}

struct super_block *get_sb_by_index(unsigned int sb_index)
{
	struct super_block *sb = NULL;
	int ret;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return NULL;
	}
	if (g_trans_sb_info.sb_tables[sb_index].sb == NULL) {
		hm_error("find sb is NULL by input sb_index[%u]\n", sb_index);
	} else {
		sb = g_trans_sb_info.sb_tables[sb_index].sb;
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return sb;
}

void *get_data_by_sb_index(unsigned int sb_index)
{
	void *data = NULL;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	data = g_trans_sb_info.sb_tables[sb_index].data;
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return data;
}

struct devhost_transfs_ops *get_ops_by_index(unsigned int sb_index)
{
	struct devhost_transfs_ops *ops = NULL;
	int ret;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return NULL;
	}
	if (g_trans_sb_info.sb_tables[sb_index].ops == NULL) {
		hm_error("find ops is NULL by input sb_index[%u]\n", sb_index);
	} else {
		ops = g_trans_sb_info.sb_tables[sb_index].ops;
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return ops;
}

size_t get_per_write_size_by_index(unsigned int sb_index)
{
	size_t per_write_size = 0;
	int ret;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return 0;
	}
	per_write_size = g_trans_sb_info.sb_tables[sb_index].per_write_size;
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return per_write_size;
}

int get_sb_index_by_sb(struct super_block *sb, unsigned int *sb_index)
{
	int ret = E_HM_OPNOTSUPP;
	unsigned int i;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	for (i = 0; i < g_trans_sb_info.sb_tables_size; i++) {
		if (g_trans_sb_info.sb_tables[i].sb == sb) {
			if (g_trans_sb_info.sb_tables[i].s_reverse_query) {
				*sb_index = i;
				ret = E_HM_OK;
				break;
			}
		}
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return ret;
}

int get_d_index_by_dentry(unsigned int sb_index, struct dentry *dentry, unsigned int *d_index)
{
	int ret;
	unsigned int i;

	(void)raw_rwlock_rdlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return ret;
	}
	ret = E_HM_OPNOTSUPP;
	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables[sb_index].dentry_table->table_lock);
	for (i = 0; i < g_trans_sb_info.sb_tables[sb_index].dentry_table->table_size; i++) {
		if (g_trans_sb_info.sb_tables[sb_index].dentry_table->datas[i] == dentry) {
			*d_index = i;
			ret = E_HM_OK;
			break;
		}
	}
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables[sb_index].dentry_table->table_lock);
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);

	return ret;
}

void set_sb_to_table(unsigned int sb_index, const struct transfs_mounted_ret *mount_ret,
		     size_t per_write_size, bool s_reverse_query)
{
	if (mount_ret == NULL || mount_ret->sb == NULL ||
	    mount_ret->vmnt == NULL || mount_ret->ops == NULL) {
		hm_error("invalid parameter\n");
		return;
	}

	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables_lock);
	g_trans_sb_info.sb_tables[sb_index].sb = mount_ret->sb;
	g_trans_sb_info.sb_tables[sb_index].vmnt = mount_ret->vmnt;
	g_trans_sb_info.sb_tables[sb_index].ops = mount_ret->ops;
	g_trans_sb_info.sb_tables[sb_index].data = mount_ret->data;
	g_trans_sb_info.sb_tables[sb_index].per_write_size = per_write_size;
	g_trans_sb_info.sb_tables[sb_index].s_reverse_query = s_reverse_query;
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
}

void release_sb_to_table(unsigned int sb_index)
{
	int ret;

	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables_lock);
	ret = check_sb_index(sb_index);
	if (ret != E_HM_OK) {
		(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
		return;
	}
	g_trans_sb_info.sb_tables[sb_index].sb = NULL;
	g_trans_sb_info.sb_tables[sb_index].vmnt = NULL;
	g_trans_sb_info.sb_tables[sb_index].ops = NULL;
	g_trans_sb_info.sb_tables_used_size--;
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables_lock);
}

static void release_data_to_table(unsigned int sb_index, unsigned int trans_data_index,
				  const get_table_func_t get_table)
{
	struct table_ctx *table = NULL;

	table = get_table_common(sb_index, get_table);
	if (table == NULL) {
		return;
	}

	(void)raw_rwlock_wrlock(&table->table_lock);
	if (trans_data_index >= table->table_size) {
		hm_error("trans_data_index[%u] is larger than table_size[%u]\n",
			 trans_data_index, table->table_size);
		(void)raw_rwlock_unlock(&table->table_lock);
		return;
	}

	if (table->datas[trans_data_index] == NULL) {
		hm_error("data index[%u] has been deleted\n", trans_data_index);
		(void)raw_rwlock_unlock(&table->table_lock);
		return;
	}

	table->datas[trans_data_index] = NULL;

	BUG_ON(table->table_used_size == 0);
	table->table_used_size--;
	(void)raw_rwlock_unlock(&table->table_lock);
}

#define define_table_str_helpers(name)							\
static struct table_ctx * get_##name##_table(unsigned int sb_index)			\
{											\
	return g_trans_sb_info.sb_tables[sb_index].name##_table;			\
}											\
											\
int alloc_##name##_to_table(unsigned int sb_index)					\
{											\
	return alloc_data_to_table(sb_index, get_##name##_table);			\
}											\
											\
int del_##name##_by_index(unsigned int sb_index, unsigned int table_index)		\
{											\
	return del_data_by_index(sb_index, table_index, get_##name##_table);		\
}											\
											\
struct name *get_##name##_by_index(unsigned int sb_index, unsigned int table_index)	\
{											\
	return (struct name *)get_data_by_index(sb_index, table_index,			\
						get_##name##_table);			\
}											\
											\
void release_##name##_to_table(unsigned int sb_index, unsigned int table_index)		\
{											\
	release_data_to_table(sb_index, table_index, get_##name##_table);		\
}											\
											\
void set_##name##_to_table(unsigned int sb_index, unsigned int trans_index,		\
			   struct name *item)						\
{											\
	if (item == NULL) {								\
		hm_error(#name" should not be NULL\n");					\
		return;									\
	}										\
											\
	(void)raw_rwlock_wrlock(&g_trans_sb_info.sb_tables[sb_index].			\
				name##_table->table_lock);				\
	g_trans_sb_info.sb_tables[sb_index].name##_table->datas[trans_index] = item;	\
	(void)raw_rwlock_unlock(&g_trans_sb_info.sb_tables[sb_index].			\
				name##_table->table_lock);				\
}

define_table_str_helpers(inode)
define_table_str_helpers(dentry)
define_table_str_helpers(file)

struct dentry *get_dentry_by_index_ref(unsigned int sb_index, unsigned int trans_dentry_index,
				       transfs_dget_t transfs_dget)
{
	struct table_ctx *table = NULL;
	struct dentry *dentry = NULL;

	table = get_table_common(sb_index, get_dentry_table);
	if (table == NULL) {
		return NULL;
	}

	(void)raw_rwlock_rdlock(&table->table_lock);
	if (trans_dentry_index >= table->table_size) {
		hm_error("trans_dentry_index[%u] is larger than table_size[%u]\n",
			 trans_dentry_index, table->table_size);
		(void)raw_rwlock_unlock(&table->table_lock);
		return NULL;
	}

	dentry = table->datas[trans_dentry_index];
	if (dentry == DATA_HOLDER) {
		hm_error("trans_dentry_index[%u] is invalid\n", trans_dentry_index);
		dentry = NULL;
	}
	if (transfs_dget != NULL && dentry != NULL) {
		transfs_dget(dentry);
	}
	(void)raw_rwlock_unlock(&table->table_lock);

	return dentry;
}
