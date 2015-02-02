#include "Utils.h"
#include <sstream>

std::string IntToString(int nValue)
{
   std::ostringstream oss;
   oss << nValue;

   return oss.str();
}

