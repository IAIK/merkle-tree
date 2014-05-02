/*
 * mt_arr_list.h
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifndef MT_ARR_LIST_H_
#define MT_ARR_LIST_H_

#include "config.h"

typedef struct merkle_tree_array_list {
  uint32_t elems;
  uint8_t *store;
} mt_al_t;

mt_al_t *mt_al_create();

void mt_al_delete(mt_al_t *mt_al);

void mt_al_add(mt_al_t *mt_al, uint8_t data[D_HASH_LENGTH]);

void mt_al_truncate(mt_al_t *mt_al, uint32_t elems);

void mt_al_print(mt_al_t *mt_al);

#endif /* MT_ARR_LIST_H_ */
