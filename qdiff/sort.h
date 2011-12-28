#pragma once

#include <vector>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost;
namespace fs = boost::filesystem;

namespace qdiff {
  class Sort
  {
  public:
    Sort(void);
    ~Sort(void);

    void quickSort(std::vector<fs::path>* ay);

  private:
    void quickSort(std::vector<fs::path>* ay, int lower, int upper);
    int partition(std::vector<fs::path>* ay, int lower, int upper);
  };
}
