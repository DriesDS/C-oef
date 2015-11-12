#ifndef _PARSE_HPP
#define _PARSE_HPP

#include <sstream>
#include <stdexcept>

template <typename T>
bool parse(const char* s, T& value, bool throwexception=false)
{
  // this is not the best solution see: 
  // http://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c
  std::stringstream ss(s);
  ss >> value;
  if(ss.fail() || !ss.eof()) {
    if(throwexception) throw std::invalid_argument("Could not parse input");
    return false;
  }
  return true;
}

#endif
