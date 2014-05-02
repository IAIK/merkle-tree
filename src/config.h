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

#define D_HASH_LENGTH 32 /*!< The length of the hash function output in bytes */
#define D_TREE_LEVELS 19 /*!< The number of levels in the tree minus one, because we don't count the root node */

static const uint32_t BLOCK_SIZE    = 4096; /*!< Block size */
static const uint32_t MAX_FILE_SIZE = 2147483647;
static const uint32_t TREE_LEVELS   = D_TREE_LEVELS;

static const uint32_t HASH_LENGTH = D_HASH_LENGTH; /*!< The length of the hash function output in bytes */

#endif /* CONFIG_H_ */
