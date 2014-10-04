#include "Flowdock.h"
#include "FlowdockAPI.h"
#include <sstream>

#include <fstream>
#include <stdlib.h>

#ifndef WIN32
#include <string.h>//?? TODO: Find out why including this?
#endif

#include <cassert>
#include <iostream>

using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { delete (x); (x) = NULL; }
#endif

#ifndef ARR_SIZE
#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])
#endif

FLOWDOCK_EXTERN int FlowdockCreate(FlowdockAPI* api)
{
   *api = new Flowdock;

   return 0;
}

FLOWDOCK_EXTERN int FlowdockFree(FlowdockAPI *api)
{
   Flowdock* pFlowdock = (Flowdock*)*api;
   delete pFlowdock;
   return 0;
}

FLOWDOCK_EXTERN int FlowdockSay(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strUsername(pstrUsername), strPassword(pstrPassword), strMessage(pstrMessage);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->Say(strOrg, strFlow, strUsername, strPassword, strMessage) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockUploadFile(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrFilePath)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strUsername(pstrUsername), strPassword(pstrPassword), strFilePath(pstrFilePath);

   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->UploadFile(strOrg, strFlow, strUsername, strPassword, strFilePath) ? 1 : 0;
}

Flowdock::Flowdock()
{
   curl_global_init(CURL_GLOBAL_ALL);
}

Flowdock::~Flowdock()
{
   curl_global_cleanup();
}

bool Flowdock::Say(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword, const std::string& strMessage)
{
   CURL *curl;
   CURLcode res;

   std::string strURL = "https://api.flowdock.com/flows/";
   strURL += strOrg;
   strURL += "/";
   strURL += strFlow;
   strURL += "/messages";

   curl = curl_easy_init();
   if( !curl )
      return false;

   curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());

   std::string ctype_header = "Content-Type: application/json";

   std::string data = "{ \"event\": \"message\",  \"content\": \"";
   data += strMessage;
   data += "\",  \"tags\":  [\"todo\", \"#feedback\", \"@all\"]}";

   curl_easy_setopt(curl, CURLOPT_USERAGENT, "ajclient/0.0.1");
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
   std::string strUserPass = strUsername + ":" + strPassword;
   curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPass.c_str());
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

   curl_easy_setopt(curl, CURLOPT_POST, 1L);
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
   curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());

   curl_slist* header = NULL;
   header = curl_slist_append(header, ctype_header.c_str());
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

   res = curl_easy_perform(curl);

   long http_code = 0;
   curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

   curl_easy_cleanup(curl);

   return static_cast<int>(http_code) == 200;
}

bool Flowdock::UploadFile(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword, const std::string& strFilePath)
{
   CURL *curl;
   CURLcode res;

   std::string strURL = "https://api.flowdock.com/flows/";
   strURL += strOrg;
   strURL += "/";
   strURL += strFlow;
   strURL += "/messages";

   curl = curl_easy_init();
   if( !curl )
      return false;

   curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());

   struct curl_httppost *formpost=NULL;
   struct curl_httppost *lastptr=NULL;
   curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, "event",
                CURLFORM_COPYCONTENTS, "file",
                CURLFORM_END);

   curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, "content",
                CURLFORM_FILE, strFilePath.c_str(),
                CURLFORM_END);

   curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

   curl_easy_setopt(curl, CURLOPT_USERAGENT, "ajclient/0.0.1");
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
   std::string strUserPass = strUsername + ":" + strPassword;
   curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPass.c_str());
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

   res = curl_easy_perform(curl);

   long http_code = 0;
   curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

   curl_easy_cleanup(curl);

   return static_cast<int>(http_code) == 200;
}
