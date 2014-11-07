#ifndef FLOWDOCKAPI_FLOW_H
#define FLOWDOCKAPI_FLOW_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h

class Flow
{
public:
   static Flow* Create(const std::string& strMessage);

protected:
   Flow(const std::string& strID,
      const std::string& strName,
      const std::string& strParameterizedName,
      int nUnreadMentions,
      bool bOpen,
      bool bJoined,
      const std::string& strURL,
      const std::string& strWebURL,
      const std::string& strJoinURL);

public:
   ~Flow();

   std::string GetIDString() const;
   std::string GetParamaterizedName() const;

protected:
   std::string m_strID;
   std::string m_strName;
   std::string m_strParameterizedName;
   //Organization
   int m_nUnreadMentions;
   bool m_bOpen;
   bool m_bJoined;
   std::string m_strURL;
   std::string m_strWebURL;
   std::string m_strJoinURL;
   //AccessMode
};

#endif

