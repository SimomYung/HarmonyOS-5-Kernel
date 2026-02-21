/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: File guard common interface
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */

#ifndef SYSMGR_INCLUDE_INTAPI_FILEGUARD_H
#define SYSMGR_INCLUDE_INTAPI_FILEGUARD_H

#include "fg_struct_def.h"
#include "stdbool.h"

int file_guard_init(void);
void file_guard_exit(void);

int file_guard_handle_message(const fg_message_t *message);
int32_t fg_dequeue_report_msg(fg_message_t **message);

int fileguard_devhost_handler(void *ctx, size_t len, int *actvret);

bool fg_check_module_valid(void);
void fg_set_module_valid(bool value);

void fg_fs_set_valid(bool value);

int fg_get_dmode(void);
void fg_set_dmode(int dmode);

bool fg_fs_module_enable(void);
bool fg_fs_rename_enable(void);
bool fg_fs_usb_enable(void);

void fg_report_new_file(const char *file_path);
void fg_report_kia_compress(const char *origin_path, const char *variant_path);
void fg_report_rename(const char *old_path, const char *new_path);
void fg_report_operate_record(uint32_t type, const char *file_path);

#define fg_report_read(fpath)  fg_report_operate_record(FG_FILE_READ, (fpath))
#define fg_report_write(fpath) fg_report_operate_record(FG_FILE_WRITE, (fpath))

void fg_report_usb_audit(block_rule_profile_t *block_rule_data, const char *usb_fname);

int insert_block_rule_currect(const char *file_path, fg_file_type_t type);
int insert_block_rule(const fg_process_data_t *process,
    const char *file_path, fg_file_type_t type);

fg_file_type_t fs_match_keyword_rules(const char *filename);
bool fs_match_compress_suffix(const char *filename);
int lookup_block_rule(pid_t pid, block_rule_profile_t *block_rule, uint32_t mask);

#endif // SYSMGR_INCLUDE_INTAPI_FILEGUARD_H
