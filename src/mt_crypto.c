/*
 * mt_crypto.c
 *
 *  Created on: 09.05.2014
 *      Author: dhein
 */

#include "sha.h"

#include "mt_crypto.h"

//----------------------------------------------------------------------
mt_error_t mt_hash(const uint8_t left[HASH_LENGTH],
    const uint8_t right[HASH_LENGTH], uint8_t message_digest[HASH_LENGTH]) {
  if (!(left && right && message_digest)) {
    return MT_ERR_ILLEGAL_PARAM;
  }
  SHA256Context ctx;
  if (SHA256Reset(&ctx) != shaSuccess) {
    return MT_ERR_ILLEGAL_STATE;
  }
  if (SHA256Input(&ctx, left, HASH_LENGTH) != shaSuccess) {
    return MT_ERR_ILLEGAL_STATE;
  }
  if (SHA256Input(&ctx, right, HASH_LENGTH) != shaSuccess) {
    return MT_ERR_ILLEGAL_STATE;
  }
  if (SHA256Result(&ctx, message_digest) != shaSuccess) {
    return MT_ERR_ILLEGAL_STATE;
  }
  return MT_SUCCESS;
}

