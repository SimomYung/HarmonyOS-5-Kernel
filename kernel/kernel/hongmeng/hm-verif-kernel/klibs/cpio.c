/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Cpio
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 19 02:05:08 2018
 */
#include <lib/cpio.h>
#include <hmkernel/compiler.h>
#ifdef __KERNEL__
#include <hmkernel/types.h>
#else
#include <hongmeng/types.h>
#endif

#ifdef __KERNEL__
#include <lib/string.h>
#endif

#ifndef ALIGN_UP
#define ALIGN_UP(val, al)    (((val) + ((al)-1UL)) & ~((al)-1UL))
#endif
static unsigned long do_hex_to_ulong(const char *str, unsigned int length)
{
	unsigned long ret = 0UL;
	unsigned int i;
	int ctmp = 0;
	unsigned int len = length;

	if (len > 16U) {
		len = 16U;
	}

	for (i = 0U; i < len; i++) {
		/* left shifting by 4 bits to mutiply it by 16 */
		ret <<= 4;

		if (str[i] >= '0' && str[i] <= '9') {
			ctmp = (unsigned char)str[i] - (unsigned char)'0';
			ret += (unsigned long)(unsigned char)ctmp;
		} else if (str[i] >= 'a' && str[i] <= 'f') {
			/* number 10 here is used to translate [a-f] to [10-15] */
			ctmp = ((unsigned char)str[i] - (unsigned char)'a') + 10;
			ret += (unsigned long)(unsigned char)ctmp;
		} else if (str[i] >= 'A' && str[i] <= 'F') {
			/* number 10 here is used to translate [A-F] to [10-15] */
			ctmp = ((unsigned char)str[i] - (unsigned char)'A') + 10;
			ret += (unsigned long)(unsigned char)ctmp;
		} else {
			break;
		}

		continue;
	}

	return ret;
}

/*
 * Parse an ASCII hex string into an unsigned long integer
 *
 * @s: the hex string
 * @len: the length of the string
 *
 * Return unsigned long integer of the given string
 * ---------------------------------------------------------------
 * CODEREVIEW CHECKLIST
 * ARG: str: from cpio archive checked, it can't be NULL. It's always valid
 *           from. call stack:
 *           1. hex_to_ulong<-cpio_parse_entry<-strip_cpio_entry(tools/cpio-strip)
 *           2. hex_to_ulong<-cpio_parse_entry<-cpio_header_parse(tools/elfloader)
 *           It's used on host to strip cpio archive and used in elfloader to parse
 *           cpio archive, usr cant't change str, so the str always a valid hex string.
 *	len: usr input, checked by caller to guarantee there's no str overflow.
 * RIGHTS: N/A
 * BUFOVF: checked by caller to guarantee there's no str overflow.
 * LOG: N/A
 * RET: N/A
 * RACING: N/A
 * LEAK: N/A
 * ARITHOVF: len should be less than 16 to not exceed ulong limit
 * ---------------------------------------------------------------
 */
unsigned long hex_to_ulong(const char *str, unsigned int len)
{
	unsigned long ret = 0UL;

	if (str != NULL) {
		ret = do_hex_to_ulong(str, len);
	}

	return ret;
}

/* Compare two strings in 'n' characters is equal or not
 * ---------------------------------------------------------------
 * CODEREVIEW CHECKLIST
 * ARG: str1/str2: usr input checked
 *		strlen: usr input, cpio_parse_entry checked num doesn't exceed max strlen
 * 			otherwise there gonna be str overflow
 * RIGHTS: N/A
 * BUFOVF: strlen checked by caller to guarantee that there's no overflow for __strncmp.
 * LOG: N/A
 * RET: N/A
 * RACING: N/A
 * LEAK: N/A
 * ARITHOVF: N/A
 * ---------------------------------------------------------------
 */
#ifndef __KERNEL__
static __pure int __strncmp(const char *str1, const char *str2, unsigned long len)
{
	if (str1 == NULL || str2 == NULL || len == 0U) {
		return 0;
	}

	unsigned long i;

	for (i = 0; i < len; i++) {
		if (str1[i] != str2[i]) {
			return (int)str1[i] - (int)str2[i];
		}
		if (str1[i] == '\0') {
			return 0;
		}
	}

	return 0;
}
#else
#define __strncmp strncmp
#endif

/*
 * Parse the header of the given CPIO entry
 *
 * @archive: The location of the CPIO archive
 * @cpioend: The end location of the CPIO archive
 * @fileinfo: The struct to store CPIO file information
 * @nextfile: Pointer to next CPIO entry
 *
 * Return 0 if success, -1 if the header is not valid, 1 if EOF.
 * ---------------------------------------------------------------
 * CODEREVIEW CHECKLIST
 * ARG: archive: usr input checked
 *		cpioend: usr input, checked if the cpio header is valid.
 *		filename/filesize/filedata in fileinfo: usr input, used to store parsed
 * 				cpio info to usr given variable, if hard coded to NULL,
 * 				it means that usr doesn't care about such info.
 * 				checked before usage.
 * RIGHTS: N/A
 * BUFOVF: str_len checked to guarantee that there's no overflow for __strncmp.
 * LOG: N/A
 * RET: __strncmp/do_client_call checked
 * RACING: N/A
 * LEAK: N/A
 * ARITHOVF: N/A
 * ---------------------------------------------------------------
 */
int cpio_parse_entry(struct cpio_header *archive,
		     void *cpioend,
		     struct cpio_fileinfo_s *fileinfo,
		     struct cpio_header **nextfile)
{
	unsigned long namelen = 0UL;
	unsigned long headsize = 0UL;
	unsigned long str_len = 0UL;
	unsigned long file = 0UL;
	unsigned long next = 0UL;
	const char *name = NULL;

	if (archive == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	/* Cpio header can't over cpioend limit. */
	name = (char *)archive + sizeof(struct cpio_header);
	if (ptr_to_ulong(name) > ptr_to_ulong(cpioend)) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	if (__strncmp(archive->c_magic, CPIO_HEADER_MAGIC, CPIO_HEADER_MAGIC_LEN) != 0) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	/* Cpio file name can't over cpioend limit. */
	namelen = hex_to_ulong(archive->c_namesize, (unsigned int)sizeof(archive->c_namesize));
	if (((~0UL) - namelen) < (ptr_to_ulong(name) + (CPIO_ALIGNMENT - 1UL)))  {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}
	file = ALIGN_UP(ptr_to_ulong(name) + namelen, CPIO_ALIGNMENT);
	if (file > ptr_to_ulong(cpioend)) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	/* File name should end with char 0. */
	if ((namelen == 0UL) || (name[namelen - 1u] != '\0')) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	/* Reach EOF */
	str_len = (namelen > CPIO_FOOTER_MAGIC_LEN) ? CPIO_FOOTER_MAGIC_LEN : namelen;

	if (__strncmp(name, CPIO_FOOTER_MAGIC, str_len) == 0) {
		__hmattr_sideff_resolved("no relative side effect");
		return 1;
	}

	/* Cpio file data can't over cpioend limit. */
	headsize = hex_to_ulong(archive->c_filesize, (unsigned int)sizeof(archive->c_filesize));
	if (((~0UL) - headsize) < (file + (CPIO_ALIGNMENT - 1UL)))  {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}
	next = ALIGN_UP(file + headsize, CPIO_ALIGNMENT);
	if (next > ptr_to_ulong(cpioend)) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}
	*nextfile = ulong_to_ptr(next, struct cpio_header);

	if (fileinfo != NULL) {
		fileinfo->filename = name;
		fileinfo->filesize = headsize;
		fileinfo->filedata = ulong_to_ptr(file, void);
	}

	return 0;
}