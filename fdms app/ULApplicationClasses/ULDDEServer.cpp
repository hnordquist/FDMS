// ULDDEServer.cpp: implementation of the ULDDEServer class.
// $History: ULDDEServer.cpp $ 
// 
// *****************  Version 5  *****************
// User: 186846       Date: 12/18/07   Time: 4:34p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// VS 2005 changes completed
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\FDMS.h" //REQT:  Change this to use the new application header.
#include "ULDDEServer.h"
#include "rtcoms.h"
#include "ULSupport.h"

#include <afxtempl.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static FARPROC ddecb;  //Should be in the class.  This is the callback function for the server.
static HSZ ULServerName = NULL;
//static HSZ ULServerTopic = NULL;  //this server only handles multiple topics.
static CList<HSZ, HSZ&> ULServerTopicList;
static UINT muiServerCount = 0;
static 	ULCallBack *m_pCB_Obj = NULL;
static	UL_CB_RequestServerFn m_pCBO_RequestServerFn = NULL;
static	UL_CB_ExecuteServerFn m_pCBO_ExecuteServerFn = NULL;
static DWORD  dde_inst = 0;  //from DDE book example

//********************************************************************************************************
// dde_cbk:   Server Callback function ; This function does the bulk of the work of the server.  There are
//specific communication types that are listened for and handled by this callback function for the
//the server "service" that is provided.
//******************************************************************************************************
HDDEDATA
//FAR PASCAL _export 
dde_cbk(UINT type, UINT /*fmt*/,
		HCONV /*conv*/, HSZ hsz1 /*the topic*/, HSZ hsz2 /*the command or request*/,
		HDDEDATA data, DWORD /*data1*/, DWORD /*data2*/)
{
	CString ReturnData;
	char RequestStr[100] = "";
	char TopicStr[100] = "";
	//	char DataStr[100] = "";
	CString Command;
	bool RetVal = FALSE; //indicates failed connect.

	HDDEDATA ReturnDDEData;

	switch (type)
	{
	case XTYP_ERROR:
		{
			break;  //not handled by this server.
		}

	case XTYP_ADVSTART:
		break;  //not handled by this server.

	case XTYP_ADVREQ:
		break;  //not handled by this server.
	case XTYP_REQUEST:
		// Return data 
		TRACE0("SERVER REQUEST received \n");
		if(hsz1 == NULL || hsz2 == NULL)
			TRACE0("SERVER REQUEST received NULL data\n");
		DdeQueryString(dde_inst, hsz1, TopicStr, 100, CP_WINANSI);  //hsz1 - contains the topic
		DdeQueryString(dde_inst, hsz2, RequestStr, 100, CP_WINANSI);  //hsz2 - contains the request


		//If this is not a EXIT message, then post a message to this application to process 
		//the newly received REQUEST.  PIPE_EXIT_ALL processing is handled else where
		//and does not require a posting to the window queue.
		if(strcmp(RequestStr, szPIPE_EXIT_ALL) != 0) //This is not an PIPE_EXIT_ALL message.
		{
			RetVal = (m_pCB_Obj->*m_pCBO_RequestServerFn)(RequestStr, &ReturnData);
			TRACE2("Callback received REQUEST from : Topic %s and Request %s\n", TopicStr, RequestStr); 
			TRACE0("Posting REQUEST message to the app message queue\n");
			AfxGetMainWnd()->PostMessage(((CFDMSApp*)AfxGetApp())->m_ProcessUpperLayerMsgQueueMsg, 0, 0L);
		}
		else //We have a PIPE_EXIT_ALL message.  Just handle here to shut down the application.
		{
			((CFDMSApp*)AfxGetApp())->m_pRT_Review->RT_Review_My_Exit();
			ReturnData = RequestStr;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
		}

		//Attempt to send data back to a requesting application.
		HSZ DataItem;
		DataItem = DdeCreateStringHandle(dde_inst, ReturnData, CP_WINANSI);
		ReturnDDEData = DdeCreateDataHandle(dde_inst, (unsigned char *)(LPCTSTR)ReturnData, strlen(ReturnData), 0, DataItem, CF_TEXT, 0);
		return ReturnDDEData;


	case XTYP_CONNECT:

		TRACE0("SERVER CONNECT received \n");

		//If the server name is this server, then see if the topic is this server topic.
		if(hsz1 == NULL || hsz2 == NULL)
			TRACE0("SERVER CONNECT received NULL data\n");

		DdeQueryString(dde_inst, hsz1, TopicStr, 100, CP_WINANSI);  //hsz1 - contains the topic
		DdeQueryString(dde_inst, hsz2, RequestStr, 100, CP_WINANSI);  //hsz2 - contains the request

		TRACE2("Callback received CONNECT from : Topic %s and Request %s\n", TopicStr, RequestStr); 
		if (!DdeCmpStringHandles(ULServerName,hsz2))
		{
			//See if the requested topic is a topic supported by this server.
			//If so, return successful connect flag.
			HSZ  Topic;
			int NumRecs = ULServerTopicList.GetCount();
			POSITION Pos = ULServerTopicList.GetHeadPosition();
			//Check each topic in the list to see if it matches the one given by the client.
			for(int i = 0; i < NumRecs && !RetVal; i++)
			{
				Topic = ULServerTopicList.GetNext(Pos);
				if (!DdeCmpStringHandles(Topic,hsz1))
				{
					TRACE0("SUCCESSFUL CONNECT TO THE SERVER\n");
					RetVal = TRUE;
				}
			}//end FOR each server topic
		}//end IF server name matches this server

		//If execution drops to this point, either the server name or topic name were not supported.
		return RetVal ? (HDDEDATA)1 : (HDDEDATA)0;

	case XTYP_EXECUTE:
		TRACE0("SERVER RECEIVED EXECUTE COMMAND\n");
		if(hsz1 == NULL || data == NULL)
			TRACE0("SERVER EXECUTE received NULL data\n");

		DdeQueryString(dde_inst, hsz1, TopicStr, 100, CP_WINANSI);  //hsz1 - contains the topic

		if(data != NULL)
		{
			Command = DdeAccessData(data, NULL); //not interested in the length of the string.
			DdeUnaccessData(data);
		}
		else if(data == NULL || hsz1 == NULL)
			TRACE0("EXECUTE did not receive a DATA or HSZ1 variable\n");


		TRACE2("Callback received EXECUTE with Topic %s and COMMAND =  %s\n", TopicStr, Command); 
		RetVal = (m_pCB_Obj->*m_pCBO_ExecuteServerFn)(Command);

		//Post a message to this application to process the newly received REQUEST.
		//		TRACE0("Posting EXECUTE message to the app message queue\n");
		//		AfxGetMainWnd()->PostMessage( theApp.m_ProcessUpperLayerMsgQueueMsg, 0, 0L);

		//Execute must return a FACK, FBUSY, or FNOTPROCESSED indicator.  //TO BE IMPLEMENTED LATER.
		return ((HDDEDATA)DDE_FACK);


	case XTYP_POKE:
		break; //not handled by this server.

	case XTYP_WILDCONNECT:
		//Do not allow wild connects. 
		//A "wild connect" is when a client is just connecting to every available service.
		break;
	case XTYP_XACT_COMPLETE:	
		TRACE0("Received XACT_COMPLETE\n");
		break;
	case XTYP_CONNECT_CONFIRM:
		TRACE0("Received CONNECT_CONFIRM\n");
		break;

	} //end switch statement

	return NULL;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ULDDEServer::ULDDEServer(
						 const char *szServerName,
						 BOOL *pSuccess,
						 CString *pErrMsg)
{
	mbValid = true;      
	pErrMsg->Empty();

	//If the server has not been created yet, then initialize it/create it.  If it has already been initialized,
	//then just increment the server counter.  
	if (muiServerCount == 0)   //Server not yet created.
	{
		int RetVal = 0;

		ddecb=MakeProcInstance((FARPROC)dde_cbk,hInst);

		//Attempt to initialize the DDE support system layer.  If successful, set flag to indicate server count is 1.
		//If the initialization fails, notify the user and display the low-level DDE error that occurred.
		if ((RetVal = DdeInitialize(&dde_inst,(PFNCALLBACK)ddecb, 
			APPCLASS_STANDARD|CBF_SKIP_ALLNOTIFICATIONS,0))	== DMLERR_NO_ERROR) 
		{
			muiServerCount = 1;

			//Set the server name for the class.
			ULServerName = DdeCreateStringHandle(dde_inst, szServerName, CP_WINANSI);	

			TRACE0("SERVER STARTED SUCCESSFULLY\n");
		}
		else //DDEInitiailze() call failed.  Notify the user of the user.
		{
			MessageBox(NULL, "DDE Server initialization failed.", "Error Message", MB_OK|MB_ICONERROR);
			mbValid = false;

			ULSupport ULSupt;
			ULSupt.ReportDDEError("DdeInitialize (SERVER)", dde_inst);
		}
	}    
	else //Server has already been created, so increment the server counter..
		muiServerCount++;

	//Go into a loop to count out 5 seconds; mimicking previous behavior (15 seconds used before) <<temporary patch>> KDM
	//	int StartTime = GetTickCount();
	//	while(GetTickCount() - StartTime < 5000);

	//Determine if the caller has passed in a pointer to receive a Success/Failure status.  If so, then set it.
	if (pSuccess != NULL) 
		*pSuccess = mbValid;
}

ULDDEServer::~ULDDEServer()
{
	//Do not shut down the server completely until the last instance of it is to be destroyed.
	muiServerCount--;
	if(muiServerCount == 0)
	{
		//Unregister the server service from the system.  This ensures it is not available to any clients.
		DdeNameService(dde_inst,ULServerName,NULL,DNS_UNREGISTER);

		//Release the string resources for the DDE server.
		DdeFreeStringHandle(dde_inst, ULServerName);  

		//Free each topic string handle that was allocated within the DDE system.
		HSZ  Topic;
		int NumRecs = ULServerTopicList.GetCount();
		POSITION Pos = ULServerTopicList.GetHeadPosition();
		for(int i = 0; i < NumRecs; i++)
		{
			Topic = ULServerTopicList.GetNext(Pos);
			DdeFreeStringHandle(dde_inst, Topic);  
		}
	}
}

bool ULDDEServer::CreateTopic(const char *szTopicName, // Input: topic name known to prospective client
							  bool (*pRequestServerFn)(CString ItemName, CString* pReturnData),
							  bool (*pExecuteServerFn)(CString Cmd))
{
	bool bSuccess = false;

	//If the DDE system was successfully initialized, then start the service.
	if (dde_inst != NULL)
	{
		//Set the topic for the server.
		HSZ ULServerTopic = DdeCreateStringHandle(dde_inst, szTopicName, CP_WINANSI);	
		ULServerTopicList.AddTail(ULServerTopic);

		//Set the REQUEST and EXECUTE function callbacks for the server.
		m_pRequestServerFn = pRequestServerFn;
		m_pExecuteServerFn = pExecuteServerFn;
		m_pCB_Obj = NULL;    //Callback object
		m_pCBO_RequestServerFn = NULL;
		m_pCBO_ExecuteServerFn = NULL;

		//Call DDENAMESERVICE to actually start the server service.
		//If the DdeNameService returns a non-zero value, the service registered successfully.
		if(DdeNameService(dde_inst,ULServerName,NULL,DNS_REGISTER)) 
			bSuccess = true;
		else
		{
			bSuccess = false;

			ULSupport ULSupt;
			ULSupt.ReportDDEError("DdeNameService (Start server)", dde_inst);
		}
	}//end IF dde_inst != NULL

	return(bSuccess);
}




bool ULDDEServer::CreateTopic(
							  const char *szTopicName,	// I Topic name known to prospective client.
							  ULCallBack *pCB_Obj,
							  UL_CB_RequestServerFn pRequestServerFn,
							  UL_CB_ExecuteServerFn pExecuteServerFn)
{
	bool bSuccess = false;

	//If the DDE system was successfully initialized, then start the service.
	if (dde_inst != NULL)
	{
		//Set the topic for the server.
		HSZ ULServerTopic = DdeCreateStringHandle(dde_inst, szTopicName, CP_WINANSI);	
		ULServerTopicList.AddTail(ULServerTopic);

		//Set the REQUEST and EXECUTE function callbacks for the server.
		m_pRequestServerFn = NULL;
		m_pExecuteServerFn = NULL;
		m_pCB_Obj = pCB_Obj;     //Callback object
		m_pCBO_RequestServerFn = pRequestServerFn;
		m_pCBO_ExecuteServerFn = pExecuteServerFn;

		//Call DDENAMESERVICE to actually start the server service.
		//If the DdeNameService returns a non-zero value, the service registered successfully.
		if(DdeNameService(dde_inst,ULServerName,NULL,DNS_REGISTER)) 
			bSuccess = true;
		else
		{
			bSuccess = false;

			ULSupport ULSupt;
			ULSupt.ReportDDEError("DdeNameService (Start server)", dde_inst);
		}

	}//end IF dde_inst != NULL

	return(bSuccess);
}

bool ULDDEServer::FindCmd(
						  CString FullCmd,	// I Cmd recieved by topic execute service fn.
						  char *naszCmds[],	// I NULL-terminated list of possible cmds.
						  UINT *puiCmdIndex,	// O Index of matching cmd if found.
						  CString *pCmdArgs,	// O Optional command args.
						  BOOL bCaseSensitive)	// Optional; default = FALSE.
{
	//	UINT uiDim = FullCmd.GetDim();
	UINT uiDim = FullCmd.GetLength();
	char *szFullCmd = new char[uiDim];
	UINT uiIndex = 0;
	bool bCmdFound = false;

	if (szFullCmd != NULL) {
		FullCmd.Delete(0);	// remove leading '['.
		strcpy(szFullCmd, FullCmd);
		while ((!bCmdFound) && (naszCmds[uiIndex] != NULL)) {
			const char *szCmd = naszCmds[uiIndex];
			size_t CmdLen = strlen(szCmd);
			BOOL bSame;

			if (bCaseSensitive) {
				bSame = strncmp(szCmd, szFullCmd, CmdLen) == 0;
			}
			else {
				bSame = _strnicmp(szCmd, szFullCmd, CmdLen) == 0;
			}

			if (bSame) {
				*puiCmdIndex = uiIndex;
				bCmdFound = TRUE;
				if (pCmdArgs != NULL) {
					// Copy remainder of FullCmd (after command portion) to args.
					size_t FullCmdLen = strlen(szFullCmd);
					if (szFullCmd[FullCmdLen - 1] == ']') {
						szFullCmd[FullCmdLen - 1] = '\0';
					}

					*pCmdArgs = &szFullCmd[CmdLen];

					// Strip any white space.
					//					pCmdArgs->StripLeadAndTrail();
					pCmdArgs->Remove(' ');
				}
			}
			uiIndex++;
		}

		delete [] szFullCmd;
	}
	return(bCmdFound);
}


