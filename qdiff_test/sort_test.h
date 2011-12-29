#pragma once
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class SortTest : public CppUnit::TestFixture
{ 
public: 
  CPPUNIT_TEST_SUITE(SortTest);
  CPPUNIT_TEST(testQuickSort);
  CPPUNIT_TEST(testAlgorithmSort);
  CPPUNIT_TEST_SUITE_END();

  void testQuickSort();
  void testAlgorithmSort();
};
