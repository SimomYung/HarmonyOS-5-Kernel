/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header file of io_uring
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 6 15:00:00 2023
 */
#ifndef IO_URING_H
#define IO_URING_H

struct io_uring_sqe {
	uint8_t opcode;
	uint8_t flags;
	uint16_t ioprio; /* not support */
	int fd;
	union {
		uint64_t off; /* offset into file */
		uint64_t addr2;
	};
	union {
		uint64_t addr; /* the pointer to buffer or iovecs */
		uint64_t splice_off_in;
	};
	uint32_t len; /* buffer size or number of iovecs */
	union {
		int rw_flags;
		uint32_t fsync_flags;
		uint16_t poll_events;
		uint32_t poll32_events; /* for compatible */
		uint32_t sync_range_flags;
		uint32_t msg_flags;
		uint32_t timeout_flags;
		uint32_t accept_flags;
		uint32_t cancel_flags;
		uint32_t open_flags;
		uint32_t statx_flags;
		uint32_t fadvise_advise;
		uint32_t splice_flags;
	};
	uint64_t user_data;
	union {
		struct {
			union {
				uint16_t buf_index;
				uint16_t buf_group;
			};
			uint16_t personality;
			int splice_fd_in;
		};
		uint64_t __pad2[3];
	};
};

enum {
	IOSQE_FIXED_FILE_BIT,
	IOSQE_IO_DRAIN_BIT,
	IOSQE_IO_LINK_BIT,
	IOSQE_IO_HARDLINK_BIT,
	IOSQE_ASYNC_BIT,
	IOSQE_BUFFER_SELECT_BIT,
};

/*
 * sqe->flags
 */
/* use fixed fileset */
#define IOSQE_FIXED_FILE	(1U)
/* issue after inflight IO */
#define IOSQE_IO_DRAIN		(1U << IOSQE_IO_DRAIN_BIT)
/* links next sqe */
#define IOSQE_IO_LINK		(1U << IOSQE_IO_LINK_BIT)
/* like LINK, but stronger */
#define IOSQE_IO_HARDLINK	(1U << IOSQE_IO_HARDLINK_BIT)
/* always go async */
#define IOSQE_ASYNC		(1U << IOSQE_ASYNC_BIT)
/* select buffer from sqe->buf_group */
#define IOSQE_BUFFER_SELECT	(1U << IOSQE_BUFFER_SELECT_BIT)

/*
 * io_uring_setup() flags
 */
#define IORING_SETUP_IOPOLL	(1U)
#define IORING_SETUP_SQPOLL	(1U << 1)	/* SQ poll thread */
#define IORING_SETUP_SQ_AFF	(1U << 2)	/* sq_thread_cpu is valid */
#define IORING_SETUP_CQSIZE	(1U << 3)	/* app defines CQ size */
#define IORING_SETUP_CLAMP	(1U << 4)	/* clamp SQ/CQ ring sizes */
#define IORING_SETUP_ATTACH_WQ	(1U << 5)	/* attach to existing wq */
#define IORING_SETUP_R_DISABLED	(1U << 6)	/* start with ring disabled */

enum {
	IORING_OP_NOP,
	IORING_OP_READV,
	IORING_OP_WRITEV,
	IORING_OP_FSYNC,
	IORING_OP_READ_FIXED,
	IORING_OP_WRITE_FIXED,
	IORING_OP_POLL_ADD,
	IORING_OP_POLL_REMOVE,
	IORING_OP_SYNC_FILE_RANGE,
	IORING_OP_SENDMSG,
	IORING_OP_RECVMSG,
	IORING_OP_TIMEOUT,
	IORING_OP_TIMEOUT_REMOVE,
	IORING_OP_ACCEPT,
	IORING_OP_ASYNC_CANCEL,
	IORING_OP_LINK_TIMEOUT,
	IORING_OP_CONNECT,
	IORING_OP_FALLOCATE,
	IORING_OP_OPENAT,
	IORING_OP_CLOSE,
	IORING_OP_FILES_UPDATE,
	IORING_OP_STATX,
	IORING_OP_READ,
	IORING_OP_WRITE,
	IORING_OP_FADVISE,
	IORING_OP_MADVISE,
	IORING_OP_SEND,
	IORING_OP_RECV,
	IORING_OP_OPENAT2,
	IORING_OP_EPOLL_CTL,
	IORING_OP_SPLICE,
	IORING_OP_PROVIDE_BUFFERS,
	IORING_OP_REMOVE_BUFFERS,
	IORING_OP_TEE,

	/*
	 * liburing applied for Framework needs to update
	 * with kernel for following opcodes.
	 */
	IORING_OP_HYPERFRAME_OPEN,
	IORING_OP_HYPERFRAME_WRITE,
	IORING_OP_HYPERFRAME_READ,
	IORING_OP_HYPERFRAME_CLOSE,

	/* this goes last, obviously */
	IORING_OP_LAST,
};

/*
 * sqe->fsync_flags
 */
#define IORING_FSYNC_DATASYNC	(1U)

/*
 * sqe->timeout_flags
 */
#define IORING_TIMEOUT_ABS	(1U)

/*
 * sqe->splice_flags
 * extends splice(2) flags
 */
#define SPLICE_F_FD_IN_FIXED	(1U << 31) /* the last bit of __u32 */

/*
 * IO completion data structure (Completion Queue Entry)
 */
struct io_uring_cqe {
	uint64_t user_data; /* sqe->data submission passed back */
	int res; /* result code for this event */
	int flags;
};

/*
 * cqe->flags
 *
 * IORING_CQE_F_BUFFER	If set, the upper 16 bits are the buffer ID
 */
#define IORING_CQE_F_BUFFER		(1U)

enum {
	IORING_CQE_BUFFER_SHIFT		= 16,
};

/*
 * Magic offsets for the application to mmap the data it needs
 */
#define IORING_OFF_SQ_RING		0ULL
#define IORING_OFF_CQ_RING		0x8000000ULL
#define IORING_OFF_SQES			0x10000000ULL

/*
 * Filled with the offset for mmap(2)
 */
struct io_sqring_offsets {
	uint32_t head;
	uint32_t tail;
	uint32_t ring_mask;
	uint32_t ring_entries;
	uint32_t flags;
	uint32_t dropped;
	uint32_t array;
	uint32_t resv1;
	uint64_t resv2;
};

/*
 * sq_ring->flags
 */
#define IORING_SQ_NEED_WAKEUP	(1U) /* needs io_uring_enter wakeup */
#define IORING_SQ_CQ_OVERFLOW	(1U << 1) /* CQ ring is overflown */

struct io_cqring_offsets {
	uint32_t head;
	uint32_t tail;
	uint32_t ring_mask;
	uint32_t ring_entries;
	uint32_t overflow;
	uint32_t cqes;
	uint32_t flags;
	uint32_t resv1;
	uint64_t resv2;
};

/*
 * cq_ring->flags
 */

/* disable eventfd notifications */
#define IORING_CQ_EVENTFD_DISABLED	(1U)

/*
 * io_uring_enter(2) flags
 */
#define IORING_ENTER_GETEVENTS	(1U)
#define IORING_ENTER_SQ_WAKEUP	(1U << 1)
#define IORING_ENTER_SQ_WAIT	(1U << 2)

/*
 * Passed in for io_uring_setup(2). Copied back with updated info on success
 */
struct io_uring_params {
	uint32_t sq_entries;
	uint32_t cq_entries;
	uint32_t flags;
	uint32_t sq_thread_cpu;
	uint32_t sq_thread_idle;
	uint32_t features;
	uint32_t wq_fd;
	uint32_t resv[3];
	struct io_sqring_offsets sq_off;
	struct io_cqring_offsets cq_off;
};

/*
 * io_uring_params->features flags
 */
#define IORING_FEAT_SINGLE_MMAP		(1U)
#define IORING_FEAT_NODROP		(1U << 1)
#define IORING_FEAT_SUBMIT_STABLE	(1U << 2)
#define IORING_FEAT_RW_CUR_POS		(1U << 3)
#define IORING_FEAT_CUR_PERSONALITY	(1U << 4)
#define IORING_FEAT_FAST_POLL		(1U << 5)
#define IORING_FEAT_POLL_32BITS 	(1U << 6)

/*
 * io_uring_register(2) opcodes and arguments
 */
enum {
	IORING_REGISTER_BUFFERS			= 0,
	IORING_UNREGISTER_BUFFERS		= 1,
	IORING_REGISTER_FILES			= 2,
	IORING_UNREGISTER_FILES			= 3,
	IORING_REGISTER_EVENTFD			= 4,
	IORING_UNREGISTER_EVENTFD		= 5,
	IORING_REGISTER_FILES_UPDATE		= 6,
	IORING_REGISTER_EVENTFD_ASYNC		= 7,
	IORING_REGISTER_PROBE			= 8,
	IORING_REGISTER_PERSONALITY		= 9,
	IORING_UNREGISTER_PERSONALITY		= 10,
	IORING_REGISTER_RESTRICTIONS		= 11,
	IORING_REGISTER_ENABLE_RINGS		= 12,

	/* this goes last */
	IORING_REGISTER_LAST
};

struct io_uring_files_update {
	uint32_t offset;
	uint32_t resv;
	uint64_t fds;
};

#define IO_URING_OP_SUPPORTED	(1U << 0)

struct io_uring_probe_op {
	uint8_t op;
	uint8_t resv;
	uint16_t flags; /* IO_URING_OP_* flags */
	uint32_t resv2;
};

struct io_uring_probe {
	uint8_t last_op; /* last opcode supported */
	uint8_t ops_len; /* length of ops[] array below */
	uint16_t resv;
	uint32_t resv2[3];
	struct io_uring_probe_op ops[];
};

struct io_uring_restriction {
	uint16_t opcode;
	union {
		uint8_t register_op; /* IORING_RESTRICTION_REGISTER_OP */
		uint8_t sqe_op;      /* IORING_RESTRICTION_SQE_OP */
		uint8_t sqe_flags;   /* IORING_RESTRICTION_SQE_FLAGS_* */
	};
	uint8_t resv;
	uint32_t resv2[3];
};

/*
 * io_uring_restriction->opcode values
 */
enum {
	/* Allow an io_uring_register(2) opcode */
	IORING_RESTRICTION_REGISTER_OP		= 0,

	/* Allow an sqe opcode */
	IORING_RESTRICTION_SQE_OP		= 1,

	/* Allow sqe flags */
	IORING_RESTRICTION_SQE_FLAGS_ALLOWED	= 2,

	/* Require sqe flags (these flags must be set on each submission) */
	IORING_RESTRICTION_SQE_FLAGS_REQUIRED	= 3,

	IORING_RESTRICTION_LAST
};

#endif /* IO_URING_H */
