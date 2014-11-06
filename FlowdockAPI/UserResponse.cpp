#include "UserResponse.h"
#include "User.h"
#include "JSON.h"
#include <iostream>

using namespace std;

bool UserResponse::Parse(const std::string& strMessage, std::vector<User*>& arrpUsers)
{
   cout << "User response: " << strMessage << endl;
   std::vector<User> arrUsers;
   JSON *value = JSON::Parse(strMessage.c_str());
   if( value == NULL )
   {
      cout << "Value is null" << endl;
      return false;
   }

   cout << "Get as array" << endl;
   std::vector<std::string> astrUsers;
   JSONArray aUsers = value->AsArray();
   cout << "elements: " << aUsers.size() << endl;
   for(unsigned int i=0; i<aUsers.size(); i++)
   {
      std::string strUserObject = aUsers[i]->Stringify();
      cout << "strUser: " << strUserObject << endl;
      User* pUser = User::Create(strUserObject);
      if( pUser )
         arrpUsers.push_back(pUser);
   }
   return true;
}
