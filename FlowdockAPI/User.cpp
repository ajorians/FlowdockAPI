#include "User.h"
#include "JSON.h"
#include <iostream>

using namespace std;

namespace 
{
   std::string WideToNarrow(const std::wstring& w)
   {
      std::string s( w.begin(), w.end() );
      return s;
   }
}

User::User(int nID, const std::string& strNick, const std::string& strEMail, const std::string& strAvatar, const std::string strName, 
                const std::string& strWebsite, bool bDisabled, time_t timeLastActivity, time_t timeLastPing, bool bInFlow, const std::string& strStatus)
:
m_nID(nID),
m_strNick(strNick),
m_strEMail(strEMail),
m_strAvatar(strAvatar),
m_strName(strName),
m_strWebsite(strWebsite),
m_bDisabled(bDisabled),
m_timeLastActivity(timeLastActivity),
m_timeLastPing(timeLastPing),
m_bInFlow(bInFlow),
m_strStatus(strStatus)
{
}

User* User::Create(const std::string& strMessage)
{
   cout << "User: " << strMessage << endl;
   JSONValue *value = JSON::Parse(strMessage.c_str());
   if( value == NULL )
      return NULL;

   if( value->IsObject() == false )
      return NULL;

   JSONObject root;
   root = value->AsObject();

   //ID
   if( root.find(L"id") == root.end() || !root[L"id"]->IsNumber() )
      return NULL;

   int nID = root[L"id"]->AsNumber();

   //Nick
   if( root.find(L"nick") == root.end() || !root[L"nick"]->IsString() )
      return NULL;

   std::string strNick = WideToNarrow(root[L"nick"]->AsString());

   //EMail
   if( root.find(L"email") == root.end() || !root[L"email"]->IsString() )
      return NULL;

   std::string strEMail = WideToNarrow(root[L"email"]->AsString());

   //Avatar
   if( root.find(L"avatar") == root.end() || !root[L"avatar"]->IsString() )
      return NULL;

   std::string strAvatar = WideToNarrow(root[L"avatar"]->AsString());

   //Name
   if( root.find(L"name") == root.end() || !root[L"name"]->IsString() )
      return NULL;

   std::string strName = WideToNarrow(root[L"name"]->AsString());

   //Website
   if( root.find(L"website") == root.end() || !root[L"website"]->IsString() )
      return NULL;

   std::string strWebsite = WideToNarrow(root[L"website"]->AsString());

   //Disabled
   if( root.find(L"disabled") == root.end() || !root[L"disabled"]->IsBool() )
      return NULL;

   bool bDisabled = root[L"disabled"]->AsBool();

   //Last_activity
   if( root.find(L"last_activity") == root.end() || !root[L"last_activity"]->IsNumber() )
      return NULL;

   double dLastActivity = root[L"last_activity"]->AsNumber();

   double dSecondsLastActivity = dLastActivity/1000;
   time_t timeLastActivity = dSecondsLastActivity;

   //Sent
   if( root.find(L"last_ping") == root.end() || !root[L"last_ping"]->IsNumber() )
      return NULL;

   double dLastPing = root[L"last_ping"]->AsNumber();

   double dSecondsLastPing = dLastPing/1000;
   time_t timeLastPing = dLastPing;

   //in_flow
   if( root.find(L"in_flow") == root.end() || !root[L"in_flow"]->IsBool() )
      return NULL;

   bool bInFlow = root[L"in_flow"]->AsBool();

   //Status
   if( root.find(L"status") == root.end() || !root[L"status"]->IsString() )
      return NULL;

   std::string strStatus = WideToNarrow(root[L"status"]->AsString());

   delete(value);

   User* pResponse = new User(
      nID,
      strNick,
      strEMail,
      strAvatar,
      strName,
      strWebsite,
      bDisabled,
      timeLastActivity,
      timeLastPing,
      bInFlow,
      strStatus);

   return pResponse;
}
