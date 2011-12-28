#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/filesystem.hpp"
#include <iostream>
#include <vector>

#include "sort.h"

using namespace boost;
namespace fs = boost::filesystem;

namespace qdiff {
  class FileService
  {
  public:
    FileService(void);
    ~FileService(void);

    std::string readFile(const std::string& file);
    long getFileSize(const fs::path& file);

    bool validPath(const fs::path& path);
    std::vector<shared_ptr<fs::path> > getFiles(const fs::path& directory, const bool sort = false) const;
    std::vector<shared_ptr<fs::path> > getDirectories(const fs::path& directory, const bool& followsymlinks, const bool sort = false) const;

    bool copyFile(const fs::path& source, const fs::path& destination);
    bool deleteFile(const fs::path& file);
    bool copyDirectory(const fs::path& source, const fs::path& destination);
    bool deleteDirectory(const fs::path& directory);
  };
}