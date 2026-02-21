/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for ocontext adapter
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_ADAPTER_OCONTEXT_H
#define HMSEL_ADAPTER_OCONTEXT_H

#include "../oconcommon.h"
#include "mls_level.h"
#include <sepol/policydb/policydb.h>
#include "policydb.h"

static inline hmsel_sid_t
adapter_ocon_sid(const struct hmsel_ocontext *oconp, unsigned int idx);
static inline void
adapter_ocon_sid_set(struct hmsel_ocontext *oconp, unsigned int idx, hmsel_sid_t sid);

static inline size_t ocon_length(ocontext_t *c)
{
	size_t len = adapter_ocon_sid((struct hmsel_ocontext *)c, 1U);
	if (len == 0U) {
		len = strlen(c->u.name);
		adapter_ocon_sid_set((struct hmsel_ocontext *)c, 1U, (hmsel_sid_t)len);
	}
	return len;
}

static inline bool ocon_match(ocontext_t *c,
			      const char *fpath, sepol_security_class_t s)
{
	size_t len = ocon_length(c);
	if ((c->v.sclass == 0 || s == c->v.sclass) &&
	    strncmp(c->u.name, fpath, len) == 0) {
		return true;
	}
	return false;
}

static inline ocontext_t *ocon_find(ocontext_t *ocon, const char *fpath,
				    sepol_security_class_t s)
{
	ocontext_t *ret = ocon;
	while (ret != NULL && !ocon_match(ret, fpath, s)) {
		ret = ret->next;
	}
	return ret;
}

static inline genfs_t *genfs_find(genfs_t *genfsp, const char *fsname)
{
	int cmp;
	genfs_t *ret = genfsp;
	while (ret != NULL &&
	       (cmp = strcmp(fsname, ret->fstype)) > 0) {
		ret = ret->next;
	}
	return (ret == NULL || cmp != 0) ? NULL : ret;
}

static inline ocontext_t *
genfs_ocon_find(genfs_t *genfsp, const char *fsname,
		const char *fpath, sepol_security_class_t s)
{
	genfs_t *fsp = genfs_find(genfsp, fsname);
	if (fsp != NULL) {
		return ocon_find(fsp->head, fpath, s);
	}
	return NULL;
}

static inline struct hmsel_ocontext *
adapter_ocon_genfs(const struct adapter_policydb *db, const char *fsname,
		   const char *fpath, hmsel_class_t sc)
{
	ocontext_t *cp = NULL;
	cp = genfs_ocon_find(db->p->genfs, fsname, fpath, (sepol_security_class_t)sc);
	return (struct hmsel_ocontext *)(uintptr_t)cp;
}

static inline struct hmsel_ocontext *
adapter_ocon_fs_use(const struct adapter_policydb *db, const char *fsname)
{
	ocontext_t *c = db->p->ocontexts[OCON_FSUSE];
	while (c != NULL && strcmp(fsname, c->u.name) != 0) {
		c = c->next;
	}
	return (struct hmsel_ocontext *)(uintptr_t)c;
}

static inline unsigned int
adapter_ocon_behavior(const struct hmsel_ocontext *oconp)
{
	const ocontext_t *c = (const ocontext_t *)(uintptr_t)oconp;
	return c->v.behavior;
}

static inline hmsel_sid_t
adapter_ocon_sid(const struct hmsel_ocontext *oconp, unsigned int idx)
{
	const ocontext_t *c = (const ocontext_t *)(uintptr_t)oconp;
	vatomic32_t *v32 = (vatomic32_t *)(uintptr_t)(&c->sid[idx]);
	return (hmsel_sid_t)vatomic32_read_acq(v32);
}

static inline void
adapter_ocon_sid_set(struct hmsel_ocontext *oconp, unsigned int idx, hmsel_sid_t sid)
{
	ocontext_t *c = (ocontext_t *)(uintptr_t)oconp;
	vatomic32_t *v32 = (vatomic32_t *)(uintptr_t)(&c->sid[idx]);
	vatomic32_write(v32, sid);
}

static inline int
adapter_ocon_context(const struct hmsel_ocontext *oconp,
		     struct hmsel_context *conp, unsigned int idx)
{
	context_struct_t *sepol_conp = &(((ocontext_t *)(uintptr_t)oconp)->context[idx]);
	return context_from_sepol(conp, sepol_conp);
}

static inline struct hmsel_ocontext *
adapter_ocon_isids(const struct adapter_policydb *db)
{
	return (struct hmsel_ocontext *)(db->p->ocontexts[OCON_ISID]);
}

static inline struct hmsel_ocontext *
adapter_ocon_next(const struct hmsel_ocontext *oconp)
{
	ocontext_t *sepol_oconp = (ocontext_t *)oconp;
	return (struct hmsel_ocontext *)sepol_oconp->next;
}

static inline bool
ocon_port_match(ocontext_t *c, uint8_t protocol, uint16_t port)
{
	return (c->u.port.protocol == protocol &&
		c->u.port.low_port <= port &&
		c->u.port.high_port >= port);
}

static inline struct hmsel_ocontext *
adapter_ocon_port(const struct adapter_policydb *db, uint8_t protocol, uint16_t port)
{
	ocontext_t *sepol_oconp = db->p->ocontexts[OCON_PORT];
	while (sepol_oconp != NULL &&
	       !ocon_port_match(sepol_oconp, protocol, port)) {
		sepol_oconp = sepol_oconp->next;
	}
	return (struct hmsel_ocontext *)sepol_oconp;
}

static inline bool
ocon_node_match(ocontext_t *c, uint32_t addr)
{
	return (c->u.node.addr == (addr & c->u.node.mask));
}

static inline struct hmsel_ocontext *
adapter_ocon_node(const struct adapter_policydb *db, uint32_t addr)
{
	ocontext_t *sepol_oconp = db->p->ocontexts[OCON_NODE];
	while (sepol_oconp != NULL &&
	       !ocon_node_match(sepol_oconp, addr)) {
		sepol_oconp = sepol_oconp->next;
	}
	return (struct hmsel_ocontext *)sepol_oconp;
}

#define ADDR6_LEN 4
static inline bool
addr6_match(uint32_t *in, uint32_t *addr, uint32_t *mask)
{
	int i = 0;
	while (i < ADDR6_LEN && addr[i] == (in[i] & mask[i])) {
		i++;
	}
	return (i == ADDR6_LEN);
}

static inline bool
ocon_node6_match(ocontext_t *c, uint32_t *in)
{
	uint32_t *mask = c->u.node6.mask;
	uint32_t *addr = c->u.node6.addr;
	return addr6_match(in, addr, mask);
}

static inline struct hmsel_ocontext *
adapter_ocon_node6(const struct adapter_policydb *db, uint32_t *addr)
{
	ocontext_t *sepol_oconp = db->p->ocontexts[OCON_NODE6];
	while (sepol_oconp != NULL &&
	       !ocon_node6_match(sepol_oconp, addr)) {
		sepol_oconp = sepol_oconp->next;
	}
	return (struct hmsel_ocontext *)sepol_oconp;
}

#ifdef CONFIG_SEHARMONY_POLICY_SHARE
static int do_ocontext_sid_init(struct hmsel_ocontext *oconp,
				struct hmsel_sidtab *sidtab,
				bool is_genfs,
				int (*func)(struct hmsel_sidtab *,
					    struct hmsel_ocontext *,
					    unsigned int, hmsel_sid_t *))
{
	int rc = E_HM_OK;
	hmsel_sid_t sid = 0;

	for (; oconp != NULL; oconp = adapter_ocon_next(oconp)) {
		if (is_genfs) {
			(void)ocon_length((ocontext_t *)(uintptr_t)oconp);
		}

		rc = func(sidtab, oconp, 0, &sid);
		if (rc != E_HM_OK) {
			hm_error("ocontext_to_sid failed.\n");
			break;
		}
	}

	return rc;
}

static inline int adapter_fsuse_ocontext_sid_init(const struct adapter_policydb *db,
						  struct hmsel_sidtab *sidtab,
						  int (*func)(struct hmsel_sidtab *,
							      struct hmsel_ocontext *,
							      unsigned int, hmsel_sid_t *))
{
	struct hmsel_ocontext *oconp = (struct hmsel_ocontext *)(uintptr_t)
					db->p->ocontexts[OCON_FSUSE];
	return do_ocontext_sid_init(oconp, sidtab, false, func);
}

static inline int adapter_genfs_ocontext_sid_init(const struct adapter_policydb *db,
						  struct hmsel_sidtab *sidtab,
						  int (*func)(struct hmsel_sidtab *,
							      struct hmsel_ocontext *,
							      unsigned int, hmsel_sid_t *))
{
	int rc = E_HM_OK;
	genfs_t *genfsp = db->p->genfs;
	struct hmsel_ocontext *oconp = NULL;

	while(genfsp != NULL) {
		oconp = (struct hmsel_ocontext *)(uintptr_t)(genfsp->head);
		rc = do_ocontext_sid_init(oconp, sidtab, true, func);
		if (rc != E_HM_OK) {
			break;
		}
		genfsp = genfsp->next;
	}

	return rc;
}
#endif

#endif
