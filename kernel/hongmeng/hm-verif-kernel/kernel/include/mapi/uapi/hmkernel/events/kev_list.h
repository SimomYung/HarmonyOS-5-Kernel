/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: KEV definition template
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 26 14:15:14 2020
 */

#include <hmkernel/types.h>

__DEFINE_KEV(SCHED_SWITCH, sched_switch,
	     NEED_TIMESTAMP,
	     KEV_ARG(2,
		     struct kev_tcb_state_s *, prev_state,
		     struct tcb_s *, next_tcb),
	     KEV_STRUCT(12,
			__u32, sequence,
			char, __KEV_STRING(pname, 16),
			char, __KEV_STRING(nname, 16),
			char, __KEV_STRING(ninfo, 48),
			__s32, pprio,
			__s32, nprio,
			__u64, pcref,
			__u64, ncref,
			__s32, prev_pid,
			__s32, prev_tid,
			__s32, next_tid,
			__u32, pstate),
	     KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
			if (prev_state->state == TCB_STATE_INVALID) {
				str_cpy_p_to_a(evt->pname, "NULL");
				evt->pprio = 0;
				evt->pcref = 0;
				evt->prev_pid = 0;
				evt->prev_tid = 0;
			} else {
				str_cpy_p_to_a(evt->pname, prev_state->name);
				evt->pprio = prev_state->prio;
				evt->pcref = prev_state->cref;
				evt->prev_pid = prev_state->pid;
				evt->prev_tid = prev_state->tid;
			}
			evt->pstate = prev_state->state;
			str_cpy_p_to_a(evt->nname, tcb_name_of(next_tcb));
			evt->nprio = sched_tcb_get_prio(next_tcb);
			mem_zero_s(evt->ninfo);
			sched_tcb_get_info(next_tcb, evt->ninfo, sizeof(evt->ninfo));
			evt->ncref = tcb_cref_of(next_tcb);
			evt->next_tid = *(ptr_cast(tcb_miscdata_of(next_tcb, MISCDATA_TCB_VTID), __s32))),
	     KEV_PRINT("prev_comm=%s prev_pid=%d prev_prio=%d prev_state=%u"
		       " ==> next_comm=%s next_pid=%d next_prio=%d\n",
		       evt->pname, evt->prev_tid, evt->pprio, evt->pstate, evt->nname,
			   evt->next_tid, evt->nprio))

DEFINE_KEV(BLOCK, block,
	   KEV_ARG(2, const struct tcb_s *, tcb, enum tcb_block_reason, reason),
	   KEV_STRUCT(3,
		   __u32, sequence,
		   char, __KEV_STRING(tname, 16),
		   __u32, reason),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      str_cpy_p_to_a(evt->tname, tcb_name_of(tcb));
		      evt->reason = reason),
	   KEV_PRINT("[seq=%u] tcb='%s', reason=%u\n", evt->sequence, evt->tname, evt->reason))

__DEFINE_KEV(UNBLOCK, unblock,
	   NEED_TIMESTAMP,
	   KEV_ARG(3, struct tcb_s *, tcb, enum tcb_block_reason, reason, __u64, delta),
	   KEV_STRUCT(12,
		      __u32, sequence,
		      char, __KEV_STRING(tgt_name, 16),
		      char, __KEV_STRING(waker_name, 16),
		      __s32, tgt_prio,
		      __s32, tgt_tid,
		      __u32, tgt_cpu,
		      __u32, iowait,
		      char, __KEV_STRING(escape, 192),
		      __u64, delay,
		      __s32, waker_pid,
		      __s32, waker_tid,
		      __u32, reason),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      const char *waker = kev_unblock_curr_waker();
		      /*
		       * if waker is NULL, waker name will be set to tcb_current later
		       * in function prepare_event
		       */
		      if (waker != NULL) {
			      str_cpy_p_to_a(evt->waker_name, waker);
			      evt->waker_pid = -1;
			      evt->waker_tid = -1;
		      } else {
			      struct tcb_s *waker_tcb = tcb_current();
			      if (waker_tcb != NULL) {
				      evt->waker_pid = *(ptr_cast(tcb_miscdata_of(waker_tcb, MISCDATA_TCB_VPID), __s32));
				      evt->waker_tid = *(ptr_cast(tcb_miscdata_of(waker_tcb, MISCDATA_TCB_VTID), __s32));
				      str_cpy_p_to_a(evt->waker_name, tcb_name_of(waker_tcb));
			      } else {
				      evt->waker_pid = 0;
				      evt->waker_tid = 0;
				      str_cpy_p_to_a(evt->waker_name, "NULL");
			      }
		      }
		      str_cpy_p_to_a(evt->tgt_name, tcb_name_of(tcb));
		      evt->tgt_prio = sched_tcb_get_prio(tcb);
		      evt->tgt_cpu = tcb_cpu_of(tcb);
		      evt->iowait = 0;
		      snprintf(evt->escape, sizeof(evt->escape),
			       "[Blocked in %s]",
			       actv_name_of(tcb_curr_actv_of_nolock(tcb)));
		      if (reason == TCB_BLOCK_FUTEX) {
				struct futex_block_at_s *block_at = &tcb->block_at.futex;
				int index = snprintf(evt->escape, sizeof(evt->escape),
						     "current_cnode_idx=[%x], lock=[%p], pc=[%p]<",
						     0, NULL, NULL);
				raw_setmem(&evt->escape[index], '>', 191 - index);
				evt->escape[191] = '\0';
				evt->iowait = (__u32)__IS_FUTEX_FLAG_IOWAIT(block_at->futex.flags);
		      }
		      u64 freq = (u64)vtimekeep_freq();
		      evt->delay = ((delta / freq) * __NSEC_PER_SEC) + ((delta % freq) * __NSEC_PER_SEC / freq);
		      evt->tgt_tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_VTID), __s32));
		      evt->reason = reason),
	   KEV_PRINT("[seq=%u] tcb=%s tid=%d prio=%d target_cpu=%03u block_reason=%u "
		     "waker=%s waker_pid=%d waker_tid=%d iowait=%u caller=%s delay=%llu\n",
			 evt->sequence, evt->tgt_name, evt->tgt_tid, evt->tgt_prio, evt->tgt_cpu, evt->reason,
		     evt->waker_name, evt->waker_pid, evt->waker_tid, evt->iowait, evt->escape, evt->delay))

DEFINE_KEV(SYSCALL, syscall,
	   KEV_ARG(2, unsigned int, scno, unsigned int, is_fastpath),
	   KEV_STRUCT(3,
		   __u32, sequence,
		   __u32, scno,
		   __u32, is_fastpath),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      evt->scno = scno; evt->is_fastpath = is_fastpath),
	   KEV_PRINT("[seq=%u] %u %s\n", evt->sequence, evt->scno,
		     (evt->is_fastpath != 0 ? "fastpath" : "slowpath")))

DEFINE_KEV(PAGEFAULT, pagefault,
	   KEV_ARG(2, unsigned long, faultpc, unsigned long, faultaddr),
	   KEV_STRUCT(3,
		   __u32, sequence,
		   __uptr_t, faultpc,
		   __uptr_t, faultaddr),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      evt->faultpc = faultpc;
		      evt->faultaddr = faultaddr),
	   KEV_PRINT("[seq=%u] faultpc=0x%lx, faultaddr=%lx\n",
		     (evt->sequence),
		     (evt->faultpc),
		     (evt->faultaddr)))

__DEFINE_KEV(IRQ, irq,
	     NEED_TIMESTAMP,
	     KEV_ARG(2,
		     unsigned int, irqno,
		     unsigned int, end),
	     KEV_STRUCT(7,
			/* make sure irqno is the first field for filter */
			__u32, irqno,
			__u32, sequence,
			__u32, end,
			__s32, prio,
			__u64, cref,
			char, __KEV_STRING(tname, 16),
			char, __KEV_STRING(actvname, 16)),
	     KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
			evt->irqno = irqno;
			evt->end = end;
			struct tcb_s *tcb = tcb_current();
			struct actv_s *actv = tcb_curr_actv_of_nolock(tcb);
			evt->prio = sched_tcb_get_prio(tcb);
			evt->cref = tcb_cref_of(tcb);
			str_cpy_p_to_a(evt->tname, tcb_name_of(tcb));
			str_cpy_p_to_a(evt->actvname, actv_name_of(actv))),
	     KEV_PRINT("[seq=%u] irqno=%u\t tcb='%16s\t' cref=0x%12"__PRIx64"\t prio=%3d\t actv='%16s\t' idx %2u\n",
		       evt->sequence, (evt->irqno), evt->tname, evt->cref, evt->prio, evt->actvname,
		       evt->end))

DEFINE_KEV(SAMPLE, sample,
	   KEV_ARG(2, const unsigned char *, s, size_t, len),
	   KEV_STRUCT(1,
		   unsigned char, __KEV_STRING(sample_data, PMU_SAMPLE_DATA_LEN)),
	   KEV_ASSIGN(mem_cpy_p_to_p(evt->sample_data, PMU_SAMPLE_DATA_LEN, s, len)),
	   KEV_PRINT("sample=0x%llx\n", ((unsigned long long)(__uptr_t)(void *)evt->sample_data)))

DEFINE_KEV(KLOG, klog,
	   KEV_ARG(4,
		   const char *, content_buffer,
		   size_t, content_size,
		   const char *, prefix_buffer,
		   size_t, prefix_size),
	   KEV_STRUCT(2,
		      __u32, sequence,
		      __u32, next_offset),
	   KEV_ASSIGN(KEV_KLOG_READ_SEQ(evt->sequence);
		      evt->next_offset = -1U;
		      evt_arg.prefix_size = prefix_size;
		      evt_arg.content_size = content_size;
		      evt_arg.content_buffer = content_buffer;
		      evt_arg.prefix_buffer = prefix_buffer),
#ifdef CONFIG_KEV_KLOG_TCB_INFO
	   KEV_PRINT("[seq=%u] pid=%-4d tid=%-4d comm=%-16s %s", evt->sequence,
		     ((struct kev_tcb_info_s *)(evt->data))->pid,
		     ((struct kev_tcb_info_s *)(evt->data))->tid,
		     ((struct kev_tcb_info_s *)(evt->data))->tcb_name,
		     KEV_KLOG_TEXT(evt)))
#else
	   KEV_PRINT("[seq=%u] %s", evt->sequence, evt->data))
#endif

__DEFINE_KEV(KPREEMPT, kpreempt,
	     NEED_TIMESTAMP,
	     KEV_ARG(1, unsigned int, tcno),
	     KEV_STRUCT(6,
			__u32, sequence,
			__u32, tcno,
			__s32, prio,
			__u64, cref,
			char, __KEV_STRING(tname, 16),
			char, __KEV_STRING(actvname, 16)),
	     KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
			evt->tcno = tcno;
			struct tcb_s *tcb = tcb_current();
			struct actv_s *actv = tcb_curr_actv_of_nolock(tcb);
			evt->prio = sched_tcb_get_prio(tcb);
			evt->cref = tcb_cref_of(tcb);
			str_cpy_p_to_a(evt->actvname, actv_name_of(actv));
			str_cpy_p_to_a(evt->tname, tcb_name_of(tcb))),
	     KEV_PRINT("[seq=%u] tcno=%u tcb='%s' cref=0x%"__PRIx64" prio=%d actv='%s'\n",
		       evt->sequence, evt->tcno, evt->tname, evt->cref, evt->prio, evt->actvname))

__DEFINE_KEV(FIQ, fiq,
	     NEED_TIMESTAMP,
	     KEV_ARG(1, enum kev_fiq_text_id, text_id),
	     KEV_STRUCT(6,
			unsigned int, sequence,
			enum kev_fiq_text_id, text_id,
			__u64, cref,
			__s32, prio,
			char, __KEV_STRING(tname, 16),
			char, __KEV_STRING(actvname, 16)),
	     KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
			evt->text_id = text_id;
			struct tcb_s *tcb = tcb_current();
			struct actv_s *actv = tcb_curr_actv_of_nolock(tcb);
			actv = tcb_curr_actv_of_nolock(tcb);
			evt->prio = sched_tcb_get_prio(tcb);
			evt->cref = tcb_cref_of(tcb);
			str_cpy_p_to_a(evt->tname, tcb_name_of(tcb));
			str_cpy_p_to_a(evt->actvname, actv_name_of(actv))),
	     KEV_PRINT("[seq=%u] %s tcb='%s' cref=0x%"__PRIx64" prio=%d actv='%s'\n",
		       evt->sequence, kev_fiq_text_of_id(evt->text_id),
		       evt->tname, evt->cref, evt->prio, evt->actvname))

DEFINE_KEV(SCHED_MIGRATE_TASK, sched_migrate_task,
	   KEV_ARG(2, struct tcb_s *, tcb, unsigned int, dest_cpu),
	   KEV_STRUCT(6,
		      unsigned int, sequence,
		      char, __KEV_STRING(name, 16),
		      __u64, cref,
		      __u32, prio,
		      __u32, orig_cpu,
		      __u32, dest_cpu),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      str_cpy_p_to_a(evt->name, tcb_name_of(tcb));
		      evt->cref = tcb_cref_of(tcb);
		      evt->prio = tcb_rq_of(tcb);
		      evt->orig_cpu = tcb_cpu_of(tcb);
		      evt->dest_cpu = dest_cpu),
	   KEV_PRINT("[seq=%u] comm=%s cref=0x%"__PRIx64" prio=%u orig_cpu=%u dest_cpu=%u\n",
		     evt->sequence, evt->name, evt->cref, evt->prio, evt->orig_cpu, evt->dest_cpu))

DEFINE_KEV(AUDIT, audit,
	   KEV_ARG(2, void *, audit_data, size_t, len),
	   KEV_STRUCT(1,
		   unsigned char , __KEV_STRING(audit_data, __AUDIT_DATA_LEN)),
	   KEV_ASSIGN(
		   mem_cpy_p_to_p(evt->audit_data, __AUDIT_DATA_LEN, audit_data, len)),
	   KEV_PRINT("audit type=0x%x\n", (*(unsigned int *)(void *)(&(evt->audit_data[0])))))

/* the definition of kev arg and kev struct must be the same as that in el2 */
__DEFINE_KEV(TRAP_EL2, trap_el2,
	     NEED_TIMESTAMP,
	     KEV_ARG(0), /* set empty here, see kev arg in el2 */
	     KEV_STRUCT(9,
			enum kev_trap_el2_type, type, /* trap in or out */
			__u64, call_id,
			__u64, x1,
			__u64, x2,
			__u64, x3,
			__u64, x4,
			__u64, x5,
			__u64, x6,
			__u64, x7),
	     KEV_ASSIGN(), /* set empty is ok, kev assign in el2 */
	     KEV_PRINT("%s: call_id=%"__PRIu64", x1=0x%"__PRIx64", x2=0x%"__PRIx64
		       ", x3=0x%"__PRIx64", x4=0x%"__PRIx64", x5=0x%"__PRIx64", x6=0x%"__PRIx64", x7=0x%"__PRIx64"\n",
		       (evt->type == KEV_EL2_ENTER ? "enter" : "exit "),
		       evt->call_id, evt->x1, evt->x2,
		       evt->x3, evt->x4, evt->x5, evt->x6, evt->x7)) /* kev print in el1 */

DEFINE_KEV(EL2_LOG, el2_log,
	   KEV_ARG(0), /* set empty here, see kev arg in el2 */
	   KEV_STRUCT(0),
	   KEV_ASSIGN(), /* set empty is ok, kev assign in el2 */
	   KEV_PRINT("%s", evt->data)) /* kev print in el1 */

__DEFINE_KEV(VCPU_EXIT, vcpu_exit,
	     NEED_TIMESTAMP,
	     KEV_ARG(0), /* set empty here, see kev arg in el2 */
	     KEV_STRUCT(7,
			__u32, sequence,
			char, __KEV_STRING(reason, 8),
			char, __KEV_STRING(opaque, 32),
			__u32, esr_el2,
			__u64, elr_el2,
			__u16, vmid,
			__u8, vcpu_id),
	     KEV_ASSIGN(), /* set empty is ok, kev assign in el2 */
	     KEV_PRINT("[seq=%u]: vmid=%u vcpu_id=%u exit_reason=%s "
		       "esr=0x%08x elr=0x%016"__PRIx64" %s\n",
		       evt->sequence, (unsigned int)evt->vmid, (unsigned int)evt->vcpu_id, evt->reason,
		       evt->esr_el2, evt->elr_el2, evt->opaque)) /* kev print in el1 */

__DEFINE_KEV(VCPU_ENTER, vcpu_enter,
	     NEED_TIMESTAMP,
	     KEV_ARG(0), /* set empty here, see kev arg in el2 */
	     KEV_STRUCT(7,
			__u32, sequence,
			__u64, duration,
			__u64, info_data,
			char, __KEV_STRING(reason, 8),
			__u32, esr_el2,
			__u16, vmid,
			__u8, vcpu_id),
	     KEV_ASSIGN(), /* set empty is ok, kev assign in el2 */
	     KEV_PRINT("[seq=%u]: vmid=%u vcpu_id=%u exit_reason=%s "
		       "esr=0x%08x data=0x%016"__PRIx64" duration=%"__PRIu64"\n",
		       evt->sequence, (unsigned int)evt->vmid, (unsigned int)evt->vcpu_id, evt->reason,
		       evt->esr_el2, evt->info_data, evt->duration)) /* kev print in el1 */

__DEFINE_KEV(EL2_IRQ, el2_irq,
	     NEED_TIMESTAMP,
	     KEV_ARG(0), /* set empty here, see kev arg in el2 */
	     KEV_STRUCT(5,
			enum kev_el2_irq_type, type,
			__u16, vmid,
			__u8, vcpu_id,
			__u8, prio,
			__u64, irq),
	     KEV_ASSIGN(), /* set empty is ok, kev assign in el2 */
	     KEV_PRINT("%-13s: vmid=%u, vcpu_id=%u irq=%03"__PRIu64" prio=0x%x\n",
		       el2_irq_type_str(evt->type),
		       (unsigned int)evt->vmid, (unsigned int)evt->vcpu_id,
		       evt->irq, evt->prio)) /* kev print in el1 */

__DEFINE_KEV(VNOTIFY, vnotify,
	   NEED_TIMESTAMP,
	   KEV_ARG(5,
		   enum __kev_vnotify_type, type,
		   __u32, chn_id,
		   __u32, status,
		   __u32, flag,
		   const void *, futex),
	   KEV_STRUCT(6,
		   enum __kev_vnotify_type, type,
		   __u32, sequence,
		   __u32, chn_id,
		   __u32, status,
		   __u32, flag,
		   const void *, futex),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		   evt->type = type;
		   evt->chn_id = chn_id;
		   evt->status = status;
		   evt->flag = flag,
		   evt->futex = futex),
	   KEV_PRINT("[seq=%u]%-12s: chn_id=%u, status=%u, flag=%u\n",
		     evt->sequence, vnotify_type_str(evt->type), evt->chn_id, evt->status,
		     evt->flag)) /* kev print in el1 */

__DEFINE_KEV(SUSPEND, suspend,
	   NEED_TIMESTAMP,
	   KEV_ARG(3,
		   const struct tcb_s *, tcb,
		   u32, old_rq,
		   s32, retval),
	   KEV_STRUCT(5,
		      __u32, sequence,
		      char, __KEV_STRING(tname, 16),
		      __u64, cref,
		      __u32, prev_rq,
		      char, __KEV_STRING(result, 12)),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      str_cpy_p_to_a(evt->tname, tcb_name_of(tcb));
		      evt->cref = tcb_cref_of(tcb);
		      evt->prev_rq = old_rq;
		      str_cpy_p_to_a(evt->result, hmstrerror(retval))),
	   KEV_PRINT("tcb=%s, cref=%llx, old_rq=%u suspend:%s\n",
		     evt->tname, evt->cref, evt->prev_rq, evt->result))

__DEFINE_KEV(SUSPEND_RESUME, suspend_resume,
	     NEED_TIMESTAMP,
	     KEV_ARG(3,
		     const char *, action,
		     unsigned long long, state,
		     bool, start),
	     KEV_STRUCT(6,
			__s32, tid,
			__s32, pid,
			char, __KEV_STRING(name, 32),
			char, __KEV_STRING(action, 32),
			__u64, state,
			__u32, start),
	     KEV_ASSIGN(struct tcb_s *tcb = tcb_current();
			evt->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_VTID), __s32));
			evt->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_VPID), __s32));
			str_cpy_p_to_a(evt->name, tcb_name_of(tcb));
			str_cpy_p_to_a(evt->action, action);
			evt->state = state;
			evt->start = (u32)start),
	     KEV_PRINT("comm=%s tid=%d pid=%d action=%s state=%llu start=%u\n",
		       evt->name, evt->tid, evt->pid, evt->action, evt->state, evt->start))

__DEFINE_KEV(VSMMU, vsmmu,
		NEED_TIMESTAMP,
		KEV_ARG(6,
			__u32, vm_id,
			__u32, phy_smmu_id,
			__u32, stream_id,
			enum kev_vsmmu_trace_type, type,
			__u32, cmd_evt_type,
			int, ret),
		KEV_STRUCT(7,
			__u32, sequence,
			__u32, vm_id,
			__u32, stream_id,
			__u32, phy_smmu_id,
			char, __KEV_STRING(info, 32),
			__u32, cmd_evt_type,
			int , ret),
		KEV_ASSIGN(
			KEV_READ_SEQ(evt->sequence);
			evt->vm_id = vm_id;
			evt->phy_smmu_id = phy_smmu_id;
			evt->stream_id = stream_id;
			if (type == KEV_VSMMU_CMD) {
				str_cpy_p_to_a(evt->info, "send cmd to phy-smmu");
			} else if (type == KEV_VSMMU_EVT) {
				str_cpy_p_to_a(evt->info, "send event to guest");
			} else if (type == KEV_VSMMU_PRI) {
				str_cpy_p_to_a(evt->info, "send pri to guest");
			} else if (type == KEV_VSMMU_STE) {
				str_cpy_p_to_a(evt->info, "set ste");
			} else {
				str_cpy_p_to_a(evt->info, "invalid type");
			}
			evt->cmd_evt_type = cmd_evt_type;
			evt->ret = ret;),
		KEV_PRINT("[seq=%u] vmid=%u %s: phy_smmu_id=%u stream_id=%u cmd_evt_type=%u, ret=%d\n",
				evt->sequence, evt->vm_id, evt->info, evt->phy_smmu_id,
				evt->stream_id, evt->cmd_evt_type, evt->ret))

#ifdef __HOST_LLT__
/*
 * On 64 bit system,
 * sizeof(struct kev_test1) should be 253 * 8
 * Let's set RB_SIZE to 4096 (512 * 8) for testing.
 *
 * On 32 bit system,
 * sizeof(struct kev_test1) should be 511 * 4
 * Let's set RB_SIZE to 4096 (1024 * 4) for testing.
 */
DEFINE_KEV(TEST1, test1,
	   KEV_ARG(1, int, val),
	   KEV_STRUCT(2, unsigned int, sequence, char, __KEV_STRING(string,
#if (__SIZEOF_LONG__ == 8)
			   2000
#else
			   2028
#endif
		)),
	   KEV_ASSIGN(KEV_READ_SEQ(evt->sequence);
		      mem_zero_s(evt->string); evt->string[0] = '\0'),
	   KEV_PRINT("%d", (int)evt->string[0]))
#endif
