#pragma once
#include <string>
#include "arguments.h"
#include "comparer.h"

namespace qdiff {
  class ArgumentHelper
  {
  public:
    ArgumentHelper(int argc, char *argv[]);
    ~ArgumentHelper(void);

    bool help();
    bool hasSource();
    std::string getSource();
    bool hasDestination();
    std::string getDestination();
    bool getRecursive();
    compareMethod getComparer();
    bool getMirror();
    bool getIgnoreCase();
    bool getFollowSymlinks();

  private:
    Arguments arg;
  };
}
