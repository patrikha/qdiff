#include <fstream>
#include <stack>
#include <algorithm>
#include "fileservice.h"
#include "oschmod.h"

using namespace boost;
using namespace qdiff;

bool pathcompare(shared_ptr<fs::path> i, shared_ptr<fs::path> j) { return (*i < *j); }

FileService::FileService(void)
{
}

FileService::~FileService(void)
{
}

std::string FileService::readFile(const std::string& file)
{
  if (!validPath(file))
    return NULL;

  long filesize = (long)fs::file_size(fs::path(file));
  char input;
  char *buffer = new char[filesize];

  std::fstream infile;
  infile.open(file.data());

  long l = 0;
  while(infile.good())
  {
    infile.get(input);
    *(buffer + l) = input;
  }

  infile.close();

  std::string content(buffer);
  delete[] buffer;
  return content;
}

long FileService::getFileSize(const fs::path& file)
{
  if (!fs::is_regular(file))
    return -1;

  return (long)fs::file_size(file);
}

bool FileService::validPath(const fs::path& path)
{
  try
  {
    return fs::exists(path);
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: invalid path " << path.string() << "; exception: " << ex.what() << std::endl;
    return false;
  }
}

std::vector<shared_ptr<fs::path> > FileService::getFiles(const fs::path& directory, const bool _sort) const
{
  std::vector<shared_ptr<fs::path> > files;

  try
  {
    if (!fs::exists(directory))
      return files;
  
    fs::directory_iterator end_itr;
    for (fs::directory_iterator itr(directory); itr != end_itr; ++itr )
    {
      if (fs::is_directory(itr->status()))
        continue;
      else
        files.push_back(shared_ptr<fs::path>(new fs::path(itr->path())));
    }
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: can't get files in directory " << directory.string() << "; exception: " << ex.what() << std::endl;
  }

  if (_sort)
    sort(files.begin(), files.end(), pathcompare);

  return files;
}

std::vector<shared_ptr<fs::path> > FileService::getDirectories(const fs::path& directory, const bool& followsymlinks, const bool _sort) const
{
  std::vector<shared_ptr<fs::path> > directories;
  try
  {
    if (!fs::exists(directory))
      return directories;
  
    fs::directory_iterator end_itr;
    for (fs::directory_iterator itr(directory); itr != end_itr; ++itr )
    {
      if (!followsymlinks && fs::is_symlink(itr->status()))
        continue;
      if (is_directory(itr->status()))
        directories.push_back(shared_ptr<fs::path>(new fs::path(itr->path())));
    }
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: can't get sub directories in directory " << directory.string() << "; exception: " << ex.what() << std::endl;
  }

  if (_sort)
    sort(directories.begin(), directories.end(), pathcompare);

  return directories;
}

bool FileService::copyFile(const fs::path& source, const fs::path& destination)
{
  try
  {
    if (fs::exists(destination))
      os_chmod(destination.string().c_str(), S_IRUSR|S_IWUSR);
    fs::copy_file(source, destination, fs::copy_option::overwrite_if_exists);
    return true;
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: copy file " << source.string() << " to " << destination.string() << "; exception: " << ex.what() << std::endl;
    return false;
  }
}

bool FileService::deleteFile(const fs::path& file)
{
  try
  {
    os_chmod(file.string().c_str(), S_IRUSR|S_IWUSR);
    return fs::remove(file);
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: delete file " << file.string() << "; exception: " << ex.what() << std::endl;
    return false;
  }
}

bool FileService::copyDirectory(const fs::path& source, const fs::path& destination)
{
  bool success = true;

  std::stack<fs::path>* stack = new std::stack<fs::path>();
  stack->push(source);

  try
  {
    while (!stack->empty())
    {
      fs::path path = stack->top();
      stack->pop();

      // dest = destination + (difference between path and source)
      fs::path dest = destination / (path.string().substr(source.string().length()));
      fs::create_directory(dest);

      fs::directory_iterator end_itr;
      for (fs::directory_iterator itr(path); itr != end_itr; ++itr )
      {
        if (fs::is_directory(itr->status()))
          stack->push(itr->path());
        else
          copyFile(itr->path(), dest / itr->path().filename());
      }
    }
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: copy directory " << source.string() << " to " << destination.string() << "; exception: " << ex.what() << std::endl;
    success = false;
  }

  delete stack;

  return success;
}

bool FileService::deleteDirectory(const fs::path& directory)
{
  try
  {
    fs::remove_all(directory);
    return true;
  }
  catch (const std::exception ex)
  {
    std::cout << "ERROR: delete directory " << directory.string() << "; exception: " << ex.what() << std::endl;
  }

  return true;
}
