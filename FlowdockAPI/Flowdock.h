#ifndef FLOWDOCKAPI_FLOWDOCK_H
#define FLOWDOCKAPI_FLOWDOCK_H

#include <pthread.h>
#include <vector>
#include <string>
#include <curl/curl.h>
#include "FlowdockAPI.h"

class ListenResponse;
class User;

class Flowdock
{
public:
   Flowdock();
   ~Flowdock();

   bool Say(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strMessage);
   bool UploadFile(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strFilePath);
   bool GetUsers(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword);

   bool IsListening();
   bool StopListening();
   bool AddListenFlow(const std::string& strOrg, const std::string& strFlow);
   bool RemoveListenFlow(const std::string& strOrg, const std::string& strFlow);
   bool StartListening(const std::string& strUserName, const std::string& strPassword);

   int GetListenMessagesCount() const;
   int GetListenMessageType(int nIndex) const;
   std::string GetListenMessageContent(int nIndex) const;
   bool RemoveListenMessage(int nIndex);

protected:
   static size_t listen_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
   static void* ListenThread(void* ptr);
   void ListenWorker();
   void ReceivedResponse(const std::string& strListenResponse);

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

   mutable pthread_mutex_t m_mutexResponse;
   std::vector<ListenResponse*> m_apResponses;

   std::vector<User*> m_apUsers;
};


#endif
