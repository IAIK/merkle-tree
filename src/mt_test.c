/*
 * mt-test.c
 *
 *  Created on: 02.05.2014
 *      Author: dhein
 */

#include "config.h"
#include "merkletree.h"
#include "mt_arr_list.h"

#define D_TEST_VALUES 17

static uint8_t test_values[D_TEST_VALUES][D_HASH_LENGTH];

static void mt_test_init() {
  for (uint32_t i = 0; i < D_TEST_VALUES; ++i) {
    for (uint32_t c = 0; c < HASH_LENGTH; ++c) {
      test_values[i][c] = i;
    }
  }
}

void mt_test_mt_al() {
  mt_al_t *list = mt_al_create();
  for (uint32_t i = 0; i < D_TEST_VALUES; ++i) {
    mt_al_add(list, test_values[i]);
    mt_al_print(list);
  }
  mt_al_truncate(list, 11);
  mt_al_print(list);
  mt_al_truncate(list, 7);
  mt_al_print(list);
  mt_al_truncate(list, 5);
  mt_al_print(list);
  for (uint32_t i = D_TEST_VALUES - 4; i < D_TEST_VALUES; ++i) {
    mt_al_add(list, test_values[i]);
    mt_al_print(list);
  }
  mt_al_truncate(list, 1);
  mt_al_print(list);
  mt_al_truncate(list, 0);
  mt_al_print(list);
}

void mt_test_tree() {
  mt_t *mt = mt_create();
  mt_print(mt);
  for (uint32_t i = 0; i < 5; ++i) {
    mt_add(mt, test_values[i],i);
    mt_print(mt);
  }
  for (uint32_t i = 0; i < 5; ++i) {
    mt_verify(mt, test_values[i], i);
  }
  mt_update(mt, test_values[7],0);
  mt_print(mt);
  mt_delete(mt);
}

int main() {
  mt_test_init();
  mt_test_tree();
  return 0;
}

