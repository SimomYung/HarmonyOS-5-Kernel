/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Common implement of access control list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 21 00:30:36 2021
 */
#include <stdlib.h>
#include <stdint.h>
#include <vfs.h>
#include <libsec/dac.h>
#include <libsec/cred.h>
#include <libsec/posix_acl.h>
#include <libhwsecurec/securec.h>
#include <libhmlog/hmlog.h>

struct sec_posix_acl *sec_posix_acl_alloc(size_t entry_num)
{
	size_t entry_size = entry_num * sizeof(struct sec_posix_acl_entry);
	size_t size = sizeof(struct sec_posix_acl) + entry_size;
	struct sec_posix_acl *acl = malloc(size);
	if (acl != NULL) {
		acl->entry_num = (uint32_t)entry_num;
		raw_refcnt_nr_init(&acl->refcnt, 1);
		/* entries are uninitialied */
	}
	return acl;
}

static inline size_t xattr_entry_num(size_t size)
{
	return (size - sizeof(struct hm_posix_acl_xattr_header)) /
		sizeof(struct hm_posix_acl_xattr_entry);
}

static int check_xattr_header(const char *xattr, size_t size)
{
	int ret = E_HM_OK;
	size_t entry_num;
	const struct hm_posix_acl_xattr_header *hdr = NULL;

	if (size < sizeof(struct hm_posix_acl_xattr_header)) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		entry_num = xattr_entry_num(size);
		if (entry_num * sizeof(struct hm_posix_acl_xattr_entry) +
		    sizeof(struct hm_posix_acl_xattr_header) != size) {
			ret = E_HM_INVAL;
		}
	}
	if (ret == E_HM_OK) {
		hdr = ptr_to_type(xattr, const struct hm_posix_acl_xattr_header *);
		if (le32toh(hdr->version) != HM_POSIX_ACL_VERSION) {
			ret = E_HM_INVAL;
		}
	}

	return ret;
}

static inline bool acl_entry_eq(const struct sec_posix_acl_entry *entry,
				size_t *i, size_t num, unsigned tag)
{
	bool res = false;
	if (*i < num && entry[*i].tag == tag) {
		(*i)++;
		res = true;
	}
	return res;
}

static int check_acl_order(const struct sec_posix_acl *acl)
{
	size_t i = 0;
	size_t num = acl->entry_num;
	const struct sec_posix_acl_entry *entry = &acl->entries[0];
	bool need_mask = false;
	bool valid = true;

	valid = acl_entry_eq(entry, &i, num, HM_POSIX_ACL_USER_OBJ);
	while (acl_entry_eq(entry, &i, num, HM_POSIX_ACL_USER)) {
		need_mask = true;
	}

	valid = valid ? acl_entry_eq(entry, &i, num, HM_POSIX_ACL_GROUP_OBJ) : valid;
	while (acl_entry_eq(entry, &i, num, HM_POSIX_ACL_GROUP)) {
		need_mask = true;
	}

	if (!acl_entry_eq(entry, &i, num, HM_POSIX_ACL_MASK) && need_mask) {
		valid = false;
	}
	valid = valid ? acl_entry_eq(entry, &i, num, HM_POSIX_ACL_OTHER) : valid;

	return valid ? E_HM_OK : E_HM_INVAL;
}

static int acl_entry_from_xattr(struct sec_posix_acl *acl, const char *xattr, size_t size)
{
	size_t i;
	size_t entry_num = xattr_entry_num(size);
	const char *data =xattr + sizeof(struct hm_posix_acl_xattr_header);
	const struct hm_posix_acl_xattr_entry *entry =
		ptr_to_type(data, const struct hm_posix_acl_xattr_entry *);
	int ret = acl->entry_num == entry_num ? E_HM_OK : E_HM_INVAL;

	for (i = 0; i < entry_num && ret == E_HM_OK; i++) {
		acl->entries[i].tag = le16toh(entry[i].tag);
		switch (acl->entries[i].tag) {
		case HM_POSIX_ACL_USER:
			acl->entries[i].qualifier.uid = le32toh(entry[i].qualifier);
			if (acl->entries[i].qualifier.uid == SEC_INVALID_UID) {
				ret = E_HM_INVAL;
			}
			break;
		case HM_POSIX_ACL_GROUP:
			acl->entries[i].qualifier.gid = le32toh(entry[i].qualifier);
			if (acl->entries[i].qualifier.gid == SEC_INVALID_GID) {
				ret = E_HM_INVAL;
			}
			break;
		case HM_POSIX_ACL_USER_OBJ:
		case HM_POSIX_ACL_GROUP_OBJ:
		case HM_POSIX_ACL_MASK:
		case HM_POSIX_ACL_OTHER:
			acl->entries[i].qualifier.padding = 0U;
			break;
		default:
			ret = E_HM_INVAL;
			break;
		}
		acl->entries[i].permission = le16toh(entry[i].permission);
		if ((acl->entries[i].permission &
		     ~(HM_POSIX_ACL_READ | HM_POSIX_ACL_WRITE | HM_POSIX_ACL_EXEC)) != 0U) {
		    ret = E_HM_INVAL;
		}
		hm_debug("acl entry t:%u p:%o id:%u\n", (unsigned)acl->entries[i].tag,
			 (unsigned)acl->entries[i].permission, acl->entries[i].qualifier.padding);
	}

	if (ret == E_HM_OK) {
		ret = check_acl_order(acl);
	}

	return ret;
}

int sec_posix_acl_from_xattr(const char *xattr, size_t size, struct sec_posix_acl **acl)
{
	int ret = (acl == NULL || xattr == NULL) ? E_HM_INVAL : E_HM_OK;
	struct sec_posix_acl *tmp = NULL;

	if (ret == E_HM_OK) {
		ret = check_xattr_header(xattr, size);
	}

	if (ret == E_HM_OK) {
		tmp = sec_posix_acl_alloc(xattr_entry_num(size));
		if (tmp == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		ret = acl_entry_from_xattr(tmp, xattr, size);
	}

	if (ret == E_HM_OK) {
		*acl = tmp;
	} else if (tmp != NULL) {
		free(tmp);
	}

	return ret;
}

static void acl_to_xattr(const struct sec_posix_acl *acl, struct hm_posix_acl_xattr_header *hdr,
			 struct hm_posix_acl_xattr_entry *entry, uint32_t num)
{
	uint32_t i;
	hdr->version = (uint32_t)htole32(HM_POSIX_ACL_VERSION);
	for (i = 0; i < num; i++) {
		entry[i].tag = htole16(acl->entries[i].tag);
		entry[i].permission = htole16(acl->entries[i].permission);
		if (acl->entries[i].tag == HM_POSIX_ACL_USER) {
			entry[i].qualifier = htole32(acl->entries[i].qualifier.uid);
		} else if (acl->entries[i].tag == HM_POSIX_ACL_GROUP) {
			entry[i].qualifier = htole32(acl->entries[i].qualifier.gid);
		} else {
			entry[i].qualifier = (uint32_t)htole32(HM_POSIX_ACL_UNDEFINED_ID);
		}
	}
}

ssize_t sec_posix_acl_to_xattr(const struct sec_posix_acl *acl, char *xattr, size_t size)
{
	int ret = sec_posix_acl_is_invalid(acl) ? E_HM_INVAL : E_HM_OK;
	ssize_t need_size = 0;
	struct hm_posix_acl_xattr_header *header = NULL;
	struct hm_posix_acl_xattr_entry *entry = NULL;
	char *data = NULL;

	if (ret == E_HM_OK) {
		need_size = (ssize_t)(sizeof(struct hm_posix_acl_xattr_header) +
				      sizeof(struct hm_posix_acl_xattr_entry) * acl->entry_num);
	}
	if (ret == E_HM_OK && xattr != NULL) {
		if (size < (size_t)need_size) {
			ret = E_HM_RANGE;
		} else {
			header = void_to_ptr(xattr, struct hm_posix_acl_xattr_header);
			data = xattr + sizeof(*header);
			entry = void_to_ptr(data, struct hm_posix_acl_xattr_entry);
			acl_to_xattr(acl, header, entry, acl->entry_num);
		}
	}
	return ret == E_HM_OK ? need_size : (ssize_t)ret;
}

static inline bool own_perm(const struct sec_posix_acl_entry *entry, unsigned short mask)
{
	return (entry->permission & mask) == mask;
}

#define FOUND_ENTRY	1
#define NEED_MASK	2
static inline void iter_entry(bool hit, int ret_succ, const struct sec_posix_acl_entry **entry, int *ret)
{
	if (hit) {
		*ret = ret_succ;
	} else {
		(*entry)++;
	}
}

static int pre_check(const struct sec_chk_cred *cred, const struct sec_cred_dac *dac_cred,
		     const struct sec_hm_acl_obj *obj,
		     const struct sec_posix_acl_entry **p_entry,
		     unsigned short mask)
{
	int ret = E_HM_OK;
	const struct sec_posix_acl_entry *entry = *p_entry;
	bool match_group = false;
	bool matched = false;
	int result;

	iter_entry(dac_cred->fsuid == obj->uid || dac_cred->fsuid == obj->cuid, \
		   FOUND_ENTRY, &entry, &ret);
	while (ret == E_HM_OK && entry->tag == HM_POSIX_ACL_USER) {
		iter_entry(dac_cred->fsuid == entry->qualifier.uid,
			   NEED_MASK, &entry, &ret);
	}
	if (ret == E_HM_OK) {
		result = sec_cred_check_dac_groups(cred, obj->gid, dac_cred->fsgid);
		matched = (result == E_HM_OK);
		if (!matched && obj->gid != obj->cgid) {
			result = sec_cred_check_dac_groups(cred, obj->cgid, dac_cred->fsgid);
			matched = (result == E_HM_OK);
		}
		match_group = matched || match_group;
		iter_entry(matched && own_perm(entry, mask),
			   NEED_MASK, &entry, &ret);
	}
	while (ret == E_HM_OK && entry->tag == HM_POSIX_ACL_GROUP) {
		result = sec_cred_check_dac_groups(cred, entry->qualifier.gid, dac_cred->fsgid);
		matched = (result == E_HM_OK);
		match_group = matched || match_group;
		iter_entry(matched && own_perm(entry, mask),
			   NEED_MASK, &entry, &ret);
	}
	*p_entry = entry;
	/* match groups but no permissions */
	if (ret == E_HM_OK && match_group) {
		ret = E_HM_ACCES;
	}
	return ret;
}

int sec_hm_acl_check(const struct sec_chk_cred *cred,
		     const struct sec_cred_dac *dac_cred,
		     const struct sec_hm_acl_obj *obj,
		     unsigned short mask)
{
	int ret = (cred == NULL || dac_cred == NULL || obj == NULL) ? E_HM_INVAL : E_HM_OK;
	unsigned perm = 0;
	const struct sec_posix_acl_entry *entry = NULL;

	if (ret == E_HM_OK && obj->acl == NULL) {
		ret = E_HM_AGAIN;
	}
	if (ret == E_HM_OK) {
		entry = &obj->acl->entries[0];
		ret = pre_check(cred, dac_cred, obj, &entry, mask);
	}
	if (ret >= E_HM_OK) {
		if (entry->tag == HM_POSIX_ACL_MASK) { /* not found, match other */
			entry++;
		}
		perm = entry->permission;
		if (ret == NEED_MASK) {
			/* sec_hm_acl_check make sure that a mask exists */
			while (entry->tag != HM_POSIX_ACL_MASK) {
				entry++;
			}
			perm &= entry->permission;
		}
		ret = ((perm & mask) == mask) ? E_HM_OK : E_HM_ACCES;
	}
	return ret;
}

int sec_posix_acl_check(const struct sec_chk_cred *cred,
			const struct sec_cred_dac *dac_cred,
			const struct sec_posix_acl_obj *obj,
			unsigned short mask)
{
	int ret = (obj == NULL) ? E_HM_INVAL : E_HM_OK;
	struct sec_hm_acl_obj hm_obj;

	if (ret == E_HM_OK) {
		hm_obj.uid = obj->uid;
		hm_obj.cuid = obj->uid;
		hm_obj.gid = obj->gid;
		hm_obj.cgid = obj->gid;
		hm_obj.acl = obj->acl;
		ret = sec_hm_acl_check(cred, dac_cred, &hm_obj, mask);
	}

	return ret;
}

struct sec_posix_acl *sec_posix_acl_dup(const struct sec_posix_acl *acl)
{
	struct sec_posix_acl *new_acl = NULL;
	uint32_t i;

	new_acl = !sec_posix_acl_is_invalid(acl) ? sec_posix_acl_alloc(acl->entry_num) : NULL;
	if (new_acl != NULL) {
		for (i = 0; i < acl->entry_num; i++) {
			new_acl->entries[i] = acl->entries[i];
		}
	}
	return new_acl;
}

void sec_posix_acl_free(struct sec_posix_acl *acl)
{
	if (sec_posix_acl_is_invalid(acl)) {
		return;
	}
	free(acl);
}

void sec_posix_acl_free_ebr(struct sec_posix_acl *acl, struct epoch_set *epoch)
{
	if (sec_posix_acl_is_invalid(acl)) {
		return;
	}
	struct sec_posix_acl *old = ebr_dereference(acl);

	ebr_assign_pointer(acl, NULL);
	(void)synchronize_ebr(epoch);
	if (sec_posix_acl_is_invalid(old) == false) {
		free(old);
	}
}

int sec_posix_acl_get(struct sec_posix_acl *acl)
{
	int ret;

	if (sec_posix_acl_is_invalid(acl)) {
		ret = E_HM_OK;
	} else {
		ret = raw_refcnt_nr_get(&acl->refcnt);
	}
	return ret;
}

void sec_posix_acl_put(struct sec_posix_acl *acl)
{
	int ret;

	if (!sec_posix_acl_is_invalid(acl)) {
		ret = raw_refcnt_nr_put(&acl->refcnt);
		if (ret == 0) {
			sec_posix_acl_free(acl);
		}
	}
}

void sec_posix_acl_put_ebr(struct sec_posix_acl *acl, struct epoch_set *epoch)
{
	int ret;

	if (!sec_posix_acl_is_invalid(acl)) {
		ret = raw_refcnt_nr_put(&acl->refcnt);
		if (ret == 0) {
			sec_posix_acl_free_ebr(acl, epoch);
		}
	}
}

static inline uint16_t uint16_shl(uint16_t u, unsigned int shift)
{
	return (uint16_t)((unsigned)u << shift);
}

static inline void convert_mode(struct sec_posix_acl_entry *entry, mode_t *mode,
				unsigned mode_shift, bool from_mode, bool to_mode)
{
	if (from_mode && to_mode) {
		entry->permission &= ((uint16_t)(*mode >> mode_shift)) | ~SEC_CAN_RWX;
		*mode &= (mode_t)((unsigned)uint16_shl(entry->permission, mode_shift) |
				  ~(unsigned)(SEC_CAN_RWX << mode_shift));
	} else if (from_mode) {
		entry->permission = (uint16_t)((*mode >> mode_shift) & SEC_CAN_RWX);
	} else { /* to_mode */
		*mode |= uint16_shl(entry->permission, mode_shift);
	}
}

/* convert ACL from/to mode */
static int acl_convert_mode(struct sec_posix_acl *acl, mode_t *mode,
			    bool from_mode, bool to_mode)
{
	uint32_t i;
	int ret = SEC_POSIX_ACL_TRIVIAL;
	mode_t m = from_mode ? (*mode & S_IRWXUGO) : 0;
	struct sec_posix_acl_entry *mask_entry = NULL;
	struct sec_posix_acl_entry *group_obj_entry = NULL;

	for (i = 0; i < acl->entry_num; i++) {
		switch (acl->entries[i].tag) {
		case HM_POSIX_ACL_USER_OBJ:
			convert_mode(&acl->entries[i], &m, SEC_SHIFT_UBIT,
				     from_mode, to_mode);
			break;
		case HM_POSIX_ACL_OTHER:
			convert_mode(&acl->entries[i], &m, SEC_SHIFT_OBIT,
				     from_mode, to_mode);
			break;
		case HM_POSIX_ACL_USER:
		case HM_POSIX_ACL_GROUP:
			ret = E_HM_OK;
			break;
		case HM_POSIX_ACL_GROUP_OBJ:
			group_obj_entry = &acl->entries[i];
			break;
		case HM_POSIX_ACL_MASK:
			mask_entry = &acl->entries[i];
			ret = E_HM_OK;
			break;
		default:
			panic("an unknown tag of ACL, tag:%u",
			      (unsigned)acl->entries[i].tag);
			break;
		}
	}
	convert_mode((mask_entry != NULL ? mask_entry : group_obj_entry),
		     &m, SEC_SHIFT_GBIT, from_mode, to_mode);
	if (to_mode) {
		*mode &= ~S_IRWXUGO;
		*mode |= m;
	}
	return ret;
}

int sec_posix_acl_convert_mode(struct sec_posix_acl *acl, mode_t *mode,
			       bool from_mode, bool to_mode)
{
	int ret = (sec_posix_acl_is_invalid(acl) || mode == NULL) ? E_HM_INVAL : E_HM_OK;

	if (ret == E_HM_OK && (!from_mode && !to_mode)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = acl_convert_mode(acl, mode, from_mode, to_mode);
	}
	return ret;
}
