#pragma once

#include <string>

namespace qdiff {
  class crc32
  {
  public:
    crc32(void);
    ~crc32(void);
    
    std::string calculate(std::string file);
  };
}