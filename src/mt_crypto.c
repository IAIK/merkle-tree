/*!
 * \file
 * \brief Implements the Merkle Tree hash interface using SHA-256 the hash
 * function.
 */

#include "sha.h"

#include "mt_crypto.h"

//----------------------------------------------------------------------
mt_error_t mt_hash(const mt_hash_t left, const mt_hash_t right,
    mt_hash_t message_digest) {
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

