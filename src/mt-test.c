/*
 * mt-test.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#include "config.h"
#include "mt_arr_list.h"

#define TEST_VALUES 17

int main() {
  uint8_t test_values[TEST_VALUES][HASH_LENGTH];
  for (uint32_t i = 0; i < TEST_VALUES; ++i) {
    for (uint32_t c = 0; c < HASH_LENGTH; ++c) {
      test_values[i][c] = i;
    }
  }
  mt_al_t *list = mt_al_create();
  for (uint32_t i = 0; i < TEST_VALUES; ++i) {
    mt_al_add(list, test_values[i]);
    mt_al_print(list);
  }
  mt_al_truncate(list, 11);
  mt_al_print(list);
  mt_al_truncate(list, 7);
  mt_al_print(list);
  mt_al_truncate(list, 5);
  mt_al_print(list);
  for (uint32_t i = TEST_VALUES-4; i < TEST_VALUES; ++i) {
    mt_al_add(list, test_values[i]);
    mt_al_print(list);
  }
  mt_al_truncate(list, 1);
  mt_al_print(list);
  mt_al_truncate(list, 0);
  mt_al_print(list);
	return 0;
}

