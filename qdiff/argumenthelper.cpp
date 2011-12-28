#include "argumenthelper.h"

using namespace qdiff;

ArgumentHelper::ArgumentHelper(int argc, char *argv[]) : arg(argc, argv)
{
}

ArgumentHelper::~ArgumentHelper(void)
{
}

bool ArgumentHelper::help()
{
  if (!arg["help"].empty() || !arg["h"].empty())
    return true;

  return false;
}

bool ArgumentHelper::hasSource()
{
  if (arg["source"].empty() && arg["s"].empty() && arg["1"].empty())
    return false;

  return true;
}

std::string ArgumentHelper::getSource()
{
  std::string source;
  if (!arg["source"].empty())
    source = arg["Comparer"];
  else if (!arg["s"].empty())
    source = arg["s"];
  else if (!arg["1"].empty())
    source = arg["1"];

  return source;
}

bool ArgumentHelper::hasDestination()
{
  if (arg["destination"].empty() && arg["d"].empty() && arg["2"].empty())
    return false;

  return true;
}

std::string ArgumentHelper::getDestination()
{
  std::string destination;
  if (!arg["destination"].empty())
    destination = arg["destination"];
  else if (!arg["d"].empty())
    destination = arg["d"];
  else if (!arg["2"].empty())
    destination = arg["2"];

  return destination;
}

bool ArgumentHelper::getRecursive()
{
  bool recursive = false;
  if (!arg["recursive"].empty())
    recursive = arg["recursive"] == "true";
  else if (!arg["r"].empty())
    recursive = arg["r"] == "true";

  return recursive;
}

compareMethod ArgumentHelper::getComparer()
{
  std::string compareOption;
  if (!arg["Comparer"].empty())
    compareOption = arg["Comparer"];
  else if (!arg["c"].empty())
    compareOption = arg["c"];
  else
    compareOption = "crc32";

  std::transform(compareOption.begin(), compareOption.end(), compareOption.begin(), tolower);

  compareMethod method;
  // switch on compare method
  if (compareOption == "crc32")
    method = &Comparer::compareCRC32;
  else if (compareOption == "size")
    method = &Comparer::compareFileSize;
  else 
    method = &Comparer::compareCRC32;

  return method;
}

bool ArgumentHelper::getMirror()
{
  bool mirror = false;
  if (!arg["mirror"].empty())
    mirror = arg["mirror"] == "true";
  else if (!arg["m"].empty())
    mirror = arg["m"] == "true";

  return mirror;
}

bool ArgumentHelper::getIgnoreCase()
{
  bool ignore = false;
  if (!arg["i"].empty())
    ignore = arg["i"] == "true";

  return ignore;
}

bool ArgumentHelper::getFollowSymlinks()
{
  bool follow = false;
  if (!arg["l"].empty())
    follow = arg["l"] == "true";

  return follow;
}
