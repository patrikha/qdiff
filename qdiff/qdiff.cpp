// crcdiff.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <algorithm>
#include "comparer.h"
#include "argumenthelper.h"
#include "fileservice.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace qdiff;

void printHelp()
{
  std::cout <<
    "qdiff - Patrik Hartlén\n" <<
    "\n" <<
    "Arguments:\n" <<
    "==========\n" <<
    "  source      : Source directory\n" <<
    "  s           : Short for source\n" <<
    "  destination : Destination directory\n" <<
    "  d           : Short for destination\n" <<
    "  recursive   : Iterate recursive on source and destination\n" <<
    "  r           : Short for recursive\n" <<
    "  comparer    : Compare method:\n" <<
    "              : [crc|size]\n" <<
    "  c           : Short for Comparer\n" <<
    "  mirror      : Mirror source to destination\n" <<
    "  m           : Short for mirror\n" <<
    "  i           : Ignore case\n" <<
    "  l           : Follow symlinks\n" <<
    "\n" <<
    "Sample:\n" <<
    "qdiff --s=[source path] --d=[destination path] --c=crc -r\n";
}

int main(int argc, char *argv[])
{
  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  if (argc == 1)
  {
    std::cout << "qdiff: missing operand after " << argv[0] << std::endl;
    std::cout << "qdiff: Try 'qdiff --help' for more information." << std::endl;
    return 1;
  }

  ArgumentHelper arg(argc, argv);

  bool recursive = arg.getRecursive();
  bool mirror = arg.getMirror();
  bool ignorecase = arg.getIgnoreCase();
  bool followsymlinks = arg.getFollowSymlinks();

  if (arg.help())
  {
    printHelp();
    return 1;
  }

  if (!arg.hasSource())
  {
    std::cout << "Source folder was not set." << std::endl;
    return 1;
  }

  if (!arg.hasDestination())
  {
    std::cout << "Destination folder was not set." << std::endl;
    return 1;
  }

  std::string source = arg.getSource();
  std::string destination = arg.getDestination();

  FileService service;
  bool validPaths = true;
  if (!service.validPath(source))
  {
    std::cout << "qdiff: " << source << ": No such directory" << std::endl;
    validPaths = false;
  }
  if (!service.validPath(destination))
  {
    if (mirror)
    {
      bool success = service.copyDirectory(source, destination);
      return success ? 0 : -1;
    }
    else
    {
      std::cout << "qdiff: " << destination << ": No such directory" << std::endl;
      validPaths = false;
    }
  }
  if (!validPaths)
    return 1;

  compareMethod method = arg.getComparer();

  Comparer comp(recursive, mirror, ignorecase, followsymlinks, method);
  return comp.compare(source, destination);
}
