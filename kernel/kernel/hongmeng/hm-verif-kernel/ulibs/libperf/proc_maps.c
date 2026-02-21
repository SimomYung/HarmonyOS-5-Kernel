/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: libperf entry skeleton
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 29 10:51:29 CST 2023
 */

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <malloc.h>
#include <inttypes.h>
#include <sys/types.h>

#include <libperf/perf.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>

#include <hmkernel/kload.h>
#include <hmkernel/capability.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

#define PERF_KLOAD_ELF_NAME "kernel.elf"

static uint32_t perm_str_2_flags(const char *perm_str)
{
	uint32_t flags = 0;

	for (; *perm_str != '\0'; perm_str++) {
		switch (*perm_str) {
		case 'r':
			flags |= PERF_PROC_MMAP_F_READ;
			break;
		case 'w':
			flags |= PERF_PROC_MMAP_F_WRITE;
			break;
		case 'x':
			flags |= PERF_PROC_MMAP_F_EXE;
			break;
		case 's':
			flags |= PERF_PROC_MMAP_F_SHARE;
			break;
		default:
			break;
		};
	}

	return flags;
}

static int add_mmap_seg(struct perf_proc_maps *maps, int pid, uint32_t cnode_idx,
			unsigned long base, unsigned long size,
			unsigned long offset, uint32_t flags, const char *fname)
{
	struct perf_proc_mmap_segment *mmap_seg = NULL;

	mmap_seg = malloc(sizeof(struct perf_proc_mmap_segment));
	if (mmap_seg == NULL) {
		return E_HM_NOMEM;
	}

	mmap_seg->pid = pid;
	mmap_seg->cnode_idx = cnode_idx;
	mmap_seg->base = base;
	mmap_seg->size = size;
	mmap_seg->flags = flags;
	mmap_seg->offset = offset;

	mmap_seg->fname = strdup(fname);
	if (mmap_seg->fname == NULL) {
		free(mmap_seg);
		return E_HM_NOMEM;
	}

	dlist_insert_tail(&maps->mmap_list, &mmap_seg->node);
	maps->count++;

	return E_HM_OK;
}

#define ANON_NAME_PREFIX	"[anon:[liblinux/"
#define ANON_NAME_SUFFIX	"]]"

static void fixup_fname(uint32_t cnode_idx, char *fname, size_t buf_sz, uint32_t flags)
{
	/* fixup sysmgr */
	if (cnode_idx == hmsrv_sysmgr_cnode_idx() && (flags & PERF_PROC_MMAP_F_EXE) != 0) {
		if (strcmp(fname, "sysmgr") == 0) {
			/* fix prehistoric name */
			NOFAIL(strncpy_s(fname, buf_sz, "sysmgr.elf", buf_sz - 1));
		} else if (strcmp(fname, "shmm") == 0) {
			/* fix vdso name */
			NOFAIL(strncpy_s(fname, buf_sz, "vdso.so", buf_sz - 1));
		}
	}

	/* fixup ko modules in devhost, e.g. "[anon:[liblinux/of.ko]]" */
	size_t prefix_len = strlen(ANON_NAME_PREFIX);
	if (strncmp(fname, ANON_NAME_PREFIX, prefix_len) == 0) {
		char *suffix = strstr(fname, ANON_NAME_SUFFIX);
		if (suffix != NULL) {
			size_t nr_cpy = (size_t)(suffix - fname) - prefix_len;
			NOFAIL(memmove_s(fname, buf_sz, fname + prefix_len, nr_cpy));
			fname[nr_cpy] = '\0';
		}
	}
}

static FILE *open_proc_maps(int pid)
{
	char path[PATH_MAX] = "";
	FILE *fp;
	int ret;

	ret = snprintf_s(path, sizeof(path), sizeof(path) - 1, "/proc/%d/maps", pid);
	if (ret <= 0) {
		return NULL;
	}

	fp = fopen(path, "r");
	if (!fp) {
		hm_error("failed to open %s\n", path);
	}

	return fp;
}

#define PROC_MAPS_LINE_MIN_FIELDS	4
#define DECIMAL 10

static int read_proc_maps(struct perf_proc_maps *maps, int pid)
{
	char *line = NULL;
	size_t len = 0;
	FILE *fp;
	int ret = E_HM_OK;

	fp = open_proc_maps(pid);
	if (!fp) {
		return E_HM_ACCES;
	}

	while (getline(&line, &len, fp) != EOF) {
		char fname[PATH_MAX] = "";
		char perm_str[5] = "";
		uint32_t cnode_idx = 0;
		uint32_t flags;
		unsigned long start_addr = 0, end_addr = 0, offset = 0;

		ret = sscanf_s(line, "%lx-%lx %s %lx %*s %*s %[^\n]",
			       &start_addr, &end_addr, perm_str, sizeof(perm_str), &offset, fname, sizeof(fname));
		if (ret < PROC_MAPS_LINE_MIN_FIELDS || start_addr > end_addr) {
			hm_warn("malformed line in /proc/%d/maps\n", pid);
			continue;
		}

		flags = perm_str_2_flags(perm_str);

		ret = hm_get_cnode_idx(pid, &cnode_idx);
		if (ret != E_HM_OK) {
			hm_warn("get cnode_idx of pid %d err: %s", pid, hmstrerror(ret));
			continue;
		}

		fixup_fname(cnode_idx, fname, sizeof(fname), flags);

		ret = add_mmap_seg(maps, pid, cnode_idx, start_addr, end_addr - start_addr, offset,
				   flags, fname);
		if (ret != E_HM_OK) {
			break;
		}
	}
	free(line);
	(void)fclose(fp);

	return ret;
}

static bool is_pid_dir(const struct dirent *entry)
{
	const char *p;

	for (p = entry->d_name; *p; p++) {
		if (!isdigit((int)*p)) {
			return false;
		}
	}

	return true;
}

static int collect_process_maps(struct perf_proc_maps *maps)
{
	DIR *procdir;
	struct dirent *entry;
	int pid;

	procdir = opendir("/proc");
	if (!procdir) {
		hm_error("open /proc failed\n");
		return E_HM_ACCES;
	}

	while ((entry = readdir(procdir)) != NULL) {
		if (!is_pid_dir(entry)) {
			continue;
		}

		pid = (int)strtol(entry->d_name, NULL, DECIMAL);
		if (pid == 0) {
			continue;
		}

		(void)read_proc_maps(maps, pid);
	}

	(void)closedir(procdir);
	return E_HM_OK;
}

static int collect_kernel_maps(struct perf_proc_maps *maps)
{
	struct kload_s kload;
	int ret;

	ret = actvcall_hmcall_perf_get_kernel_load_info(&kload, sizeof(struct kload_s));
	if (ret == E_HM_NOSYS) {
		hm_info("kernel perf not supported\n");
		return E_HM_OK;
	} else if (ret != E_HM_OK) {
		hm_warn("failed to get kernel load info\n");
		return ret;
	}

	ret = add_mmap_seg(maps, 0, 0, kload.base, kload.base_len, 0,
			   PERF_PROC_MMAP_F_READ | PERF_PROC_MMAP_F_EXE,
			   PERF_KLOAD_ELF_NAME);
	return ret;
}

int perf_collect_process_maps(struct perf_proc_maps *maps)
{
	int ret;

	BUG_ON(maps == NULL);

	dlist_init(&maps->mmap_list);
	maps->count = 0;

	ret = collect_process_maps(maps);
	if (ret == E_HM_OK) {
		ret = collect_kernel_maps(maps);
	}

	if (ret != E_HM_OK) {
		/* clear partial maps */
		perf_destroy_process_maps(maps);
	}

	return ret;
}

void perf_destroy_process_maps(struct perf_proc_maps *maps)
{
	struct perf_proc_mmap_segment *pos = NULL;
	struct perf_proc_mmap_segment *tmp = NULL;

	BUG_ON(maps == NULL);

	dlist_for_each_entry_safe(pos, tmp, &maps->mmap_list, struct perf_proc_mmap_segment, node) {
		dlist_delete(&pos->node);
		maps->count--;

		free(pos->fname);
		free(pos);
	}
}
