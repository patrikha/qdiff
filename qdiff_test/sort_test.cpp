#include "sort_test.h"
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "..\qdiff\sort.h"

using namespace boost;
namespace fs = boost::filesystem;
using namespace qdiff;

CPPUNIT_TEST_SUITE_REGISTRATION(SortTest);

void SortTest::testQuickSort()
{
  std::vector<fs::path> paths;
  fs::path b = "b";
  fs::path a = "a";
  fs::path c = "c";
  paths.push_back(const_cast<fs::path&>(b));
  paths.push_back(const_cast<fs::path&>(a));
  paths.push_back(const_cast<fs::path&>(c));
  Sort sort;

  sort.quickSort(&paths);

  CPPUNIT_ASSERT("a" == paths[0].string());
  CPPUNIT_ASSERT("b" == paths[1].string());
  CPPUNIT_ASSERT("c" == paths[2].string());
}

bool cmp (shared_ptr<fs::path> i, shared_ptr<fs::path> j) { return (*i < *j); }

void SortTest::testAlgorithmSort()
{
  std::vector<shared_ptr<fs::path> > paths;
  fs::path b = "b";
  fs::path a = "a";
  fs::path c = "c";
  paths.push_back(shared_ptr<fs::path>(new fs::path("b")));
  paths.push_back(shared_ptr<fs::path>(new fs::path("a")));
  paths.push_back(shared_ptr<fs::path>(new fs::path("c")));

  sort(paths.begin(), paths.end(), cmp);

  CPPUNIT_ASSERT("a" == paths[0]->string());
  CPPUNIT_ASSERT("b" == paths[1]->string());
  CPPUNIT_ASSERT("c" == paths[2]->string());
}