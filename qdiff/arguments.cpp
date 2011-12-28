#include "arguments.h"

#include <vector>
#include <iterator>
#include <sstream>
#include "boost/regex.hpp"
#include "boost/algorithm/string/regex.hpp"

using namespace qdiff;
using namespace boost;

Arguments::Arguments(int argc, char* argv[])
{
  regex splitter("^-{1,2}|="); // ^-{1,2}|^/|=|:
  regex remover("^['\"\"]?(.*?)['\"\"]?$");
  std::string parameter;

  if (argc <= 1)
    return;

  for (int i = 1; i < argc; i++)
  {
    std::stringstream ss;
    std::vector<std::string> parts;
    algorithm::split_regex(parts, argv[i], splitter); //max 3???

    switch (parts.size())
    {
      // Found a value (for the last parameter found (space separator))
    case 1:
      if (!parameter.empty())
      {
        if (parameters[parameter].empty())
        {
          parts[0] = regex_replace(parts[0], remover, "$1", boost::match_default); // TODO
          parameters[parameter] = parts[0];
        }
        parameter.clear();
      }
      // else no parameter waiting for a value, add it as value to parameter argc
      ss << i;
      parameters[ss.str()] = parts[0];
      break;
      // Found just a parameter
    case 2:
      // The last parameter is still waiting. With no value, set it to true.
      if (!parameter.empty())
      {
        if (parameters[parameter].empty())
          parameters[parameter] = "true";
      }
      parameter = parts[1];
      break;
      // Parameter with enclosed value
    case 3:
      // The last parameter is still waiting. With no value, set it to true.
      if (!parameter.empty())
      {
        if (parameters[parameter].empty())
          parameters[parameter] = "true";
      }
      parameter = parts[1];
      //// Remove possible enclosing characters (",')
      if (parameters[parameter].empty())
      {
        parts[2] = regex_replace(parts[2], remover, "$1", boost::match_default); // TODO
        parameters[parameter] = parts[2];
      }
      parameter.clear();
      break;
    }

    // In case a parameter is still waiting
    if (!parameter.empty())
    {
      if (parameters[parameter].empty())
        parameters[parameter] = "true";
    }
  }
}

Arguments::~Arguments(void)
{
}

std::string Arguments::operator[](const std::string& key)
{
  return parameters[key];
}
