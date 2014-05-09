/*
 * config.h
 *
 * To be auto generated later on.
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#define HASH_LENGTH                      32u  /*!< The length of the hash function output in bytes */
#define TREE_LEVELS                      20u  /*!< The number of levels in the tree */
#define BLOCK_SIZE                     4096u  /*!< The size of a single block in the file */
#define MAX_FILE_SIZE   UINT32_C(2147483647)  /*!< The maximum size of a file */
#define MT_AL_MAX_ELEMS (MAX_FILE_SIZE / BLOCK_SIZE) /*!< The maximum number of elements in a Merkle Tree array list. Essential for integer overflow protection! */

/*!
 * Hash data type.
 * TODO: Johannes, is it ok to put this into config.h?
 */
typedef uint8_t mt_hash_t[HASH_LENGTH];


#endif /* CONFIG_H_ */
