#ifndef FLOWDOCKUSERLIST_H
#define FLOWDOCKUSERLIST_H

#include <string>
#include <vector>

class User;

class FlowdockUserList
{
public:
   FlowdockUserList(const std::string& strOrg, const std::string& strFlow, const std::string& strUsername, const std::string& strPassword);
   bool GetUsers(std::vector<User*>& apUsers);

protected:
   static size_t userList_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

protected:
   std::string m_strData;
};


#endif
