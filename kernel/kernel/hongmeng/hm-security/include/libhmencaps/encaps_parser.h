/*
* Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
* Description: ENCAPS header file
* Author: Huawei OS Kernel Lab
* Create: Thu Apr 25 19:57:33 2024
*/
#ifndef ENCAPS_PARSER_H
#define ENCAPS_PARSER_H
#include <libsec/cred.h>
#include <libsec/encaps_cred.h>
int encaps_parser(const char *encap_str, struct sec_encaps **const encap_list, uint32_t *count);
struct sec_encaps *encaps_list_merge(
    struct sec_encaps *encaps1, uint32_t size1, struct sec_encaps *encaps2, uint32_t size2, uint32_t *size_mrg);
int get_encaps_value_from_str(const char *encaps_str, char *key, uint32_t *value, enum encaps_type *type);
#endif
