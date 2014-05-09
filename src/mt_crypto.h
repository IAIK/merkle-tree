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

//----------------------------------------------------------------------
mt_error_t mt_hash(const uint8_t left[HASH_LENGTH],
    const uint8_t right[HASH_LENGTH], uint8_t message_digest[HASH_LENGTH]);

#endif /* MT_CRYPTO_H_ */
