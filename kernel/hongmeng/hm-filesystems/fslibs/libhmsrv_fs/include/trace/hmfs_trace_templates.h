/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: hmfs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 2 15:30:54 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#define HMFS_STRUCT_TRACE_PRINT_FLAG_U64_DEF	\
struct trace_print_flags_u64 {			\
	unsigned long long	mask;		\
	const char		*name;		\
}

#define __assign_symbolic(_dst, _value, symbol_array...) do {	\
	HMFS_STRUCT_TRACE_PRINT_FLAG_U64_DEF;			\
	static const struct trace_print_flags_u64 symbols[] = {symbol_array, {-1, NULL}};	\
	__assign_str(_dst, NULL);				\
	for (int i = 0; symbols[i].name; i++) {			\
		if (_value == symbols[i].mask) {		\
			__assign_str(_dst, symbols[i].name)	\
		}						\
	}							\
} while (0)

#define show_dev(dev)		((unsigned int) ((dev) >> 20)), ((unsigned int) ((dev) & ((1U << 20) - 1)))
#define show_dev_index(entry)   show_dev((entry)->dev), ((unsigned long)((entry)->index))
#define show_bio_op(op)		((op) & FS_IO_OPT_MASK)

#define LONGEST_FILE_DATA_TYPE_TEMPLATE	"Cold NODE"
#define assign_data_type(_dst, _type)					\
	__assign_symbolic(_dst, _type,					\
		{ CURCHU_HOT_DATA, 	"Hot DATA" },			\
		{ CURCHU_WARM_DATA, 	"Warm DATA" },			\
		{ CURCHU_COLD_DATA, 	"Cold DATA" },			\
		{ CURCHU_HOT_NODE, 	"Hot NODE" },			\
		{ CURCHU_WARM_NODE, 	"Warm NODE" },			\
		{ CURCHU_COLD_NODE, 	"Cold NODE" },			\
		{ NO_CHECK_TYPE, 	"No TYPE" })

#define LONGEST_FILE_GC_TYPE_TEMPLATE	"Foreground GC"
#define assign_gc_type(_dst, _type)						\
	__assign_symbolic(_dst, _type,						\
		{ FG_GC,	"Foreground GC" },			\
		{ BG_GC,	"Background GC" })

#define LONGEST_FILE_ALLOC_MODE_TEMPLATE	"AT_SSR-mode"
#define assign_alloc_mode(_dst, _type)						\
	__assign_symbolic(_dst, _type,						\
		{ LFS,		"LFS-mode" },				\
		{ SSR,		"SSR-mode" },				\
		{ AT_SSR,	"AT_SSR-mode" })

#define LONGEST_FILE_VICTIM_POLICY_TEMPLATE	"Age-threshold"
#define assign_victim_policy(_dst, _type)					\
	__assign_symbolic(_dst, _type,						\
		{ HMFS_GC_GREEDY,	"Greedy" },			\
		{ HMFS_GC_CB,		"Cost-Benefit" },		\
		{ HMFS_GC_AT,		"Age-threshold" })

#define MAX_FILE_MODE_TEMPLATE	"FILE"
#define ASIGN_FILE_MODE(_dst, _type)	\
if ((_type) == 0) {		\
	__assign_str(_dst, "FILE");	\
} else if ((_type) == 1) {	\
	__assign_str(_dst, "DIR");	\
} else {				\
	__assign_str(_dst, NULL);	\
}

#define MAX_BLOCK_TYPE_TEMPLATE	"OUT-OF-PLACE"
#define ASIGN_BLOCK_TYPE(_dst, _type)			\
__assign_symbolic(_dst, _type,				\
		{ DATA,		"NODE" },		\
		{ NODE,		"DATA" },		\
		{ META,		"META" },		\
		{ META_FLUSH,	"META_FLUSH" },		\
		{ IPU,		"IN-PLACE" },		\
		{ OPU,		"OUT-OF-PLACE" })

#define MAX_FSCACHE_TYPE_TEMPLATE	"JOURNAL"
#define ASIGN_FSCACHE_TYPE_TEMP(_dst, _type)	\
if ((_type) == FC_HANDLE_TYPE_SYNC) {		\
	__assign_str(_dst, "SYNC");	\
} else if ((_type) == FC_HANDLE_TYPE_WB) {	\
	__assign_str(_dst, "WB");	\
} else if ((_type) == FC_HANDLE_TYPE_JOURNAL) {	\
	__assign_str(_dst, "JOURNAL");	\
} else {				\
	__assign_str(_dst, NULL);	\
}

#define MAX_TEMP_TYPE_TEMPLATE		"COLD"
#define ASIGN_TEMP_TYPE(_dst, _type) 	\
if ((_type) == HOT) {			\
	__assign_str(_dst, "HOT");	\
} else if ((_type) == WARM) {		\
	__assign_str(_dst, "WARM");	\
} else if ((_type) == COLD) {		\
	__assign_str(_dst, "COLD");	\
} else {				\
	__assign_str(_dst, NULL);	\
}

#define ASIGN_FS_IO_OP_TYPE_READ(_dst, _type)	\
if ((_type) == FS_IO_READ) {			\
	__assign_str(_dst, "READ");		\
}

#define ASIGN_FS_IO_OP_TYPE_WRITE(_dst, _type)	\
if ((_type) == FS_IO_WRITE) {			\
	__assign_str(_dst, "WRITE");		\
}

#define ASIGN_FS_IO_OP_TYPE_DISCARD(_dst, _type)	\
if ((_type) == FS_IO_DISCARD) {			\
	__assign_str(_dst, "DISCARD");			\
}

#define ASIGN_FS_IO_OP_TYPE_GC(_dst, _type)	\
if ((_type) == FS_IO_GC) {			\
	__assign_str(_dst, "GC");		\
}

#define MAX_FS_IO_OP_TEMPLATE		"UNKNOWN"
#define ASIGN_FS_IO_OP_TYPE(_dst, _type)		\
do {							\
	__assign_str(_dst, NULL);			\
	ASIGN_FS_IO_OP_TYPE_READ(_dst, _type)		\
	ASIGN_FS_IO_OP_TYPE_WRITE(_dst, _type)		\
	ASIGN_FS_IO_OP_TYPE_DISCARD(_dst, _type)	\
	ASIGN_FS_IO_OP_TYPE_GC(_dst, _type)		\
} while (0)

#define TRACE_EVENT_VNODE_TEMPLATE_TP_FIELD	TP_FIELD(	\
	__field(dev_t,	dev)			\
	__field(unsigned long,	index)		\
	__field(unsigned long,	pino)		\
	__field(mode_t, mode)			\
	__field(long long,	size)		\
	__field(unsigned int, nlink)		\
	__field(unsigned long long, blocks)	\
	__field(unsigned char,	advise)		\
)

#define TP_ASSIGN_DEV_INDEX_TEMPLATE					\
if (vn != NULL) {							\
	__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;	\
	__entry->index = vn->index;					\
}

#define TRACE_EVENT_VNODE_TEMPLATE_ASSIGN_ENTRY		\
do {							\
	__entry->pino	= pino;				\
	__entry->mode	= vn->mode;			\
	__entry->nlink	= vn->lnkcnt;			\
	__entry->size	= vn->size;			\
	__entry->blocks	= vn->blocks;			\
	__entry->advise	= advise;			\
} while (0)

#define TRACE_EVENT_VNODE_TEMPLATE_TP_PRINT	\
TP_PRINT("dev = (%d,%d), ino = %lu, pino = %lu, mode = 0x%hx, "		\
	"size = %lld, linkcnt = %u, blocks = %llu, advise = 0x%x",	\
	show_dev_index(__entry),					\
	(unsigned long)__entry->pino,					\
	__entry->mode,							\
	__entry->size,							\
	(unsigned int)__entry->nlink,					\
	(unsigned long long)__entry->blocks,				\
	(unsigned char)__entry->advise)

#define TRACE_EVENT_VNODE_TEMPLATE(group, name) \
TRACE_EVENT(group, name,								\
	TP_PROTO(vfs_node_t *, vn, unsigned long, pino, unsigned char, advise), TP_ARGS(vn, pino, advise), \
	TRACE_EVENT_VNODE_TEMPLATE_TP_FIELD,						\
	TP_ASSIGN(									\
		TP_ASSIGN_DEV_INDEX_TEMPLATE						\
		TRACE_EVENT_VNODE_TEMPLATE_ASSIGN_ENTRY;				\
	),										\
	TRACE_EVENT_VNODE_TEMPLATE_TP_PRINT						\
)

#define TRACE_EVENT_PAGE_TEMPLATE_TP_FIELD			\
TP_FIELD(							\
	__field(dev_t,		dev)				\
	__field(ino_t,		index)				\
	__string(pgtype, 	MAX_BLOCK_TYPE_TEMPLATE)	\
	__string(dir, 		MAX_FILE_MODE_TEMPLATE)		\
	__field(unsigned long, 	page_index)			\
	__field(int, 		dirty)				\
	__field(int, 		uptodate)			\
)

#define TRACE_EVENT_PAGE_TEMPLATE_ASSIGN_PAGE							\
if (page != NULL) {										\
	if (FSCACHE_NODE(PAGE_MAPPING(page)) != NULL) {						\
		__entry->dev	= FSCACHE_NODE(PAGE_MAPPING(page))->i_sb != NULL ?			\
					FSCACHE_NODE(PAGE_MAPPING(page))->i_sb->s_dev_id : 0;	\
		__entry->index	= FSCACHE_NODE(PAGE_MAPPING(page))->index;				\
	}											\
	__entry->page_index	= PAGE_INDEX(page);							\
}

#define TRACE_EVENT_PAGE_TEMPLATE_ASSIGN_ENTRY					\
do {										\
	ASIGN_BLOCK_TYPE(pgtype, type);						\
	ASIGN_FILE_MODE(dir, S_ISDIR(FSCACHE_NODE(PAGE_MAPPING(page))->mode));	\
	__entry->dirty	= page_dirty(page);					\
	__entry->uptodate = page_uptodate(page);				\
} while (0)

#define TRACE_EVENT_PAGE_TEMPLATE_TP_PRINT				\
TP_PRINT("dev = (%d,%d), ino = %lu, %s, %s, page_index = %lu, "		\
	"dirty = %d, uptodate = %d",					\
	show_dev_index(__entry),					\
	__get_str(pgtype),						\
	__get_str(dir),							\
	(unsigned long)__entry->page_index,				\
	__entry->dirty,							\
	__entry->uptodate)

#define TRACE_EVENT_PAGE_TEMPLATE(group, name)				\
TRACE_EVENT(group, name,						\
	TP_PROTO(fscache_page_t *, page, int, type), TP_ARGS(page, type),	\
	TRACE_EVENT_PAGE_TEMPLATE_TP_FIELD,				\
	TP_ASSIGN(							\
		TRACE_EVENT_PAGE_TEMPLATE_ASSIGN_PAGE			\
		TRACE_EVENT_PAGE_TEMPLATE_ASSIGN_ENTRY;			\
	),								\
	TRACE_EVENT_PAGE_TEMPLATE_TP_PRINT				\
)

#define TRACE_EVENT_BIO_TEMPLATE_TP_FIELD			\
TP_FIELD(							\
	__field(dev_t,		dev)				\
	__field(dev_t,		target)				\
	__field(int,		op_flags)			\
	__field(uint64_t,	sector)				\
	__field(unsigned int,	size)				\
	__string(pgtype, 	MAX_BLOCK_TYPE_TEMPLATE)	\
	__string(fsop,		MAX_FS_IO_OP_TEMPLATE)		\
)

#define TRACE_EVENT_BIO_TEMPLATE_TP_ASSIGN			\
do {								\
	__entry->dev 		= s_dev_id;			\
	__entry->target 	= dev_id;			\
	__entry->op_flags 	= fi_flags;			\
	__entry->sector 	= fi_sector;			\
	__entry->size 		= size;				\
	ASIGN_BLOCK_TYPE(pgtype, type);				\
	ASIGN_FS_IO_OP_TYPE(fsop, show_bio_op(fi_flags));	\
} while (0)

#define TRACE_EVENT_BIO_TEMPLATE_PRINT					\
TP_PRINT("dev = (%d,%d)/(%d,%d), rw = %s, op_flags = %d, type = %s,"	\
	" sector = %ld, size = %u",					\
	show_dev(__entry->target),					\
	show_dev(__entry->dev),						\
	__get_str(fsop),						\
	__entry->op_flags,						\
	__get_str(pgtype),						\
	(unsigned long)__entry->sector,					\
	__entry->size							\
)

#define TRACE_EVENT_BIO_TEMPLATE(group, name) 				\
TRACE_EVENT(group, name,						\
	TP_PROTO(dev_t, s_dev_id, dev_t, dev_id, unsigned int, fi_flags,\
		uint64_t, fi_sector, unsigned int, size, int, type),	\
	TP_ARGS(s_dev_id, dev_id, fi_flags, fi_sector, size, type),	\
	TRACE_EVENT_BIO_TEMPLATE_TP_FIELD,				\
	TP_ASSIGN(TRACE_EVENT_BIO_TEMPLATE_TP_ASSIGN;),			\
	TRACE_EVENT_BIO_TEMPLATE_PRINT					\
)

#define TRACE_EVENT_DISCARD_TEMPLATE_TP_FIELD		\
TP_FIELD(						\
	__field(dev_t,			dev)		\
	__field(unsigned long long, 	blkstart)	\
	__field(unsigned long long,	 blklen)	\
)

#define TRACE_EVENT_DISCARD_TEMPLATE_TP_ASSIGN	\
do {						\
	__entry->dev 		= dev_id;	\
	__entry->blkstart 	= blkstart;	\
	__entry->blklen		= blklen;	\
} while (0)

#define TRACE_EVENT_DISCARD_TEMPLATE_TP_PRINT			\
TP_PRINT("dev = (%d,%d), blkstart = 0x%llx, blklen = 0x%llx",	\
	show_dev(__entry->dev),					\
	(unsigned long long)__entry->blkstart,			\
	(unsigned long long)__entry->blklen			\
)

#define TRACE_EVENT_DISCARD_TEMPLATE(group, name)		\
TRACE_EVENT(group, name,					\
	TP_PROTO(dev_t, dev_id, unsigned long long, blkstart,	\
		unsigned long long, blklen),			\
	TP_ARGS(dev_id, blkstart, blklen),			\
	TRACE_EVENT_DISCARD_TEMPLATE_TP_FIELD,			\
	TP_ASSIGN(TRACE_EVENT_DISCARD_TEMPLATE_TP_ASSIGN;),	\
	TRACE_EVENT_DISCARD_TEMPLATE_TP_PRINT			\
)

#define TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE_TP_FIELD	\
TP_FIELD(							\
	__field(dev_t,		dev)				\
	__field(uint32_t,	freed)				\
)

#define TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE_TP_ASSIGN	\
do {								\
	__entry->dev		= sb->s_dev_id;			\
	__entry->freed		= freed;			\
} while (0)

#define TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE_PRINT		\
TP_PRINT("dev = (%d,%d), shrunk: freed %ld",			\
	show_dev(__entry->dev),					\
	__entry->freed)						\

#define TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE(group, name)		\
TRACE_EVENT(group, name,						\
	TP_PROTO(struct super_block *, sb, uint32_t, freed),		\
	TP_ARGS(sb, freed),						\
	TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE_TP_FIELD,		\
	TP_ASSIGN(TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE_TP_ASSIGN;),	\
	TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE_PRINT			\
)

#define TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE_TP_FIELD	\
TP_FIELD(							\
	__field(dev_t,		dev)				\
		__field(unsigned long,	index)			\
		__field(unsigned int,	node_cnt)		\
)

#define TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE_TP_ASSIGN	\
do {								\
	__entry->node_cnt	= node_cnt;			\
} while (0)

#define TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE_PRINT		\
TP_PRINT("dev = (%d,%d), ino = %lu, destroyed: node_cnt = %u",	\
	show_dev_index(__entry),				\
	__entry->node_cnt)					\

#define TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE(group, name)			\
TRACE_EVENT(group, name,							\
	TP_PROTO(vfs_node_t *, vn, unsigned int, node_cnt),			\
	TP_ARGS(vn, node_cnt),							\
	TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE_TP_FIELD,			\
	TP_ASSIGN(TP_ASSIGN_DEV_INDEX_TEMPLATE					\
		TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE_TP_ASSIGN;),		\
	TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE_PRINT				\
)

#define TRACE_EVENT_VNODE_EXIT_TEMPLATE_IP_FIELD	\
TP_FIELD(						\
	__field(dev_t,	dev)				\
	__field(ino_t,	index)				\
	__field(int,	res)				\
)

#define TRACE_EVENT_VNODE_EXIT_TEMPLATE_PRINT	\
TP_PRINT("dev = (%d,%d), ino = %lu, ret = %d",	\
	show_dev_index(__entry),		\
	__entry->res				\
)

#define TRACE_EVENT_VNODE_EXIT_TEMPLATE(group, name)		\
TRACE_EVENT(group, name,					\
	TP_PROTO(vfs_node_t *, vn, int, res), TP_ARGS(vn, res),	\
	TRACE_EVENT_VNODE_EXIT_TEMPLATE_IP_FIELD,		\
	TP_ASSIGN(						\
		TP_ASSIGN_DEV_INDEX_TEMPLATE			\
		__entry->res = res;				\
	),							\
	TRACE_EVENT_VNODE_EXIT_TEMPLATE_PRINT			\
)

#define TRACE_EVENT_LINK_TEMPLATE_TP_FIELD			\
TP_FIELD(							\
	__field(dev_t,			dev)			\
	__field(unsigned long,		index)			\
	__field(mode_t,			mode)			\
	__field(uint64_t, 		size)			\
	__field(unsigned long long, 	blocks)			\
	__string(name,			dentry_name(dentry))	\
)

#define TRACE_EVENT_LINK_TEMPLATE_TP_ASSIGN		\
do {							\
	__entry->mode = mode;				\
	__entry->size = vn->size;			\
	__entry->blocks = vn->blocks;			\
	__assign_str(name, dentry_name(dentry));	\
} while (0)

#define TRACE_EVENT_LINK_TEMPLATE_PRINT		\
TP_PRINT("dev = (%d,%d), dir ino = %lu, size = %lld,"	\
	" blocks = %llu, name = %s, mode = 0x%hx",	\
	show_dev_index(__entry),			\
	__entry->size,					\
	__entry->blocks,				\
	__get_str(name),				\
	__entry->mode)					\

#define TRACE_EVENT_LINK_TEMPLATE(group, name)					\
TRACE_EVENT(group, name,							\
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, mode_t, mode),	\
	TP_ARGS(vn, dentry, mode),						\
	TRACE_EVENT_LINK_TEMPLATE_TP_FIELD,					\
	TP_ASSIGN(TP_ASSIGN_DEV_INDEX_TEMPLATE					\
		TRACE_EVENT_LINK_TEMPLATE_TP_ASSIGN;),				\
	TRACE_EVENT_LINK_TEMPLATE_PRINT						\
)

#define TRACE_EVENT_VFS_NODE_T_TEMPLATE_TP_FIELD	\
TP_FIELD(						\
	__field(dev_t, dev)				\
	__field(unsigned long, ino)			\
	__field(uint64_t, size)				\
	__field(mode_t, mode)				\
)

#define TRACE_EVENT_VFS_NODE_T_TEMPLATE_TP_ASSIGN				\
do {										\
	if (vn != NULL) {							\
		__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;	\
		__entry->ino = vn->index;					\
		__entry->size = vn->size;					\
		__entry->mode = vn->mode;					\
	}									\
} while (0)

#define TRACE_EVENT_VFS_NODE_T_TEMPLATE_PRINT	\
TP_PRINT("dev = (%d,%d) ino = %lu,"		\
	" i_mode = 0x%hx, size = %llu",		\
	show_dev(__entry->dev),			\
	__entry->ino,				\
	__entry->mode, __entry->size)		\

#define TRACE_EVENT_VFS_NODE_T_TEMPLATE(group, name)		\
TRACE_EVENT(group, name,					\
	TP_PROTO(vfs_node_t *, vn),				\
	TP_ARGS(vn),						\
	TRACE_EVENT_VFS_NODE_T_TEMPLATE_TP_FIELD,		\
	TP_ASSIGN(						\
		TRACE_EVENT_VFS_NODE_T_TEMPLATE_TP_ASSIGN;	\
	),							\
	TRACE_EVENT_VFS_NODE_T_TEMPLATE_PRINT			\
)

#define TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE_FIELD	\
TP_FIELD(						\
	__field(dev_t, dev)				\
	__field(bool, is_dir)				\
	__field(long long, count)			\
)

#define TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE_ASSIGN	\
TP_ASSIGN(						\
	__entry->dev	= sb->s_dev_id;			\
	__entry->is_dir	= is_dir;			\
	__entry->count	= count;			\
)

#define TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE_PRINT	\
TP_PRINT("dev = (%d,%d), %s, dirty count = %lld",	\
	show_dev(__entry->dev),				\
	__entry->is_dir ? "DIR" : "FILE",		\
	__entry->count)

#define TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE(group, name)				\
TRACE_EVENT(group, name,							\
	TP_PROTO(struct super_block *, sb, bool, is_dir, long long, count),	\
	TP_ARGS(sb, is_dir, count),						\
	TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE_FIELD,				\
	TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE_ASSIGN,				\
	TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE_PRINT				\
)

#define TRACE_EVENT_VNODE_INEDX_TEMPLATE(group, name, print_prefix)	\
TRACE_EVENT(group, name,						\
	TP_PROTO(unsigned long, ino_num),				\
	TP_ARGS(ino_num),						\
	TP_FIELD(__field(unsigned long, ino_num)),			\
	TP_ASSIGN(__entry->ino_num = ino_num;),				\
	TP_PRINT(print_prefix ": ino %u", __entry->ino_num)		\
)

#define TRACE_EVENT_MAPPING_TEMPLATE_ASSIGN_ENTRY				\
do{										\
	ASIGN_BLOCK_TYPE(pgtype, type);						\
	ASIGN_FILE_MODE(dir, S_ISDIR(FSCACHE_NODE(mapping)->mode));		\
	__entry->dirty	= mapping->pages_dirty;					\
} while(0)

#define TRACE_EVENT_MAPPING_TEMPLATE_ASSIGN_PAGE				\
if (FSCACHE_NODE(mapping) != NULL) {						\
	__entry->dev	= FSCACHE_NODE(mapping)->i_sb != NULL ?			\
				FSCACHE_NODE(mapping)->i_sb->s_dev_id : 0;	\
	__entry->index	= FSCACHE_NODE(mapping)->index;				\
}

#define TRACE_EVENT_MAPPING_TEMPLATE_TP_FIELD			\
TP_FIELD(							\
	__field(dev_t,		dev)				\
	__field(ino_t,		index)				\
	__string(pgtype, 	MAX_BLOCK_TYPE_TEMPLATE)	\
	__string(dir, 		MAX_FILE_MODE_TEMPLATE)		\
	__field(unsigned int, 	dirty)				\
)

#define TRACE_EVENT_MAPPING_TEMPLATE_TP_PRINT				\
TP_PRINT("dev = (%d,%d), ino = %lu, %s, %s, dirty = %u",		\
	show_dev_index(__entry),					\
	__get_str(pgtype),						\
	__get_str(dir),							\
	__entry->dirty)

#define TRACE_EVENT_MAPPING_TEMPLATE(group, name)				\
TRACE_EVENT(group, name,						\
	TP_PROTO(const fscache_mapping_t *, mapping, int, type), TP_ARGS(mapping, type),	\
	TRACE_EVENT_MAPPING_TEMPLATE_TP_FIELD,				\
	TP_ASSIGN(							\
		TRACE_EVENT_MAPPING_TEMPLATE_ASSIGN_PAGE			\
		TRACE_EVENT_MAPPING_TEMPLATE_ASSIGN_ENTRY;			\
	),								\
	TRACE_EVENT_MAPPING_TEMPLATE_TP_PRINT				\
)

#define TRACE_EVENT_END_IO_TEMPLATE_TP_FIELD	\
TP_FIELD(					\
	__field(dev_t,		dev)		\
	__field(unsigned int,	fi_flags)	\
	__field(int,		fi_status)	\
)

#define TRACE_EVENT_END_IO_TEMPLATE_TP_ASSIGN		\
TP_ASSIGN(						\
	__entry->dev		= fsio->devcon->dev_id;	\
	__entry->fi_flags 	= fsio->fi_flags;	\
	__entry->fi_status 	= fsio->fi_status;			\
)

#define TRACE_EVENT_END_IO_TEMPLATE_TP_PRINT			\
TP_PRINT("dev = (%d,%d), fi_flags = %lu, fi_status = %d",	\
	show_dev(__entry->dev),					\
	__entry->fi_flags,					\
	__entry->fi_status					\
)

#define TRACE_EVENT_END_IO_TEMPLATE(group, name)	\
TRACE_EVENT(group, name,				\
	TP_PROTO(struct fs_io *, fsio),			\
	TP_ARGS(fsio),					\
	TRACE_EVENT_END_IO_TEMPLATE_TP_FIELD,		\
	TRACE_EVENT_END_IO_TEMPLATE_TP_ASSIGN,		\
	TRACE_EVENT_END_IO_TEMPLATE_TP_PRINT		\
)
