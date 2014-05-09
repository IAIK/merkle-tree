/*
 * mt_err.h
 *
 *  Created on: 09.05.2014
 *      Author: dhein
 */

#ifndef MT_ERR_H_
#define MT_ERR_H_

typedef enum mt_error {
  MT_SUCCESS           =  0,
  MT_ERR_OUT_Of_MEMORY = -1,
  MT_ERR_ILLEGAL_PARAM = -2,
  MT_ERR_ILLEGAL_STATE = -3,
  MT_ERR_UNSPECIFIED   = -4
} mt_error_t;

#endif /* MT_ERR_H_ */
