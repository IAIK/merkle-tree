/* Copyright (c) IAIK, Graz University of Technology, 2015.
 * All rights reserved.
 * Contact: http://opensource.iaik.tugraz.at
 * 
 * This file is part of the Merkle Tree Library.
 * 
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and SIC. For further information
 * contact us at http://opensource.iaik.tugraz.at.
 * 
 * Alternatively, this file may be used under the terms of the GNU General
 * Public License as published by the Free Software Foundation version 2.
 * 
 * The Merkle Tree Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with the Merkle Tree Library. If not, see <http://www.gnu.org/licenses/>.
 */
/*!
 * \file
 * \brief Unit tests for testing the Merkle Tree implementation.
 */
#include "merkletree.h"

#include <string.h>

#include <cppunit/extensions/HelperMacros.h>

#define D_TEST_VALUES 17

class MerkleTreeTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( MerkleTreeTest );
  CPPUNIT_TEST(testSimpleTree);
  CPPUNIT_TEST(testSkewedTree);
  CPPUNIT_TEST_SUITE_END();

private:
  mt_t *mt;
  uint8_t test_values[D_TEST_VALUES][HASH_LENGTH];

  void mt_test_init(void)
  {
    for (uint32_t i = 0; i < D_TEST_VALUES; ++i) {
      for (uint32_t c = 0; c < HASH_LENGTH; ++c) {
        test_values[i][c] = i;
      }
    }
  }

public:
  void setUp()
  {
    mt = mt_create();
    mt_test_init();
  }

  void tearDown()
  {
    mt_delete(mt);
  }

  void testSimpleTree()
  {
    unsigned char ROOT_5_1[HASH_LENGTH] = { 0x24, 0x39, 0x37, 0xFE, 0x91, 0xB8,
        0xAF, 0xCC, 0xF7, 0x79, 0x51, 0xAF, 0x4E, 0x94, 0x6C, 0x99, 0x3E, 0x21,
        0xCF, 0xE1, 0x34, 0x64, 0x4F, 0xAD, 0x15, 0xDA, 0x30, 0x2E, 0xF0, 0x93,
        0xAE, 0x68 };
    unsigned char ROOT_5_2[HASH_LENGTH] = { 0x8F, 0x6F, 0xA8, 0x61, 0x7E, 0x7B,
        0x9D, 0x00, 0x4C, 0xF8, 0xE7, 0x4B, 0x54, 0x25, 0xF9, 0xA5, 0xA7, 0x27,
        0x37, 0xFD, 0xE3, 0xAD, 0x35, 0x73, 0xBB, 0xD1, 0x81, 0xD8, 0xD2, 0x60,
        0x82, 0x7F };
    mt_hash_t root;
    memset(root, 0, sizeof(mt_hash_t));
    for (uint32_t i = 0; i < 5; ++i) {
      CPPUNIT_ASSERT(mt_add(mt, test_values[i], HASH_LENGTH) == MT_SUCCESS);
    }
    for (uint32_t i = 0; i < 5; ++i) {
      CPPUNIT_ASSERT(
          mt_verify(mt, test_values[i], HASH_LENGTH, i) == MT_SUCCESS);
    }
    CPPUNIT_ASSERT(mt_get_root(mt, root) == MT_SUCCESS);
    CPPUNIT_ASSERT(!memcmp(ROOT_5_1, root, HASH_LENGTH));
    CPPUNIT_ASSERT(mt_update(mt, test_values[7], HASH_LENGTH, 0) == MT_SUCCESS);
    CPPUNIT_ASSERT(mt_get_root(mt, root) == MT_SUCCESS);
    CPPUNIT_ASSERT(!memcmp(ROOT_5_2, root, HASH_LENGTH));
  }

  mt_error_t add(mt_hash_t h) {
    return mt_add(mt, h, sizeof(mt_hash_t));
  }

  mt_error_t update(mt_hash_t h, uint32_t offset) {
    return mt_update(mt, h, sizeof(mt_hash_t), offset);
  }

  void testSkewedTree() {
    for (uint32_t i = 0; i < 3; ++i) {
      CPPUNIT_ASSERT(add(test_values[i]) == MT_SUCCESS);
    }
    CPPUNIT_ASSERT(update(test_values[3], 2) == MT_SUCCESS);
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(MerkleTreeTest);
