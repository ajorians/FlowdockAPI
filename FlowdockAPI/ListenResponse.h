#ifndef FLOWDOCKAPI_LISTENRESPONSE_H
#define FLOWDOCKAPI_LISTENRESPONSE_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h
#include "FlowdockAPI.h"

class ListenResponse
{
public:
   static ListenResponse* Create(const std::string& strMessage);

   EventType GetEvent() const;
   std::string GetContent() const;
   int GetUser() const;
   std::string GetFlow() const;
   int GetMessageID() const;

protected:
   ListenResponse(EventType eEvent, const std::string& strThreadId, const std::vector<std::string>& astrTags, const std::string& strUUID, int nID,
                  const std::string& strFlow, const std::string& strContent, time_t timeSent, const std::string& strApp,
                  const std::vector<std::string>& astrAttachments, int nUser);

protected:
   EventType m_eEvent;
   std::string m_strThreadId;
   std::vector<std::string> m_astrTags;
   std::string m_strUUID;
   bool m_bPersist;
   int m_nID;
   std::string m_strFlow;
   std::string m_strContent;//Could be vector?
   time_t m_timeSent;
   std::string m_strApp;
   std::vector<std::string> m_astrAttachments;
   int m_nUser;
};

#endif

