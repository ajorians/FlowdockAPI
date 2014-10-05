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

   bool IsListening();
   bool StopListening();
   bool AddListenFlow(const std::string& strOrg, const std::string& strFlow);
   bool RemoveListenFlow(const std::string& strOrg, const std::string& strFlow);
   bool StartListening(const std::string& strUserName, const std::string& strPassword);

protected:
   static size_t listen_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
   static void* ListenThread(void* ptr);
   void ListenWorker();

protected:
   pthread_t m_threadListen;
   pthread_mutex_t m_mutexListen;
   volatile bool m_bExit;
   volatile bool m_bListening;
   std::string m_strListenUsername;
   std::string m_strListenPassword;
   struct OrgFlowPair
   {
      std::string m_strOrg;
      std::string m_strFlow;
   };
   std::vector<OrgFlowPair> m_aListenOrgFlows;
};


#endif
