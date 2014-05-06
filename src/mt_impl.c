/*
 * mt-data.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sha.h"

#include "merkletree.h"
#include "mt_crypto.h"

//static const char LogTable256[256] =
//    {
//#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
//        -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, LT(4), LT(5), LT(5),
//        LT(6), LT(6), LT(6), LT(6), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7),
//        LT(7), LT(7) };
//
////----------------------------------------------------------------------
//static uint32_t mt_log2(uint32_t v) {
//  uint32_t r;                  // r will be lg(v)
//  register unsigned int t, tt; // temporaries
//
//  if ((tt = v >> 16)) {
//    r = ((t = tt >> 8)) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
//  } else {
//    r = ((t = v >> 8)) ? 8 + LogTable256[t] : LogTable256[v];
//  }
//  return r;
//}

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
void mt_delete(mt_t *mt) {
  for (uint32_t i = 0; i < TREE_LEVELS; ++i) {
    free(mt->level[i]);
  }
  free(mt);
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
  mt_al_add(mt->level[0], hash);
  mt->elems += 1;
  if (mt->elems == 1) {
    return;
  }
  SHA256Context ctx;
  if (SHA256Reset(&ctx) != shaSuccess) {
    // TODO Error code handling
    return;
  }
  uint8_t message_digest[D_HASH_LENGTH];
  memcpy(message_digest, hash, HASH_LENGTH);
  uint32_t q = offset;    // quotient
  uint32_t l = 0;         // level
  while (q > 0) {
    if ((q & 1) != 0) {
      // First add the right neighbor (the new one)
      if (SHA256Input(&ctx, message_digest, HASH_LENGTH) != shaSuccess) {
        // TODO Error code handling
        return;
      }
      // Concat left neighbor (the old one)
      uint8_t const * const left_neighbor = mt_al_get(mt->level[l], q - 1);
      if (SHA256Input(&ctx, left_neighbor, HASH_LENGTH) != shaSuccess) {
        // TODO Error code handling
        return;
      }
      if (SHA256Result(&ctx, message_digest) != shaSuccess) {
        // TODO Error code handling
        return;
      }
      mt_al_add(mt->level[l + 1], message_digest);
    } else {
      // TODO
    }
    q >>= 1;
    l += 1;
  }
}

//----------------------------------------------------------------------
void mt_print(mt_t *mt) {
  if (!mt) {
    // TODO Error Code Handling
    return;
  }
  for (uint32_t i = 0; i < TREE_LEVELS; ++i) {
    if (mt->level[i]->elems == 0) {
      return;
    }
    printf(
        "==================== Merkle Tree level[%02d]: ====================\n",
        i);
    mt_al_print(mt->level[i]);
  }
}
