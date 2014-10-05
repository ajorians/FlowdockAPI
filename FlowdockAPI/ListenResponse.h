#ifndef FLOWDOCKAPI_LISTENRESPONSE_H
#define FLOWDOCKAPI_LISTENRESPONSE_H

#include <string>
#include <vector>

enum ListenEvent
{
   Message,
   Activity_User
};

class ListenResponse
{
public:
   static ListenResponse* Create(const std::string& strMessage);

protected:
   ListenResponse(ListenEvent eEvent, const std::vector<std::string>& astrTags, const std::string& strUUID, double dID,
                  const std::string& strFlow, const std::string& strContent, double dSent, const std::string& strApp,
                  const std::vector<std::string>& astrAttachments, double dUser);

protected:
   ListenEvent m_eEvent;
   std::vector<std::string> m_astrTags;
   std::string m_strUUID;
   bool m_bPersist;
   double m_dID;
   std::string m_strFlow;
   std::string m_strContent;//Could be vector?
   double m_dSent;//This is a timestamp; TODO: convert it
   std::string m_strApp;
   std::vector<std::string> m_astrAttachments;
   double m_dUser;
};

#endif
