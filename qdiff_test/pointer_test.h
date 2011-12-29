#pragma once
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "boost/shared_ptr.hpp"

using namespace boost;

class MyObject
{
  public:
    MyObject(void);
    ~MyObject(void);
};

class PointerTest : public CppUnit::TestFixture
{ 
public: 
  CPPUNIT_TEST_SUITE(PointerTest);
  CPPUNIT_TEST(testPointers);
  CPPUNIT_TEST(testSharedPointers);
  CPPUNIT_TEST_SUITE_END();

  void testPointers();
  void testSharedPointer(shared_ptr<std::string> str);
  void testPointer(std::string* pstr);
  void testReference(std::string& rstr);
  void testConstReference(const std::string& rstr);

  void testSharedPointers();
  void testVectorByRef(const std::vector<MyObject>& vector);
  void testSharedPtr(const MyObject& o);
};
