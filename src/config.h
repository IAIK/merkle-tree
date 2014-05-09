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
#define TREE_LEVELS                      19u  /*!< The number of levels in the tree minus one, because we don't count the root node */
#define BLOCK_SIZE                     4096u  /*!< Block size */
#define MAX_FILE_SIZE   UINT32_C(2147483647)
#define MT_AL_MAX_ELEMS (MAX_FILE_SIZE / BLOCK_SIZE) /*!< The maximum number of elements in a Merkle Tree array list. Essential for integer overflow protection! */

#endif /* CONFIG_H_ */
