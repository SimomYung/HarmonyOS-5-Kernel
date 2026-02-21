/* vi: set sw=4 ts=4: */
/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 22 14:32:13 2022
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#ifndef RESTORCON_H
#define RESTORCON_H 1
#include <stdlib.h>

#define MATCHPATHCON_BASEONLY 1 /* Only process the base file_contexts file. */
#define MATCHPATHCON_NOTRANS  2 /* Do not perform any context translation. */
#define MATCHPATHCON_VALIDATE 4 /* Validate/canonicalize contexts at init time. */

void set_matchpathcon_flags(unsigned int flags);

void set_matchpathcon_canoncon(int (*f) (const char *p, unsigned l, char **c));

int matchpathcon_init_prefix(const char *path, const char *subset);

void matchpathcon_checkmatches(char *str __attribute__((unused)));

void set_matchpathcon_printf(void (*f) (const char *fmt, ...));

void matchpathcon_filespec_eval(void);

void matchpathcon_filespec_destroy(void);

int matchpathcon_index(const char *name, mode_t mode, char **con);

int matchpathcon_filespec_add(ino_t ino, int specind, const char *file);
int security_canonicalize_context_raw(const char *con, char **canoncon);

void freecon(char *con);

int lgetfilecon_raw(const char *path, char **context);

int lsetfilecon(const char *path, const char *context);

int is_context_customizable(const char *scontext);
void matchpathcon_fini_prefix(void);
int matchpathcon_init(const char *path);
#endif
