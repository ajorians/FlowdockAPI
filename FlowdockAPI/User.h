#ifndef FLOWDOCKAPI_USER_H
#define FLOWDOCKAPI_USER_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h

class User
{
public:
   static User* Create(const std::string& strMessage);

protected:
   User(int nID, const std::string& strNick, const std::string& strEMail, const std::string& strAvatar, const std::string strName, 
                const std::string& strWebsite, bool bDisabled, time_t timeLastActivity, time_t timeLastPing, bool bInFlow, const std::string& strStatus);

public:
   ~User();

   std::string GetIDString() const;
   std::string GetNickname() const;

protected:
   int m_nID;
   std::string m_strNick;
   std::string m_strEMail;
   std::string m_strAvatar;
   std::string m_strName;
   std::string m_strWebsite;
   bool m_bDisabled;
   time_t m_timeLastActivity;
   time_t m_timeLastPing;
   bool m_bInFlow;
   std::string m_strStatus;
};

#endif

