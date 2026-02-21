/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define sections and types of policy databases
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 19 11:01:56 CST 2019
 */
/*
 * All data in policy.db is stored as litte endian.
 * Please remember to perform the data conversion when creating new meta data.
 * If you modify the code to add new meta data(especially integers), please remenber:
 * 1) Convert data to litte endian representation(by htole64() or htole32() etc.) before store them to db data stucture.
 *    (in code of build tool(build_policy.bin))
 * 2) Convert raw queried data(usually from share mem) to host reprensentation(by le64toh() or le32toh() etc.).
 *    (in code of libpolicy(libpolicy.so))
 */
#ifndef __LIBSECPLC_POLICY_FMT_H_
#define __LIBSECPLC_POLICY_FMT_H_


#include <stdint.h>

#define HEX_CHARS_PER_U8 2
#define CHKSUM_ARRAY_SIZE 32 // SHA256: 256/8/sizeof(uint8_t)
#define MAX_CHKSUM_STR_LEN (CHKSUM_ARRAY_SIZE * HEX_CHARS_PER_U8)

// Type property signature
// Also as mask to filter out specific bit from "uint32_t property"
#define PERMISSIVE_SUBJ (1U)
#define PERMISSIVE_OBJ (1U << 1)
#define NO_LOG_SUBJ (1U << 2)
#define NO_LOG_OBJ (1U << 3)
#define CACHE_SUBJ (1U << 4)
#define CACHE_OBJ (1U << 5)
#define INVALID_PROPERTY (1U << 31)

enum SEC_TYPE {
	NULL_SECTION,
	HDR,
	SECTBL,
	POLICY_UNIT,
	POLICY_HDR,
	ABILITY_HDR,
	MRANGE_UNIT,
	PORT_RANGE_UNIT,
	FILEOBJ,
	EXTYPE,
	EXABILITY,
	EXACTION,
	STRTBL,
	TYPE_TRANSITION_HDR,
	TYPE_TRANSITION_UNIT,
	PATTERN,
	PATTERN_UNIT,
	POLICY_DESC,
	POLICY_DESC_IDX,
	INNER_ABILITY,
	MAX_SEC
};

enum POLICY_DESC_TYPE {
	TYPY_DEF_DESC,
	ABILITY_DEF_DESC,
	ACTION_DEF_DESC,
	MAX_DESC
};

typedef struct sec_hdr {
	uint64_t sec_num;
	uint64_t off;
	uint64_t sz;
} sec_hdr_t;

typedef struct sec {
	uint64_t type;
	uint64_t sz;
	uint64_t off;
} sec_t;

struct hdr {
	uint64_t version;
	struct sec_hdr sec_hdr;
	uint64_t sz;
	uint8_t chksum[CHKSUM_ARRAY_SIZE];
};

struct policy_hdr_unit {
	uint64_t sub_id;
	uint64_t off; /* policy unit offset */
	uint64_t sz; /* policy unit size */
};

/* identify each unit of type trantition in type_transition_hdr */
struct type_transition_hdr_unit {
	uint64_t sub_id;
	uint64_t off; /* policy unit offset */
	uint64_t sz; /* policy unit size */
};

struct type_transition_unit {
	uint64_t obj_id;
	uint64_t act_id;
	uint64_t tgt_id;
};

struct policy_unit {
	uint64_t obj_id;
	uint64_t act_id;
};

struct fileobj_path_unit {
	uint64_t sub_id;
	uint64_t name_off;
};

struct pattern_path {
	uint64_t stem_off;
	uint64_t stem_size;
	uint64_t unit_off;
	uint64_t size;
};

struct pattern_unit {
	uint64_t sub_id;
	uint64_t name_off;
};

/* in type_id ascending order */
struct policy_desc_unit {
	uint64_t policy_type;
	uint32_t type_id;
	uint32_t property;
	uint64_t unit_off;
};

/* in idx ascending order */
struct policy_desc_idx_unit {
	uint64_t idx;
	uint64_t policy_type;
	uint64_t type_id;
	uint64_t unit_off;
};

#define ABILITY_WIDTH 4U
typedef struct {
	uint64_t ability[ABILITY_WIDTH];
} ability_t;

#define ABILITY_BITS_PER_ELEMENT (sizeof(((ability_t*)0)->ability[0]) * 8U)

struct ability_hdr_unit {
	uint64_t sub_id;
	ability_t ability;
	uint64_t mrange_off; /* mrange offset */
	uint64_t mrange_sz; /* mrange sz */
	uint64_t port_range_off; /* port range offset */
	uint64_t port_range_sz; /* port range sz */
};

struct sec_plc_mem_range_unit {
	uint64_t start;
	uint64_t end;
};

typedef struct sec_plc_mem_range_unit sec_plc_mem_range_t;

struct mrange_unit {
	uint64_t start;
	uint64_t end;
};

typedef struct mrange_unit mrange_t;

struct sec_plc_port_range_unit {
	uint64_t start;
	uint64_t end;
};

typedef struct sec_plc_port_range_unit sec_plc_port_range_t;

/*
 * Check use-after-free:
 * All pointers in struct are just initialized in source/libpolicy, and they
 * will point to the memory which policy file mapped. Libpolicy supply traverse
 * interface used to get every port_range_unit.
 *
 * For the caller, dereference units obtained using the traversal interface
 * will not cause UAF. They also need to ensure that these pointers are not
 * dereferenced and set to NULL after policy_munmap().
 */
struct sec_plc_port_range_iterator {
	struct sec_plc_port_range_unit *unit_curr;
	struct sec_plc_port_range_unit *unit_last;
};

#define ERR_INVAL (-1)

#define SEC_PLC_ENFORCE 0U
#define SEC_PLC_PERMISSIVE 1U
#define SEC_PLC_DISABLE 2U
#define SEC_PLC_CACHE 4U

#endif
