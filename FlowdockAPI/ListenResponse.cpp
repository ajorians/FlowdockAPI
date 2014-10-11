#include "ListenResponse.h"
#include "JSON.h"
#include <iostream>

using namespace std;

std::string WideToNarrow(const std::wstring& w)
{
   std::string s( w.begin(), w.end() );
   return s;
}

ListenResponse::ListenResponse(ListenEvent eEvent, const std::vector<std::string>& astrTags, const std::string& strUUID, double dID,
                  const std::string& strFlow, const std::string& strContent, time_t timeSent, const std::string& strApp,
                  const std::vector<std::string>& astrAttachments, double dUser)
: m_eEvent(eEvent),
m_astrTags(astrTags),
m_strUUID(strUUID),
m_dID(dID),
m_strFlow(strFlow),
m_strContent(strContent),
m_timeSent(timeSent),
m_strApp(strApp),
m_astrAttachments(astrAttachments),
m_dUser(dUser)
{
}

ListenResponse* ListenResponse::Create(const std::string& strMessage)
{
   cout << "Received response: " << strMessage << endl;
   JSONValue *value = JSON::Parse(strMessage.c_str());
   if( value == NULL )
      return NULL;

   if( value->IsObject() == false )
      return NULL;

   JSONObject root;
   root = value->AsObject();

   //Event
   if( root.find(L"event") == root.end() || !root[L"event"]->IsString() )
      return NULL;
   
   std::string strEvent = WideToNarrow(root[L"event"]->AsString());
   cout << "StrEvent: " << strEvent << endl;
   ListenEvent eEvent = Message;
   if( strEvent == "activity.user" )
      eEvent = Activity_User;

   //Tags
   if( root.find(L"tags") == root.end() || !root[L"tags"]->IsArray() )
      return NULL;

   std::vector<std::string> astrTags;
   JSONArray aTags = root[L"tags"]->AsArray();
   for(unsigned int i=0; i<aTags.size(); i++)
   {
      std::string strTag = WideToNarrow(aTags[i]->Stringify());
      cout << "Tag: " << strTag << endl;
      astrTags.push_back(strTag);
   }

   //UUID
   if( root.find(L"uuid") == root.end() || !root[L"uuid"]->IsString() )
      return NULL;

   std::string strUUID = WideToNarrow(root[L"uuid"]->AsString());
   cout << "UUID: " << strUUID << endl;

   //Persist

   //ID
   if( root.find(L"id") == root.end() || !root[L"id"]->IsNumber() )
      return NULL;

   double dID = root[L"id"]->AsNumber();

   //Flow
   if( root.find(L"flow") == root.end() || !root[L"flow"]->IsString() )
      return NULL;

   std::string strFlow = WideToNarrow(root[L"flow"]->AsString());

   //Content

   //Sent
   if( root.find(L"sent") == root.end() || !root[L"sent"]->IsNumber() )
      return NULL;

   double dSent = root[L"sent"]->AsNumber();

   double dSecondsSinceEpoch = dSent/1000;
   time_t timeSent = dSecondsSinceEpoch;

   //App
   if( root.find(L"app") == root.end() || !root[L"app"]->IsString() )
      return NULL;

   std::string strApp = WideToNarrow(root[L"app"]->AsString());

   //Attachments

   //User
   if( root.find(L"user") == root.end() || !root[L"user"]->IsNumber() )
      return NULL;

   double dUser = root[L"user"]->AsNumber();

   delete(value);

   ListenResponse* pResponse = new ListenResponse(
      eEvent,
      astrTags,
      strUUID,
      dID,
      strFlow,
      std::string(),//Content
      timeSent,
      strApp,
      std::vector<std::string>(),//Attachments
      dUser);

   return pResponse;
}
