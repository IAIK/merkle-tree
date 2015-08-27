/*!
 * \file
 * \brief Implements very simple tests which require manual checking.
 * Superseded by tests defined in the tests/ folder.
 */

#include "mt_config.h"
#include "merkletree.h"
#include "mt_arr_list.h"

#include <stdio.h>

#define D_TEST_VALUES 17

static uint8_t test_values[D_TEST_VALUES][HASH_LENGTH];

static void mt_test_init()
{
  for (uint32_t i = 0; i < D_TEST_VALUES; ++i) {
    for (uint32_t c = 0; c < HASH_LENGTH; ++c) {
      test_values[i][c] = i;
    }
  }
}

void mt_test_mt_al()
{
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

void mt_test_tree()
{
  mt_t *mt = mt_create();
  mt_print(mt);
  for (uint32_t i = 0; i < 5; ++i) {
    mt_add(mt, test_values[i], HASH_LENGTH);
    mt_print(mt);
  }
  for (uint32_t i = 0; i < 5; ++i) {
    if (mt_verify(mt, test_values[i], HASH_LENGTH, i) == MT_ERR_ROOT_MISMATCH) {
      printf("Root mismatch error!\n");
      return;
    }
  }
  mt_update(mt, test_values[7], HASH_LENGTH, 0);
  mt_print(mt);
  mt_delete(mt);
}

int main()
{
  mt_test_init();
  mt_test_tree();
  return 0;
}

