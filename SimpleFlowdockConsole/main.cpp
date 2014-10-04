#include <iostream>
#include <string>
#include <fstream>
#include "FlowdockAPI.h"
#include "Library.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

int main(int argc, char *argv[])
{
   std::string strOrg, strFlow, strUser, strPassword, strUploadFile, strMessage;
   for(int i=0; i<argc; i++)
   {
      std::string str = argv[i];
      if( str == "--org" )
         strOrg = argv[i+1];

      if( str == "--flow" )
         strFlow = argv[i+1];

      if( str == "--user" )
         strUser = argv[i+1];

      if( str == "--password" )
         strPassword = argv[i+1];

      if( str == "--upload" )
         strUploadFile = argv[i+1];

      if( str == "--say" )
         strMessage = argv[i+1];
   }
   if( argc < 2 || strOrg.length() == 0 || strFlow.length() == 0 || strUser.length() == 0 || strPassword.length() == 0 )
   {
      cout << "Usage: " << argv[0] << " --org aj-org --flow main --user ajorians@gmail.com --password abc123" << endl;
      cout << "--upload file or --say message" << endl;
      return 0;
   }

   RLibrary library(
#ifdef WIN32
      "..\\..\\FlowdockAPI\\Debug\\FlowdockAPI.dll"
#else
      "/home/ajorians/Documents/git/FlowdockAPI/Build/FlowdockAPI/libFlowdockAPI.so"
#endif
      );

   if( !library.Load() )
      return 0;

   FlowdockCreateFunc CreateAPI = (FlowdockCreateFunc)library.Resolve("FlowdockCreate");
   if( !CreateAPI )
      return 0;

   FlowdockAPI pFlowdock = NULL;
   CreateAPI(&pFlowdock);

   ///

   if( strMessage.length() > 0 )
   {
      FlowdockSayFunc Say = (FlowdockSayFunc)library.Resolve("FlowdockSay");
      if( !Say )
         return 0;

      Say(pFlowdock, strOrg.c_str(), strFlow.c_str(), strUser.c_str(), strPassword.c_str(), strMessage.c_str());
   }

   ///

   if( strUploadFile.length() > 0 )
   {
      FlowdockUploadFileFunc Upload = (FlowdockUploadFileFunc)library.Resolve("FlowdockUploadFile");
      if( !Upload )
         return 0;

      Upload(pFlowdock, strOrg.c_str(), strFlow.c_str(), strUser.c_str(), strPassword.c_str(), strUploadFile.c_str());
   }

   ///

   FlowdockFreeFunc FreeAPI = (FlowdockFreeFunc)library.Resolve("FlowdockFree");
   if( !FreeAPI )
      return 0;

   FreeAPI(&pFlowdock);

   return 0;
}

