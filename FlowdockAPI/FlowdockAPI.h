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
typedef int (*FlowdockSayFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage);
typedef int (*FlowdockUploadFileFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrFilePath);

typedef int (*FlowdockIsListeningFunc)(FlowdockAPI api);
typedef int (*FlowdockStopListeningFunc)(FlowdockAPI api);
typedef int (*FlowdockAddListenFlowFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
typedef int (*FlowdockRemoveListenFlowFunc)(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
typedef int (*FlowdockStartListeningFunc)(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

FLOWDOCK_EXTERN int FlowdockCreate(FlowdockAPI* api);
FLOWDOCK_EXTERN int FlowdockFree(FlowdockAPI* api);
FLOWDOCK_EXTERN int FlowdockSay(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrMessage);
FLOWDOCK_EXTERN int FlowdockUploadFile(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow, const char* pstrUsername, const char* pstrPassword, const char* pstrFilePath);

FLOWDOCK_EXTERN int FlowdockIsListening(FlowdockAPI api);
FLOWDOCK_EXTERN int FlowdockStopListening(FlowdockAPI api);//Can be a tad slow; timeout is currently 60 seconds
FLOWDOCK_EXTERN int FlowdockAddListenFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
FLOWDOCK_EXTERN int FlowdockRemoveListenFlow(FlowdockAPI api, const char* pstrOrg, const char* pstrFlow);
FLOWDOCK_EXTERN int FlowdockStartListening(FlowdockAPI api, const char* pstrUsername, const char* pstrPassword);

#endif
