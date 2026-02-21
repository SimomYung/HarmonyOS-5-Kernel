/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 20 14:41:30 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#define DEV_MAJOR(dev)	((unsigned int) ((dev) >> 20U))
#define DEV_MINOR(dev)	((unsigned int) ((dev) & ((1U << 20U) - 1U)))

#define RWBS_LEN 8
#define CMD_LEN 16

#ifdef CONFIG_MAS_BLK
#define DEFINE_BLOCK_RQ_EVENT(FUNC)							\
	TRACE_EVENT(block, FUNC,							\
		TP_PROTO(struct udk_request *, rq),					\
		TP_ARGS(rq),								\
		TP_FIELD(								\
			__field(uint32_t, dev)						\
			__field(uint64_t, sector)					\
			__field(uint32_t, nr_sector)					\
			__field(uint32_t, bytes)					\
			__array(char, rwbs, RWBS_LEN)					\
			__array(char, comm, HMCAP_THREAD_NAME_SIZE)			\
			__array(char, cmd, CMD_LEN)					\
		),									\
		TP_ASSIGN(								\
			__entry->dev = MKDEV(rq->disk->major, rq->disk->first_minor);	\
			__entry->sector    = rq->sector;				\
			__entry->nr_sector = rq->data_len >> 9;				\
			__entry->bytes = rq->data_len;					\
			udk_blk_fill_rwbs(__entry->rwbs, rq->cmd_flags, rq->mas_cmd_flags);	\
			hm_trace_tcb_name_of(__entry->comm);				\
			memcpy_s(__entry->cmd, CMD_LEN, "\0", 1);			\
		),									\
		TP_PRINT("%d,%d %s %u (%s) %llu + %u [%s]",				\
			 DEV_MAJOR(__entry->dev), DEV_MINOR(__entry->dev),		\
			 __entry->rwbs, __entry->bytes, __entry->cmd,			\
			 (unsigned long long)__entry->sector,				\
			 __entry->nr_sector, __entry->comm				\
		)									\
	)
#else
#define DEFINE_BLOCK_RQ_EVENT(FUNC)							\
	TRACE_EVENT(block, FUNC,							\
		TP_PROTO(struct udk_request *, rq),					\
		TP_ARGS(rq),								\
		TP_FIELD(								\
			__field(uint32_t, dev)						\
			__field(uint64_t, sector)					\
			__field(uint32_t, nr_sector)					\
			__field(uint32_t, bytes)					\
			__array(char, rwbs, RWBS_LEN)					\
			__array(char, comm, HMCAP_THREAD_NAME_SIZE)			\
			__array(char, cmd, CMD_LEN)					\
		),									\
		TP_ASSIGN(								\
			__entry->dev = MKDEV(rq->disk->major, rq->disk->first_minor);	\
			__entry->sector    = rq->sector;				\
			__entry->nr_sector = rq->data_len >> 9;				\
			__entry->bytes = rq->data_len;					\
			udk_blk_fill_rwbs(__entry->rwbs, rq->cmd_flags);		\
			hm_trace_tcb_name_of(__entry->comm);				\
			memcpy_s(__entry->cmd, CMD_LEN, "\0", 1);			\
		),									\
		TP_PRINT("%d,%d %s %u (%s) %llu + %u [%s]",				\
			 DEV_MAJOR(__entry->dev), DEV_MINOR(__entry->dev),		\
			 __entry->rwbs, __entry->bytes, __entry->cmd,			\
			 (unsigned long long)__entry->sector,				\
			 __entry->nr_sector, __entry->comm				\
		)									\
	)
#endif

DEFINE_BLOCK_RQ_EVENT(block_rq_issue)

TRACE_EVENT(block, block_rq_complete,
	TP_PROTO(struct udk_request *, rq, int, error, unsigned int, nr_bytes),
	TP_ARGS(rq, error, nr_bytes),
	TP_FIELD(
		__field(uint32_t, dev)
		__field(uint64_t, sector)
		__field(uint32_t, nr_sector)
		__field(int, error)
		__array(char, rwbs, RWBS_LEN)
		__array(char, cmd, CMD_LEN)
		),
	TP_ASSIGN(
		__entry->dev = MKDEV(rq->disk->major, rq->disk->first_minor);
		__entry->sector    = rq->sector;
		__entry->nr_sector = nr_bytes >> 9;
		__entry->error     = error;
#ifdef CONFIG_MAS_BLK
		udk_blk_fill_rwbs(__entry->rwbs, rq->cmd_flags, rq->mas_cmd_flags);
#else
		udk_blk_fill_rwbs(__entry->rwbs, rq->cmd_flags);
#endif
		memcpy_s(__entry->cmd, CMD_LEN, "\0", 1);
	),
	TP_PRINT("%d,%d %s (%s) %llu + %u [%d]",
		 DEV_MAJOR(__entry->dev), DEV_MINOR(__entry->dev),
		 __entry->rwbs, __entry->cmd,
		 (unsigned long long)__entry->sector,
		 __entry->nr_sector, __entry->error
	)
)

#define DEV_NAME_LEN 16
#define STR_LEN 16
TRACE_EVENT(ufs, ufshcd_command,
	TP_PROTO(const char *, dev_name, const char *, str, unsigned int, tag,
		 uint32_t, doorbell, int, transfer_len, uint32_t, intr,
		 uint64_t, lba, uint8_t, opcode),
	TP_ARGS(dev_name, str, tag, doorbell, transfer_len, intr, lba, opcode),
	TP_FIELD(
		__array(char, dev_name, DEV_NAME_LEN)
		__array(char, str, STR_LEN)
		__field(unsigned int, tag)
		__field(uint32_t, doorbell)
		__field(int, transfer_len)
		__field(uint32_t, intr)
		__field(uint64_t, lba)
		__field(uint8_t, opcode)
	),
	TP_ASSIGN(
		memcpy_s(__entry->dev_name, DEV_NAME_LEN, dev_name,
			 strlen(dev_name) + 1U);
		memcpy_s(__entry->str, STR_LEN, str, strlen(str) + 1U);
		__entry->tag = tag;
		__entry->doorbell = doorbell;
		__entry->transfer_len = transfer_len;
		__entry->intr = intr;
		__entry->lba = lba;
		__entry->opcode = opcode;
	),
	TP_PRINT("%s: %s: tag: %u, DB: 0x%x, size: %d, IS: %u, LBA: %llu, opcode: 0x%x",
		 __entry->str, __entry->dev_name, __entry->tag,
		 __entry->doorbell, __entry->transfer_len,
		 __entry->intr, __entry->lba, (uint32_t)__entry->opcode
	)
)

TRACE_EVENT(block, block_bio_remap,
	TP_PROTO(struct udk_bio *, bio, uint32_t, dev, uint64_t, from),
	TP_ARGS(bio, dev, from),
	TP_FIELD(
		__field(uint32_t, dev)
		__field(uint64_t, sector)
		__field(uint32_t, nr_sector)
		__field(uint32_t, old_dev)
		__field(uint64_t, old_sector)
		),
	TP_ASSIGN(
		__entry->dev = bio->bi_dev->devt;
		__entry->sector = bio->bi_sector;
		__entry->nr_sector = udk_bio_sectors(bio);
		__entry->old_dev = dev;
		__entry->old_sector = from;
	),
	TP_PRINT("%d,%d %llu + %u <- (%d,%d) %llu",
		 DEV_MAJOR(__entry->dev), DEV_MINOR(__entry->dev),
		 (unsigned long long)__entry->sector,
		 __entry->nr_sector,
		 DEV_MAJOR(__entry->old_dev), DEV_MINOR(__entry->old_dev),
		 (unsigned long long)__entry->old_sector)
)
