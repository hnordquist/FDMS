//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ULSupport.h $ 
// 
// *****************  Version 5  *****************
// User: Longo        Date: 12/10/04   Time: 4:46p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// Added Full file path to shared rad file
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


#pragma once

//  this will eventually replace GEN.H
#define uiMAX_SEB_BOXES		6  	// The maximum number of boxes to create in the UPPER LAYER

	typedef unsigned int       RESOURCE_ID;

  struct UL_GEN_TIME_STRUCT {
		    int hr;
		    int min;
		    int sec;
		    int msec;
		   };

 struct UL_GEN_DATE_STRUCT {
		    int yr;
		    int mon;
		    int day;
		   };

 struct UL_MSG_DATA_STRUCT{
	 int StationID;
	 int year;
	 int month;
	 int day;
	 int hour;
	 int minute;
	 int second;
 };

class ULSupport  
{
public:
	UL_MSG_DATA_STRUCT ConvertTimeMatchMsgToStruct(CString TimeMatchMsg);
	void ReportDDEError(CString Source, DWORD DDEInstance);
	CString GetCurrentDate();
	//static long mlOriginalTickCount;
	//static double GetElapsedSeconds();
	static CString ULIniPath; //assigned at startup
	static CString ULAppSettingsIniPath;
	static CString ULIRSIniPath;
	static CString ULIRSDataPath;

	ULSupport();
	virtual ~ULSupport();

};

