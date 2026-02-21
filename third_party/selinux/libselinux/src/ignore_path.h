/*
* Copyright (c) 2024 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef _IGNORE_PATH_H_
#define _IGNORE_PATH_H_

#include <stdbool.h>
#include <stddef.h>
#define SLASH_SUFFIX_LEN 1
#define STAR_SUFFIX_LEN 2
#define SYSTEM_IGNORE_CFG_PATH "/system/etc/selinux/ignore_cfg"
#define VENDOR_IGNORE_CFG_PATH "/vendor/etc/selinux/ignore_cfg"

enum skip_type {
    SKIP_NONE = 0,
    SKIP_SUB_DIR = 1,
    SKIP_SELF_SUB_DIR = 2
};

typedef struct ignore_path_node {
    char *path;
    struct ignore_path_node *next;
} ignore_path_node_t;

typedef struct ignore_paths {
    ignore_path_node_t *slash_suffix_paths;
    ignore_path_node_t *star_suffix_paths;
} ignore_paths_t;

ignore_path_node_t *insert_ignore_path(ignore_path_node_t **paths_ptr, const char *line);
size_t trim_newline(char *str);

typedef struct path_info {
    ignore_path_node_t **paths_ptr;
    unsigned int suffix_len;
} path_info_t;

path_info_t trim_suffix_and_get_path_info(char *line, size_t real_length);
bool load_ignore_cfg_from_file(const char *cfg_path);
bool load_ignore_cfg();
enum skip_type skip_ignore_relabel(const char *path);
void free_ignore_list(ignore_path_node_t **list_ptr);

#endif // IGNORE_PATH_H
