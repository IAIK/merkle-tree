/*
 * MerkleTree.h
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MERKLETREE_H_
#define MERKLETREE_H_

#include <stdlib.h>

#include "config.h"
#include "mt_arr_list.h"

/*!
 * \brief defines the Merkle Tree data type
 *
 * A Merkle Tree is used for ...
 */
typedef struct merkle_tree {
  uint32_t elems;
  mt_al_t *level[D_TREE_LEVELS];
} mt_t;

/*!
 * \brief creates a new instance of the Merkle Tree data type
 *
 * This function tries to create a new Merkle Tree data type for ...
 */
mt_t *mt_create(void);
/*!
 *
 * \brief deletes the specified Merkle Tree instance
 *
 * \param[in] mt a pointer to the merkle tree instance to delete
 */
void mt_delete(mt_t *mt);

void mt_add(mt_t * const mt, const uint8_t mac[D_HASH_LENGTH], const uint32_t offset);

void mt_update(mt_t * const mt, const uint8_t mac[D_HASH_LENGTH], const uint32_t offset);

void mt_verify(mt_t * const mt, const uint8_t mac[D_HASH_LENGTH], const uint32_t offset);

void mt_truncate(mt_t *mt, uint32_t last_valid);

/*!
 * Print a human readable representation of the Merkle Tree
 * @param mt a pointer to the Merkle Tree data type instance to print
 */
void mt_print(mt_t *mt);

#endif /* MERKLETREE_H_ */
#ifdef __cplusplus
}
#endif
