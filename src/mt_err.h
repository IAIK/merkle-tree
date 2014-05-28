/*
 * mt_err.h
 *
 *  Created on: 09.05.2014
 *      Author: dhein
 */

#ifndef MT_ERR_H_
#define MT_ERR_H_

/*!
 * \brief Used to convey error information, if an Merkle Tree operation
 * fails.
 */
typedef enum mt_error {
  MT_SUCCESS           =  0, /*!< Operation terminated successfully */
  MT_ERR_OUT_Of_MEMORY = -1, /*!< There was not enough memory to complete the operation */
  MT_ERR_ILLEGAL_PARAM = -2, /*!< At least one of the specified parameters was illegal */
  MT_ERR_ILLEGAL_STATE = -3, /*!< The operation reached an illegal state */
  MT_ERR_ROOT_MISMATCH = -4, /*!< Signals the failure of a root hash verification */
  MT_ERR_UNSPECIFIED   = -5  /*!< A general error occurred */
} mt_error_t;

/*!
 * \brief wraps a given expression (e.g. a function call) with a test if the
 * result is not MT_SUCCESS and if this is the case returns the error code
 */
#define MT_ERR_CHK(f) do {mt_error_t r = f;if (r != MT_SUCCESS) {return r;}} while (0)


#endif /* MT_ERR_H_ */
