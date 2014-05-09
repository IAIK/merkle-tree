/*
 * mt_arr_list.h
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#ifndef MT_ARR_LIST_H_
#define MT_ARR_LIST_H_

#include <stdio.h>

#include "config.h"
#include "mt_err.h"

/*!
 * \brief A resizable array list for hash values
 *
 * The Merkle Tree array list data structure is a simple, resizable array
 * list. It allows to add new elements to the end of the list, truncating
 * the list, and read and write access to existing elements. Finally, the
 * list is able to print itself to standard out.
 *
 * The list uses a simple memory allocation algorithm. Whenever the number of
 * elements reaches a power of two + 1, enough space to hold the next power
 * of two elements is allocated. So for example, if 4 elements were already
 * allocated, and a 5th is to be added, enough memory for 8 elements is
 * allocated.
 */
typedef struct merkle_tree_array_list {
  uint32_t elems; /*!< number of elements in the list */
  uint8_t *store; /*!< pointer to the address of the first element in the list */
} mt_al_t;

/*!
 * \brief Creates a new Merkle Tree array list instance.
 *
 * @return a pointer to the freshly created Merkle Tree array list instance.
 */
mt_al_t *mt_al_create(void);

/*!
 * \brief Deletes an existing Merkle Tree array list instance.
 *
 * @param[in] mt_al
 */
void mt_al_delete(mt_al_t *mt_al);

/*!
 * \brief Adds a new element to the list.
 *
 * @param mt_al[in] the Merkle Tree array list data type instance
 * @param data[in] the hash to add to the array list
 * @return MT_SUCCESS if adding the element is successful;
 *         MT_ERR_ILLEGAL_PARAM if any of the incoming parameters is null;
 *         MT_ERR_OUT_OF_MEMORY if the array list cannot allocate any more space to grow;
 *         MT_ERR_ILLEGAL_STATE if an integer overflow in the allocation code occurs.
 */
mt_error_t mt_al_add(mt_al_t *mt_al, const uint8_t data[HASH_LENGTH]);

/*!
 * \brief Update a specific hash value with the given new value
 *
 * @param mt_al[in,out] the Merkle Tree array list data type instance
 * @param data[in] the new hash value
 * @param offset[in] the index/offset of the hash value to update
 */
mt_error_t mt_al_update(const mt_al_t *mt_al, const uint8_t data[HASH_LENGTH],
    const uint32_t offset);

/*!
 * \brief Either updates the last element in the list, or adds a new element
 *
 * This is a restricted add or update function. It can either add a new
 * element if offset equals the index of the last element plus one, or update
 * the last element in the list if offset equals the index of the last
 * element in the list. This is a useful function for appending data to the
 * Merkle tree. The checks on the index work as a sanity check.
 *
 * @param mt_al[in,out] the Merkle Tree array list data type instance
 * @param data[in] the new hash value to either add or update
 * @param offset[in] the index/offset of the hash value to update. This value
 *   must either point to the last element of the list or to the first new
 *   index.
 */
mt_error_t mt_al_add_or_update(mt_al_t *mt_al, const uint8_t data[HASH_LENGTH],
    const uint32_t offset);

/*!
 * \brief Truncates the list of hash values to the given number of elements
 *
 * @param mt_al[in] the Merkle Tree array list data type instance
 * @param elems the number of elements to truncate the array list to
 */
mt_error_t mt_al_truncate(mt_al_t *mt_al, const uint32_t elems);

/*!
 * \brief Return a hash from the hash array list
 *
 * @param mt_al[in] the Merkle Tree array list data type instance
 * @param offset[in] the offset of the element to fetch
 * @return a pointer to the requested hash element in the array list
 */
const uint8_t * mt_al_get(const mt_al_t *mt_al, const uint32_t offset);

/*!
 * \brief Checks if the element at the given offset has a right neighbor
 *
 * @param mt_al[in] the Merkle Tree array list data type instance
 * @param offset[in] the offset of the element for which to look for a
 * neighbor
 * @return true if the element at the given offset has a neighbor.
 */
static inline uint32_t hasRightNeighbor(const mt_al_t *mt_al,
    const uint32_t offset) {
  if (!mt_al) {
    return 0;
  }
  return (offset + 1) < mt_al->elems;
}

/*!
 * \brief Returns the number of elements in the list
 *
 * @param mt_al[in] the Merkle Tree array list data type instance
 * @return the number of elements in the list
 */
static inline uint32_t getSize(const mt_al_t *mt_al) {
  if (!mt_al) {
    return 0;
  }
  return mt_al->elems;
}

/*!
 * \brief Print a given buffer as hex formated string
 *
 * @param buffer[in] the buffer to print
 * @param size[in] the size of the buffer
 */
static inline void mt_al_print_hex_buffer(const uint8_t *buffer,
    const uint32_t size) {
  if (!buffer) {
    return;
  }
  for (uint32_t i = 0; i < size; ++i) {
    printf("%02X", buffer[i]);
  }
}

/*!
 * \brief Print the Merkle Tree array list of hashes
 *
 * @param mt_al[in] the Merkle Tree data type instance
 */
void mt_al_print(const mt_al_t *mt_al);

#endif /* MT_ARR_LIST_H_ */
