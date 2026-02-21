/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: uev list for lock tracing
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 20 10:57:42 2022
 */

#ifdef CONFIG_LOCK_TRACE_UEV_LOGGING

UEV_POINT(LOCKS, lock_event,
	  UEV_ARG(3, enum locktrace_event_type, type, uint64_t, data, vatomic32_t *, lock_id_p),
	  UEV_TRACE_FIELD(4, enum locktrace_event_type, type, uint64_t, ts, uint64_t, data,
			  uint64_t, lock_id),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "lock_event");
			   uev_data_write_int(&trace_data, type);
			   uev_data_write_u64(&trace_data, uev_trace_lock_event_get_time());
			   uev_data_write_u64(&trace_data, data);
			   uev_data_write_u64(&trace_data, lock_id_p ? uev_trace_lock_event_set_lockid(lock_id_p) : 0UL);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, type)
			       OUTPUT_NUM(uint64_t, ts)
			       OUTPUT_NUM(uint64_t, data)
			       OUTPUT_NUM(uint64_t, lock_id)))

UEV_POINT(LOCKS, lock_pair_event,
	  UEV_ARG(4, enum locktrace_event_type, type, uint64_t, data, vatomic32_t *, lock_id_p1, vatomic32_t *, lock_id_p2),
	  UEV_TRACE_FIELD(5, enum locktrace_event_type, type, uint64_t, ts, uint64_t, data,
			  uint64_t, lock_id1, uint64_t, lock_id2),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "lock_event");
			   uev_data_write_int(&trace_data, type);
			   uev_data_write_u64(&trace_data, uev_trace_lock_event_get_time());
			   uev_data_write_u64(&trace_data, data);
			   uev_data_write_u64(&trace_data, lock_id_p1 ? uev_trace_lock_event_set_lockid(lock_id_p1) : 0UL);
			   uev_data_write_u64(&trace_data, lock_id_p2 ? uev_trace_lock_event_set_lockid(lock_id_p2) : 0UL);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, type)
			       OUTPUT_NUM(uint64_t, ts)
			       OUTPUT_NUM(uint64_t, data)
			       OUTPUT_NUM(uint64_t, lock_id1)
			       OUTPUT_NUM(uint64_t, lock_id2)))

#endif /* #ifdef CONFIG_LOCK_TRACE_UEV_LOGGING */
