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
static void mt_hash(const uint8_t left[D_HASH_LENGTH],
    const uint8_t right[D_HASH_LENGTH], uint8_t message_digest[D_HASH_LENGTH]) {
  // TODO Error Handling
  SHA256Context ctx;
  SHA256Reset(&ctx);
  SHA256Input(&ctx, left, HASH_LENGTH);
  SHA256Input(&ctx, right, HASH_LENGTH);
  SHA256Result(&ctx, message_digest);
}

//----------------------------------------------------------------------
mt_t *mt_create(void) {
  mt_t *mt = calloc(1, sizeof(mt_t));
  if (!mt) {
    // TODO Error code handling?
    return NULL;
  }
  for (int32_t i = 0; i < TREE_LEVELS; ++i) {
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
  for (int32_t i = 0; i < TREE_LEVELS; ++i) {
    free(mt->level[i]);
  }
  free(mt);
}

//----------------------------------------------------------------------
void mt_add(mt_t * const mt, const uint8_t hash[D_HASH_LENGTH],
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
  uint8_t message_digest[D_HASH_LENGTH];
  memcpy(message_digest, hash, HASH_LENGTH);
  uint32_t q = offset;
  uint32_t l = 0;         // level
  while (q > 0) {
    if ((q & 1) != 0) {
      uint8_t const * const left = mt_al_get(mt->level[l], q - 1);
      mt_hash(left, message_digest, message_digest);
      mt_al_add_or_update(mt->level[l + 1], message_digest, (q >> 1));
    } else {
      // TODO
    }
    q >>= 1;
    l += 1;
  }
}

//----------------------------------------------------------------------
static uint32_t hasNextLevelExceptRoot(mt_t const * const mt, uint32_t cur_lvl) {
  if (!mt) {
    return 0;
  }
  // TODO fix number of levels, we need ot count root!
  return (cur_lvl + 1 < TREE_LEVELS - 1)
      & (getSize(mt->level[(cur_lvl + 1)]) > 0);
}

//----------------------------------------------------------------------
static const uint8_t *findRightNeighbor(const mt_t *mt, uint32_t offset,
    int32_t l) {
  if (!mt) {
    // TODO Error required?
    return NULL;
  }
  do {
    if (offset < getSize(mt->level[l])) {
      return mt_al_get(mt->level[l], offset);
    }
    l -= 1;
    offset <<= 1;
  } while (l > -1);
  // This can happen, if there is no neighbor.
  return NULL;
}

//----------------------------------------------------------------------
void mt_verify(mt_t * const mt, const uint8_t hash[D_HASH_LENGTH],
    const uint32_t offset) {
  if (!mt) {
    // TODO Error handling
    return;
  }
  if (!hash) {
    // TODO Error handling
    return;
  }
  if (offset >= mt->elems) {
    // TODO Error handling
    return;
  }
  uint8_t message_digest[D_HASH_LENGTH];
  memcpy(message_digest, hash, HASH_LENGTH);
  uint32_t q = offset;
  uint32_t l = 0;         // level
  while (hasNextLevelExceptRoot(mt, l)) {
    if (!(q & 0x01)) { // left subtree
      // If I am the left neighbor (even index), we need to check if a right
      // neighbor exists.
      const uint8_t *right;
      if ((right = findRightNeighbor(mt, q + 1, l)) != NULL) {
        mt_hash(message_digest, right, message_digest);
      }
    } else {           // right subtree
      // In the right subtree, there must always be a left neighbor!
      uint8_t const * const left = mt_al_get(mt->level[l], q - 1);
      mt_hash(left, message_digest, message_digest);
    }
    q >>= 1;
    l += 1;
  }
  mt_print_hash(message_digest);
}

void mt_update(mt_t * const mt, const uint8_t hash[D_HASH_LENGTH],
    const uint32_t offset) {
  if (!mt) {
    // TODO Error handling
    return;
  }
  if (!hash) {
    // TODO Error handling
    return;
  }
  if (offset >= mt->elems) {
    // TODO Error handling
    return;
  }
  mt_al_update(mt->level[0], hash, offset);
  uint8_t message_digest[D_HASH_LENGTH];
  memcpy(message_digest, hash, HASH_LENGTH);
  uint32_t q = offset;
  uint32_t l = 0;         // level
  while (hasNextLevelExceptRoot(mt, l)) {
    if (!(q & 0x01)) { // left subtree
      // If I am the left neighbor (even index), we need to check if a right
      // neighbor exists.
      const uint8_t *right;
      if ((right = findRightNeighbor(mt, q + 1, l)) != NULL) {
        mt_hash(message_digest, right, message_digest);
      }
    } else {           // right subtree
      // In the right subtree, there must always be a left neighbor!
      uint8_t const * const left = mt_al_get(mt->level[l], q - 1);
      mt_hash(left, message_digest, message_digest);
    }
    q >>= 1;
    l += 1;
    mt_al_update(mt->level[l], message_digest, q);
  }
}

//----------------------------------------------------------------------
void mt_print(mt_t *mt) {
  if (!mt) {
    // TODO Error Code Handling
    return;
  }
  for (int32_t i = 0; i < TREE_LEVELS; ++i) {
    if (mt->level[i]->elems == 0) {
      return;
    }
    printf(
        "==================== Merkle Tree level[%02d]: ====================\n",
        i);
    mt_al_print(mt->level[i]);
  }
}
