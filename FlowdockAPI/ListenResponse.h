#ifndef FLOWDOCKAPI_LISTENRESPONSE_H
#define FLOWDOCKAPI_LISTENRESPONSE_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h

enum ListenEvent
{
   Message,
   Activity_User
};

class ListenResponse
{
public:
   static ListenResponse* Create(const std::string& strMessage);

   ListenEvent GetEvent() const;
   std::string GetContent() const;

protected:
   ListenResponse(ListenEvent eEvent, const std::vector<std::string>& astrTags, const std::string& strUUID, double dID,
                  const std::string& strFlow, const std::string& strContent, time_t timeSent, const std::string& strApp,
                  const std::vector<std::string>& astrAttachments, const std::string& strUser);

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
   std::string m_strUser;
};

#endif

