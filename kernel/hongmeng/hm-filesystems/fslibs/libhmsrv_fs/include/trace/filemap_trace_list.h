/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: filemap tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 7 15:30:54 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(filemap, mm_filemap_delete_from_page_cache,
	TP_PROTO(fscache_page_t *, pg),
	TP_ARGS(pg),
	TP_FIELD(
		__field(unsigned long, pfn)
		__field(unsigned long, i_ino)
		__field(unsigned long, index)
		__field(dev_t, s_dev)
		__field(void *, pg)
	),
	TP_ASSIGN(
		__entry->pg = (void *)pg;
		if (pg == NULL) {
			__entry->pfn = 0;
			__entry->i_ino = 0;
			__entry->s_dev = 0;
		} else {
			__entry->pfn = fscache_get_page_pfn(pg);
			__entry->index = PAGE_INDEX(pg);
			if (PAGE_MAPPING(pg) != NULL && PAGE_MAPPING(pg)->host != NULL) {
				__entry->i_ino = PAGE_MAPPING(pg)->host->index;
				__entry->s_dev = (PAGE_MAPPING(pg)->host->i_sb != NULL) ?
								PAGE_MAPPING(pg)->host->i_sb->s_dev_id :
								PAGE_MAPPING(pg)->host->dev_id;
			} else {
				__entry->i_ino = 0;
				__entry->s_dev = 0;
			}
		}
	),
	TP_PRINT("dev %d:%d ino %lx page=%p pfn=%lu ofs=%lu",
		((unsigned int)((__entry->s_dev) >> 20)), ((unsigned int)((__entry->s_dev) & ((1U << 20) - 1))),
		__entry->i_ino,
		__entry->pg,
		__entry->pfn,
		__entry->index << PAGE_SHIFT)
)

TRACE_EVENT(filemap, mm_filemap_add_to_page_cache,
	TP_PROTO(fscache_page_t *, pg),
	TP_ARGS(pg),
	TP_FIELD(
		__field(unsigned long, pfn)
		__field(unsigned long, i_ino)
		__field(unsigned long, index)
		__field(dev_t, s_dev)
		/* Currently, hm doesn't support pfn to fscache page address.
		 * So we add an entry member pg to store page address.
		 */
		__field(void *, pg)
	),
	TP_ASSIGN(
		__entry->pg = (void *)pg;
		if (pg == NULL) {
			__entry->pfn = 0;
			__entry->i_ino = 0;
			__entry->s_dev = 0;
		} else {
			__entry->pfn = fscache_get_page_pfn(pg);
			__entry->index = PAGE_INDEX(pg);
			if (PAGE_MAPPING(pg) != NULL && PAGE_MAPPING(pg)->host != NULL) {
				__entry->i_ino = PAGE_MAPPING(pg)->host->index;
				__entry->s_dev = (PAGE_MAPPING(pg)->host->i_sb != NULL) ?
								PAGE_MAPPING(pg)->host->i_sb->s_dev_id :
								PAGE_MAPPING(pg)->host->dev_id;
			} else {
				__entry->i_ino = 0;
				__entry->s_dev = 0;
			}
		}
	),
	TP_PRINT("dev %d:%d ino %lx page=%p pfn=%lu ofs=%lu",
		((unsigned int)((__entry->s_dev) >> 20)), ((unsigned int)((__entry->s_dev) & ((1U << 20) - 1))),
		__entry->i_ino,
		__entry->pg,
		__entry->pfn,
		__entry->index << PAGE_SHIFT)
)

TRACE_EVENT(filemap, filemap_set_wb_err,
		TP_PROTO(fscache_mapping_t *, mapping, int, err),
		TP_ARGS(mapping, err),
		TP_FIELD(
			__field(unsigned long, i_ino)
			__field(dev_t, s_dev)
			__field(uint32_t, errseq)
		),
		TP_ASSIGN(
			if (mapping == NULL) {
				__entry->i_ino = 0;
				__entry->s_dev = 0;
				__entry->errseq = 0;
			} else {
				__entry->errseq = err;
				if (mapping->host != NULL) {
					__entry->i_ino = mapping->host->index;
					__entry->s_dev = (mapping->host->i_sb != NULL) ?
									mapping->host->i_sb->s_dev_id :
									mapping->host->dev_id;
				} else {
					__entry->i_ino = 0;
					__entry->s_dev = 0;
				}
			}
		),
		TP_PRINT("dev=%d:%d ino=0x%lx errseq=0x%x",
			((unsigned int)((__entry->s_dev) >> 20)), ((unsigned int)((__entry->s_dev) & ((1U << 20) - 1))),
			__entry->i_ino, __entry->errseq)
)

TRACE_EVENT(filemap, file_check_and_advance_wb_err,
		TP_PROTO(struct file *, file, int, err),
		TP_ARGS(file, err),
		TP_FIELD(
			__field(struct file *, file)
			__field(unsigned long, i_ino)
			__field(dev_t, s_dev)
			__field(uint32_t, old)
			__field(uint32_t, new)
		),
		TP_ASSIGN(
			if (file == NULL) {
				__entry->file = NULL;
				__entry->i_ino = 0;
				__entry->s_dev = 0;
				__entry->old = 0;
				__entry->new = 0;
			} else {
				__entry->file = file;
				if (file->node != NULL) {
					__entry->i_ino = file->node->index;
					__entry->s_dev = (file->node->i_sb != NULL) ?
									file->node->i_sb->s_dev_id :
									file->node->dev_id;
				} else {
					__entry->i_ino = 0;
					__entry->s_dev = 0;
				}
				__entry->old = (uint32_t)err;
				__entry->new = (uint32_t)err;
			}
		),
		TP_PRINT("file=%p dev=%d:%d ino=0x%lx old=0x%x new=0x%x",
			__entry->file,
			((unsigned int)((__entry->s_dev) >> 20)), ((unsigned int)((__entry->s_dev) & ((1U << 20) - 1))),
			__entry->i_ino, __entry->old, __entry->new
		)
)
