#ifndef FLOWDOCKAPI_LISTENRESPONSE_H
#define FLOWDOCKAPI_LISTENRESPONSE_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h

enum ListenEvent
{
   Message,
   Comment,//These are the thread response messages
   Activity_User,
   Tag_Change
};

class ListenResponse
{
public:
   static ListenResponse* Create(const std::string& strMessage);

   ListenEvent GetEvent() const;
   std::string GetContent() const;
   int GetUser() const;
   std::string GetFlow() const;
   int GetMessageID() const;

protected:
   ListenResponse(ListenEvent eEvent, const std::vector<std::string>& astrTags, const std::string& strUUID, double dID,
                  const std::string& strFlow, const std::string& strContent, time_t timeSent, const std::string& strApp,
                  const std::vector<std::string>& astrAttachments, int nUser);

protected:
   ListenEvent m_eEvent;
   std::vector<std::string> m_astrTags;
   std::string m_strUUID;
   bool m_bPersist;
   double m_dID;
   std::string m_strFlow;
   std::string m_strContent;//Could be vector?
   time_t m_timeSent;
   std::string m_strApp;
   std::vector<std::string> m_astrAttachments;
   int m_nUser;
};

#endif

