/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Common interface for sysmgr procfs
 * Author: Huawei OS Kernel Lab
 * Create: Wed 27 September 2023 16:00
 */
#ifndef SYSMGR_PROCFS_PROCFSMGR_PID_H
#define SYSMGR_PROCFS_PROCFSMGR_PID_H

int procfs_pid_yama_check_open(unsigned long credential, pid_t pid);

struct parse_wchan {
	int *cnt;
	char *wchan;
};

#ifdef CONFIG_VFS_FUSION
#define PROC_COMM_SIZE 256
struct pipe_process_info {
	int pid;
	int fd;
	uint32_t cnode_idx;
	char *name;
	void *data;
	ssize_t offset;
};

int procfs_pipe_info_fill_entry(struct process_s *process, void *pipedata);
int procfs_pipe_info_cb(struct pipe_process_info *pinfo);
ssize_t pipe_info_entry(struct pipe_process_info *pinfo);

int fs_procfs_pid_read(int pid, unsigned int nsid, unsigned long long seq,
		       struct hm_actv_buf mem_buf, struct hm_actv_buf actv_buf);
int fs_procfs_check_tid(pid_t pid, pid_t tid, unsigned int nsid, unsigned long long seq);
#endif

pid_t get_current_tid_in_ns(pid_t tid, unsigned int nsid);
int nsid_of_cnode_idx(unsigned int cnode_idx, int flag, unsigned int *nsid);
pid_t get_pid_of_cnode_idx_in_ns(unsigned long credential,
				 uint32_t cnode_idx, unsigned int nsid);
int get_cnode_idx_in_caller_pidns(unsigned long credential, pid_t pid,
				  uint32_t *cnode_idx, unsigned int nsid);
int procfs_get_irq_num(void);
int get_threads_nr(unsigned long credential, unsigned int nsid, int pid, unsigned long long seq);
int get_processes_nr(void);

#ifdef CONFIG_RESMGR
bool process_in_anco(struct process_s *process);
#endif

int pid_attr_current_read(int pid, unsigned long long seq, unsigned int pid_ns,
char *data, size_t data_len);

int pid_attr_current_write(int pid, unsigned long long seq, unsigned int pid_ns,
 const char *src, size_t size, uint64_t *pos);

int tid_attr_current_read(int tid, unsigned long long seq, unsigned int pid_ns,
char *data, size_t data_len);

int tid_attr_current_write(int tid, unsigned long long seq, unsigned int pid_ns,
 const char *src, size_t size, uint64_t *pos);

#endif
