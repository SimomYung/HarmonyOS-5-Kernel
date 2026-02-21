/*
* Copyright (c) 2024 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* Skipping relabel path if define in ignore_cfg. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ignore_path.h"
#include "callbacks.h"

static g_system_config_loaded = false;
static g_vendor_config_loaded = false;
ignore_paths_t g_ignore_paths = {NULL, NULL};

ignore_path_node_t *insert_ignore_path(ignore_path_node_t **paths_ptr, const char *line)
{
	ignore_path_node_t *new_node = malloc(sizeof(ignore_path_node_t));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->path = strdup(line);
	if (new_node->path == NULL) {
		free(new_node);
		return NULL;
	}
	new_node->next = *paths_ptr;
	*paths_ptr = new_node;
	return new_node;
}

size_t trim_newline(char *str)
{
	size_t length = strlen(str);
	if (length == 0) {
		return 0;
	}
	if (str[length - 1] == '\n') {
		str[--length] = '\0';
	}
	if (length > 0 && str[length - 1] == '\r') {
		str[--length] = '\0';
	}
	return length;
}

path_info_t trim_suffix_and_get_path_info(char *line, size_t real_length)
{
	path_info_t info;
	if (line[real_length - SLASH_SUFFIX_LEN] == '/') {
		info.paths_ptr = &g_ignore_paths.slash_suffix_paths;
		info.suffix_len = SLASH_SUFFIX_LEN;
	} else if (real_length > 1
				&& line[real_length - SLASH_SUFFIX_LEN] == '*'
				&& line[real_length - STAR_SUFFIX_LEN] == '/') {
		info.paths_ptr = &g_ignore_paths.star_suffix_paths;
		info.suffix_len = STAR_SUFFIX_LEN;
	} else {
		info.paths_ptr = NULL;
		info.suffix_len = 0;
	}
	if (info.paths_ptr != NULL) {
		line[real_length - info.suffix_len] = '\0';
	}
	return info;
}

bool load_ignore_cfg_from_file(const char *cfg_path)
{
	FILE *file = fopen(cfg_path, "r");
	if (file == NULL) {
		selinux_log(SELINUX_INFO, "Failed to open file: %s\n", cfg_path);
		return false;
	}

	char *line = NULL;
	size_t len = 0;
	bool result = true;
	while (getline(&line, &len, file) != -1) {
		size_t real_length = trim_newline(line);
		if (real_length > 0) {
			path_info_t info = trim_suffix_and_get_path_info(line, real_length);
			if (info.paths_ptr == NULL) {
				continue;
			}
			line[real_length - info.suffix_len] = '\0';
			ignore_path_node_t *new_node = insert_ignore_path(info.paths_ptr, line);
			if (new_node == NULL) {
				selinux_log(SELINUX_ERROR, "Failed to allocate memory or duplicate string: %s\n", line);
				continue;
			}
		}
	}

	if (fclose(file)) {
		selinux_log(SELINUX_ERROR, "Failed to close file: %s\n", strerror(errno));
	}
	free(line);
	return result;
}

bool load_ignore_cfg()
{
	if (g_system_config_loaded && g_vendor_config_loaded) {
		return true;
	}
	bool result = true;
	if (!g_system_config_loaded) {
		g_system_config_loaded = load_ignore_cfg_from_file(SYSTEM_IGNORE_CFG_PATH);
	}
	if (!g_vendor_config_loaded) {
		g_vendor_config_loaded = load_ignore_cfg_from_file(VENDOR_IGNORE_CFG_PATH);
	}
	if (!g_system_config_loaded || !g_vendor_config_loaded) {
		result = false;
	}
	return result;
}

static bool find_ignore_path(ignore_path_node_t *current, const char *path)
{
	while (current != NULL) {
		if (strcmp(path, current->path) == 0) {
			return true;
		}
		current = current->next;
	}
	return false;
}

enum skip_type skip_ignore_relabel(const char *path)
{
	if (find_ignore_path(g_ignore_paths.slash_suffix_paths, path)) {
		return SKIP_SELF_SUB_DIR;
	}
	if (find_ignore_path(g_ignore_paths.star_suffix_paths, path)) {
		return SKIP_SUB_DIR;
	}
	return SKIP_NONE;
}

void free_ignore_list(ignore_path_node_t **list_ptr)
{
	while (*list_ptr != NULL) {
		ignore_path_node_t *temp = *list_ptr;
		*list_ptr = (*list_ptr)->next;
		free(temp->path);
		free(temp);
	}
	*list_ptr = NULL;
}

void free_ignore_cfg()
{
	free_ignore_list(&g_ignore_paths.slash_suffix_paths);
	free_ignore_list(&g_ignore_paths.star_suffix_paths);
	g_system_config_loaded = false;
	g_vendor_config_loaded = false;
}
