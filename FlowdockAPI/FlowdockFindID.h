#ifndef FLOWDOCKFINDID_H
#define FLOWDOCKFINDID_H

#include <string>
#include <vector>

class Flow;

class FlowdockFindID
{
public:
   FlowdockFindID(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword, const std::string& strFlowID);
   bool GetFlowName(std::string& strFlow);

protected:
   static size_t flowfind_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

protected:
   std::string m_strData;
};


#endif
