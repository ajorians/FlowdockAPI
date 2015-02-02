#ifndef FLOWDOCKAPI_FLOWDOCKAPI_H
#define FLOWDOCKAPI_FLOWDOCKAPI_H

#ifdef WIN32
#define FLOWDOCK_EXTERN	extern "C" __declspec(dllexport)
#else
#define FLOWDOCK_EXTERN extern "C"
#endif

typedef void*	FlowdockAPI;

typedef int (*FlowdockCreateFunc)(FlowdockAPI* api);
typedef int (*FlowdockFreeFunc)(FlowdockAPI* api);
typedef int (*FlowdockSetOrgFlowFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
typedef int (*FlowdockSetUsernamePasswordFunc)(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

typedef int (*FlowdockSayFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
typedef int (*FlowdockSayOrgFlowMessageFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
typedef int (*FlowdockSayDefaultsFunc)(FlowdockAPI api, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);

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

typedef int (*FlowdockGetListenMessageCountFunc)(FlowdockAPI api);
typedef int (*FlowdockGetListenMessageTypeFunc)(FlowdockAPI api, int nIndex);
typedef int (*FlowdockGetMessageContentFunc)(FlowdockAPI api, int nIndex, char* pstrMessage, int& nSizeOfMessage);
typedef int (*FlowdockGetMessageUserFunc)(FlowdockAPI api, int nIndex, char* pstrMessageUser, int& nSizeOfMessageUser);
typedef int (*FlowdockGetMessageFlowFunc)(FlowdockAPI api, int nIndex, char* pstrMessageFlow, int& nSizeOfMessageFlow);
typedef int (*FlowdockGetMessageIDFunc)(FlowdockAPI api, int nIndex, int& nMessageID);
typedef int (*FlowdockRemoveListenMessageFunc)(FlowdockAPI api, int nIndex);//Hopefully nIndex is 0 or you are aware of threading issues

typedef int (*FlowdockGetNicknameForUserFunc)(FlowdockAPI api, char* pstrUser, char* pstrNickname, int& nSizeOfNickname);

typedef int (*FlowdockGetFlowByIDFunc)(FlowdockAPI api, char* pstrID, char* pstrFlowName, int& nSizeOfFlowName);

FLOWDOCK_EXTERN int FlowdockCreate(FlowdockAPI* api);
FLOWDOCK_EXTERN int FlowdockFree(FlowdockAPI* api);
FLOWDOCK_EXTERN int FlowdockSetOrgFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
FLOWDOCK_EXTERN int FlowdockSetUsernamePassword(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

FLOWDOCK_EXTERN int FlowdockSay(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
FLOWDOCK_EXTERN int FlowdockSayOrgFlowMessage(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);
FLOWDOCK_EXTERN int FlowdockSayDefaults(FlowdockAPI api, const char* pstrMessage, int nCommentTo, const char* pstrTags, const char* pstrExternalUserName);

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

FLOWDOCK_EXTERN int FlowdockGetListenMessageCount(FlowdockAPI api);
FLOWDOCK_EXTERN int FlowdockGetListenMessageType(FlowdockAPI api, int nIndex);
FLOWDOCK_EXTERN int FlowdockGetMessageContent(FlowdockAPI api, int nIndex, char* pstrMessage, int& nSizeOfMessage);
FLOWDOCK_EXTERN int FlowdockGetMessageUser(FlowdockAPI api, int nIndex, char* pstrMessageUser, int& nSizeOfMessageUser);
FLOWDOCK_EXTERN int FlowdockGetMessageFlow(FlowdockAPI api, int nIndex, char* pstrMessageFlow, int& nSizeOfMessageFlow);
FLOWDOCK_EXTERN int FlowdockGetMessageID(FlowdockAPI api, int nIndex, int& nMessageID);
FLOWDOCK_EXTERN int FlowdockRemoveListenMessage(FlowdockAPI api, int nIndex);//Hopefully nIndex is 0 or you are aware of threading issues

FLOWDOCK_EXTERN int FlowdockGetNicknameForUser(FlowdockAPI api, char* pstrUser, char* pstrNickname, int& nSizeOfNickname);

FLOWDOCK_EXTERN int FlowdockGetFlowByID(FlowdockAPI api, char* pstrID, char* pstrFlowName, int& nSizeOfFlowName);

#endif
