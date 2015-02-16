#include "FlowdockUserList.h"

#include "UserResponse.h"
#include <curl/curl.h>
#include "Defines.h"
#include "User.h"

FlowdockUserList::FlowdockUserList(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword)
{
   CURL *curl;
   CURLcode res;

   curl = curl_easy_init();
   if( !curl )
      return;

   std::string strURL = "https://api.flowdock.com/flows/";
   strURL += strOrg;
   strURL += "/";
   strURL += strFlow;
   strURL += "/users";
   curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());

   curl_easy_setopt(curl, CURLOPT_USERAGENT, "ajclient/0.0.1");
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
   std::string strUserPass = strUsername + ":" + strPassword;
   curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPass.c_str());
#ifdef CURL_VERBOSE_OUTPUT
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, userList_callback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);

   res = curl_easy_perform(curl);

   long http_code = 0;
   curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

   curl_easy_cleanup(curl);
}

bool FlowdockUserList::GetUsers(std::vector<User*>& apUsers)
{
   if( apUsers.size() > 0 ) {
      for(std::vector<User*>::iterator it = apUsers.begin(); it != apUsers.end(); it++) {
         delete *it;
      }
      apUsers.clear();
   }
   return UserResponse::Parse(m_strData, apUsers);
}

size_t FlowdockUserList::userList_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
   FlowdockUserList* pThis = (FlowdockUserList*)userdata;

   const char* pstr = reinterpret_cast<char*>(ptr);
   pThis->m_strData.append(pstr);

   return size * nmemb;
}


