#include "User.h"
#include "JSON.h"
#include <iostream>
#include <sstream>

using namespace std;

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

User::~User()
{

}

int User::GetID() const
{
   return m_nID;
}

std::string User::GetNickname() const
{
   return m_strNick;
}

std::string User::GetEMail() const
{
   return m_strEMail;
}

User* User::Create(const std::string& strMessage)
{
   JSON *value = JSON::Parse(strMessage.c_str());
   if( value == NULL )
      return NULL;

   if( value->IsObject() == false )
      return NULL;

   JSONObjects root;//TODO: Make const&; didn't do that now because have to change ["id"] to at("id)
   root = value->AsObject();

   //ID
   if( root.find("id") == root.end() || !root["id"]->IsNumber() )
      return NULL;

   int nID = root["id"]->AsNumber();

   //Nick
   if( root.find("nick") == root.end() || !root["nick"]->IsString() )
      return NULL;

   std::string strNick = root["nick"]->AsString();

   //EMail
   if( root.find("email") == root.end() || !root["email"]->IsString() )
      return NULL;

   std::string strEMail = root["email"]->AsString();

   //Avatar
   if( root.find("avatar") == root.end() || !root["avatar"]->IsString() )
      return NULL;

   std::string strAvatar = root["avatar"]->AsString();

   //Name
   if( root.find("name") == root.end() || !root["name"]->IsString() )
      return NULL;

   std::string strName = root["name"]->AsString();

   //Website
   std::string strWebsite;
   if( root.find("website") != root.end() && root["website"]->IsString() ) {
      strWebsite = root["website"]->AsString();
   }

   //Disabled
   if( root.find("disabled") == root.end() || !root["disabled"]->IsBool() )
      return NULL;

   bool bDisabled = root["disabled"]->AsBool();

   //Last_activity
   time_t timeLastActivity = 0;
   if( root.find("last_activity") != root.end() && root["last_activity"]->IsNumber() )
   {
      double dLastActivity = root["last_activity"]->AsNumber();

      double dSecondsLastActivity = dLastActivity/1000;
      timeLastActivity = dSecondsLastActivity;
   }

   //Sent
   time_t timeLastPing = 0;
   if( root.find("last_ping") != root.end() && root["last_ping"]->IsNumber() )
   {
      double dLastPing = root["last_ping"]->AsNumber();

      double dSecondsLastPing = dLastPing/1000;
      timeLastPing = dLastPing;
   }

   //in_flow
   if( root.find("in_flow") == root.end() || !root["in_flow"]->IsBool() )
      return NULL;

   bool bInFlow = root["in_flow"]->AsBool();

   //Status
   std::string strStatus;
   if( root.find("status") != root.end() && root["status"]->IsString() ) {
      strStatus = root["status"]->AsString();
   }

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
