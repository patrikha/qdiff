#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/filesystem.hpp"
#include <string>
#include <vector>
#include "fileservice.h"

using namespace boost;
namespace fs = boost::filesystem;

namespace qdiff {
  struct PAIR
  {
    PAIR(const shared_ptr<fs::path> s, const shared_ptr<fs::path> d) : source(s), destination(d) {}

    const shared_ptr<fs::path> source;
    const shared_ptr<fs::path> destination;
  };

  struct CompareItem
  {
    char status;
    shared_ptr<fs::path> source;
    shared_ptr<fs::path> destination;
  };

  class Comparer;
  typedef bool (Comparer::*compareMethod)(const fs::path&, const fs::path&);

  class Comparer
  {
  public:
    Comparer(const bool& recursive, const bool& mirror, const bool& ignorecase, const bool& followsymlinks, compareMethod method);
    ~Comparer(void);

    int compare(const fs::path& source, const fs::path& destination);
    void reportDiffFile(const CompareItem& diffFile, compareMethod method);
    void reportDiffDirectory(const CompareItem& diffDirectory);
    void mirrorDiffFile(const CompareItem& diffFile, compareMethod method);
    void mirrorDiffDirectory(const CompareItem& diffDirectory);
    bool compareFileSize(const fs::path& source, const fs::path& destination);
    bool compareCRC32(const fs::path& source, const fs::path& destination);

  private:
    std::vector<shared_ptr<CompareItem> > difference(const std::vector<shared_ptr<fs::path> > source, const std::vector<shared_ptr<fs::path> > destination) const;

    FileService* fileService;
    bool recursive;
    bool mirror;
    bool ignorecase;
    bool followsymlinks;
    compareMethod method;
  };
}
