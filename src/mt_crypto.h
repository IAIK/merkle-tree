/*
 * mt_crypto.h
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifndef MT_CRYPTO_H_
#define MT_CRYPTO_H_

#include "config.h"
#include "mt_err.h"

/*!
 * \brief Compute the hash of the left input concatenated with the right input.
 *
 * This function computes the following: h(left||right),
 * where h is the hash function, left is the left subtree root hash, right is
 * the right subtree root hash, and || is the concatination operation.
 *
 * @param left[in] the root hash of the left subtree
 * @param right[in] the root hash of the right subtree
 * @param message_digest[out] the result of h(left||right)
 * @return MT_SUCCESS if computing the hash was successful;
 *         MT_ERR_ILLEGAL_PARAM if any of the incoming parameters is null;
 *         MT_ERR_ILLEGAL_STATE if the underlying hash function reports an
 *         error.
 */
mt_error_t mt_hash(const mt_hash_t left, const mt_hash_t right,
    mt_hash_t message_digest);

#endif /* MT_CRYPTO_H_ */
