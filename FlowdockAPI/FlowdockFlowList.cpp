#include "FlowdockFlowList.h"

#include "FlowResponse.h"
#include "Flow.h"
#include <curl/curl.h>
#include <cassert>

FlowdockFlowList::FlowdockFlowList(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword)
{
   CURL *curl;
   CURLcode res;

   curl = curl_easy_init();
   if( !curl )
      return;

   std::string strURL = "https://api.flowdock.com/flows";
   curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());

   curl_easy_setopt(curl, CURLOPT_USERAGENT, "ajclient/0.0.1");
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
   std::string strUserPass = strUsername + ":" + strPassword;
   curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPass.c_str());
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, flowList_callback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);

   res = curl_easy_perform(curl);

   long http_code = 0;
   curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

   curl_easy_cleanup(curl);
}

bool FlowdockFlowList::GetFlows(std::vector<Flow*>& apFlows)
{
   if( apFlows.size() > 0 ) {
      for(std::vector<Flow*>::iterator it = apFlows.begin(); it != apFlows.end(); it++) {
         delete *it;
      }
      apFlows.clear();
   }
   return FlowResponse::Parse(m_strData, apFlows);
}

size_t FlowdockFlowList::flowList_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
   FlowdockFlowList* pThis = (FlowdockFlowList*)userdata;

   const char* pstr = reinterpret_cast<char*>(ptr);
   pThis->m_strData.append(pstr);

   return size * nmemb;
}


