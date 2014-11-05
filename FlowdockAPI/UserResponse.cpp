#include "UserResponse.h"
#include "User.h"
#include "JSON.h"
#include <iostream>

using namespace std;

namespace 
{
   std::string WideToNarrow(const std::wstring& w)
   {
      std::string s( w.begin(), w.end() );
      return s;
   }
}

void UserResponse::Parse(const std::string& strMessage, std::vector<User*>& arrpUsers)
{
   cout << "User response: " << strMessage << endl;
   return;
   /*std::vector<User> arrUsers;
   JSONValue *value = JSON::Parse(strMessage.c_str());
   if( value == NULL )
   {
      cout << "Value is null" << endl;
      return;
   }

   cout << "Get as array" << endl;
   std::vector<std::string> astrUsers;
   JSONArray aUsers = value->AsArray();
   cout << "elements: " << aUsers.size() << endl;
   for(unsigned int i=0; i<aUsers.size(); i++)
   {
      std::string strUser = WideToNarrow(aUsers[i]->Stringify());
      cout << "strUser: " << strUser << endl;
      User* pUser = User::Create(strUser);
      if( pUser )
         arrpUsers.push_back(pUser);
   }*/
}
