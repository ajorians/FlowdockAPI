#ifndef FLOWDOCKFLOWLIST_H
#define FLOWDOCKFLOWLIST_H

#include <string>
#include <vector>

class Flow;

class FlowdockFlowList
{
public:
   FlowdockFlowList(const std::string& strUsername, const std::string& strPassword);
   bool GetFlows(std::vector<Flow*>& apFlows);

protected:
   static size_t flowList_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

protected:
   std::string m_strData;
};


#endif
