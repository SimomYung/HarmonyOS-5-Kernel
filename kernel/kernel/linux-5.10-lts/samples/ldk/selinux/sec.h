/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_SECURITY_H__
#define __LIBDH_LINUX_SECURITY_H__

struct sel_struct_size {
	unsigned short policydb_size;
	unsigned short hashtab_size;
	unsigned short hashtab_node_size;
	unsigned short symtab_size;
	unsigned short class_datum_size;
	unsigned short common_datum_size;
	unsigned short constraint_node_size;
	unsigned short constraint_expr_size;
	unsigned short type_set_size;
	unsigned short ebitmap_node_size;
	unsigned short ebitmap_size;
	unsigned short role_datum_size;
	unsigned short user_datum_size;
	unsigned short type_datum_size;
	unsigned short cond_bool_datum_size;
	unsigned short avtab_key_size;
	unsigned short avtab_ext_perms_size;
	unsigned short avtab_datum_size;
	unsigned short avtab_node_size;
	unsigned short avtab_size;
	unsigned short role_trans_size;
	unsigned short role_allow_size;
	unsigned short context_size;
	unsigned short ocontext_size;
	unsigned short genfs_size;
	unsigned short mls_level_size;
	unsigned short mls_range_size;
	unsigned short cond_node_size;
	unsigned short cond_expr_size;
	unsigned short cond_av_list_size;
};

int sec_load_policy(const char *buffer, unsigned long size);
int sec_update_enforce(int value);
int sec_current_task_sid(uint32_t *sid);
int sec_avc_perm(unsigned int tsid, unsigned short tclass, unsigned int requested);
int sec_avc_check(unsigned int ssid,
	unsigned int tsid, unsigned short tclass, unsigned int requested);
int socket_sockcreate_sid(unsigned int tsid, unsigned short tclass, unsigned int *socksid);
unsigned short socket_type_to_secclass(int domain, int type, int protocol);
int sec_update_cache_threshold(int value);
bool sec_is_context_match_current_sid(const char *context);

struct policydb;
struct policydb *security_get_sel_policy(void);
void security_init_sel_struct_size(struct sel_struct_size *sel_struct);
#endif /* !__LIBDH_LINUX_SECURITY_H__ */
