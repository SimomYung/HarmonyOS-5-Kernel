/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Requester frame actions head file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 30 18:12:10 2019
 */
#ifndef ULIBS_LIBREQUESTER_REQUESTER_H
#define ULIBS_LIBREQUESTER_REQUESTER_H
// libc
#include <stdbool.h>
// hongmeng
#include <libsysif/utils.h>
#include <libhmucap/ucap.h>
#include <libsec/cred_common.h>

/*
 * We are consistent with activation transaction of the kernel.
 * src: the direct senders transfering the messages
 * fwd: the original senders of forward messages
 */
struct requester {
	uint32_t src_cnode_idx;
	union {
		/* cnode_idx is an alias of fwd_cnode_idx */
		uint32_t fwd_cnode_idx;
		uint32_t cnode_idx;
	};
	/* cred (base and shared cred) of actv call chain,
	 * we directly use the ptr which points to cocred object,
	 * so the user should guarantee the life cycle of
	 * cred object is not ended */
	__sec_data(libsec, ptr) void *fwd_base_cred;
	__sec_data(libsec, ptr) void *fwd_sh_cred;
	__sec_data(libsec, ptr) void *src_sh_cred;
	__sec_data(libsec, ptr) void *src_base_cred;
	cred_idx_t src_cred_idx;
	cred_idx_t fwd_cred_idx;
	uint32_t local_mem_flag;
};

enum cred_source_type  {
	CRED_SOURCE_POP_SRC,
	CRED_SOURCE_POP_FWD,
	CRED_SOURCE_SPEC_CRED,
	CRED_SOURCE_FROM_CALLER,
	CRED_SOURCE_FROM_SELF
};

struct sec_local_cred;
int requester_init(struct requester *req);
int requester_init_simple(struct requester *req);
int requester_init_simple_src(struct requester *req);
int requester_assign_cred(struct requester *req, enum cred_source_type cred_s);
int requester_assign_cred_with_spec(struct requester *req, enum cred_source_type cred_s, struct sec_local_cred *cred);

static inline struct requester requester_ctor(uint32_t fwd, uint32_t src)
{
	return (struct requester){
		.fwd_cnode_idx = fwd,
		.src_cnode_idx = src,
	};
}

static inline struct requester requester_ctor_simple(uint32_t cidx)
{
	return requester_ctor(cidx, cidx);
}

static inline struct requester requester_dummy(void)
{
	return requester_ctor_simple(0);
}

enum requester_state_type {
	REQUESTER_DEFAULT,
	REQUESTER_INIT,
	REQUESTER_OVERRIDE,
	REQUESTER_FROMSELF,
	REQUESTER_FROMSELF_INVALID
};

struct requester_frame {
	struct requester requester;
	enum requester_state_type requester_state;
};

extern __thread struct requester_frame *t_requester_current_frame;

static inline struct requester_frame *requester_current_frame(void)
{
	return t_requester_current_frame;
}

static inline bool requester_frame_is_avail(const struct requester_frame *req)
{
	return (req != NULL &&
		req->requester_state != REQUESTER_DEFAULT &&
		req->requester_state != REQUESTER_FROMSELF_INVALID);
}

static inline struct requester_frame *requester_frame_replace(
	struct requester_frame *new_frame)
{
	struct requester_frame *old_frame = t_requester_current_frame;
	t_requester_current_frame = new_frame;
	return old_frame;
}

static inline void requester_frame_restore(struct requester_frame *frame)
{
	t_requester_current_frame = frame;
}

static inline void requester_frame_cleanup(struct requester_frame **frame)
{
	requester_frame_restore(*frame);
}

static inline void requester_frame_assign_cred_from_req(struct requester_frame *frame, struct requester *req)
{
	frame->requester.src_sh_cred = req->src_sh_cred;
	frame->requester.src_base_cred = req->src_base_cred;
	frame->requester.fwd_sh_cred = req->fwd_sh_cred;
	frame->requester.fwd_base_cred = req->fwd_base_cred;
}

static inline bool requester_frame_is_fromself(void)
{
	return ((t_requester_current_frame != NULL) &&
		(t_requester_current_frame->requester_state == REQUESTER_FROMSELF ||
		 t_requester_current_frame->requester_state == REQUESTER_FROMSELF_INVALID));
}

static inline bool requester_frame_is_default(void)
{
	return ((t_requester_current_frame != NULL) &&
		t_requester_current_frame->requester_state == REQUESTER_DEFAULT);
}

/* caller should guarantee the cred's lifetime is not ended before assign */
static inline void requester_frame_assign_src_cred(struct requester_frame *frame, void *sh_cred,
						   void *b_cred)
{
	frame->requester.src_sh_cred = sh_cred;
	frame->requester.src_base_cred = b_cred;
}

static inline void requester_frame_assign_fwd_cred(struct requester_frame *frame, void *sh_cred,
						   void *b_cred)
{
	frame->requester.fwd_sh_cred = sh_cred;
	frame->requester.fwd_base_cred = b_cred;
}

/*
 * We can't add do-while wrapper for this macro because of the cleanup attribute
 * depending on the scope.
 */
#define requester_frame_replace_temporarily(REQUESTER, STATE)			\
	struct requester_frame __requester_frame = {				\
		.requester = (REQUESTER),					\
		.requester_state = (STATE)					\
	};									\
	struct requester_frame *__requester_frame_old				\
		__attribute__ ((unused))					\
		__attribute__ ((cleanup(requester_frame_cleanup))) = 		\
			requester_frame_replace(&__requester_frame)

#define requester_frame_replace_begin(REQUESTER, STATE) { \
	requester_frame_replace_temporarily(REQUESTER, STATE)
#define requester_frame_repalce_end() }

#define requester_frame_from_self_temp() \
	requester_frame_replace_temporarily(requester_dummy(), REQUESTER_FROMSELF_INVALID)

int requester_frame_set_init_frame(void);
void requester_frame_rst_init_frame(void);

#endif /* ifndef ULIBS_LIBREQUESTER_REQUESTER_H */
