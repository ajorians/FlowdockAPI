#include "Utils.h"
#include <sstream>

std::string IntToString(int nValue)
{
   std::ostringstream oss;
   oss << nValue;

   return oss.str();
}

void MyStrCopy(char* pstrDst, const char* pstrSrc, int nSize)
{
#ifdef _WIN32
   strcpy_s(pstrDst, nSize, pstrSrc);
#else
   strcpy(pstrDst, pstrSrc);
#endif
}

