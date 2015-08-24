/*
 * config.h
 *
 * To be auto generated later on.
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifndef MT_CONFIG_H_
#define MT_CONFIG_H_

#include <stdint.h>

#define HASH_LENGTH                      32u  /*!< The length of the hash function output in bytes */
#define TREE_LEVELS                      20u  /*!< The number of levels in the tree */
#define MT_AL_MAX_ELEMS              524288u  /*!< The maximum number of elements in a Merkle Tree array list. Essential for integer overflow protection! */

/*!
 * Hash data type.
 */
typedef uint8_t mt_hash_t[HASH_LENGTH];


#endif /* MT_CONFIG_H_ */
