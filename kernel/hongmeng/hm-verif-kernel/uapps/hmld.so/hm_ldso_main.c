/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Main procedure of hmld
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */

#include <fcntl.h>

#include <jmp.h>
#include <dynlink.h>
#include <hm_ldso.h>

#include <sys/mman.h>
#include <hmasm/page.h> /* for PAGE_SIZE */
#include <hmasm/stackprotector.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <hmkernel/cache.h>
#include <libhmucap/ucap.h>
#include <hmkernel/cache.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_auxv.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/pac.h>
#endif

#define ELF_EHDR_BUF_SIZE 896
#define LOAD_OFFSET_SIZE (512 * 1024)

#define LIBC_KASAN_NAME "libc_kasan.so"
#define LIBC_SYS_NAME "libc-sys.so"
#define LIBC_NAME "libc.so"

#define LIBC_PATH "/lib/libc.so"
#define LIBC_SYS_PATH "/lib/libc-sys.so"
#define LIBC32_PATH "/lib32/libc.so"
#define LIBCILP32_PATH "/libilp32/libc.so"

struct ldso_elf {
	size_t phdr;
	size_t phent;
	size_t phnum;
	size_t base;
	size_t base_len;
	size_t ventry;
};

static bool is_valid_elf(const Elf_Ehdr *eh)
{
	bool ret = true;
	if (!(eh->e_type == ET_DYN || eh->e_type == ET_EXEC)) {
		ret = false;
	}

	if (eh->e_phentsize != sizeof(Elf_Phdr)) {
		ret = false;
	}

	if ((eh->e_phnum < 1) || (eh->e_phnum > UINT16_MAX / eh->e_phentsize)) {
		ret = false;
	}

	return ret;
}

static inline unsigned prot_from_ph(const Elf_Phdr *ph)
{
	return ((((ph->p_flags & PF_R) != 0) ? PROT_READ : 0) |
		(((ph->p_flags & PF_W) != 0) ? PROT_WRITE : 0) |
		(((ph->p_flags & PF_X) != 0) ? PROT_EXEC : 0));
}

static int zero_bss_seg(uintptr_t bss_start, uintptr_t bss_end)
{
	uintptr_t bss_start_align;
	size_t bss_left_len;

	bss_start_align = (bss_start + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1));
	mem_zero_b((char *)bss_start, bss_start_align - bss_start);
	bss_left_len = bss_end - bss_start_align;
	if (bss_left_len != 0) {
		if (ldso_mmap((void *)bss_start_align, bss_left_len,
			      PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED)
			return E_HM_INVAL;
	}

	return E_HM_OK;
}

static inline char touch_data_seg(const void *touch_ptr,
				  size_t touch_len)
{
	volatile char touch_read = '\0';
	size_t len;

	len = 0;
	while (len < touch_len) {
		touch_read = *((char *)touch_ptr + len);
		len += PAGE_SIZE;
	}
	return touch_read;
}

static Elf_Ehdr* ldso_load_elf_eh(int fd, char *buf, size_t len)
{
	size_t phsize;
	Elf_Ehdr *eh = NULL;

	/* read elf header */
	size_t l = (size_t)ldso_read(fd, buf, len);
	if (l < sizeof(*eh)) {
		hm_error("ldso read elf header len: len=%zu\n", l);
		return NULL;
	}

	eh = (Elf_Ehdr *)(void *)buf;
	if (!is_valid_elf(eh)) {
		hm_error("ldso check elf header failed\n");
		return NULL;
	}

	phsize = (size_t)eh->e_phentsize * (size_t)eh->e_phnum;
	if (phsize > len - sizeof(*eh)) {
		hm_panic("size is to small: phsize=%zx, num=%d\n", phsize, (int)(eh->e_phnum));
	}

	if (eh->e_phoff + phsize > l) {
		hm_panic("don't supported yet, need re-alloc memory\n");
	}

	return eh;
}

static int ldso_load_elf_section(const Elf_Ehdr *eh, int fd, unsigned char *map,
				 size_t addr_min, size_t load_offset)
{
	int rc;
	size_t i;
	unsigned prot = 0;
	Elf_Phdr *ph = NULL;
	Elf_Phdr *ph0 = NULL;
	off_t off_start = 0;
	size_t this_min = 0;
	size_t this_max = 0;
	size_t load_max = 0;
	unsigned char *base = map - addr_min;

	ph = (Elf_Phdr *)(void *)((char *)eh + eh->e_phoff);
	ph0 = ph;
	for (ph = ph0, i = eh->e_phnum; i != 0; i--, ph = (void *)((char *)ph + eh->e_phentsize)) {
		if (ph->p_type != PT_LOAD) {
			continue;
		}
		this_min = (ph->p_vaddr & (~(PAGE_SIZE - 1))) + load_offset;
		this_max = (size_t)((ph->p_vaddr + ph->p_filesz + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1))) + load_offset;
		load_max = (size_t)((ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1))) + load_offset;
		off_start = (off_t)(ph->p_offset & (~(PAGE_SIZE - 1)));

		prot = prot_from_ph(ph);
		/* mmap this file section into memory. */
		if (ldso_mmap(base + this_min, this_max - this_min, (int)prot,
			      MAP_PRIVATE | MAP_FIXED,
			      fd, off_start) == MAP_FAILED) {
			return E_HM_INVAL;
		}

		/* memory zero bss section */
		if (ph->p_memsz > ph->p_filesz && ((ph->p_flags & PF_W) != 0)) {
			rc = zero_bss_seg(ptr_to_ulong(base + ph->p_vaddr + ph->p_filesz + load_offset),
					  ptr_to_ulong(base + load_max));
			if (rc != E_HM_OK) {
				return rc;
			}
		}

		/* needs to touch data segment to avoid copy form user or copy to user failed */
		if ((ph->p_flags & PF_W) != 0) {
			(void)touch_data_seg(base + this_min, this_max - this_min);
		}
	}

	return E_HM_OK;
}

static int ldso_map_elf(int fd, size_t load_offset, struct ldso_elf *elf)
{
	int err;
	size_t i, map_len;
	size_t addr_max = 0;
	Elf_Phdr *ph = NULL;
	size_t addr_min = SIZE_MAX;
	unsigned char *map = MAP_FAILED;
	Elf_Ehdr *eh, buf[(ELF_EHDR_BUF_SIZE + sizeof(Elf_Ehdr)) / sizeof(Elf_Ehdr)];
	eh = ldso_load_elf_eh(fd, (char *)buf, sizeof(buf));
	if (eh == NULL) {
		return E_HM_INVAL;
	}
	ph = (Elf_Phdr *)(void *)((char *)buf + eh->e_phoff);
	for (i = eh->e_phnum; i > 0; i--, ph = (void *)((char *)ph + eh->e_phentsize)) {
		if (ph->p_type != PT_LOAD) {
			continue;
		}
		if (ph->p_vaddr < addr_min) {
			addr_min = (size_t)ph->p_vaddr;
		}
		if (ph->p_vaddr + ph->p_memsz > addr_max) {
			addr_max = (size_t)(ph->p_vaddr + ph->p_memsz);
		}
	}
	addr_max += (PAGE_SIZE - 1) + load_offset;
	addr_max &= ~(PAGE_SIZE - 1);
	addr_min += load_offset;
	addr_min &= ~(PAGE_SIZE - 1);
	map_len = addr_max - addr_min;
	/* reserve [addr_min:addr_max] in current process vspace */
	map = ldso_mmap(ulong_to_ptr(addr_min, void), map_len, 0, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (map == MAP_FAILED) {
		return E_HM_NOMEM;
	}
	err = ldso_load_elf_section(eh, fd, map, addr_min, load_offset);
	if (err != E_HM_OK) {
		(void)ldso_munmap(map, map_len);
		return err;
	}
	/* record elf information */
	elf->base = ptr_to_ulong(map);
	elf->base_len = map_len;
	elf->phnum = eh->e_phnum;
	elf->phent = eh->e_phentsize;
	elf->phdr = elf->base + (size_t)eh->e_phoff;
	elf->ventry = (size_t)eh->e_entry + elf->base;
	return 0;
}

static int ldso_load_elf(const char *path, struct ldso_elf *elf,
			 size_t load_offset)
{
	int fd;
	int err;
	int ret;

	if (path == NULL || elf == NULL || *path == '\0') {
		return E_HM_INVAL;
	}

	fd = ldso_open(path, O_RDONLY, 0);
	if (fd < 0) {
		hm_error("ldso: open %s failed: %s", path, hmstrerror(fd));
		return E_HM_INVAL;
	}

	err = ldso_map_elf(fd, load_offset, elf);
	if (err != 0) {
		ret = ldso_close(fd);
		if (ret != 0) {
			hm_warn("ldso close failed: %s\n", hmstrerror(ret));
		}
		hm_error("ldso: failed to map file %s, fd=%d to process: %s\n", path, fd,
			 hmstrerror(err));
		return err;
	}

	return ldso_close(fd);
}

#ifdef LDSO_DEBUG

static inline void ldso_debug(int argc, char *argv[])
{
	char **envp = argv + argc + 1;
	for (int i = 0; i < argc; i++) {
		hm_debug("ldso: argc=%d, argv[%d]=%s\n", argc, i, argv[i]);
	}
	for (int i = 0; envp[i] != NULL; i++) {
		hm_debug("ldso: envp[%d]=%s\n", i, envp[i]);
	}
}

#endif

static int find_so_in_dynamic(unsigned long num, unsigned long hdr, const char *lib_name)
{
	unsigned long i = 0;
	unsigned long index = 0;
	unsigned long alsr_offset = 0;

	Elf_Phdr *phdr_base = ulong_to_ptr(hdr, Elf_Phdr);
	for (i = 0; i < num; i++) {
		if (phdr_base[i].p_type == PT_DYNAMIC) {
			index = i;
		}
		if (phdr_base[i].p_type == PT_PHDR) {
			alsr_offset = hdr - phdr_base[i].p_vaddr;
		}
	}

	Elf_Phdr *dyn_phdr = phdr_base + index;
	unsigned long dyn_segment_addr = dyn_phdr->p_vaddr + alsr_offset;
	unsigned long dyn_ent_num = dyn_phdr->p_memsz / sizeof(Elf_Dyn);

	unsigned long dynstr = 0;
	Elf_Dyn *dyn_base = ulong_to_ptr(dyn_segment_addr, Elf_Dyn);
	for (i = 0; i < dyn_ent_num; i++) {
		if (dyn_base[i].d_tag == DT_STRTAB) {
			dynstr = dyn_base[i].d_un.d_ptr + alsr_offset;
		}
	}

	for (i = 0; i < dyn_ent_num; i++) {
		if (dyn_base[i].d_tag == DT_NEEDED) {
			index = dyn_base[i].d_un.d_val;
			hm_debug("lib name : %s\n", ulong_to_ptr(dynstr, char) + index);
			if (strcmp(ulong_to_ptr(dynstr, char) + index, lib_name) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

static void fix_auxv(const struct ldso_elf *libc_so, size_t **auxv)
{
	int i;
	size_t *fix_auxv = *auxv;

	for (i = 0; fix_auxv[i] != 0; i += STEP_SIZE) {
		if (fix_auxv[i] == AT_BASE) {
			fix_auxv[i + 1] = ptr_to_ulong(libc_so->base);
		} else if (fix_auxv[i] == AT_LIBCMAPLEN) {
			fix_auxv[i + 1] = libc_so->base_len;
		}
	}
}

static bool paths_access(char *libc_path, char *path[], size_t len)
{
	int fd;
	int ret;
	bool res = false;

	for (int i = 0; i < (int)len; i++) {
		fd = ldso_open(path[i], O_RDONLY, 0);
		if (fd >= 0) {
			NOFAIL(strncpy_s(libc_path, _POSIX_PATH_MAX,
					 path[i], strlen(path[i])));

			ret = ldso_close(fd);
			if (ret != 0) {
				hm_warn("ldso close %s failed: %s\n",
					path[i], hmstrerror(ret));
			}
			res = true;
			break;
		}
	}
	return res;
}

static void find_default_libc(char *libc_path, unsigned int compatible)
{
	bool found = false;
	char *libc_default[] = {
		LIBC_PATH,
		LIBC_SYS_PATH,
	};
	char *libc_compat[] = {
		LIBC32_PATH,
	};
	char *libc_ilp32[] = {
		LIBCILP32_PATH,
	};

	if ((compatible & (1U << AT_FLAGS_COMPAT32_BIT)) != 0) {
		found = paths_access(libc_path, libc_compat, ARRAY_SIZE(libc_compat));
	} else if ((compatible & (1U << AT_FLAGS_ILP32_BIT)) != 0) {
		found = paths_access(libc_path, libc_ilp32, ARRAY_SIZE(libc_ilp32));
	} else {
		found = paths_access(libc_path, libc_default, ARRAY_SIZE(libc_default));
	}
	BUG_ON(found == false, "No libc found!\n");
}

#ifdef CONFIG_SUPPORT_MULT_LIBC
const char *valid_libc_dir[] = {
	"/usr/local/hpe_um",
#ifdef CONFIG_OPENHARMONY
	"/data/hm/lib",
#endif
	"/lib",
	NULL
};
static void find_interp_dir(char *dir, size_t dir_size, size_t *aux)
{
	Elf_Phdr *phdr = (void *)aux[AT_PHDR];
	size_t interp_off = 0;
	size_t base = 0;
	char tmp_dir[_POSIX_PATH_MAX] = {0};

	for (size_t i = aux[AT_PHNUM]; i; i--, phdr = (void *)((char *)phdr + aux[AT_PHENT])) {
		if (phdr->p_type == PT_INTERP) {
			interp_off = (size_t)phdr->p_vaddr;
		}
		if (phdr->p_type == PT_PHDR) {
			base = aux[AT_PHDR] - phdr->p_vaddr;
		}
	}
	if (interp_off != 0) {
		NOFAIL(strncpy_s(tmp_dir, _POSIX_PATH_MAX,
				 (char *)(uintptr_t)(interp_off + base), _POSIX_PATH_MAX - 1));
		char *interp_name = strrchr(tmp_dir, '/');
		if (interp_name != NULL) {
			tmp_dir[interp_name - tmp_dir] = '\0';
			NOFAIL(strncpy_s(dir, dir_size, tmp_dir, _POSIX_PATH_MAX - 1));
		}
	}
}

static bool is_valid_libc(char *dir)
{
	bool valid = false;

	for (int i = 0; valid_libc_dir[i] != NULL; i++) {
		if (strcmp(dir, valid_libc_dir[i]) == 0) {
			valid = true;
			break;
		}
	}

	return valid;
}

#endif

static void find_libc_path(char *libc_path, unsigned int compatible, size_t *aux)
{
	int err;
	int i = 0;
	int fd = -1;
	const char *dir = ((compatible & (1U << AT_FLAGS_COMPAT32_BIT)) != 0) ? "/lib32" :
			  (((compatible & (1U << AT_FLAGS_ILP32_BIT)) != 0) ? "/libilp32" : "/lib");
	const char *libc_names[] = {
		LIBC_KASAN_NAME,
		LIBC_SYS_NAME,
		LIBC_NAME,
		NULL,
	};

	for (i = 0; libc_names[i] != NULL; i++) {
		if (find_so_in_dynamic((unsigned long)aux[AT_PHNUM], (unsigned long)aux[AT_PHDR],
				       libc_names[i]) == 1) {
			break;
		}
	}
	if ((unsigned int)i == ARRAY_SIZE(libc_names) - 1) {
		find_default_libc(libc_path, compatible);
	} else {
#ifdef CONFIG_SUPPORT_MULT_LIBC
		char tmp_dir[_POSIX_PATH_MAX] = {0};

		find_interp_dir(tmp_dir, _POSIX_PATH_MAX, aux);
		/* To ensure security, trustlist verification is added for libc dir. */
		if (tmp_dir[0] == '/' && is_valid_libc((char *)tmp_dir)) {
			/*
			 * Find for libc in the directory at the same level as interpreter.
			 * If the find fails, find for libc in the default path.
			 */
			err = snprintf_s(libc_path, _POSIX_PATH_MAX, _POSIX_PATH_MAX - 1, "%s/%s",
					 tmp_dir, libc_names[i]);
			BUG_ON(err <= 0, "snprintf_s failed when find libc path!\n");
			fd = ldso_open(libc_path, O_RDONLY, 0);
		}
	}
	if (fd >= 0) {
		(void)ldso_close(fd);
	} else {
#else
		UNUSED(fd);
#endif
		/* default libc path */
		err = snprintf_s(libc_path, _POSIX_PATH_MAX, _POSIX_PATH_MAX - 1, "%s/%s",
				 dir, libc_names[i]);
		BUG_ON(err <= 0, "snprintf_s failed when find libc path!\n");
	}
}

#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __preinit_array_start []) (void) __attribute__((weak));
extern void (*const __preinit_array_end []) (void) __attribute__((weak));

static void _hm_ldso_preinit(void)
{
	/* For dynamically linked executable, the preinit array is
	 * executed by ldso. And the preinit array should be executed
	 * before _init. */
	uintptr_t pa = (uintptr_t)&__preinit_array_start;
	for (; pa < ((uintptr_t)&__preinit_array_end); pa += sizeof(void(*)())) {
		void (*func)(void);
		func = *(void (**)(void))pa;
		PAC_FECFI_SIGN(func, void(*)(void));
		func();
	}
}
#endif

void _hm_ldso_main(void *sp, int argc, char *argv[])
{
	int i;
	int err;
	unsigned int compatible = 0;
	size_t load_offset;
	struct ldso_elf libc_so;
	size_t aux[LDSO_AUX_CNT];
	size_t *auxv = NULL;
	char libc_path[_POSIX_PATH_MAX] = {0};

	mem_zero_a(aux);
	i = argc + 1;
	while (argv[i] != NULL) {
		i++;
	}
	auxv = (void *)(argv + i + 1);
	for (i = 0; auxv[i] != 0; i += STEP_SIZE) {
		if (auxv[i] < LDSO_AUX_CNT) {
			aux[auxv[i]] = auxv[i + 1];
		}
		if (auxv[i] == AT_FLAGS) {
			compatible = (unsigned int)auxv[i + 1];
		}
		if (auxv[i] == AT_RANDOM) {
			stack_canary_update((*(unsigned long *)auxv[i + 1]));
		}
	}
#ifdef PAC_FWCFI_SYSSERVICE
	_hm_ldso_preinit();
#endif

	find_libc_path(libc_path, compatible, aux);
#ifdef LDSO_DEBUG
	ldso_debug();
#endif

	/* NOTE: later parse libc from elf DT_NEEDED section */
	mem_zero_s(libc_so);

	/*
	 * we need to load libc to the address jump over hmld.so, aux[AT_BASE] is the
	 * hmld.so load base address of hmld.so.
	 * hmld.so max vaddr of PT_LOAD is 200K+, reserve 512K is enough for now.
	 */
	load_offset = aux[AT_BASE] + LOAD_OFFSET_SIZE;
	err = ldso_load_elf(libc_path, &libc_so, load_offset);
	if (err != E_HM_OK) {
		hm_panic("ldso: load libc [%s] failed: %s\n", libc_path, hmstrerror(err));
	}
	hm_debug("LDSO: jump to libc entry [0x%zx], base=0x%zx, main elf entry: 0x%zx\n",
		 libc_so.ventry, libc_so.base, aux[AT_ENTRY]);
	/* fix auxv */
	fix_auxv(&libc_so, &auxv);

	/*
	 * jump to libc entry.
	 * - set sp to sp register.
	 * - set p to x0/r0 register.
	 */
	jmp_ventry(ulong_to_ptr(libc_so.ventry, void), sp);

	/* unreachable */
	__builtin_unreachable();
}
