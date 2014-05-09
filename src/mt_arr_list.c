/*
 * mt_arr_list.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */
#include "mt_arr_list.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*!
 * \brief Computes the next highest power of two
 *
 * This nice little algorithm is taken from
 * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
static uint32_t round_next_power_two(uint32_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  v += (v == 0); // handle v == 0 edge case
  return v;
}

//----------------------------------------------------------------------
static int is_power_of_two(uint32_t v) {
  return (v != 0) && ((v & (v - 1)) == 0);
}

//----------------------------------------------------------------------
mt_al_t *mt_al_create() {
  return calloc(1, sizeof(mt_al_t));
}

//----------------------------------------------------------------------
void mt_al_delete(mt_al_t *mt_al) {
  free(mt_al->store);
  free(mt_al);
}

//----------------------------------------------------------------------
mt_error_t mt_al_add(mt_al_t *mt_al, const uint8_t data[HASH_LENGTH]) {
  if (!(mt_al && data)) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  if (mt_al->elems == 0) {
    // Add first element
    mt_al->store = malloc(HASH_LENGTH);
    if (!mt_al->store) {
      return MT_ERR_OUT_Of_MEMORY;
    }
  } else if (is_power_of_two(mt_al->elems)) {
    // Need more memory
    // Prevent integer overflow during size calculation
    if (((mt_al->elems << 1) < mt_al->elems)
        || (mt_al->elems << 1 > MT_AL_MAX_ELEMS)) {
      return MT_ERR_ILLEGAL_STATE;
    }
    size_t alloc = mt_al->elems * 2 * HASH_LENGTH;
    uint8_t *tmp = realloc(mt_al->store, alloc);
    if (!tmp) {
      return MT_ERR_OUT_Of_MEMORY;
    }
//    fprintf(stderr, "Allocated memory: %x, Old: %p, New: %p\n", alloc / HASH_LENGTH,
//        mt_al->store, tmp);
    mt_al->store = tmp;
  }
  memcpy(&mt_al->store[mt_al->elems * HASH_LENGTH], data, HASH_LENGTH);
  mt_al->elems += 1;
  return MT_SUCCESS;
}

//----------------------------------------------------------------------
mt_error_t mt_al_update(const mt_al_t *mt_al, const uint8_t data[HASH_LENGTH],
    const uint32_t offset) {
  if (!(mt_al && data && offset < mt_al->elems)) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  memcpy(&mt_al->store[offset * HASH_LENGTH], data, HASH_LENGTH);
  return MT_SUCCESS;
}

//----------------------------------------------------------------------
mt_error_t mt_al_add_or_update(mt_al_t *mt_al, const uint8_t data[HASH_LENGTH],
    const uint32_t offset) {
  if (!(mt_al && data) || offset > mt_al->elems) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  if (offset == mt_al->elems) {
    return mt_al_add(mt_al, data);
  } else {
    return mt_al_update(mt_al, data, offset);
  }
}

//----------------------------------------------------------------------
mt_error_t mt_al_truncate(mt_al_t *mt_al, const uint32_t elems) {
  if (!(mt_al && elems < mt_al->elems)) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  mt_al->elems = elems;
  if (elems == 0) {
    free(mt_al->store);
    return MT_SUCCESS;
  }
  uint32_t alloc = round_next_power_two(elems) * HASH_LENGTH;
  uint8_t *tmp = realloc(mt_al->store, alloc);
  if (!tmp) {
    return MT_ERR_OUT_Of_MEMORY;
  }
//  fprintf(stderr, "Allocated memory: %x, Old: %p, New: %p\n",
//      alloc / HASH_LENGTH, mt_al->store, tmp);
  mt_al->store = tmp;
  return MT_SUCCESS;
}

//----------------------------------------------------------------------
const uint8_t *mt_al_get(const mt_al_t *mt_al, const uint32_t offset) {
  if (!(mt_al && offset < mt_al->elems)) {
    return NULL;
  }
  return &mt_al->store[offset * HASH_LENGTH];
}

//----------------------------------------------------------------------
void mt_al_print_hex_buffer(const uint8_t *buffer,
    const uint32_t size) {
  if (!buffer) {
    printf("[ERROR][mt_al_print_hex_buffer]: Merkle Tree array list is NULL");
    return;
  }
  for (uint32_t i = 0; i < size; ++i) {
    printf("%02X", buffer[i]);
  }
}

//----------------------------------------------------------------------
void mt_al_print(const mt_al_t *mt_al) {
  if (!mt_al) {
    printf("[ERROR][mt_al_print]: Merkle Tree array list is NULL");
    return;
  }
  printf("[%08X\n", mt_al->elems);
  for (uint32_t i = 0; i < mt_al->elems; ++i) {
    mt_al_print_hex_buffer(&mt_al->store[i * HASH_LENGTH], HASH_LENGTH);
    printf("\n");
  }
  printf("]\n");
}
