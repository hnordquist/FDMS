/*
Copyright (c) 2014, Los Alamos National Security, LLC
All rights reserved.
Copyright 2014. Los Alamos National Security, LLC. This software was produced under U.S. Government contract 
DE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security, 
LLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.  
NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, 
OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works, 
such modified software should be clearly marked, so as not to confuse it with the version available from LANL.

Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided 
that the following conditions are met:
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
•	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
nor the names of its contributors may be used to endorse or promote products derived from this software without specific 
prior written permission. 
THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// ULDDEClient.cpp: implementation of the ULDDEClient class.
// $History: ULDDEClient.cpp $ 
// 
// *****************  Version 5  *****************
// User: Longo        Date: 11/24/04   Time: 3:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// compiler warning removal
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/19/04   Time: 12:03p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// housekeeping
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULDDEClient.h"
#include "ULSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static UINT muiClientCount = 0;
static DWORD  dde_inst = 0;  //from DDE book example

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ULDDEClient::ULDDEClient(const char *szServerName, 	// I; Identification "published" by server.
						 const char *szTopicName,	// I; Server-supported topic name.
						 BOOL *pSuccess,			// O; TRUE=>DDE conversation begun.
						 CString *pErrMsg)
{
	UINT RetVal = DMLERR_NO_ERROR ;

	//	dde_inst = 0; //initialize the client instance handle.
	*pErrMsg = "";
	mbValid = FALSE;      
	mpConversation = NULL;
	mhInst = NULL;
	mServerName = szServerName;

	if (muiClientCount == 0) 
	{
		if((RetVal = DdeInitialize(&dde_inst, NULL, APPCLASS_STANDARD | APPCMD_CLIENTONLY | CBF_SKIP_ALLNOTIFICATIONS, 0)) == DMLERR_NO_ERROR)
		{
			TRACE0("Client Initialization SUCCESSFUL!\n");
			muiClientCount++;
		}
		else 
			MessageBox(NULL, "Client Initialization FAILED.", "Bad News", MB_OK);
	}                                   
	else 
		muiClientCount++;



	//kdm	if (mpDDE_Client == NULL) {
	if (RetVal == DMLERR_NO_ERROR) //successful initialization of DDE system.
	{
		//Go into a loop to count out 1.5 seconds <<temporary patch>> KDM
		/*		int StartTime = GetTickCount();
		while(GetTickCount() - StartTime < 1500);
		*/
		HSZ ServerName;
		HSZ TopicName;
		ServerName = DdeCreateStringHandle(dde_inst, szServerName, CP_WINANSI);
		TopicName = DdeCreateStringHandle(dde_inst, szTopicName, CP_WINANSI);
		mpConversation = DdeConnect(dde_inst, ServerName, TopicName, NULL);

		//Free the strings that were created.  They aren't needed past this point.
		DdeFreeStringHandle(dde_inst, ServerName);
		DdeFreeStringHandle(dde_inst, TopicName);

		if (mpConversation != NULL) 
		{
			mbValid = true;
			TRACE0("Client established conversation with server\n");
		}

		if (!mbValid)
		{
			if (pErrMsg->IsEmpty()) 
				*pErrMsg = "Failed to establish conversation\nwith DDE server.";
			TRACE0("Failed to establish CLIENT conversation with DDE Server\n");
		}
	}
	else  //failed initialization of the DDE system.  Report the error to the user.
	{
		ULSupport ULSupt;

		*pErrMsg = "Failed to create DDE client.";
		ULSupt.ReportDDEError("DdeInitialize (CLIENT)", dde_inst);
		TRACE0("Failed initiatlization of DDE Client\n");
	}

	if (pSuccess != NULL) 
		*pSuccess = mbValid;

	//	*pSuccess = true;  //debug only
}

ULDDEClient::~ULDDEClient()
{
	//kdm  delete mpConversation;
	DdeDisconnect(mpConversation);

	//Decrement the number of clients.
	muiClientCount--;
	if (muiClientCount == 0) 
	{
		//kdm    delete mpDDE_Client;
		//kdm  mpDDE_Client = NULL;	
		DdeUninitialize(dde_inst);
	}
}


const char *ULDDEClient::GetServerName()
{
	return(mServerName);
}

BOOL ULDDEClient::Execute(
						  const char *szKeyword,	// I; Cmd / keyword understood by DDE server.
						  const char *szParam,	// I; Parameter(s) associated with keyword.
						  double /*dTimeOutSecs*/)	// I; (Optional = 0 => no timeout).
{
	BOOL bSuccess = FALSE;                                                     
	char CmdMsg[128]  = "", TempStr[30] = "";

	if (mbValid) 
	{
		sprintf(CmdMsg, "[%s", szKeyword);
		if (szParam == NULL)
			strcat(CmdMsg, "]");
		else 
		{
			sprintf(TempStr, " %s]", szParam);
			strcat(CmdMsg, TempStr);
		}

		//Send the command message to the server to execute.
		TRACE1("FDMS: Client Sending EXECUTE to a server : CMD = %s\n",CmdMsg);

		HDDEDATA rv =DdeClientTransaction((unsigned char *)&CmdMsg[0], strlen(CmdMsg) + 1, mpConversation, NULL, 0, XTYP_EXECUTE, 3000, NULL);

		//Go into a loop to count out 3 seconds <<temporary patch>> KDM
		/*		int StartTime = GetTickCount();
		while(GetTickCount() - StartTime < 3000);
		*/
		if(rv)
		{
			bSuccess = true;
			DdeFreeDataHandle(rv);
		}
		else
		{
			ULSupport ULSupt;
			ULSupt.ReportDDEError("DdeClientTransaction (EXECUTE)", dde_inst);
		}

	}//end IF mbValid is true


	return(bSuccess);
}

//*************************************************************************************************************
//Request () : this method is used when a client wishes to make a REQUEST of a DDEServer.
//The items required in the calling parameters are as follows:
//	const char *szItemName,	// Input -  Item name for request. (Topic name) 
//						ie. from TT client to RAD server, topic = TT_TO_RAD
//	CString *pRetMsg,			// Output -  Response from received from the DDE server.
// double dTimeOutSecs)	// Input; (Optional)  Input zero = 0 => no timeout.
//*************************************************************************************************************
BOOL ULDDEClient::Request(	const char *szItemName,		CString *pRetMsg,	double dTimeOutSecs)
{       
	static bool Locked = false;
	BOOL bSuccess = FALSE;  
	if (mbValid && !Locked) 
	{
		Locked = true;

		//Request a transaction from a server.
		HSZ ItemName;
		ItemName = DdeCreateStringHandle(dde_inst, szItemName, CP_WINANSI);

		TRACE1( "FDMS: Sending REQUEST %s\n", szItemName);

		//Issue the client request to the server.
		HDDEDATA RetData =DdeClientTransaction(NULL, 0, mpConversation, ItemName, CF_TEXT, XTYP_REQUEST, DWORD(dTimeOutSecs * 1000), NULL);
		//Go into a loop to count out 3 seconds <<temporary patch>> KDM
		/*		int StartTime = GetTickCount();
		while(GetTickCount() - StartTime < 3000);
		*/

		//Verify that the call to the DdeClientTransaction was successful.  If it was, then clean up the memory that
		//was allocated to send and/or receive the requested data from the server.
		DdeFreeStringHandle(dde_inst, ItemName);
		if(RetData)
		{
			bSuccess = true;

			//Before freeing the data returned to this client from a server, copy the contents 
			//into a string that is returned to the calling function.
			*pRetMsg = DdeAccessData(RetData, NULL); //not interested in the length of the string.
			DdeUnaccessData(RetData);
			TRACE1("client REQUEST was successful : received %s\n", *pRetMsg);
		}
		else //Get the error and show to the user.
		{
			ULSupport ULSupt;
			ULSupt.ReportDDEError("DdeClientTransaction (REQUEST)", dde_inst);
			TRACE0("client REQUEST failed\n");
		}

		//Free any returned data.
		DdeFreeDataHandle(RetData);

		Locked = false;
	} //end IF mbValid is true

	//If the DDE Server is already busy, then return a flag to indicate success.
	if(Locked)
		*pRetMsg= "NO_ERROR_BUSY";
	return Locked ? true : (bSuccess);
}


