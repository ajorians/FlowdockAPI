#ifndef FLOWDOCKAPI_FLOWDOCKAPI_H
#define FLOWDOCKAPI_FLOWDOCKAPI_H

#ifdef WIN32
#define FLOWDOCK_EXTERN	extern "C" __declspec(dllexport)
#else
#define FLOWDOCK_EXTERN extern "C"
#endif

enum EventType
{
   Message,
   MessageEdit,
   Comment,//These are the thread response messages
   Activity_User,
   Tag_Change,
   Message_Delete,
   Thread_Change
};

typedef struct
{
   EventType eEvent;
   char Message[512];
   int nUserId;
   int nThreadId;
} FlowMessage;

typedef void(*FlowMessageCallback)(FlowMessage message, void* pUserData);

typedef void*	FlowdockAPI;

typedef int (*FlowdockCreateFunc)(FlowdockAPI* api, int nVerbose);
typedef int (*FlowdockFreeFunc)(FlowdockAPI* api);
typedef int (*FlowdockSetOrgFlowFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
typedef int (*FlowdockSetUsernamePasswordFunc)(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

typedef int (*FlowdockSayFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
typedef int (*FlowdockSayOrgFlowMessageFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
typedef int (*FlowdockSayDefaultsFunc)(FlowdockAPI api, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);

typedef int( *FlowdockTagFunc )( FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, int nCommentTo, const char* pstrTags );

typedef int (*FlowdockUploadFileFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrFilePath);
typedef int (*FlowdockUploadOrgFlowFileFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrFilePath);
typedef int (*FlowdockUploadFileDefaultsFunc)(FlowdockAPI api, const char* pstrFilePath);

typedef int (*FlowdockGetUsersFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword);
typedef int (*FlowdockGetFlowsFunc)(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

typedef int (*FlowdockIsListeningFunc)(FlowdockAPI api);
typedef int (*FlowdockStopListeningFunc)(FlowdockAPI api);
typedef int (*FlowdockAddListenFlowFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
typedef int (*FlowdockRemoveListenFlowFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
typedef int (*FlowdockStartListeningFunc)(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);
typedef int (*FlowdockStartListeningDefaultsFunc)(FlowdockAPI api);

typedef int (*FlowdockAddListenCallbackFunc)(FlowdockAPI api, FlowMessageCallback cb, void* pUserData);

typedef int (*FlowdockGetNicknameForUserFunc)(FlowdockAPI api, int nUser, char* pstrNickname, int& nSizeOfNickname);
typedef int (*FlowdockGetEMailForUserFunc)(FlowdockAPI api, int nUser, char* pstrEmailAddress, int& nSizeOfEMailAddress);

typedef int (*FlowdockGetFlowByIDFunc)(FlowdockAPI api, char* pstrID, char* pstrFlowName, int& nSizeOfFlowName);

FLOWDOCK_EXTERN int FlowdockCreate(FlowdockAPI* api, int nVerbose);
FLOWDOCK_EXTERN int FlowdockFree(FlowdockAPI* api);
FLOWDOCK_EXTERN int FlowdockSetOrgFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
FLOWDOCK_EXTERN int FlowdockSetUsernamePassword(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

FLOWDOCK_EXTERN int FlowdockSay(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
FLOWDOCK_EXTERN int FlowdockSayOrgFlowMessage(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
FLOWDOCK_EXTERN int FlowdockSayDefaults(FlowdockAPI api, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);

FLOWDOCK_EXTERN int FlowdockTag( FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, int nCommentTo, const char* pstrTags );

FLOWDOCK_EXTERN int FlowdockUploadFile(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrFilePath);
FLOWDOCK_EXTERN int FlowdockUploadOrgFlowFile(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrFilePath);
FLOWDOCK_EXTERN int FlowdockUploadFileDefaults(FlowdockAPI api, const char* pstrFilePath);

FLOWDOCK_EXTERN int FlowdockGetUsers(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword);
FLOWDOCK_EXTERN int FlowdockGetFlows(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

FLOWDOCK_EXTERN int FlowdockIsListening(FlowdockAPI api);
FLOWDOCK_EXTERN int FlowdockStopListening(FlowdockAPI api);//Can be a tad slow; timeout is currently 60 seconds
FLOWDOCK_EXTERN int FlowdockAddListenFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
FLOWDOCK_EXTERN int FlowdockRemoveListenFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
FLOWDOCK_EXTERN int FlowdockStartListening(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);
FLOWDOCK_EXTERN int FlowdockStartListeningDefaults(FlowdockAPI api);

FLOWDOCK_EXTERN int FlowdockAddListenCallback(FlowdockAPI api, FlowMessageCallback cb, void* pData);

FLOWDOCK_EXTERN int FlowdockGetNicknameForUser(FlowdockAPI api, int nUser, char* pstrNickname, int& nSizeOfNickname);
FLOWDOCK_EXTERN int FlowdockGetEMailForUser(FlowdockAPI api, int nUser, char* pstrEmailAddress, int& nSizeOfEMailAddress);

FLOWDOCK_EXTERN int FlowdockGetFlowByID(FlowdockAPI api, char* pstrID, char* pstrFlowName, int& nSizeOfFlowName);

#endif
