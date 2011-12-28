#pragma once

#include <map>
#include <string>

namespace qdiff {
  class Arguments
  {
  public:
    Arguments(int argc, char* argv[]);
    ~Arguments(void);

    std::string operator[](const std::string& key);

  private:
    std::map<std::string, std::string> parameters;
  };
}