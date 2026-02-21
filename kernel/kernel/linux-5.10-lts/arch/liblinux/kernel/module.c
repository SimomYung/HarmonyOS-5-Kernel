// SPDX-License-Identifier: GPL-2.0-only
#include <linux/genalloc.h>
#include <linux/printk.h>
#include <linux/moduleloader.h>
#include <linux/set_memory.h>
#include <linux/kmemleak.h>

#include <asm/sections.h>

/*
 * liblinux modules pool
 * should be placed near to ".text" section by lds
 */
struct gen_pool *module_mem_pool = NULL;

void arch_module_vmalloc_init(void)
{
	int ret;

	ret = liblinux_pal_module_alloc_init(MODULES_VADDR, MODULES_VSIZE);
	if (ret < 0) {
		pr_warn("liblinux: module alloc init failed, err=%d\n", ret);
		goto err_out;
	}

	module_mem_pool = gen_pool_create(PAGE_SHIFT, -1);
	if (module_mem_pool == NULL) {
		ret = -ENOMEM;
		pr_warn("liblinux: module mem pool create failed, err=%d\n", ret);
		goto err_out;
	}

	ret = gen_pool_add(module_mem_pool,
			   MODULES_VADDR, MODULES_VSIZE, -1);
	if (ret) {
		ret = -ENOMEM;
		pr_warn("liblinux: module mem pool init failed, err=%d\n", ret);
		goto err_out;
	}
	return;

err_out:
	return;
}

void * __override module_alloc(unsigned long size)
{
	unsigned long addr;
	int ret;

	addr = gen_pool_alloc(module_mem_pool, size);
	if (addr == 0) {
		return NULL;
	}

	ret = liblinux_pal_module_alloc_prepare((void *)addr, size);
	if (ret < 0) {
		gen_pool_free(module_mem_pool, addr, size);
		return NULL;
	}

	kmemleak_alloc((void *)addr, size, 1, GFP_KERNEL);
	return (void *)addr;
}

#ifdef CONFIG_HMKIP
void *module_alloc_hkip(unsigned long size_all, unsigned long size_hkiped)
{
	unsigned long addr;
	int ret;
	if (size_all < size_hkiped) {
		return NULL;
	}
	addr = gen_pool_alloc(module_mem_pool, size_all);
	if (addr == 0) {
		return NULL;
	}

	ret = liblinux_pal_module_alloc_prepare_hkip((void *)addr, size_hkiped);
	if (ret < 0) {
        goto err;
	}

	ret = liblinux_pal_module_alloc_prepare((void *)(addr + size_hkiped), size_all - size_hkiped);
	if (ret < 0) {
	    goto release_hkiped;
	}

	kmemleak_alloc((void *)addr, size_all, 1, GFP_KERNEL);
	return (void *)addr;
release_hkiped:
	(void)liblinux_pal_module_alloc_release(addr, &size_hkiped);
err:
	gen_pool_free(module_mem_pool, addr, size_all);
	return NULL;
}
#endif /* CONFIG_HMKIP */

void module_memfree(void *module_region)
{
	unsigned long size;
	int ret;

	/* module_region may be null, see free_module func in kernel/module.c */
	if (module_region == NULL) {
		return;
	}

	kmemleak_free(module_region);

	ret = liblinux_pal_module_alloc_release(module_region, &size);
	if (ret < 0) {
		pr_warn("liblinux: invalid module mem %p\n", module_region);
		return;
	}

	gen_pool_free(module_mem_pool, (unsigned long)module_region, size);
}

#ifdef CONFIG_GENERIC_BUG
void module_bug_finalize(const Elf_Ehdr *hdr,
					const Elf_Shdr *sechdrs,
					struct module *mod)
{
}
void module_bug_cleanup(struct module *mod)
{
}
#endif

/* kernel/extable.c */
int core_kernel_text(unsigned long addr)
{
	/* module is not in core kernel text */
	if ((addr >= MODULES_VADDR) && (addr < MODULES_END))
		return 0;
	/* assume all other address are core kernel text */
	/* can only used by `patch_map()` */
	return 1;
}

int core_kernel_data(unsigned long addr)
{
	if (addr >= (unsigned long)_sdata &&
	    addr < (unsigned long)_edata)
		return 1;
	return 0;
}

int kernel_text_address(unsigned long addr)
{
	if (core_kernel_text(addr) || is_module_text_address(addr))
		return 1;

	return 0;
}

void *text_poke(void *addr, const void *opcode, size_t len)
{
	return memcpy(addr, opcode, len);
}

void text_poke_sync(void)
{
}

void apply_retpolines(s32 *start, s32 *end)
{
}
void apply_returns(s32 *start, s32 *end)
{
}
void apply_alternatives(struct alt_instr *start, struct alt_instr *end)
{
}

void alternatives_smp_module_add(struct module *mod, char *name,
					void *locks, void *locks_end,
					void *text, void *text_end)
{
	if (IS_ENABLED(CONFIG_HOTPLUG_CPU)) {
		panic("%s\n", __func__);
	}
}
void alternatives_smp_module_del(struct module *mod)
{
}

struct plt_entry __override get_plt_entry(u64 val, void *pc)
{
	/*
	 * MOVK/MOVN/MOVZ opcode:
	 * +--------+------------+--------+-----------+-------------+---------+
	 * | sf[31] | opc[30:29] | 100101 | hw[22:21] | imm16[20:5] | Rd[4:0] |
	 * +--------+------------+--------+-----------+-------------+---------+
	 *
	 * Rd     := 0x10 (x16)
	 * hw     := 0b00 (no shift), 0b01 (lsl #16), 0b10 (lsl #32)
	 * opc    := 0b11 (MOVK), 0b00 (MOVN), 0b10 (MOVZ)
	 * sf     := 1 (64-bit variant)
	 */
	return (struct plt_entry){
		/* for liblinux, bit63-bit48 should be `0`, so we use MOVZ for mov0 */
		cpu_to_le32(0xd2800010 | ((( val      ) & 0xffff)) << 5),
		cpu_to_le32(0xf2a00010 | ((( val >> 16) & 0xffff)) << 5),
		cpu_to_le32(0xf2c00010 | ((( val >> 32) & 0xffff)) << 5),
		cpu_to_le32(0xd61f0200)
	};
}

bool __override plt_entries_equal(const struct plt_entry *a,
				  const struct plt_entry *b)
{
	return a->mov0 == b->mov0 &&
	       a->mov1 == b->mov1 &&
	       a->mov2 == b->mov2;
}

/*
 * export module related syscall symbols as expected in <linux/syscalls.h>
 */
extern long __liblinux_sys_init_module(const void *umod,
				       unsigned long len, const char *uargs);
extern long __liblinux_sys_delete_module(const char *name_user,
					 unsigned int flags);

/* GUARD for temporary change fs */
static inline void __fs_guard_restore(mm_segment_t *oldfs) { set_fs(*oldfs); }
#define LIBLINUX_FS_GUARD(name, fs)     \
	mm_segment_t __oldfs_##name __attribute__((cleanup(__fs_guard_restore))); \
	__oldfs_##name = get_fs();      \
	set_fs(fs);

int sys_init_module(const void *umod,
		    unsigned long len, const char *uargs)
{
	LIBLINUX_FS_GUARD(_, KERNEL_DS);
	return __liblinux_sys_init_module(umod, len, uargs);
}
int sys_delete_module(const char *name_user, unsigned int flags)
{
	LIBLINUX_FS_GUARD(_, KERNEL_DS);
	return __liblinux_sys_delete_module(name_user, flags);
}
