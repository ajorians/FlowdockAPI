#ifndef FLOWDOCKAPI_FLOWDOCK_H
#define FLOWDOCKAPI_FLOWDOCK_H

#include <pthread.h>
#include <vector>
#include <string>
#include <curl/curl.h>
#include "FlowdockAPI.h"

class Flowdock
{
public:
   Flowdock();
   ~Flowdock();

   bool Say(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strMessage);
   bool UploadFile(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strFilePath);

protected:

protected:

};


#endif
