#include "Flowdock.h"
#include "FlowdockAPI.h"
#include "ListenResponse.h"
#include "UserResponse.h"
#include <sstream>

#include <fstream>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#ifndef WIN32
#include <string.h>//?? TODO: Find out why including this?
#include <unistd.h>//For usleep
#endif

#include <cassert>
#include <iostream>

#include "FlowdockUserList.h"
#include "User.h"
#include "FlowdockFlowList.h"
#include "Flow.h"

#include "JSON.h"
#include "Utils.h"

using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { delete (x); (x) = NULL; }
#endif

#ifndef ARR_SIZE
#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])
#endif

#ifdef _MSC_VER 
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

FLOWDOCK_EXTERN int FlowdockCreate(FlowdockAPI* api, int nVerbose)
{
   *api = new Flowdock(nVerbose==1);

   return 0;
}

FLOWDOCK_EXTERN int FlowdockFree(FlowdockAPI *api)
{
   Flowdock* pFlowdock = (Flowdock*)*api;
   delete pFlowdock;
   return 0;
}

FLOWDOCK_EXTERN int FlowdockSetOrgFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow);
   Flowdock* pFlowdock = (Flowdock*)api;
   pFlowdock->SetOrgFlow(strOrg, strFlow);
   return 0;
}

FLOWDOCK_EXTERN int FlowdockSetUsernamePassword(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword)
{
   std::string strUsername(pstrUsername), strPassword(pstrPassword);
   Flowdock* pFlowdock = (Flowdock*)api;
   pFlowdock->SetUsernamePassword(strUsername, strPassword);
   return 0;
}

FLOWDOCK_EXTERN int FlowdockSay(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strUsername(pstrUsername), strPassword(pstrPassword), strMessage(pstrMessage);
   std::string strTags(pstrTags), strExternalUsername(pstrExternalUserName);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->Say(strOrg, strFlow, strUsername, strPassword, strMessage, nCommentTo, strTags, strExternalUsername) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockSayOrgFlowMessage(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strMessage(pstrMessage);
   std::string strTags(pstrTags), strExternalUsername(pstrExternalUserName);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->Say(strOrg, strFlow, strMessage, nCommentTo, strTags, strExternalUsername) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockSayDefaults(FlowdockAPI api, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName)
{
   std::string strMessage(pstrMessage);
   std::string strTags(pstrTags), strExternalUsername(pstrExternalUserName);
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->Say(strMessage, nCommentTo, strTags, strExternalUsername) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockTag( FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, int nCommentTo, const char* pstrTags )
{
   std::string strOrg( pstrOrg ), strFlow( pstrFlow ), strUsername( pstrUsername ), strPassword( pstrPassword );
   std::string strTags( pstrTags );
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->Tag( strOrg, strFlow, strUsername, strPassword, nCommentTo, strTags ) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockUploadFile(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrFilePath)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strUsername(pstrUsername), strPassword(pstrPassword), strFilePath(pstrFilePath);

   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->UploadFile(strOrg, strFlow, strUsername, strPassword, strFilePath) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockUploadOrgFlowFile(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrFilePath)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strFilePath(pstrFilePath);

   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->UploadFile(strOrg, strFlow, strFilePath) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockUploadFileDefaults(FlowdockAPI api, const char* pstrFilePath)
{
   std::string strFilePath(pstrFilePath);

   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->UploadFile(strFilePath) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockGetUsers(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword)
{
   std::string strOrg(pstrOrg), strFlow(pstrFlow), strUsername(pstrUsername), strPassword(pstrPassword);

   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->GetUsers(strOrg, strFlow, strUsername, strPassword) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockGetFlows(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword)
{
   std::string strUsername(pstrUsername), strPassword(pstrPassword);

   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->GetFlows(strUsername, strPassword) ? 1 : 0;
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

FLOWDOCK_EXTERN int FlowdockStartListeningDefaults(FlowdockAPI api)
{
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->StartListening() ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockAddListenCallback(FlowdockAPI api, FlowMessageCallback cb, void* pData)
{
   Flowdock* pFlowdock = (Flowdock*)api;
   return pFlowdock->AddListenCallback(cb, pData) ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockGetNicknameForUser(FlowdockAPI api, int nUser, char* pstrNickname, int& nSizeOfNickname)
{
   Flowdock* pFlowdock = (Flowdock*)api;

   std::string strNickname;
   bool bOK = pFlowdock->GetNicknameForUser(nUser, strNickname);
   if( !bOK ) return 0;
   if( nSizeOfNickname > 0 )
   {
      memcpy(pstrNickname, strNickname.c_str(), nSizeOfNickname + 1);
   }
   else
   {
      nSizeOfNickname = (int)strNickname.size();
   }

   return strNickname.size()>0 ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockGetEMailForUser(FlowdockAPI api, int nUser, char* pstrEmailAddress, int& nSizeOfEMailAddress)
{
   Flowdock* pFlowdock = (Flowdock*)api;

   std::string strEMail;
   bool bOK = pFlowdock->GetEmailForUser(nUser, strEMail);
   if (!bOK) return 0;
   if (nSizeOfEMailAddress > 0)
   {
      memcpy(pstrEmailAddress, strEMail.c_str(), nSizeOfEMailAddress + 1);
   }
   else
   {
      nSizeOfEMailAddress = (int)strEMail.size();
   }

   return strEMail.size()>0 ? 1 : 0;
}

FLOWDOCK_EXTERN int FlowdockGetFlowByID(FlowdockAPI api, char* pstrID, char* pstrFlowName, int& nSizeOfFlowName)
{
   Flowdock* pFlowdock = (Flowdock*)api;

   std::string strID(pstrID);
   std::string strFlowName;
   bool bOK = pFlowdock->GetFlowNameByID(strID, strFlowName);
   if( !bOK ) return 0;
   if( nSizeOfFlowName > 0 )
   {
      memcpy(pstrFlowName, strFlowName.c_str(), nSizeOfFlowName + 1);
   }
   else
   {
      nSizeOfFlowName = (int)strFlowName.size();
   }

   return strFlowName.size()>0 ? 1 : 0;
}

Flowdock::Flowdock(bool bVerbose)
:
#ifdef USE_PTHREADS
#ifdef WIN32
   m_mutexListen(PTHREAD_MUTEX_INITIALIZER)
#endif
#endif
   m_bExit(false), m_bListening(false), m_bVerbose(bVerbose)
{
#ifdef USE_PTHREADS
   pthread_mutex_init(&m_mutexListen, NULL);
   m_threadListen = pthread_self();
#endif
   curl_global_init(CURL_GLOBAL_ALL);
}

Flowdock::~Flowdock()
{
   StopListening();

   curl_global_cleanup();
}

bool Flowdock::SetOrgFlow(const std::string& strOrg, const std::string& strFlow)
{
   m_strDefaultOrg = strOrg;
   m_strDefaultFlow = strFlow;
   return true;
}

bool Flowdock::SetUsernamePassword(const std::string& strUsername, const std::string& strPassword)
{
   m_strDefaultUsername = strUsername;
   m_strDefaultPassword = strPassword;
   return true;
}

bool Flowdock::Say(const std::string& strOrg, 
   const std::string& strFlow, 
   const std::string& strUsername, 
   const std::string& strPassword, 
   const std::string& strMessage,
   int nCommentTo,
   const std::vector<std::string>& arrstrTags, 
   const std::string& strExternalName )
{
   CURL *curl;
   CURLcode res;

   std::string strURL = "https://api.flowdock.com/flows/";
   strURL += strOrg;
   strURL += "/";
   strURL += strFlow;
   strURL += "/messages";
   if( nCommentTo != -1 ) {
      strURL += "/" + IntToString(nCommentTo) + "/comments";
   }

   curl = curl_easy_init();
   if( !curl )
      return false;

   curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());

   std::string ctype_header = "Content-Type: application/json";

   JSONObjects objs;
   if( nCommentTo != -1 ) {
      objs["event"] = new JSON("comment");
   }
   else {
      objs["event"] = new JSON("message");
   }
   objs["content"] = new JSON(strMessage);

   JSONArray jsonTags;
   for(std::vector<std::string>::size_type i=0; i<arrstrTags.size(); i++) {
      jsonTags.push_back(new JSON(arrstrTags[i]));
   }
   if( jsonTags.size() > 0 )
      objs["tags"] = new JSON(jsonTags);

   if( strExternalName.length() > 0 )
      objs["external_user_name"] = new JSON(strExternalName);

   JSON json(objs);

   std::string data = json.Stringify();

   curl_easy_setopt(curl, CURLOPT_USERAGENT, "ajclient/0.0.1");
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
   std::string strUserPass = strUsername + ":" + strPassword;
   curl_easy_setopt(curl, CURLOPT_USERPWD, strUserPass.c_str());
#ifdef CURL_VERBOSE_OUTPUT
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

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

bool Flowdock::Say(const std::string& strOrg, 
   const std::string& strFlow, 
   const std::string& strUserName, 
   const std::string& strPassword, 
   const std::string& strMessage,
   int nCommentTo,
   const std::string& strTagsCommaSeparated /*= std::string()*/, 
   const std::string& strExternalName /*= std::string()*/)
{
   std::vector<std::string> arrTags;

   size_t  start = 0, end = 0;

   while ( end != string::npos)
   {
      end = strTagsCommaSeparated.find( ",", start);

      // If at end, use length=maxLength.  Else use length=end-start.
      arrTags.push_back( strTagsCommaSeparated.substr( start,
         (end == string::npos) ? string::npos : end - start));

      // If at end, use start=maxSize.  Else use start=end+delimiter.
      start = (   ( end > (string::npos - 1) )
         ?  string::npos  :  end + 1);
   }
   return Say(strOrg, strFlow, strUserName, strPassword, strMessage, nCommentTo, arrTags, strExternalName);
}

bool Flowdock::Say(const std::string& strOrg, 
   const std::string& strFlow, 
   const std::string& strMessage,
   int nCommentTo, 
   const std::string& strTagsCommaSeparated, 
   const std::string& strExternalName)
{
   assert(!m_strDefaultUsername.empty() && !m_strDefaultPassword.empty());
   if( m_strDefaultUsername.empty() || m_strDefaultPassword.empty() )
      return false;
   return Say(strOrg, strFlow, m_strDefaultUsername, m_strListenPassword, strMessage, nCommentTo, strTagsCommaSeparated, strExternalName);
}

bool Flowdock::Say(const std::string& strMessage, int nCommentTo,
   const std::string& strTagsCommaSeparated, 
   const std::string& strExternalName)
{
   assert( !m_strDefaultOrg.empty() && !m_strDefaultFlow.empty() && !m_strDefaultUsername.empty() && !m_strDefaultPassword.empty());
   if( m_strDefaultOrg.empty() || m_strDefaultFlow.empty() ||m_strDefaultUsername.empty() || m_strDefaultPassword.empty() )
      return false;
   return Say(m_strDefaultOrg, m_strDefaultFlow, m_strDefaultUsername, m_strListenPassword, strMessage, nCommentTo, strTagsCommaSeparated, strExternalName);
}

bool Flowdock::Tag( const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, int nCommentTo, const std::vector<std::string>& arrstrTags )
{
   CURL *curl;
   CURLcode res;

   std::string strURL = "https://api.flowdock.com/flows/";
   strURL += strOrg;
   strURL += "/";
   strURL += strFlow;
   strURL += "/messages";
   if ( nCommentTo != -1 )
   {
      strURL += "/" + IntToString( nCommentTo );
   }

   curl = curl_easy_init();
   if ( !curl )
      return false;

   curl_easy_setopt( curl, CURLOPT_URL, strURL.c_str() );

   std::string ctype_header = "Content-Type: application/json";

   JSONObjects objs;

   JSONArray jsonTags;
   for ( std::vector<std::string>::size_type i = 0; i < arrstrTags.size(); i++ )
   {
      jsonTags.push_back( new JSON( arrstrTags[i] ) );
   }
   if ( jsonTags.size() > 0 )
      objs["tags"] = new JSON( jsonTags );

   JSON json( objs );

   std::string data = json.Stringify();

   curl_easy_setopt( curl, CURLOPT_USERAGENT, "ajclient/0.0.1" );
   curl_easy_setopt( curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
   std::string strUserPass = strUserName + ":" + strPassword;
   curl_easy_setopt( curl, CURLOPT_USERPWD, strUserPass.c_str() );
#ifdef CURL_VERBOSE_OUTPUT
   curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L );
#endif

   curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L );
   curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L );

   curl_easy_setopt( curl, CURLOPT_POST, 1L );
   curl_easy_setopt( curl, CURLOPT_POSTFIELDS, data.c_str() );
   curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, data.size() );

   curl_slist* header = NULL;
   header = curl_slist_append( header, ctype_header.c_str() );
   curl_easy_setopt( curl, CURLOPT_HTTPHEADER, header );
   curl_easy_setopt( curl, CURLOPT_CUSTOMREQUEST, "PUT" );

   res = curl_easy_perform( curl );

   long http_code = 0;
   curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &http_code );

   curl_easy_cleanup( curl );

   return static_cast<int>( http_code ) == 200;
}

bool Flowdock::Tag( const std::string& strOrg, const std::string& strFlow, const std::string& strUserName, const std::string& strPassword, int nCommentTo, const std::string& strTagsCommaSeparated )
{
   std::vector<std::string> arrTags;

   size_t  start = 0, end = 0;

   while ( end != string::npos )
   {
      end = strTagsCommaSeparated.find( ",", start );

      // If at end, use length=maxLength.  Else use length=end-start.
      arrTags.push_back( strTagsCommaSeparated.substr( start,
         ( end == string::npos ) ? string::npos : end - start ) );

      // If at end, use start=maxSize.  Else use start=end+delimiter.
      start = ( ( end > ( string::npos - 1 ) )
                ? string::npos : end + 1 );
   }
   return Tag( strOrg, strFlow, strUserName, strPassword, nCommentTo, arrTags );
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
#ifdef CURL_VERBOSE_OUTPUT
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

   res = curl_easy_perform(curl);

   long http_code = 0;
   curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

   curl_easy_cleanup(curl);

   return static_cast<int>(http_code) == 200;
}

bool Flowdock::UploadFile(const std::string& strOrg, const std::string& strFlow, const std::string& strFilePath)
{
   assert(!m_strDefaultUsername.empty() && !m_strDefaultPassword.empty());
   if( m_strDefaultUsername.empty() || m_strDefaultPassword.empty() )
      return false;
   return UploadFile(strOrg, strFlow, m_strDefaultUsername, m_strListenPassword, strFilePath);
}

bool Flowdock::UploadFile(const std::string& strFilePath)
{
   assert( !m_strDefaultOrg.empty() && !m_strDefaultFlow.empty() && !m_strDefaultUsername.empty() && !m_strDefaultPassword.empty());
   if( m_strDefaultOrg.empty() || m_strDefaultFlow.empty() ||m_strDefaultUsername.empty() || m_strDefaultPassword.empty() )
      return false;
   return UploadFile(m_strDefaultOrg, m_strDefaultFlow, m_strDefaultUsername, m_strListenPassword, strFilePath);
}

bool Flowdock::GetUsers(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword)
{
   FlowdockUserList UserListRetriever(strOrg, strFlow, strUsername, strPassword);
   return UserListRetriever.GetUsers(m_apUsers);
}

bool Flowdock::GetFlows(const std::string& strUsername, const std::string& strPassword)
{
   FlowdockFlowList FlowListRetriever(strUsername, strPassword);
   return FlowListRetriever.GetFlows(m_apFlows);
}

bool Flowdock::IsListening()
{
   return m_bListening;
}

bool Flowdock::StopListening()
{
   if( !IsListening() )//Can't stop if not running
      return false;

#ifdef USE_PTHREADS
   pthread_mutex_lock( &m_mutexListen );
#else
   m_mutexListen.lock();
#endif
   m_bExit = true;
#ifdef USE_PTHREADS
   pthread_mutex_unlock( &m_mutexListen );
#else
   m_mutexListen.unlock();
#endif

#ifdef USE_PTHREADS
   pthread_join( m_threadListen, NULL);
#else
   m_threadListen.join();
#endif
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
   m_aListenOrgFlows.push_back(orgFlow);

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

#ifdef USE_PTHREADS
   int iRet = pthread_create( &m_threadListen, NULL, Flowdock::ListenThread, (void*)this);
#else
   std::thread t(Flowdock::ListenThread, (void*)this);
   m_threadListen = std::move(t);
#endif
   return true;
}

bool Flowdock::StartListening()
{
   assert( !m_strDefaultUsername.empty() && !m_strDefaultPassword.empty());
   if( m_strDefaultUsername.empty() || m_strDefaultPassword.empty() )
      return false;
   return StartListening(m_strDefaultUsername, m_strDefaultPassword);
}

bool Flowdock::AddListenCallback(FlowMessageCallback cb, void* pData)
{
   MessageCallbackAndDataPair messageCallbackPair;
   messageCallbackPair.cb = cb;
   messageCallbackPair.pUserData = pData;
   m_aListenCallbacks.push_back(messageCallbackPair);
   return false;
}

bool Flowdock::GetNicknameForUser(int nUser, std::string& strNickname) const
{
   if( m_apUsers.empty() )
      return false;

   for(std::vector<User*>::const_iterator it = m_apUsers.begin(); it != m_apUsers.end(); it++)
   {
      const User* pUser = *it;
      if( pUser->GetID() == nUser )
      {
         strNickname = pUser->GetNickname();
         return true;
      }
   }
   return false;
}

bool Flowdock::GetEmailForUser(int nUser, std::string& strEMailAddress) const
{
   if (m_apUsers.empty())
      return false;

   for (std::vector<User*>::const_iterator it = m_apUsers.begin(); it != m_apUsers.end(); it++)
   {
      const User* pUser = *it;
      if (pUser->GetID() == nUser)
      {
         strEMailAddress = pUser->GetEMail();
         return true;
      }
   }
   return false;
}

bool Flowdock::GetFlowNameByID(const std::string& strID, std::string& strFlowname) const
{
   if( m_apFlows.empty() )
      return false;

   for(std::vector<Flow*>::const_iterator it = m_apFlows.begin(); it != m_apFlows.end(); it++)
   {
      const Flow* pFlow = *it;
      if( pFlow->GetIDString() == strID )
      {
         strFlowname = pFlow->GetParamaterizedName();
         return true;
      }
   }
   return false;
}

int Flowdock::listen_progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
   Flowdock* pThis = (Flowdock*)clientp;
   return pThis->m_bExit ? 1 : 0;
}

size_t Flowdock::listen_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
   //Note: even though this is a callback it was setup from the worker thread.  Be careful about calling certain member
   //variables.  Perhaps add lock(s) if needed
 
   Flowdock* pThis = (Flowdock*)userdata;
   if( pThis->m_bExit )
      return 0;

   const char* pstr = reinterpret_cast<char*>(ptr);
   pThis->ReceivedResponse(pstr);

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
         bFirst = false;
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
#ifdef CURL_VERBOSE_OUTPUT
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

      curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, listen_progress);
      curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void*)this);
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, listen_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);

      res = curl_easy_perform(curl);
      if( res == CURLE_ABORTED_BY_CALLBACK )
      {
      }
      else if (res == CURLE_UNSUPPORTED_PROTOCOL)
      {
         assert(false);//SSL  Need to copy DLLS :)
      }
      else if (res != CURLE_OK)
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

void Flowdock::ReceivedResponse(const std::string& strListenResponse)
{
   if( m_bVerbose )
   {
      cout << "Received response: " << strListenResponse << endl;
   }

   if (strListenResponse == "\n\r\n")
      return;

   ListenResponse* pResponse = ListenResponse::Create(strListenResponse);

   if (pResponse != NULL)
   {
      FlowdockMessage message;
      message.eEvent = pResponse->GetEvent();
      MyStrCopy(message.Message, pResponse->GetContent().c_str(), sizeof(message.Message));
      message.nUserId = pResponse->GetUser();
      message.nThreadId = -1;
      message.nMessageId = pResponse->GetMessageID();

      message.bAdded = pResponse->IsAdding();
      std::vector<std::string> astrAddedTags = pResponse->GetAddedTags();
      std::vector<std::string> astrRemovedTags = pResponse->GetRemovedTags();
      message.nAddedTags = (int)astrAddedTags.size();
      message.nRemovedTags = (int)astrRemovedTags.size();
      for (int i = 0; i < message.nAddedTags; i++)
      {
         MyStrCopy(message.AddedTags[i], astrAddedTags[i].c_str(), 128);
      }
      for (int i = 0; i < message.nRemovedTags; i++)
      {
         MyStrCopy(message.RemovedTags[i], astrRemovedTags[i].c_str(), 128);
      }

      for (int i = 0; i < m_aListenCallbacks.size(); i++)
      {
         m_aListenCallbacks[i].cb(message, m_aListenCallbacks[i].pUserData);
      }

      delete pResponse;
   }
}





