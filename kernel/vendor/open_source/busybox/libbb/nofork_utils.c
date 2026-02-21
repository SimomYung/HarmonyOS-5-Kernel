/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023.
 * Description: add funcs modified from xfuncs to adapt to nofork
 * Author: xiongshenglan <xiongshenglan@huawei.com>
 * Create: 2023-09-26
 */
#include "libbb.h"

typedef struct ino_dev_hash_bucket_struct {
	ino_t ino;
	dev_t dev;
	/*
	 * Above fields can be 64-bit, while pointer may be 32-bit.
	 * Putting "next" field here may reduce size of this struct:
	 */
	struct ino_dev_hash_bucket_struct *next;
	/*
	 * Reportedly, on cramfs a file and a dir can have same ino.
	 * Need to also remember "file/dir" bit:
	 */
	char isdir; /* bool */
	char name[1];
} ino_dev_hashtable_bucket_t;

#define HASH_SIZE      311u   /* Should be prime */
#define hash_inode(i)  ((unsigned)(i) % HASH_SIZE)

/* array of [HASH_SIZE] elements */
static ino_dev_hashtable_bucket_t **ino_dev_hashtable;
 
char* FAST_FUNC malloc_readlink_or_warn(const char *path)
{
	char *buf = malloc_readlink(path);
 
	if (!buf && errno == ENOMEM) {
		bb_simple_error_msg(bb_msg_memory_exhausted);
		return NULL;
	} else if (!buf) {
		/* EINVAL => "file: Invalid argument" => puzzled user */
		const char *errmsg = "not a symlink";
		int err = errno;
		if (err != EINVAL)
			errmsg = strerror(err);
		bb_error_msg("%s: cannot read link: %s", path, errmsg);
		return NULL;
	}
	return buf;
}
 
 
/*
 * NOTE: This function returns a malloced char* that you will have to free
 * yourself.
 */
char* FAST_FUNC malloc_readlink(const char *path)
{
	char *buf;
	int readsize;
 
	buf = malloc(PATH_MAX);
	if (!buf) {
		return NULL;
	}
 
	readsize = readlink(path, buf, PATH_MAX);
	if (readsize == -1 || PATH_MAX < readsize + 1) {
		free(buf);
		return NULL;
	}
 
	buf[readsize] = '\0';
	return buf;
}

int FAST_FUNC print_numbered_lines_nofork(struct number_state *ns, const char *filename)
{
	FILE *fp = fopen_or_warn_stdin(filename);
	unsigned N;
	char *line;

	if (!fp)
		return EXIT_FAILURE;

	N = ns->start;
	while ((line = malloc_fgetline(fp)) != NULL) {
		if (ns->all
		 || (ns->nonempty && line[0])
		) {
			printf("%*u%s", ns->width, N, ns->sep);
			N += ns->inc;
		} else if (ns->empty_str)
			fputs_stdout(ns->empty_str);
		puts(line);
		free(line);
	}
	ns->start = N;

	fclose(fp);

	return EXIT_SUCCESS;
}

/* Get line.  Remove trailing \n */
char* FAST_FUNC malloc_fgetline(FILE *file)
{
	size_t i;
	char *c = bb_get_chunk_from_file_nofork(file, &i);

	if (c == NULL) return NULL;
	
	if (i && c[--i] == '\n')
		c[i] = '\0';

	return c;
}

char* FAST_FUNC bb_get_chunk_from_file_nofork(FILE *file, size_t *end)
{
	int ch;
	size_t idx = 0;
	char *linebuf = NULL;

	while ((ch = getc(file)) != EOF) {
		/* grow the line buffer as necessary */
		if (!(idx & 0xff)) {
			if (idx == ((size_t)-1) - 0xff)
				goto RET_1;
			linebuf = realloc(linebuf, idx + 0x100);
			if (linebuf == NULL)
				goto RET_1;
		}
		linebuf[idx++] = (char) ch;
		if (ch == '\0')
			break;
		if (end && ch == '\n')
			break;
	}
	if (end)
		*end = idx;
	if (linebuf) {
		linebuf = realloc(linebuf, idx + 1);
		if (linebuf == NULL)
			goto RET_1;
		linebuf[idx] = '\0';
	}
 RET_1:
		bb_simple_error_msg(bb_msg_memory_exhausted);
		return NULL;

	return linebuf;
}

/* Add statbuf to statbuf hash table */
void FAST_FUNC add_to_ino_dev_hashtable_nofork(const struct stat *statbuf, const char *name)
{
	int i;
	ino_dev_hashtable_bucket_t *bucket;

	if (!name)
		name = "";
	bucket = malloc(sizeof(ino_dev_hashtable_bucket_t) + strlen(name));
	if (bucket == NULL) {
		bb_error_msg("malloc in add_to_ino_dev_hashtable fails.\n");
		return;
	}
	bucket->ino = statbuf->st_ino;
	bucket->dev = statbuf->st_dev;
	bucket->isdir = !!S_ISDIR(statbuf->st_mode);
	strcpy(bucket->name, name);

	if (!ino_dev_hashtable) {
		ino_dev_hashtable = malloc(HASH_SIZE * sizeof(*ino_dev_hashtable));
		if (ino_dev_hashtable < 0) {
			bb_error_msg("malloc in add_to_ino_dev_hashtable fails.\n");
			free(bucket);
			return;
		}
		memset_s(ino_dev_hashtable, 0, HASH_SIZE * sizeof(*ino_dev_hashtable));
	}
	i = hash_inode(statbuf->st_ino);
	bucket->next = ino_dev_hashtable[i];
	ino_dev_hashtable[i] = bucket;
}

/* Concatenate path and filename to new allocated buffer.
 * Add '/' only as needed (no duplicate // are produced).
 * If path is NULL, it is assumed to be "/".
 * filename should not be NULL.
 */
 
char* FAST_FUNC concat_path_file_nofork(const char *path, const char *filename)
{
	char *lc;
 
	if (!path)
		path = "";
	lc = last_char_is(path, '/');
	while (*filename == '/')
		filename++;
	return asprintf_nofork("%s%s%s", path, (lc==NULL ? "/" : ""), filename);
}

/* Die with an error message if we can't malloc() enough space and do an
 * sprintf() into that space.
 */
char* FAST_FUNC asprintf_nofork(const char *format, ...)
{
	va_list p;
	int r;
	char *string_ptr;
 
	va_start(p, format);
	r = vasprintf(&string_ptr, format, p);
	va_end(p);
 
	if (r < 0) {
		bb_simple_error_msg(bb_msg_memory_exhausted);
		return NULL;
	}
	return string_ptr;
}