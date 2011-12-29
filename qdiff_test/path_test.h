#pragma once
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class PathTest : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(PathTest);
  CPPUNIT_TEST(testPathManagement);
  CPPUNIT_TEST_SUITE_END();

  void testPathManagement();
};

