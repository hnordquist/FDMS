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
//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ULSupport.cpp $ 
// 
// *****************  Version 8  *****************
// User: Longo        Date: 3/09/05    Time: 3:47p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// 
// *****************  Version 7  *****************
// User: Longo        Date: 12/10/04   Time: 4:46p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// Added Full file path to shared rad file
// 
// *****************  Version 6  *****************
// User: Longo        Date: 11/24/04   Time: 3:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// compiler warning removal
// 
// *****************  Version 5  *****************
// User: Longo        Date: 11/22/04   Time: 3:00p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// error message repair
// 
// *****************  Version 4  *****************
// User: Longo        Date: 9/23/04    Time: 4:42p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// Added IRS specific value
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULSupport.h"
#include "DDEML.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ULSupport::ULSupport()
{
}

ULSupport::~ULSupport()
{

}

CString ULSupport::ULAppSettingsIniPath;
CString ULSupport::ULIniPath;
CString ULSupport::ULIRSIniPath;
CString ULSupport::ULIRSDataPath;

/*
double ULSupport::GetElapsedSeconds()
{
	DWORD lCount = GetTickCount();
	double dElapsedSecs = (double)(lCount - mlOriginalTickCount) / 1000.0;
	return(dElapsedSecs);

	return 1;
}
*/
CString ULSupport::GetCurrentDate()
{
	CString DateString;
	COleDateTime CurrentDateTime = COleDateTime::GetCurrentTime();
	char MonthName[6] = "";

	int month = CurrentDateTime.GetMonth();
	sprintf(MonthName, "%s",  month == 1? "Jan" : month == 2? "Feb" : month == 3? "Mar" : month == 4 ? "Apr" : 
			month == 5 ? "May" : month == 6 ? "Jun" : month == 7 ? "Jul" : month == 8 ? "Aug" : month == 9 ? "Sep" :
			month == 10 ? "Oct" : month == 11 ? "Nov" : month == 12 ? "Dec" : "Error");

	//The format is Month day year  hour:minute:second - i.e. Apr 20 2002  14:20:46
	DateString.Format("%s %d %d  %02d:%02d:%02d", MonthName, CurrentDateTime.GetDay(), 
		CurrentDateTime.GetYear(), CurrentDateTime.GetHour(), CurrentDateTime.GetMinute(), 
		CurrentDateTime.GetSecond());
	return DateString;

}

void ULSupport::ReportDDEError(CString Source, DWORD DDEInstance)
{
	CString PrefaceStr;
	UINT _uint = GetPrivateProfileInt("UPPER_LAYER_REVIEW_TOOLS", "DDE_VERBOSE_DEBUG_MSGS", 0, ULIniPath);
	bool VerboseMsgs = (_uint == 0 ? false : true);

	PrefaceStr.Format("Error Reported by %s\n\n", Source);

	//If the DDE instance handle is valid, then get the last DDE error information.
	//Otherwise, report that the instance handle was null and no other information was attainable.
	if(DDEInstance != NULL && VerboseMsgs)
	{
		CString AppName;
		GetPrivateProfileString("UPPER_LAYER_REVIEW_TOOLS", "APPLICATION_NAME", "",
			AppName.GetBuffer(150), 150, ULIniPath);
		AppName.ReleaseBuffer();

		DWORD ErrorCode = DdeGetLastError(DDEInstance);

		if(ErrorCode ==    DMLERR_ADVACKTIMEOUT )
			MessageBox(NULL,PrefaceStr +  "DDEClientTransaction Error : DMLERR_ADVACKTIMEOUT", AppName + "  : Error Message", MB_OK);
		else if (  ErrorCode ==DMLERR_BUSY )
			MessageBox(NULL, PrefaceStr +  "DDEClientTransaction Error : DMLERR_BUSY", AppName + "  : Error Message", MB_OK);
		else if(ErrorCode ==DMLERR_DATAACKTIMEOUT  )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_DATAACKTIMEOUT", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_DLL_NOT_INITIALIZED  )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_DLL_NOT_INITIALIZED", AppName + "  : Error Message", MB_OK);
		else if( ErrorCode ==DMLERR_DLL_USAGE   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_DLL_USAGE", AppName + "  : Error Message", MB_OK);
		else if(   ErrorCode ==DMLERR_EXECACKTIMEOUT   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_EXECACKTIMEOUT", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_INVALIDPARAMETER   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_INVALIDPARAMETER", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_LOW_MEMORY    )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_LOW_MEMORY", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_MEMORY_ERROR   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_MEMORY_ERROR", AppName + "  : Error Message", MB_OK);
		else if(   ErrorCode ==DMLERR_NOTPROCESSED   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_NOTPROCESSED", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_NO_CONV_ESTABLISHED )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_NO_CONV_ESTABLISHED", AppName + "  : Error Message", MB_OK);
		else if(   ErrorCode ==DMLERR_POKEACKTIMEOUT  )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_POKEACKTIMEOUT", AppName + "  : Error Message", MB_OK);
		else if(   ErrorCode ==DMLERR_POSTMSG_FAILED  )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_POSTMSG_FAILED", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_REENTRANCY   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_REENTRANCY", AppName + "  : Error Message", MB_OK);
		else if(  ErrorCode ==DMLERR_SERVER_DIED   )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_SERVER_DIED", AppName + "  : Error Message", MB_OK);
		else if(   ErrorCode ==DMLERR_SYS_ERROR    )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_SYS_ERROR", AppName + "  : Error Message", MB_OK);
		else if(   ErrorCode ==DMLERR_UNADVACKTIMEOUT  )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_UNADVACKTIMEOUT", AppName + "  : Error Message", MB_OK);
		else if( ErrorCode ==DMLERR_UNFOUND_QUEUE_ID     )
			MessageBox(NULL, PrefaceStr +   "DDEClientTransaction Error : DMLERR_UNFOUND_QUEUE_ID", AppName + "  : Error Message", MB_OK);
	}
	else if (VerboseMsgs) 
		//dde instance handle is invalid. Report this to the user.  This is reported regardless
		// no no no //of whether VERBOSE MSGS is turned on.
	{
		PrefaceStr += ": DDE instance handle is NULL; \n\nNo other information available on the DDE error that occurred.";
		MessageBox(NULL, PrefaceStr, "FDMS: DDE Error Message", MB_OK | MB_ICONERROR);
	}

}

UL_MSG_DATA_STRUCT ULSupport::ConvertTimeMatchMsgToStruct(CString TimeMatchMsg)
{
	UL_MSG_DATA_STRUCT MessageData;
	char TempStr[10] = "";

	//Extract the station ID from the original message;
	strncpy(TempStr, (LPCSTR) TimeMatchMsg, 5);
	MessageData.StationID = atoi(TempStr);
	memset(TempStr,  '\0',10);

	//Extract the year, month, and day.
	strncpy(TempStr, (LPCSTR) TimeMatchMsg.Mid(5), 4);
	MessageData.year = atoi(TempStr);
	memset(TempStr,  '\0',10);

	strncpy(TempStr, (LPCSTR) TimeMatchMsg.Mid(9), 2);
	MessageData.month = atoi(TempStr);
	memset(TempStr,  '\0',10);

	strncpy(TempStr, (LPCSTR) TimeMatchMsg.Mid(11), 2);
	MessageData.day = atoi(TempStr);
	memset(TempStr,  '\0',10);
	
	//Extract the hour, minute, and seconds
	strncpy(TempStr, (LPCSTR) TimeMatchMsg.Mid(13), 2);
	MessageData.hour = atoi(TempStr);
	memset(TempStr,  '\0',10);

	strncpy(TempStr, (LPCSTR) TimeMatchMsg.Mid(15), 2);
	MessageData.minute = atoi(TempStr);
	memset(TempStr,  '\0',10);

	strncpy(TempStr, (LPCSTR) TimeMatchMsg.Mid(17), 2);
	MessageData.second = atoi(TempStr);

	return MessageData;
}
