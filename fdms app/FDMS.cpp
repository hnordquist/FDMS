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
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
//
// $History: FDMS.cpp $
// 
// *****************  Version 24  *****************
// User: 186846       Date: 2/24/10    Time: 5:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 23  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 22  *****************
// User: 186846       Date: 12/19/07   Time: 2:42p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 21  *****************
// User: Longo        Date: 12/18/07   Time: 12:32p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 20  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 19  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 18  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 17  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 16  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 15  *****************
// User: Longo        Date: 12/21/04   Time: 4:13p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
// 
// *****************  Version 14  *****************
// User: Longo        Date: 12/10/04   Time: 6:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 13  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 12  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 11  *****************
// User: Longo        Date: 11/22/04   Time: 3:03p
// Updated in $/FDMS/FDMS Application/FDMSNET
// block UL while loading on startup
// 
// *****************  Version 10  *****************
// User: Longo        Date: 11/19/04   Time: 4:28p
// Updated in $/FDMS/FDMS Application/FDMSNET
// find UL files via the FM, if it is configured correctly
// 
// *****************  Version 9  *****************
// User: Longo        Date: 11/19/04   Time: 12:02p
// Updated in $/FDMS/FDMS Application/FDMSNET
// toolbar works now
// 
// *****************  Version 8  *****************
// User: Longo        Date: 11/19/04   Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// toolbar code added
// 
// *****************  Version 7  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FDMS.h"
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CycleFrame.h"
#include "FDMSDoc.h"
#include "testdrawView1.h"
#include "testdrawView2.h"
#include "testdrawView3.h"
#include "testdrawView4.h"
#include "fortran.h"
#include "dconfig.h"

#include ".\SelectMeasurement.h"
#include ".\fdms.h"
#include ".\askoncebox.h"
#include ".\regwrapper.h"

#include "ULSupport.h"

#include "ULAboutBoxDlg.h"
#include ".\AppUl\StartUpMsgdlg.h"

#include ".\tests\test.h"

#include ".\DoAssay.h"
#include ".\xlc\XListCtrlTestDlg.h"

#include ".\LegText.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define APP_NAME "FDMS"


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	static bool m_bDoneGot;
	static CString m_sVer;
	static CString m_sTime;
	static BOOL GetAppInfo();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static BOOL GetLastWriteTime(PTCHAR pszFullPath, CString& lpszString);
public:
	virtual BOOL OnInitDialog();
};


/////////////////////////////////////////////////////////////////////////////
// CFDMSApp

BEGIN_MESSAGE_MAP(CFDMSApp, CWinApp)
	//{{AFX_MSG_MAP(CFDMSApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_EDIT_INIFILE, OnEditIni)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_EDIT_RESETACKNOWLEDGEMENTS, OnEditResetacknowledgements)
	ON_COMMAND(ID_EDIT_CONFIG_O_RAMA_C, OnEditConfigDUMP_O_RAMA_Current)
	ON_COMMAND(ID_EDIT_CONFIG_O_RAMA_D, OnEditConfigDUMP_O_RAMA_Default)
	ON_COMMAND(ID_VIEW_PROCESSINGLOG, OnViewLog)
	ON_COMMAND_RANGE(ID_EDIT_BACKGROUNDCYCLES, ID_EDIT_BACKGROUNDCYCLES + (MENU_SPAN -1),  OnEditPrefBGC)
	ON_COMMAND_RANGE(ID_EDIT_BACKGROUNDCYCLETIME, ID_EDIT_BACKGROUNDCYCLETIME  + (MENU_SPAN -1),  OnEditPrefBGCT)
	ON_COMMAND_RANGE(ID_EDIT_DELTAENRICHMENT, ID_EDIT_DELTAENRICHMENT  + (MENU_SPAN -1),  OnEditPrefDE)
	ON_COMMAND_RANGE(ID_EDIT_DELTACOOLINGTIME, ID_EDIT_DELTACOOLINGTIME  + (MENU_SPAN -1),  OnEditPrefDCT)
	ON_COMMAND_RANGE(ID_EDIT_STANDARDMEASUREMENTCYCLES, ID_EDIT_STANDARDMEASUREMENTCYCLES  + (MENU_SPAN -1),  OnEditPrefSMC)
	ON_COMMAND_RANGE(ID_EDIT_STANDARDMEASUREMENTCYCLETIME, ID_EDIT_STANDARDMEASUREMENTCYCLETIME  + (MENU_SPAN -1),  OnEditPrefSMCT)
	ON_COMMAND_RANGE(ID_EDIT_DETECTOR_ID, ID_EDIT_DETECTOR_ID  + (MENU_SPAN -1),  OnEditPrefDID)
	ON_COMMAND_RANGE(ID_TOOLMENUID, ID_TOOLMENUID + 10,  OnToolGoose)
	ON_COMMAND(ID_TOOLS_IMPORT, OnImportButton)

	ON_COMMAND(ID_APP_EXIT, OnAppExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDMSApp construction

CFDMSApp::CFDMSApp()
{
	m_bPrefChangedMain = false;
	m_bPrefChangedChild = false;
	m_pMeasurementDialog = NULL;
	//m_MLog.SetSize(512);
	bSL = false;
	m_pLogLog = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFDMSApp object

CFDMSApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFDMSApp initialization
//typedef int (*DLLFunction)(SPersonality* sPersonality);


SUBROUTINE READDATABASE(
						CHARACTER	dbfile,
						INTEGER*	i, 
						CHARACTER	dbheader, 
						CHARACTER	facility, 
						CHARACTER	id, 
						INTEGER*	mtype,
						INTEGER*    mstatus,     // integer array(10000) measurement type
						REAL*		en,
						REAL*		bu,
						INTEGER*	dcdateday,
						INTEGER*	dcdatemonth,
						INTEGER*	dcdateyear,
						INTEGER*	icycle,
						REAL*		thresA,
						REAL*		thresB,
						INTEGER*	mdateday,
						INTEGER*	mdatemonth,
						INTEGER*	mdateyear,
						REAL*		ct,
						REAL*		na,
						REAL*		nb,
						REAL*		nc,
						REAL*		gamma1,
						REAL*		gamma2,
						CHARACTER	detector, 
						REAL*		lognog,
						REAL*		logbu,
						REAL*		logct,
						REAL*		logdcn,
						REAL*		loggamma,
						INTEGER*	ierror
						);

SUBROUTINE WRITEDATABASE(

						 CHARACTER dbfilename,// character*280
						 INTEGER* ii,        // integer number of entries in database.csv
						 CHARACTER dbheader,  // character*200 database.csv file header
						 CHARACTER facility,  // character*32 array(10000) facility names
						 CHARACTER id,        // character*32 array(10000) item IDs
						 INTEGER* mtype,     // integer array(10000) measurement type
						 // 1 cycle verification
						 // 2 assembly verification
						 // 3 item reverification
						 INTEGER* mstatus,     // integer array(10000) measurement type
						 REAL* en,        // real*4 array(10000) 235U enrichment level in %
						 REAL* bu,        // real*4 array(10000) declared burnup in GWd/MT
						 INTEGER* dcday,     // integer array(10000) dischange day 1-31
						 INTEGER* dcmonth,   // integer array(10000) dischange month 1-12
						 INTEGER* dcyear,    // integer array(10000) dischange year 1900-3000
						 INTEGER* icycle,    // integer array(10000)
						 REAL* thresA,    // real*4 array(10000) threshold for neutron channel A
						 REAL* thresB,    // real*4 array(10000) threshold for neutron channel B
						 INTEGER* mday,      // integer array(10000) measurement day 1-31
						 INTEGER* mmonth,    // integer array(10000) measurement month 1-12
						 INTEGER* myear,     // integer array(10000) measurement year 1900-3000
						 REAL* ct,        // real*4 array(10000) declared cooling time in years
						 REAL* na,        // real*4 array(10000) neutrons A
						 REAL* nb,        // real*4 array(10000) neutrons B
						 REAL* nc,        // real*4 array(10000) neutrons C
						 REAL* gamma1,    // real*4 array(10000) gamma dose 1
						 REAL* gamma2,    // real*4 array(10000) gamma dose 2
						 CHARACTER	detector

						 );

SUBROUTINE READMEASUREMENTPLAN(
							   CHARACTER	mpfile,
							   INTEGER*	ii, 
							   CHARACTER	dbheader, 
							   CHARACTER	facility, 
							   CHARACTER	id, 
							   INTEGER*	mtype,
							   INTEGER*	mstatus,
							   REAL*		en,
							   REAL*		bu,
							   INTEGER*	dcdateday,
							   INTEGER*	dcdatemonth,
							   INTEGER*	dcdateyear,
							   INTEGER*	icycle,
							   REAL*		thresA,
							   REAL*		thresB,
							   INTEGER*	mdateday,
							   INTEGER*	mdatemonth,
							   INTEGER*	mdateyear,
							   REAL*		ct,
							   REAL*		na,
							   REAL*		nb,
							   REAL*		nc,
							   REAL*		gamma1,
							   REAL*		gamma2,
							   CHARACTER	detector, 
							   REAL*		lognog,
							   REAL*		logbu,
							   REAL*		logct,
							   REAL*		logdcn,
							   REAL*		loggamma,
							   INTEGER*	ierror
							   );

SUBROUTINE GETINFOFORPLOTS(
						   LOGICAL*	tf1,
						   LOGICAL*	tf2,
						   LOGICAL*	tf3,
						   LOGICAL*	tf4, 
						   LOGICAL*	tf5,
						   LOGICAL*	tf6,
						   LOGICAL*	tf7,
						   LOGICAL*	tf8,	
						   INTEGER*	idb,
						   INTEGER*	mtype,
						   REAL*		en,		
						   REAL*		lognog,	
						   INTEGER*	icycle,
						   REAL*		logbu,  
						   REAL*		logct,
						   REAL*		logdcn, 
						   REAL*		loggamma, 
						   INTEGER*	imp,
						   INTEGER*	mtypemp,
						   REAL*		enmp,
						   REAL*		lognogmp,	
						   INTEGER*	icyclemp,
						   REAL*		logbump, 	
						   REAL*		logctmp, 
						   REAL*		logdcnmp, 
						   REAL*		loggammamp,
						   INTEGER*	mstatusmp, 	
						   INTEGER*	imr,
						   REAL*		delta_en, 	REAL*	delta_ct,	
						   REAL*		xgr1, 	REAL*	ygr1, 	
						   REAL*		xgr2, 	REAL*	ygr2, 	
						   REAL*		xgr3, 	REAL*	ygr3,
						   REAL*		xgr4, 	REAL*	ygr4,

						   REAL*		xbl1, 	REAL*	ybl1, 	
						   REAL*		xbl2, 	REAL*	ybl2, 
						   REAL*		xbl3, 	REAL*	ybl3,
						   REAL*		xbl4, 	REAL*	ybl4,

						   REAL*		xgd1, 	REAL*	ygd1, 
						   REAL*		xgd2, 	REAL*	ygd2, 	
						   REAL*		xgd3, 	REAL*	ygd3,
						   REAL*		xgd4, 	REAL*	ygd4,

						   REAL*		xnu1, 	REAL*	ynu1, 
						   REAL*		xnu2, 	REAL*	ynu2, 	
						   REAL*		xnu3, 	REAL*	ynu3,
						   REAL*		xnu4, 	REAL*	ynu4,

						   REAL*		xsp1, 	REAL*	ysp1, 	
						   REAL*		xsp2, 	REAL*	ysp2, 
						   REAL*		xsp3, 	REAL*	ysp3,	
						   REAL*		xsp4, 	REAL*	ysp4,	

						   REAL*		xmr1, 	REAL*	ymr1, 
						   REAL*		xmr2, 	REAL*	ymr2, 	
						   REAL*		xmr3, 	REAL*	ymr3,
						   REAL*		xmr4, 	REAL*	ymr4,

						   INTEGER*	igr1, 	INTEGER* ibl1,	
						   INTEGER*	igr2, 	INTEGER* ibl2,
						   INTEGER*	igr3, 	INTEGER* ibl3,	
						   INTEGER*	igr4, 	INTEGER* ibl4,	

						   INTEGER*	igd,  	INTEGER* inu,	INTEGER* isp,

						   LOGICAL*	fit1,	LOGICAL* fit2,	LOGICAL* fit3,	
						   REAL*		a1, 	REAL*	b1,		REAL*	syx21, 	REAL*	sx21, REAL*	meanx1,	
						   REAL*		a2, 	REAL*	b2,		REAL*	syx22, 	REAL*	sx22, REAL*	meanx2,
						   REAL*		a3, 	REAL*	b3,		REAL*	syx23,	REAL*	sx23, REAL*	meanx3,

						   INTEGER*	ifit1, 	INTEGER*	ifit2, 	INTEGER*	ifit3,

						   REAL*		p4x,	REAL*	p4s
						   );

SUBROUTINE WRITEMEASUREMENTPLAN(
								CHARACTER mpfilename,// character*280
								INTEGER* ii,        // integer number of entries in database.csv
								CHARACTER dbheader,  // character*200 database.csv file header
								CHARACTER facility,  // character*32 array(1000) facility names
								CHARACTER id,        // character*32 array(1000) item IDs
								INTEGER* mtype,     // integer array(1000) measurement type
								// 1 cycle verification
								// 2 assembly verification
								// 3 item reverification
								INTEGER* mstatus,   // integer array(1000)
								REAL* en,        // real*4 array(1000) 235U enrichment level in %
								REAL* bu,        // real*4 array(1000) declared burnup in GWd/MT
								INTEGER* dcday,     // integer array(1000) dischange day 1-31
								INTEGER* dcmonth,   // integer array(1000) dischange day 1-12
								INTEGER* dcyear,    // integer array(1000) dischange day 1900-3000
								INTEGER* icycle,    // integer array(1000)
								REAL* thresA,    // real*4 array(1000) threshold for neutron channel A
								REAL* thresB,    // real*4 array(1000) threshold for neutron channel B
								INTEGER* mday,      // integer array(1000) measurement day 1-31
								INTEGER* mmonth,    // integer array(1000) measurement month 1-12
								INTEGER* myear,     // integer array(1000) measurement year 1900-3000
								REAL* ct,        // real*4 array(1000) declared cooling time in years
								REAL* na,        // real*4 array(1000) neutrons A
								REAL* nb,        // real*4 array(1000) neutrons B
								REAL* nc,        // real*4 array(1000) neutrons C
								REAL* gamma1,    // real*4 array(1000) gamma dose 1
								REAL* gamma2,    // real*4 array(1000) gamma dose 2
								CHARACTER detector  // character*32 array(1000) detector names

								);

SUBROUTINE UPDATEMEASUREMENTS(

							  CHARACTER mfilename, // character*280
							  INTEGER* i,         // integer member to be added
							  CHARACTER dbheader,  // character*200 database.csv file header
							  CHARACTER facility,  // character*32 array(1000) facility names
							  CHARACTER id,        // character*32 array(1000) item IDs
							  INTEGER* mtype,     // integer array(1000) measurement type
							  // 1 cycle verification
							  // 2 assembly verification
							  // 3 item reverification
							  INTEGER* mstatus,   // integer array(1000)
							  REAL* en,        // real*4 array(1000) 235U enrichment level in %
							  REAL* bu,        // real*4 array(1000) declared burnup in GWd/MT
							  INTEGER* dcday,     // integer array(1000) dischange day 1-31
							  INTEGER* dcmonth,   // integer array(1000) dischange day 1-12
							  INTEGER* dcyear,    // integer array(1000) dischange day 1900-3000
							  INTEGER* icycle,    // integer array(1000)
							  REAL* thresA,    // real*4 array(1000) threshold for neutron channel A
							  REAL* thresB,    // real*4 array(1000) threshold for neutron channel B
							  INTEGER* mday,      // integer array(1000) measurement day 1-31
							  INTEGER* mmonth,    // integer array(1000) measurement month 1-12
							  INTEGER* myear,     // integer array(1000) measurement year 1900-3000
							  REAL* ct,        // real*4 array(1000) declared cooling time in years
							  REAL* na,        // real*4 array(1000) neutrons A
							  REAL* nb,        // real*4 array(1000) neutrons B
							  REAL* nc,        // real*4 array(1000) neutrons C
							  REAL* gamma1,    // real*4 array(1000) gamma dose 1
							  REAL* gamma2,    // real*4 array(1000) gamma dose 2
							  CHARACTER detector // character*32 array(1000) Detector IDs

							  );

SUBROUTINE UPDATEMEASUREMENTSBG(
								CHARACTER mfilename, // character*280
								CHARACTER dbheader,  // character*200 mp.csv file header

								INTEGER* mday,      // integer  measurement day 1-31
								INTEGER* mmonth,    // integer measurement month 1-12
								INTEGER* myear,     // integer  measurement year 1900-3000
								REAL* na,        // real*4  neutrons A
								REAL* nb,        // real*4 neutrons B
								REAL* nc,        // real*4 neutrons C
								REAL* gamma1,    // real*4 gamma dose 1
								REAL* gamma2,    // real*4  gamma dose 2
								CHARACTER detector // character*32 Detector IDs


								);





int		g_iEntriesInDB;
char	g_szDBFilePathName[280];
char	g_szDBFileHeader[200];
char	g_szFacilityNames[DB_ARRAYSIZE][MAX_NAME_LENGTH];
char	g_szItemIDs[DB_ARRAYSIZE][MAX_ID_LENGTH];
int		g_iMeasurementType[DB_ARRAYSIZE];
float	g_fEnrichmentLevel[DB_ARRAYSIZE];
float	g_fBurnUp[DB_ARRAYSIZE];
int		g_iDischargeDateDay[DB_ARRAYSIZE];
int		g_iDischargeDateMonth[DB_ARRAYSIZE];
int		g_iDischargeDateYear[DB_ARRAYSIZE];
int		g_iCycle[DB_ARRAYSIZE];
int		g_iStatus[DB_ARRAYSIZE];
float	g_fNChanAThresh[DB_ARRAYSIZE];
float	g_fNChanBThresh[DB_ARRAYSIZE];
int		g_iMeasurementDateDay[DB_ARRAYSIZE];
int		g_iMeasurementDateMonth[DB_ARRAYSIZE];
int		g_iMeasurementDateYear[DB_ARRAYSIZE];
float	g_fCoolingTime[DB_ARRAYSIZE];
float	g_fNChanA[DB_ARRAYSIZE];
float	g_fNChanB[DB_ARRAYSIZE];
float	g_fNChanC[DB_ARRAYSIZE];
float	g_fGDose1[DB_ARRAYSIZE];
float	g_fGDose2[DB_ARRAYSIZE];
double	g_dTemp[DB_ARRAYSIZE];
float	g_fLogCoolingTime[DB_ARRAYSIZE];
float	g_fLogBurnup[DB_ARRAYSIZE];
float	g_fLogDecayCorTotNeutrons[DB_ARRAYSIZE];
float	g_fLogGammaDose[DB_ARRAYSIZE];
float	g_fLogNoverG[DB_ARRAYSIZE];
char	g_szDetectorIDs[DB_ARRAYSIZE][MAX_NAME_LENGTH];

int		g_iError;
BOOL	g_bPlot1;
BOOL	g_bPlot2;
BOOL	g_bPlot3;
BOOL	g_bPlot4;
BOOL	g_bIncludeGood;
BOOL	g_bAcceptGood;
BOOL	g_bIncludeSuspect;
BOOL	g_bPlotMostRecent = true;

char	g_szMLFilePathName[280];

int		g_iMPItemIndices[MP_ARRAYSIZE];
int		g_iMPEntriesIn;
char	g_szMPFilePathName[280];
char	g_szMPFileHeader[200];
char	g_szMPFacilityNames[MP_ARRAYSIZE][MAX_NAME_LENGTH];
char	g_szMPItemIDs[MP_ARRAYSIZE][MAX_ID_LENGTH];
int		g_iMPMeasurementType[MP_ARRAYSIZE];
float	g_fMPEnrichmentPercent[MP_ARRAYSIZE];
float	g_fMPBurnUp[MP_ARRAYSIZE];
float	g_fMPLogCoolingTime[MP_ARRAYSIZE];
float	g_fMPLogGammaDose[MP_ARRAYSIZE];
int		g_iMPStatus[MP_ARRAYSIZE];
int		g_iMPIndexMostRecentMeasurement;
int		g_iMPDischargeDateDay[MP_ARRAYSIZE];
int		g_iMPDischargeDateMonth[MP_ARRAYSIZE];
int		g_iMPDischargeDateYear[MP_ARRAYSIZE];
int		g_iMPCycle[MP_ARRAYSIZE];
float	g_fMPNChanAThresh[MP_ARRAYSIZE];
float	g_fMPNChanBThresh[MP_ARRAYSIZE];
int		g_iMPMeasurementDateDay[MP_ARRAYSIZE];
//int		g_iMPMeasurementStat[MP_ARRAYSIZE];
int		g_iMPMeasurementDateMonth[MP_ARRAYSIZE];
int		g_iMPMeasurementDateYear[MP_ARRAYSIZE];
float	g_fMPCoolingTime[MP_ARRAYSIZE];
float	g_fMPNChanA[MP_ARRAYSIZE];
float	g_fMPNChanB[MP_ARRAYSIZE];
float	g_fMPNChanC[MP_ARRAYSIZE];
float	g_fMPGDose1[MP_ARRAYSIZE];
float	g_fMPGDose2[MP_ARRAYSIZE];
float	g_fMPLogBurnup[MP_ARRAYSIZE];
float	g_fMPLogDecayCorTotNeutrons[MP_ARRAYSIZE];
float	g_fMPLogNoverG[MP_ARRAYSIZE];

char	g_szMPDetectorIDs[MP_ARRAYSIZE][MAX_NAME_LENGTH];

float	g_fDeltaEnrichment;
float	g_fDeltaCoolingTime;

float	g_fGrayDataX1[DB_ARRAYSIZE];
float	g_fGrayDataY1[DB_ARRAYSIZE];
float	g_fGrayDataX2[DB_ARRAYSIZE];
float	g_fGrayDataY2[DB_ARRAYSIZE];
float	g_fGrayDataX3[DB_ARRAYSIZE];
float	g_fGrayDataY3[DB_ARRAYSIZE];
float	g_fGrayDataX4[DB_ARRAYSIZE];
float	g_fGrayDataY4[DB_ARRAYSIZE];

float	g_fBlueDataX1[DB_ARRAYSIZE];
float	g_fBlueDataY1[DB_ARRAYSIZE];
float	g_fBlueDataX2[DB_ARRAYSIZE];
float	g_fBlueDataY2[DB_ARRAYSIZE];
float	g_fBlueDataX3[DB_ARRAYSIZE];
float	g_fBlueDataY3[DB_ARRAYSIZE];
float	g_fBlueDataX4[DB_ARRAYSIZE];
float	g_fBlueDataY4[DB_ARRAYSIZE];

float	g_fMPGoodDataX1[MP_ARRAYSIZE];
float	g_fMPGoodDataY1[MP_ARRAYSIZE];
float	g_fMPGoodDataX2[MP_ARRAYSIZE];
float	g_fMPGoodDataY2[MP_ARRAYSIZE];
float	g_fMPGoodDataX3[MP_ARRAYSIZE];
float	g_fMPGoodDataY3[MP_ARRAYSIZE];
float	g_fMPGoodDataX4[MP_ARRAYSIZE];
float	g_fMPGoodDataY4[MP_ARRAYSIZE];

float	g_fMPNeutralDataX1[MP_ARRAYSIZE];
float	g_fMPNeutralDataY1[MP_ARRAYSIZE];
float	g_fMPNeutralDataX2[MP_ARRAYSIZE];
float	g_fMPNeutralDataY2[MP_ARRAYSIZE];
float	g_fMPNeutralDataX3[MP_ARRAYSIZE];
float	g_fMPNeutralDataY3[MP_ARRAYSIZE];
float	g_fMPNeutralDataX4[MP_ARRAYSIZE];
float	g_fMPNeutralDataY4[MP_ARRAYSIZE];

float	g_fMPSuspectDataX1[MP_ARRAYSIZE];
float	g_fMPSuspectDataY1[MP_ARRAYSIZE];
float	g_fMPSuspectDataX2[MP_ARRAYSIZE];
float	g_fMPSuspectDataY2[MP_ARRAYSIZE];
float	g_fMPSuspectDataX3[MP_ARRAYSIZE];
float	g_fMPSuspectDataY3[MP_ARRAYSIZE];
float	g_fMPSuspectDataX4[MP_ARRAYSIZE];
float	g_fMPSuspectDataY4[MP_ARRAYSIZE];

float	g_fMostRecentDataX1;
float	g_fMostRecentDataY1;
float	g_fMostRecentDataX2;
float	g_fMostRecentDataY2;
float	g_fMostRecentDataX3;
float	g_fMostRecentDataY3;
float	g_fMostRecentDataX4;
float	g_fMostRecentDataY4;

int		g_iGrayCount1;
int		g_iBlueCount1;
int		g_iGrayCount2;
int		g_iBlueCount2;
int		g_iGrayCount3;
int		g_iBlueCount3;
int		g_iGrayCount4;
int		g_iBlueCount4;
int		g_iGoodCount;
int		g_iNeutralCount;
int		g_iSuspectCount;

BOOL	g_bFit1Possible;
BOOL	g_bFit2Possible;
BOOL	g_bFit3Possible;

float	g_fSlope1;
float	g_fIntercept1;
float	g_fStatY1;
float	g_fStatX1;
float	g_fMeanX1;

float	g_fSlope2;
float	g_fIntercept2;
float	g_fStatY2;
float	g_fStatX2;
float	g_fMeanX2;

float	g_fSlope3;
float	g_fIntercept3;
float	g_fStatY3;
float	g_fStatX3;
float	g_fMeanX3;

int		g_iFitPoints1;
int		g_iFitPoints2;
int		g_iFitPoints3;

float	g_fCVAverages[CV_ARRAYSIZE];
float	g_fCVOneSigma[CV_ARRAYSIZE];

float	g_fBkgMeasurements[MaxChannels];


void CFDMSApp::InitF77Globals()
{

	for (int uiy =0; uiy < eMenuTypeCountMax; uiy++)
	{
		m_bMenusConstructed[uiy] = false;
	}

	g_iEntriesInDB = 0;
	g_szDBFilePathName[0] = NULL;
	g_szDBFileHeader[0] = NULL;
	int i;
	for (i = 0; i < DB_ARRAYSIZE; i++)
	{
		g_szFacilityNames[i][0] = NULL;
		g_szItemIDs[i][0] = NULL;
		g_iMeasurementType[i] = 0;
		g_fEnrichmentLevel[i] = 0.0;
		g_fBurnUp[i] = 0.0;
		g_iDischargeDateDay[i] = 0;
		g_iDischargeDateMonth[i] = 0;
		g_iDischargeDateYear[i] = 0;
		g_iCycle[i] = 0;
		g_iStatus[i] = 0;
		g_fNChanAThresh[i] = 0.0;
		g_fNChanBThresh[i] = 0.0;
		g_iMeasurementDateDay[i] = 0;
		g_iMeasurementDateMonth[i] = 0;
		g_iMeasurementDateYear[i] = 0;
		g_fCoolingTime[i] = 0.0;
		g_fNChanA[i] = 0.0;
		g_fNChanB[i] = 0.0;
		g_fNChanC[i] = 0.0;
		g_fGDose1[i] = 0.0;
		g_fGDose2[i] = 0.0;
		g_dTemp[i] = 0;
		g_fLogCoolingTime[i] = 0.0;
		g_fLogBurnup[i] = 0.0;
		g_fLogDecayCorTotNeutrons[i] = 0.0;
		g_fLogGammaDose[i] = 0.0;

//		g_dMeasurementTime[i] = 0.0;

		g_fGrayDataX1[i] = 0.0;
		g_fGrayDataY1[i] = 0.0;
		g_fGrayDataX2[i] = 0.0;
		g_fGrayDataY2[i] = 0.0;
		g_fGrayDataX3[i] = 0.0;
		g_fGrayDataY3[i] = 0.0;
		g_fBlueDataX1[i] = 0.0;
		g_fBlueDataY1[i] = 0.0;
		g_fBlueDataX2[i] = 0.0;
		g_fBlueDataY2[i] = 0.0;
		g_fBlueDataX3[i] = 0.0;
		g_fBlueDataY3[i] = 0.0;
		g_szDetectorIDs[i][0] = NULL;

	}
	for (i = 0; i < MP_ARRAYSIZE; i++)
	{
		g_fMPGoodDataX1[i] = 0.0;
		g_fMPGoodDataY1[i] = 0.0;
		g_fMPGoodDataX2[i] = 0.0;
		g_fMPGoodDataY2[i] = 0.0;
		g_fMPGoodDataX3[i] = 0.0;
		g_fMPGoodDataY3[i] = 0.0;

		g_fMPSuspectDataX1[i] = 0.0;
		g_fMPSuspectDataY1[i] = 0.0;
		g_fMPSuspectDataX2[i] = 0.0;
		g_fMPSuspectDataY2[i] = 0.0;
		g_fMPSuspectDataX3[i] = 0.0;
		g_fMPSuspectDataY3[i] = 0.0;
	}


	g_iError = 0;
	g_bPlot1 = FALSE;
	g_bPlot2 = FALSE;
	g_bPlot3 = FALSE;
	g_bPlot4 = FALSE;
	g_bIncludeGood = false;
	g_bAcceptGood = false;
	g_bIncludeSuspect = false;
	g_bPlotMostRecent = true;

	g_iMPEntriesIn = 0;
	g_szMPFilePathName[0] = NULL;
	g_szMPFileHeader[0] = NULL;

	for (i = 0; i < MP_ARRAYSIZE; i++)
	{
		ClearMPEntry(i);
	}

	g_fDeltaEnrichment = (float)0.15;
	g_fDeltaCoolingTime = 5.0f;


	g_iMPIndexMostRecentMeasurement = 0;


	g_fMostRecentDataX1 = 0.0;
	g_fMostRecentDataY1 = 0.0;
	g_fMostRecentDataX2 = 0.0;
	g_fMostRecentDataY2 = 0.0;
	g_fMostRecentDataX3 = 0.0;
	g_fMostRecentDataY3 = 0.0;

	g_iGrayCount1 = 0;
	g_iBlueCount1 = 0;
	g_iGrayCount2 = 0;
	g_iBlueCount2 = 0;
	g_iGrayCount3 = 0;
	g_iBlueCount3 = 0;
	g_iGoodCount = 0;
	g_iSuspectCount = 0;

	g_bFit1Possible = false;
	g_bFit2Possible = false;
	g_bFit3Possible = false;

	g_fSlope1 = 0.0;
	g_fIntercept1 = 0.0;
	g_fStatY1 = 0.0;
	g_fStatX1 = 0.0;
	g_fMeanX1 = 0.0;

	g_fSlope2 = 0.0;
	g_fIntercept2 = 0.0;
	g_fStatY2 = 0.0;
	g_fStatX2 = 0.0;
	g_fMeanX2 = 0.0;

	g_fSlope3 = 0.0;
	g_fIntercept3 = 0.0;
	g_fStatY3 = 0.0;
	g_fStatX3 = 0.0;
	g_fMeanX3 = 0.0;

	g_iFitPoints1 = 0;
	g_iFitPoints2 = 0;
	g_iFitPoints3 = 0;

}

BOOL CFDMSApp::InitIniFileandRelatedValues()
{

	CIniFileProcessor::SetDefaultSource();  // sets g_cIniFile

	strcpy(g_szDBFilePathName,CIniFileProcessor::DefaultSourcePath);
	strcpy(g_szMPFilePathName,CIniFileProcessor::DefaultSourcePath);
	strcpy(g_szMLFilePathName,CIniFileProcessor::DefaultSourcePath);

	//if the ini file isn't there create a "default" one
	if (_access(CIniFileProcessor::DefaultSourceFullPath,0)!=0)
	{
		if (!MakeNewConfigFile())
		{
			CString cString;
			cString.Format("Unable to find or create the FDMS configuration file\r\n%s\r\n\r\nTerminating .... ",
				CIniFileProcessor::DefaultSourceFullPath);
			AfxMessageBox(cString,MB_OK);
			return FALSE;
		}
	}

	CIniFileProcessor::CacheIniFileValues();

	CSelectMeasurement::MakeFieldMap();
	XListCtrlDlg::MakeFieldMap();
//	ArtfulMeasurement::MakeFieldMap();

	// logging directly defaults to ini file location
	CLogFile::SetLogLoc();

	CIniFileProcessor::ExtractFlags(m_lpCmdLine);

	GETPROFBOOL(eDEMOVERSION, m_bDemoVersion);
	GETPROFSTR(eMEASUREMENTS,g_szMLFilePathName,sizeof(g_szMLFilePathName));
	GETPROFSTR(eDATABASE,g_szDBFilePathName,sizeof(g_szDBFilePathName));
	GETPROFSTR(eMEASUREMENTPLAN,g_szMPFilePathName,sizeof(g_szMPFilePathName));

	GetDeltaEnrichmentValue(g_fDeltaEnrichment);
	GetDeltaCoolingTimeValue(g_fDeltaCoolingTime);

	return TRUE;

}

BOOL CFDMSApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	AfxInitRichEdit2();
	BOOL bExitValue = FALSE;
	AfxEnableControlContainer();
	CoInitialize(NULL);
	// Standard initialization

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("LANL-FDMS"));  // todo: this must be correctly matched with that used by INCC
	//
	//Let the user know that application is starting
	CStartUpMsgDlg StartUpDlg;
	StartUpDlg.Create(IDD_STARTUP_MSG_DLG, NULL);
	StartUpDlg.ShowWindow(SW_SHOW);

	InitF77Globals();

	for (int cht = NeutA; cht < MaxChannels; cht++)
	{
		g_fBkgMeasurements[cht] = .0f;
	}

	if (!InitIniFileandRelatedValues())
		return FALSE;

	m_pDocTemplate1 = new CMultiDocTemplate(
		IDR_FDMSTYPE,
		RUNTIME_CLASS(CFDMSDoc),
		RUNTIME_CLASS(CNvBUFrame), // custom MDI child frame
		RUNTIME_CLASS(CGraphNeutVBUView));
	AddDocTemplate(m_pDocTemplate1);

	m_pDocTemplate2 = new CMultiDocTemplate(
		IDR_FDMSTYPE,
		RUNTIME_CLASS(CFDMSDoc),
		RUNTIME_CLASS(CGBUvTFrame), // custom MDI child frame
		RUNTIME_CLASS(CGraphGDBVCTView));
	AddDocTemplate(m_pDocTemplate2);

	m_pDocTemplate3 = new CMultiDocTemplate(
		IDR_FDMSTYPE,
		RUNTIME_CLASS(CFDMSDoc),
		RUNTIME_CLASS(CNvGFrame), // custom MDI child frame
		RUNTIME_CLASS(CGraphNeutVGammaView));
	AddDocTemplate(m_pDocTemplate3);

	m_pDocTemplate4 = new CMultiDocTemplate(
		IDR_FDMSTYPE,
		RUNTIME_CLASS(CFDMSDoc),
		RUNTIME_CLASS(CCycleFrame), // custom MDI child frame
		RUNTIME_CLASS(CGraphCycleVerifView));
	AddDocTemplate(m_pDocTemplate4);

	CAboutDlg::GetAppInfo();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// start up presuming no upper layer integration
	bool bNoUL = true;
	bool bULError = false;

	ULStartup(bNoUL, bULError, StartUpDlg);

	if (bNoUL && !bULError)
	{
		if (!ObtainRuntimeAssets())
			return FALSE;
		DoFinalInitialization(0 != cmdInfo.m_bShowSplash);
		AfxGetApp()->Run();
	}

	return bExitValue;
}

BOOL CFDMSApp::ObtainRuntimeAssets()
{
	if (_access(g_szDBFilePathName,06)==-1)
	{
		DWORD cchCurDir = MAX_PATH; 
		LPTSTR lpszCurDir; 
		TCHAR tchBuffer[MAX_PATH+1];
		lpszCurDir = tchBuffer; 
		GetCurrentDirectory(cchCurDir, lpszCurDir); 
		lpszCurDir[MAX_PATH] = '\0';

		bool bGot = false;
		CString cString;
		cString.Format("The FDMS database file '%s' cannot be found (or is unreadable).\r\nDo you want to browse for the file manually?\r\nChoose 'Yes' to browse for the FDMS database file manually, choose 'No' to exit FDMS.",
			g_szDBFilePathName);

		do 
		{
			if(AfxMessageBox(cString,MB_YESNO|MB_ICONQUESTION) == IDNO)
				return FALSE;
			else
			{
				char *p;
				char* filename;
				p = strrchr(g_szDBFilePathName , '\\' );
				if (p)
				{
					int result;
					result = p - g_szDBFilePathName + 1;
					filename =  g_szDBFilePathName  + result;
				}
				else
				{
					filename = "*.csv";
				}
				char szFilter[] = "Database Files (*.csv)|*.csv|All Files (*.*)|*.*||";

				CFileDialog ldFile(TRUE,"csv",filename, OFN_FILEMUSTEXIST | 
					OFN_PATHMUSTEXIST,szFilter,NULL);
				if(ldFile.DoModal() == IDOK)
				{
					CIniFileProcessor::SaveSingleValue(eDATABASE,ldFile.GetPathName());
					strcpy(g_szDBFilePathName,ldFile.GetPathName());

					bGot = true;
				}
			}
		} while (!bGot);

		SetCurrentDirectory(lpszCurDir); 
	}

	if (_access(g_szMPFilePathName,06)==-1)
	{

		bool bGot = false;

		CString cString;
		cString.Format(
			"The FDMS measurement plan file '%s' cannot be found (or is unreadable).\r\nDo you want to browse for the file manually?\r\nChoose 'Yes' to browse for the FDMS measurment plan file manually, choose 'No' to exit FDMS.",
			g_szMPFilePathName);

		do
		{
			if(AfxMessageBox(cString,MB_YESNO|MB_ICONQUESTION) == IDNO)
				return FALSE;
			else
			{
				char *p;
				char* filename;
				p = strrchr(g_szMPFilePathName , '\\' );
				if (p)
				{
					int result;
					result = p - g_szMPFilePathName + 1;
					filename =  g_szMPFilePathName  + result;
				}
				else
				{
					filename = "*.csv";
				}
				char szFilter[] = "Measurement Plan Files (*.csv)|*.csv|All Files (*.*)|*.*||";
				CFileDialog ldFile(TRUE,"csv",filename, OFN_FILEMUSTEXIST | 
					OFN_PATHMUSTEXIST,szFilter,NULL);
				if(ldFile.DoModal() == IDOK)
				{
					CIniFileProcessor::SaveSingleValue(eMEASUREMENTPLAN, ldFile.GetPathName());
					strcpy(g_szMPFilePathName,ldFile.GetPathName());
					bGot = true;
				}
			}
		} while (!bGot);
	}

	if (_access(g_szMLFilePathName,06)==-1)
	{

		bool bGot = false;

		CString cString;
		cString.Format(
			"The FDMS measurement results file '%s' cannot be found (or is unreadable).\r\nDo you want to browse for the file manually?\r\nChoose 'Yes' to browse for the FDMS measurment results file manually, choose 'No' to exit FDMS.",
			g_szMLFilePathName);

		do
		{
			if(AfxMessageBox(cString,MB_YESNO|MB_ICONQUESTION) == IDNO)
				return FALSE;
			else
			{
				char *p;
				char* filename;
				p = strrchr(g_szMLFilePathName , '\\' );
				if (p)
				{
					int result;
					result = p - g_szMLFilePathName + 1;
					filename =  g_szMLFilePathName  + result;
				}
				else
				{
					filename = "*.csv";
				}
				char szFilter[] = "Measurement Results Files (*.csv)|*.csv|All Files (*.*)|*.*||";
				CFileDialog ldFile(TRUE,"csv",filename, OFN_FILEMUSTEXIST | 
					OFN_PATHMUSTEXIST,szFilter,NULL);
				if(ldFile.DoModal() == IDOK)
				{
					CIniFileProcessor::SaveSingleValue(eMEASUREMENTS, ldFile.GetPathName());
					strcpy(g_szMLFilePathName,ldFile.GetPathName());
					bGot = true;
				}
			}
		} while (!bGot);
	}

	return TRUE;
}

void CFDMSApp::ULStartup(bool& bNoUpperLayer, bool& bUpperLayerError, CDialog& rStartUpDlg)
{

	// now, set up the "UL"
	UINT uiNum_Args;
	char **ppszCommand_Line_Args = NULL;

	//Load the full path to the three UL INI files that control configuration.
	CIniFileProcessor::PrepareULInfo();

	if (!CIniFileProcessor::GetULPresent())  // no irs.ini found and/or facility manager not installed: thus no UL is present
	{   
		rStartUpDlg.DestroyWindow();
		return;
	}

	CString Arguments;
	Arguments.Format("%s", m_lpCmdLine);

	//NOTE: This check will ensure that only one instance of the application runs at a time, per facility.
	//In this way, multiple instances of the EXE can run, but only one per configured facility.
	//The assumption is that each facility configuration of the application will use its own
	//uniquely configured INI file for the facility.  The APPLICATIONSETTINGS.INI file, in this
	//example, is used to create a lock.  If the INI file is already open, it is assumed that there
	//is an instance of the application running for this facility.
	if(Review_Tool_GetCmdLineArgs(m_hInstance, __argc, Arguments, &uiNum_Args, &ppszCommand_Line_Args) && 
		Review_Tool_Startup_Check(ULSupport::ULAppSettingsIniPath, APP_NAME, uiNum_Args, (const char**)ppszCommand_Line_Args))
	{
		bNoUpperLayer = false;
		bUpperLayerError = false;
	}
	else
	{
		rStartUpDlg.DestroyWindow();
		return;
	}

	//Register the message that will trigger processing of the message queue.
	m_ProcessUpperLayerMsgQueueMsg = RegisterWindowMessage("PROCESS_UPPER_LAYER_MSG");

	if(!(bNoUpperLayer && bUpperLayerError))
	{
		m_pRT_Title = new RTTitle(0, APP_NAME " - Version " "1.0", "DEVELOPER NAME HERE" " - " "SOMEONE@lanl.gov", FALSE);
		//	m_pMainWnd->ShowWindow(SW_SHOW); //had to be moved down into the bowels of the upper layer
		//	m_pMainWnd->UpdateWindow();//had to be moved down into the bowels of the upper layer
		// see FDMSReview::RT_Review_Begin

		int iFileHandle = 0;

		// Open the ....INI to lock out and prevent other instances of this 
		//application running for the same measurement system. 
		iFileHandle = _open(ULSupport::ULAppSettingsIniPath, ios::in);//, filebuf::sh_none);
		if (iFileHandle == -1)
		{
			TCHAR buff[256];
			sprintf(buff, "%s\r\nAttempt to open %s failed\n", strerror( errno ), ULSupport::ULAppSettingsIniPath );
			MessageBox(NULL, buff, "Error Message",  MB_ICONERROR);
			delete m_pRT_Title;
			rStartUpDlg.DestroyWindow(); //The splash will come up soon....
			bUpperLayerError = true;
			return;
		}

		// Create all the upper layer objects required to establish connections to other review tools.
		if (m_pRT_Title  != NULL &&
			(m_pRT_Init = new RTInit(ULSupport::ULIniPath, 0)) != NULL &&  //read all the UPPER LAYER PARAMETERS
			(m_pRT_Setup = new FDMSSetup) != NULL &&                                                     
			(m_pRT_ImpReimp = new FDMSImpReimp) != NULL &&
			(m_pRT_Review = new FDMSReview) != NULL &&
			(m_pReview_Tool = new Review_Tool(IDR_FDMSTYPE, *m_pRT_Setup, *m_pRT_ImpReimp, 
			*m_pRT_ImpReimp, *m_pRT_Review, *m_pRT_Init, m_pRT_Title)) != NULL) 
		{
			//Creation of all the upper layer objects was successful.  Go ahead and start the 
			//review tool communication client and servers.
			if(m_pReview_Tool)
			{
				rStartUpDlg.DestroyWindow(); //The splash will come up soon....
				m_pReview_Tool->Review_Tool_Begin(TRUE, APP_NAME, __argc , ppszCommand_Line_Args);  

				// exiting here in UL condition

				//Clean up the upper layer objects.
				if(m_pRT_Title) delete m_pRT_Title;
				if(m_pReview_Tool) delete m_pReview_Tool;
				if(m_pRT_Init) delete m_pRT_Init;
				if(m_pRT_Setup) delete m_pRT_Setup;
				if(m_pRT_Review) delete m_pRT_Review;
				if(m_pRT_ImpReimp) delete m_pRT_ImpReimp;
				m_pRT_Title = NULL;
				m_pReview_Tool = NULL;
				m_pRT_Init = NULL;
				m_pRT_Setup = NULL;
				m_pRT_Review = NULL;
				m_pRT_ImpReimp = NULL;

			}
			else 
			{
				rStartUpDlg.DestroyWindow();
				MessageBox(NULL, "Could not read the ULAYR.INI file, running solo.", "Error Message",  MB_ICONERROR);
				bUpperLayerError = true;
			}
		}
		else //creation of one of the upper layer objects failed.  Inform the user.
		{
			rStartUpDlg.DestroyWindow();
			MessageBox(NULL, "Could not start upper layer, running solo.","Error Message", MB_ICONERROR);
		}
		_close(iFileHandle);
	}  // UL and no initial error
	else  //Connection to the database not possible.  Fatal error.
	{
		rStartUpDlg.DestroyWindow();
		MessageBox(NULL,
			"Could not connect to the upper layer, running solo", "FDMS", 
			MB_ICONEXCLAMATION | MB_OK);
	}  // no UL and no error

	if (ppszCommand_Line_Args && *ppszCommand_Line_Args)
		delete [] (*ppszCommand_Line_Args);

}


BOOL CFDMSApp::DoFinalInitialization(const bool  bSplash)
{
	CString newTitle, nt;

	SetAllMessagesOnHold(true);

	if (m_bDemoVersion)
		nt.LoadString(IDR_DMAINFRAME);
	else
		nt.LoadString(IDR_MAINFRAME);

#ifdef _DEBUG
	newTitle.Format("%s %s (%s)", nt, CAboutDlg::m_sVer, CAboutDlg::m_sTime);
#else
	newTitle.Format("%s %s", nt, CAboutDlg::m_sVer);
#endif 

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	pMainFrame->SetWindowText(newTitle);
	pMainFrame->SetTitle(newTitle);
	AfxGetMainWnd()->SetWindowText(newTitle);
	m_pMainWnd = pMainFrame;


	GetSavedBkgMeasurements();
	ReadDatabase();
	UpdateGraphData();

	AddUserInfoToMenus((CMainFrame*)m_pMainWnd, eMain);

	CreateToolMenu((CMainFrame*)m_pMainWnd);
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	/*
	if (!m_pMainWnd->IsIconic() && bSplash &&
			m_splash.Create(m_pMainWnd))
		{
			m_splash.ShowWindow(SW_SHOW);
			m_splash.UpdateWindow();
			m_splash.SetTimer(1, 500, NULL);
	}
	m_dwSplashTime = ::GetCurrentTime();
	*/

	//TestRADFDMS();
	//TestFDMSRAD();

	m_pLogLog = new LoggingLog(m_pMainWnd);
	m_pLogLog->Create();
	m_pLogLog->ShowWindow(SW_HIDE);


	SetAllMessagesOnHold(false);

	return TRUE;

}


void CFDMSApp::GetDeltaEnrichmentValue(float& fv)
{
	char temp[_MAX_PATH];
	GETPROFSTR(eDELTA_ENRICHMENT,temp,sizeof(temp));
	fv = (float)atof(temp);
}

void CFDMSApp::GetDeltaCoolingTimeValue(float& fv)
{
	char temp[_MAX_PATH];
	GETPROFSTR(eDELTA_COOLINGTIME,temp,sizeof(temp));
	fv = (float)atof(temp);
}

void CFDMSApp::ClearMPEntry(int iIndex)
{
	if (iIndex < 0 || iIndex > g_iMPEntriesIn - 1)
		return;

	TRACE("Clear MP[%d] of %d\n", iIndex, g_iMPEntriesIn);

	g_iMPItemIndices[iIndex] = -1;
	g_szMPFacilityNames[iIndex][0] = NULL;
	g_szMPItemIDs[iIndex][0] = NULL;
	g_iMPMeasurementType[iIndex] = 0;
	g_fMPEnrichmentPercent[iIndex] = 0.0;
	g_fMPBurnUp[iIndex] = 0.0;
	g_fMPLogCoolingTime[iIndex] = 0.0;
	g_fMPLogGammaDose[iIndex] = 0.0;
	g_iMPStatus[iIndex] = eUnmeasured;
	g_iMPDischargeDateDay[iIndex] = 0;
	g_iMPDischargeDateMonth[iIndex] = 0;
	g_iMPDischargeDateYear[iIndex] = 0;
	g_iMPCycle[iIndex] = 0;
	g_fMPNChanAThresh[iIndex] = 0.0;
	g_fMPNChanBThresh[iIndex] = 0.0;
	g_iMPMeasurementDateDay[iIndex] = 0;
	//g_iMPMeasurementStat[iIndex] = eUnmeasured;
	g_iMPMeasurementDateMonth[iIndex] = 0;
	g_iMPMeasurementDateYear[iIndex] = 0;
	g_fMPCoolingTime[iIndex] = 0.0;
	g_fMPNChanA[iIndex] = 0.0;
	g_fMPNChanB[iIndex] = 0.0;
	g_fMPNChanC[iIndex] = 0.0;
	g_fMPGDose1[iIndex] = 0.0;
	g_fMPGDose2[iIndex] = 0.0;
	g_fMPLogBurnup[iIndex] = 0.0;
	g_fMPLogDecayCorTotNeutrons[iIndex] = 0.0;
	g_fMPLogNoverG[iIndex] = 0.0;
	g_szMPDetectorIDs[iIndex][0] = NULL;
}

static void gll_szUtilCopy(char* t, char* x, const int el_maxo)
{
	int len = strlen(x);
	len = (len < el_maxo ? len : el_maxo - 1);
	memset(t, 0, el_maxo);
	strncpy(t, x, len);
	t[el_maxo - 1] = 0;
}

bool CFDMSApp::RemoveGlobalMPEntry(int iIndex)
{
	bool bRemoved = false;

	// out:of:range guard
	if (iIndex < 0 || iIndex > g_iMPEntriesIn - 1)
		return bRemoved;

	// removing last element in the array
	if (iIndex == (g_iMPEntriesIn - 1))
	{
		bRemoved = true;  // implicitly removed by simply decrementing the counter
	}

	if (iIndex == 0)
	{
		// dealing with the first in the array
		g_iMPItemIndices[iIndex];
	}
	// for all elements less than last element AND greater or equal to the target element, shift them
	int i;
	for (i = iIndex; i < (g_iMPEntriesIn - 1); i++)
	{
		bRemoved = true;
		gll_szUtilCopy(g_szMPFacilityNames[i], g_szMPFacilityNames[i+1], MAX_NAME_LENGTH);
		gll_szUtilCopy(g_szMPItemIDs[i], g_szMPItemIDs[i+1], MAX_ID_LENGTH);

		g_iMPMeasurementType[i] = g_iMPMeasurementType[i+1];
		g_fMPEnrichmentPercent[i] = g_fMPEnrichmentPercent[i+1];
		g_fMPBurnUp[i] =		g_fMPBurnUp[i+1];
		g_fMPLogCoolingTime[i] =g_fMPLogCoolingTime[i+1];
		g_fMPLogGammaDose[i] =	g_fMPLogGammaDose[i+1];
		g_iMPStatus[i] =		g_iMPStatus[i+1];
		g_iMPDischargeDateDay[i] = g_iMPDischargeDateDay[i+1];
		g_iMPDischargeDateMonth[i] = g_iMPDischargeDateMonth[i+1];
		g_iMPDischargeDateYear[i] = g_iMPDischargeDateYear[i+1];
		g_iMPCycle[i] =			g_iMPCycle[i+1];
		g_fMPNChanAThresh[i] =  g_fMPNChanAThresh[i+1];
		g_fMPNChanBThresh[i] =  g_fMPNChanBThresh[i+1];
		g_iMPMeasurementDateDay[i] = g_iMPMeasurementDateDay[i+1];
		//g_iMPMeasurementStat[i] = g_iMPMeasurementStat[i+1];
		g_iMPMeasurementDateMonth[i] = g_iMPMeasurementDateMonth[i+1];
		g_iMPMeasurementDateYear[i] = g_iMPMeasurementDateYear[i+1];
		g_fMPCoolingTime[i] =	g_fMPCoolingTime[i+1];
		g_fMPNChanA[i] =		g_fMPNChanA[i+1];
		g_fMPNChanB[i] =		g_fMPNChanB[i+1];
		g_fMPNChanC[i] =		g_fMPNChanC[i+1];
		g_fMPGDose1[i] =		g_fMPGDose1[i+1];
		g_fMPGDose2[i] =		g_fMPGDose2[i+1];
		g_fMPLogBurnup[i] =		g_fMPLogBurnup[i+1];
		g_fMPLogDecayCorTotNeutrons[i] = g_fMPLogDecayCorTotNeutrons[i+1];
		g_fMPLogNoverG[i] = g_fMPLogNoverG[i+1];
		g_iMPItemIndices[i] = i; // carry the real index along with the entry data

		gll_szUtilCopy(g_szMPDetectorIDs[i], g_szMPDetectorIDs[i+1], MAX_NAME_LENGTH);
	}
	ClearMPEntry(i);  // skips < 0 and > # (entries -1)
	if (bRemoved)
		g_iMPEntriesIn--;
	return bRemoved;
}

void CFDMSApp::FindMenuItem(CMenu* pTargMenu, LPCTSTR MenuString, int& pos, CMenu*& pMenu)
{
	ASSERT(pTargMenu);
	//ASSERT(::IsMenu(Menu->GetSafeHmenu()));

	int count = pTargMenu->GetMenuItemCount();
	for (int i = 0; i < count; i++)
	{
		CString str;
		if (pTargMenu->GetMenuString(i, str, MF_BYPOSITION) &&
			(strcmp(str, MenuString) == 0))
		{
			pMenu = pTargMenu->GetSubMenu(i);
			pos = i;
			return;
		}
	}
	pMenu = NULL;
	pos =  -1;
}


void CFDMSApp::AddUserInfoToMenus(CFrameWnd* pFrame, tMenuType index)
{
	try
	{
		CMenu* pTopMenu = pFrame->GetMenu();

		TRACE2("AddUserInfoToMenus %x %x\n", pTopMenu, pFrame);

		int pos;
		CMenu* pMenu;
		FindMenuItem(pTopMenu, "&Edit", pos, pMenu);

		if (!m_bMenusConstructed[index] && pMenu != NULL)
		{
			if (index < eULMenu)
			{
				CString menuTitle;
				int ite = 0;
				for (int mid = ID_EDIT_BACKGROUNDCYCLES; mid <= ID_EDIT_DETECTOR_ID; mid += MENU_SPAN, ite++)
				{
					CMenu* px = pMenu->GetSubMenu(ite);
					if (px)
					{
						px->DeleteMenu(0, MF_BYPOSITION);  // hack attack, remove the unused separator
					}
					LoadCustomMenuValues(px, mid, index);
				}

				// create basic confimation entries
				if (index == 0)
					InitializeConfirmationValues(pMenu);
			}
			else if (index == eULMenu)
			{
			}
			m_bMenusConstructed[index] = true;
		}

	} catch (...)
	{
	}

}


void CFDMSApp::InitializeConfirmationValues(CMenu* pMenu)
{
	bool bv;

	GETPROFBOOL(eBGCYCLES_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "974007723",(bv ? 0 : IDYES));

	GETPROFBOOL(eBGCYCLETIME_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "4195034951", (bv ? 0 : IDYES));

	GETPROFBOOL(eCYCLES_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "2629986530", (bv ? 0 : IDYES));

	GETPROFBOOL(eCYCLETIME_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "3326617086",  (bv ? 0 : IDYES));

	GETPROFBOOL(eDELTA_ENRICHMENT_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "220898687", (bv ? 0 : IDYES));

	GETPROFBOOL(eDELTA_COOLINGTIME_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "3485736876", (bv ? 0 : IDYES));

	GETPROFBOOL(eDETECTOR_ID_CHOICE_CONF,bv);
	CRegWrapper::SetPrivateIniInt(CAskOnceBox::ASK_ONCE_KEY_NAME, "814570223", (bv ? 0 : IDYES));

	/*********
	Windows Registry Editor Version 5.00

	[HKEY_CURRENT_USER\Software\LANL\FDMS\ao]
	"974007723"=dword:00000000
	"974007723txt"="Change the current value of the 'Background Cycles' setting?"
	"4195034951"=dword:00000000
	"4195034951txt"="Change the current value of the 'Background Cycle Time' setting?"
	"220898687"=dword:00000000
	"220898687txt"="Change the current value of the 'Delta Enrichment' setting?"
	"3485736876"=dword:00000000
	"3485736876txt"="Change the current value of the 'Delta Cooling Time' setting?"
	"2629986530"=dword:00000000
	"2629986530txt"="Change the current value of the 'Standard Measurement Cycles' setting?"
	"3326617086"=dword:00000000
	"3326617086txt"="Change the current value of the 'Standard Measurement Cycle Time' setting?"
	"814570223"=dword:00000000
	"814570223txt"="Change the current value of the 'Detector ID' setting?"

	*******/

	// config file menu item check
	{
		TCHAR CONFIG_O_RAMA_KEY_NAME[] = "cfg";
		LONG lRecalledState = 0;
		TCHAR CONFIG_O_RAMA_KEY_VALUE[] = "cfg";

		LONG lRes = CRegWrapper::GetPrivateIniInt(CONFIG_O_RAMA_KEY_NAME,(LPCTSTR)CONFIG_O_RAMA_KEY_VALUE,&lRecalledState);

		if (lRes == 0x0 && lRecalledState != 0x0)
		{
			CString menuTitle;
			pMenu->AppendMenu(MF_SEPARATOR);
			menuTitle.LoadString(ID_EDIT_CONFIG_O_RAMA_D);
			pMenu->AppendMenu(MF_STRING,ID_EDIT_CONFIG_O_RAMA_D,menuTitle);
			menuTitle.LoadString(ID_EDIT_CONFIG_O_RAMA_C);
			pMenu->AppendMenu(MF_STRING,ID_EDIT_CONFIG_O_RAMA_C,menuTitle);
		}
	}
}

void CFDMSApp::UnitizeCustomMenuEntry(CString& entry, UINT nID)
{
	switch (nID)
	{
	case ID_EDIT_BACKGROUNDCYCLES:
	case ID_EDIT_BACKGROUNDCYCLETIME:
	case ID_EDIT_DELTAENRICHMENT:
	case ID_EDIT_DELTACOOLINGTIME:
	case ID_EDIT_STANDARDMEASUREMENTCYCLES:
	case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
		{
			CString s;
			s.LoadString(nID+1000); // hack attack
			if (s.GetLength() > 0)
			{
				entry.Append(s);
				//entry.Append(" ");
			}
		}
		break;
	default:
		break;
	}
}

void CFDMSApp::NotifyMainChangedPrefs()
{
	m_bPrefChangedMain = true;
}


void CFDMSApp::NotifyChildChangedPrefs()
{
	m_bPrefChangedChild = true;
	((CMainFrame*)m_pMainWnd)->bNeedsMenuUpdate = true;
}

bool   CFDMSApp::bNeedsMenuUpdate() const
{
	return 	m_bPrefChangedChild ||  m_bPrefChangedMain;
}


void CFDMSApp::CheckCustomMenuItems(CFrameWnd* pFWin, bool bChild)
{
	if (pFWin == NULL) return;

	if (!(m_bPrefChangedChild ||  m_bPrefChangedMain) && !bChild) // nothing changed
		return;

	for (int zid = ID_EDIT_BACKGROUNDCYCLES; zid <= ID_EDIT_DETECTOR_ID; zid += MENU_SPAN)
	{

		CMenu* 	pEstateMenu = GetPopUpForPref(pFWin, zid);
		if (pEstateMenu == NULL)
			continue;

		tIniNames curval = GetRelatedIniVal(zid);;

		char cCurVal[128];
		GETPROFSTR(curval,cCurVal,sizeof(cCurVal));

		int cival = 0;
		float cfval = 0.0;
		switch (zid)
		{
		case  ID_EDIT_BACKGROUNDCYCLES:
		case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
		case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
		case  ID_EDIT_BACKGROUNDCYCLETIME:
			sscanf(cCurVal,"%d", &cival);
			break;
		case  ID_EDIT_DELTAENRICHMENT:
		case  ID_EDIT_DELTACOOLINGTIME:
			sscanf(cCurVal, "%g", &cfval);
			break;
		case  ID_EDIT_DETECTOR_ID:
			break;
		default:
			break;
		}

		//	CString pre_ent;
		//	BOOL g;
		int mid;

		int len = pEstateMenu->GetMenuItemCount();

		for (int i = 0; i < len; i++)
		{
			int ival = 0;
			float fval = 0.0;
			bool bCheck = false;
			mid = zid + (i+1);

			CString str;
			//int xyz = pEstateMenu->GetMenuString(mid, str, MF_BYCOMMAND);
			GetMenuItemValue(str, zid);  // take off any dross
			str.Trim();

			// get the real value
			switch (zid)
			{
			case  ID_EDIT_BACKGROUNDCYCLES:
			case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
			case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
			case  ID_EDIT_BACKGROUNDCYCLETIME:
				sscanf(str,"%d", &ival);
				if (ival == cival)
					bCheck = true;
				else
					bCheck = false;
				break;
			case  ID_EDIT_DELTAENRICHMENT:
			case  ID_EDIT_DELTACOOLINGTIME:
				sscanf(str, "%g", &fval);
				if (fval == cfval)
					bCheck = true;
				else
					bCheck = false;
				break;
			case  ID_EDIT_DETECTOR_ID:
				if (str.Compare(cCurVal) == 0)
					bCheck = true;
				else
					bCheck = false;
				break;
			default:
				break;
			}

			if (bCheck)
				pEstateMenu->CheckMenuItem(mid, MF_CHECKED | MF_BYCOMMAND);
			else
				pEstateMenu->CheckMenuItem(mid, MF_UNCHECKED   | MF_BYCOMMAND   );
		}
	}

	if (bChild) // child window changing a child menu, need to update main menu too
	{
		m_bPrefChangedMain = false; //any changes from main would have been picked up so turn the flag off
	}
	else
	{
		// main window updating due to child window change
		m_bPrefChangedChild = false;
	}
}


void CFDMSApp::LoadCustomMenuValues(CMenu* pEstateMenu, int zid, int index)
{
	if (pEstateMenu == NULL)
		return;
	tIniNames subject, curval;

	switch (zid)
	{
	case  ID_EDIT_BACKGROUNDCYCLES:
		curval = eBGCYCLES;
		subject = eBGCYCLES_CHOICE;
		break;
	case  ID_EDIT_BACKGROUNDCYCLETIME:
		curval = eBGCYCLETIME;
		subject = eBGCYCLETIME_CHOICE;
		break;
	case  ID_EDIT_DELTAENRICHMENT:
		curval = eDELTA_ENRICHMENT;
		subject = eDELTA_ENRICHMENT_CHOICE;
		break;
	case  ID_EDIT_DELTACOOLINGTIME:
		curval = eDELTA_COOLINGTIME;
		subject = eDELTA_COOLINGTIME_CHOICE;
		break;
	case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
		curval = eCYCLES;
		subject = eCYCLES_CHOICE;
		break;
	case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
		curval = eCYCLETIME;
		subject = eCYCLETIME_CHOICE;
		break;
	case  ID_EDIT_DETECTOR_ID:
		curval = eDETECTOR_ID;
		subject = eDETECTOR_ID_CHOICE;
		break;
	default:
		return;
	}

	CStringArray array;
	int len = 64;

	GETPROFSTRARRAY(subject, array, len);

	char cCurVal[128];
	GETPROFSTR(curval,cCurVal,sizeof(cCurVal));

	int cival = 0;
	float cfval = 0.0;
	switch (zid)
	{
	case  ID_EDIT_BACKGROUNDCYCLES:
	case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
	case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
	case  ID_EDIT_BACKGROUNDCYCLETIME:
		sscanf(cCurVal,"%d", &cival);
		break;
	case  ID_EDIT_DELTAENRICHMENT:
	case  ID_EDIT_DELTACOOLINGTIME:
		sscanf(cCurVal, "%g", &cfval);
		break;
	case  ID_EDIT_DETECTOR_ID:
		break;
	default:
		break;
	}

	CString pre_ent;
	BOOL g;
	int mid;

	for (int i = 0; i < len; i++)
	{
		int ival = 0;
		float fval = 0.0;
		bool bCheck = false;

		mid = zid + (i+1);
		pre_ent = array.ElementAt(i);
		pre_ent.Trim();

		// get the real value
		switch (zid)
		{
		case  ID_EDIT_BACKGROUNDCYCLES:
		case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
		case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
		case  ID_EDIT_BACKGROUNDCYCLETIME:
			sscanf(pre_ent,"%d", &ival);
			if (ival == cival)
				bCheck = true;
			else
				bCheck = false;
			break;
		case  ID_EDIT_DELTAENRICHMENT:
		case  ID_EDIT_DELTACOOLINGTIME:
			sscanf(pre_ent, "%g", &fval);
			if (fval == cfval)
				bCheck = true;
			else
				bCheck = false;
			break;
		case  ID_EDIT_DETECTOR_ID:
			if (pre_ent.Compare(cCurVal) == 0)
				bCheck = true;
			else
				bCheck = false;
			break;
		default:
			break;
		}

		// build the fixed menus
		if (!m_bMenusConstructed[index])
		{
			switch (zid)
			{
			case  ID_EDIT_BACKGROUNDCYCLES:
			case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
			case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
			case  ID_EDIT_BACKGROUNDCYCLETIME:
				pre_ent.Format("% 4d", ival);
				break;
			case  ID_EDIT_DELTAENRICHMENT:
			case  ID_EDIT_DELTACOOLINGTIME:
				pre_ent.Format("%5.2lf", fval);
				break;
			case  ID_EDIT_DETECTOR_ID:
				break;
			default:
				break;
			}
			UnitizeCustomMenuEntry(pre_ent, zid);
			g = pEstateMenu->AppendMenu(MF_STRING, mid, pre_ent);
		}

		// do the initial check
		if (bCheck)
			pEstateMenu->CheckMenuItem(mid, MF_CHECKED | MF_BYCOMMAND);
	}

}



void CFDMSApp::CreateToolMenu(CFrameWnd* pFrame)
{
	CMenu* pTopMenu = pFrame->GetMenu();
	int pos;
	CMenu* pMenu;
	FindMenuItem(pTopMenu, "&Tools", pos, pMenu);
	if (pMenu)
	{
		pMenu->DeleteMenu(0, MF_BYPOSITION);  // hack attack, remove the unused separator
	}
	else
		return;

	if (((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_bULToolsAvailable)
	{
		BOOL b;
		unsigned i;
		int j;
		CString s;
		for (j = 0; j < ((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_ToolBox.GetCount(); j++)
		{
			i = ((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_ToolBox.GetItemData(j);
			((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_ToolBox.GetLBText(j, s);
			b = pMenu->AppendMenu(MF_STRING, ID_TOOLMENUID + j, s);
		}

		if (((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_bImportSourceAvailable)
		{
			CString is;
			is.LoadString(ID_TOOLS_IMPORT);
			b = pMenu->AppendMenu(MF_STRING, ID_TOOLS_IMPORT, is);
		}
	}
	else
		pTopMenu->EnableMenuItem(pos, MF_DISABLED | MF_GRAYED);

}

CMenu* CFDMSApp::GetPopUpForPref(CWnd* pFWin, UINT nID)
{
	UINT pos = ((nID - ID_EDIT_BACKGROUNDCYCLES)/MENU_SPAN);// + 1;
	//CMenu* pEstateMenu = NULL;
	CMenu* pTopMenu = NULL;
	if (pFWin)
		pTopMenu = pFWin->GetMenu();
	else
		return NULL;

	int epos;
	CMenu* pEditMenu;
	FindMenuItem(pTopMenu, "&Edit", epos, pEditMenu);

	CMenu* pPrimary = pEditMenu->GetSubMenu(pos);

	return pPrimary;
}

tIniNames CFDMSApp::GetRelatedIniVal(UINT nID)
{
	tIniNames curval;

	switch (nID)
	{
	case  ID_EDIT_BACKGROUNDCYCLES:
		curval = eBGCYCLES;
		break;
	case  ID_EDIT_BACKGROUNDCYCLETIME:
		curval = eBGCYCLETIME;
		break;
	case  ID_EDIT_DELTAENRICHMENT:
		curval = eDELTA_ENRICHMENT;
		break;
	case  ID_EDIT_DELTACOOLINGTIME:
		curval = eDELTA_COOLINGTIME;
		break;
	case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
		curval = eCYCLES;
		break;
	case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
		curval = eCYCLETIME;
		break;
	case ID_EDIT_DETECTOR_ID:
		curval = eDETECTOR_ID;
		break;
	default:
		curval = ININAMECOUNT;
		break;
	}
	return curval;
}

void CFDMSApp::SetRelatedGlobalValue(tIniNames nID)
{
	switch (nID)
	{
	case  eBGCYCLES:
		break;
	case  eBGCYCLETIME:
		break;
	case  eDELTA_ENRICHMENT:
		GetDeltaEnrichmentValue(g_fDeltaEnrichment);
		break;
	case  eDELTA_COOLINGTIME:
		GetDeltaCoolingTimeValue(g_fDeltaCoolingTime);
		break;
	case  eCYCLES:
		break;
	case eCYCLETIME:
		break;
	case eDETECTOR_ID:
		break;
	default:
		break;
	}
}

void CFDMSApp::GetMenuItemValue(CString& menustr, UINT nScopeID)
{
	int ival = 0;
	float fval = 0.0;
	switch (nScopeID)
	{
	case  ID_EDIT_BACKGROUNDCYCLES:
	case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
	case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
	case  ID_EDIT_BACKGROUNDCYCLETIME:
		sscanf(menustr,"%d", &ival);
		break;
	case  ID_EDIT_DELTAENRICHMENT:
	case  ID_EDIT_DELTACOOLINGTIME:
		sscanf(menustr, "%g", &fval);
		break;
	case  ID_EDIT_DETECTOR_ID:
		return;
		break;
	default:
		return;
		break;
	}

	switch (nScopeID)
	{
	case  ID_EDIT_BACKGROUNDCYCLES:
	case  ID_EDIT_STANDARDMEASUREMENTCYCLES:
	case ID_EDIT_STANDARDMEASUREMENTCYCLETIME:
	case  ID_EDIT_BACKGROUNDCYCLETIME:
		menustr.Format("%d", ival);
		break;
	case  ID_EDIT_DELTAENRICHMENT:
	case  ID_EDIT_DELTACOOLINGTIME:
		menustr.Format("%.2lf", fval);
		break;
	case  ID_EDIT_DETECTOR_ID:
		break;
	default:
		break;
	}
}

void CFDMSApp::SetTemporaryIniValue(CMenu* pMenu, UINT nID, UINT nScopeID)
{
	if (!pMenu)
		return;

	CString str;
	int xyz = pMenu->GetMenuString(nID, str, MF_BYCOMMAND);

	//("JFL todo: Warning this must be thoroughly tested, the GetMenuString operation does not always work correctly!!!!!");

	GetMenuItemValue(str, nScopeID);  // take off any dross

	// Now set value in str to be the real value for this preference entry
	tIniNames id = GetRelatedIniVal(nScopeID);

	CString question;
	question.LoadString(id + SETTING_eBASEVALUE);
	question.Insert(0, "Change the current value of the '");
	question.Append("' setting?");
	CAskOnceBox x (AfxGetMainWnd(), question, "Change Value?");

	// Create and show the dialog box
	INT_PTR nRet = -1;
	nRet = x.AskOnce();

	switch ( nRet )
	{
	case -1: 
		AfxMessageBox("Dialog box could not be created!");
		break;
	case IDYES:
		// save the new selection value
		CIniFileProcessor::SaveSingleValue(id, str.GetBuffer()); 
		CheckMenuItem(pMenu, nID);  // check the menu
		SetRelatedGlobalValue(id);	// save the value into the globals, if required
		RefreshRelatedPlotz(nScopeID);  // update the graphs, if required
		break;
	case IDNO:
		// Don't do anything
		break;
	default:
		// never get here
		break;
	};
}

void CFDMSApp::RefreshRelatedPlotz(UINT nScopeID)
{
	switch (nScopeID)
	{
	case  ID_EDIT_DELTAENRICHMENT:
	case  ID_EDIT_DELTACOOLINGTIME:
		UpdateGraphData();
		UpdateAllDocs();
		break;
	default:
		break;
	}
}

void CFDMSApp::CheckMenuItem(CMenu * pMenu, UINT nID)
{
	if (pMenu)
	{
		int count = pMenu->GetMenuItemCount();
		for (int i = 0; i < count; i++)
		{
			pMenu->CheckMenuItem(i, MF_UNCHECKED   | MF_BYPOSITION   );
		}
		pMenu->CheckMenuItem(nID, MF_CHECKED   | MF_BYCOMMAND   );
	}
}

void CFDMSApp::OnEditResetacknowledgements()
{
	CAskOnceBox::ResetAllAskOnceVlaues();
}

void CFDMSApp::OnEditPrefDCT(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_DELTACOOLINGTIME);
	NotifyMainChangedPrefs();
}
void CFDMSApp::OnEditPrefDE(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_DELTAENRICHMENT);
	NotifyMainChangedPrefs();
}
void CFDMSApp::OnEditPrefSMC(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_STANDARDMEASUREMENTCYCLES);
	NotifyMainChangedPrefs();
}
void CFDMSApp::OnEditPrefSMCT(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_STANDARDMEASUREMENTCYCLETIME);
	NotifyMainChangedPrefs();
}
void CFDMSApp::OnEditPrefBGC(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_BACKGROUNDCYCLES);
	NotifyMainChangedPrefs();
}
void CFDMSApp::OnEditPrefBGCT(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_BACKGROUNDCYCLETIME);
	NotifyMainChangedPrefs();
}
void CFDMSApp::OnEditPrefDID(UINT nID)
{
	CMenu* pMenu = GetPopUpForPref(AfxGetMainWnd(), nID);
	SetTemporaryIniValue(pMenu, nID, ID_EDIT_DETECTOR_ID);
	NotifyMainChangedPrefs();
}

void CFDMSApp::OnToolGoose(UINT nID)
{

	// The menu entry ID_TOOLMENUID + j should invoke tool 'i'
	// It should do this by:
	if (((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_bULToolsAvailable)
	{
		int j = nID - ID_TOOLMENUID;
		((CMainFrame*)m_pMainWnd)->m_wndToolBar.StartTheTool(j);
	}

}

void CFDMSApp::OnImportButton()
{
	if (((CMainFrame*)m_pMainWnd)->m_wndToolBar.m_bULToolsAvailable)
	{
		((CMainFrame*)m_pMainWnd)->OnImportButton();
	}
}

void CFDMSApp::ReadDatabase()
{
	CHARACTER C0(g_szDBFilePathName,280);
	CHARACTER C1(g_szDBFileHeader,200); // todo: jfl init to all null, or null terminate prior to usage
	CHARACTER C2((char*)g_szFacilityNames,MAX_NAME_LENGTH);
	CHARACTER C3((char*)g_szItemIDs,MAX_ID_LENGTH);
	CHARACTER _C4((char*)g_szDetectorIDs,MAX_NAME_LENGTH);  // add to arg list

	READDATABASE(
		C0,	//280
		&g_iEntriesInDB,
		C1, //200
		C2, //MAX_NAME_LENGTH
		C3, //MAX_ID_LENGTH
		g_iMeasurementType,
		g_iStatus,
		g_fEnrichmentLevel,
		g_fBurnUp,
		g_iDischargeDateDay,
		g_iDischargeDateMonth,
		g_iDischargeDateYear,
		g_iCycle,
		g_fNChanAThresh,
		g_fNChanBThresh,
		g_iMeasurementDateDay,
		g_iMeasurementDateMonth,
		g_iMeasurementDateYear,
		g_fCoolingTime,
		g_fNChanA,
		g_fNChanB,
		g_fNChanC,
		g_fGDose1,
		g_fGDose2,
		_C4,
		g_fLogNoverG,
		g_fLogBurnup,
		g_fLogCoolingTime,
		g_fLogDecayCorTotNeutrons,
		g_fLogGammaDose,
		&g_iError
		);

	char *szTemp = C1(0);  // this somehow puts a terminator in the string, but I don't understand this part
	for (int i = 0; i < g_iEntriesInDB; i++)
	{
		szTemp = C2(i);
		szTemp = C3(i);
		szTemp = _C4(i);
	}

	if (g_iError != 0)
		AfxMessageBox("Error Reading Database File",MB_OK);

	CHARACTER C4(g_szMPFilePathName,280);
	CHARACTER C5(g_szMPFileHeader,200);
	CHARACTER C6((char*)g_szMPFacilityNames,MAX_NAME_LENGTH);
	CHARACTER C7((char*)g_szMPItemIDs,MAX_ID_LENGTH);
	CHARACTER C8((char*)g_szMPDetectorIDs,MAX_NAME_LENGTH);  // add to arg list

	READMEASUREMENTPLAN(
		C4,//280					//measurement plan file path name 
		&g_iMPEntriesIn,			// 
		C5,//200					//measurement plan csv header
		C6,//MAX_NAME_LENGTH						//facility names
		C7,//MAX_ID_LENGTH						//Item IDs
		g_iMPMeasurementType,		//	
		g_iMPStatus,				//
		g_fMPEnrichmentPercent,		//	
		g_fMPBurnUp,				//
		g_iMPDischargeDateDay,		//dcdateday,
		g_iMPDischargeDateMonth,	//dcdatemonth,
		g_iMPDischargeDateYear,		//dcdateyear,
		g_iMPCycle,					//icycle,
		g_fMPNChanAThresh,			//thresA,
		g_fMPNChanBThresh,			//thresB,
		g_iMPMeasurementDateDay,	//mdateday,
		g_iMPMeasurementDateMonth,	//mdatemonth,
		g_iMPMeasurementDateYear,	//mdateyear,
		g_fMPCoolingTime,			//ct,
		g_fMPNChanA,				//na,
		g_fMPNChanB,				//nb,
		g_fMPNChanC,				//nc,
		g_fMPGDose1,				//gamma1,
		g_fMPGDose2,				//gamma2,
		C8,							// detector
		g_fMPLogNoverG,
		g_fMPLogBurnup,				//logbu,
		g_fMPLogCoolingTime,		//logct,
		g_fMPLogDecayCorTotNeutrons,//logdcn,
		g_fMPLogGammaDose,			//loggamma,
		&g_iError					//ierror
		);
	if (g_iError != 0)
		AfxMessageBox("Error Reading Measurement Plan File",MB_OK);
	szTemp = C5(0);
	int i;
	for (i = 0; i < g_iMPEntriesIn; i++)
	{
		szTemp = C6(i);
		szTemp = C7(i);
		szTemp = C8(i);
	}

	for (i = 0; i < g_iMPEntriesIn; i++)
	{

		g_iMPItemIndices[i] = i;
	}
}

void CFDMSApp::WriteDatabase()
{
	
	CHARACTER C0(g_szDBFilePathName,280);
	CHARACTER C1(g_szDBFileHeader,200);
	CHARACTER C2((char*)g_szFacilityNames,MAX_NAME_LENGTH);
	CHARACTER C3((char*)g_szItemIDs,MAX_ID_LENGTH);
	CHARACTER C4((char*)g_szDetectorIDs,MAX_NAME_LENGTH);

	C0(0);
	C1(0);
	C2(0);
	C3(0);
	C4(0);
	
	WRITEDATABASE(
		C0,	//280
		&g_iEntriesInDB,
		C1, //200
		C2, //MAX_NAME_LENGTH
		C3, //MAX_ID_LENGTH
		g_iMeasurementType,
		g_iStatus,
		g_fEnrichmentLevel,
		g_fBurnUp,
		g_iDischargeDateDay,
		g_iDischargeDateMonth,
		g_iDischargeDateYear,
		g_iCycle,
		g_fNChanAThresh,
		g_fNChanBThresh,
		g_iMeasurementDateDay,
		g_iMeasurementDateMonth,
		g_iMeasurementDateYear,
		g_fCoolingTime,
		g_fNChanA,
		g_fNChanB,
		g_fNChanC,
		g_fGDose1,
		g_fGDose2,
		C4
		);


}

void CFDMSApp::WriteMeasurementPlan()
{
	CHARACTER C4(g_szMPFilePathName,280);
	CHARACTER C5(g_szMPFileHeader,200);
	CHARACTER C6((char*)g_szMPFacilityNames,MAX_NAME_LENGTH);
	CHARACTER C7((char*)g_szMPItemIDs,MAX_ID_LENGTH);
	CHARACTER C8((char*)g_szMPDetectorIDs,MAX_NAME_LENGTH);

	C4(0);
	C5(0);
	C6(0);
	C7(0);
	C8(0);

	WRITEMEASUREMENTPLAN(
		C4,//280					//measurement plan file path name 
		&g_iMPEntriesIn,			// 
		C5,//200					//measurement plan csv header
		C6,//MAX_NAME_LENGTH						//facility names
		C7,//MAX_ID_LENGTH						//Item IDs
		g_iMPMeasurementType,		//	
		g_iMPStatus,				//
		g_fMPEnrichmentPercent,		//	
		g_fMPBurnUp,				//
		g_iMPDischargeDateDay,		//dcdateday,
		g_iMPDischargeDateMonth,	//dcdatemonth,
		g_iMPDischargeDateYear,		//dcdateyear,
		g_iMPCycle,					//icycle,
		g_fMPNChanAThresh,			//thresA,
		g_fMPNChanBThresh,			//thresB,
		g_iMPMeasurementDateDay,	//mdateday,
		g_iMPMeasurementDateMonth,	//mdatemonth,
		g_iMPMeasurementDateYear,	//mdateyear,
		g_fMPCoolingTime,			//ct,
		g_fMPNChanA,				//na,
		g_fMPNChanB,				//nb,
		g_fMPNChanC,				//nc,
		g_fMPGDose1,				//gamma1,
		g_fMPGDose2,					//gamma2,
		C8							// detector ids
		);
}

void CFDMSApp::UpdateMeasurements(int i)  // i++ adjusted for fortran
{
	CHARACTER C4(g_szMLFilePathName,280);
	CHARACTER C5(g_szMPFileHeader,200);
	CHARACTER C6((char*)g_szMPFacilityNames,MAX_NAME_LENGTH);
	CHARACTER C7((char*)g_szMPItemIDs,MAX_ID_LENGTH);
	CHARACTER C8((char*)g_szMPDetectorIDs,MAX_NAME_LENGTH);

	C4(0);
	C5(0);
	C6(0);
	C7(0);
	C8(0);

	try
	{

		UPDATEMEASUREMENTS(
			C4,//280					//measurements file path name 
			&i,			// 
			C5,//200					//measurement plan csv header
			C6,//MAX_NAME_LENGTH						//facility names
			C7,//MAX_ID_LENGTH						//Item IDs
			g_iMPMeasurementType,		//	
			g_iMPStatus,				//
			g_fMPEnrichmentPercent,		//	
			g_fMPBurnUp,				//
			g_iMPDischargeDateDay,		//dcdateday,
			g_iMPDischargeDateMonth,	//dcdatemonth,
			g_iMPDischargeDateYear,		//dcdateyear,
			g_iMPCycle,					//icycle,
			g_fMPNChanAThresh,			//thresA,
			g_fMPNChanBThresh,			//thresB,
			g_iMPMeasurementDateDay,	//mdateday,
			g_iMPMeasurementDateMonth,	//mdatemonth,
			g_iMPMeasurementDateYear,	//mdateyear,
			g_fMPCoolingTime,			//ct,
			g_fMPNChanA,				//na,
			g_fMPNChanB,				//nb,
			g_fMPNChanC,				//nc,
			g_fMPGDose1,				//gamma1,
			g_fMPGDose2,				//gamma2,
			C8							// detector ids
			);

	} catch (...)
	{
		TRACE("UPDATEMEASUREMENTS exception\n");
	}
}
void CFDMSApp::UpdateMeasurementsBG(COleDateTime& cNow)
{

	// get the current globally specified detector name
	char cdid[2*MAX_NAME_LENGTH];
	GETPROFSTR(eDETECTOR_ID,cdid,sizeof(cdid));

	CHARACTER C4(g_szMLFilePathName,280);
	CHARACTER C5(g_szMPFileHeader,200);
	CHARACTER C6((char*)cdid,MAX_NAME_LENGTH);

	C4(0);
	C5(0);
	C6(0);

	int d,m,y;
	d = cNow.GetDay();
	m = cNow.GetMonth();
	y = cNow.GetYear();

	try
	{

		UPDATEMEASUREMENTSBG(
			C4,//280					//measurements file path name 
			C5,//200					//measurement plan csv header
			&d,	//mdateday,
			&m,	//mdatemonth,
			&y,	//mdateyear,
			&g_fBkgMeasurements[NeutA],			//na,
			&g_fBkgMeasurements[NeutB],			//nb,
			&g_fBkgMeasurements[NeutC],			//nc,
			&g_fBkgMeasurements[Gamma1],		//gamma1,
			&g_fBkgMeasurements[Gamma2],			//gamma2,
			C6
			);

	} catch (...)
	{
		TRACE("UPDATEMEASUREMENTSBG exception\n");
	}
}
void CFDMSApp::UpdateMeasurementsVBG(COleDateTime& cbtime, float bkg[5], PTCHAR pszDetcte)
{

	CHARACTER C4(g_szMLFilePathName,280);
	CHARACTER C5(g_szMPFileHeader,200);
	CHARACTER C6((char*)pszDetcte,MAX_NAME_LENGTH);

	C4(0);
	C5(0);
	C6(0);

	int d,m,y;
	d = cbtime.GetDay();
	m = cbtime.GetMonth();
	y = cbtime.GetYear();

	try
	{

		UPDATEMEASUREMENTSBG(
			C4,//280					//measurements file path name 
			C5,//200					//measurement plan csv header
			&d,	//mdateday,
			&m,	//mdatemonth,
			&y,	//mdateyear,
			&bkg[NeutA],			//na,
			&bkg[NeutB],			//nb,
			&bkg[NeutC],			//nc,
			&bkg[Gamma1],		//gamma1,
			&bkg[Gamma2],			//gamma2,
			C6
			);

	} catch (...)
	{
		TRACE("UPDATEMEASUREMENTSBG exception\n");
	}
}


void CFDMSApp::UpdateGraphData()
{
	//	for (int i = 0; i < g_iEntriesInDB; i++)
	//	{
	//		TRACE("Going IN: logCT:%g/tloggamma:%g/tlogbu:%g\n",
	//			g_fLogCoolingTime[i], g_fLogGammaDose[i], g_fLogBurnup[i]);
	//	}
	/*
	TRACE("-------***-----***------***----***---***-----***\n");

	TRACE("IN:  Plot1:%d  Plot2:%d  Plot3:%d  Plot4:%d  MostRecent:%d\n",g_bPlot1,g_bPlot2,g_bPlot3,g_bPlot4,g_iMPIndexMostRecentMeasurement);
	TRACE("IN:  Good:%d  Neutral:%d  Suspect:%d\n",g_iGoodCount, g_iNeutralCount, g_iSuspectCount);  
	TRACE("IN1: Gray:%d  Blue:%d  Fit:%d  Points:%d\n",
		g_iGrayCount1, g_iBlueCount1, g_bFit1Possible, g_iFitPoints1);
	TRACE("IN2: Gray:%d  Blue:%d  Fit:%d  Points:%d\n",
		g_iGrayCount2, g_iBlueCount2, g_bFit2Possible, g_iFitPoints2);
	TRACE("IN3: Gray:%d  Blue:%d  Fit:%d  Points:%d\n",
		g_iGrayCount3, g_iBlueCount3, g_bFit3Possible, g_iFitPoints3);
	TRACE("IN4: Gray:%d  Blue:%d\n",
		g_iGrayCount4, g_iBlueCount4);

	TRACE("---*--***\n");
	*/

	GETINFOFORPLOTS(
		&g_bPlot1, &g_bPlot2, &g_bPlot3, &g_bPlot4,
		&g_bIncludeGood,
		&g_bIncludeSuspect,
		&g_bAcceptGood,
		&g_bPlotMostRecent,
		&g_iEntriesInDB,
		g_iMeasurementType,
		g_fEnrichmentLevel,
		g_fLogNoverG,
		g_iCycle,
		g_fLogBurnup,
		g_fLogCoolingTime,
		g_fLogDecayCorTotNeutrons,
		g_fLogGammaDose,
		&g_iMPEntriesIn,
		g_iMPMeasurementType,
		g_fMPEnrichmentPercent,
		g_fMPLogNoverG,
		g_iMPCycle,
		g_fMPLogBurnup,
		g_fMPLogCoolingTime,
		g_fMPLogDecayCorTotNeutrons,
		g_fMPLogGammaDose,
		g_iMPStatus,
		&g_iMPIndexMostRecentMeasurement,
		&g_fDeltaEnrichment,
		&g_fDeltaCoolingTime,
		g_fGrayDataX1,		g_fGrayDataY1,
		g_fGrayDataX2,		g_fGrayDataY2,
		g_fGrayDataX3,		g_fGrayDataY3,
		g_fGrayDataX4,		g_fGrayDataY4,

		g_fBlueDataX1,		g_fBlueDataY1,
		g_fBlueDataX2,		g_fBlueDataY2,
		g_fBlueDataX3,		g_fBlueDataY3,
		g_fBlueDataX4,		g_fBlueDataY4,

		g_fMPGoodDataX1,	g_fMPGoodDataY1,
		g_fMPGoodDataX2,	g_fMPGoodDataY2,
		g_fMPGoodDataX3,	g_fMPGoodDataY3,
		g_fMPGoodDataX4,	g_fMPGoodDataY4,

		g_fMPNeutralDataX1,	g_fMPNeutralDataY1,
		g_fMPNeutralDataX2,	g_fMPNeutralDataY2,
		g_fMPNeutralDataX3,	g_fMPNeutralDataY3,
		g_fMPNeutralDataX4,	g_fMPNeutralDataY4,

		g_fMPSuspectDataX1,	g_fMPSuspectDataY1,
		g_fMPSuspectDataX2,	g_fMPSuspectDataY2,
		g_fMPSuspectDataX3,	g_fMPSuspectDataY3,
		g_fMPSuspectDataX4,	g_fMPSuspectDataY4,

		&g_fMostRecentDataX1,&g_fMostRecentDataY1,
		&g_fMostRecentDataX2,&g_fMostRecentDataY2,
		&g_fMostRecentDataX3,&g_fMostRecentDataY3,
		&g_fMostRecentDataX4,&g_fMostRecentDataY4,

		&g_iGrayCount1,		&g_iBlueCount1,
		&g_iGrayCount2,		&g_iBlueCount2,
		&g_iGrayCount3,		&g_iBlueCount3,
		&g_iGrayCount4,		&g_iBlueCount4,

		&g_iGoodCount,		&g_iNeutralCount,	&g_iSuspectCount,
		&g_bFit1Possible,	&g_bFit2Possible,	&g_bFit3Possible,

		&g_fSlope1,	&g_fIntercept1,	&g_fStatY1, &g_fStatX1, &g_fMeanX1,
		&g_fSlope2,	&g_fIntercept2, &g_fStatY2, &g_fStatX2, &g_fMeanX2,
		&g_fSlope3, &g_fIntercept3, &g_fStatY3, &g_fStatX3, &g_fMeanX3,

		&g_iFitPoints1, &g_iFitPoints2, &g_iFitPoints3,

		g_fCVAverages, g_fCVOneSigma
		);

/*

	TRACE("OUT:  Plot1:%d  Plot2:%d  Plot3:%d  Plot4:%d  MostRecent:%d\n",g_bPlot1,g_bPlot2,g_bPlot3,g_bPlot4,g_iMPIndexMostRecentMeasurement);
	TRACE("OUT:  Good:%d  Neutral:%d  Suspect:%d\n",g_iGoodCount, g_iNeutralCount, g_iSuspectCount);  
	TRACE("OUT1: Gray:%d  Blue:%d  Fit:%d  Points:%d\n",
		g_iGrayCount1, g_iBlueCount1, g_bFit1Possible, g_iFitPoints1);
	TRACE("OUT2: Gray:%d  Blue:%d  Fit:%d  Points:%d\n",
		g_iGrayCount2, g_iBlueCount2, g_bFit2Possible, g_iFitPoints2);
	TRACE("OUT3: Gray:%d  Blue:%d  Fit:%d  Points:%d\n",
		g_iGrayCount3, g_iBlueCount3, g_bFit3Possible, g_iFitPoints3);
	TRACE("OUT4: Gray:%d  Blue:%d\n",
		g_iGrayCount4, g_iBlueCount4);
	TRACE("-------***-----***------***----***---***-----***\n");
	*/
}


void CFDMSApp::UpdateAllDocs()
{
	CObList  pDocList;
	POSITION pos = GetFirstDocTemplatePosition();
	while(pos)
	{
		CDocTemplate* pTemplate = GetNextDocTemplate(pos); 
		//		this works but isn't what I wanted
		//		CString cString;
		//		pTemplate->GetDocString(cString, CDocTemplate::docName );
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while(pos2)
		{
			CDocument* pDocument = pTemplate->GetNextDoc(pos2);
			if(pDocument)
				pDocList.AddHead(pDocument);
		}
	}
	if(!pDocList.IsEmpty())
		pos = pDocList.GetHeadPosition();
	while(pos)
	{  
		// Call some CDocument function for each document.
		( (CDocument*)pDocList.GetNext(pos) )
			->UpdateAllViews(NULL);
	}
}


bool CFDMSApp::MakeNewConfigFile()
{
	return CIniFileProcessor::MakeNewConfigFile();
}

void CFDMSApp::SaveFullConfig(bool bUseDefault)
{
	CFileDialog fd(FALSE, "ini",
		"fdmsconfig",
		OFN_OVERWRITEPROMPT,
		NULL,
		this->GetMainWnd());

	if (fd.DoModal()==IDOK )
	{
		CString pathName = fd.GetPathName();
		CIniFileProcessor::SaveFullConfigFile(pathName, bUseDefault);
	}
}

void CFDMSApp::OnEditConfigDUMP_O_RAMA_Current()
{
	SaveFullConfig(false);

}
void CFDMSApp::OnEditConfigDUMP_O_RAMA_Default()
{
	SaveFullConfig(true);

}

void CFDMSApp::OnViewLog()
{
		bSL = !bSL;
		m_pLogLog->ShowWindow(bSL ? SW_SHOW : SW_HIDE);
}

void CFDMSApp::SetAllMessagesOnHold(bool ProcessMsgs)
{
	if (m_pRT_Review)
		m_pRT_Review->SetMessagesOnHold(ProcessMsgs);

	if (m_pRT_Setup)
		m_pRT_Setup->SetMessagesOnHold(ProcessMsgs);

	if (m_pRT_ImpReimp)
		m_pRT_ImpReimp->SetMessagesOnHold(ProcessMsgs);

}

void CFDMSApp::OnEditIni() 
{
	CIniFileProcessor ifp(AfxGetMainWnd());

	ifp.DoModal();

}

int CFDMSApp::ExitInstance()
{

	int ret;

	CIniFileProcessor::Terminate();
	CSelectMeasurement::CleanUpStatics();
//	ArtfulMeasurement::CleanUpStatics();
	XListCtrlDlg::CleanUpStatics();
	CoUninitialize();

	ret = CWinApp::ExitInstance();
	return ret;
}


void CFDMSApp::GetSavedBkgMeasurements()
{
	char szString[128];
	for (int cht = NeutA; cht < MaxChannels; cht++)
	{
		GETPROFSTR(tIniNames(eBACKGROUND_A + cht),szString,sizeof(szString));
		g_fBkgMeasurements[cht] = (float)atof(szString);
	}
}

// WM_HELPPROMPTADDR is used internally to get the address of 
//	m_dwPromptContext from the associated frame window. This is used
//	during message boxes to setup for F1 help while that msg box is
//	displayed. lResult is the address of m_dwPromptContext.
#define WM_HELPPROMPTADDR	0x0376

/////////////////////////////////////////////////////////////////////////////
// Context sensitive help support (see Technical note TN028 for more details)

#define HID_BASE_PROMPT     0x00030000UL        // IDP

int CFDMSApp::MyMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt, LPCTSTR lpszTitle)
{
	// disable windows for modal dialog
	EnableModeless(FALSE);
	HWND hWndTop;
	HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);

	// WINBUG: re-enable the parent window, so that focus is restored 
	// correctly when the dialog is dismissed.
	if (hWnd != hWndTop)
		EnableWindow(hWnd, TRUE);

	// set help context if possible
	DWORD* pdwContext = NULL;
	if (hWnd != NULL)
	{
		// use app-level context or frame level context
		LRESULT lResult = ::SendMessage(hWnd, WM_HELPPROMPTADDR, 0, 0);
		if (lResult != 0)
			pdwContext = (DWORD*)lResult;
	}
	// for backward compatibility use app context if possible
	if (pdwContext == NULL && this != NULL)
		pdwContext = &m_dwPromptContext;

	DWORD dwOldPromptContext = 0;
	if (pdwContext != NULL)
	{
		// save old prompt context for restoration later
		dwOldPromptContext = *pdwContext;
		if (nIDPrompt != 0)
			*pdwContext = HID_BASE_PROMPT+nIDPrompt;
	}

	// determine icon based on type specified
	if ((nType & MB_ICONMASK) == 0)
	{
		switch (nType & MB_TYPEMASK)
		{
		case MB_OK:
		case MB_OKCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_YESNO:
		case MB_YESNOCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_ABORTRETRYIGNORE:
		case MB_RETRYCANCEL:
			// No default icon for these types, since they are rarely used.
			// The caller should specify the icon.
			break;
		}
	}

#ifdef _DEBUG
	if ((nType & MB_ICONMASK) == 0)
		TRACE(traceAppMsg, 0, "Warning: no icon specified for message box.\n");
#endif

	int nResult =
		::MessageBox(hWnd, lpszPrompt, lpszTitle, nType);

	// restore prompt context if possible
	if (pdwContext != NULL)
		*pdwContext = dwOldPromptContext;

	// re-enable windows
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	EnableModeless(TRUE);

	return nResult;
}


bool CAboutDlg::m_bDoneGot;
CString CAboutDlg::m_sVer;
CString CAboutDlg::m_sTime;

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_bDoneGot = false;
	m_sVer = _T("");
	m_sTime = _T("");
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFDMSApp::OnAppAbout()
{
	CLegText x;
	x.DoModal();
}

BOOL CAboutDlg::GetLastWriteTime(PTCHAR pszFullPath, CString& lpszString)
{
	CFileStatus status;

	if( CFile::GetStatus( pszFullPath, status ) )   // static function
	{
		SYSTEMTIME st;
		status.m_mtime.GetAsSystemTime(st);
		COleDateTime dt(st);
		lpszString = dt.Format();// date and time in LANG_USER_DEFAULT
	}
	return TRUE;
}

BOOL CAboutDlg::GetAppInfo()
{
	if (m_bDoneGot)
		return TRUE;

	// Get version information from the application
	BOOL bOK = FALSE;
	TCHAR szFullPath[_MAX_PATH];
	DWORD dwVerHnd;
	DWORD dwVerInfoSize;

	GetModuleFileName(AfxGetInstanceHandle(), szFullPath, sizeof(szFullPath));

	GetLastWriteTime(szFullPath, m_sTime);

	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE  hMem;
		LPVOID  lpvMem;
		TCHAR    szGetName[256];
		UINT	cchVer = 0;
		LPSTR	lszVer = NULL;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);

		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
		lstrcpy(szGetName, _T("\\StringFileInfo\\040904B0\\FileVersion"));
		BOOL fRet = VerQueryValue(lpvMem, szGetName, (LPVOID*)&lszVer, &cchVer);

		if (fRet && cchVer && lszVer)
		{
			m_sVer = lszVer;
			bOK = TRUE;
		}

		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
	return bOK;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetAppInfo();

	CString s;
#ifdef _DEBUG
	s.Format(_T("FDMS %s (%s)"),
			(LPCTSTR)m_sVer, (LPCTSTR)m_sTime);
#else
	s.Format(_T("FDMS %s"),
			(LPCTSTR)m_sVer);
#endif 
	SetDlgItemText(IDC_VERSION_LABEL, s);

	return TRUE;
}


void CFDMSApp::OnAppExit()
{
	if (m_pRT_Review)
	{

		BOOL bExitQ = m_pRT_Review->RT_Review_Exit_Question(IDR_FDMSTYPE, FALSE);
		if (!bExitQ)
			return;
	}
	CWinApp::OnAppExit();


}
void CFDMSApp::AppendToLogF( char * format, ...)
{
	if (m_MLog.GetCount() > 32767)
		return;

	char mbuf[1024];
	mbuf[0] = 0;

	va_list args;
	va_start( args, format );
	vsprintf( mbuf, format, args );
	va_end( args);
	m_pLogLog->AddLine(mbuf);
	m_MLog.Add(mbuf);
#ifdef _DEBUG
	if (mbuf[0] != 0)
	{
		strcat(mbuf, "\n");
		OutputDebugStringA( mbuf);
	}
#endif
}
void CFDMSApp::AppendToLog(LPCTSTR s)
{
	if (m_MLog.GetCount() > 32767)
		return;

	m_pLogLog->AddLine(s);
	m_MLog.Add(s);
}

PTCHAR CFDMSApp::GetFileHeader(CSVFileHeaderIDs id, USHORT length)
{
	static CHAR loosebuffer[64];
	if (length > 0)
	{
		ZeroMemory(loosebuffer, sizeof(loosebuffer));
		strncpy(loosebuffer, m_fileheaderstrings[id], size_t(length));
		return loosebuffer;
	}
	else
		return m_fileheaderstrings[id];
}


PTCHAR CFDMSApp::m_fileheaderstrings[efhHeaderCount] =
{
	"Facility","ID","Measurement type","Status","Enrichment (%)","Burnup (GWd/MT)",
	"Discharge day","month","year","cycle #","Thres A","Thres B", "Measurement day",
	"month","year","Cooling Time (years)", "NA", "NB", "NC", "G1", "G2","Detector"
};


