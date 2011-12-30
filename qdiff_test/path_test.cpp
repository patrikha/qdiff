#include "path_test.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

CPPUNIT_TEST_SUITE_REGISTRATION(PathTest);

void PathTest::testPathManagement()
{
  fs::path a("C:\\patha\\pathb\\filea.ext");
  CPPUNIT_ASSERT("filea.ext" == a.filename());

  fs::path b("C:\\patha\\pathb");
  fs::path c("C:\\pathx");

  fs::path d = b / c.filename();
  CPPUNIT_ASSERT("C:\\patha\\pathb\\pathx" == d.string());

  //c + a - b
  fs::path e = c / (a.string().substr(b.string().length()));
  CPPUNIT_ASSERT("C:\\pathx\\filea.ext" == e.string());

  CPPUNIT_ASSERT("C:\\pathx" == e.parent_path().string());
}
