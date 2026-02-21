/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of fts
 * Author: Huawei RTOS
 * Create: Tue Aug 13 11:17:00 2021
 */

#include <sys/param.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fts.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALIGNBYTES      (__alignof__ (long double) - 1)
#define align(p)        (((unsigned long int) (p) + ALIGNBYTES) & ~ALIGNBYTES)

#define is_dot(s) (strcmp((s), ".") == 0 || strcmp((s), "..") == 0)

#define is_set(opts, flag) (((unsigned int)(opts) & (flag)) != 0)
#define set(opts, flag) ((opts) = (int)((unsigned int)(opts) | (flag)))
#define reset(opts, flag) ((opts) = (int)((unsigned int)(opts) & (~(flag))))

/* fts_build flags */
#define BCHILD          1               /* fts_children */
#define BREAD           2               /* fts_read */

#define PATHLEN_PLUS 1024

static size_t fts_maxstrsize(char * const *str)
{
    size_t max = 0;
    for (char * const *s = str; *s; ++s) {
        size_t len = strlen(*s);
        max = MAX(len, max);
    }
    return max;
}

static int fts_allocp(FTS *f, size_t len)
{
    if (f == NULL) {
        return -1;
    }

    if (f->fts_pathlen + len > USHRT_MAX || len > UINT_MAX - f->fts_pathlen) {
        errno = ENAMETOOLONG;
        free(f->fts_path);
        f->fts_path = NULL;
        f->fts_pathlen = 0;
        return -1;
    }
    if (f->fts_pathlen + len + PATHLEN_PLUS > USHRT_MAX ||
        UINT_MAX - (f->fts_pathlen + len) <= PATHLEN_PLUS) {
        f->fts_pathlen = USHRT_MAX;
    } else {
        f->fts_pathlen = (unsigned int)(f->fts_pathlen + len + PATHLEN_PLUS);
    }
    if (f->fts_path == NULL) {
        f->fts_path = malloc(f->fts_pathlen);
        if (f->fts_path == NULL) {
            f->fts_pathlen = 0;
            return -1;
        }
    } else {
        char *p = malloc(f->fts_pathlen);
        if (p == NULL) {
            free(f->fts_path);
            f->fts_path = NULL;
            f->fts_pathlen = 0;
            return -1;
        }
        (void)strcpy(p, f->fts_path);
        free(f->fts_path);
        f->fts_path = p;
    }
    return 0;
}


static FTS *fts_initf(char * const *argv, int opts, int (*cmp)(const FTSENT **, const FTSENT **))
{
    FTS *f = calloc(sizeof(FTS), 1);
    if (f == NULL) {
        return NULL;
    }

    if (is_set(opts, FTS_LOGICAL)) {
        set(opts, FTS_NOCHDIR);
    }
    f->fts_options = opts;
    f->fts_compar = cmp;
    f->fts_rfd = -1;
    size_t maxsize = fts_maxstrsize(argv);
    if (fts_allocp(f, MAX(maxsize, MAXPATHLEN)) != 0) {
        free(f);
        f = NULL;
    }

    return f;
}

static FTSENT *fts_allocfe(FTS *f, const char *name, size_t namelen)
{
    if(namelen > USHRT_MAX) {
        errno = ENAMETOOLONG;
        return NULL;
    }

    size_t len = sizeof(FTSENT) + namelen;
    if (!is_set(f->fts_options, FTS_NOSTAT)) {
        len += sizeof(struct stat) + ALIGNBYTES;
    }
    FTSENT *fe = calloc(len, 1);
        if (fe == NULL) {
                return (NULL);
    }

    (void)memmove(fe->fts_name, name, namelen + 1);
    fe->fts_namelen = (unsigned short)namelen;

    fe->fts_statp = (is_set(f->fts_options, FTS_NOSTAT)) ? NULL :
                    (struct stat*)align((unsigned long)(fe->fts_name + namelen + 1));

    fe->fts_symfd = -1;
    fe->fts_path = f->fts_path;
    fe->fts_pathlen = (unsigned short)f->fts_pathlen;
    fe->fts_instr = FTS_NOINSTR;
    return (fe);
}

static FTSENT *fts_initp(FTS *f)
{
    FTSENT *parent = fts_allocfe(f, "", 0);
    if (parent == NULL) {
        return NULL;
    }
    parent->fts_level = FTS_ROOTPARENTLEVEL;

    if (!is_set(f->fts_options, FTS_NOCHDIR)) {
        f->fts_rfd = open(".", O_RDONLY | O_CLOEXEC, 0);
        if (f->fts_rfd == -1) {
            set(f->fts_options, FTS_NOCHDIR);
        }
    }

    return parent;
}

static FTSENT *fts_sort(FTS *f, FTSENT *head, unsigned int size)
{
    if (size > f->fts_nitems) {
        unsigned int nitems = (size > UINT_MAX - f->fts_nitems) ? size : (f->fts_nitems + size);
        if (f->fts_array == NULL) {
            f->fts_array = malloc(nitems * sizeof(FTSENT*));
            if (f->fts_array == NULL) {
                f->fts_nitems = 0;
                return (head);
            }
            f->fts_nitems = nitems;
        } else {
            FTSENT **array = malloc(nitems * sizeof(FTSENT*));
            if (array == NULL) {
                free(f->fts_array);
                f->fts_array = NULL;
                f->fts_nitems = 0;
                return (head);
            }
            (void)memcpy(array, f->fts_array, f->fts_nitems * sizeof(FTSENT*));
            free(f->fts_array);
            f->fts_nitems = nitems;
            f->fts_array = array;
        }
    }

    FTSENT *item = head;
    for (size_t i = 0; i < size; ++i) {
        f->fts_array[i] = item;
        item = item->fts_link;
    }
    qsort(f->fts_array, size, sizeof(FTSENT*),
          (int (*)(const void *, const void *))f->fts_compar);

    item = f->fts_array[0];
    for (size_t i = 1; i < size; ++i) {
        item->fts_link = f->fts_array[i];
        item = f->fts_array[i];
    }
    item->fts_link = NULL;

    return (f->fts_array[0]);
}

static int fts_checkcycle(FTSENT *fe)
{
    dev_t dev = fe->fts_dev;
    ino_t ino = fe->fts_ino;
    for (FTSENT *parent = fe->fts_parent; parent->fts_level > FTS_ROOTPARENTLEVEL; parent = parent->fts_parent) {
        if (parent->fts_dev == dev && parent->fts_ino == ino) {
            fe->fts_cycle = parent;
            return -1;
        }
    }
    return 0;
}

static unsigned short fts_stat(const FTS *f, FTSENT *fe, unsigned int is_follow)
{
    struct stat sb;
    struct stat *sbp = &sb;

    int saved_errno;
    if (is_follow != 0 || is_set(f->fts_options, FTS_LOGICAL)) {
        if (stat(fe->fts_accpath, sbp) != 0) {
            saved_errno = errno;
            if (lstat(fe->fts_accpath, sbp) == 0) {
                return FTS_SLNONE;
            }
            fe->fts_errno = saved_errno;
            return FTS_NS;
        }
    } else {
        if (lstat(fe->fts_accpath, sbp) != 0) {
            fe->fts_errno = errno;
            return FTS_NS;
        }
    }

    if (!is_set(f->fts_options, FTS_NOSTAT)) {
        (void)memcpy(fe->fts_statp, sbp, sizeof(struct stat));
    }

    if (S_ISDIR(sbp->st_mode)) {
        if (is_dot(fe->fts_name)) {
            return FTS_DOT;
        } else {
            fe->fts_dev = sbp->st_dev;
            fe->fts_ino = sbp->st_ino;
            fe->fts_nlink = sbp->st_nlink;

            if (fts_checkcycle(fe) != 0) {
                return FTS_DC;
            }
            return FTS_D;
        }
    } else if (S_ISLNK(sbp->st_mode)) {
        return FTS_SL;
    } else if (S_ISREG(sbp->st_mode)) {
        return FTS_F;
    }
    return FTS_DEFAULT;
}

static void fts_freelink(FTSENT *head)
{
    FTSENT *cur = head;
    while (cur != NULL) {
        FTSENT *tmp = cur->fts_link;
        free(cur);
        cur = tmp;
    }
}

static FTSENT *fts_initroots(FTS *f, FTSENT *parent, char * const *argv)
{
    unsigned int nitems = 0;
    FTSENT *head = NULL;
    FTSENT *last = NULL;
    FTSENT *cur = NULL;
    for (char * const *name = argv; *name; ++name) {
        cur = fts_allocfe(f, *name, strlen(*name));
        if (cur == NULL) {
            fts_freelink(head);
            return NULL;
        }

        cur->fts_parent = parent;
        cur->fts_level = FTS_ROOTLEVEL;
        cur->fts_link = NULL;
        cur->fts_accpath = cur->fts_name;
        cur->fts_info = fts_stat(f, cur, (unsigned int)is_set(f->fts_options, FTS_COMFOLLOW));

        if (last == NULL) {
            last = head = cur;
        } else {
            last->fts_link = cur;
            last = cur;
        }
        ++nitems;
    }

    if (nitems > 1 && f->fts_compar) {
        head = fts_sort(f, head, nitems);
    }

    return head;
}

FTS *fts_open(char * const *argv, int opts, int (*cmp)(const FTSENT **, const FTSENT **))
{
    if (argv == NULL || is_set(opts, ~FTS_OPTIONMASK)) {
        errno = EINVAL;
        return NULL;
    }

    if ((is_set(opts, FTS_LOGICAL) && is_set(opts, FTS_PHYSICAL)) ||
        (!is_set(opts, FTS_LOGICAL) && !is_set(opts, FTS_PHYSICAL))) {
        errno = EINVAL;
        return NULL;
    }

    FTS *f = fts_initf(argv, opts, cmp);
    if (f == NULL) {
        return NULL;
    }

    FTSENT *parent = fts_initp(f);
    if (parent == NULL) {
        goto parent_error;
    }

    FTSENT *head = fts_initroots(f, parent, argv);
    if (head == NULL) {
        goto roots_error;
    }

    f->fts_cur = fts_allocfe(f, "", 0);
    if (f->fts_cur == NULL) {
        goto initnode_error;
    }
    f->fts_cur->fts_link = head;
    f->fts_cur->fts_info = FTS_INIT;

    return f;

initnode_error:
    fts_freelink(head);
roots_error:
    free(parent);
parent_error:
    if (f->fts_path != NULL) {
        free(f->fts_path);
    }
    if (f->fts_array != NULL) {
        free(f->fts_array);
    }
    if (f->fts_rfd != -1) {
        (void)close(f->fts_rfd);
    }
    free(f);
    return NULL;
}

static void fts_prefollow(FTS *f, FTSENT *fe)
{
    fe->fts_info = fts_stat(f, fe, 1);
    if (fe->fts_info == FTS_D && !is_set(f->fts_options, FTS_NOCHDIR)) {
        fe->fts_symfd = open(".", O_RDONLY | O_CLOEXEC, 0);
        if (fe->fts_symfd == -1) {
            fe->fts_errno = errno;
            fe->fts_info = FTS_ERR;
        } else {
            fe->fts_flags |= FTS_SYMFOLLOW;
        }
    }
}

static int fts_chdir(const FTS *f, const FTSENT *fe, int fd, const char *input_path)
{
    if (is_set(f->fts_options, FTS_NOCHDIR)) {
        return 0;
    }

    if (fd < 0 && input_path == NULL) {
        return -1;
    }

    char path[PATH_MAX + 1] = {0x00};
    if(input_path != NULL && (strlen(input_path) > PATH_MAX || realpath(input_path, path) == NULL)) {
        return -1;
    }
    int finalfd = fd < 0 ? open(path, O_RDONLY | O_CLOEXEC) : fd;
    if (finalfd < 0) {
        return -1;
    }

    int ret = -1;
    if (fe) {
        struct stat sb;
        if (fstat(finalfd, &sb) == -1) {
            goto out;
        }

        if (sb.st_ino != fe->fts_ino || sb.st_dev != fe->fts_dev) {
            errno = ENOENT;
            goto out;
        }
    }
    ret = fchdir(finalfd);

out:
    if (finalfd != fd) {
            int save_errno = errno;
            (void)close(finalfd);
            errno = save_errno;
    }
    return ret;
}

static void fts_adjustp(FTS *f, FTSENT *head)
{
    for (FTSENT *p = head;
        p->fts_level >= FTS_ROOTLEVEL;
        p = p->fts_link ? p->fts_link : p->fts_parent) {
        if (p->fts_accpath != p->fts_name) {
            p->fts_accpath =
                f->fts_path + (unsigned int)(p->fts_accpath - p->fts_path);
        }
        p->fts_path = f->fts_path;
    }
}

static FTSENT *fts_init_fepath(FTS *f, FTSENT *parent, FTSENT *cur)
{
    size_t namlen = cur->fts_namelen;
    cur->fts_level = parent->fts_level + 1;
    if (is_set(f->fts_options, FTS_NOCHDIR)) {
        unsigned int maxlen = f->fts_pathlen;
        unsigned int uselen = 0;
        if (f->fts_path[parent->fts_pathlen - 1] == '/') {
            uselen = parent->fts_pathlen;
        } else {
            f->fts_path[parent->fts_pathlen] = '/';
            uselen = ++parent->fts_pathlen;
        }
        if (maxlen < uselen + namlen + 1 && fts_allocp(f, namlen + 1) != 0) {
            parent->fts_info = FTS_ERR;
            set(f->fts_options, FTS_STOP);
            return (NULL);
        }
        (void)memmove(f->fts_path + uselen,
            cur->fts_name, namlen + 1);
        cur->fts_pathlen = (unsigned short)(uselen + namlen);
        cur->fts_accpath = f->fts_path;
    } else {
        cur->fts_accpath = cur->fts_name;
    }
    cur->fts_info = fts_stat(f, cur, 0);
    return cur;
}

static FTSENT *fts_initchild(FTS *f, FTSENT *parent, FTSENT *last, struct dirent *d, int type)
{
    size_t namlen = strlen(d->d_name);
    FTSENT *cur = fts_allocfe(f, d->d_name, namlen);
    if (cur == NULL) {
        parent->fts_info = FTS_ERR;
        set(f->fts_options, FTS_STOP);
        return (NULL);
    }
    cur->fts_parent = parent;
    if (is_set(f->fts_options, FTS_NAMEONLY)) {
        cur->fts_info = FTS_NSOK;
    } else if (fts_init_fepath(f, parent, cur) == NULL) {
        int saved_errno = errno;
        free(cur);
        errno = saved_errno;
        return NULL;
    }

    cur->fts_link = NULL;
    if (last) {
        last->fts_link = cur;
    }
    return cur;
}

static DIR *fts_walkin(FTS *f, FTSENT *fe, int type)
{
    if (fe->fts_level == SHRT_MAX) {
        fe->fts_info = FTS_ERR;
        set(f->fts_options, FTS_STOP);
        errno = ENAMETOOLONG;
        return (NULL);
    }
    DIR *dir = opendir(fe->fts_accpath);
    if (dir == NULL) {
        if (type == BREAD) {
            fe->fts_info = FTS_DNR;
            fe->fts_errno = errno;
        }
        return NULL;
    }
    if (!is_set(f->fts_options, FTS_NOCHDIR) &&
         !is_set(f->fts_options, FTS_NAMEONLY) &&
         fts_chdir(f, fe, dirfd(dir), NULL) != 0) {
        (void)closedir(dir);
        return NULL;
    }
    return dir;
}

static int fts_walkout(FTS *f, FTSENT *fe, FTSENT *child, unsigned int childitems, int type)
{
    if (is_set(f->fts_options, FTS_NAMEONLY)) {
        return 0;
    }
    if (is_set(f->fts_options, FTS_NOCHDIR)) {
        if (f->fts_path[fe->fts_pathlen - 1] == '/') {
            --fe->fts_pathlen;
        }
        f->fts_path[fe->fts_pathlen] = '\0';
    } else if(type != BREAD || childitems == 0) {
        if (fe->fts_level == FTS_ROOTLEVEL ?
            (fts_chdir(f, NULL, f->fts_rfd, NULL) != 0) :
            (fts_chdir(f, fe->fts_parent, -1, "..") != 0)) {
                fe->fts_info = FTS_ERR;
                set(f->fts_options, FTS_STOP);
                fts_freelink(child);
                return -1;
        }
    }
    return 0;
}

static FTSENT *fts_initchildren(FTS *f, FTSENT *fe, int type)
{
    DIR *dir = fts_walkin(f, fe, type);
    if (dir == NULL) {
        return NULL;
    }
    FTSENT *head = NULL;
    unsigned int nitems = 0;
    int pchange = 0;
    char *oldaddr = f->fts_path;
    FTSENT *last = head;
    FTSENT *cur = head;
    for (struct dirent *d = readdir(dir); d != NULL; d = readdir(dir)) {
        if (!is_set(f->fts_options, FTS_SEEDOT) && is_dot(d->d_name)) {
            continue;
        }

        cur = fts_initchild(f, fe, last, d, type);
        if (cur == NULL || is_set(f->fts_options, FTS_STOP)) {
            if (cur != NULL) {
                free(cur);
            }
            fts_freelink(head);
            (void)closedir(dir);
            return NULL;
        }
        pchange = (int)(oldaddr != f->fts_path);
        head = (head == NULL) ? cur : head;
        last = cur;

        ++nitems;
    }
    if (pchange != 0) {
        fts_adjustp(f, head);
    }

    (void)closedir(dir);
    if (fts_walkout(f, fe, head, nitems, type) == -1) {
        return NULL;
    }

    if (nitems == 0 && type == BREAD) {
        fe->fts_info = FTS_DP;
    }
    if (nitems > 1 && f->fts_compar) {
        head = fts_sort(f, head, nitems);
    }
    return head;
}

static void fts_loadroot(FTS *f, FTSENT *root)
{
    root->fts_pathlen = root->fts_namelen;
    (void)memmove(f->fts_path, root->fts_name, root->fts_namelen + 1);
    root->fts_accpath = f->fts_path;

    char *name = strrchr(root->fts_name, '/');
    if (name != NULL && (name != root->fts_name || root->fts_namelen > 1)) {
        root->fts_namelen = (unsigned short)strlen(++name);
        (void)memmove(root->fts_name, name, root->fts_namelen + 1);
    }

    f->fts_dev = root->fts_dev;
}

static char *fts_get_pathend(FTS *f, FTSENT *fe)
{
    if (f->fts_path[fe->fts_pathlen - 1] == '/') {
        return f->fts_path + fe->fts_pathlen;
    } else {
        f->fts_path[fe->fts_pathlen] = '/';
        return f->fts_path + fe->fts_pathlen + 1;
    }
}

static FTSENT *fts_rdown_dir(FTS *f)
{
    FTSENT *fe = f->fts_cur;
    if (fe->fts_instr == FTS_SKIP ||
        (is_set(f->fts_options, FTS_XDEV) && fe->fts_dev != f->fts_dev)) {
        if (is_set(fe->fts_flags, FTS_SYMFOLLOW)) {
            (void)close(fe->fts_symfd);
            fe->fts_symfd = -1;
        }
        if (f->fts_child != NULL) {
            fts_freelink(f->fts_child);
            f->fts_child = NULL;
        }
        fe->fts_info = FTS_DP;
        return fe;
    }
    if (is_set(f->fts_options, FTS_NAMEONLY)) {
        reset(f->fts_options, FTS_NAMEONLY);
        fts_freelink(f->fts_child);
        f->fts_child = NULL;
    }
    if (f->fts_child != NULL) {
        if (fts_chdir(f, fe, -1, fe->fts_accpath) != 0) {
            fe->fts_errno = errno;
            fe->fts_flags |= FTS_DONTCHDIR;
            for (FTSENT *child = f->fts_child; child != NULL; child = child->fts_link) {
                child->fts_accpath = child->fts_parent->fts_accpath;
            }
        }
    } else {
        f->fts_child = fts_initchildren(f, fe, BREAD);
        if (f->fts_child == NULL && is_set(f->fts_options, FTS_STOP)) {
            if (is_set(f->fts_options, FTS_STOP)) {
                return NULL;
            }
            return fe;
        }
    }
    char *end = fts_get_pathend(f, fe);
    FTSENT *next = f->fts_child;
    if (next != NULL) {
        if (next->fts_parent != f->fts_cur) {
            set(f->fts_options, FTS_STOP);
            return NULL;
        }
        (void)memmove(end, next->fts_name, next->fts_namelen + 1);
        f->fts_child = NULL;
        f->fts_cur = next;
    }
    return next;
}

static FTSENT *fts_rdown(FTS *f)
{
    FTSENT *fe = f->fts_cur;
    if (fe == NULL) {
        return NULL;
    }

    if (fe->fts_info == FTS_SL || fe->fts_info == FTS_SLNONE) {
        if (fe->fts_instr == FTS_FOLLOW) {
            fts_prefollow(f, fe);
            return fe;
        }
    } else if (fe->fts_info == FTS_D) {
        return fts_rdown_dir(f);
    }

    return NULL;
}

static FTSENT *fts_rnext(FTS *f)
{
    if (f->fts_cur == NULL) {
        return NULL;
    }
    FTSENT *fe = f->fts_cur->fts_link;
    int readed = 0;
    while (fe != NULL && readed == 0) {
        free(f->fts_cur);
        f->fts_cur = fe;
        if (fe->fts_instr == FTS_SKIP) {
            fe = fe->fts_link;
            continue;
        }

        if (fe->fts_level == FTS_ROOTLEVEL) {
            fts_loadroot(f, fe);
            return fe;
        }

        FTSENT *parent = fe->fts_parent;
        char *end = fts_get_pathend(f, parent);
        (void)memmove(end, fe->fts_name, fe->fts_namelen + 1);

        if (fe->fts_info == FTS_SL || fe->fts_info == FTS_SLNONE) {
            if (fe->fts_instr == FTS_FOLLOW) {
                fts_prefollow(f, fe);
            }
        }

        readed = 1;
    }

    return fe;
}

static FTSENT *fts_rup(FTS *f)
{
    if (f->fts_cur == NULL) {
        return NULL;
    }
    FTSENT *fe = f->fts_cur->fts_parent;
    free(f->fts_cur);
    f->fts_cur = NULL;

    if (fe->fts_level == FTS_ROOTPARENTLEVEL) {
        free(fe);
        errno = 0;
        return NULL;
    }

    if (is_set(f->fts_options, FTS_NOCHDIR)) {
        f->fts_path[fe->fts_pathlen] = '\0';
    } else if ((fe->fts_flags & FTS_DONTCHDIR) == 0) {
        if (fe->fts_level == FTS_ROOTLEVEL) {
            if (fts_chdir(f, NULL, f->fts_rfd, NULL) != 0) {
                f->fts_cur = fe;
                int saved_errno = errno;
                (void)close(f->fts_rfd);
                f->fts_rfd = -1;
                errno = saved_errno;
                set(f->fts_options, FTS_STOP);
                return NULL;
            }
        } else if ((fe->fts_flags & FTS_SYMFOLLOW) != 0) {
            if (fts_chdir(f, NULL, fe->fts_symfd, NULL) != 0) {
                f->fts_cur = fe;
                int saved_errno = errno;
                (void)close(fe->fts_symfd);
                fe->fts_symfd = -1;
                errno = saved_errno;
                set(f->fts_options, FTS_STOP);
                return NULL;
            }
            (void)close(fe->fts_symfd);
            fe->fts_symfd = -1;
        } else {
            if (fts_chdir(f, fe->fts_parent, -1, "..") != 0) {
                f->fts_cur = fe;
                set(f->fts_options, FTS_STOP);
                return NULL;
            }
        }
    }

    fe->fts_info = FTS_DP;
    f->fts_cur = fe;

    return fe;
}

FTSENT *fts_read(FTS *f)
{
    if (f == NULL) {
        errno = EINVAL;
        return NULL;
    }

    FTSENT *fe = f->fts_cur;
    if (fe == NULL || is_set(f->fts_options, FTS_STOP)) {
        return NULL;
    }

    if (fe->fts_instr == FTS_AGAIN) {
        fe->fts_info = fts_stat(f, fe, 0);
        fe->fts_instr = FTS_NOINSTR;
        return fe;
    }

    fe = fts_rdown(f);
    if (fe == NULL && !is_set(f->fts_options, FTS_STOP)) {
        fe = fts_rnext(f);
        if (fe == NULL && !is_set(f->fts_options, FTS_STOP)) {
            fe = fts_rup(f);
        }
    }

    if (fe != NULL) {
        fe->fts_instr = FTS_NOINSTR;
    }
    return fe;
}

FTSENT *fts_children(FTS *f, int instr)
{
    if (f == NULL || (instr != 0 && instr != FTS_NAMEONLY)) {
        errno = EINVAL;
        return NULL;
    }

    FTSENT *fe = f->fts_cur;
    if (fe == NULL) {
        return NULL;
    }

    if (is_set(f->fts_options, FTS_STOP)) {
        return NULL;
    }

    if (fe->fts_info == FTS_INIT) {
        return fe->fts_link;
    }

    if (fe->fts_info != FTS_D) {
        return (NULL);
    }

    if (instr == FTS_NAMEONLY) {
        set(f->fts_options, FTS_NAMEONLY);
    }

    errno = 0;

    f->fts_child = fts_initchildren(f, fe, BCHILD);
    if (f->fts_child == NULL) {
        set(f->fts_options, FTS_STOP);
        return NULL;
    }

    return f->fts_child;
}

int fts_close(FTS *f)
{
    if (f == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (f->fts_cur) {
        FTSENT *fe, *freep;
        for (fe = f->fts_cur; fe->fts_level > FTS_ROOTPARENTLEVEL;) {
            freep = fe;
            fe = fe->fts_link ? fe->fts_link : fe->fts_parent;
            if ((freep->fts_flags & FTS_SYMFOLLOW) != 0) {
                (void)close(freep->fts_symfd);
            }
            free(freep);
        }
        free(fe);
    }

    if (f->fts_child) {
        fts_freelink(f->fts_child);
    }
    if (f->fts_path) {
        free(f->fts_path);
    }
    if (f->fts_array) {
        free(f->fts_array);
    }

    int saved_errno = 0;
    if (!is_set(f->fts_options, FTS_NOCHDIR) && fts_chdir(f, NULL, f->fts_rfd, NULL) != 0) {
        saved_errno = errno;
    }
    (void)close(f->fts_rfd);

    free(f);

    if (saved_errno != 0) {
        errno = saved_errno;
        return -1;
    }
    return 0;
}

int fts_set(FTS *f, FTSENT *fe, int instr)
{
    if (f == NULL || fe == NULL || (instr != FTS_AGAIN &&
        instr != FTS_FOLLOW && instr != FTS_NOINSTR && instr != FTS_SKIP)) {
            errno = EINVAL;
            return -1;
    }
    fe->fts_instr = (unsigned short)instr;
    return 0;
}

