/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: dma_buf_udk_pal.h
 * Create: 2024/2/10
 */

#ifndef _DMA_BUF_PAL_H
#define _DMA_BUF_PAL_H

#include <linux/types.h>

#define SG_CHAIN	0x01UL
#define SG_END		0x02UL

enum dma_buf_udk_minor_cmd {
	CMD_HEAP_ALLOC = 0,
	CMD_BUF_GET,
	CMD_BUF_PUT,
	CMD_BUF_ATTACH,
	CMD_BUF_DETACH,
	CMD_BUF_ATTACHMENT,
	CMD_BUF_UNATTACHMENT,
	CMD_BUF_GET_DEVICE,
	CMD_BUF_BEGIN_CPU_ACCESS,
	CMD_BUF_END_CPU_ACCESS,
	CMD_BUF_GET_FD,
	CMD_BUF_ATTACH_LB,
	CMD_BUF_DETACH_LB,
	CMD_HEAP_CACHE_OPERATE,
	CMD_BUF_VMAP,
	CMD_BUF_VUNMAP,
	CMD_BUF_KMAP,
	CMD_BUF_KUNMAP,
	CMD_DATA_COPY,
	CMD_MK_PROT,
	CMD_GET_HEAP_TOTAL_SIZE,
	CMD_TEST = 100,
};

enum dma_buf_ldk_minor_cmd {
	CMD_FREE = 0,
	CMD_SVM_FLUSH_CACHE,
	CMD_LB,
};

struct pal_udk_char_context {
	void *private_data;
	unsigned int io_flags;
	void *chrdev;
};

struct pal_udk_file {
	struct pal_udk_char_context ctx;
	void *filp_node;
	void *fops;
};

struct pal_udk_dma_buf {
    size_t size;
	void *file;
	struct list_head attachments;
	void *ops;
	unsigned int vmapping_counter;
	void *vmap_ptr;
	unsigned int vmapping_counter_ldk;
	void *vmap_ptr_ldk;
	const char *exp_name;
	const char *name;
	//void *owner;
	struct list_head list_node;
	void *priv;
	unsigned int name_lock;
	void *ldk_ptr;
	void *pal_ptr;
	void *udk_ptr;
	bool from_udk;
};

struct pal_udk_device {
	char *name;
	void *parent;
	void *dt_node;
	void *base;
	void *priv;
	/* this function is used to release the owner of pal_udk_device or pal_udk_device itself */
	void *release_func;

	int parent_devid;

	unsigned long long  *dma_mask;
	bool dma_coherent;
	unsigned int coherent_dma_mask;
	const void *dma_range_map;
};

struct pal_udk_ext_scatterlist {
	unsigned long page_link;
	unsigned int offset;
	unsigned int length;
	unsigned long dma_address;
	unsigned int dma_length;
};

struct pal_udk_ext_sg_table {
	struct pal_udk_ext_scatterlist *sgl; /* the list */
	unsigned int nents;  /* number of mapped entries */
	unsigned int orig_nents; /* original size of list */
	void *udk_sg_ptr;//udk sgl ptr
};

struct pal_udk_dma_buf_attachment {
	void *dmabuf;
	void *ldk_device;
	struct list_head node;
	void *sgt;
	enum dma_data_direction dir;
	bool peer2peer;
	void *importer_ops;
	void *importer_priv;
	void *priv;
	unsigned long dma_map_attrs;
	void *udk_sgt;
};

struct dma_buf_ioctl_heap_alloc {
    const char *name;
    size_t len;
    unsigned int heap_flags;
	int *fd;
	struct pal_udk_dma_buf *dmabuf;
	void *ldk_ptr;
	int out_size;
};

struct dma_buf_ioctl_attach {
    void *dmabuf;
    struct pal_udk_device *dev;
	int dev_size;
    struct pal_udk_dma_buf_attachment *attachment; /* out */
	int out_size;
};

struct dma_buf_ioctl_detach {
    void *dmabuf;
    struct pal_udk_dma_buf_attachment *attachment;
};

struct dma_buf_ioctl_buf_get {
    int fd;
    struct pal_udk_dma_buf *dmabuf; /* out */
	int out_size;
	void *ldk_ptr;
};

struct dma_buf_ioctl_map_attachment {
    struct pal_udk_dma_buf_attachment *attachment;
	int attachment_size;
    enum dma_data_direction dirct;
    struct pal_udk_ext_sg_table *table; /* out */
	int out_size;
};

struct dma_buf_ioctl_device_get {
    void *dmabuf;
    struct pal_udk_device *dev; /* out */
	int out_size;
};
struct dma_buf_ioctl_vmap {
    void *dmabuf;
    void *vaddr; /* out */
	unsigned long page_num;
	int out_size;
};

struct dma_buf_ioctl_cpu_access {
    void *dmabuf;
    enum dma_data_direction direction;
	int *ret;
};

struct dma_buf_ioctl_get_fd {
    void *dmabuf;
    int flags;
    int *fd;
};
struct dma_buf_ioctl_attach_lb {
    int fd;
    unsigned int pid;
    unsigned long offset;
    size_t size;
	int *ret;
};
struct dma_buf_ioctl_detach_lb {
    int fd;
	int *ret;
};

struct dma_buf_ioctl_cache_op {
    int fd;
    unsigned long uaddr;
	unsigned long offset;
    unsigned long length;
    unsigned int cmd;
	int *ret;
};
struct dma_buf_ioctl_mk_prot {
    void *dmabuf;
    int *prot;
};

struct dma_buf_svm_flush_cache {
    unsigned long start;
    unsigned long len;
};

#ifdef CONFIG_DMABUF_UDK_EN
struct dma_buf *dma_buf_call_dma_buf_get(int fd);
void dma_buf_call_dma_buf_put(struct dma_buf *buf);
struct dma_buf_attachment *dma_buf_call_dma_buf_attach(struct dma_buf *buf, struct device *dev);
int dma_buf_call_dma_buf_detach(struct dma_buf *buf, struct dma_buf_attachment *attach);
struct sg_table *dma_buf_call_dma_buf_attachment(struct dma_buf_attachment *attach, enum dma_data_direction direction);
void dma_buf_call_dma_buf_unattachment(struct dma_buf_attachment *attach, struct sg_table *sg, enum dma_data_direction direction);
struct device *dma_buf_call_device_get(struct dma_buf *buf);
struct dma_buf * dma_buf_call_heap_alloc(const char *name, size_t len, unsigned int heap_flags);
int dma_buf_call_dma_buf_begin_cpu_access(struct dma_buf *buf, enum dma_data_direction direction);
int dma_buf_call_dma_buf_end_cpu_access(struct dma_buf *buf, enum dma_data_direction direction);
int dma_buf_call_dma_buf_fd(struct dma_buf *dmabuf, int flags);
void *dma_buf_call_dma_buf_vmap(struct dma_buf *buf);
void dma_buf_call_dma_buf_vunmap(struct dma_buf *buf, void *vaddr);
void *dma_buf_call_dma_buf_kmap(struct dma_buf *buf, unsigned long page_num);
void dma_buf_call_dma_buf_kunmap(struct dma_buf *buf, unsigned long page_num, void *vaddr);
int dma_buf_call_dma_buf_attach_lb(int fd, unsigned int pid, unsigned long offset, size_t size);
int dma_buf_call_dma_buf_detach_lb(int fd);
int dma_buf_call_heap_cache_operate(int fd, unsigned long uaddr, unsigned long offset, unsigned long length, unsigned int cmd);
int dma_buf_call_helper_mk_prot(struct dma_buf *buf);
#else
static inline struct dma_buf *dma_buf_call_dma_buf_get(int fd) {return NULL;}
static inline void dma_buf_call_dma_buf_put(struct dma_buf *buf) {}
static inline struct dma_buf_attachment *dma_buf_call_dma_buf_attach(struct dma_buf *buf, struct device *dev) {return NULL;}
static inline int dma_buf_call_dma_buf_detach(struct dma_buf *buf, struct dma_buf_attachment *attach) {return -1;}
static inline struct sg_table *dma_buf_call_dma_buf_attachment(struct dma_buf_attachment *attach,
							       enum dma_data_direction direction) {return NULL;}
static inline void dma_buf_call_dma_buf_unattachment(struct dma_buf_attachment *attach, struct sg_table *sg,
						     enum dma_data_direction direction) {}
static inline struct device *dma_buf_call_device_get(struct dma_buf *buf) {return NULL;}
static inline struct dma_buf * dma_buf_call_heap_alloc(const char *name, size_t len, unsigned int heap_flags) {return NULL;}
static inline int dma_buf_call_dma_buf_begin_cpu_access(struct dma_buf *buf, enum dma_data_direction direction) {return -1;}
static inline int dma_buf_call_dma_buf_end_cpu_access(struct dma_buf *buf, enum dma_data_direction direction) {return -1;}
static inline int dma_buf_call_dma_buf_fd(struct dma_buf *dmabuf, int flags) {return -1;}
static inline void *dma_buf_call_dma_buf_vmap(struct dma_buf *buf) {return NULL;}
static inline void dma_buf_call_dma_buf_vunmap(struct dma_buf *buf, void *vaddr) {}
static inline void *dma_buf_call_dma_buf_kmap(struct dma_buf *buf, unsigned long page_num) {return NULL;}
static inline void dma_buf_call_dma_buf_kunmap(struct dma_buf *buf, unsigned long page_num, void *vaddr) {}
static inline int dma_buf_call_dma_buf_attach_lb(int fd, unsigned int pid, unsigned long offset, size_t size) {return -1;}
static inline int dma_buf_call_dma_buf_detach_lb(int fd) {return -1;}
static inline int dma_buf_call_heap_cache_operate(int fd, unsigned long uaddr, unsigned long offset,
						  unsigned long length, unsigned int cmd) {return -1;}
static inline int dma_buf_call_helper_mk_prot(struct dma_buf *buf) {return -1;}
#endif /* CONFIG_DMABUF_UDK_EN */

#endif /* _DMA_BUF_PAL_H */
