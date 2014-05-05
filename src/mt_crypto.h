/*
 * mt_crypto.h
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifndef MT_CRYPTO_H_
#define MT_CRYPTO_H_

#include "config.h"

void hash_reset(void *ctx);
void hash_input(void *ctx, uint8_t const * const msg, size_t len);
void hash_result(void *ctx, uint8_t md[D_HASH_LENGTH]);

#endif /* MT_CRYPTO_H_ */
