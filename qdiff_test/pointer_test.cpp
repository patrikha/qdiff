#include "pointer_test.h"
#include <string>
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(PointerTest);

MyObject::MyObject(void)
{
}

MyObject::~MyObject(void)
{
}

void PointerTest::testPointers()
{
  std::cout << std::endl;

  // shared_ptr copied, content is same
  shared_ptr<std::string> str(new std::string("A shared_ptr string"));
  std::cout << "1 shared_ptr address: " << &str << " string address: " << &(*str) << std::endl;
  testSharedPointer(str);

  // pointer is copied, points to same
  std::string* pstr = new std::string("A pointer string");
  std::cout << "1 pointer address: " << &pstr << " points to: " << pstr << std::endl;
  testPointer(pstr);
  delete pstr;

  // reference is same
  std::string rstr("A reference string");
  std::cout << "1 reference address: " << &rstr << std::endl;
  testReference(rstr);
  testConstReference(rstr);
}

void PointerTest::testSharedPointer(shared_ptr<std::string> str)
{
  std::cout << "2 shared_ptr address: " << &str << " string address: " << &(*str) << std::endl;
}

void PointerTest::testPointer(std::string* pstr)
{
  std::cout << "2 pointer address: " << &pstr << " points to: " << pstr << std::endl;
}

void PointerTest::testReference(std::string& rstr)
{
  std::cout << "2 reference address: " << &rstr << std::endl;
}

void PointerTest::testConstReference(const std::string& rstr)
{
  std::cout << "2 const reference address: " << &rstr << std::endl;
}

void PointerTest::testSharedPointers()
{
  {
    std::vector<shared_ptr<MyObject>> vector;
    shared_ptr<MyObject> a(new MyObject());
    shared_ptr<MyObject> b(new MyObject());
    vector.push_back(a);
    vector.push_back(b);
    testSharedPtr(*a);
  }
  {
    std::vector<MyObject> vector;
    MyObject o;
    vector.push_back(o);
    testVectorByRef(vector);
  }
  {
    std::vector<shared_ptr<MyObject>> vector;
    shared_ptr<MyObject> a(new MyObject());
    shared_ptr<MyObject> b(new MyObject());
    vector.push_back(a);
    vector[0] = b;
  }
}

void PointerTest::testVectorByRef(const std::vector<MyObject>& vector)
{
  MyObject o = vector[0];
}

void PointerTest::testSharedPtr(const MyObject& o)
{
}