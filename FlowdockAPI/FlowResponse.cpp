#include "FlowResponse.h"
#include "Flow.h"
#include "JSON.h"
#include <iostream>

using namespace std;

bool FlowResponse::Parse(const std::string& strMessage, std::vector<Flow*>& arrpFlows)
{
   JSON *value = JSON::Parse(strMessage.c_str());
   if( value == NULL )
   {
      cout << "Value is null" << endl;
      return false;
   }

   std::vector<std::string> astrFlows;
   JSONArray aFlows = value->AsArray();
   for(unsigned int i=0; i<aFlows.size(); i++)
   {
      std::string strFlowObject = aFlows[i]->Stringify();
      Flow* pFlow = Flow::Create(strFlowObject);
      if( pFlow )
         arrpFlows.push_back(pFlow);
   }
   return arrpFlows.size() > 0;
}
