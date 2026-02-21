/*
* Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
* Description: ENCAPS header file
* Author: Huawei OS Kernel Lab
* Create: Thu Apr 25 19:57:33 2024
*/
#ifndef LIBHMENCAPS_STR2INT_HASH_H
#define LIBHMENCAPS_STR2INT_HASH_H
struct encaps_str2int {
  char *str; 
  unsigned int hash_vl;
};

struct encaps_str2int *find_encaps_str2int (register const char *str, register size_t len);

#endif
