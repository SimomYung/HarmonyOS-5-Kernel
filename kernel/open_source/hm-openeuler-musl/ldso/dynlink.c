#define _GNU_SOURCE
#define SYSCALL_NO_TLS 1
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <elf.h>
#include <sys/mman.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <link.h>
#include <setjmp.h>
#include <pthread.h>
#include <ctype.h>
#include <dlfcn.h>
#include <semaphore.h>
#include <sys/membarrier.h>
#include "pthread_impl.h"
#include "fork_impl.h"
#include "libc.h"
#include "dynlink.h"
#include "sys/prctl.h"
#ifdef CONFIG_HMKIP
/* prot user only for hkip support */
#define PROT_HKIP   0x8000 /* mmap for hkip support */
#define PT_HKIP  0x60000008
#endif
#include <hmdsosym.h>
#include <stdbool.h>

int __malloc_replaced;
int __aligned_alloc_replaced;

static char *g_filename_bak;

#define SYS_PATH_DFLT SYSLIBDIR ":" LIBDIR

static void error(const char *, ...);

#define MAXP2(a,b) (-(-(a)&-(b)))
#define ALIGN(x,y) ((x)+(y)-1 & -(y))

#define container_of(p,t,m) ((t*)((char *)(p)-offsetof(t,m)))
#define countof(a) ((sizeof (a))/(sizeof (a)[0]))
#define DSO_FLAGS_NODELETE 0x1
#define DSO_FLAGS_DEEPBIND 0x2

#ifdef CONFIG_HMKIP
enum hkip_seg_ind {
	ZERO = 0,
	ONE,
	TWO
};
#endif
struct debug {
	int ver;
	void *head;
	void (*bp)(void);
	int state;
	void *base;
};

struct td_index {
	size_t args[2];
	struct td_index *next;
};

#define MAX_EXTERNAL_NAME_NUM 4

struct dso {
#if DL_FDPIC
	struct fdpic_loadmap *loadmap;
#else
	unsigned char *base;
#endif
	char *name;
	size_t *dynv;
	struct dso *next, *prev;

	/* mark the dso status */
	unsigned int flags;

	Phdr *phdr;
	int phnum;
	size_t phentsize;
	Sym *syms;
	size_t syment;
	/* different from syms, this indicate .symtab */
	uint64_t symtab_addr;
	uint32_t symtab_size;
	Elf_Symndx *hashtab;
	uint32_t *ghashtab;
	int16_t *versym;
	char *strings;
	uint32_t dynstr_size;
	/* different from strings, this indicate .strtab */
	uint64_t strtab_addr;
	uint32_t strtab_size;

	struct dso *syms_next, *lazy_next;
	size_t *lazy, lazy_cnt;
	unsigned char *map;
	size_t map_len;
	dev_t dev;
	ino_t ino;
	char relocated;
	char constructed;
	char kernel_mapped;
	char mark;
	char bfs_built;
	char runtime_loaded;
	char by_dlopen;
	char do_openclose;
	struct dso **deps, *needed_by;
	size_t ndeps_direct;
	size_t next_dep;
	pthread_t ctor_visitor;
	int nr_dlopen;
	char *rpath_orig, *rpath;
	struct tls_module tls;
	size_t tls_id;
	size_t relro_start, relro_end;
	uintptr_t *new_dtv;
	unsigned char *new_tls;
	struct td_index *td_index;
	struct dso *fini_next;
	char *shortname;
	char *name_ext[MAX_EXTERNAL_NAME_NUM];
	unsigned int nr_ext;
#if DL_FDPIC
	unsigned char *base;
#else
	struct fdpic_loadmap *loadmap;
#endif
#ifdef CONFIG_HMKIP
	bool enable_hkip;
	size_t roafinit_start, roafinit_end;
#endif
	struct funcdesc {
		void *addr;
		size_t *got;
	} *funcdescs;
	size_t *got;
	char buf[];
};

#ifdef CONFIG_ARM64_BTI_USER
struct gnu_property {
    uint32_t pr_type;
    uint32_t pr_datasz;
};

#define GNU_PROPERTY_TYPE_0_NAME "GNU"
/* .note.gnu.property types for EM_AARCH64: */
#define GNU_PROPERTY_AARCH64_FEATURE_1_AND   0xc0000000
/* Bits for GNU_PROPERTY_AARCH64_FEATURE_1_BTI */
#define GNU_PROPERTY_AARCH64_FEATURE_1_BTI  (1U << 0)
#define NOTE_NAME_SZ (sizeof(GNU_PROPERTY_TYPE_0_NAME))
/* Note types with note name GNU */
#define NT_GNU_PROPERTY_TYPE_0 5
#define ELF_GNU_PROPERTY_ALIGN 8
#endif

/* Record symbol info of dso */
struct dso_sym_info *dso_syminfo = NULL;

static void do_init_dso_sym_info(void *addr, struct dso_sym_info **cur_sym_info,
				 struct dso_sym_info *prev_sym_info)
{
	uint32_t i;
	struct dso_sym_entry *entries = NULL;

	(*cur_sym_info) = (struct dso_sym_info *)addr;
	(*cur_sym_info)->nr_valid_entry = 0;
	(*cur_sym_info)->nr_sym_entry = (MEM_SIZE_FOR_DSO_INFO -
					 ((uint32_t)(uintptr_t)&((*cur_sym_info)->main_elf_entry) -
					  (uint32_t)(uintptr_t)(*cur_sym_info))) /
					(sizeof(struct dso_sym_entry));
	entries = &(*cur_sym_info)->main_elf_entry;
	for (i = 0; i < (*cur_sym_info)->nr_sym_entry; i++) {
		entries[i].flag = 0;
	}
	strcpy(&((*cur_sym_info)->magic[0]), DSO_INFO_MAGIC);
	(*cur_sym_info)->prev_page = (uint64_t)(uintptr_t)prev_sym_info;
	(*cur_sym_info)->next_page = 0;
	if (prev_sym_info != NULL) {
		prev_sym_info->next_page = (uint64_t)(uintptr_t)(*cur_sym_info);
	}
}

static int add_new_page(struct dso_sym_entry **entry,
		         struct dso_sym_info **cur_sym_info,
		         struct dso_sym_info *last_valid_info)
{
	void *addr = NULL;

	addr = mmap(NULL, MEM_SIZE_FOR_DSO_INFO,
		    PROT_READ | PROT_WRITE,
		    MAP_PRIVATE | MAP_ANON,
		    0, 0);
	if (addr == MAP_FAILED) {
		dprintf(2, "ERROR: Failed to map the MMAP address\n");
		return -EINVAL;
	}
	do_init_dso_sym_info(addr, cur_sym_info, last_valid_info);
	(*entry) = &(*cur_sym_info)->main_elf_entry;

	return 0;
}

static bool find_entry(struct dso_sym_info *tmp_dso_info, struct dso_sym_entry **entry, uint64_t base)
{
	uint32_t i;
	struct dso_sym_entry *entries = NULL;

	entries = &tmp_dso_info->main_elf_entry;
	for (i = 0; i < tmp_dso_info->nr_sym_entry; i++) {
		if (entries[i].flag == 0) {
			if ((*entry) == NULL) {
				(*entry) = &entries[i];
			}
		} else {
			if (entries[i].base == base) {
				(*entry) = &entries[i];
				return true;
			}
		}
	}

	return false;
}

static int __add_dso_syminfo_nolock(struct dso_sym_entry *sym_entry)
{
	int rc = 0;
	bool found = false;
	struct dso_sym_info *cur_sym_info = NULL;
	struct dso_sym_info *last_valid_info = NULL;
	struct dso_sym_info *tmp_dso_info = NULL;
	struct dso_sym_entry *entry = NULL;

	if (dso_syminfo == NULL) {
		dprintf(2, "ERROR: invalid syminfo mem\n");
		return -EINVAL;
	}
	cur_sym_info = dso_syminfo;
	while (cur_sym_info != NULL) {
		last_valid_info = cur_sym_info;
		found = find_entry(cur_sym_info, &entry, sym_entry->base);
		if (found) {
			return -EEXIST;
		}
		if (entry != NULL && tmp_dso_info == NULL) {
			tmp_dso_info = cur_sym_info;
		}
		cur_sym_info = (void *)(uintptr_t)(cur_sym_info->next_page);
	}

	if (entry != NULL) {
		cur_sym_info = tmp_dso_info;
	} else {
		rc = add_new_page(&entry, &cur_sym_info, last_valid_info);
		if (rc != 0) {
			return rc;
		}
	}
	memcpy(entry, sym_entry, sizeof(struct dso_sym_entry));
	entry->flag = 1;
	cur_sym_info->nr_valid_entry++;

	return rc;
}

static size_t count_syms(struct dso *p);
static void __fill_dso_syminfo(struct dso *p)
{
	struct dso_sym_entry entry;

	if (p == NULL) {
		return;
	}
	entry.elf_type = ET_DYN;
	entry.fname = (uint64_t)(uintptr_t)p->name;
	entry.fname_len = (unsigned int)(strlen(p->name) + 1);
	/* if the so was stripped, don't have .symtab section, use .dynsym instead */
	if (p->symtab_addr == (uint64_t)(uintptr_t)MAP_FAILED) {
		entry.symtab_addr = (uint64_t)(uintptr_t)p->syms;
		entry.symtab_len  = (unsigned int)(p->syment * count_syms(p));
	} else {
		entry.symtab_addr = p->symtab_addr;
		entry.symtab_len  = p->symtab_size;
	}

	/* if the so was stripped, don't have .strtab section, use .dynstr instead */
	if (p->strtab_addr == (uint64_t)(uintptr_t)MAP_FAILED) {
		entry.strtab_addr = (uint64_t)(uintptr_t)p->strings;
		entry.strtab_len = p->dynstr_size;
	} else {
		entry.strtab_addr = p->strtab_addr;
		entry.strtab_len = p->strtab_size;
	}
	entry.base = (uint64_t)(uintptr_t)p->map;
	entry.base_len = (unsigned int)p->map_len;
	if (p->tls.size > 0) {
		entry.tls_offset = (uint64_t)(p->tls.offset);
	} else {
		entry.tls_offset = (uint64_t)-1;
	}
	(void)__add_dso_syminfo_nolock(&entry);
}

static void __drop_dso_syminfo(const void *base, struct dso *dso)
{
	bool found;
	struct dso_sym_entry *entry = NULL;
	struct dso_sym_info *cur_sym_info = NULL;
	struct dso_sym_info *prev_sym_info = NULL;
	struct dso_sym_info *next_sym_info = NULL;
	unsigned int page_offset;

	if (base == NULL || dso_syminfo == NULL) {
		return;
	}
	cur_sym_info = dso_syminfo;
	while (cur_sym_info != NULL) {
		found = find_entry(cur_sym_info, &entry, (uint64_t)(uintptr_t)base);
		if (found) {
			break;
		}
		cur_sym_info = (void *)(uintptr_t)(cur_sym_info->next_page);
	}
	if (cur_sym_info == NULL) {
		return;
	}

	if (dso != NULL) {
		if (entry->strtab_addr != (uint64_t)(uintptr_t)dso->strings &&
		    entry->strtab_addr != (uint64_t)(uintptr_t)MAP_FAILED) {
			page_offset = entry->strtab_addr & (PAGE_SIZE - 1);
			(void)munmap((void *)(uintptr_t)(entry->strtab_addr - page_offset),
				     entry->strtab_len + page_offset);
		}
		if (entry->symtab_addr != (uint64_t)(uintptr_t)dso->syms &&
		    entry->symtab_addr != (uint64_t)(uintptr_t)MAP_FAILED) {
			page_offset = entry->symtab_addr & (PAGE_SIZE - 1);
			(void)munmap((void *)(uintptr_t)(entry->symtab_addr - page_offset),
				     entry->symtab_len + page_offset);
		}
	}
	entry->flag = 0;
	cur_sym_info->nr_valid_entry--;
	if (cur_sym_info->nr_valid_entry == 0 && cur_sym_info != dso_syminfo) {
		prev_sym_info = (void *)(uintptr_t)(cur_sym_info->prev_page);
		next_sym_info = (void *)(uintptr_t)(cur_sym_info->next_page);
		munmap(cur_sym_info, MEM_SIZE_FOR_DSO_INFO);
		prev_sym_info->next_page = (uint64_t)(uintptr_t)next_sym_info;
		if (next_sym_info != NULL) {
			next_sym_info->prev_page = (uint64_t)(uintptr_t)prev_sym_info;
		}
	}
}

static void __attribute__((noinline)) pthread_himalloc_tcache_addr_copy(long long addr)
{
	volatile pthread_t tmp = __pthread_self();
	tmp->himalloc_tcache_addr = addr;
}

struct symdef {
	Sym *sym;
	struct dso *dso;
};

typedef void (*stage3_func)(size_t *, size_t *);

static struct builtin_tls {
	char c;
	struct pthread pt;
	void *space[16];
} builtin_tls[1];
#define MIN_TLS_ALIGN offsetof(struct builtin_tls, pt)

#define ADDEND_LIMIT 4096
static size_t *saved_addends, *apply_addends_to;

static struct dso ldso;
static struct dso *head, *tail, *fini_head, *syms_tail, *lazy_head;
static char *env_path, *sys_path;
static unsigned long long gencnt;
static int runtime;
static int ldd_mode;
static int ldso_fail;
static int noload;
static int shutting_down;
static jmp_buf *rtld_fail;
static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
static struct debug debug;
static struct tls_module *tls_tail;
static size_t tls_cnt, tls_offset, tls_align = MIN_TLS_ALIGN;
static size_t static_tls_cnt;
static pthread_mutex_t init_fini_lock;
static pthread_cond_t ctor_cond;
static struct dso *builtin_deps[2];
static struct dso *const no_deps[1];
static struct dso *builtin_ctor_queue[4];
static struct dso **main_ctor_queue;
static struct fdpic_loadmap *app_loadmap;
static struct fdpic_dummy_loadmap app_dummy_loadmap;
#define NR_NATIVE_LIBS 3
static struct dso *native_dso[NR_NATIVE_LIBS];

struct debug *_dl_debug_addr = &debug;

#ifdef _DEFAULT_MALLOC
extern hidden int __malloc_replaced;
#else
hidden int __malloc_replaced;
void __malloc_donate(char *start, char *end)
{
}
#endif

hidden void (*const __init_array_start)(void)=0, (*const __fini_array_start)(void)=0;

extern hidden void (*const __init_array_end)(void), (*const __fini_array_end)(void);

weak_alias(__init_array_start, __init_array_end);
weak_alias(__fini_array_start, __fini_array_end);

static int find_section_by_name(const char *name, Ehdr *eh, Shdr *sh,
				const char *shstrtab, size_t shstrtab_size)
{
	int ret = -1;
	Shdr *temp = NULL;
	int index = (int)(eh->e_shnum - 1);

	for (; index >= 0; index--) {
		temp = &sh[index];

		if (temp->sh_name >= shstrtab_size) {
			break;
		}

		if (strcmp(name, shstrtab + temp->sh_name) == 0) {
			ret = index;
			break;
		}
	}

	return ret;
}

static void map_symtab_and_strtab(struct dso *dso)
{
	dso->strtab_addr = (uint64_t)(uintptr_t)MAP_FAILED;
	dso->strtab_size = 0U;
}

static void generate_libc_syminfo(void)
{
	int fd;
	ssize_t size;
	Ehdr *eh = NULL;
	Ehdr buf[(896 + sizeof(Ehdr))/sizeof(Ehdr)];

	fd = open(ldso.name, O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		return;
	}

	size = read(fd, buf, sizeof(buf));
	if (size < 0) {
		close(fd);
		return;
	}

	eh = buf;
	if (size < sizeof(Ehdr) || (eh->e_type != ET_DYN && eh->e_type != ET_EXEC)) {
		close(fd);
		return;
	}

	map_symtab_and_strtab(&ldso);
	close(fd);
}

/* reserve for special use */
#define LIBC_NAME_LEN  32

#define AT_DSOINFO	(AT_L3_CACHEGEOMETRY + 1)
#define AT_LIBCMAPLEN	(AT_L3_CACHEGEOMETRY + 3)

static void set_dso_syminfo(size_t *auxv)
{
	char *addr = NULL;

	if (dso_syminfo != NULL) {
		dprintf(2, "ERROR: dso_sym_info has been initted\n");
		return;
	}
	for (; auxv[0]; auxv += 2) {
		if (auxv[0] == AT_DSOINFO) {
			addr = (char *)auxv[1];
		} else if (auxv[0] == AT_LIBCMAPLEN) {
			ldso.map_len = (size_t)auxv[1];
		}
	}
	if (addr == NULL) {
		return;
	}
	dso_syminfo = (struct dso_sym_info *)addr;
}

int fill_dso_syminfo(struct dso_sym_entry *sym_entry)
{
	int ret = -EINVAL;

	if (sym_entry != NULL) {
		pthread_rwlock_wrlock(&lock);
		ret = __add_dso_syminfo_nolock(sym_entry);
		pthread_rwlock_unlock(&lock);
	}

	return ret;
}

void drop_dso_syminfo(const void *base)
{
	if (base == NULL) {
		return;
	}
	pthread_rwlock_wrlock(&lock);
	__drop_dso_syminfo(base, NULL);
	pthread_rwlock_unlock(&lock);
}

static int dl_strcmp(const char *l, const char *r)
{
	for (; *l==*r && *l; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
}
#define strcmp(l,r) dl_strcmp(l,r)

/* Compute load address for a virtual address in a given dso. */
#if DL_FDPIC
static void *laddr(const struct dso *p, size_t v)
{
	size_t j=0;
	if (!p->loadmap) return p->base + v;
	for (j=0; v-p->loadmap->segs[j].p_vaddr >= p->loadmap->segs[j].p_memsz; j++);
	return (void *)(v - p->loadmap->segs[j].p_vaddr + p->loadmap->segs[j].addr);
}
static void *laddr_pg(const struct dso *p, size_t v)
{
	size_t j=0;
	size_t pgsz = PAGE_SIZE;
	if (!p->loadmap) return p->base + v;
	for (j=0; ; j++) {
		size_t a = p->loadmap->segs[j].p_vaddr;
		size_t b = a + p->loadmap->segs[j].p_memsz;
		a &= -pgsz;
		b += pgsz-1;
		b &= -pgsz;
		if (v-a<b-a) break;
	}
	return (void *)(v - p->loadmap->segs[j].p_vaddr + p->loadmap->segs[j].addr);
}
static void (*fdbarrier(void *p))()
{
	void (*fd)();
	__asm__("" : "=r"(fd) : "0"(p));
	return fd;
}
#define fpaddr(p, v) fdbarrier((&(struct funcdesc){ \
	laddr(p, v), (p)->got }))
#else
#define laddr(p, v) (void *)((p)->base + (v))
#define laddr_pg(p, v) laddr(p, v)
#define fpaddr(p, v) ((void (*)())laddr(p, v))
#endif

static void decode_vec(size_t *v, size_t *a, size_t cnt)
{
	size_t i;
	for (i=0; i<cnt; i++) a[i] = 0;
	for (; v[0]; v+=2) if (v[0]-1<cnt-1) {
		a[0] |= 1UL<<v[0];
		a[v[0]] = v[1];
	}
}

static int search_vec(size_t *v, size_t *r, size_t key)
{
	for (; v[0]!=key; v+=2)
		if (!v[0]) return 0;
	*r = v[1];
	return 1;
}

static uint32_t sysv_hash(const char *s0)
{
	const unsigned char *s = (void *)s0;
	uint_fast32_t h = 0;
	while (*s) {
		h = 16*h + *s++;
		h ^= h>>24 & 0xf0;
	}
	return h & 0xfffffff;
}

static uint32_t gnu_hash(const char *s0)
{
	const unsigned char *s = (void *)s0;
	uint_fast32_t h = 5381;
	for (; *s; s++)
		h += h*32 + *s;
	return h;
}

static Sym *sysv_lookup(const char *s, uint32_t h, struct dso *dso)
{
	size_t i;
	Sym *syms = dso->syms;
	Elf_Symndx *hashtab = dso->hashtab;
	char *strings = dso->strings;
	for (i=hashtab[2+h%hashtab[0]]; i; i=hashtab[2+hashtab[0]+i]) {
		if ((!dso->versym || dso->versym[i] >= 0)
		    && (!strcmp(s, strings+syms[i].st_name)))
			return syms+i;
	}
	return 0;
}

static Sym *gnu_lookup(uint32_t h1, uint32_t *hashtab, struct dso *dso, const char *s)
{
	uint32_t nbuckets = hashtab[0];
	uint32_t *buckets = hashtab + 4 + hashtab[2]*(sizeof(size_t)/4);
	uint32_t i = buckets[h1 % nbuckets];

	if (!i) return 0;

	uint32_t *hashval = buckets + nbuckets + (i - hashtab[1]);

	for (h1 |= 1; ; i++) {
		uint32_t h2 = *hashval++;
		if ((h1 == (h2|1)) && (!dso->versym || dso->versym[i] >= 0)
		    && !strcmp(s, dso->strings + dso->syms[i].st_name))
			return dso->syms+i;
		if (h2 & 1) break;
	}

	return 0;
}

static Sym *gnu_lookup_filtered(uint32_t h1, uint32_t *hashtab, struct dso *dso, const char *s, uint32_t fofs, size_t fmask)
{
	const size_t *bloomwords = (const void *)(hashtab+4);
	size_t f = bloomwords[fofs & (hashtab[2]-1)];
	if (!(f & fmask)) return 0;

	f >>= (h1 >> hashtab[3]) % (8 * sizeof f);
	if (!(f & 1)) return 0;

	return gnu_lookup(h1, hashtab, dso, s);
}

#define OK_TYPES (1<<STT_NOTYPE | 1<<STT_OBJECT | 1<<STT_FUNC | 1<<STT_COMMON | 1<<STT_TLS | 1<<STT_GNU_IFUNC)
#define OK_BINDS (1<<STB_GLOBAL | 1<<STB_WEAK | 1<<STB_GNU_UNIQUE)

#ifndef ARCH_SYM_REJECT_UND
#define ARCH_SYM_REJECT_UND(s) 0
#endif

struct sym_hook {
	sym_reloc_hook reloc_hook;
	void* reloc_ctx;
	sym_runtime_resolve_hook rt_resolve_hook;
};

static pthread_key_t sym_hook_key = PTHREAD_KEYS_MAX;
static pthread_rwlock_t hook_key_lock = PTHREAD_RWLOCK_INITIALIZER;

static inline bool __rlhook_ptr_is_valid(const void *ptr)
{
	/* valid reloc hook return ptr should not in zero-page */
	return ((unsigned long)ptr < (unsigned long)-RLHOOK_ERRNO_MAX)
		&& ((unsigned long)ptr > PAGE_SIZE);
}

static inline bool __rlhook_ptr_lazy_bind(const void *ptr)
{
	return (unsigned long)ptr == (unsigned long)-RLHOOK_ERR_LAZY_BIND;
}

static struct sym_hook *__sym_hook_get(void)
{
	struct sym_hook *hook = NULL;

	if (sym_hook_key < PTHREAD_KEYS_MAX) {
		hook = pthread_getspecific(sym_hook_key);
	}
	return hook;
}

static void __sym_reloc_exhook(const char *name, struct symdef *def, struct sym_hook *hook)
{
	static struct dso hook_dso = {0};
	static Sym exsym = {0};
	const Sym *sym = def->sym;
	void *st_value = NULL;

	st_value = hook->reloc_hook(hook->reloc_ctx, name,
				    (sym == NULL) ? NULL : (void *)sym->st_value);
	if (__rlhook_ptr_is_valid(st_value) || __rlhook_ptr_lazy_bind(st_value)) {
		/* switch to `base == 0` hook dso */
		def->dso = &hook_dso;
		exsym.st_value = (uintptr_t)st_value;
		def->sym = &exsym;
	}
}

#if defined(__GNUC__)
__attribute__((always_inline))
#endif
static inline struct symdef find_sym2(struct dso *dso, const char *s, int need_def, int use_deps)
{
	uint32_t h = 0, gh = gnu_hash(s), gho = gh / (8*sizeof(size_t)), *ght;
	size_t ghm = 1ul << gh % (8*sizeof(size_t));
	struct symdef def = {0};
	struct dso **deps = use_deps ? dso->deps : 0;
	struct sym_hook *hook = NULL;

	for (; dso; dso=use_deps ? *deps++ : dso->syms_next) {
		Sym *sym;
		if ((ght = dso->ghashtab)) {
			sym = gnu_lookup_filtered(gh, ght, dso, s, gho, ghm);
		} else {
			if (!h) h = sysv_hash(s);
			sym = sysv_lookup(s, h, dso);
		}
		if (!sym) continue;
		if (!sym->st_shndx)
			if (need_def || (sym->st_info&0xf) == STT_TLS
			    || ARCH_SYM_REJECT_UND(sym))
				continue;
		if (!sym->st_value)
			if ((sym->st_info&0xf) != STT_TLS)
				continue;
		if (!(1<<(sym->st_info&0xf) & OK_TYPES)) continue;
		if (!(1<<(sym->st_info>>4) & OK_BINDS)) continue;
		def.sym = sym;
		def.dso = dso;
		break;
	}

	hook = __sym_hook_get();
	/* hook only UND symbol and self symbol */
	if ((hook != NULL) && ((def.sym == NULL) || (def.dso == dso))) {
		__sym_reloc_exhook(s, &def, hook);
	}

	return def;
}

static struct symdef find_sym(struct dso *dso, const char *s, int need_def)
{
	return find_sym2(dso, s, need_def, 0);
}

static struct symdef find_sym_ex(struct dso *dso, const char *s, int type)
{
	struct symdef def = {0};
	if ((dso->flags & DSO_FLAGS_DEEPBIND) == DSO_FLAGS_DEEPBIND) {
		/*
		 * If DEEPBIND is set, resolve symbols from dso itself first.
		 * Check the DEEPBIND section of dlopen man-page.
		 */
		def = find_sym(dso, s, type==REL_PLT);
	}

	if (def.sym == NULL) {
		struct dso *ctx = (type == REL_COPY) ? head->syms_next : head;
		def = find_sym(ctx, s, type==REL_PLT);
	}
	return def;
}

static void reloc_dso(struct dso *p);

static void do_relocs(struct dso *dso, size_t *rel, size_t rel_size, size_t stride)
{
	unsigned char *base = dso->base;
	Sym *syms = dso->syms;
	char *strings = dso->strings;
	Sym *sym;
	const char *name;
	int type;
	int sym_index;
	struct symdef def;
	size_t *reloc_addr;
	size_t sym_val;
	size_t tls_val;
	size_t addend;
	int skip_relative = 0, reuse_addends = 0, save_slot = 0;

	if (dso == &ldso) {
		/* Only ldso's REL table needs addend saving/reuse. */
		if (rel == apply_addends_to)
			reuse_addends = 1;
		skip_relative = 1;
	}

	for (; rel_size; rel+=stride, rel_size-=stride*sizeof(size_t)) {
		if (skip_relative && IS_RELATIVE(rel[1], dso->syms)) continue;
		type = R_TYPE(rel[1]);
		if (type == REL_NONE) continue;
		reloc_addr = laddr(dso, rel[0]);

		if (stride > 2) {
			addend = rel[2];
		} else if (type==REL_GOT || type==REL_PLT|| type==REL_COPY) {
			addend = 0;
		} else if (reuse_addends) {
			/* Save original addend in stage 2 where the dso
			 * chain consists of just ldso; otherwise read back
			 * saved addend since the inline one was clobbered. */
			if (head==&ldso)
				saved_addends[save_slot] = *reloc_addr;
			addend = saved_addends[save_slot++];
		} else {
			addend = *reloc_addr;
		}

		sym_index = R_SYM(rel[1]);
		if (sym_index) {
			sym = syms + sym_index;
			name = strings + sym->st_name;
			def = (sym->st_info>>4) == STB_LOCAL
				? (struct symdef){ .dso = dso, .sym = sym }
				: find_sym_ex(dso, name, type);
			if ((def.sym) && (type == REL_PLT)) {
				if ((def.sym->st_info&0xf) == STT_GNU_IFUNC) {
					/* It may contain copy relocations which depend on libraries's relocations. */
					reloc_dso(def.dso);
					*reloc_addr = (size_t)laddr(def.dso, def.sym->st_value) + addend;
					*reloc_addr = ((size_t(*)(void))*reloc_addr)();
					continue;
				}
				if (__rlhook_ptr_lazy_bind((void *)(uintptr_t)def.sym->st_value)) {
					/*
					 * relocate initial .got.rel
					 *
					 * [IHI0056B_aaelf64.pdf (pp.25)]
					 * R_AARCH64_JUMP_SLOT
					 * The initial value stored in the place is the offset to the
					 * entry sequence stub for the dynamic linker.
					 * It must be adjusted during initial loading by the offset of
					 * the load address of the segment from its link address.
					 */
					*reloc_addr = (size_t)laddr(dso, *reloc_addr);
					continue;
				}
			}

			if (!def.sym && (sym->st_shndx != SHN_UNDEF
			    || sym->st_info>>4 != STB_WEAK)) {
				if (dso->lazy && (type==REL_PLT || type==REL_GOT)) {
					dso->lazy[3*dso->lazy_cnt+0] = rel[0];
					dso->lazy[3*dso->lazy_cnt+1] = rel[1];
					dso->lazy[3*dso->lazy_cnt+2] = addend;
					dso->lazy_cnt++;
					continue;
				}
				error("Error relocating %s: %s: symbol not found",
					dso->name, name);
				if (runtime) longjmp(*rtld_fail, 1);
				continue;
			}
		} else {
			sym = 0;
			def.sym = 0;
			def.dso = dso;
		}

		sym_val = def.sym ? (size_t)laddr(def.dso, def.sym->st_value) : 0;
		tls_val = def.sym ? def.sym->st_value : 0;

		if ((type == REL_TPOFF || type == REL_TPOFF_NEG)
		    && def.dso->tls_id > static_tls_cnt) {
			error("Error relocating %s: %s: initial-exec TLS "
				"resolves to dynamic definition in %s",
				dso->name, name, def.dso->name);
			longjmp(*rtld_fail, 1);
		}

		switch(type) {
		case REL_OFFSET:
			addend -= (size_t)reloc_addr;
		case REL_SYMBOLIC:
		case REL_GOT:
		case REL_PLT:
			*reloc_addr = sym_val + addend;
			break;
		case REL_USYMBOLIC:
			memcpy(reloc_addr, &(size_t){sym_val + addend}, sizeof(size_t));
			break;
		case REL_RELATIVE:
			*reloc_addr = (size_t)base + addend;
			break;
		case REL_SYM_OR_REL:
			if (sym) *reloc_addr = sym_val + addend;
			else *reloc_addr = (size_t)base + addend;
			break;
		case REL_COPY:
			memcpy(reloc_addr, (void *)sym_val, sym->st_size);
			break;
		case REL_OFFSET32:
			*(uint32_t *)reloc_addr = sym_val + addend
				- (size_t)reloc_addr;
			break;
		case REL_FUNCDESC:
			*reloc_addr = def.sym ? (size_t)(def.dso->funcdescs
				+ (def.sym - def.dso->syms)) : 0;
			break;
		case REL_FUNCDESC_VAL:
			if ((sym->st_info&0xf) == STT_SECTION) *reloc_addr += sym_val;
			else *reloc_addr = sym_val;
			reloc_addr[1] = def.sym ? (size_t)def.dso->got : 0;
			break;
		case REL_DTPMOD:
			*reloc_addr = def.dso->tls_id;
			break;
		case REL_DTPOFF:
			*reloc_addr = tls_val + addend - DTP_OFFSET;
			break;
#ifdef TLS_ABOVE_TP
		case REL_TPOFF:
			*reloc_addr = tls_val + def.dso->tls.offset + TPOFF_K + addend;
			break;
#else
		case REL_TPOFF:
			*reloc_addr = tls_val - def.dso->tls.offset + addend;
			break;
		case REL_TPOFF_NEG:
			*reloc_addr = def.dso->tls.offset - tls_val + addend;
			break;
#endif
		case REL_TLSDESC:
			if (stride<3) addend = reloc_addr[1];
			if (def.dso->tls_id > static_tls_cnt) {
				struct td_index *new = malloc(sizeof *new);
				if (!new) {
					error(
					"Error relocating %s: cannot allocate TLSDESC for %s",
					dso->name, sym ? name : "(local)" );
					longjmp(*rtld_fail, 1);
				}
				new->next = dso->td_index;
				dso->td_index = new;
				new->args[0] = def.dso->tls_id;
				new->args[1] = tls_val + addend - DTP_OFFSET;
				reloc_addr[0] = (size_t)__tlsdesc_dynamic;
				reloc_addr[1] = (size_t)new;
			} else {
				reloc_addr[0] = (size_t)__tlsdesc_static;
#ifdef TLS_ABOVE_TP
				reloc_addr[1] = tls_val + def.dso->tls.offset
					+ TPOFF_K + addend;
#else
				reloc_addr[1] = tls_val - def.dso->tls.offset
					+ addend;
#endif
			}
#ifdef TLSDESC_BACKWARDS
			/* Some archs (32-bit ARM at least) invert the order of
			 * the descriptor members. Fix them up here. */
			size_t tmp = reloc_addr[0];
			reloc_addr[0] = reloc_addr[1];
			reloc_addr[1] = tmp;
#endif
			break;
		case REL_IRELATIVE:
			*reloc_addr = (size_t)base + addend;
			*reloc_addr = ((size_t(*)(void))*reloc_addr)();
			break;
		default:
			error("Error relocating %s: unsupported relocation type %d",
				dso->name, type);
			if (runtime) longjmp(*rtld_fail, 1);
			continue;
		}
	}
}

static void redo_lazy_relocs()
{
	struct dso *p = lazy_head, *next;
	lazy_head = 0;
	for (; p; p=next) {
		next = p->lazy_next;
		size_t size = p->lazy_cnt*3*sizeof(size_t);
		p->lazy_cnt = 0;
		do_relocs(p, p->lazy, size, 3);
		if (p->lazy_cnt) {
			p->lazy_next = lazy_head;
			lazy_head = p;
		} else {
			free(p->lazy);
			p->lazy = 0;
			p->lazy_next = 0;
		}
	}
}

/* A huge hack: to make up for the wastefulness of shared libraries
 * needing at least a page of dirty memory even if they have no global
 * data, we reclaim the gaps at the beginning and end of writable maps
 * and "donate" them to the heap. */

static void reclaim(struct dso *dso, size_t start, size_t end)
{
	if (start >= dso->relro_start && start < dso->relro_end) start = dso->relro_end;
	if (end   >= dso->relro_start && end   < dso->relro_end) end = dso->relro_start;
	if (start >= end) return;
	char *base = laddr_pg(dso, start);
	__malloc_donate(base, base+(end-start));
}

static void reclaim_gaps(struct dso *dso)
{
	Phdr *ph = dso->phdr;
	size_t phcnt = dso->phnum;

	for (; phcnt--; ph=(void *)((char *)ph+dso->phentsize)) {
		if (ph->p_type!=PT_LOAD) continue;
		if ((ph->p_flags&(PF_R|PF_W))!=(PF_R|PF_W)) continue;
		reclaim(dso, ph->p_vaddr & -PAGE_SIZE, ph->p_vaddr);
		reclaim(dso, ph->p_vaddr+ph->p_memsz,
			ph->p_vaddr+ph->p_memsz+PAGE_SIZE-1 & -PAGE_SIZE);
	}
}

static ssize_t read_loop(int fd, void *p, size_t n)
{
	for (size_t i=0; i<n; ) {
		ssize_t l = read(fd, (char *)p+i, n-i);
		if (l<0) {
			if (errno==EINTR) continue;
			else return -1;
		}
		if (l==0) return i;
		i += l;
	}
	return n;
}

extern int is_loading_ldk;

#define MB(n)		(1024L*1024L*(n))

#ifndef align_up
#define align_up(x, a) ({typeof(x) __mask = (typeof(x))(a) - (typeof(x))1;\
			(((x) + (__mask)) & ~(__mask));})
#endif

#ifndef align_down
#define align_down(x, a) ({typeof(x) __mask = (typeof(x))(a) - (typeof(x))1;\
			  ((x) & ~(__mask));})
#endif

#ifndef PR_SET_VMA
#define PR_SET_VMA 0x53564d41
#endif

#ifndef PR_SET_VMA_ANON_NAME
#define PR_SET_VMA_ANON_NAME 0
#endif

#define __PROT_MASK ((uint32_t)PROT_READ | (uint32_t)PROT_WRITE | (uint32_t)PROT_EXEC | \
                     (uint32_t)PROT_GROWSUP | (uint32_t)PROT_GROWSDOWN)

static void *mmap_fixed(void *p, size_t n, int prot, int flags, int fd, off_t off)
{
	unsigned long addr_start = (unsigned long)p;
	static int no_map_fixed;
	char *q;
	int orig_fd = -1, orig_off = -1, orig_prot = prot;
	int orig_n = n;

	if (!n) return p;

	if ((prot & (PROT_READ|PROT_WRITE|PROT_EXEC)) &&
				!(addr_start & (MB(2)-1)) &&
					 !(n & (MB(2)-1)) ) {
		dprintf(2, "ldk-huge-mappings: mmap_fixed %p-%p (%lx) prot %s%s%s flags %x fd %d off %ld\n",
				p, (char*)p+n, n,
				(prot & PROT_READ) ? "r" : "-",
				(prot & PROT_WRITE) ? "w" : "-",
				(prot & PROT_EXEC) ? "x" :"-",
				flags, fd, off);

		/* we use anonymous 2MB huge mapping, so we need write protection right
		 * to fill up data from file system.
		 */
		orig_fd = fd;
		orig_off = off;
		orig_prot = prot;

		prot |= PROT_WRITE;
		flags |= MAP_PRIVATE|MAP_HUGE_2MB|MAP_ANONYMOUS|MAP_POPULATE|MAP_HUGETLB;
		fd = -1;
		off = 0;
	}

	if (!no_map_fixed) {
		q = mmap(p, n, prot, flags|MAP_FIXED, fd, off);
		if (!DL_NOMMU_SUPPORT || q != MAP_FAILED || errno != EINVAL) {
			if ((flags & MAP_HUGETLB) && (q != MAP_FAILED)) {
				if (orig_fd >= 0) {
					fd = orig_fd;
					off = orig_off;
					goto load_file;
				}
				goto fixed_prot;
			}
			return q;
		}
		no_map_fixed = 1;
	}
	/* Fallbacks for MAP_FIXED failure on NOMMU kernels. */
	if (flags & MAP_ANONYMOUS) {
		memset(p, 0, n);
		return p;
	}
	ssize_t r;
load_file:
	if (lseek(fd, off, SEEK_SET) < 0) return MAP_FAILED;
	for (q=p; n; q+=r, off+=r, n-=r) {
		r = read(fd, q, n);
		if (r < 0 && errno != EINTR) return MAP_FAILED;
		if (!r) {
			memset(q, 0, n);
			break;
		}
	}
fixed_prot:
	if (orig_prot != prot) {
		if (orig_prot & PROT_EXEC)
			(void)prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, p, orig_n, g_filename_bak);
		if (0 != mprotect((void*)p, orig_n, ((uint32_t)orig_prot & __PROT_MASK)))
			return MAP_FAILED;
	}
	return p;
}

static void unmap_library(struct dso *dso)
{
	__drop_dso_syminfo(dso->map, dso);
	if (dso->loadmap) {
		size_t i;
		for (i=0; i<dso->loadmap->nsegs; i++) {
			if (!dso->loadmap->segs[i].p_memsz)
				continue;
			munmap((void *)dso->loadmap->segs[i].addr,
				dso->loadmap->segs[i].p_memsz);
		}
		free(dso->loadmap);
	} else if (dso->map && dso->map_len) {
		munmap(dso->map, dso->map_len);
	}
}

static void decode_dyn(struct dso *p);

#ifdef CONFIG_ARM64_BTI_USER
/* MPROT_BTI keep same with sysmgr */
#define MPROT_BTI 0x20
static uint32_t parse_elf_property(uint32_t type, const char* data)
{
    uint32_t prot = 0;
    if (type == GNU_PROPERTY_AARCH64_FEATURE_1_AND) {
        const uint32_t *p = data;
        if((*p & GNU_PROPERTY_AARCH64_FEATURE_1_BTI) != 0) {
            prot |= MPROT_BTI;
        }
    }
    return prot;
}
static uint32_t parse_prot(const char *data, ssize_t *off, ssize_t datasz)
{
    ssize_t o;
    const struct gnu_property *pr;
    uint32_t ret;
    o = *off;
    ssize_t sz = datasz - *off;

    if(sz < sizeof(*pr))
        return 0;
    pr = (const struct gnu_property *)(data + o);
    o += sizeof(*pr);
    if (pr->pr_datasz > sz)
        return 0;
    ret = parse_elf_property(pr->pr_type, data + o);
    return ret;
}

static unsigned parse_extra_prot_fd(int fd, Phdr *ph)
{
    union {
        Elf64_Nhdr nh;
        char data[0x400];
    } gnu_data;
    ssize_t len = pread(fd, gnu_data.data, ph->p_offset, ph->p_filesz);
    if (len < 0) return 0;
    if (gnu_data.nh.n_type != NT_GNU_PROPERTY_TYPE_0 || gnu_data.nh.n_type != NOTE_NAME_SZ) {
        return 0;
    }

    ssize_t off_gp = (sizeof(gnu_data.nh)) + NOTE_NAME_SZ;
    off_gp = (off_gp + ELF_GNU_PROPERTY_ALIGN - 1) & (-(ELF_GNU_PROPERTY_ALIGN));
    ssize_t datasz_gp = off_gp + gnu_data.nh.n_descsz;
    return parse_prot((char *)gnu_data.data, &off_gp, datasz_gp);
}

static unsigned parse_extra_prot_mem(const char *mem, Phdr *ph)
{
    union {
        Elf64_Nhdr nh;
        char data[0x400];
    } gnu_data;

	memcpy(gnu_data.data, (const char *)mem + ph->p_offset, ph->p_filesz);
    if (gnu_data.nh.n_type != NT_GNU_PROPERTY_TYPE_0 || gnu_data.nh.n_type != NOTE_NAME_SZ) {
        return 0;
    }
    ssize_t off_gp = (sizeof(gnu_data.nh)) + NOTE_NAME_SZ;
    off_gp = (off_gp + ELF_GNU_PROPERTY_ALIGN - 1) & (-(ELF_GNU_PROPERTY_ALIGN));
    ssize_t datasz_gp = off_gp + gnu_data.nh.n_descsz;
    return parse_prot((char *)gnu_data.data, &off_gp, datasz_gp);
}
#endif

static void *map_library(int fd, struct dso *dso)
{
	Ehdr buf[(896+sizeof(Ehdr))/sizeof(Ehdr)];
	void *allocated_buf=0;
	size_t phsize;
	size_t addr_min=SIZE_MAX, addr_max=0, map_len;
	size_t this_min, this_max, this_file_max;
	size_t nsegs = 0;
	off_t off_start = 0;
	Ehdr *eh;
	Phdr *ph, *ph0, *last_load_ph;
#ifdef CONFIG_HMKIP
	void *se_buf = 0;
#endif
	unsigned prot;
#ifdef CONFIG_ARM64_BTI_USER
	unsigned ext_prot = 0;
#endif
	unsigned char *map=MAP_FAILED, *base;
	unsigned char *base_this_min_ro = NULL;
	unsigned char *dso_str_addr = NULL;
	bool load_huge_ldk = false;
	unsigned dso_str_prot = 0U;
	size_t dso_str_len = 0UL;
	unsigned char *this_file_max_ro = NULL;
	off_t off_start_ro = (off_t)0U;
	size_t dyn=0;
	size_t tls_image=0;
	size_t i;
	int flags = 0;
	ssize_t l = read(fd, buf, sizeof buf);

	eh = buf;
	if (l<0) return 0;
	if (l<sizeof *eh || (eh->e_type != ET_DYN && eh->e_type != ET_EXEC))
		goto noexec;
	phsize = eh->e_phentsize * eh->e_phnum;
	if (phsize > sizeof buf - sizeof *eh) {
		allocated_buf = malloc(phsize);
		if (!allocated_buf) return 0;
		l = pread(fd, allocated_buf, phsize, eh->e_phoff);
		if (l < 0) goto error;
		if (l != phsize) goto noexec;
		ph = ph0 = allocated_buf;
	} else if (eh->e_phoff + phsize > l) {
		l = pread(fd, buf+1, phsize, eh->e_phoff);
		if (l < 0) goto error;
		if (l != phsize) goto noexec;
		ph = ph0 = (void *)(buf + 1);
	} else {
		ph = ph0 = (void *)((char *)buf + eh->e_phoff);
	}
	last_load_ph = NULL;
	for (i=eh->e_phnum; i; i--, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type == PT_DYNAMIC) {
			dyn = ph->p_vaddr;
		} else if (ph->p_type == PT_TLS) {
			tls_image = ph->p_vaddr;
			dso->tls.align = ph->p_align;
			dso->tls.len = ph->p_filesz;
			dso->tls.size = ph->p_memsz;
		} else if (ph->p_type == PT_GNU_RELRO) {
			dso->relro_start = ph->p_vaddr & -PAGE_SIZE;
			dso->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
		} else if (ph->p_type == PT_GNU_STACK) {
			if (!runtime && ph->p_memsz > __default_stacksize) {
				__default_stacksize =
					ph->p_memsz < DEFAULT_STACK_MAX ?
					ph->p_memsz : DEFAULT_STACK_MAX;
			}
		}
#ifdef CONFIG_HMKIP
		if (ph->p_type == PT_HKIP) {
			size_t hkip_sz = (size_t)ph->p_filesz;
			se_buf = malloc(hkip_sz);
			if (!se_buf) goto noexec;
			l = pread(fd, se_buf, hkip_sz, ph->p_offset);
			if (l < 0) goto error;
			if (l != hkip_sz) goto noexec;
			unsigned long *hkip_seg = se_buf;
			if (hkip_seg[ZERO] != 0) {
				dso->enable_hkip = true;
			}
			if (dso->enable_hkip) {
				dso->roafinit_start = hkip_seg[ONE];
				dso->roafinit_end = hkip_seg[TWO];
			}
		}
#endif
#ifdef CONFIG_ARM64_BTI_USER
		if (ph->p_type == PT_GNU_PROPERTY || ph->p_type == PT_NOTE)
					ext_prot = parse_extra_prot_fd(fd, ph);
#endif
		if (ph->p_type != PT_LOAD) continue;
		nsegs++;
		if (ph->p_vaddr < addr_min) {
			addr_min = ph->p_vaddr;
			off_start = ph->p_offset;
			prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
				((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
				((ph->p_flags&PF_X) ? PROT_EXEC : 0));
#ifdef CONFIG_ARM64_BTI_USER
			prot |= ext_prot;
#endif
		}
		if (ph->p_vaddr+ph->p_memsz > addr_max) {
			addr_max = ph->p_vaddr+ph->p_memsz;
		}

		if (is_loading_ldk && last_load_ph &&
			!(ph->p_vaddr & (MB(2)-1)) &&
			!(last_load_ph->p_vaddr & (MB(2)-1)) &&
			(last_load_ph->p_flags & (PF_R|PF_W|PF_X))) {
			if (last_load_ph->p_memsz > MB(6) && last_load_ph->p_align < MB(2)) {
				last_load_ph->p_align = MB(2);
			}
		}
		last_load_ph = ph;
	}
	if (is_loading_ldk && last_load_ph &&
			!(last_load_ph->p_vaddr & (MB(2)-1)) &&
			(last_load_ph->p_flags & (PF_R|PF_W|PF_X))) {
		if (last_load_ph->p_memsz > MB(6) && last_load_ph->p_align < MB(2)) {
			last_load_ph->p_align = MB(2);
		}
	}

	if (!dyn) goto noexec;
	if (DL_FDPIC && !(eh->e_flags & FDPIC_CONSTDISP_FLAG)) {
		dso->loadmap = calloc(1, sizeof *dso->loadmap
			+ nsegs * sizeof *dso->loadmap->segs);
		if (!dso->loadmap) goto error;
		dso->loadmap->nsegs = nsegs;
		for (ph=ph0, i=0; i<nsegs; ph=(void *)((char *)ph+eh->e_phentsize)) {
			if (ph->p_type != PT_LOAD) continue;
			prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
				((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
				((ph->p_flags&PF_X) ? PROT_EXEC : 0));
#ifdef CONFIG_ARM64_BTI_USER
			prot |= ext_prot;
#endif
			map = mmap(0, ph->p_memsz + (ph->p_vaddr & PAGE_SIZE-1),
				prot, MAP_PRIVATE,
				fd, ph->p_offset & -PAGE_SIZE);
			if (map == MAP_FAILED) {
				unmap_library(dso);
				goto error;
			}
			dso->loadmap->segs[i].addr = (size_t)map +
				(ph->p_vaddr & PAGE_SIZE-1);
			dso->loadmap->segs[i].p_vaddr = ph->p_vaddr;
			dso->loadmap->segs[i].p_memsz = ph->p_memsz;
			i++;
			if (prot & PROT_WRITE) {
				size_t brk = (ph->p_vaddr & PAGE_SIZE-1)
					+ ph->p_filesz;
				size_t pgbrk = brk + PAGE_SIZE-1 & -PAGE_SIZE;
				size_t pgend = brk + ph->p_memsz - ph->p_filesz
					+ PAGE_SIZE-1 & -PAGE_SIZE;
				if (pgend > pgbrk && mmap_fixed(map+pgbrk,
					pgend-pgbrk, prot,
					MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,
					-1, off_start) == MAP_FAILED)
					goto error;
				memset(map + brk, 0, pgbrk-brk);
			}
		}
		map = (void *)dso->loadmap->segs[0].addr;
		map_len = 0;
		goto done_mapping;
	}
	addr_max += PAGE_SIZE-1;
	addr_max &= -PAGE_SIZE;
	addr_min &= -PAGE_SIZE;
	off_start &= -PAGE_SIZE;
	map_len = addr_max - addr_min + off_start;
	/* The first time, we map too much, possibly even more than
	 * the length of the file. This is okay because we will not
	 * use the invalid part; we just need to reserve the right
	 * amount of virtual address space to map over later. */

	/* Now demand paging is not supported on hongmeng, so we need to
	 * use PROT_NONE flag to reserve virtual memory in process.
	 * When demand paging is supported, remove this fix. */
	if (is_loading_ldk) {
		size_t orig_map_len = map_len;
		char *orig_map;

		map_len = align_up(map_len + MB(2), MB(2));

		orig_map = DL_NOMMU_SUPPORT
			? mmap((void *)(addr_min), map_len, PROT_READ|PROT_WRITE|PROT_EXEC,
				MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)
			: mmap((void *)(addr_min), map_len, PROT_NONE,
				MAP_PRIVATE, fd, off_start);

		map = (void*)align_up(((unsigned long)orig_map), MB(2));
		map_len -= (size_t)map - (size_t)orig_map;

		dprintf(2, "ldk-huge-mappings: mmap-fixed total return %p-%p require %p-%p map-len %lx->%lx map %p->%p\n",
					(void*)map, (char*)map + map_len,
					(void*)addr_min, (char*)addr_min + map_len,
					orig_map_len, map_len,
					orig_map, map);

	} else {
		map = DL_NOMMU_SUPPORT
			? mmap((void *)(addr_min), map_len, PROT_READ|PROT_WRITE|PROT_EXEC,
				MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)
			: mmap((void *)addr_min, map_len, PROT_NONE,
				MAP_PRIVATE, fd, off_start);
	}

	if (map==MAP_FAILED) goto error;
	dso->map = map;
	dso->map_len = map_len;
	/* If the loaded file is not relocatable and the requested address is
	 * not available, then the load operation must fail. */
	if (eh->e_type != ET_DYN && addr_min && map!=(void *)addr_min) {
		errno = EBUSY;
		goto error;
	}
	base = map - addr_min;
	dso->phdr = 0;
	dso->phnum = 0;
	dso->base = base;
	dso->dynv = laddr(dso, dyn);
	for (ph=ph0, i=eh->e_phnum; i; i--, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type != PT_LOAD) continue;
		/* Check if the programs headers are in this load segment, and
		 * if so, record the address for use by dl_iterate_phdr. */
		if (!dso->phdr && eh->e_phoff >= ph->p_offset
		    && eh->e_phoff+phsize <= ph->p_offset+ph->p_filesz) {
			dso->phdr = (void *)(base + ph->p_vaddr
				+ (eh->e_phoff-ph->p_offset));
			dso->phnum = eh->e_phnum;
			dso->phentsize = eh->e_phentsize;
		}
		this_min = ph->p_vaddr & -PAGE_SIZE;
		this_max = ph->p_vaddr+ph->p_memsz+PAGE_SIZE-1 & -PAGE_SIZE;
		off_start = ph->p_offset & -PAGE_SIZE;
		this_file_max = ph->p_vaddr+ph->p_filesz+PAGE_SIZE-1 & -PAGE_SIZE;
		prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
			((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
			((ph->p_flags&PF_X) ? PROT_EXEC : 0));
	    flags = MAP_PRIVATE | MAP_FIXED;
#ifdef CONFIG_HMKIP
		if (dso->enable_hkip) {
			if (dso->roafinit_start < dso->roafinit_end &&
				this_min <= dso->roafinit_start && this_max >= dso->roafinit_end)
				prot |= PROT_HKIP;
			if (this_min <= dso->relro_start && this_max >= dso->relro_end) {
				if (this_min < dso->relro_start && mmap_fixed(base + this_min,
						dso->relro_start - this_min, (int)prot, flags, fd,
						off_start) == MAP_FAILED)
					goto error;
				/* split relro from rw data to protect */
				if (mmap_fixed(base + dso->relro_start, dso->relro_end - dso->relro_start,
						(int)(prot | PROT_HKIP), flags, fd,
						off_start + dso->relro_start - this_min) == MAP_FAILED)
					goto error;
				if (this_max > dso->relro_end && mmap_fixed(base + dso->relro_end,
						this_file_max - dso->relro_end, (int)prot, flags, fd,
						off_start + dso->relro_end - this_min) == MAP_FAILED)
					goto error;
 
				goto after_mmap;
			}
			if ((prot & PROT_EXEC) == PROT_EXEC)
				prot |= PROT_HKIP;
			if (((prot & PROT_READ) == PROT_READ) &&
				((prot & PROT_EXEC) == 0) &&
				((prot & PROT_WRITE) == 0))
				prot |= PROT_HKIP;
			}
#endif
		/* On Hongmeng:
		 *   Now native service don't turn on demand-paging for file mapping, so if we map the
		 *   whole section to memory, will populate all the pages from vfs.
		 *   But actually file may be not that large and E_HM_NXIO will be reported.
		 *
		 *   So here we need to map the filesize to memory first and map the rest pages as ANONYMOUS. */
		void *rc;
		if ((ph->p_align == MB(2))) {
			size_t n;

			n = this_file_max-this_min;
			if (dso->relro_end == ((ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE))
				dso->relro_end = ph->p_vaddr + align_up(ph->p_memsz, MB(2));
			n = align_up(n, MB(2));
			rc = mmap_fixed(base+this_min, n, (int)prot, flags, fd, off_start);
		} else {
			rc = mmap_fixed(base+this_min, this_file_max-this_min, (int)prot, flags, fd, off_start);
		}
		if (rc == MAP_FAILED)
			goto error;
#ifdef CONFIG_HMKIP
after_mmap:
#endif
		if (ph->p_memsz > ph->p_filesz) {
			size_t brk = (size_t)base+ph->p_vaddr+ph->p_filesz;
			size_t pgbrk = brk+PAGE_SIZE-1 & -PAGE_SIZE;

			if (ph->p_flags & PF_W) {
				memset((void *)brk, 0, pgbrk-brk & PAGE_SIZE-1);
			}
			if (ph->p_align == MB(2)) {
				/* the the ending of last segment is used for BSS segment, and */
				/* our anonymous huge mappings don't be zeroed by sysmgr */
				memset((void*)brk, 0, align_up(pgbrk, MB(2)) - brk);
				pgbrk = align_up(pgbrk, MB(2));
				this_max = align_up(this_max, MB(2));
			}
			if (pgbrk-(size_t)base < this_max) {
				void *p;

				p = mmap_fixed((void *)pgbrk, (size_t)base+this_max-pgbrk,
						(int)prot, (int)((unsigned int)flags|MAP_ANONYMOUS), -1, 0);
				if (p == MAP_FAILED) {
					goto error;
				} else if (ph->p_align == MB(2) && (prot & PROT_WRITE)) {
					/* the anonymous huge mappings don't be zeroed by sysmgr */
					memset((void *)pgbrk, 0, (size_t)base+this_max-pgbrk);
				}
			}
		}
		/*
		 * Here records the the base and this_min of the first Read-only PT_LOAD range which
		 * can be used to locate the mapping address and offset of the dso->strings
		 * so that the strings range can be directly populate mmapped to reduce
		 * potential page fault caused by accessing strings.
		 */
		if (((ph->p_flags & PF_W) == 0U) && (base_this_min_ro == NULL)) {
			dso_str_prot = prot;
			base_this_min_ro = base + this_min;
			off_start_ro = off_start;
			this_file_max_ro = base + this_file_max;
			load_huge_ldk = is_loading_ldk && (ph->p_align == MB(2));
		}
	}
	decode_dyn(dso);
	/*
	 * Populate string tables for dso to avoid file page faults.
	 * Specifically, ldso module only pre-populates large-size string tables
	 * whose size is greater than two pages.
	 * Only large string tables are populated here because the overhead of populating
	 * small-size string tables is similar to that of handling page faults.
	 */
#ifdef CONFIG_HMKIP
	if (!load_huge_ldk && (dso->dynstr_size > (PAGE_SIZE + PAGE_SIZE)) && !dso->enable_hkip) {
#else
	if (!load_huge_ldk && (dso->dynstr_size > (PAGE_SIZE + PAGE_SIZE))) {
#endif
		if (((uintptr_t)dso->strings >= (uintptr_t)base_this_min_ro) &&
		    (((uintptr_t)dso->strings + dso->dynstr_size) <= (uintptr_t)this_file_max_ro)) {
			/*
			 * Note: current dynstr section pre-populating is designed to save overhead
			 * of handling page fault when relocating the relative so files.
			 * It uses PAGE_SIZE as alignment granularity rather than MB(2), due to
			 * large range may covering too many sections on which it can introduce
			 * addition mapping overhead to populate memory. Besize, ignoring the return
			 * value of this madvise is ok because willneed option may failed on distinct
			 * reasons(such as critical system memory status) and it is necessary to retry
			 * page fault to load its dynamic string table.
			 */
			dso_str_addr = (unsigned char *)align_down((uintptr_t)dso->strings, PAGE_SIZE);
			dso_str_len = align_up(((uintptr_t)dso->strings + dso->dynstr_size), PAGE_SIZE) - (uintptr_t)dso_str_addr;
			(void)madvise(dso_str_addr, dso_str_len, MADV_WILLNEED);
		}
	}

	for (i=0; ((size_t *)(base+dyn))[i]; i+=2)
		if (((size_t *)(base+dyn))[i]==DT_TEXTREL) {
			if (mprotect(map, map_len, PROT_READ|PROT_WRITE|PROT_EXEC)
			    && errno != ENOSYS)
				goto error;
			break;
		}

	map_symtab_and_strtab(dso);
done_mapping:
	if (dso->tls.size) dso->tls.image = laddr(dso, tls_image);
#ifdef CONFIG_HMKIP
	if (se_buf) free(se_buf);
#endif
	if (allocated_buf) free(allocated_buf);
	return map;
noexec:
	errno = ENOEXEC;
error:
	if (map!=MAP_FAILED) unmap_library(dso);
#ifdef CONFIG_HMKIP
	if (se_buf) free(se_buf);
#endif
	if (allocated_buf) free(allocated_buf);
	return 0;
}

static int path_open(const char *name, const char *s, char *buf, size_t buf_size)
{
	size_t l;
	int fd;
	for (;;) {
		s += strspn(s, ":\n");
		l = strcspn(s, ":\n");
		if (l-1 >= INT_MAX) return -1;
		if (snprintf(buf, buf_size, "%.*s/%s", (int)l, s, name) < buf_size) {
			if ((fd = open(buf, O_RDONLY|O_CLOEXEC))>=0) return fd;
			switch (errno) {
			case ENOENT:
			case ENOTDIR:
			case EACCES:
			case ENAMETOOLONG:
				break;
			default:
				/* Any negative value but -1 will inhibit
				 * futher path search. */
				return -2;
			}
		}
		s += l;
	}
}

static int fixup_rpath(struct dso *p, char *buf, size_t buf_size)
{
	size_t n, l;
	const char *s, *t, *origin;
	char *d;
	if (p->rpath || !p->rpath_orig) return 0;
	if (!strchr(p->rpath_orig, '$')) {
		p->rpath = p->rpath_orig;
		return 0;
	}
	n = 0;
	s = p->rpath_orig;
	while ((t=strchr(s, '$'))) {
		if (strncmp(t, "$ORIGIN", 7) && strncmp(t, "${ORIGIN}", 9))
			return 0;
		s = t+1;
		n++;
	}
	if (n > SSIZE_MAX/PATH_MAX) return 0;

	if (p->kernel_mapped) {
		/* $ORIGIN searches cannot be performed for the main program
		 * when it is suid/sgid/AT_SECURE. This is because the
		 * pathname is under the control of the caller of execve.
		 * For libraries, however, $ORIGIN can be processed safely
		 * since the library's pathname came from a trusted source
		 * (either system paths or a call to dlopen). */
		if (libc.secure)
			return 0;
		l = readlink("/proc/self/exe", buf, buf_size);
		if (l == -1) switch (errno) {
		case ENOENT:
		case ENOTDIR:
		case EACCES:
			break;
		default:
			return -1;
		}
		if (l >= buf_size)
			return 0;
		buf[l] = 0;
		origin = buf;
	} else {
		origin = p->name;
	}
	t = strrchr(origin, '/');
	if (t) {
		l = t-origin;
	} else {
		/* Normally p->name will always be an absolute or relative
		 * pathname containing at least one '/' character, but in the
		 * case where ldso was invoked as a command to execute a
		 * program in the working directory, app.name may not. Fix. */
		origin = ".";
		l = 1;
	}
	/* Disallow non-absolute origins for suid/sgid/AT_SECURE. */
	if (libc.secure && *origin != '/')
		return 0;
	p->rpath = malloc(strlen(p->rpath_orig) + n*l + 1);
	if (!p->rpath) return -1;

	d = p->rpath;
	s = p->rpath_orig;
	while ((t=strchr(s, '$'))) {
		memcpy(d, s, t-s);
		d += t-s;
		memcpy(d, origin, l);
		d += l;
		/* It was determined previously that the '$' is followed
		 * either by "ORIGIN" or "{ORIGIN}". */
		s = t + 7 + 2*(t[1]=='{');
	}
	strcpy(d, s);
	return 0;
}

static void decode_dyn(struct dso *p)
{
	size_t dyn[DYN_CNT];
	decode_vec(p->dynv, dyn, DYN_CNT);
	p->syms = laddr(p, dyn[DT_SYMTAB]);
	p->syment = dyn[DT_SYMENT];
	p->strings = laddr(p, dyn[DT_STRTAB]);
	p->dynstr_size = dyn[DT_STRSZ];
	if (dyn[0]&(1<<DT_HASH))
		p->hashtab = laddr(p, dyn[DT_HASH]);
	if (dyn[0]&(1<<DT_RPATH))
		p->rpath_orig = p->strings + dyn[DT_RPATH];
	if (dyn[0]&(1<<DT_RUNPATH))
		p->rpath_orig = p->strings + dyn[DT_RUNPATH];
	if (dyn[0]&(1<<DT_PLTGOT))
		p->got = laddr(p, dyn[DT_PLTGOT]);
	if (search_vec(p->dynv, dyn, DT_GNU_HASH))
		p->ghashtab = laddr(p, *dyn);
	if (search_vec(p->dynv, dyn, DT_VERSYM))
		p->versym = laddr(p, *dyn);
}

static size_t count_syms(struct dso *p)
{
	if (p->hashtab) return p->hashtab[1];

	size_t nsym, i;
	uint32_t *buckets = p->ghashtab + 4 + (p->ghashtab[2]*sizeof(size_t)/4);
	uint32_t *hashval;
	for (i = nsym = 0; i < p->ghashtab[0]; i++) {
		if (buckets[i] > nsym)
			nsym = buckets[i];
	}
	if (nsym) {
		hashval = buckets + p->ghashtab[0] + (nsym - p->ghashtab[1]);
		do nsym++;
		while (!(*hashval++ & 1));
	}
	return nsym;
}

static void *dl_mmap(size_t n)
{
	void *p;
	int prot = PROT_READ|PROT_WRITE, flags = MAP_ANONYMOUS|MAP_PRIVATE;
#ifdef SYS_mmap2
	p = (void *)__syscall(SYS_mmap2, 0, n, prot, flags, -1, 0);
#else
	p = (void *)__syscall(SYS_mmap, 0, n, prot, flags, -1, 0);
#endif
	return (unsigned long)p > -4096UL ? 0 : p;
}

static void makefuncdescs(struct dso *p)
{
	static int self_done;
	size_t nsym = count_syms(p);
	size_t i, size = nsym * sizeof(*p->funcdescs);

	if (!self_done) {
		p->funcdescs = dl_mmap(size);
		self_done = 1;
	} else {
		p->funcdescs = malloc(size);
	}
	if (!p->funcdescs) {
		if (!runtime) a_crash();
		error("Error allocating function descriptors for %s", p->name);
		longjmp(*rtld_fail, 1);
	}
	for (i=0; i<nsym; i++) {
		if ((p->syms[i].st_info&0xf)==STT_FUNC && p->syms[i].st_shndx) {
			p->funcdescs[i].addr = laddr(p, p->syms[i].st_value);
			p->funcdescs[i].got = p->got;
		} else {
			p->funcdescs[i].addr = 0;
			p->funcdescs[i].got = 0;
		}
	}
}

static bool is_match_name_ext(struct dso *p, const char *name)
{
	bool match = false;
	unsigned int i;

	for (i = 0; i < p->nr_ext; i++) {
		if (strcmp(p->name_ext[i], name) == 0) {
			match = true;
			break;
		}
	}

	return match;
}

static void insert_name_ext(struct dso *p, const char *name)
{
	if (p->nr_ext < MAX_EXTERNAL_NAME_NUM) {
		p->name_ext[p->nr_ext] = strdup(name);
		p->nr_ext++;
	}
}

static struct dso *load_library(const char *name, struct dso *needed_by)
{
	char buf[2*PATH_MAX+2];
	const char *pathname;
	unsigned char *map;
	struct dso *p, temp_dso = {0};
	int fd;
	struct stat st;
	size_t alloc_size;
	int n_th = 0;
	int is_self = 0;

	if (!*name) {
		errno = EINVAL;
		return 0;
	}

	/* Catch and block attempts to reload the implementation itself */
	if (name[0]=='l' && name[1]=='i' && name[2]=='b') {
		static const char reserved[] =
			"c-sys.c_kasan.c.pthread.rt.m.dl.util.xnet.crypt.resolv.";
		const char *rp, *next;
		for (rp=reserved; *rp; rp=next) {
			next = strchr(rp, '.') + 1;
			if (strncmp(name+3, rp, next-rp) == 0)
				break;
		}
		if (*rp) {
			if (ldd_mode) {
				/* Track which names have been resolved
				 * and only report each one once. */
				static unsigned reported;
				unsigned mask = 1U<<(rp-reserved);
				if (!(reported & mask)) {
					reported |= mask;
					dprintf(1, "\t%s => %s (%p)\n",
						name, ldso.name,
						ldso.base);
				}
			}
			is_self = 1;
		}
	}
	if (!strcmp(name, ldso.name)) is_self = 1;
	if (is_self) {
		if (!ldso.prev) {
			tail->next = &ldso;
			ldso.prev = tail;
			tail = &ldso;
		}
		/* increase libc dlopen refcnt */
		a_inc(&ldso.nr_dlopen);
		return &ldso;
	}
	if (strchr(name, '/')) {
		pathname = name;
		fd = open(name, O_RDONLY|O_CLOEXEC);
	} else {
		/* Search for the name to see if it's already loaded */
		for (p=head->next; p; p=p->next) {
			if (!((p->shortname && (strcmp(p->shortname, name) == 0)) ||
				is_match_name_ext(p, name))) {
				continue;
			}
			if (p->do_openclose == 0) a_inc(&p->nr_dlopen);
			return p;
		}
		if (strlen(name) > PATH_MAX) return 0;
		fd = -1;
		if (env_path) fd = path_open(name, env_path, buf, sizeof buf);
		for (p=needed_by; fd == -1 && p; p=p->needed_by) {
			if (fixup_rpath(p, buf, sizeof buf) < 0)
				fd = -2; /* Inhibit further search. */
			if (p->rpath)
				fd = path_open(name, p->rpath, buf, sizeof buf);
		}
		if (fd == -1) {
			if (!sys_path) {
				char *prefix = 0;
				size_t prefix_len;
				if (ldso.name[0]=='/') {
					char *s, *t, *z;
					for (s=t=z=ldso.name; *s; s++)
						if (*s=='/') z=t, t=s;
					prefix_len = z-ldso.name;
					if (prefix_len < PATH_MAX)
						prefix = ldso.name;
				}
				if (!prefix) {
					prefix = "";
					prefix_len = 0;
				}
				char etc_ldso_path[prefix_len + 1
					+ sizeof "/etc/ld-musl-" LDSO_ARCH ".path"];
				snprintf(etc_ldso_path, sizeof etc_ldso_path,
					"%.*s/etc/ld-musl-" LDSO_ARCH ".path",
					(int)prefix_len, prefix);
				fd = open(etc_ldso_path, O_RDONLY|O_CLOEXEC);
				if (fd>=0) {
					size_t n = 0;
					if (!fstat(fd, &st)) n = st.st_size;
					if ((sys_path = malloc(n+1)))
						sys_path[n] = 0;
					if (!sys_path || read_loop(fd, sys_path, n)<0) {
						free(sys_path);
						sys_path = "";
					}
					close(fd);
				} else if (errno != ENOENT) {
					sys_path = "";
				}
			}
#ifdef __ILP32__
			if (!sys_path) sys_path = "/libilp32:/usr/libilp32:/lib:/usr/local/lib:/usr/lib";
#else
			if (!sys_path) sys_path = SYS_PATH_DFLT;
#endif
			fd = path_open(name, sys_path, buf, sizeof buf);
		}
		pathname = buf;
	}
	if (fd < 0) return 0;
	if (fstat(fd, &st) < 0) {
		close(fd);
		return 0;
	}
	for (p=head->next; p; p=p->next) {
		if (p->dev == st.st_dev && p->ino == st.st_ino) {
			/* If this library was previously loaded with a
			 * pathname but a search found the same inode,
			 * setup its shortname so it can be found by name. */
			if (!p->shortname && pathname != name) {
				p->shortname = strrchr(p->name, '/')+1;
			} else {
				insert_name_ext(p, strrchr(pathname, '/')+1);
			}
			close(fd);
			/* increase dlopen refcnt */
			if (p->do_openclose == 0) a_inc(&p->nr_dlopen);
			return p;
		}
	}
	map = noload ? 0 : map_library(fd, &temp_dso);
	close(fd);
	if (!map) return 0;

	/* Avoid the danger of getting two versions of libc mapped into the
	 * same process when an absolute pathname was used. The symbols
	 * checked are chosen to catch both musl and glibc, and to avoid
	 * false positives from interposition-hack libraries. */
	if (find_sym(&temp_dso, "__libc_start_main", 1).sym &&
	    find_sym(&temp_dso, "stdin", 1).sym) {
		unmap_library(&temp_dso);
		return load_library("libc.so", needed_by);
	}
	/* Past this point, if we haven't reached runtime yet, ldso has
	 * committed either to use the mapped library or to abort execution.
	 * Unmapping is not possible, so we can safely reclaim gaps. */
	if (!runtime) reclaim_gaps(&temp_dso);

	/* Allocate storage for the new DSO. When there is TLS, this
	 * storage must include a reservation for all pre-existing
	 * threads to obtain copies of both the new TLS, and an
	 * extended DTV capable of storing an additional slot for
	 * the newly-loaded DSO. */
	alloc_size = sizeof *p + strlen(pathname) + 1;
	if (runtime && temp_dso.tls.image) {
		size_t per_th = temp_dso.tls.size + temp_dso.tls.align
			+ sizeof(void *) * (tls_cnt+3);
		n_th = libc.threads_minus_1 + 1;
		if (n_th > SSIZE_MAX / per_th) alloc_size = SIZE_MAX;
		else alloc_size += n_th * per_th;
	}
	p = calloc(1, alloc_size);
	if (!p) {
		unmap_library(&temp_dso);
		return 0;
	}
	memcpy(p, &temp_dso, sizeof temp_dso);
	p->dev = st.st_dev;
	p->ino = st.st_ino;
	p->needed_by = needed_by;
	p->name = p->buf;
	p->nr_dlopen = 1;
	p->runtime_loaded = runtime;
	strcpy(p->name, pathname);
	/* Add a shortname only if name arg was not an explicit pathname. */
	if (pathname != name) p->shortname = strrchr(p->name, '/')+1;
	if (p->tls.image) {
		p->tls_id = ++tls_cnt;
		tls_align = MAXP2(tls_align, p->tls.align);
#ifdef TLS_ABOVE_TP
		p->tls.offset = tls_offset + ( (p->tls.align-1) &
			(-tls_offset + (uintptr_t)p->tls.image) );
		tls_offset = p->tls.offset + p->tls.size;
#else
		tls_offset += p->tls.size + p->tls.align - 1;
		tls_offset -= (tls_offset + (uintptr_t)p->tls.image)
			& (p->tls.align-1);
		p->tls.offset = tls_offset;
#endif
		p->new_dtv = (void *)(-sizeof(size_t) &
			(uintptr_t)(p->name+strlen(p->name)+sizeof(size_t)));
		p->new_tls = (void *)(p->new_dtv + n_th*(tls_cnt+1));
		if (tls_tail) tls_tail->next = &p->tls;
		else libc.tls_head = &p->tls;
		tls_tail = &p->tls;
	}

	tail->next = p;
	p->prev = tail;
	tail = p;

	if (runtime)
		p->by_dlopen = 1;

	if (DL_FDPIC) makefuncdescs(p);

	if (ldd_mode) dprintf(1, "\t%s => %s (%p)\n", name, pathname, p->base);

	__fill_dso_syminfo(p);

	return p;
}

static void load_direct_deps(struct dso *p)
{
	size_t i, cnt=0;

	if (p->deps) return;
	/* For head, all preloads are direct pseudo-dependencies.
	 * Count and include them now to avoid realloc later. */
	if (p==head) for (struct dso *q=p->next; q; q=q->next)
		cnt++;
	for (i=0; p->dynv[i]; i+=2)
		if (p->dynv[i] == DT_NEEDED) cnt++;
	/* Use builtin buffer for apps with no external deps, to
	 * preserve property of no runtime failure paths. */
	p->deps = (p==head && cnt<2) ? builtin_deps :
		calloc(cnt+1, sizeof *p->deps);
	if (!p->deps) {
		error("Error loading dependencies for %s", p->name);
		if (runtime) longjmp(*rtld_fail, 1);
	}
	cnt=0;
	if (p==head) for (struct dso *q=p->next; q; q=q->next)
		p->deps[cnt++] = q;
	for (i=0; p->dynv[i]; i+=2) {
		if (p->dynv[i] != DT_NEEDED) continue;
		struct dso *dep = load_library(p->strings + p->dynv[i+1], p);
		if (!dep) {
			error("Error loading shared library %s: %m (needed by %s)",
				p->strings + p->dynv[i+1], p->name);
			if (runtime) longjmp(*rtld_fail, 1);
			continue;
		}
		p->deps[cnt++] = dep;
	}
	p->deps[cnt] = 0;
	p->ndeps_direct = cnt;
}

static void load_deps(struct dso *p)
{
	if (p->deps) return;
	for (; p; p=p->next)
		load_direct_deps(p);
}

static void extend_bfs_deps(struct dso *p)
{
	size_t i, j, cnt, ndeps_all;
	struct dso **tmp;

	/* Can't use realloc if the original p->deps was allocated at
	 * program entry and malloc has been replaced, or if it's
	 * the builtin non-allocated trivial main program deps array. */
	int no_realloc = (__malloc_replaced && !p->runtime_loaded)
		|| p->deps == builtin_deps;

	if (p->bfs_built) return;
	ndeps_all = p->ndeps_direct;

	/* Mark existing (direct) deps so they won't be duplicated. */
	for (i=0; p->deps[i]; i++)
		p->deps[i]->mark = 1;

	/* For each dependency already in the list, copy its list of direct
	 * dependencies to the list, excluding any items already in the
	 * list. Note that the list this loop iterates over will grow during
	 * the loop, but since duplicates are excluded, growth is bounded. */
	for (i=0; p->deps[i]; i++) {
		struct dso *dep = p->deps[i];
		for (j=cnt=0; j<dep->ndeps_direct; j++)
			if (!dep->deps[j]->mark) cnt++;
		tmp = no_realloc ?
			malloc(sizeof(*tmp) * (ndeps_all+cnt+1)) :
			realloc(p->deps, sizeof(*tmp) * (ndeps_all+cnt+1));
		if (!tmp) {
			error("Error recording dependencies for %s", p->name);
			if (runtime) longjmp(*rtld_fail, 1);
			continue;
		}
		if (no_realloc) {
			memcpy(tmp, p->deps, sizeof(*tmp) * (ndeps_all+1));
			no_realloc = 0;
		}
		p->deps = tmp;
		for (j=0; j<dep->ndeps_direct; j++) {
			if (dep->deps[j]->mark) continue;
			dep->deps[j]->mark = 1;
			p->deps[ndeps_all++] = dep->deps[j];
		}
		p->deps[ndeps_all] = 0;
	}
	p->bfs_built = 1;
	for (p=head; p; p=p->next)
		p->mark = 0;
}

static void load_preload(char *s)
{
	int tmp;
	char *z;
	for (z=s; *z; s=z) {
		for (   ; *s && (isspace(*s) || *s==':'); s++);
		for (z=s; *z && !isspace(*z) && *z!=':'; z++);
		tmp = *z;
		*z = 0;
		load_library(s, 0);
		*z = tmp;
	}
}

static void add_syms(struct dso *p)
{
	if (!p->syms_next && syms_tail != p) {
		syms_tail->syms_next = p;
		syms_tail = p;
	}
}

static void revert_syms(struct dso *old_tail)
{
	struct dso *p, *next;
	/* Chop off the tail of the list of dsos that participate in
	 * the global symbol table, reverting them to RTLD_LOCAL. */
	for (p=old_tail; p; p=next) {
		next = p->syms_next;
		p->syms_next = 0;
	}
	syms_tail = old_tail;
}

static void do_mips_relocs(struct dso *p, size_t *got)
{
	size_t i, j, rel[2];
	unsigned char *base = p->base;
	i=0; search_vec(p->dynv, &i, DT_MIPS_LOCAL_GOTNO);
	if (p==&ldso) {
		got += i;
	} else {
		while (i--) *got++ += (size_t)base;
	}
	j=0; search_vec(p->dynv, &j, DT_MIPS_GOTSYM);
	i=0; search_vec(p->dynv, &i, DT_MIPS_SYMTABNO);
	Sym *sym = p->syms + j;
	rel[0] = (unsigned char *)got - base;
	for (i-=j; i; i--, sym++, rel[0]+=sizeof(size_t)) {
		rel[1] = R_INFO(sym-p->syms, R_MIPS_JUMP_SLOT);
		do_relocs(p, rel, sizeof rel, 2);
	}
}

static void reloc_dso(struct dso *p)
{
	size_t dyn[DYN_CNT];
	if (p->relocated != 0) return;
	decode_vec(p->dynv, dyn, DYN_CNT);
	if (NEED_MIPS_GOT_RELOCS != 0)
		do_mips_relocs(p, laddr(p, dyn[DT_PLTGOT]));
	do_relocs(p, laddr(p, dyn[DT_JMPREL]), dyn[DT_PLTRELSZ],
		2+((dyn[DT_PLTREL]==DT_RELA) ? 1 : 0)); /* 2: stride */
	do_relocs(p, laddr(p, dyn[DT_REL]), dyn[DT_RELSZ], 2);
	do_relocs(p, laddr(p, dyn[DT_RELA]), dyn[DT_RELASZ], 3);

	if (head != &ldso && p->lazy == NULL && p->relro_start != p->relro_end &&
	    mprotect(laddr(p, p->relro_start), p->relro_end-p->relro_start, PROT_READ)
	    && errno != ENOSYS) {
		error("Error relocating %s: RELRO protection failed: %m",
			p->name);
		if (runtime != 0) longjmp(*rtld_fail, 1);
	}

	p->relocated = 1;
}


static void reloc_all(struct dso *p)
{
	for (; p; p=p->next) {
		reloc_dso(p);
	}
}

static void kernel_mapped_dso(struct dso *p)
{
	size_t min_addr = -1, max_addr = 0, cnt;
	Phdr *ph = p->phdr;
	for (cnt = p->phnum; cnt--; ph = (void *)((char *)ph + p->phentsize)) {
		if (ph->p_type == PT_DYNAMIC) {
			p->dynv = laddr(p, ph->p_vaddr);
		} else if (ph->p_type == PT_GNU_RELRO) {
			p->relro_start = ph->p_vaddr & -PAGE_SIZE;
			p->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
		} else if (ph->p_type == PT_GNU_STACK) {
			if (!runtime && ph->p_memsz > __default_stacksize) {
				__default_stacksize =
					ph->p_memsz < DEFAULT_STACK_MAX ?
					ph->p_memsz : DEFAULT_STACK_MAX;
			}
		}
		if (ph->p_type != PT_LOAD) continue;
		if (ph->p_vaddr < min_addr)
			min_addr = ph->p_vaddr;
		if (ph->p_vaddr+ph->p_memsz > max_addr)
			max_addr = ph->p_vaddr+ph->p_memsz;
	}
	min_addr &= -PAGE_SIZE;
	max_addr = (max_addr + PAGE_SIZE-1) & -PAGE_SIZE;
	p->map = p->base + min_addr;
	p->map_len = max_addr - min_addr;
	p->kernel_mapped = 1;
}

void __libc_exit_fini()
{
	struct dso *p;
	size_t dyn[DYN_CNT];
	pthread_t self = __pthread_self();

	/* Take both locks before setting shutting_down, so that
	 * either lock is sufficient to read its value. The lock
	 * order matches that in dlopen to avoid deadlock. */
	pthread_rwlock_wrlock(&lock);
	pthread_mutex_lock(&init_fini_lock);
	shutting_down = 1;
	pthread_rwlock_unlock(&lock);
	for (p=fini_head; p; p=p->fini_next) {
		while (p->ctor_visitor && p->ctor_visitor!=self)
			pthread_cond_wait(&ctor_cond, &init_fini_lock);
		if (!p->constructed) continue;
		decode_vec(p->dynv, dyn, DYN_CNT);
		if (dyn[0] & (1<<DT_FINI_ARRAY)) {
			size_t n = dyn[DT_FINI_ARRAYSZ]/sizeof(size_t);
			size_t *fn = (size_t *)laddr(p, dyn[DT_FINI_ARRAY])+n;
			while (n--) ((void (*)(void))*--fn)();
		}
#ifndef NO_LEGACY_INITFINI
		if ((dyn[0] & (1<<DT_FINI)) && dyn[DT_FINI])
			fpaddr(p, dyn[DT_FINI])();
#endif
	}
}

void __ldso_atfork(int who)
{
	if (who<0) {
		pthread_rwlock_wrlock(&lock);
		pthread_mutex_lock(&init_fini_lock);
	} else {
		pthread_mutex_unlock(&init_fini_lock);
		pthread_rwlock_unlock(&lock);
	}
}

static struct dso **queue_ctors(struct dso *dso)
{
	size_t cnt, qpos, spos, i;
	struct dso *p, **queue, **stack;

	if (ldd_mode) return 0;

	/* Bound on queue size is the total number of indirect deps.
	 * If a bfs deps list was built, we can use it. Otherwise,
	 * bound by the total number of DSOs, which is always safe and
	 * is reasonable we use it (for main app at startup). */
	if (dso->bfs_built) {
		for (cnt=0; dso->deps[cnt]; cnt++)
			dso->deps[cnt]->mark = 0;
		cnt++; /* self, not included in deps */
	} else {
		for (cnt=0, p=head; p; cnt++, p=p->next)
			p->mark = 0;
	}
	cnt++; /* termination slot */
	if (dso==head && cnt <= countof(builtin_ctor_queue))
		queue = builtin_ctor_queue;
	else
		queue = calloc(cnt, sizeof *queue);

	if (!queue) {
		error("Error allocating constructor queue: %m\n");
		if (runtime) longjmp(*rtld_fail, 1);
		return 0;
	}

	/* Opposite ends of the allocated buffer serve as an output queue
	 * and a working stack. Setup initial stack with just the argument
	 * dso and initial queue empty... */
	stack = queue;
	qpos = 0;
	spos = cnt;
	stack[--spos] = dso;
	dso->next_dep = 0;
	dso->mark = 1;

	/* Then perform pseudo-DFS sort, but ignoring circular deps. */
	while (spos<cnt) {
		p = stack[spos++];
		while (p->next_dep < p->ndeps_direct) {
			if (p->deps[p->next_dep]->mark) {
				p->next_dep++;
			} else {
				stack[--spos] = p;
				p = p->deps[p->next_dep];
				p->next_dep = 0;
				p->mark = 1;
			}
		}
		queue[qpos++] = p;
	}
	queue[qpos] = 0;
	for (i=0; i<qpos; i++) queue[i]->mark = 0;
	for (i=0; i<qpos; i++)
		if (queue[i]->ctor_visitor && queue[i]->ctor_visitor->tid < 0) {
			error("State of %s is inconsistent due to multithreaded fork\n",
				queue[i]->name);
			free(queue);
			if (runtime) longjmp(*rtld_fail, 1);
		}

	return queue;
}

static void do_preinit(void)
{
	size_t *v = NULL;
	size_t *fn = NULL;
	size_t n = 0;

	/* The preinit_array section is not allowed in DSO, see
	 * http://www.sco.com/developers/gabi/latest/ch5.dynamic.html#init_fini */
	for (v = head->dynv; v[0]; v += 2) {
		if (v[0] == DT_PREINIT_ARRAY)
			fn = laddr(head, v[1]);
		else if (v[0] == DT_PREINIT_ARRAYSZ)
			n = v[1] / sizeof(size_t);
	}

	if (n == 0 || fn == NULL) {
		return;
	}
	while (n) {
		((void (*)(void))*fn++)();
		n--;
	}
}

static void do_init_fini(struct dso **queue)
{
	struct dso *p;
	size_t dyn[DYN_CNT], i;
	pthread_t self = __pthread_self();

	pthread_mutex_lock(&init_fini_lock);
	for (i=0; (p=queue[i]); i++) {
		while ((p->ctor_visitor && p->ctor_visitor!=self) || shutting_down)
			pthread_cond_wait(&ctor_cond, &init_fini_lock);
		if (p->ctor_visitor || p->constructed)
			continue;
		p->ctor_visitor = self;

		decode_vec(p->dynv, dyn, DYN_CNT);
		if (dyn[0] & ((1<<DT_FINI) | (1<<DT_FINI_ARRAY))) {
			p->fini_next = fini_head;
			fini_head = p;
		}

		pthread_mutex_unlock(&init_fini_lock);

#ifndef NO_LEGACY_INITFINI
		if ((dyn[0] & (1<<DT_INIT)) && dyn[DT_INIT])
			fpaddr(p, dyn[DT_INIT])();
#endif
		if (dyn[0] & (1<<DT_INIT_ARRAY)) {
			size_t n = dyn[DT_INIT_ARRAYSZ]/sizeof(size_t);
			size_t *fn = laddr(p, dyn[DT_INIT_ARRAY]);
			while (n--) ((void (*)(void))*fn++)();
		}

		pthread_mutex_lock(&init_fini_lock);
		p->ctor_visitor = 0;
		p->constructed = 1;
		pthread_cond_broadcast(&ctor_cond);
	}
	pthread_mutex_unlock(&init_fini_lock);
}

void __libc_start_init(void)
{
	do_preinit();
	do_init_fini(main_ctor_queue);
	if (!__malloc_replaced && main_ctor_queue != builtin_ctor_queue)
		free(main_ctor_queue);
	main_ctor_queue = 0;
}

static void dl_debug_state(void)
{
}

weak_alias(dl_debug_state, _dl_debug_state);

void __init_tls(size_t *auxv)
{
}

static void update_tls_size()
{
	libc.tls_cnt = tls_cnt;
	libc.tls_align = tls_align;
	libc.tls_size = ALIGN(
		(1+tls_cnt) * sizeof(void *) +
		tls_offset +
		sizeof(struct pthread) +
		tls_align * 2,
	tls_align);
}

static void install_new_tls(void)
{
	sigset_t set;
	pthread_t self = __pthread_self(), td;
	struct dso *dtv_provider = container_of(tls_tail, struct dso, tls);
	uintptr_t (*newdtv)[tls_cnt+1] = (void *)dtv_provider->new_dtv;
	struct dso *p;
	size_t i, j;
	size_t old_cnt = self->dtv[0];

	__block_app_sigs(&set);
	__tl_lock();
	/* Copy existing dtv contents from all existing threads. */
	for (i=0, td=self; !i || td!=self; i++, td=td->next) {
		memcpy(newdtv+i, td->dtv,
			(old_cnt+1)*sizeof(uintptr_t));
		newdtv[i][0] = tls_cnt;
	}
	/* Install new dtls into the enlarged, uninstalled dtv copies. */
	for (p=head; ; p=p->next) {
		if (p->tls_id <= old_cnt) continue;
		unsigned char *mem = p->new_tls;
		for (j=0; j<i; j++) {
			unsigned char *new = mem;
			new += ((uintptr_t)p->tls.image - (uintptr_t)mem)
				& (p->tls.align-1);
			memcpy(new, p->tls.image, p->tls.len);
			newdtv[j][p->tls_id] =
				(uintptr_t)new + DTP_OFFSET;
			mem += p->tls.size + p->tls.align;
		}
		if (p->tls_id == tls_cnt) break;
	}

	/* Broadcast barrier to ensure contents of new dtv is visible
	 * if the new dtv pointer is. The __membarrier function has a
	 * fallback emulation using signals for kernels that lack the
	 * feature at the syscall level. */

	__membarrier(MEMBARRIER_CMD_PRIVATE_EXPEDITED, 0);

	/* Install new dtv for each thread. */
	for (j=0, td=self; !j || td!=self; j++, td=td->next) {
		td->dtv = newdtv[j];
	}

	__tl_unlock();
	__restore_sigs(&set);
}

/*
 * The libc_kasan.so is C library which enables kasan.
 * When libc_kasan.so is used, we should use the file path
 * below to make sure the lib name is correct.
 */
#ifdef LIBC_SYS_VERSION
#define LIBC_NAME "libc-sys.so"
#elif defined LIBC_ENABLE_KASAN
#define LIBC_NAME "libc_kasan.so"
#else
#define LIBC_NAME "libc.so"
#endif

#ifdef __ILP32__
#define COMPAT_FIND_PATH "/libilp32:/usr/libilp32"
#define COMPAT_DEFAULT_PATH "/libilp32"
#else
#define COMPAT_FIND_PATH "/lib32:/usr/lib32"
#define COMPAT_DEFAULT_PATH "/lib32"
#endif

#define FIND_PATH "/lib:/usr/lib"
#define DEFAULT_PATH "/lib"

static void fixup_libc_name(char *libc_name)
{
	int fd;
	static char name[LIBC_NAME_LEN];
	size_t is_compat = 0;
	char find_path[LIBC_NAME_LEN];

	search_vec(libc.auxv, &is_compat, AT_FLAGS);
	if (is_compat) {
		strncpy(find_path, COMPAT_FIND_PATH, strlen(COMPAT_FIND_PATH) + 1);
	} else {
		strncpy(find_path, FIND_PATH, strlen(FIND_PATH) + 1);
	}

	fd = path_open(libc_name, find_path, name, sizeof name);
	if (fd < 0) {
		if (is_compat) {
			(void)snprintf(name, LIBC_NAME_LEN, "%s/%s", COMPAT_DEFAULT_PATH, libc_name);
		} else {
			(void)snprintf(name, LIBC_NAME_LEN, "%s/%s", DEFAULT_PATH, libc_name);
		}
	} else {
		close(fd);
	}
	ldso.shortname = libc_name;
	ldso.name = name;
}

static void load_native_library(struct dso *app)
{
#ifdef LIBC_ENABLE_KASAN
	/*
	 * The hmsrv_fs_kasan and hmsrv_net_kasan are compiled when kasan is enabled.
	 * And the uapps of hm-verif-kernel will use kasan libraries. To avoid the
	 * conflicts between two kinds of librares, the different native_libs is added.
	 */
	const char *native_libs[NR_NATIVE_LIBS] = {
		"libhmulibs.so.0",
		"libhmsrv_fs_kasan.so.0",
		"libhmsrv_net_kasan.so",
	};
#else
	const char *native_libs[NR_NATIVE_LIBS] = {
		"libhmulibs.so.0",
		"libhmsrv_fs.so.0",
		"libhmsrv_net.so",
	};
#endif
	for (int i = 0; i < NR_NATIVE_LIBS; ++i) {
		native_dso[i] = load_library(native_libs[i], app);
		if (native_dso[i] == NULL) {
			error("Error loading hongmeng native libs %s", native_libs[i]);
			return;
		}
	}
}


static bool is_native_dso(const struct dso *p)
{
	for (int i = 0; i < NR_NATIVE_LIBS; ++i) {
		if (p == native_dso[i]) {
			return true;
		}
	}
	return false;
}

static void add_native_dso_syms()
{
	for (int i = 0; i < NR_NATIVE_LIBS; ++i) {
		if (native_dso[i] != NULL) {
			add_syms(native_dso[i]);
		}
	}
	return;
}


/* Stage 1 of the dynamic linker is defined in dlstart.c. It calls the
 * following stage 2 and stage 3 functions via primitive symbolic lookup
 * since it does not have access to their addresses to begin with. */

/* Stage 2 of the dynamic linker is called after relative relocations
 * have been processed. It can make function calls to static functions
 * and access string literals and static data, but cannot use extern
 * symbols. Its job is to perform symbolic relocations on the dynamic
 * linker itself, but some of the relocations performed may need to be
 * replaced later due to copy relocations in the main program. */

hidden void __dls2(unsigned char *base, size_t *sp)
{
	size_t *auxv;
	for (auxv=sp+1+*sp+1; *auxv; auxv++);
	auxv++;
	if (DL_FDPIC) {
		void *p1 = (void *)sp[-2];
		void *p2 = (void *)sp[-1];
		if (!p1) {
			size_t aux[AUX_CNT];
			decode_vec(auxv, aux, AUX_CNT);
			if (aux[AT_BASE]) ldso.base = (void *)aux[AT_BASE];
			else ldso.base = (void *)(aux[AT_PHDR] & -4096);
		}
		app_loadmap = p2 ? p1 : 0;
		ldso.loadmap = p2 ? p2 : p1;
		ldso.base = laddr(&ldso, 0);
	} else {
		ldso.base = base;
	}
	Ehdr *ehdr = (void *)ldso.base;
	ldso.shortname = "libc.so";
	ldso.phnum = ehdr->e_phnum;
	ldso.phdr = laddr(&ldso, ehdr->e_phoff);
	ldso.phentsize = ehdr->e_phentsize;
	kernel_mapped_dso(&ldso);
	decode_dyn(&ldso);

	if (DL_FDPIC) makefuncdescs(&ldso);

	/* Prepare storage for to save clobbered REL addends so they
	 * can be reused in stage 3. There should be very few. If
	 * something goes wrong and there are a huge number, abort
	 * instead of risking stack overflow. */
	size_t dyn[DYN_CNT];
	decode_vec(ldso.dynv, dyn, DYN_CNT);
	size_t *rel = laddr(&ldso, dyn[DT_REL]);
	size_t rel_size = dyn[DT_RELSZ];
	size_t symbolic_rel_cnt = 0;
	apply_addends_to = rel;
	for (; rel_size; rel+=2, rel_size-=2*sizeof(size_t))
		if (!IS_RELATIVE(rel[1], ldso.syms)) symbolic_rel_cnt++;
	if (symbolic_rel_cnt >= ADDEND_LIMIT) a_crash();
	size_t addends[symbolic_rel_cnt+1];
	saved_addends = addends;

	head = &ldso;
	reloc_all(&ldso);

	ldso.relocated = 0;

	/* Call dynamic linker stage-2b, __dls2b, looking it up
	 * symbolically as a barrier against moving the address
	 * load across the above relocation processing. */
	struct symdef dls2b_def = find_sym(&ldso, "__dls2b", 0);
	if (DL_FDPIC) ((stage3_func)&ldso.funcdescs[dls2b_def.sym-ldso.syms])(sp, auxv);
	else ((stage3_func)laddr(&ldso, dls2b_def.sym->st_value))(sp, auxv);
}

/* Stage 2b sets up a valid thread pointer, which requires relocations
 * completed in stage 2, and on which stage 3 is permitted to depend.
 * This is done as a separate stage, with symbolic lookup as a barrier,
 * so that loads of the thread pointer and &errno can be pure/const and
 * thereby hoistable. */

void __dls2b(size_t *sp, size_t *auxv)
{
	/* Setup early thread pointer in builtin_tls for ldso/libc itself to
	 * use during dynamic linking. If possible it will also serve as the
	 * thread pointer at runtime. */
	search_vec(auxv, &__hwcap, AT_HWCAP);
	libc.auxv = auxv;
	libc.tls_size = sizeof builtin_tls;
	libc.tls_align = tls_align;
	if (__init_tp(__copy_tls((void *)builtin_tls)) < 0) {
		a_crash();
	}
	fixup_libc_name(LIBC_NAME);

	struct symdef dls3_def = find_sym(&ldso, "__dls3", 0);
	if (DL_FDPIC) ((stage3_func)&ldso.funcdescs[dls3_def.sym-ldso.syms])(sp, auxv);
	else ((stage3_func)laddr(&ldso, dls3_def.sym->st_value))(sp, auxv);
}

/* Stage 3 of the dynamic linker is called with the dynamic linker/libc
 * fully functional. Its job is to load (if not already loaded) and
 * process dependencies and relocations for the main application and
 * transfer control to its entry point. */

void __dls3(size_t *sp, size_t *auxv)
{
	static struct dso app, vdso;
	size_t aux[AUX_CNT];
	size_t i;
	char *env_preload=0;
	char *replace_argv0=0;
	size_t vdso_base;
	int argc = *sp;
	char **argv = (void *)(sp+1);
	char **argv_orig = argv;
	char **envp = argv+argc+1;

	/* Find aux vector just past environ[] and use it to initialize
	 * global data that may be needed before we can make syscalls. */
	__environ = envp;
	for (i=(size_t)(argc+1); argv[i]; i++);
	libc.auxv = auxv = (void *)(argv+i+1);

	decode_vec(auxv, aux, AUX_CNT);
	search_vec(auxv, &__sysinfo, AT_SYSINFO);
	__pthread_self()->sysinfo = __sysinfo;
	libc.page_size = aux[AT_PAGESZ];
	libc.secure = ((aux[0]&0x7800)!=0x7800 || aux[AT_UID]!=aux[AT_EUID]
		|| aux[AT_GID]!=aux[AT_EGID] || aux[AT_SECURE]);

	/* Only trust user/env if kernel says we're not suid/sgid */
	if (!libc.secure) {
		env_path = getenv("LD_LIBRARY_PATH");
		env_preload = getenv("LD_PRELOAD");
	}

	/* Find start address of memory which is used to
	 * record dso symbol info. */
	set_dso_syminfo(auxv);
	/* special post handle of symtab and strtab of libc.so */
	generate_libc_syminfo();
	/* fill symbol of libc.so  */
	__fill_dso_syminfo(&ldso);

	/* If the main program was already loaded by the kernel,
	 * AT_PHDR will point to some location other than the dynamic
	 * linker's program headers. */
	if (aux[AT_PHDR] != (size_t)ldso.phdr) {
		size_t interp_off = 0;
		size_t tls_image = 0;
		/* Find load address of the main program, via AT_PHDR vs PT_PHDR. */
		Phdr *phdr = app.phdr = (void *)aux[AT_PHDR];
		app.phnum = aux[AT_PHNUM];
		app.phentsize = aux[AT_PHENT];
		for (i=aux[AT_PHNUM]; i; i--, phdr=(void *)((char *)phdr + aux[AT_PHENT])) {
			if (phdr->p_type == PT_PHDR)
				app.base = (void *)(aux[AT_PHDR] - phdr->p_vaddr);
			else if (phdr->p_type == PT_INTERP)
				interp_off = (size_t)phdr->p_vaddr;
			else if (phdr->p_type == PT_TLS) {
				tls_image = phdr->p_vaddr;
				app.tls.len = phdr->p_filesz;
				app.tls.size = phdr->p_memsz;
				app.tls.align = phdr->p_align;
			}
		}
		if (DL_FDPIC) app.loadmap = app_loadmap;
		if (app.tls.size) app.tls.image = laddr(&app, tls_image);
		if ((aux[0] & (1UL<<AT_EXECFN))
		    && strncmp((char *)aux[AT_EXECFN], "/proc/", 6))
			app.name = (char *)aux[AT_EXECFN];
		else
			app.name = argv[0];
		kernel_mapped_dso(&app);
		decode_dyn(&app);
	} else {
		int fd;
		char *ldname = argv[0];
		size_t l = strlen(ldname);
		if (l >= 3 && !strcmp(ldname+l-3, "ldd")) ldd_mode = 1;
		argv++;
		while (argv[0] && argv[0][0]=='-' && argv[0][1]=='-') {
			char *opt = argv[0]+2;
			*argv++ = (void *)-1;
			if (!*opt) {
				break;
			} else if (!memcmp(opt, "list", 5)) {
				ldd_mode = 1;
			} else if (!memcmp(opt, "library-path", 12)) {
				if (opt[12]=='=') env_path = opt+13;
				else if (opt[12]) *argv = 0;
				else if (*argv) env_path = *argv++;
			} else if (!memcmp(opt, "preload", 7)) {
				if (opt[7]=='=') env_preload = opt+8;
				else if (opt[7]) *argv = 0;
				else if (*argv) env_preload = *argv++;
			} else if (!memcmp(opt, "argv0", 5)) {
				if (opt[5]=='=') replace_argv0 = opt+6;
				else if (opt[5]) *argv = 0;
				else if (*argv) replace_argv0 = *argv++;
			} else {
				argv[0] = 0;
			}
		}
		argv[-1] = (void *)(argc - (argv-argv_orig));
		if (!argv[0]) {
			dprintf(2, "musl libc (" LDSO_ARCH ")\n"
				"Version %s\n"
				"Dynamic Program Loader\n"
				"Usage: %s [options] [--] pathname%s\n",
				__libc_version, ldname,
				ldd_mode ? "" : " [args]");
			_exit(1);
		}
		fd = open(argv[0], O_RDONLY);
		if (fd < 0) {
			dprintf(2, "%s: cannot load %s: %s\n", ldname, argv[0], strerror(errno));
			_exit(1);
		}
		Ehdr *ehdr = map_library(fd, &app);
		if (!ehdr) {
			dprintf(2, "%s: %s: Not a valid dynamic program\n", ldname, argv[0]);
			_exit(1);
		}
		close(fd);
		ldso.name = ldname;
		app.name = argv[0];
		aux[AT_ENTRY] = (size_t)laddr(&app, ehdr->e_entry);
		/* Find the name that would have been used for the dynamic
		 * linker had ldd not taken its place. */
		if (ldd_mode) {
			for (i=0; i<app.phnum; i++) {
				if (app.phdr[i].p_type == PT_INTERP)
					ldso.name = laddr(&app, app.phdr[i].p_vaddr);
			}
			dprintf(1, "\t%s (%p)\n", ldso.name, ldso.base);
		}
	}
	if (app.tls.size) {
		libc.tls_head = tls_tail = &app.tls;
		app.tls_id = tls_cnt = 1;
#ifdef TLS_ABOVE_TP
		app.tls.offset = GAP_ABOVE_TP;
		app.tls.offset += (-GAP_ABOVE_TP + (uintptr_t)app.tls.image)
			& (app.tls.align-1);
		tls_offset = app.tls.offset + app.tls.size;
#else
		tls_offset = app.tls.offset = app.tls.size
			+ ( -((uintptr_t)app.tls.image + app.tls.size)
			& (app.tls.align-1) );
#endif
		tls_align = MAXP2(tls_align, app.tls.align);
	}
	if (DL_FDPIC) {
		makefuncdescs(&app);
		if (!app.loadmap) {
			app.loadmap = (void *)&app_dummy_loadmap;
			app.loadmap->nsegs = 1;
			app.loadmap->segs[0].addr = (size_t)app.map;
			app.loadmap->segs[0].p_vaddr = (size_t)app.map
				- (size_t)app.base;
			app.loadmap->segs[0].p_memsz = app.map_len;
		}
		argv[-3] = (void *)app.loadmap;
	}

	/* Initial dso chain consists only of the app. */
	head = tail = syms_tail = &app;

	/* Donate unused parts of app and library mapping to malloc */
	reclaim_gaps(&app);
	reclaim_gaps(&ldso);

	/* Load preload/needed libraries, add symbols to global namespace. */
	ldso.deps = (struct dso **)no_deps;
	if (env_preload) load_preload(env_preload);
 	load_deps(&app);
	/* load native library automatically, such as ulib.so */
	load_native_library(&app);
	/*
	 * native library should be added to syms list at the same time with libc.so,
	 * and must ahead of libc.so
	 * */
	for (struct dso *p=head; p; p=p->next) {
		if (is_native_dso(p)) {
			continue;
		} else if (p == &ldso) {
			add_native_dso_syms();
			add_syms(p);
		} else {
			add_syms(p);
		}
	}

	/* Attach to vdso, if provided by the kernel, last so that it does
	 * not become part of the global namespace.  */
	if (search_vec(auxv, &vdso_base, AT_SYSINFO_EHDR) && vdso_base) {
		Ehdr *ehdr = (void *)vdso_base;
		Phdr *phdr = vdso.phdr = (void *)(vdso_base + ehdr->e_phoff);
		vdso.phnum = ehdr->e_phnum;
		vdso.phentsize = ehdr->e_phentsize;
		for (i=ehdr->e_phnum; i; i--, phdr=(void *)((char *)phdr + ehdr->e_phentsize)) {
			if (phdr->p_type == PT_DYNAMIC)
				vdso.dynv = (void *)(vdso_base + phdr->p_offset);
			if (phdr->p_type == PT_LOAD)
				vdso.base = (void *)(vdso_base - phdr->p_vaddr + phdr->p_offset);
		}
		vdso.name = "";
		vdso.shortname = "linux-gate.so.1";
		vdso.relocated = 1;
		vdso.deps = (struct dso **)no_deps;
		decode_dyn(&vdso);
		vdso.prev = tail;
		tail->next = &vdso;
		tail = &vdso;
	}

	for (i=0; app.dynv[i]; i+=2) {
		if (!DT_DEBUG_INDIRECT && app.dynv[i]==DT_DEBUG)
			app.dynv[i+1] = (size_t)&debug;
		if (DT_DEBUG_INDIRECT && app.dynv[i]==DT_DEBUG_INDIRECT) {
			size_t *ptr = (size_t *) app.dynv[i+1];
			*ptr = (size_t)&debug;
		}
	}

	/* This must be done before final relocations, since it calls
	 * malloc, which may be provided by the application. Calling any
	 * application code prior to the jump to its entry point is not
	 * valid in our model and does not work with FDPIC, where there
	 * are additional relocation-like fixups that only the entry point
	 * code can see to perform. */
	main_ctor_queue = queue_ctors(&app);

	/* Initial TLS must also be allocated before final relocations
	 * might result in calloc being a call to application code. */
	update_tls_size();
	void *initial_tls = builtin_tls;
	if (libc.tls_size > sizeof builtin_tls || tls_align > MIN_TLS_ALIGN) {
		initial_tls = calloc(libc.tls_size, 1);
		if (!initial_tls) {
			dprintf(2, "%s: Error getting %zu bytes thread-local storage: %m\n",
				argv[0], libc.tls_size);
			_exit(127);
		}
	}
	static_tls_cnt = tls_cnt;

	/* The main program must be relocated LAST since it may contain
	 * copy relocations which depend on libraries' relocations. */
	reloc_all(app.next);
	reloc_all(&app);

	/* Actual copying to new TLS needs to happen after relocations,
	 * since the TLS images might have contained relocated addresses. */
	if (initial_tls != builtin_tls) {
		volatile pthread_t self = __pthread_self();

		if (__init_tp(__copy_tls(initial_tls)) < 0) {
			a_crash();
		}

		/* When using himalloc, himalloc_tcache_addr in struct pthread need to be copied */
		pthread_himalloc_tcache_addr_copy(self->himalloc_tcache_addr);
	} else {
		size_t tmp_tls_size = libc.tls_size;
		pthread_t self = __pthread_self();
		/* Temporarily set the tls size to the full size of
		 * builtin_tls so that __copy_tls will use the same layout
		 * as it did for before. Then check, just to be safe. */
		libc.tls_size = sizeof builtin_tls;
		if (__copy_tls((void*)builtin_tls) != self) a_crash();
		libc.tls_size = tmp_tls_size;
	}

	if (ldso_fail) _exit(127);
	if (ldd_mode) _exit(0);

	/* Determine if malloc was interposed by a replacement implementation
	 * so that calloc and the memalign family can harden against the
	 * possibility of incomplete replacement. */
	if (find_sym(head, "malloc", 1).dso != &ldso)
		__malloc_replaced = 1;
	if (find_sym(head, "aligned_alloc", 1).dso != &ldso)
		__aligned_alloc_replaced = 1;

	/* Switch to runtime mode: any further failures in the dynamic
	 * linker are a reportable failure rather than a fatal startup
	 * error. */
	runtime = 1;

	debug.ver = 1;
	debug.bp = dl_debug_state;
	debug.head = head;
	debug.base = ldso.base;
	debug.state = RT_CONSISTENT;
	_dl_debug_state();

	if (replace_argv0) argv[0] = replace_argv0;

	errno = 0;

	CRTJMP((void *)aux[AT_ENTRY], argv-1);
	for(;;);
}

static void prepare_lazy(struct dso *p, int from_mem_flag)
{
	size_t dyn[DYN_CNT], n, flags1=0;
	decode_vec(p->dynv, dyn, DYN_CNT);
	search_vec(p->dynv, &flags1, DT_FLAGS_1);
	if (dyn[DT_BIND_NOW] || (dyn[DT_FLAGS] & DF_BIND_NOW) || (!from_mem_flag && (flags1 & DF_1_NOW)))
		return;
	n = dyn[DT_RELSZ]/2 + dyn[DT_RELASZ]/3 + dyn[DT_PLTRELSZ]/2 + 1;
	if (NEED_MIPS_GOT_RELOCS) {
		size_t j=0; search_vec(p->dynv, &j, DT_MIPS_GOTSYM);
		size_t i=0; search_vec(p->dynv, &i, DT_MIPS_SYMTABNO);
		n += i-j;
	}
	p->lazy = calloc(n, 3*sizeof(size_t));
	if (!p->lazy) {
		error("Error preparing lazy relocation for %s: %m", p->name);
		longjmp(*rtld_fail, 1);
	}
	p->lazy_next = lazy_head;
	lazy_head = p;
}

int is_loading_ldk = 0;

static void release_freed_huge_pages(void)
{
	int fd;

	fd = open("/proc/sys/vm/nr_hugepages", O_RDWR);
	if (fd >= 0) {
		write(fd, "0\n", sizeof("0\n"));
		close(fd);
	}
}

void *dlopen(const char *file, int mode)
{
	struct dso *volatile p, *orig_tail, *orig_syms_tail, *orig_lazy_head, *next;
	struct tls_module *orig_tls_tail;
	size_t orig_tls_cnt, orig_tls_offset, orig_tls_align;
	size_t i;
	int cs;
	jmp_buf jb;
	struct dso **volatile ctor_queue = 0;

	if (!file) return head;
	if (runtime) {
		if (strstr(file, "libdh-linux.so.5.10")) {
			is_loading_ldk = 1;
			g_filename_bak = file;
			dprintf(2, "ldk-huge-mappings: %s\n", is_loading_ldk ? "on": "off");
		}
	}

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	pthread_rwlock_wrlock(&lock);
	__inhibit_ptc();

	debug.state = RT_ADD;
	_dl_debug_state();

	p = 0;
	if (shutting_down) {
		error("Cannot dlopen while program is exiting.");
		goto end;
	}
	orig_tls_tail = tls_tail;
	orig_tls_cnt = tls_cnt;
	orig_tls_offset = tls_offset;
	orig_tls_align = tls_align;
	orig_lazy_head = lazy_head;
	orig_syms_tail = syms_tail;
	orig_tail = tail;
	noload = mode & RTLD_NOLOAD;

	rtld_fail = &jb;
	if (setjmp(*rtld_fail)) {
		/* Clean up anything new that was (partially) loaded */
		revert_syms(orig_syms_tail);
		for (p=orig_tail->next; p; p=next) {
			next = p->next;
			while (p->td_index) {
				void *tmp = p->td_index->next;
				free(p->td_index);
				p->td_index = tmp;
			}
			free(p->funcdescs);
			if (p->rpath != p->rpath_orig)
				free(p->rpath);
			free(p->deps);
			unmap_library(p);
			free(p);
		}
		free(ctor_queue);
		ctor_queue = 0;
		if (!orig_tls_tail) libc.tls_head = 0;
		tls_tail = orig_tls_tail;
		if (tls_tail) tls_tail->next = 0;
		tls_cnt = orig_tls_cnt;
		tls_offset = orig_tls_offset;
		tls_align = orig_tls_align;
		lazy_head = orig_lazy_head;
		tail = orig_tail;
		tail->next = 0;
		p = 0;
		goto end;
	} else p = load_library(file, head);

	if (!p) {
		error(noload ?
			"Library %s is not already loaded" :
			"Error loading shared library %s: %m",
			file);
		goto end;
	}

	if (mode & RTLD_NODELETE) {
		p->flags |= DSO_FLAGS_NODELETE;
	}
	if (mode & RTLD_DEEPBIND) {
		p->flags |= DSO_FLAGS_DEEPBIND;
	}

	if (is_loading_ldk) {
		is_loading_ldk = 0;
		dprintf(2, "ldk-huge-mappings: turn off\n");
		release_freed_huge_pages();
	}

	/* First load handling */
	p->do_openclose = 1;
	load_deps(p);
	p->do_openclose = 0;
	extend_bfs_deps(p);
	pthread_mutex_lock(&init_fini_lock);
	int constructed = p->constructed;
	if (!constructed) ctor_queue = queue_ctors(p);
	pthread_mutex_unlock(&init_fini_lock);
	if (!p->relocated && (mode & RTLD_LAZY)) {
		prepare_lazy(p, 0);
		for (i=0; p->deps[i]; i++)
			if (!p->deps[i]->relocated)
				prepare_lazy(p->deps[i], 0);
	}
	if (!p->relocated || (mode & RTLD_GLOBAL)) {
		/* Make new symbols global, at least temporarily, so we can do
		 * relocations. If not RTLD_GLOBAL, this is reverted below. */
		add_syms(p);
		for (i=0; p->deps[i]; i++)
			add_syms(p->deps[i]);
	}
	if (!p->relocated) {
		reloc_all(p);
	}

	/* If RTLD_GLOBAL was not specified, undo any new additions
	 * to the global symbol table. This is a nop if the library was
	 * previously loaded and already global. */
	if (!(mode & RTLD_GLOBAL))
		revert_syms(orig_syms_tail);

	/* Processing of deferred lazy relocations must not happen until
	 * the new libraries are committed; otherwise we could end up with
	 * relocations resolved to symbol definitions that get removed. */
	redo_lazy_relocs();
	update_tls_size();
	if (tls_cnt != orig_tls_cnt)
		install_new_tls();
	orig_tail = tail;
end:
	debug.state = RT_CONSISTENT;
	_dl_debug_state();
	__release_ptc();
	if (p) gencnt++;
	pthread_rwlock_unlock(&lock);
	if (ctor_queue) {
		do_init_fini(ctor_queue);
		free(ctor_queue);
	}
	pthread_setcancelstate(cs, 0);
	return p;
}

void *dlopen5(const char *file, int mode, sym_reloc_hook reloc_hook,
	      void *reloc_ctx, sym_runtime_resolve_hook rt_resolve_hook)
{
	struct dso *p = NULL;
	int ret;
	struct sym_hook hook = {
		.reloc_hook = reloc_hook,
		.reloc_ctx = reloc_ctx,
		.rt_resolve_hook = rt_resolve_hook
	};

	pthread_rwlock_rdlock(&hook_key_lock);
	if (sym_hook_key == PTHREAD_KEYS_MAX) {
		pthread_rwlock_unlock(&hook_key_lock);
		pthread_rwlock_wrlock(&hook_key_lock);
		if (sym_hook_key == PTHREAD_KEYS_MAX) {
			ret = pthread_key_create(&sym_hook_key, NULL);
			if (ret) {
				pthread_rwlock_unlock(&hook_key_lock);
				return NULL;
			}
		}
	}
	pthread_rwlock_unlock(&hook_key_lock);

	pthread_setspecific(sym_hook_key, (void *)&hook);
	p = dlopen(file, mode);
	pthread_setspecific(sym_hook_key, NULL);

	/* fix GOT for runtime lazy binding */
	if ((p != NULL) && (hook.rt_resolve_hook != NULL)) {
		ret = mprotect(laddr(p, p->relro_start), p->relro_end - p->relro_start,
			       PROT_READ | PROT_WRITE);
		if (ret) {
			goto err;
		}
		p->got[0] = (size_t)p->dynv;	/* .dynamic */
		p->got[1] = (size_t)p;		/* struct link_map, a.k.a. struct dso */
		p->got[2] = (size_t)hook.rt_resolve_hook;
		ret = mprotect(laddr(p, p->relro_start), p->relro_end - p->relro_start,
			       PROT_READ);
		if (ret) {
			goto err;
		}
	}

	return (void *)p;
err:
	__dlclose(p);
	return NULL;
}

hidden int __dl_invalid_handle(void *h)
{
	struct dso *p;
	for (p=head; p; p=p->next) if (h==p) return 0;
	error("Invalid library handle %p", (void *)h);
	return 1;
}

static void *addr2dso(size_t a)
{
	struct dso *p;
	size_t i;
	if (DL_FDPIC) for (p=head; p; p=p->next) {
		i = count_syms(p);
		if (a-(size_t)p->funcdescs < i*sizeof(*p->funcdescs))
			return p;
	}
	for (p=head; p; p=p->next) {
		if (DL_FDPIC && p->loadmap) {
			for (i=0; i<p->loadmap->nsegs; i++) {
				if (a-p->loadmap->segs[i].p_vaddr
				    < p->loadmap->segs[i].p_memsz)
					return p;
			}
		} else {
			Phdr *ph = p->phdr;
			size_t phcnt = p->phnum;
			size_t entsz = p->phentsize;
			size_t base = (size_t)p->base;
			for (; phcnt--; ph=(void *)((char *)ph+entsz)) {
				if (ph->p_type != PT_LOAD) continue;
				if (a-base-ph->p_vaddr < ph->p_memsz)
					return p;
			}
			if (a-(size_t)p->map < p->map_len)
				return 0;
		}
	}
	return 0;
}

static void *do_dlsym(struct dso *p, const char *s, void *ra)
{
	int use_deps = 0;
	if (p == head || p == RTLD_DEFAULT) {
		p = head;
	} else if (p == RTLD_NEXT) {
		p = addr2dso((size_t)ra);
		if (!p) p=head;
		p = p->next;
	} else if (__dl_invalid_handle(p)) {
		return 0;
	} else
		use_deps = 1;
	struct symdef def = find_sym2(p, s, 0, use_deps);
	if (!def.sym) {
		error("Symbol not found: %s", s);
		return 0;
	}
	if ((def.sym->st_info&0xf) == STT_TLS)
		return __tls_get_addr((tls_mod_off_t []){def.dso->tls_id, def.sym->st_value-DTP_OFFSET});
	if (DL_FDPIC && (def.sym->st_info&0xf) == STT_FUNC)
		return def.dso->funcdescs + (def.sym - def.dso->syms);
	return laddr(def.dso, def.sym->st_value);

}

static void free_name_ext(struct dso *p)
{
	unsigned int i;

	for (i = 0; i < p->nr_ext; i++) {
		free(p->name_ext[i]);
	}
}

static int do_dlclose(struct dso *p)
{
	size_t n;
	struct dso *d;

	if (__dl_invalid_handle(p))
		return -1;

	/*
	 * to be consistent with linux, process invokes dlclose the linked so
	 * which has been dlopen again. success should be returned.
	 */
	if (p->nr_dlopen > 1) {
		(void)a_fetch_add(&p->nr_dlopen, -1);
		return 0;
	}

	if (!p->by_dlopen) {
		error("Library %s is not loaded by dlopen", p->name);
		return -1;
	}

	/* dso is marked  as RTLD_NODELETE library, do nothing here. */
	if ((p->flags & DSO_FLAGS_NODELETE) != 0) {
		return 0;
	}

	/* avoid depending each other */
	if (p->do_openclose != 0) {
		return 0;
	}

	(void)a_fetch_add(&p->nr_dlopen, -1);

	/* call destructors if needed */
	if (p->constructed) {
		size_t dyn[DYN_CNT];
		decode_vec(p->dynv, dyn, DYN_CNT);
		if (dyn[0] & (1<<DT_FINI_ARRAY)) {
			n = dyn[DT_FINI_ARRAYSZ] / sizeof(size_t);
			size_t *fn = (size_t *)laddr(p, dyn[DT_FINI_ARRAY]) + n;
			while (n--)
				((void (*)(void))*--fn)();
		}
		p->constructed = 0;
	}

	if (p->deps != no_deps) {
		p->do_openclose = 1;
		for (unsigned i = 0; i < p->ndeps_direct; ++i) {
			(void)do_dlclose(p->deps[i]);
		}
	}

	/* remove dso symbols from global list */
	if (p->syms_next) {
		for (d = head; d->syms_next != p; d = d->syms_next)
			; /* NOP */
		d->syms_next = p->syms_next;
	} else if (p == syms_tail) {
		for (d = head; d->syms_next != p; d = d->syms_next)
			; /* NOP */
		d->syms_next = NULL;
		syms_tail = d;
	}

	/* remove dso from lazy list if needed */
	if (p == lazy_head) {
		lazy_head = p->lazy_next;
	} else if (p->lazy_next) {
		for (d = lazy_head; d->lazy_next != p; d = d->lazy_next)
			; /* NOP */
		d->lazy_next = p->lazy_next;
	}

	/* remove dso from fini list */
	if (p == fini_head) {
		fini_head = p->fini_next;
	} else if (p->fini_next) {
		for (d = fini_head; d->fini_next != p; d = d->fini_next)
			; /* NOP */
		d->fini_next = p->fini_next;
	}

	/* remove dso from global dso list */
	if (p == tail) {
		tail = p->prev;
		tail->next = NULL;
	} else {
		p->next->prev = p->prev;
		p->prev->next = p->next;
	}

	if (p->lazy != NULL)
		free(p->lazy);
	if (p->deps != no_deps)
		free(p->deps);
	unmap_library(p);

	free_name_ext(p);

	free(p);

	return 0;
}

hidden int __dlclose(void *p)
{
	int rc;
	pthread_rwlock_wrlock(&lock);
	__inhibit_ptc();
	rc = do_dlclose(p);
	__release_ptc();
	pthread_rwlock_unlock(&lock);
	return rc;
}

int dladdr(const void *addr_arg, Dl_info *info)
{
	size_t addr = (size_t)addr_arg;
	struct dso *p;
	Sym *sym, *bestsym = NULL;
	uint32_t nsym;
	char *strings;
	size_t best = 0;
	size_t besterr = -1;

	if (!info) return 0;

	pthread_rwlock_rdlock(&lock);
	p = addr2dso(addr);
	pthread_rwlock_unlock(&lock);

	if (!p) return 0;

	sym = p->syms;
	strings = p->strings;
	nsym = count_syms(p);

	if (DL_FDPIC) {
		size_t idx = (addr-(size_t)p->funcdescs)
			/ sizeof(*p->funcdescs);
		if (idx < nsym && (sym[idx].st_info&0xf) == STT_FUNC) {
			best = (size_t)(p->funcdescs + idx);
			bestsym = sym + idx;
			besterr = 0;
		}
	}

	if (!best) for (; nsym; nsym--, sym++) {
		if (sym->st_value
		 && (1<<(sym->st_info&0xf) & OK_TYPES)
		 && (1<<(sym->st_info>>4) & OK_BINDS)) {
			size_t symaddr = (size_t)laddr(p, sym->st_value);
			if (symaddr > addr || symaddr <= best)
				continue;
			best = symaddr;
			bestsym = sym;
			besterr = addr - symaddr;
			if (addr == symaddr)
				break;
		}
	}

	if (best && besterr > bestsym->st_size-1) {
		best = 0;
		bestsym = 0;
	}

	info->dli_fname = p->name;
	info->dli_fbase = p->map;

	if (!best) {
		info->dli_sname = 0;
		info->dli_saddr = 0;
		return 1;
	}

	if (DL_FDPIC && (bestsym->st_info&0xf) == STT_FUNC)
		best = (size_t)(p->funcdescs + (bestsym - p->syms));
	info->dli_sname = strings + bestsym->st_name;
	info->dli_saddr = (void *)best;

	return 1;
}

hidden void *__dlsym(void *restrict p, const char *restrict s, void *restrict ra)
{
	void *res;

	if (!s) return 0;
	pthread_rwlock_rdlock(&lock);
	res = do_dlsym(p, s, ra);
	pthread_rwlock_unlock(&lock);
	return res;
}

hidden void *__dlsym_redir_time64(void *restrict p, const char *restrict s, void *restrict ra)
{
#if _REDIR_TIME64
	const char *suffix, *suffix2 = "";
	char redir[36];

	/* Map the symbol name to a time64 version of itself according to the
	 * pattern used for naming the redirected time64 symbols. */
	size_t l = strnlen(s, sizeof redir);
	if (l<4 || l==sizeof redir) goto no_redir;
	if (s[l-2]=='_' && s[l-1]=='r') {
		l -= 2;
		suffix2 = s+l;
	}
	if (l<4) goto no_redir;
	if (!strcmp(s+l-4, "time")) suffix = "64";
	else suffix = "_time64";

	/* Use the presence of the remapped symbol name in libc to determine
	 * whether it's one that requires time64 redirection; replace if so. */
	snprintf(redir, sizeof redir, "__%.*s%s%s", (int)l, s, suffix, suffix2);
	if (find_sym(&ldso, redir, 1).sym) s = redir;
no_redir:
#endif
	return __dlsym(p, s, ra);
}

int dl_iterate_phdr(int(*callback)(struct dl_phdr_info *info, size_t size, void *data), void *data)
{
	struct dso *current;
	struct dl_phdr_info info;
	int ret = 0;

	if (!callback) return -EINVAL;

	for(current = head; current;) {
		info.dlpi_addr      = (uintptr_t)current->base;
		info.dlpi_name      = current->name;
		info.dlpi_phdr      = current->phdr;
		info.dlpi_phnum     = current->phnum;
		info.dlpi_adds      = gencnt;
		info.dlpi_subs      = 0;
		info.dlpi_tls_modid = current->tls_id;
		info.dlpi_tls_data = !current->tls_id ? 0 :
			__tls_get_addr((tls_mod_off_t[]){current->tls_id,0});

		ret = (callback)(&info, sizeof (info), data);

		if (ret != 0) break;

		pthread_rwlock_rdlock(&lock);
		current = current->next;
		pthread_rwlock_unlock(&lock);
	}
	return ret;
}

static void error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if (!runtime) {
		vdprintf(2, fmt, ap);
		dprintf(2, "\n");
		ldso_fail = 1;
		va_end(ap);
		return;
	}
	__dl_vseterr(fmt, ap);
	va_end(ap);
}

static void *alloc_library(const void *mem, struct dso *dso)
{
	size_t phsize;
	size_t addr_min = SIZE_MAX, addr_max = 0, map_len;
	size_t this_min, this_max;
	off_t off_start;
	Ehdr *eh;
	Phdr *ph, *ph0;
	unsigned prot;
#ifdef CONFIG_ARM64_BTI_USER
	unsigned ext_prot = 0;
#endif
	unsigned char *map = MAP_FAILED, *base;
	size_t dyn = 0;
	size_t tls_image = 0;
	size_t i;

	eh = (Ehdr *)mem;
	phsize = (size_t)eh->e_phentsize * (size_t)eh->e_phnum;
	ph = ph0 = (Phdr *)((const char *)mem + eh->e_phoff);

	/* found library [min-max] virtual address */
	for (i = eh->e_phnum; i; i--, ph = (void *)((char *)ph + eh->e_phentsize)) {
		if (ph->p_type == PT_DYNAMIC) {
			dyn = ph->p_vaddr;
		} else if (ph->p_type == PT_TLS) {
			tls_image = ph->p_vaddr;
			dso->tls.align = ph->p_align;
			dso->tls.len = ph->p_filesz;
			dso->tls.size = ph->p_memsz;
		} else if (ph->p_type == PT_GNU_RELRO) {
			dso->relro_start = ph->p_vaddr & -PAGE_SIZE;
			dso->relro_end = (ph->p_vaddr + ph->p_memsz) & -PAGE_SIZE;
		}
#ifdef CONFIG_ARM64_BTI_USER
		if (ph->p_type == PT_GNU_PROPERTY || ph->p_type == PT_NOTE)
			ext_prot = parse_extra_prot_mem((const char *)mem, ph);
#endif
		if (ph->p_type != PT_LOAD) continue;
		if (ph->p_vaddr < addr_min) {
			addr_min = ph->p_vaddr;
			off_start = ph->p_offset;
		}
		if (ph->p_vaddr + ph->p_memsz > addr_max) {
			addr_max = ph->p_vaddr + ph->p_memsz;
		}
	}

	/* .dynamic section is not found, maybe not dynamic library */
	if (!dyn) goto noexec;

	addr_max += PAGE_SIZE - 1;
	addr_max &= -PAGE_SIZE;
	addr_min &= -PAGE_SIZE;
	off_start &= -PAGE_SIZE;
	map_len = addr_max - addr_min + off_start;

	/* The first time, we map too much, possibly even more than
	 * the length of the file. This is okay because we will not
	 * use the invalid part; we just need to reserve the right
	 * amount of virtual address space to map over later. */
	map = mmap((void *)addr_min, map_len, PROT_NONE,
		   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED) goto error;
	dso->map = map;
	dso->map_len = map_len;
	/* If the loaded file is not relocatable and the requested address is
	 * not available, then the load operation must fail. */
	if (eh->e_type != ET_DYN && addr_min && map != (void *)addr_min) {
		errno = EBUSY;
		goto error;
	}
	base = map - addr_min;
	dso->phdr = 0;
	dso->phnum = 0;
	for (ph = ph0, i = eh->e_phnum; i;
	     i--, ph = (void *)((char *)ph + eh->e_phentsize)) {
		if (ph->p_type != PT_LOAD) continue;
		/* Check if the programs headers are in this load segment, and
		 * if so, record the address for use by dl_iterate_phdr. */
		if (!dso->phdr && eh->e_phoff >= ph->p_offset
		    && eh->e_phoff + phsize <= ph->p_offset + ph->p_filesz) {
			dso->phdr = (void *)(base + ph->p_vaddr
					     + (eh->e_phoff - ph->p_offset));
			dso->phnum = eh->e_phnum;
			dso->phentsize = eh->e_phentsize;
		}
		this_min = ph->p_vaddr & -PAGE_SIZE;
		this_max = ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1 & -PAGE_SIZE;
		off_start = ph->p_offset & -PAGE_SIZE;
		prot = (((ph->p_flags & PF_R) ? PROT_READ : 0) |
			((ph->p_flags & PF_W) ? PROT_WRITE : 0) |
			((ph->p_flags & PF_X) ? PROT_EXEC : 0));
#ifdef CONFIG_ARM64_BTI_USER
		prot |= ext_prot;
#endif
		if (mmap(base + this_min, this_max - this_min, prot | PROT_WRITE,
			 MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0) == MAP_FAILED)
			goto error;
		/* NOTE: mmap with MAP_ANONYMOUS flags will memset memory to zero by default
		 *       so we just need to copy filesz to target memory, don't care about bss. */
		memcpy(base + this_min, (const char *)mem + off_start,
		       ph->p_filesz + (ph->p_vaddr & (PAGE_SIZE - 1)));
		/* no write permition */
		if (!(ph->p_flags & PF_W) &&
		    mprotect(base + this_min, this_max - this_min, prot))
			goto error;
	}

	for (i = 0; ((size_t *)(base + dyn))[i]; i += 2) {
		if (((size_t *)(base + dyn))[i] == DT_TEXTREL) {
			if (mprotect(map, map_len, PROT_READ | PROT_WRITE | PROT_EXEC)
			    && errno != ENOSYS)
				goto error;
			break;
		}
	}

	dso->base = base;
	dso->dynv = laddr(dso, dyn);
	if (dso->tls.size) dso->tls.image = laddr(dso, tls_image);
	return map;
noexec:
	errno = ENOEXEC;
error:
	if (map != MAP_FAILED) unmap_library(dso);
	return 0;
}

static struct dso *load_library_from_mem(const void *mem, const char *name,
					 struct dso *needed_by)
{
	unsigned char *map;
	struct dso *p, temp_dso = {0};
	size_t alloc_size;
	int n_th = 0;

	map = alloc_library(mem, &temp_dso);
	if (!map) return 0;

	decode_dyn(&temp_dso);

	/* Allocate storage for the new DSO. When there is TLS, this
	 * storage must include a reservation for all pre-existing
	 * threads to obtain copies of both the new TLS, and an
	 * extended DTV capable of storing an additional slot for
	 * the newly-loaded DSO. */
	alloc_size = sizeof *p + strlen(name) + 1;
	if (temp_dso.tls.image) {
		size_t per_th = temp_dso.tls.size + temp_dso.tls.align
				+ sizeof(void *) * (tls_cnt + 3);
		n_th = libc.threads_minus_1 + 1;
		if (n_th > SSIZE_MAX / per_th) alloc_size = SIZE_MAX;
		else alloc_size += n_th * per_th;
	}
	p = calloc(1, alloc_size);
	if (!p) {
		unmap_library(&temp_dso);
		return 0;
	}
	memcpy(p, &temp_dso, sizeof temp_dso);
	p->needed_by = needed_by;
	p->name = p->buf;
	p->nr_dlopen = 1;
	p->runtime_loaded = runtime;
	strcpy(p->name, name);
	if (p->tls.image) {
		p->tls_id = ++tls_cnt;
		tls_align = MAXP2(tls_align, p->tls.align);
#ifdef TLS_ABOVE_TP
		p->tls.offset = tls_offset + ((tls_align - 1) &
					      -(tls_offset + (uintptr_t)p->tls.image));
		tls_offset = p->tls.offset + p->tls.size;
#else
		tls_offset += p->tls.size + p->tls.align - 1;
		tls_offset -= (tls_offset + (uintptr_t)p->tls.image)
			      & (p->tls.align - 1);
		p->tls.offset = tls_offset;
#endif
		p->new_dtv = (void *)(-sizeof(size_t) &
				      (uintptr_t)(p->name + strlen(p->name) + sizeof(size_t)));
		p->new_tls = (void *)(p->new_dtv + n_th * (tls_cnt + 1));
		if (tls_tail) tls_tail->next = &p->tls;
		else libc.tls_head = &p->tls;
		tls_tail = &p->tls;
	}

	tail->next = p;
	p->prev = tail;
	tail = p;
	p->by_dlopen = 1;
	/* dso loaded by dlopen_from_mem should not have any deps */
	p->deps = (struct dso **)no_deps;
	__fill_dso_syminfo(p);
	return p;
}

void *dlopen_from_mem(const void *mem, const char *name, int mode)
{
	int cs;
	struct tls_module *orig_tls_tail;
	struct dso **volatile ctor_queue = 0;
	size_t orig_tls_cnt, orig_tls_offset, orig_tls_align;
	struct dso *volatile p, *orig_tail, *orig_syms_tail, *orig_lazy_head, *next;
	jmp_buf jb;

	if (!mem) return NULL;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	pthread_rwlock_wrlock(&lock);
	__inhibit_ptc();

	p = 0;
	if (shutting_down) {
		error("Cannot dlopen while program is exiting.");
		goto end;
	}
	orig_tls_tail = tls_tail;
	orig_tls_cnt = tls_cnt;
	orig_tls_offset = tls_offset;
	orig_tls_align = tls_align;
	orig_lazy_head = lazy_head;
	orig_syms_tail = syms_tail;
	orig_tail = tail;
	noload = (unsigned int)mode & RTLD_NOLOAD;

	rtld_fail = &jb;
	if (setjmp(*rtld_fail)) {
		/* Chop off symbols of the new dso */
		revert_syms(orig_syms_tail);
		/*
		 * Assuming dependence loading disabled, the loop should
		 * execute only once
		 */
		for (p=orig_tail->next; p; p=next) {
			next = p->next;
			while (p->td_index) {
				void *tmp = p->td_index->next;
				free(p->td_index);
				p->td_index = tmp;
			}
			free(p->funcdescs);
			if (p->rpath != p->rpath_orig)
				free(p->rpath);
			/*
			 * Does not free p->deps because
			 * dlopen_from_mem should not have any deps
			 */
			unmap_library(p);
			free(p);
		}
		free(ctor_queue);
		ctor_queue = 0;
		if (!orig_tls_tail) libc.tls_head = 0;
		tls_tail = orig_tls_tail;
		if (tls_tail) tls_tail->next = 0;
		tls_cnt = orig_tls_cnt;
		tls_offset = orig_tls_offset;
		tls_align = orig_tls_align;
		lazy_head = orig_lazy_head;
		tail = orig_tail;
		tail->next = 0;
		p = 0;
		goto end;
	} else p = load_library_from_mem(mem, name, head);
	if (!p) {
		error(noload ?
		      "Library [%s] is not already loaded" :
		      "Error loading shared library %s: %m"
		     , name);
		goto end;
	}

	pthread_mutex_lock(&init_fini_lock);
	if (!p->constructed) ctor_queue = queue_ctors(p);
	pthread_mutex_unlock(&init_fini_lock);
	if (!p->relocated && ((unsigned int)mode & RTLD_LAZY)) {
		prepare_lazy(p, 1);
	}
	if (!p->relocated || ((unsigned int)mode & RTLD_GLOBAL)) {
		/* Make new symbols global, at least temporarily, so we can do
		 * relocations. If not RTLD_GLOBAL, this is reverted below. */
		add_syms(p);
	}

	if (!p->relocated) {
		reloc_all(p);
	}

	/* If RTLD_GLOBAL was not specified, undo any new additions
	 * to the global symbol table. This is a nop if the library was
	 * previously loaded and already global. */
	if (!((unsigned int)mode & RTLD_GLOBAL))
		revert_syms(orig_syms_tail);

	/* Processing of deferred lazy relocations must not happen until
	 * the new libraries are committed; otherwise we could end up with
	 * relocations resolved to symbol definitions that get removed. */
	redo_lazy_relocs();

	update_tls_size();
	if (tls_cnt != orig_tls_cnt)
		install_new_tls();
	_dl_debug_state();
	orig_tail = tail;
end:
	__release_ptc();
	if (p) gencnt++;
	pthread_rwlock_unlock(&lock);
	if (ctor_queue) {
		do_init_fini(ctor_queue);
		free(ctor_queue);
	}
	pthread_setcancelstate(cs, 0);
	return p;
}

/*
 * If fork is invoked in the constructor/destructor and then exit is invoked
 * by the subprocess, the subprocess will always wait on init_fini_lock or
 * pthread_cond_wait(&ctor_cond, &init_fini_lock).
 * So need to clean it up after fork.
 */
void __child_clean_ctor(void)
{
       struct dso *p;

       pthread_mutex_init(&init_fini_lock, NULL);
       for (p = fini_head; p; p = p->fini_next) {
               p->ctor_visitor = 0;
               p->constructed = 1;
       }
}

void __reclaim_hmldso(void)
{
	int i;
	Ehdr *eh;
	Phdr *ph;

	if (dso_syminfo == NULL) {
		return;
	}

	eh = (Ehdr *)(unsigned long)dso_syminfo->hmldso_elf_entry.base;
	ph = (Phdr *)((char *)eh + eh->e_phoff);
	for (i = eh->e_phnum; i > 0; i--, ph = (void *)((char *)ph + eh->e_phentsize)) {
		if (ph->p_type != PT_LOAD) continue;
		if (ph->p_flags & PF_W) {
			char *start = (char *)(((unsigned long)eh + ph->p_vaddr) & ~(PAGE_SIZE - 1));
			char *end = (char *)ALIGN((unsigned long)eh + ph->p_vaddr + ph->p_memsz, (unsigned long)PAGE_SIZE);
			munmap(start, (size_t)(end - start));
		}
	}
}
