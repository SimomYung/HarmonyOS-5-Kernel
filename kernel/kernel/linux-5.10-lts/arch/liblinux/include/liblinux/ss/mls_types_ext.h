/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Type definitions for the multi-level security (MLS) policy.
 *
 * Author : Stephen Smalley, <sds@tycho.nsa.gov>
 */
/*
 * Updated: Trusted Computer Solutions, Inc. <dgoeddel@trustedcs.com>
 *
 *	Support for enhanced MLS infrastructure.
 *
 * Copyright (C) 2004-2005 Trusted Computer Solutions, Inc.
 */

#ifdef CONFIG_HM_SEPOL_POLICYDB

#ifndef _LIBLINUX_SS_MLS_TYPES_H_
#define _LIBLINUX_SS_MLS_TYPES_H_

struct mls_semantic_cat {
	uint32_t low;	/* first bit this struct represents */
	uint32_t high;	/* last bit represented - equals low for a single cat */
	struct mls_semantic_cat *next;
};

struct mls_semantic_level {
	uint32_t sens;
	struct mls_semantic_cat *cat;
};

struct mls_semantic_range {
	struct mls_semantic_level level[2];
};

#endif	/* _LIBLINUX_SS_MLS_TYPES_H_ */

#endif  /* CONFIG_HM_SEPOL_POLICYDB */
