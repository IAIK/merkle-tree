/*
 * MerkleTreeTest.cpp
 *
 *  Created on: May 28, 2014
 *      Author: dhein
 */
/*
 * AesSivTest.cpp
 *
 *  Created on: May 14, 2014
 *      Author: dhein
 */
#include "merkletree.h"

#include <cppunit/extensions/HelperMacros.h>

class MerkleTreeTest: public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( MerkleTreeTest );
  CPPUNIT_TEST(testSimpleTree);
  CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown()
  {
  }

  void testSimpleTree()
  {
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(MerkleTreeTest);




