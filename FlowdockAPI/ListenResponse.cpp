#include "ListenResponse.h"
#include "JSON.h"
#include "Utils.h"
#include <cstring>//For strlen
#include <cstdlib>//For atoi
#include <iostream>

using namespace std;

ListenResponse::ListenResponse(EventType eEvent, const std::string& strThreadId, const std::vector<std::string>& astrAddTags, const std::vector<std::string>& astrRemovedTags, const std::string& strUUID, int nID,
                  const std::string& strFlow, const std::string& strContent, time_t timeSent, const std::string& strApp, bool bAdded,
                  const std::vector<std::string>& astrAttachments, int nUser)
: m_eEvent(eEvent),
m_strThreadId( strThreadId ),
m_astrAddedTags(astrAddTags),
m_astrRemovedTags(astrRemovedTags),
m_strUUID(strUUID),
m_nID(nID),
m_strFlow(strFlow),
m_strContent(strContent),
m_timeSent(timeSent),
m_strApp(strApp),
m_bAdded( bAdded ),
m_astrAttachments(astrAttachments),
m_nUser(nUser)
{
}

ListenResponse* ListenResponse::Create(const std::string& strMessage)
{
   JSON *value = JSON::Parse(strMessage);
   if( value == NULL )
      return NULL;

   if( value->IsObject() == false )
      return NULL;

   JSONObjects root;
   root = value->AsObject();

   //Threadid
   std::string strThreadId;
   if (root.find("thread_id") != root.end() && root.at("thread_id")->IsString())
      strThreadId = root.at("thread_id")->AsString();

   //Event
   if( root.find("event") == root.end() || !root.at("event")->IsString() )
      return NULL;

   std::string strEvent = root["event"]->AsString();
   EventType eEvent = Message;
   if ( strEvent == "activity.user" )
      eEvent = Activity_User;
   else if ( strEvent == "comment" )
      eEvent = Comment;
   else if ( strEvent == "tag-change" )
      eEvent = Tag_Change;
   else if ( strEvent == "message-edit" )
      eEvent = MessageEdit;
   else if( strEvent == "message-delete" )
      eEvent = Message_Delete;
   else if( strEvent == "thread-change" )
      eEvent = Thread_Change;
   else if( strEvent == "emoji-reaction" )
      eEvent = Emoji_Reaction;

   //Tags
   if( root.find("tags") == root.end() || !root["tags"]->IsArray() )
      return NULL;

   std::vector<std::string> astrTags;
   JSONArray aTags = root["tags"]->AsArray();
   for(unsigned int i=0; i<aTags.size(); i++)
   {
      std::string strTag = aTags[i]->Stringify();
      astrTags.push_back(strTag);
   }

   //UUID
   std::string strUUID;
   if( root.find("uuid") != root.end() && root["uuid"]->IsString() ) {
      strUUID = root["uuid"]->AsString();
   }

   //Persist

   //ID
   if( root.find("id") == root.end() || !root["id"]->IsNumber() )
      return NULL;

   int nID = (int)root["id"]->AsNumber();
   if( eEvent == Comment && astrTags.size() > 0 )
   {
      for(unsigned int i=0; i<astrTags.size(); i++)
      {
         std::string strTag = astrTags[i];
         if( strTag.length() > strlen("\"influx:") )
         {
            std::string strType = strTag.substr(0, strlen("\"influx:"));
            if( strType == "\"influx:" )
            {
               std::string strID = strTag.substr(strlen("\"influx:"), strTag.length() - strlen("\"influx:")-1/*closing quote*/);
               nID = atoi(strID.c_str());
            }
         }
      }
   }

   //Flow
   if( root.find("flow") == root.end() || !root["flow"]->IsString() )
      return NULL;

   std::string strFlow = root["flow"]->AsString();

   //Content
   //Looks like content is an array showing last activity when somebody is typing
   //And a string when somebody wrote something
   std::string strContent;
   bool bAdded = false;
   std::vector<std::string> arrAddedTags;
   std::vector<std::string> arrRemovedTags;
   if( eEvent != Comment && root.find("content") != root.end() && root["content"]->IsString() ) {
      strContent = root["content"]->AsString();
   }
   else if( eEvent == Comment && root.find("content") != root.end() && root["content"]->IsObject() ) {
      JSON* pContent = root["content"];
      if( pContent->HasChild("text") && pContent->Child("text")->IsString() ) {
         strContent = pContent->Child("text")->AsString();
      }
   }
   else if ( eEvent == Tag_Change && root.find( "content" ) != root.end() && root["content"]->IsObject() )
   {
      JSON* pContent = root["content"];
      if ( pContent->HasChild( "message" ) && pContent->Child( "message" )->IsNumber() )
      {
         nID = (int)pContent->Child( "message" )->AsNumber();
      }
      if (pContent->HasChild("add") && pContent->Child("add")->IsArray())
      {
         JSONArray addedTags = pContent->Child("add")->AsArray();
         for (int i = 0; i < addedTags.size(); i++)
         {
            std::string strAddedTag = addedTags[i]->AsString();
            arrAddedTags.push_back(strAddedTag);
         }
      }
      if (pContent->HasChild("remove") && pContent->Child("remove")->IsArray())
      {
         JSONArray removedTags = pContent->Child("remove")->AsArray();
         for (int i = 0; i < removedTags.size(); i++)
         {
            std::string strRemovedTag = removedTags[i]->AsString();
            arrRemovedTags.push_back(strRemovedTag);
         }
      }
   }
   else if (eEvent == MessageEdit && root.find("content") != root.end() && root["content"]->IsObject())
   {
      JSON* pContent = root["content"];
      if (pContent->HasChild("message") && pContent->Child("message")->IsNumber())
      {
         nID = (int)pContent->Child("message")->AsNumber();
      }
      if (pContent->HasChild("updated_content") && pContent->Child("updated_content")->IsString())
      {
         strContent = pContent->Child("updated_content")->AsString();
      }
   }
   else if (eEvent == Emoji_Reaction && root.find("content") != root.end() && root["content"]->IsObject())
   {
      JSON* pContent = root["content"];

      std::string strEmoji;
      if (pContent->HasChild("emoji") && pContent->Child("emoji")->IsString())
      {
         strEmoji = pContent->Child("emoji")->AsString();
      }

      if (pContent->HasChild("type") && pContent->Child("type")->IsString())
      {
         std::string strType = pContent->Child("type")->AsString();
         bAdded = strType == "add";
      }

      strContent = strEmoji;
   }

   //Sent
   if( root.find("sent") == root.end() || !root["sent"]->IsNumber() )
      return NULL;

   double dSent = root["sent"]->AsNumber();

   double dSecondsSinceEpoch = dSent/1000;
   time_t timeSent = (time_t)dSecondsSinceEpoch;

   //App
   std::string strApp;
   if( root.find("app") != root.end() && root["app"]->IsString() ) {
      strApp = root["app"]->AsString();
   }

   //Attachments
   if( root.find("attachments") != root.end() && root["attachments"]->IsArray() ) {
      JSONArray arrAttachments = root["attachments"]->AsArray();
      for(JSONArray::size_type i=0; i<arrAttachments.size(); i++) {
         JSON* pAttachment = arrAttachments[i];
         if( pAttachment->IsObject() ) {
            const JSONObjects& obj = pAttachment->AsObject();

            std::string strPath;
            if( obj.find("path") != obj.end() && obj.at("path")->IsString() ) {
               strPath = obj.at("path")->AsString();
            }

            std::string strFileName;
            if( obj.find("file_name") != obj.end() && obj.at("file_name")->IsString() ) {
               strFileName = obj.at("file_name")->AsString();
            }

            //image {width, height}

            int nFileSize;
            if( obj.find("file_size") != obj.end() && obj.at("file_size")->IsNumber() ) {
               nFileSize = (int)obj.at("file_size")->AsNumber();
            }

            std::string strContentType;
            if( obj.find("content_type") != obj.end() && obj.at("content_type")->IsString() ) {
               strContentType = obj.at("content_type")->AsString();
            }

            //thumbnail {path, width, height}

            std::string strContentDisposition;
            if( obj.find("content_disposition") != obj.end() && obj.at("content_disposition")->IsString() ) {
               strContentDisposition = obj.at("content_disposition")->AsString();
            }

            //TODO: Save info
         }
      }
   }

   //User
   if( root.find("user") == root.end() || !root["user"]->IsString() )//Looks like a number but is a string
      return NULL;

   std::string strUser = root["user"]->AsString();
   int nUser = atoi(strUser.c_str());

   delete(value);

   ListenResponse* pResponse = new ListenResponse(
      eEvent,
      strThreadId,
      arrAddedTags,
      arrRemovedTags,
      strUUID,
      nID,
      strFlow,
      strContent,
      timeSent,
      strApp,
      bAdded,
      std::vector<std::string>(),//Attachments
      nUser);

   return pResponse;
}

EventType ListenResponse::GetEvent() const
{
   return m_eEvent;
}

std::string ListenResponse::GetContent() const
{
   return m_strContent;
}

int ListenResponse::GetUser() const
{
   return m_nUser;
}

std::string ListenResponse::GetFlow() const
{
   return m_strFlow;
}

std::string ListenResponse::GetThreadId() const
{
   return m_strThreadId;
}

int ListenResponse::GetMessageID() const
{
   return m_nID;
}

bool ListenResponse::IsAdding() const
{
   return m_bAdded;
}

std::vector<std::string> ListenResponse::GetAddedTags() const
{
   return m_astrAddedTags;
}

std::vector<std::string> ListenResponse::GetRemovedTags() const
{
   return m_astrRemovedTags;
}
