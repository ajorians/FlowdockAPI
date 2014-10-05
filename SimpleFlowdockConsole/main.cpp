#include <iostream>
#include <string>
#include <fstream>
#include "FlowdockAPI.h"
#include "Library.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifndef WIN32
#include <unistd.h>//For usleep
#include <limits.h>//For PATH_MAX
#endif

using namespace std;

int main(int argc, char *argv[])
{
   bool bListen = false;
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

      if( str == "--listen" )
         bListen = true;
   }
   if( argc < 2 || strOrg.length() == 0 || strFlow.length() == 0 || strUser.length() == 0 || strPassword.length() == 0 )
   {
      cout << "Usage: " << argv[0] << " --org aj-org --flow main --user ajorians@gmail.com --password abc123" << endl;
      cout << "--upload file or --say message or --listen" << endl;
      return 0;
   }

   std::string strPath;
#ifdef WIN32
   strPath = "..\\..\\FlowdockAPI\\Debug\\FlowdockAPI.dll"
#else
   char path[PATH_MAX] = "/proc/self/exe";
   char dest[PATH_MAX];
   readlink(path, dest, PATH_MAX);
   std::string strEXE(dest);
   strEXE = strEXE.substr(0, strEXE.rfind('/'));//EXE folder
   cout << "EXE: " << strEXE << endl;
   strEXE = strEXE.substr(0, strEXE.rfind('/'));//Build folder
   cout << "EXE: " << strEXE << endl;

   strPath = strEXE + "/FlowdockAPI/libFlowdockAPI.so";

#endif
   RLibrary library(strPath);

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

   if( bListen )
   {
      FlowdockAddListenFlowFunc AddListenFlow = (FlowdockAddListenFlowFunc)library.Resolve("FlowdockAddListenFlow");
      if( !AddListenFlow )
         return 0;

      AddListenFlow(pFlowdock, "aj-org", "main");

      FlowdockStartListeningFunc StartListening = (FlowdockStartListeningFunc)library.Resolve("FlowdockStartListening");
      if( !StartListening )
         return 0;

      StartListening(pFlowdock, strUser.c_str(), strPassword.c_str());
      //Sleep while it is listening
      for(int i=0; i<10; i++)
      {
#ifdef _WIN32
         Sleep(1000);//1 second
#else
         usleep(1000*1000);
#endif
      }
   }

   ///

   FlowdockFreeFunc FreeAPI = (FlowdockFreeFunc)library.Resolve("FlowdockFree");
   if( !FreeAPI )
      return 0;

   FreeAPI(&pFlowdock);

   return 0;
}
