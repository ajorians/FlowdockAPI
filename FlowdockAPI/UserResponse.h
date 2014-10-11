#ifndef FLOWDOCKAPI_USERRESPONSE_H
#define FLOWDOCKAPI_USERRESPONSE_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h

class User;

class UserResponse
{
public:
   static void Parse(const std::string& strMessage, std::vector<User*>& arrpUsers);
};

#endif

