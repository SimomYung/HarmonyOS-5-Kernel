/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide hooks of function_trace for trace framework
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 24 19:54:44 2019
 */
#include <hmkernel/errno.h>
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmelf/elf_load.h>
#include <libhmtrace/hmtrace.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libhmtrace/ftrace/function_trace.h>

#ifdef CONFIG_FUNCTION_TRACE
#include <hmasm/ftrace.h>
#include "function_trace.h"
#include "function_graph.h"

#define BITS_PER_INT			32

/* used for instruction calculation in address transition */
#define BL_INSN_OFFSET			0x00ffffff
#define BL_JUMP_FLAG			0xff000000
#define BL_JUMP_FORWARD			0x97000000
#define BL_ADDR_MASK			0x00ffffff
#define PAGE_BASE_ADDR_MASK		0xfffffffffffff000

/* acquire the absolute distance to the current bl insn, maybe forward or backward */
#define distance_to_bl(bl_insn)		(((bl_insn) & BL_INSN_OFFSET) << 2)

/* acquire the 2 low bits of immediate data of adrp insn */
#define immlo_of_adrp(adrp_insn)	(((adrp_insn) & 0x60000000) >> 29)

/* acquire the 19 high bits of immediate data of adrp insn */
#define immhi_of_adrp(adrp_insn)	(((adrp_insn) & 0x00ffffe0) >> 3)

/*
 * acquire the real distance from the page base addr of current adrp insn
 * to the page base addr of mcount
 */
#define distance_to_adrp(adrp_insn)	\
((immhi_of_adrp(adrp_insn) + immlo_of_adrp(adrp_insn)) << 12)

/* acquire the inner page offset of mcount in its page based on ldr insn */
#define ldr_inner_page_offset(ldr_insn)	((((ldr_insn) & 0x003ffc00) >> 10) * 8)
#define FTRACE_SWAB_BYTE_1_SHIFT	8
#define FTRACE_SWAB_BYTE_3_SHIFT	24

static inline __u32 swab32(__u32 val)
{
	return	(((__u32)(val) & (__u32)0x000000ffUL) << FTRACE_SWAB_BYTE_3_SHIFT) |
		(((__u32)(val) & (__u32)0x0000ff00UL) << FTRACE_SWAB_BYTE_1_SHIFT) |
		(((__u32)(val) & (__u32)0x00ff0000UL) >> FTRACE_SWAB_BYTE_1_SHIFT) |
		(((__u32)(val) & (__u32)0xff000000UL) >> FTRACE_SWAB_BYTE_3_SHIFT);
}

enum symbol_mode sym_mode;
static char *elf_file_name = NULL;
static unsigned long offset_of_aslr = 0;
static struct elf_context *elf_context = NULL;
struct symbol_desc *elf_sym_desc = NULL;
static struct ftrace_stub_section *ftrace_stub = NULL;
static struct hmtrace_class hmtrace_class_ftrace = {
	.name = "function_trace",
	.event_table = {
		.uev_table = NULL,
	},
	.register_event = NULL,
	.unregister_event = NULL,
	.class_entry = HMTRACE_CLASS_ENTRY_INIT(hmtrace_class_ftrace.class_entry),
	.enable_event = NULL,
	.disable_event = NULL,
	.prepare_for_show = NULL,
	.get_available_events_list = NULL,
	.freeram = NULL
};

static void release_ftrace_stub(void)
{
	if (ftrace_stub != NULL) {
		process_unload_ftrace_stub_section(ftrace_stub);
		free(ftrace_stub);
		ftrace_stub = NULL;
	}
}

static void release_sym_desc(void)
{
	if (elf_sym_desc != NULL) {
		process_unload_symbol(elf_sym_desc);
		free(elf_sym_desc);
		elf_sym_desc = NULL;
	}
}

static void release_elf_context(void)
{
	if (elf_context != NULL) {
		elf_destroy_ctx(elf_context);
		free(elf_context);
		elf_context = NULL;
	}
}

static void release_resource(void)
{
	release_ftrace_stub();
	release_sym_desc();
	release_elf_context();
}

static __u32 endian_code(__u32 val)
{
	__u32 ret;
#if ((__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) && defined (CONFIG_AARCH64))
	ret = swab32(val);
#else
	ret = val;
#endif
	return ret;
}

static int function_trace_handler_hook(void)
{
	int err = E_HM_OK;
	uint32_t record_len = 0;
	uintptr_t reg_current_fp = 0;
	uintptr_t stack_frame_fp = 0;
	struct function_trace_record record = {0};
	struct rb* prb = NULL;

	prb = hm_trace_rb_get(&hmtrace_class_ftrace.class_entry);
	/* record ip while set tracing on */
	if (*(prb->private.ftrace_enable) == 0) {
		asm volatile("mov %0, fp" : "=r"(reg_current_fp));

		/* get the pc of traced function */
		record.ip = *(uintptr_t *)(reg_current_fp + sizeof(unsigned long));
		stack_frame_fp = *(uintptr_t *)reg_current_fp;

		/* get the pc of parent func of traced function */
		record.parent_ip = *(uintptr_t *)(stack_frame_fp + sizeof(unsigned long));

		err = hm_clock_gettime(CLOCK_MONOTONIC, &(record.time));
		if (err < E_HM_OK) {
			hm_warn("function trace get clock time failed\n");
		} else {
			record_len = sizeof(struct function_trace_record);
			size_t len = hm_ring_buffer_write_event(prb, &record, (size_t)record_len);
			if (len == 0) {
				err = E_HM_INVAL;
				hm_warn("function trace write record data failed: %s\n",
					hmstrerror(err));
			}
		}
	}
	hm_trace_rb_put(prb);

	return err;
}

static bool is_adrp_insn(uint32_t insn)
{
	return (insn & 0x9F000000) == 0x90000000;
}

/*
 * Function addr_trans is used to calculate the addr where will modify code
 * based on ftrace_stub_site_vaddr. The main procedure is that parsing the
 * instruction of bl, adrp and ldr on aarch64 platform, and then acquire the
 * addr where will store the loaded addr of mcount in .got.plt section.
 * For every ftrace_stub_site_vaddr, it will return the same addr.
 */
static uintptr_t addr_trans(uintptr_t stub_site)
{
	uint32_t insn_of_bl_mcount;
	uint32_t insn_of_adrp;
	uint32_t insn_of_ldr;
	uint32_t distance_to_bl_mcount;
	uintptr_t addr_of_mcount_plt;
	uintptr_t adrp_page_base_addr;
	uintptr_t page_distance_to_adrp;
	uintptr_t mcount_page_base_addr;
	uintptr_t inner_page_offset;

	/* acquire the instruction of bl mcount */
	insn_of_bl_mcount = endian_code(*(uint32_t *)(stub_site + offset_of_aslr));

	/* calculate the distance from mcount@plt to bl mcount */
	distance_to_bl_mcount = distance_to_bl(insn_of_bl_mcount);

	/* judge whether jump forward or backward and calculate the addr of mcount@plt */
	if ((insn_of_bl_mcount & BL_JUMP_FLAG) == BL_JUMP_FORWARD) {
		addr_of_mcount_plt = (stub_site + distance_to_bl_mcount) & BL_ADDR_MASK;
	} else {
		addr_of_mcount_plt = (stub_site - distance_to_bl_mcount) & BL_ADDR_MASK;
	}

	/* acquire the instruction of adrp and check */
	insn_of_adrp = endian_code(*(uint32_t *)(addr_of_mcount_plt + offset_of_aslr));
	if (!is_adrp_insn(insn_of_adrp)) {
		return 0;
	}

	/* acquire the page base addr of adrp insn */
	adrp_page_base_addr = addr_of_mcount_plt & PAGE_BASE_ADDR_MASK;

	/* calculate the pages align distance from mcount to adrp */
	page_distance_to_adrp = (uintptr_t)distance_to_adrp((uintptr_t)insn_of_adrp);
	mcount_page_base_addr = adrp_page_base_addr + page_distance_to_adrp;

	/* parse the ldr insn to calculate the inner_page_offset of mcount */
	insn_of_ldr = endian_code(*(uint32_t *)(addr_of_mcount_plt + offset_of_aslr + sizeof(int)));
	inner_page_offset = ldr_inner_page_offset(insn_of_ldr);

	return mcount_page_base_addr + inner_page_offset;
}

static void endian_addr_code(uint32_t *low_addr, uint32_t *high_addr)
{
#if ((__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) && defined (CONFIG_AARCH64))
	uint32_t tmp;

	tmp = *low_addr;
	*low_addr = *high_addr;
	*high_addr = tmp;
#else
	UNUSED(low_addr);
	UNUSED(high_addr);
#endif
}

static int modify_code_to_handler_hook(void)
{
	int err;
	uintptr_t *stub_site_vaddr = NULL;
	uintptr_t dest_addr;
	uintptr_t hook_addr;
	uint32_t low_addr_of_hook;
	uint32_t high_addr_of_hook;

	if (ftrace_stub == NULL || ftrace_stub->stub_site_num == 0) {
		return E_HM_INVAL;
	}

	/* replace the addr of mcount by the addr of function_trace_hook */
	stub_site_vaddr = ftrace_stub->stub_site_vaddr;
	dest_addr = addr_trans(stub_site_vaddr[0]);
	if (dest_addr == 0) {
		return E_HM_POSIX_FAULT;
	}

	hook_addr = (uintptr_t)function_trace_handler_hook;
	low_addr_of_hook = (uint32_t)hook_addr;
	high_addr_of_hook = (uint32_t)(hook_addr >> BITS_PER_INT);

	endian_addr_code(&low_addr_of_hook, &high_addr_of_hook);
	err = actvcall_hmcall_ftrace_modify_code(dest_addr + offset_of_aslr,
					low_addr_of_hook);
	if (err != E_HM_OK) {
		hm_warn("modify addr of mcount to hook low addr failed: %s\n",
			hmstrerror(err));
		return err;
	}

	err = actvcall_hmcall_ftrace_modify_code(dest_addr + offset_of_aslr + sizeof(int),
					high_addr_of_hook);
	if (err != E_HM_OK) {
		hm_warn("modify addr of mcount to hook high addr failed: %s\n",
			hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

static int modify_code_to_nop(void)
{
	int err;
	uint64_t i;
	uint64_t stub_site_num;
	uintptr_t *stub_site_vaddr = NULL;

	if (ftrace_stub == NULL) {
		return E_HM_INVAL;
	}

	stub_site_num = ftrace_stub->stub_site_num;
	stub_site_vaddr = ftrace_stub->stub_site_vaddr;

	/* replace bl mcount with nop insn to reduce the performance expenses */
	for (i = 0; i < stub_site_num; i++) {
		if (stub_site_vaddr[i] == 0) {
			continue;
		}

		/* modify code for all stub sites even if someone failed */
		err = actvcall_hmcall_ftrace_modify_code(stub_site_vaddr[i] + offset_of_aslr,
						NOP_INSN);
		if (err != E_HM_OK) {
			hm_warn("modify insn to nop failed: %s\n", hmstrerror(err));
		}
	}

	return E_HM_OK;
}

static int modify_code_to_mcount(void)
{
	int err;
	uint64_t i;
	uint64_t stub_site_num;
	uintptr_t *stub_site_vaddr = NULL;
	uint32_t *stub_site_insn = NULL;

	if (ftrace_stub == NULL) {
		return E_HM_INVAL;
	}

	stub_site_num = ftrace_stub->stub_site_num;
	stub_site_vaddr = ftrace_stub->stub_site_vaddr;
	stub_site_insn = ftrace_stub->stub_site_insn;

	/* replace the nop insn with bl mcount */
	for (i = 0; i < stub_site_num; i++) {
		if (stub_site_vaddr[i] == 0) {
			continue;
		}

		err = actvcall_hmcall_ftrace_modify_code(stub_site_vaddr[i] + offset_of_aslr,
						stub_site_insn[i]);
		if (err != E_HM_OK) {
			hm_warn("restore origin insn of bl mcount failed: %s\n",
				hmstrerror(err));
			return err;
		}
	}

	return E_HM_OK;
}

static int insn_replace_for_trace_init(void)
{
	int err;

	ftrace_stub = (struct ftrace_stub_section *)malloc(sizeof(struct ftrace_stub_section));
	if (ftrace_stub == NULL) {
		hm_warn("struct ftrace_stub alloc failed\n");
		release_resource();
		return E_HM_NOMEM;
	}

	err = process_load_ftrace_stub_section(elf_context, ftrace_stub);
	if (err != E_HM_OK) {
		hm_debug("load ftrace stub section failed: %s\n", hmstrerror(err));
		free(ftrace_stub);
		ftrace_stub = NULL;
		return err;
	}

	err = modify_code_to_mcount();
	if (err != E_HM_OK) {
		hm_warn("modify code from nop to bl mcount failed: %s\n",
			hmstrerror(err));
		err = modify_code_to_nop();
		if (err != E_HM_OK) {
			hm_warn("replace insn to nop failed: %s\n", hmstrerror(err));
		}
		release_resource();
		return E_HM_POSIX_FAULT;
	}

	err = modify_code_to_handler_hook();
	if (err != E_HM_OK) {
		hm_warn("modify addr of mcount to handler hook failed: %s\n",
			hmstrerror(err));
		err = modify_code_to_nop();
		if (err != E_HM_OK) {
			hm_warn("replace insn to nop failed: %s\n", hmstrerror(err));
		}
		release_resource();
		return E_HM_POSIX_FAULT;
	}

	return E_HM_OK;
}

static void read_symbol_desc(struct bunch *sym_bunch,
			     struct symbol_desc *sym_desc)
{
	sym_desc->fname = bunch_read_string(sym_bunch);
	sym_desc->flags = bunch_read_uint32(sym_bunch);
	sym_desc->base = bunch_read_uint64(sym_bunch);
	sym_desc->offset = bunch_read_uint64(sym_bunch);
	sym_desc->base_len = bunch_read_uint32(sym_bunch);
	sym_desc->dso_sym.symtab_addr = bunch_read_uint64(sym_bunch);
	sym_desc->dso_sym.strtab_addr = bunch_read_uint64(sym_bunch);
	sym_desc->dso_sym.symtab_len = bunch_read_uint32(sym_bunch);
	sym_desc->dso_sym.strtab_len = bunch_read_uint32(sym_bunch);
}

static int acquire_elf_load_info(pid_t pid)
{
	int err;
	int count;
	struct bunch *sym_bunch = NULL;

	sym_bunch = bunch_ipc_create(0);
	if (sym_bunch == NULL) {
		err = E_HM_NOMEM;
		hm_warn("bunch_ipc_create failed\n");
		return err;
	}

	err = hm_proc_elf_load_info(pid, sym_bunch);
	if (err != E_HM_OK) {
		hm_warn("load process symbol description failed: %s\n",
			 hmstrerror(err));
		bunch_ipc_destroy(sym_bunch);
		return err;
	}

	/* skip process count */
	(void)bunch_read_int32(sym_bunch);

	/* skip process cnode_idx */
	(void)bunch_read_uint32(sym_bunch);

	/* read symbol_desc count */
	count = bunch_read_int32(sym_bunch);
	for (int i = 0; i < count; i++) {
		struct symbol_desc sym_desc = { 0 };

		/* find the elf file */
		read_symbol_desc(sym_bunch, &sym_desc);
		if ((sym_desc.flags & MAIN_ELF_SYMBOL) != 0U) {
			elf_file_name = strdup(sym_desc.fname);
			if (elf_file_name == NULL) {
				err = E_HM_NOMEM;
				break;
			}
			offset_of_aslr = sym_desc.base;
			break;
		}
	}

	bunch_ipc_destroy(sym_bunch);

	return err;
}

static int parse_elf_binary_file(pid_t pid)
{
	int err;

	err = acquire_elf_load_info(pid);
	if (err != E_HM_OK) {
		hm_warn("acquire the elf load info failed: %s\n", hmstrerror(err));
		return err;
	}

	/* parse elf to get the symbol and ftrace_stub_section */
	elf_context = (struct elf_context *)malloc(sizeof(struct elf_context));
	if (elf_context == NULL) {
		hm_warn("struct elf_context alloc failed\n");
		free(elf_file_name);
		elf_file_name = NULL;
		return E_HM_NOMEM;
	}

	err = process_parse_elf(elf_file_name, elf_context);
	if (err != E_HM_OK) {
		hm_warn("parse elf %s failed: %s\n", elf_file_name, hmstrerror(err));
		free(elf_context);
		elf_context = NULL;
		free(elf_file_name);
		elf_file_name = NULL;
		return err;
	}
	free(elf_file_name);
	elf_file_name = NULL;

	return E_HM_OK;
}

int hm_ftrace_init(void)
{
	int err;
	pid_t pid;

	pid = hm_getpid();
	BUG_ON(pid < 0);

	hm_trace_init(pid);

	err = hm_trace_register_class(&hmtrace_class_ftrace);
	if (err != E_HM_OK) {
		hm_warn("function trace register failed:%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = parse_elf_binary_file(pid);
		if (err != E_HM_OK) {
			hm_warn("parse elf binary file failed: %s\n", hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		err = insn_replace_for_trace_init();
		if (err != E_HM_OK) {
			hm_debug("instruction replacement for function trace init failed: %s\n",
				 hmstrerror(err));
		}
		release_elf_context();
	}

	return err;
}

int hm_ftrace_parse_elf_file(pid_t pid)
{
	int err;

	err = parse_elf_binary_file(pid);
	if (err == E_HM_OK) {
		if (elf_context->elf_header.elf_class == ELFCLASS32) {
			sym_mode = SYMBOL_MODE_32;
		} else {
			sym_mode = SYMBOL_MODE_64;
		}
		elf_sym_desc = (struct symbol_desc *)malloc(sizeof(struct symbol_desc));
		if (elf_sym_desc == NULL) {
			hm_warn("alloc sym_desc failed\n");
			release_resource();
			err = E_HM_NOMEM;
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(*elf_sym_desc);
		err = process_load_symbol(elf_context, elf_sym_desc);
		if (err != E_HM_OK) {
			release_resource();
		}
	}

	if (err == E_HM_OK) {
		elf_sym_desc->offset = offset_of_aslr;
	}

	return err;
}

static void format_show_data(char* buf, size_t buf_size,
			     const struct function_trace_record *record,
			     const struct symbol_entry *ip_entry,
			     const struct symbol_entry *parent_ip_entry)
{
	int ret;

	ret = snprintf_s(buf, buf_size, buf_size - 1,
			 "[%ld.%06ld]%16s 0x%lx <-- %16s 0x%lx",
			 record->time.tv_sec, record->time.tv_nsec / 1000L,
			 ip_entry->name, record->ip,
			 parent_ip_entry->name, record->parent_ip);
	if (ret < 0) {
		hm_warn("snprintf_s failed\n");
	}
}

int hm_ftrace_lookup_symbol(const struct function_trace_record *record,
			    char* buf, size_t buf_size)
{
	int err = E_HM_OK;
	struct symbol_entry ip_entry;
	struct symbol_entry parent_ip_entry;

	if (buf_size > HMTRACE_MAX_ENTRY_LEN) {
		hm_info("invalid buf_size: %zu\n", buf_size);
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		mem_zero_s(ip_entry);
		mem_zero_s(parent_ip_entry);
		err = symbol_lookup_by_addr(elf_sym_desc, sym_mode,
					    record->ip, &ip_entry);
		if (err < 0) {
			hm_info("unable to lookup symbol: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		/* main function has no parent ip */
		if (strcmp(ip_entry.name, "main") != 0) {
			err = symbol_lookup_by_addr(elf_sym_desc, sym_mode,
						    record->parent_ip, &parent_ip_entry);
			if (err < 0) {
				hm_info("unable to lookup symbol: %s\n", hmstrerror(err));
			} else {
				format_show_data(buf, buf_size, record,
						 &ip_entry, &parent_ip_entry);
			}
		}
	}
	return err;
}

void hm_ftrace_release_elf_context(void)
{
	release_elf_context();
}

void hm_ftrace_release_sym_desc(void)
{
	release_sym_desc();
}
#else
int hm_ftrace_init(void)
{
	return E_HM_NOSYS;
}

int hm_ftrace_lookup_symbol(const struct function_trace_record *record,
			    char* buf, size_t buf_size)
{
	UNUSED(record);
	UNUSED(buf);
	UNUSED(buf_size);
	return E_HM_NOSYS;
}

int hm_ftrace_parse_elf_file(pid_t pid)
{
	UNUSED(pid);
	return E_HM_NOSYS;
}

void hm_ftrace_release_elf_context(void)
{
	return;
}

void hm_ftrace_release_sym_desc(void)
{
	return;
}
#endif
