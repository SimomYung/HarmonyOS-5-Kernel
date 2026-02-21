/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: definintion for property related structure
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 10 21:44:04 2022
 */

#ifndef ULIBS_LIBSEC_PROPERTY_PROPS_H
#define ULIBS_LIBSEC_PROPERTY_PROPS_H

#include <stdint.h>

/* property header */
#define HOLDER_PROPT_UNIT(...)
#define HOLDER_PROPT_BIT_FIELD(name, unit) unit name : 1
#define HOLDER_PROPT_BOOL(name, ptype, dtype, unit) HOLDER_PROPT_BIT_FIELD(name, unit)
#define HOLDER_PROPT_UCHAR(name, ptype, dtype, unit) HOLDER_PROPT_UNIT(name, unit)
#define HOLDER_PROPT_UINT32(...) HOLDER_PROPT_UCHAR(__VA_ARGS__)
#define HOLDER_PROPT_INT32(...) HOLDER_PROPT_UCHAR(__VA_ARGS__)
#define HOLDER_PROPT_INT64(...) HOLDER_PROPT_UCHAR(__VA_ARGS__)
#define HOLDER_PROPT_UINT64(...) HOLDER_PROPT_UCHAR(__VA_ARGS__)
struct property_props {
#define PROP_REGISTER_UNFIXED_LEN_TYPE(...)
#define PROP_REGISTER_FIXED_LEN_TYPE(name, ptype, dtype, unit, ...) HOLDER_##dtype(name, _, _, unit);
#include "property_kind_decl.h"
#undef HOLDER_PROPT_UNIT
#define HOLDER_PROPT_UNIT(name, unit) unit name
#undef HOLDER_PROPT_BIT_FIELD
#define HOLDER_PROPT_BIT_FIELD(...)
#include "property_kind_decl.h"
#undef PROP_REGISTER_FIXED_LEN_TYPE
#undef PROP_REGISTER_UNFIXED_LEN_TYPE
};

#endif
