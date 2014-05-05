/*
 * mt-data.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */
#include <stdlib.h>

#include "sha.h"

#include "merkletree.h"
#include "mt_crypto.h"

//----------------------------------------------------------------------
mt_t *mt_create(void) {
  mt_t *mt = calloc(1, sizeof(mt_t));
  if (!mt) {
    // TODO Error code handling?
    return NULL;
  }
  for (uint32_t i = 0; i < TREE_LEVELS; ++i) {
    mt_al_t *tmp = mt_al_create();
    if (!tmp) {
      for (uint32_t j = 0; j < i; ++j) {
        free(mt->level[j]);
      }
      free(mt);
      // TODO Error code handling?
      return NULL;
    }
    mt->level[i] = tmp;
  }
  return mt;
}

//----------------------------------------------------------------------
void mt_add_block(mt_t * const mt, const uint8_t hash[D_HASH_LENGTH],
    const uint32_t offset) {
  if (!mt) {
    // TODO Error handling
    return;
  }
  if (!hash) {
    // TODO Error handling
    return;
  }
  if (offset != mt->elems) {
    // TODO Error handling
    return;
  }
  if (mt->elems == 0) {
    mt_al_add(mt->level[0], hash);
    mt->elems += 1;
  }
  SHA256Context ctx;
  if (SHA256Reset(&ctx) != shaSuccess) {
    // TODO Error code handling
    return;
  }
  mt_al_add(mt->level[0], hash);
  uint8_t message_digest[D_HASH_LENGTH];
  uint32_t q = mt->elems + 1; // quotient
  uint32_t r = 0;             // remainder
  uint32_t l = 1;             // level
  while (q != 1) {
    r = q & 1;
    q >>= 2;
    if (r != 0) {
      // TODO Get left neighbor
      if (SHA256Input(&ctx, NULL, HASH_LENGTH) != shaSuccess) {
        // TODO Error code handling
        return;
      }
      // Concat with right neighbor (the new one)
      if (SHA256Input(&ctx, hash, HASH_LENGTH) != shaSuccess) {
        // TODO Error code handling
        return;
      }
      if (SHA256Result(&ctx, message_digest) != shaSuccess) {
        // TODO Error code handling
        return;
      }
      mt_al_add(mt->level[l],message_digest);
    }
  }
}
