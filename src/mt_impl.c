/*
 * mt-data.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */
#include "merkletree.h"
#include "mt_crypto.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------
mt_t *mt_create(void)
{
  mt_t *mt = calloc(1, sizeof(mt_t));
  if (!mt) {
    return NULL;
  }
  for (uint32_t i = 0; i < TREE_LEVELS; ++i) {
    mt_al_t *tmp = mt_al_create();
    if (!tmp) {
      for (uint32_t j = 0; j < i; ++j) {
        mt_al_delete(mt->level[j]);
      }
      free(mt);
      return NULL;
    }
    mt->level[i] = tmp;
  }
  return mt;
}

//----------------------------------------------------------------------
void mt_delete(mt_t *mt)
{
  for (uint32_t i = 0; i < TREE_LEVELS; ++i) {
    mt_al_delete(mt->level[i]);
  }
  free(mt);
}

//----------------------------------------------------------------------
mt_error_t mt_add(mt_t *mt, const uint8_t *tag, const size_t len)
{
  if (!(mt && tag && len <= HASH_LENGTH)) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  mt_error_t err = MT_ERR_UNSPECIFIED;
  uint8_t message_digest[HASH_LENGTH];
  // TODO do this more efficiently!
  memset(message_digest, 0, HASH_LENGTH);
  memcpy(message_digest, tag, len);
  err = mt_al_add(mt->level[0], message_digest);
  if (err != MT_SUCCESS) {
    return err;
  }
  mt->elems += 1;
  if (mt->elems == 1) {
    return MT_SUCCESS;
  }
  uint32_t q = mt->elems - 1;
  uint32_t l = 0;         // level
  while (q > 0 && l < TREE_LEVELS) {
    if ((q & 1) != 0) {
      uint8_t const * const left = mt_al_get(mt->level[l], q - 1);
      err = mt_hash(left, message_digest, message_digest);
      if (err != MT_SUCCESS) {
        return err;
      }
      err = mt_al_add_or_update(mt->level[l + 1], message_digest, (q >> 1));
      if (err != MT_SUCCESS) {
        return err;
      }
    }
    q >>= 1;
    l += 1;
  }
  return MT_SUCCESS;
}

//----------------------------------------------------------------------
uint32_t mt_get_size(const mt_t *mt)
{
  assert(mt);
  return mt_al_get_size(mt->level[0]);
}

//----------------------------------------------------------------------
int mt_exists(mt_t *mt, const uint32_t offset)
{
  if (!mt || offset > MT_AL_MAX_ELEMS) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  return (mt_al_get(mt->level[0], offset) != NULL);
}

//----------------------------------------------------------------------
static uint32_t hasNextLevelExceptRoot(mt_t const * const mt, uint32_t cur_lvl)
{
  if (!mt) {
    return 0;
  }
  return (cur_lvl + 1 < TREE_LEVELS - 1)
      & (mt_al_get_size(mt->level[(cur_lvl + 1)]) > 0);
}

//----------------------------------------------------------------------
static const uint8_t *findRightNeighbor(const mt_t *mt, uint32_t offset,
    int32_t l)
{
  if (!mt) {
    return NULL;
  }
  do {
    if (offset < mt_al_get_size(mt->level[l])) {
      return mt_al_get(mt->level[l], offset);
    }
    l -= 1;
    offset <<= 1;
  } while (l > -1);
  // This can happen, if there is no neighbor.
  return NULL;
}

//----------------------------------------------------------------------
mt_error_t mt_verify(const mt_t *mt, const uint8_t *tag, const size_t len,
    const uint32_t offset)
{
  if (!(mt && tag && len <= HASH_LENGTH && (offset < mt->elems))) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  mt_error_t err = MT_ERR_UNSPECIFIED;
  uint8_t message_digest[HASH_LENGTH];
  // TODO do this more efficiently!
  memset(message_digest, 0, HASH_LENGTH);
  memcpy(message_digest, tag, len);
  uint32_t q = offset;
  uint32_t l = 0;         // level
  while (hasNextLevelExceptRoot(mt, l)) {
    if (!(q & 0x01)) { // left subtree
      // If I am the left neighbor (even index), we need to check if a right
      // neighbor exists.
      const uint8_t *right;
      if ((right = findRightNeighbor(mt, q + 1, l)) != NULL) {
        err = mt_hash(message_digest, right, message_digest);
        if (err != MT_SUCCESS) {
          return err;
        }
      }
    } else {           // right subtree
      // In the right subtree, there must always be a left neighbor!
      uint8_t const * const left = mt_al_get(mt->level[l], q - 1);
      err = mt_hash(left, message_digest, message_digest);
      if (err != MT_SUCCESS) {
        return err;
      }
    }
    q >>= 1;
    l += 1;
  }
  mt_print_hash(message_digest);
  int r = memcmp(message_digest, mt_al_get(mt->level[l], q), HASH_LENGTH);
  if (r) {
    return MT_ERR_ROOT_MISMATCH;
  } else {
    return MT_SUCCESS;
  }
}

//----------------------------------------------------------------------
mt_error_t mt_update(const mt_t *mt, const uint8_t *tag, const size_t len,
    const uint32_t offset)
{
  if (!(mt && tag && len <= HASH_LENGTH && (offset < mt->elems))) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  mt_error_t err = MT_ERR_UNSPECIFIED;
  uint8_t message_digest[HASH_LENGTH];
  // TODO Do this more efficiently
  memset(message_digest, 0, HASH_LENGTH);
  memcpy(message_digest, tag, len);
  err = mt_al_update(mt->level[0], message_digest, offset);
  if (err != MT_SUCCESS) {
    return err;
  }
  uint32_t q = offset;
  uint32_t l = 0;         // level
  while (hasNextLevelExceptRoot(mt, l)) {
    if (!(q & 0x01)) { // left subtree
      // If I am the left neighbor (even index), we need to check if a right
      // neighbor exists.
      const uint8_t *right;
      if ((right = findRightNeighbor(mt, q + 1, l)) != NULL) {
        err = mt_hash(message_digest, right, message_digest);
        if (err != MT_SUCCESS) {
          return err;
        }
      }
    } else {           // right subtree
      // In the right subtree, there must always be a left neighbor!
      uint8_t const * const left = mt_al_get(mt->level[l], q - 1);
      err = mt_hash(left, message_digest, message_digest);
      if (err != MT_SUCCESS) {
        return err;
      }
    }
    q >>= 1;
    l += 1;
    err = mt_al_update(mt->level[l], message_digest, q);
    if (err != MT_SUCCESS) {
      return err;
    }
  }
  return MT_SUCCESS;
}

//----------------------------------------------------------------------
mt_error_t mt_get_root(mt_t *mt, mt_hash_t *root)
{
  if (!(mt && root)) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  uint32_t l = 0;         // level
  while (hasNextLevelExceptRoot(mt, l)) {
    l += 1;
  }
  memcpy(root, mt_al_get(mt->level[l], 0), sizeof(mt_hash_t));
  return MT_SUCCESS;
}

//----------------------------------------------------------------------
void mt_print_hash(const mt_hash_t hash)
{
  if (!hash) {
    printf("[ERROR][mt_print_hash]: Hash NULL");
  }
  mt_al_print_hex_buffer(hash, HASH_LENGTH);
  printf("\n");
}

//----------------------------------------------------------------------
void mt_print(const mt_t *mt)
{
  if (!mt) {
    printf("[ERROR][mt_print]: Merkle Tree NULL");
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
