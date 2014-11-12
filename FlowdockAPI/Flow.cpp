#include "Flow.h"
#include "JSON.h"
#include <iostream>
#include <sstream>

using namespace std;

Flow::Flow(const std::string& strID,
   const std::string& strName,
   const std::string& strParameterizedName,
   int nUnreadMentions,
   bool bOpen,
   bool bJoined,
   const std::string& strURL,
   const std::string& strWebURL,
   const std::string& strJoinURL)
:
m_strID(strID),
m_strName(strName),
m_strParameterizedName(strParameterizedName),
m_nUnreadMentions(nUnreadMentions),
m_bOpen(bOpen),
m_bJoined(bJoined),
m_strURL(strURL),
m_strWebURL(strWebURL),
m_strJoinURL(strJoinURL)
{

}

Flow::~Flow()
{

}

std::string Flow::GetIDString() const
{
   return m_strID;
}

std::string Flow::GetParamaterizedName() const
{
   return m_strParameterizedName;
}

Flow* Flow::Create(const std::string& strMessage)
{
   JSON *value = JSON::Parse(strMessage.c_str());
   if( value == NULL ) {
      cout << "Problems paring this JSON: " << strMessage << endl;
      return NULL;
   }

   if( value->IsObject() == false ){
      cout << "Problems thinking this is an object: " << strMessage << endl;
      return NULL;
   }

   JSONObjects root;//TODO: Make const&; didn't do that now because have to change ["id"] to at("id)
   root = value->AsObject();

   //ID
   if( root.find("id") == root.end() || !root["id"]->IsString() )
      return NULL;

   std::string strID = root["id"]->AsString();

   //Name
   if( root.find("name") == root.end() || !root["name"]->IsString() )
      return NULL;

   std::string strName = root["name"]->AsString();

   //ParameterizedName
   if( root.find("parameterized_name") == root.end() || !root["parameterized_name"]->IsString() )
      return NULL;

   std::string strParameterizedName = root["parameterized_name"]->AsString();

   //unread_mentions
   int nUnreadMentions = 0;
   if( root.find("unread_mentions") != root.end() && root["unread_mentions"]->IsNumber() ) {
      nUnreadMentions = (int)root["unread_mentions"]->AsNumber();
   }

   //open
   if( root.find("open") == root.end() || !root["open"]->IsBool() )
      return NULL;

   bool bOpen = root["open"]->AsBool();

   //joined
   if( root.find("joined") == root.end() || !root["joined"]->IsBool() )
      return NULL;

   bool bJoined = root["joined"]->AsBool();

   //url
   std::string strURL;
   if( root.find("url") != root.end() && root["url"]->IsString() ) {
      strURL = root["url"]->AsString();
   }

   //web_url
   std::string strWebURL;
   if( root.find("web_url") != root.end() && root["web_url"]->IsString() ) {
      strWebURL = root["web_url"]->AsString();
   }

   //join_url
   std::string strJoinURL;
   if( root.find("join_url") != root.end() && root["join_url"]->IsString() ) {
     strJoinURL = root["join_url"]->AsString();
   }

   delete(value);

   Flow* pResponse = new Flow(
      strID,
      strName,
      strParameterizedName,
      nUnreadMentions,
      bOpen,
      bJoined,
      strURL,
      strWebURL,
      strJoinURL);

   return pResponse;
}
