/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Id generator management
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 06 15:16:18 2019
 */
#ifndef ULIBS_LIBALGO_ID_GENERATOR_H
#define ULIBS_LIBALGO_ID_GENERATOR_H

#include <hongmeng/types.h>
#include <hongmeng/errno.h>

struct id_generator_attr {
	unsigned int min_index;
	unsigned int max_index;
};

struct id_generator;

/*
 * func: id_generator_create
 *
 * create an index generator according to generator attributes
 * @attr: the attribute of index generator
 * @id_generator: the point of index generator point
 *
 */
int id_generator_create(const struct id_generator_attr *attr,
			struct id_generator **id_generator);

/*
 * func: id_generator_alloc
 *
 * get an unusing index from target index generator
 * @id_generator: target index generator point
 * @id: unusing index
 *
 */
int id_generator_alloc(struct id_generator *id_generator,
		       unsigned int *id);

/*
 * func: id_generator_free
 *
 * free an unusing index into target index generator
 * @id_generator: index generator point
 * @id: waiting free index
 *
 */
void id_generator_free(struct id_generator *id_generator,
		       unsigned int id);

/*
 * func: id_generator_destroy
 *
 * destroy target index generator
 * @id_generator: index generator point
 *
 */
void id_generator_destroy(struct id_generator *id_generator);

void struct_id_generator_free(void *p);
void *struct_id_generator_malloc(void);
size_t struct_id_generator_size(void);
#endif
