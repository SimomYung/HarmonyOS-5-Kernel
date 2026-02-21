/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei RTOS
 * Create: Tue Aug 13 11:17:00 2021
 */

#ifndef _FTS_H_
#define _FTS_H_

#include <stdint.h>
#include <sys/types.h>

// FTSENT fts_level
#define FTS_ROOTPARENTLEVEL     (-1)
#define FTS_ROOTLEVEL            0

// FTSENT fts_info
#define FTS_D            1              /* preorder directory */
#define FTS_DC           2              /* directory that causes cycles */
#define FTS_DEFAULT      3              /* none of the above */
#define FTS_DNR          4              /* unreadable directory */
#define FTS_DOT          5              /* dot or dot-dot */
#define FTS_DP           6              /* postorder directory */
#define FTS_ERR          7              /* error; errno is set */
#define FTS_F            8              /* regular file */
#define FTS_INIT         9              /* initialized only */
#define FTS_NS          10              /* stat(2) failed */
#define FTS_NSOK        11              /* no stat(2) requested */
#define FTS_SL          12              /* symbolic link */
#define FTS_SLNONE      13              /* symbolic link without target */

// FTSENT fts_flags
#define FTS_DONTCHDIR    0x01           /* don't chdir .. to the parent */
#define FTS_SYMFOLLOW    0x02           /* followed a symlink to get here */

// FTSENT fts_instr
#define FTS_AGAIN        1              /* read node again */
#define FTS_FOLLOW       2              /* follow symbolic link */
#define FTS_NOINSTR      3              /* no instructions */
#define FTS_SKIP         4              /* discard node */

struct stat;

typedef struct _ftsent {
        struct _ftsent *fts_cycle;      /* cycle node */
        struct _ftsent *fts_parent;     /* parent directory */
        struct _ftsent *fts_link;       /* next file in directory */
        long nouse1;
        void *nouse2;
        char *fts_accpath;              /* access path */
        char *fts_path;                 /* root path */
        int fts_errno;                  /* errno for this node */
        int fts_symfd;                  /* fd for symlink */
        unsigned short fts_pathlen;     /* strlen(fts_path) */
        unsigned short fts_namelen;     /* strlen(fts_name) */

        ino_t fts_ino;          /* inode */
        dev_t fts_dev;          /* device */
        nlink_t fts_nlink;      /* link count */

        short fts_level;                /* depth (-1 to N) */
        unsigned short fts_info;        /* user flags for FTSENT structure */
        unsigned short fts_flags;       /* private flags for FTSENT structure */
        unsigned short fts_instr;       /* fts_set() instructions */

        struct stat *fts_statp; /* stat(2) information */
        char fts_name[1];               /* file name */
} FTSENT;

// FTS fts_options
#define FTS_COMFOLLOW   0x001           /* follow command line symlinks */
#define FTS_LOGICAL     0x002           /* logical walk */
#define FTS_NOCHDIR     0x004           /* don't change directories */
#define FTS_NOSTAT      0x008           /* don't get stat info */
#define FTS_PHYSICAL    0x010           /* physical walk */
#define FTS_SEEDOT      0x020           /* return dot and dot-dot */
#define FTS_XDEV        0x040           /* don't cross devices */
#define FTS_OPTIONMASK  0x0ff           /* valid user option mask */

#define FTS_NAMEONLY    0x100           /* (private) child names only */
#define FTS_STOP        0x200           /* (private) unrecoverable error */

typedef struct {
        struct _ftsent *fts_cur;        /* current node */
        struct _ftsent *fts_child;      /* linked list of children */
        struct _ftsent **fts_array;     /* sort array */
        dev_t fts_dev;                  /* starting device # */
        char *fts_path;                 /* path for this descent */
        int fts_rfd;                    /* fd for root */
        unsigned int fts_pathlen;       /* sizeof(path) */
        unsigned int fts_nitems;        /* elements in the sort array */
        int (*fts_compar)               /* compare function */
                (const struct _ftsent **, const struct _ftsent **);

        int fts_options;                /* fts_open options, global flags */
} FTS;

#ifdef __cplusplus
extern "C" {
#endif
FTSENT *fts_children(FTS *f, int instr);
int fts_close(FTS *f);
FTS *fts_open(char * const *argv, int opts, int (*cmp)(const FTSENT **, const FTSENT **));
FTSENT *fts_read(FTS *f);
int fts_set(FTS *f, FTSENT *fe, int instr);
#ifdef __cplusplus
}
#endif


#endif /* !_FTS_H_ */
