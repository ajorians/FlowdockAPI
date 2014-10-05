#include "Flowdock.h"
#include "FlowdockAPI.h"
#include <sstream>

#include <fstream>
#include <stdlib.h>

#ifndef WIN32
#include <string.h>//?? TODO: Find out why including this?
#include <unistd.h>//For usleep
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

FLOWDOCK_EXTERN int FlowdockIsListening(FlowdockAPI api)
{
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->IsListening() ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockStopListening(FlowdockAPI api)
{
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->StopListening() ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockAddListenFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->AddListenFlow(strOrg, strFlow) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockRemoveListenFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->RemoveListenFlow(strOrg, strFlow) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockStartListening(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword)
{
   std::string strUsername(pstrUsername), strPassword(pstrPassword);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->StartListening(strUsername, strPassword) ? 1 : 0;
}

Flowdock::Flowdock()
:
#ifdef WIN32
   m_mutexListen(PTHREAD_MUTEX_INITIALIZER),
#endif
   m_bExit(false), m_bListening(false)
{
//#ifdef WIN32
   pthread_mutex_init(&m_mutexListen, NULL);
//#endif
   m_threadListen = pthread_self();
   curl_global_init(CURL_GLOBAL_ALL);
}

Flowdock::~Flowdock()
{
   StopListening();

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

bool Flowdock::IsListening()
{
   return m_bListening;
}

bool Flowdock::StopListening()
{
   if( !IsListening() )//Can't stop if not running
      return false;

   pthread_mutex_lock( &m_mutexListen );
   m_bExit = true;
   pthread_mutex_unlock( &m_mutexListen );

   pthread_join( m_threadListen, NULL);
   return true;
}

bool Flowdock::AddListenFlow(const std::string& strOrg, const std::string& strFlow)
{
   if( IsListening() )//Shouldn't be called mid-listening
      return false;

   OrgFlowPair orgFlow;
   orgFlow.m_strOrg = strOrg;
   orgFlow.m_strFlow = strFlow;
   //TODO: Check if org/flow already exists
   m_aListenOrgFlows.push_back(orgFlow);;

   return false;
}

bool Flowdock::RemoveListenFlow(const std::string& strOrg, const std::string& strFlow)
{
   if( IsListening() )//Shouldn't be called mid-listening
      return false;

   return false;
}

bool Flowdock::StartListening(const std::string& strUserName, const std::string& strPassword)
{
   if( IsListening() )//Can't listen if already listening
      return false;

   m_strListenUsername = strUserName;
   m_strListenPassword = strPassword;

   int iRet = pthread_create( &m_threadListen, NULL, Flowdock::ListenThread, (void*)this);
   return true;
}

size_t Flowdock::listen_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
   //Note: even though this is a callback it was setup from the worker thread.  Be careful about calling certain member
   //variables.  Perhaps add lock(s) if needed
 
   Flowdock* pThis = (Flowdock*)userdata;
   if( pThis->m_bExit )
      return 0;

   const char* pstr = reinterpret_cast<char*>(ptr);
   //TODO: Do stuff with pstr
   cout << "Message: " << pstr << endl;

   return size * nmemb;
}

void* Flowdock::ListenThread(void* ptr)
{
   Flowdock* pThis = (Flowdock*)ptr;
   pThis->m_bListening = true;
   pThis->ListenWorker();
   pThis->m_bListening = false;

   return NULL;
}

void Flowdock::ListenWorker()
{
   //Note: this is a worker thread so be careful about calling certain member variables.  Perhaps add lock(s) if needed
   assert(m_strListenUsername.length() != 0);
   assert(m_strListenPassword.length() != 0);

   while( !m_bExit )
   {
      CURL *curl;
      CURLcode res;

      std::string strURL = "https://stream.flowdock.com/flows/?filter=";

      if( m_aListenOrgFlows.size() == 0 )
      {
         assert(false);//Expected something in there.  Nonetheless handling this case
#ifdef _WIN32
         Sleep(100);//1/10 second
#else
         usleep(100*1000);
#endif
         continue;
      }

      bool bFirst = true;
      for(std::vector<OrgFlowPair>::const_iterator it = m_aListenOrgFlows.begin(); it!= m_aListenOrgFlows.end(); it++)
      {
         if( !bFirst )
            strURL += ",";

         strURL += (*it).m_strOrg;
         strURL += "/";
         strURL += (*it).m_strFlow;
      }

      curl = curl_easy_init();
      if( !curl )
      {
         //Not expected; perhaps could set some sort of error state so client can know.  Going to try again
#ifdef _WIN32
         Sleep(3000);//3 seconds
#else
         usleep(3000*1000);
#endif
         continue;
      }

      curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());

      curl_easy_setopt(curl, CURLOPT_USERAGENT, "ajclient/0.0.1");
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      std::string strUserPass = m_strListenUsername + ":" + m_strListenPassword;
      curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPass.c_str());
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, listen_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);

      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60/*seconds*/);

      res = curl_easy_perform(curl);
      if( res == CURLE_OPERATION_TIMEDOUT )
      {
      }
      else if( res != 0 )
      {
      }

      long http_code = 0;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

      curl_easy_cleanup(curl);

      //TODO: Check static_cast<int>(http_code) == 200;

#ifdef _WIN32
         Sleep(3000);//3 seconds
#else
         usleep(3000*1000);
#endif

   }
}







