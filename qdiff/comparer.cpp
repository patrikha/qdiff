#include "comparer.h"
#include "crc32.h"
#include "sort.h"
#include <iostream>
#include <vector>
#include <stack>
#include <locale>

using namespace qdiff;

#define inc(index, size) \
    if (*index < size) ++*index;

Comparer::Comparer(const bool& recursive, const bool& mirror, const bool& ignorecase, const bool& followsymlinks, compareMethod method) :
  recursive(recursive), mirror(mirror), ignorecase(ignorecase), followsymlinks(followsymlinks), method(method)
{
  fileService = new FileService();
}

Comparer::~Comparer(void)
{
  delete fileService;
}

int Comparer::compare(const fs::path& source, const fs::path& destination)
{
  if (!fileService->validPath(source) || !fileService->validPath(destination))
    return 1;

  std::stack<shared_ptr<PAIR> >* stack = new std::stack<shared_ptr<PAIR> >();
  stack->push(shared_ptr<PAIR>(new PAIR(shared_ptr<fs::path>(new fs::path(source)), shared_ptr<fs::path>(new fs::path(destination)))));

  while (!stack->empty())
  {
    shared_ptr<PAIR> pair = stack->top();
    stack->pop();

    const std::vector<shared_ptr<fs::path> > sourceFiles = fileService->getFiles(*pair->source, true);
    const std::vector<shared_ptr<fs::path> > destinationFiles = fileService->getFiles(*pair->destination, true);

    const std::vector<shared_ptr<CompareItem> > diffFiles = difference(sourceFiles, destinationFiles);

    for (uint32_t i = 0; i < diffFiles.size(); i++)
    {
      // add destination if missing
      shared_ptr<CompareItem> diffFile = diffFiles[i];
      if (diffFile->status == 'S')
        diffFile->destination = shared_ptr<fs::path>(new fs::path(*pair->destination / diffFile->source->filename()));

      if (mirror)
        mirrorDiffFile(*diffFile, method);
      else
        reportDiffFile(*diffFile, method);
    }

    if (recursive)
    {
      const std::vector<shared_ptr<fs::path> > sourceDirectories = fileService->getDirectories(*pair->source, followsymlinks, true);
      const std::vector<shared_ptr<fs::path> > destinationDirectories = fileService->getDirectories(*pair->destination, followsymlinks, true);

      const std::vector<shared_ptr<CompareItem> > diffDirectories = difference(sourceDirectories, destinationDirectories);

      for (uint32_t i = 0; i < diffDirectories.size(); i++)
      {
        if (diffDirectories[i]->status != '=')
        {
          // add destination if missing
          shared_ptr<CompareItem> diffDirectory = diffDirectories[i];
          if (diffDirectory->status == 'S')
            diffDirectory->destination = shared_ptr<fs::path>(new fs::path(*pair->destination / diffDirectory->source->filename()));

          if (mirror)
            mirrorDiffDirectory(*diffDirectory);
          else
            reportDiffDirectory(*diffDirectory);
        }
        else
        {
          //std::cout << "Common subdirectories: " << diffDirectories[i]->source.file_string() << " and " << diffDirectories[i]->destination.file_string() << std::endl;
          stack->push(shared_ptr<PAIR>(new PAIR(diffDirectories[i]->source, diffDirectories[i]->destination)));
        }
      }
    }
  }
  delete stack;

  return 0;
}

void Comparer::reportDiffFile(const CompareItem& diffFile, compareMethod method)
{
  if (diffFile.status == 'S')
    std::cout << "Only in " << diffFile.source->parent_path().file_string() << ": " << diffFile.source->leaf() << std::endl;
  else if (diffFile.status == 'D')
    std::cout << "Only in " << diffFile.destination->parent_path().file_string() << ": " << diffFile.destination->leaf() << std::endl;
  else
  {
    if (!(this->*method)(diffFile.source->string(), diffFile.destination->string()))
    {
      std::cout << diffFile.source->leaf() << " is different.";
    }
  }
}

void Comparer::reportDiffDirectory(const CompareItem& diffDirectory)
{
  if (diffDirectory.status == 'S')
    std::cout << "Only in " << diffDirectory.source->parent_path().file_string() << ": " << diffDirectory.source->leaf() << std::endl;
  else if (diffDirectory.status == 'D')
    std::cout << "Only in " << diffDirectory.destination->parent_path().file_string() << ": " << diffDirectory.destination->leaf() << std::endl;
}

void Comparer::mirrorDiffFile(const CompareItem& diffFile, compareMethod method)
{
  if (diffFile.status == 'S')
    fileService->copyFile(*diffFile.source.get(), *diffFile.destination.get());
  else if (diffFile.status == 'D')
    fileService->deleteFile(*diffFile.destination.get());
  else
  {
    if (!(this->*method)(diffFile.source.get()->string(), diffFile.destination.get()->string()))
    {
      fileService->copyFile(*diffFile.source.get(), *diffFile.destination.get());
    }
  }
}

void Comparer::mirrorDiffDirectory(const CompareItem& diffDirectory)
{
  if (diffDirectory.status == 'S')
    fileService->copyDirectory(*diffDirectory.source.get(), *diffDirectory.destination.get());
  else if (diffDirectory.status == 'D')
    fileService->deleteDirectory(*diffDirectory.destination.get());
}

std::vector<shared_ptr<CompareItem> > Comparer::difference(const std::vector<shared_ptr<fs::path> > source, const std::vector<shared_ptr<fs::path> > destination) const
{
  std::vector<shared_ptr<CompareItem> > differences;

  int sizeSource = source.size();
  int sizeDestination = destination.size();
  int s = 0, d = 0;

  if (sizeSource == 0 && sizeDestination == 0)
    return differences;

  shared_ptr<fs::path> sourcePath;
  shared_ptr<fs::path> destinationPath;
  int compare;

  do
  {
    if (s < sizeSource) sourcePath = source[s];
    if (d < sizeDestination) destinationPath = destination[d];

    shared_ptr<CompareItem> item(new CompareItem());

    if (s == sizeSource)
      compare = 1;
    else if (d == sizeDestination)
      compare = -1;
    else
    {
      if (!ignorecase)
        compare = sourcePath->leaf().compare(destinationPath->leaf());
      else
      {
        std::string sourceLeaf = sourcePath->leaf();
        std::string destinationLeaf = destinationPath->leaf();
        std::transform(sourceLeaf.begin(), sourceLeaf.end(), sourceLeaf.begin(), tolower);
        std::transform(destinationLeaf.begin(), destinationLeaf.end(), destinationLeaf.begin(), tolower);
        compare = sourceLeaf.compare(destinationLeaf);
      }
    }

    if (compare != 0)
    {
      if (compare < 0)
      { // source < destination
        item->status = 'S';
        item->source = sourcePath;
        inc(&s, sizeSource);
      }
      else
      { // source > destination
        item->status = 'D';
        item->destination = destinationPath;
        inc(&d, sizeDestination);
      }
    }
    else
    { // source == destination
      item->status = '=';
      item->source = sourcePath;
      item->destination = destinationPath;
      inc(&s, sizeSource);
      inc(&d, sizeDestination);
    }

    differences.push_back(item);

  } while (!(s == sizeSource && d == sizeDestination));

  return differences;
}

bool Comparer::compareFileSize(const fs::path& source, const fs::path& destination)
{
  return fileService->getFileSize(source) == fileService->getFileSize(destination);
}

bool Comparer::compareCRC32(const fs::path& source, const fs::path& destination)
{
  crc32 crc;
  return crc.calculate(source.file_string()) == crc.calculate(destination.file_string());
}
