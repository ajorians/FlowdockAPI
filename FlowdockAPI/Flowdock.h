#ifndef FLOWDOCKAPI_FLOWDOCK_H
#define FLOWDOCKAPI_FLOWDOCK_H

#ifdef USE_PTHREADS
#include <pthread.h>
#else
#include <thread>
#include <mutex>
#endif
#include <vector>
#include <string>
//#include <curl/curl.h>
#include "FlowdockAPI.h"

class ListenResponse;
class User;
class Flow;

class Flowdock
{
public:
   Flowdock(bool bVerbose);
   ~Flowdock();

   bool SetOrgFlow(const std::string& strOrg, const std::string& strFlow);
   bool SetUsernamePassword(const std::string& strUsername, const std::string& strPassword);

   bool Say(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strMessage, int nCommentTo, const std::vector<std::string>& arrstrTags, const std::string& strExternalName);
   bool Say(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strMessage, int nCommentTo, const std::string& strTagsCommaSeparated, const std::string& strExternalName);
   bool Say(const std::string& strOrg, const std::string& strFlow, const std::string& strMessage, int nCommentTo, const std::string& strTagsCommaSeparated, const std::string& strExternalName);
   bool Say(const std::string& strMessage, int nCommentTo = -1, const std::string& strTagsCommaSeparated = std::string(), const std::string& strExternalName = std::string());

   bool AddEmojiReaction(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, int nMessageID, const std::string& strEmojiReaction );
   bool AddEmojiReaction(int nMessageID, const std::string& strEmojiReaction );

   bool Tag( const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, int nCommentTo, const std::vector<std::string>& arrstrTags);
   bool Tag( const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, int nCommentTo, const std::string& strTagsCommaSeparated );

   bool UploadFile(const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, const std::string& strFilePath);
   bool UploadFile(const std::string& strOrg, const std::string& strFlow, const std::string& strFilePath);
   bool UploadFile(const std::string& strFilePath);

   bool GetUsers(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword);
   bool GetFlows(const std::string& strUsername, const std::string& strPassword);

   bool IsListening();
   bool StopListening();
   bool AddListenFlow(const std::string& strOrg, const std::string& strFlow);
   bool RemoveListenFlow(const std::string& strOrg, const std::string& strFlow);

   bool StartListening(const std::string& strUserName, const std::string& strPassword);
   bool StartListening();

   bool AddListenCallback(FlowMessageCallback cb, void* pData);

   bool GetNicknameForUser(int nUser, std::string& strNickname) const;
   bool GetEmailForUser(int nUser, std::string& strEMailAddress) const;
   bool GetFlowNameByID(const std::string& strID, std::string& strFlowname) const;

protected:
   static int listen_progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
   static size_t listen_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
   static void* ListenThread(void* ptr);
   void ListenWorker();
   void ReceivedResponse(const std::string& strListenResponse);

protected:
   std::string m_strDefaultOrg;
   std::string m_strDefaultFlow;
   std::string m_strDefaultUsername;
   std::string m_strDefaultPassword;

#ifdef USE_PTHREADS
   pthread_t m_threadListen;
   pthread_mutex_t m_mutexListen;
#else
   std::thread m_threadListen;
   std::mutex m_mutexListen;
#endif
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

   struct MessageCallbackAndDataPair
   {
      FlowMessageCallback cb;
      void* pUserData;
   };
   std::vector<MessageCallbackAndDataPair> m_aListenCallbacks;

   std::vector<User*> m_apUsers;
   std::vector<Flow*> m_apFlows;

   bool m_bVerbose;
};


#endif
