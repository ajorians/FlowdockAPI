#ifndef FLOWDOCKAPI_FLOWRESPONSE_H
#define FLOWDOCKAPI_FLOWRESPONSE_H

#include <string>
#include <vector>
#include <ctime>//Maybe time.h

class Flow;

class FlowResponse
{
public:
   static bool Parse(const std::string& strMessage, std::vector<Flow*>& arrpFlows);
};

#endif

