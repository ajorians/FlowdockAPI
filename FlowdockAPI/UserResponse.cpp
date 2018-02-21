#include "UserResponse.h"
#include "User.h"
#include "JSON.h"
#include <iostream>

using namespace std;

bool UserResponse::Parse(const std::string& strMessage, std::vector<User*>& arrpUsers)
{
   JSON *value = JSON::Parse(strMessage);
   if( value == NULL )
   {
      cout << "Value is null" << endl;
      return false;
   }

   std::vector<std::string> astrUsers;
   JSONArray aUsers = value->AsArray();
   for(unsigned int i=0; i<aUsers.size(); i++)
   {
      std::string strUserObject = aUsers[i]->Stringify();
      User* pUser = User::Create(strUserObject);
      if( pUser )
         arrpUsers.push_back(pUser);
   }
   return true;
}
