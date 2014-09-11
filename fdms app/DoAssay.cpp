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
// $History: DoAssay.cpp $
// 
// *****************  Version 15  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 14  *****************
// User: 186846       Date: 12/18/07   Time: 4:34p
// Updated in $/FDMS/FDMS Application/FDMSNET
// VS 2005 changes completed
// 
// *****************  Version 13  *****************
// User: Longo        Date: 12/18/07   Time: 12:32p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 12  *****************
// User: Longo        Date: 7/27/05    Time: 3:54p
// Updated in $/FDMS/FDMS Application/FDMSNET
// more info presented to user upon grand communciation failure
// 
// *****************  Version 11  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 10  *****************
// User: Longo        Date: 12/10/04   Time: 6:24p
// Updated in $/FDMS/FDMS Application/FDMSNET
// merged live and demo, checkpoint checkin
// 
// *****************  Version 9  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 8  *****************
// User: Longo        Date: 12/10/04   Time: 9:58a
// Updated in $/FDMS/FDMS Application/FDMSNET
// UL messages blocked
// 
// *****************  Version 7  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 6  *****************
// User: Longo        Date: 11/22/04   Time: 3:03p
// Updated in $/FDMS/FDMS Application/FDMSNET
// block UL while doing a measurement
// 
// *****************  Version 5  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work

#include "stdafx.h"
#include "math.h"
#include "time.h"
#include <process.h>
#include <mmsystem.h>

#include "FDMS.h"
#include "DoAssay.h"
#include "Prompt.h"
#include "Abort.h"

#include "SerialCom.h"

#include "Warning.h"
#include "dconfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TIMER_GRAND_TIC		7000
#define RECEIVE_GRAND_DAT	7003
#define GOTGRANDVALUE		7004
#define NEXTSTATE			7006
#define ONE_CYCLE			7007
#define ACCOMPLISHED		7008
#define GRANDSHUTDOWN		7009
#define GRANDDATA			7010
#define SHUTDOWN			7011
#define HAVEGRANDOFFSETS	7012
#define GRANDOFFSETFAILED	7013
#define GRANDOFFSETOOB		7014

#define TIC_TIMER_TIME		250

#define MESSAGE				5501


#define WATCHBUTTONTEXT		"\r\n&Watch\r\nF4"
#define NOWATCHBUTTONTEXT	"No\r\n&Watch\r\nF4"
#define ACCBUTTONTEXT		"\r\n&Accomplished\r\nF8"
#define CANCELBUTTONTEXT	"\r\n&Cancel Assay\r\nF12"

#define BACKGROUND			0
#define BACKGROUNDOFFSET	1
#define BACKGROUNDNOOFFSET	2
#define NORMAL				3

#define ENDFLAG				9999

#define COLOR_BLUE RGB(0,0,255)
#define COLOR_GREEN RGB(0,191,0)


#define GRAND_NODE			1

#define ACCUMULATORSIZE		1024

DataRec gl_DataRecord;

int		g_iDemoCount;

AnalysisRec g_sScanRecs[MAXIMUM_CYCLES_AUTHORIZED];
int		g_dScanRecMax;

IMPLEMENT_DYNAMIC(CDoAssay, CDialog)


/////////////////////////////////////////////////////////////////////////////
// CDoAssay dialog

CDoAssay::CDoAssay(CWnd* pParent /*=NULL*/)
: CDialog(CDoAssay::IDD, pParent)
{

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	bDemoVersion = pApp->m_bDemoVersion;

	//{{AFX_DATA_INIT(CDoAssay)
	//}}AFX_DATA_INIT

	m_dCurrentState = 0;
	m_bShutDown = false;
	m_bDoingBackground = false;	
	m_bDoingNormalize = false;	
	m_dGrandState = 800;
	m_Grand_TicCount = 0;
	m_bGRANDFreeRun = true;
	m_szGrandCmdBuffer[0] = NULL;
	//	m_bUsingAltHV = false;
	m_bPass = false;
	ClearAccumulator();
	m_iMode = eBackgroundAssay;
	m_iItem = 0;
	CBWatch = NULL;
	CBOk = NULL;
	CBCancelAssay = NULL;
	m_bOffset = false;
	ZeroMemory(g_sScanRecs,sizeof(g_sScanRecs));
	m_psMeasurementData = NULL;
	m_szSound[0] = NULL;
	m_pGrandCom = NULL;
	m_dAssayIndex = -1;
	mbWantRemote = false;
	m_pActivityLog = NULL;
	m_pCommLog = NULL;
	m_dStepIncrement = -1;
	((CFDMSApp*)AfxGetApp())->SetAllMessagesOnHold(true);
}

CDoAssay::~CDoAssay()
{
	((CFDMSApp*)AfxGetApp())->SetAllMessagesOnHold(false);

	if (CBWatch) delete CBWatch;
	if (CBOk) delete CBOk;
	if (CBCancelAssay) delete CBCancelAssay;
	if (m_pGrandCom)
	{
		m_pGrandCom->UnRegister(0);
		m_pGrandCom->DoStop();
		m_pGrandCom->PostThreadMessage(IDC_COM_AFXTERMINATE,0,0);  //also calls ~
		::WaitForSingleObject(m_pGrandCom->m_hThread,2000);
		m_pGrandCom = NULL;
	}

	if (m_pActivityLog)
		delete m_pActivityLog;
	if (m_pCommLog)
		delete m_pCommLog;

}

void CDoAssay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoAssay)
	DDX_Control(pDX, IDC_PROGRESS2, m_pProgress2);
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress1);
	DDX_Control(pDX, IDC_STATIC_ASSM, m_StaticAssm);
	DDX_Control(pDX, IDC_EDIT_ACTIVITY2, m_EditGrand);
	DDX_Control(pDX, IDC_EDIT_ACTIVITY, m_EditActivity);
	DDX_Control(pDX, IDC_STATIC_INSTRUCTION, m_pStaticInstruction);
	DDX_Control(pDX, IDC_STATIC_COMMENT, m_pStaticComment);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDoAssay, CDialog)
	//{{AFX_MSG_MAP(CDoAssay)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDWATCH, OnWatchButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDACK, OnAck)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(NEXTSTATE,StateMachine)
	ON_MESSAGE(GOTGRANDVALUE,StateMachine)
	ON_MESSAGE(GRANDDATA,StateMachine)
	ON_MESSAGE(TIMER_GRAND_TIC,GrandStateMachine)
	ON_MESSAGE(IDC_RECEIVE,CmdDistributor)
	ON_MESSAGE(RECEIVE_GRAND_DAT,GrandStateMachine)
	ON_MESSAGE(GRANDSHUTDOWN,GrandStateMachine)
	ON_MESSAGE(SHUTDOWN,StateMachine)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoAssay message handlers
void CDoAssay::PostToActions(char *text)
{
	m_Actions = text;
	m_pActivityLog->WriteString(text);
	m_Actions += "\r\r\n";
	int lines;
	lines = m_EditActivity.GetLineCount();
	m_EditActivity.SetSel(-2,-1,FALSE);
	//append the new data
	m_EditActivity.ReplaceSel(m_Actions,false);
	//if we didn't get a new line appended then
	//strip off a bunch of stuff and try again
	if (m_EditActivity.GetLineCount() == lines)
	{
		//strip off 5000 characters
		m_EditActivity.SetSel(0,5000,true);
		m_EditActivity.ReplaceSel("",false);
		//append the stuff we tried a moment ago
		m_EditActivity.SetSel(-2,-1,true);
		m_EditActivity.ReplaceSel(m_Actions,false);
	}


}

/*
GROSS GAMMAS:       
G1=0.000E+1 [1.91E-5
G2=0.000E+1 [1.91E-5
"]80
GROSS NEUTRON COUNTS
A=        0 [0.00   
B=        0 [0.00   
C=        0 [0.00 "]80
*/

void CDoAssay::PostToGrand(char *text)
{
	m_Grand = text;
	m_pCommLog->WriteString(text);

	m_Grand += "\r\r\n";
	int lines;
	lines = m_EditGrand.GetLineCount();
	m_EditGrand.SetSel(-2,-1,FALSE);
	//append the new data
	m_EditGrand.ReplaceSel(m_Grand,false);
	if (m_EditGrand.GetLineCount() == lines)
	{
		m_EditGrand.SetSel(0,5000,true);
		m_EditGrand.ReplaceSel("",false);
		m_EditGrand.SetSel(-2,-1,true);
		m_EditGrand.ReplaceSel(m_Grand,false);
	}
}


void CDoAssay::PostInstruction(char *text)
{ 
	m_pStaticInstruction.SetWindowText(text);
	SetActiveWindow();
	// why set the parent mainframe of the app active here????
	// ::SetWindowPos(AfxGetMainWnd()->m_hWnd, 
	//  	HWND_NOTOPMOST, -1,-1,-1,-1,SWP_NOMOVE | SWP_NOSIZE);
}

void CDoAssay::PostComment(char *text)
{
	m_pStaticComment.SetWindowText(text);
}

void CDoAssay::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_GRAND_TIC)
	{
		PostMessage(TIMER_GRAND_TIC,TIMER_GRAND_TIC,0);
	}
	if (bDemoVersion)
	{
		if (nIDEvent == TIMER_ALL_TIC)
		{
			if (m_pGrandCom)
			{
				char * temp = (char*)malloc(sizeof("TICK"));
				strcpy(temp,"TICK");
				m_pGrandCom->PostThreadMessage(TIMER_ALL_TIC,0,(LPARAM)temp);
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDoAssay::CreateTheThread()
{
	if (!bDemoVersion)
	{
		m_pGrandCom = (CSerialCom*)AfxBeginThread(RUNTIME_CLASS(CSerialCom));
	}
}


BOOL CDoAssay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char temp[_MAX_PATH];
	COLORREF	g_dColor;// = GetSysColor(COLOR_BTNFACE);
	g_dColor = RGB(0,255,255);

	CRect cRect;
	GetDlgItem(IDC_BUTTON1A)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	GetDlgItem(IDC_BUTTON1A)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON1A)->ShowWindow(SW_HIDE);
	CBWatch = new CColorButton();
	CBWatch->SetFaceColor(g_dColor,false);
	CBWatch->SetMultiLine(true);
	CBWatch->Create(WATCHBUTTONTEXT,BS_PUSHBUTTON|BS_NOTIFY,cRect,this,IDWATCH);
	CBWatch->EnableWindow(true);
	CBWatch->ShowWindow(SW_SHOW);

	GetDlgItem(IDOKA)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	GetDlgItem(IDOKA)->EnableWindow(false);
	GetDlgItem(IDOKA)->ShowWindow(SW_HIDE);
	CBOk = new CColorButton();
	CBOk->SetFaceColor(g_dColor,false);
	CBOk->SetMultiLine(true);
	CBOk->Create(ACCBUTTONTEXT,BS_PUSHBUTTON|BS_NOTIFY,cRect,this,IDOK);
	CBOk->EnableWindow(true);
	CBOk->ShowWindow(SW_SHOW);

	GetDlgItem(IDCANCELA)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	GetDlgItem(IDCANCELA)->EnableWindow(false);
	GetDlgItem(IDCANCELA)->ShowWindow(SW_HIDE);
	CBCancelAssay = new CColorButton();
	CBCancelAssay->SetFaceColor(g_dColor,false);
	CBCancelAssay->SetMultiLine(true);
	CBCancelAssay->Create(CANCELBUTTONTEXT,BS_PUSHBUTTON|BS_NOTIFY,cRect,this,IDCANCEL);
	CBCancelAssay->EnableWindow(true);
	CBCancelAssay->ShowWindow(SW_SHOW);


	//start with watch section hidden
	m_bMinimal = false;

	GETPROFBOOL(eMINIGRAND,m_bMiniGrand);

	if (bDemoVersion)
	{
		m_bMiniGrand = true; // force true always
	}

	CreateTheThread();

	if (m_pGrandCom)
	{
		m_dCurrentState = 100;
	}
	else
	{
		AfxMessageBox("Cannot instantiate communications",MB_OK);
		EndDialog(IDCANCEL);
		m_dCurrentState = 1000;
		return FALSE;
	}

	char szPort[16];
	char szParity[16];
	GETPROFSTR(ePORT, szPort,sizeof(szPort));
	unsigned uBaudRate = (unsigned)GETPROFINT(eBAUD);
	unsigned uDataBits = (unsigned)GETPROFINT(eDATABITS);
	unsigned uStopBits = (unsigned)GETPROFINT(eSTOPBITS);
	GETPROFSTR(ePARITY, szParity,sizeof(szParity));

	if (m_pGrandCom->SetUpPort(szPort, uBaudRate, 
		uDataBits, uStopBits, 
		szParity, szPort, false) == false)
	{
		AfxMessageBox("Cannot instantiate communications",MB_OK);
		TCHAR szBuf[512]; 
		LPVOID lpMsgBuf = 0;
		DWORD dw = GetLastError(); 

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		wsprintf(szBuf, 
		"Cannot instantiate communications\r\n%s failed with error %d: %s", 
		"Opening port to instrument", dw, lpMsgBuf); 

		AfxMessageBox(szBuf, MB_OK); 

		if (lpMsgBuf) LocalFree(lpMsgBuf);
		EndDialog(IDCANCEL);
		m_dCurrentState = 1000;
		return FALSE;
	}
	else
	{


	}

	m_pGrandCom->PostThreadMessage(IDC_COM_START,NULL,NULL);

	m_dGrandCmdTimeOut = GETPROFINT(eCOMMANDTIMEOUT);

	//	//disconnect the current grand comm
	//	m_pGrandCom->UnRegister(0);
	if (bDemoVersion)
	{
		m_pGrandCom->m_bMiniGrand = true;
	}
	//connect this object to grand comm
	m_pGrandCom->Register(this,GRAND_NODE);

	//maximize the amount of text in the two cedits
	m_EditActivity.SetLimitText(INT_MAX);
	m_EditGrand.SetLimitText(INT_MAX);

	m_pActivityLog = new CLogFile(eACTIVITYLOG);
	m_pCommLog = new CLogFile(eCOMMLOG);

	if (m_bMiniGrand)
	{
		m_pActivityLog->WriteString("Mini-GRAND");
	}
	else
	{
		m_pActivityLog->WriteString("GRAND 3");
	}


	//going to need a large font for the "instructions"
	InstructionFont.CreateFont(-24,0,0,0, FW_BOLD,0,0,0,0,0,0,0,0,NULL);
	//need a cyrillic font for the activity window
	//	int dFontSize;
	if (ItemFont.CreateFont(30,0,0,0,400,0,0,0,0,0,0,0,0,"ARIAL BLACK"))
		m_StaticAssm.SetFont(&ItemFont,false);

	CString cTemp;
	if (m_iMode == eBackgroundAssay)
	{
		m_StaticAssm.SetWindowText("Background");
		cTemp = "Background Assay";
		m_pActivityLog->WriteString(cTemp);
		m_bDoingBackground = true;
		m_bDoingNormalize = false;
		gl_DataRecord.ChPos[0] = (short)0;
		gl_DataRecord.ChPos[1] = (short)ENDFLAG;

		//	m_fCurrentCountTime = (float)m_dGrandCycleTime;

		if (m_psMeasurementData)
		{
			//m_bOffset = (m_psMeasurementData->iType == BACKGROUNDOFFSET)?TRUE:FALSE;

			GETPROFBOOL(eOFFSET_CNTRL,m_bOffset);

			m_iCycleTime = m_psMeasurementData->iTime;
			m_iCycles = m_psMeasurementData->iPoints;

			m_dGrandCycleTime = m_iCycleTime;
		}
		else
		{
			m_iCycleTime = GETPROFINT(eBGCYCLETIME);
			m_iCycles = GETPROFINT(eBGCYCLES);

			GETPROFBOOL(eOFFSET_CNTRL,m_bOffset);

		}

		// check for remote offset toggle
		GETPROFBOOL(eOFFSET_CNTRL_REMOTE,mbWantRemote);

	}
	else 
	{
		cTemp = "Normal Assay";

		m_pActivityLog->WriteString(cTemp);
		m_pActivityLog->WriteString(m_cID);

		m_StaticAssm.SetWindowText(m_cID);
		m_bDoingBackground = false;
		m_bDoingNormalize = false;
		if (m_psMeasurementData)
		{
			double dPos = 100;
			double dDecrement = (dPos*2.0) / (m_psMeasurementData->iPoints+1.0);
			int i;
			for (i = 0; i < m_psMeasurementData->iPoints; i++)
			{
				dPos -= dDecrement;
				gl_DataRecord.ChPos[i] = (short)dPos;
			}
			gl_DataRecord.ChPos[i] = (short)ENDFLAG;
			m_iCycleTime = m_psMeasurementData->iTime;
			m_iCycles = m_psMeasurementData->iPoints;
		}
		else
		{
			gl_DataRecord.ChPos[0] = (short)+75;
			gl_DataRecord.ChPos[1] = (short)0;
			gl_DataRecord.ChPos[2] = (short)-75;
			m_iCycleTime = GETPROFINT(eCYCLETIME);
			m_iCycles = 3;
		}
	}
	if (bDemoVersion)
	{
		cTemp += " - Demo Version";
	}
	SetWindowText(cTemp);

	if (m_bMiniGrand)
	{
		SetDlgItemText(IDC_STATIC_DEVICELBL, "Mini-GRAND");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_DEVICELBL, "GRAND 3");
	}
	SetDataInfoLabel();

	gl_DataRecord.ChPos[19] =(short)ENDFLAG;

	//start the control state machine
	PostMessage(NEXTSTATE,0,0);

	//save the original window rectangle
	GetWindowRect(&m_Rect);

	//hide the bottom part of the dialog box
	OnWatchButton();

	//get the cycle time on for the grand state machine
	m_dGrandTickle = GETPROFINT(eTICKLE);

	//get the offset limits
	m_iOffsetMaxTime = GETPROFINT(eOFFSET_TIME);
	m_iOffsetMin = GETPROFINT(eOFFSET_LIMIT_LOW);
	m_iOffsetMax = GETPROFINT(eOFFSET_LIMIT_HIGH);

	//get the scale text
	GETPROFSTR(eSCALE,m_szScale,sizeof(m_szScale));

	//start the grand state machine by starting the timer
	SetTimer(TIMER_GRAND_TIC,m_dGrandTickle,0);

	if (bDemoVersion)
	{
		SetTimer(TIMER_ALL_TIC,1000,NULL);
	}

	//get the normal to alternate hv gamma gate
	m_dGrossGammaGate1 = GETPROFINT(eGAMMAGATE1);

	//get the max allowable gamma gate
	m_dGrossGammaGate2 = GETPROFINT(eGAMMAGATE2);
	//get the duration of the grand monitor times - default to 60 seconds
	if (m_psMeasurementData)
		m_dGrandCycleTime = m_psMeasurementData->iTime;
	else
		m_dGrandCycleTime = GETPROFINT(eCYCLETIME);
	/*	
	m_iCycleTime = (int)m_dGrandCycleTime;
	m_fCurrentCountTime = (float)m_dGrandCycleTime;
	*/
	if (m_bMiniGrand)
		m_pProgress1.SetRange(0,(short)m_iCycleTime);
	else
		m_pProgress1.SetRange(0,(short)(m_iCycleTime * m_iCycles));

	if (m_psMeasurementData)
	{
		m_pProgress1.SetRange(0,(short)m_psMeasurementData->iTime * (short)m_psMeasurementData->iPoints);
		m_pProgress2.SetRange(0,(short)m_psMeasurementData->iPoints);
	}

	m_pProgress1.SetPos(0);
	m_pProgress2.SetPos(0);

	//get the minimum number of samples
	//	m_dPassiveMinSamp = GETPROFINT(eMINSAMPLES);
	//	m_dPassiveMaxSamp = GETPROFINT(eMAXSAMPLES);

	//get the IC background 
	GETPROFSTR(eICBACKGROUND1,temp,sizeof(temp));
	m_fICBackground1 = (float)atof(temp);
	GETPROFSTR(eICBACKGROUND2,temp,sizeof(temp));
	m_fICBackground2 = (float)atof(temp);

	switch (m_iMode) {
	case eBackgroundAssay:
		m_dCurrentState = 300;
		break;
	case eNormalAssay:
	case eBaselineAssay:
	case eReVerificationAssay:
		m_dCurrentState = 0;
		break;
	}

	if (bDemoVersion)
	{
		SetModeForUpdateParams();
		UpdateParams("EMPTY");
	}

	return TRUE;
}

void CDoAssay::SetDataInfoLabel()
{
	if (m_bMiniGrand)
	{
		SetDlgItemText(IDC_STATIC_DEVICEDATALBL, "Mini-GRAND Raw Data");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_DEVICEDATALBL, "GRAND 3 Raw Data");
	}
}

void
CDoAssay::UpdateParams(char* )
{
}

void
CDoAssay::SetModeForUpdateParams()
{
}


HBRUSH CDoAssay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT index = pWnd->GetDlgCtrlID();

	if (index == IDC_STATIC_INSTRUCTION)
	{
		pDC->SetBkColor(RGB(0,0,255));
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SelectObject(&InstructionFont);
	}

	if (index == IDC_STATIC_COMMENT)
	{
		pDC->SelectObject(&InstructionFont);
	}

	//	if (index == IDC_ISR_HIGHVOLTAGE)
	//	{
	//		if (m_bUsingAltHV)
	//			pDC->SetBkColor(RGB(255,255,0));
	//		pDC->SetTextColor(RGB(0,0,255));
	//	}
	//
	//	if (index == IDC_ISR_ACCIDENTALS)
	//	{
	//		if (m_bISRFailed)
	//		{
	//			pDC->SetBkColor(RGB(255,0,0));
	//			pDC->SetTextColor(RGB(255,255,255));
	//		}
	//	}

	//	if (index == IDC_GRAND_LEAKDET)
	//	{
	//		if (g_bLeakDetExcessive)
	//		{
	//			pDC->SetBkColor(RGB(255,0,0));
	//			pDC->SetTextColor(RGB(255,255,255));
	//		}
	//		else
	//			pDC->SetTextColor(COLOR_BLUE);
	//	}
	//
	if (index == IDC_GRAND_COUNTTIME)
		pDC->SetTextColor(COLOR_BLUE);
	else if (index == IDC_GRAND_G1)
		pDC->SetTextColor(COLOR_BLUE);
	else if (index == IDC_GRAND_G2)
		pDC->SetTextColor(COLOR_BLUE);
	else if (index == IDC_GRAND_NA)
		pDC->SetTextColor(COLOR_BLUE);
	else if (index == IDC_GRAND_NB)
		pDC->SetTextColor(COLOR_BLUE);
	else if (index == IDC_GRAND_NC)
		pDC->SetTextColor(COLOR_BLUE);

	return hbr;
}

LRESULT CDoAssay::CmdDistributor(WPARAM WParam, LPARAM LParam)
{
	if (WParam == GRAND_NODE)
		GrandCmdAccumulator(WParam, LParam);
	return 0;

}

void CDoAssay::ClearAccumulator()
{
	m_fGrossGamma1 = 0.0;
	m_fGrossGamma2 = 0.0;
	m_fGrossGammaAverage1 = 0.0;
	m_fGrossGammaAverage2 = 0.0;
	m_dGrossGammaCnt = 0;
	m_dNeutronCnt = 0;
	//	m_fLeakDetAverage = 0.0;
	//	m_fLeakDetSum = 0.0;
	m_fNeutronA = 0.0;
	m_fNeutronB = 0.0;
	m_fNeutronC = 0.0;
	m_fNeutronAAverage = 0.0;
	m_fNeutronBAverage = 0.0;
	m_fNeutronCAverage = 0.0;
	//m_dTPrev = 0;
	//m_dCPrev = 0;
}

bool CDoAssay::Accumulate(char *Data)
{
	char temp[64];
	char temp0[16];
	char temp1[16];
	char temp2[16];
	char temp3[16];
	bool result;
	float GrossGammas1 = 0.0;
	float GrossGammas2 = 0.0;
	result = true;


	//Mini-Grand
	//A:     0.00/     0.00  B:     0.00/     0.00  C:     0.00/     0.00
	//0:0.0000e+1/0.0000e+1  1:0.0000e+1/0.0000e+1

	if ((Data[0] =='A')&&(Data[1]==':'))
	{
		strncpy(temp1,Data+2,9);temp1[9] = NULL;
		strncpy(temp2,Data+25,9);temp2[9] = NULL;
		strncpy(temp3,Data+48,9);temp3[9] = NULL;
		m_fNeutronA = (float)atof(temp1);
		m_fNeutronB = (float)atof(temp2);
		m_fNeutronC = (float)atof(temp3);
		sprintf(temp,"%8.3E",m_fNeutronA);
		SetDlgItemText(IDC_GRAND_NA,temp);
		sprintf(temp,"%8.3E",m_fNeutronB);
		SetDlgItemText(IDC_GRAND_NB,temp);
		sprintf(temp,"%8.3E",m_fNeutronC);
		SetDlgItemText(IDC_GRAND_NC,temp);

		PostMessage(GRANDDATA,GRANDDATA,0);
	}

	if ((Data[0] == '0')&&(Data[1]==':'))
	{
		int i1,i2;
		float d1,d2;
		sscanf(Data,"%d:%e/%e%d:%e/%e",&i1,&GrossGammas1,&d1,&i2,&GrossGammas2,&d2);

		if (!m_bDoingBackground)
		{
			GrossGammas1 -= m_fICBackground1;
			GrossGammas2 -= m_fICBackground2;
		}
		if (GrossGammas1 < 0.0) GrossGammas1 = 0.0;
		if (GrossGammas2 < 0.0) GrossGammas2 = 0.0;
		m_fGrossGamma1 += GrossGammas1;
		m_fGrossGamma2 += GrossGammas2;
		m_dGrossGammaCnt++;

		m_fGrossGammaAverage1 = m_fGrossGamma1/m_dGrossGammaCnt;
		m_fGrossGammaAverage2 = m_fGrossGamma2/m_dGrossGammaCnt;
		sprintf(temp,"%8.3E",m_fGrossGammaAverage1);
		SetDlgItemText(IDC_GRAND_G1,temp);
		sprintf(temp,"%8.3E",m_fGrossGammaAverage2);
		SetDlgItemText(IDC_GRAND_G2,temp);

		if ((m_fGrossGammaAverage1 > (float)m_dGrossGammaGate2) ||
			(m_fGrossGammaAverage2 > (float)m_dGrossGammaGate2))
		{
			m_bGRANDFreeRun = false;
			PostMessage(GRANDSHUTDOWN,GRANDSHUTDOWN,0);
			PostMessage(SHUTDOWN,SHUTDOWN,0);
			sprintf(temp,"Gross Gammas = %8.3E in excess of %d!  Aborting.",
				__max(m_fGrossGammaAverage1,m_fGrossGammaAverage2),m_dGrossGammaGate2);
			CWarning Warning;
			Warning.m_szMessage = temp;
			Warning.DoModal();
			m_bShutDown = true;
			result = false;
		}
	}

	// GROSS GAMMAS:       G1=5.018E+0 [1.25E+0G2=1.526E-4 [1.91E-5                  "]80
	BOOL G3Check = FALSE;
	if (
		((strstr(Data,"GROSS GAMMAS:"))!=NULL) ||
		(G3Check = (strstr(Data,"TIME = ")) && (strstr(Data,"G1=")))
		)
	{
		strncpy(temp0,Data+6, 6);
		strncpy(temp1,Data+23,10);
		strncpy(temp2,Data+43,10);
		temp0[6] = NULL;
		temp1[10] = NULL;
		temp2[10] = NULL;
		sscanf(temp0,"%d",&m_dGrandTime);
		sscanf(temp1,"%G",&GrossGammas1);
		sscanf(temp2,"%G",&GrossGammas2);

		if (G3Check && !m_bMiniGrand)
		{
			m_pProgress1.SetPos((m_dGrandCycleTime*m_dStepIncrement) +
				(int)((float)m_iCycleTime * ((float)m_dGrandTime/(float)m_dGrandCycleTime)));  
		}

		if (!m_bDoingBackground)
		{
			GrossGammas1 -= m_fICBackground1;
			GrossGammas2 -= m_fICBackground2;
		}
		if (GrossGammas1 < 0.0) GrossGammas1 = 0.0;
		if (GrossGammas2 < 0.0) GrossGammas2 = 0.0;

		//test gates
		if (strstr(Data,"?")==NULL)
		{
			m_fGrossGamma1 += GrossGammas1;
			m_fGrossGamma2 += GrossGammas2;
			m_dGrossGammaCnt++;
			m_fGrossGammaAverage1 = m_fGrossGamma1/m_dGrossGammaCnt;
			m_fGrossGammaAverage2 = m_fGrossGamma2/m_dGrossGammaCnt;
			sprintf(temp,"%8.3E",m_fGrossGammaAverage1);
			SetDlgItemText(IDC_GRAND_G1,temp);
			sprintf(temp,"%8.3E",m_fGrossGammaAverage2);
			SetDlgItemText(IDC_GRAND_G2,temp);


			if ((m_fGrossGammaAverage1 > (float)m_dGrossGammaGate2) ||
				(m_fGrossGammaAverage2 > (float)m_dGrossGammaGate2))
			{
				m_bGRANDFreeRun = false;
				PostMessage(GRANDSHUTDOWN,GRANDSHUTDOWN,0);
				PostMessage(SHUTDOWN,SHUTDOWN,0);
				sprintf(temp,"Gross Gammas = %8.3E in excess of %d!  Aborting.",
					__max(m_fGrossGammaAverage1,m_fGrossGammaAverage2),m_dGrossGammaGate2);
				CWarning Warning;
				Warning.m_szMessage = temp;
				Warning.DoModal();
				m_bShutDown = true;
				result = false;
			}

		}
		else
			Beep(1000,100);
	}


	// 		NET NEUT RATES (C/S)A=  1170.23 [6.71   B=     0.00 [0.00   C=     0.00 [0.00 "^80
	if (strstr(Data,"NET NEUT RATES")!=NULL)
	{
		float ns[3] = {0.,0.,0.};
		float nses[3] = {0.,0.,0.};

		//int cnt =
		sscanf(Data, "%*s %*s %*s %*s %f [%f%*s%f [%f%*s%f [%f%*s",
			&ns[0], &nses[0], &ns[1], &nses[1], &ns[2], &nses[2]);

		m_fNeutronA = ns[0];
		m_fNeutronB = ns[1];
		m_fNeutronC = ns[2];

		sprintf(temp,"%8.3E",m_fNeutronA);
		SetDlgItemText(IDC_GRAND_NA,temp);
		sprintf(temp,"%8.3E",m_fNeutronB);
		SetDlgItemText(IDC_GRAND_NB,temp);
		sprintf(temp,"%8.3E",m_fNeutronC);
		SetDlgItemText(IDC_GRAND_NC,temp);
	}

	// grand 3 neutron collection state only?
	//TIME =     5        A =     1470 CTS    B =        0 CTS    C =        0 CTS   @80
	if ((strstr(Data,"TIME = ")) && (strstr(Data,"A =")))
	{

		int dCNow;

		strncpy(temp0,Data+6, 6);
		strncpy(temp1,Data+23,9);
		strncpy(temp2,Data+43,9);
		strncpy(temp3,Data+63,9);
		temp0[6] = NULL;
		temp1[9] = NULL;
		temp2[9] = NULL;
		temp3[9] = NULL;

		sscanf(temp0,"%d",&m_dGrandTime);// add this to get time value from string

		dCNow = clock();
		if (m_dNeutronCnt == 1)
			m_dCFirst = dCNow;

		m_dGrandTime = ((m_dGrandCycleTime - 1) - m_dGrandTime);

		m_dNeutronCnt++;

		m_fNeutronA = (float)atof(temp1);  //these are accumulated
		m_fNeutronB = (float)atof(temp2);
		m_fNeutronC = (float)atof(temp3);

		if (m_dGrandTime > 0)  // sort of an estimate, not very accurate at all ...
		{
			//double duration = (double)(dCNow - m_dCFirst) / CLOCKS_PER_SEC;

			m_fNeutronAAverage = m_fNeutronA/m_dGrandTime;//duration;
			m_fNeutronBAverage = m_fNeutronB/m_dGrandTime;//duration;
			m_fNeutronCAverage = m_fNeutronC/m_dGrandTime;//duration;

			m_fNeutronA = m_fNeutronAAverage;
			m_fNeutronB = m_fNeutronBAverage;
			m_fNeutronC = m_fNeutronCAverage;
		}
	}

	if (strstr(Data,"NEUTRON COUNTER RATE"))
	{
		strncpy(temp1,Data+24,9);
		strncpy(temp2,Data+44,9);
		strncpy(temp3,Data+64,9);
		temp1[9] = NULL;
		temp2[9] = NULL;
		temp3[9] = NULL;

		m_fNeutronA = (float)atof(temp1);
		m_fNeutronB = (float)atof(temp2);
		m_fNeutronC = (float)atof(temp3);

		sprintf(temp,"%8.3E",m_fNeutronA);
		SetDlgItemText(IDC_GRAND_NA,temp);
		sprintf(temp,"%8.3E",m_fNeutronB);
		SetDlgItemText(IDC_GRAND_NB,temp);
		sprintf(temp,"%8.3E",m_fNeutronC);
		SetDlgItemText(IDC_GRAND_NC,temp);
	}

	return result;
}

void CDoAssay::CrackGains(char *Data, int* iValue1, int* iValue2, int* iValue3, int* iValue4)
{
	if (strlen(Data) < 42) 
	{
		*iValue1 = *iValue2 = *iValue3 = *iValue4 = 0;
		return;
	}
	sscanf(Data,"%*d:%d %*d:%d %*d:%d %*d:%d", iValue1, iValue2, iValue3, iValue4);
}


LRESULT CDoAssay::GrandStateMachine(WPARAM WParam, LPARAM LParam)
{

	char* Data = (char*)LParam;
	char buf[64];

#ifdef _DEBUG
	if (true)
#else
	if (m_pCommLog->bGo)
#endif
	{
		CString cTemp;
		if (WParam == MESSAGE)
			cTemp.Format("State:%d lParam:%s",m_dGrandState,Data);
		else
			cTemp.Format("State:%d (%d)",m_dGrandState, WParam);
		PostToGrand((LPSTR)LPCSTR(cTemp));
#ifdef _DEBUG
		TRACE1("CDoAssay::GrandStateMachine() %s\n",cTemp);
#endif
	}

	if (WParam == GRANDSHUTDOWN)
	{
		m_dGrandState = (m_bMiniGrand)?1800:800;
		m_bGRANDFreeRun = false;
	}

	switch (m_dGrandState) {
	case 0:
		//end anything that is going on
		ClearAccumulator();
		if (m_bMiniGrand)
		{
			TRACE0("DoAssay::GrandStateMachine() Send \\r State:0\n");
			m_pGrandCom->Transmit(-1,"\r");
			m_Grand_TicCount = 0;
			PostToGrand("\\r sent");
			m_dGrandState = 50;
		}
		else
		{
			m_pGrandCom->Transmit(-1,"END\r");
			PostToGrand("END sent");
			m_dGrandState = 100;
			m_pGrandCom->Transmit(-1,"ACQUIRE\r");
			PostToGrand("ACQUIRE sent");
		}
		m_Grand_TicCount = 0;
		break;

	case 50://mini-grand only
		if (WParam == MESSAGE)
		{
			TRACE1("CDoAssay::GrandStateMachine 50-MESSAGE, >%s<\n",Data);
			PostToGrand(Data);
			if ((strstr(Data,"??"))||((Data[0]=='8')&&(Data[1]=='f')))
				m_dGrandState = 1000;
			else if (strstr(Data,"MG Monitor"))
				m_dGrandState = 55;
			else if ((Data[0] == '0') || (Data[0] == 'A'))//| (strstr(Data,"Press Q")))
			{
				m_pGrandCom->Transmit(1,"Q"); 
				PostToGrand("Send Q");
			}
			m_Grand_TicCount = 0;
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			TRACE2("CDoAssay::GrandStateMachine 50-TIMER_GRAND_TIC, %d::%d\n",m_Grand_TicCount,m_dGrandCmdTimeOut);
			PostToGrand("Timer Tic");
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{	
				//post time out error to isr window
				PostToGrand("Timeout on \\r command.");
				m_pGrandCom->Transmit(-1,"\r");
				m_Grand_TicCount = 0;
				m_dGrandState = 55;
			}
		}
		else
		{
			TRACE2("CDoAssay::GrandStateMachine 50-UNKNOWN, %d::%d\n",m_Grand_TicCount,m_dGrandCmdTimeOut);
		}
		break;

	case 55://mini-grand only
		if (WParam == MESSAGE)
		{
			PostToGrand(Data);
			if ((Data[0]=='A')||(Data[0]=='0'))
			{
				m_Grand_TicCount = 0;
				m_dGrandState = 50;
				m_pGrandCom->Transmit(-1,"Q");
			}
			else if (strstr(Data,"MG Setup"))
				m_dGrandState = 1000;
			else if (strstr(Data,"MG Monitor"))
				m_pGrandCom->Transmit(-1,"1\r");
			m_Grand_TicCount = 0;
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{	
				//post time out error to isr window
				PostToGrand("Timeout on last command.");
				m_pGrandCom->Transmit(-1,"1\r");
				m_Grand_TicCount = 0;
				m_dGrandState = 55;
			}
		}
		break;

	case 100://grand III only
		//send setup cycle time
		_itoa(m_dGrandCycleTime,buf,10);
		strcat(buf,"\r");
		m_pGrandCom->Transmit(-1,buf);
		sprintf(buf,"Set time to %d",m_dGrandCycleTime);
		PostToGrand(buf);
		m_dGrandTime = m_dGrandCycleTime;
		SetDlgItemInt(IDC_GRAND_COUNTTIME,m_dGrandCycleTime);
		m_dGrandState = 200;
		break;

	case 200://grand III only
		//start
		m_pGrandCom->Transmit(-1,"NEXT\rDOWN\r");
		PostToGrand("NEXT\r\nDOWN");
		m_dGrandState = 300;
		ClearAccumulator();
		break;

	case 300://grand III only
		//get condition
		m_pGrandCom->Transmit(-1,"ECHO\rUP\rECHO\rDOWN\r");
		PostToGrand("ECHO\r\nUP\r\nECHO\r\nDOWN");
		m_dGrandState = 400;
		m_Grand_TicCount = 0;
		m_dMessCount = 0;
		break;

	case 400://grand III only
		//get status response
		if (m_bGRANDFreeRun)
		{
			m_dGrandState = 800;
			break;
		}
		if (WParam == MESSAGE)
		{
			m_dMessCount++;
			PostToGrand(Data);
			if (strstr(Data,"NET NEUT RATES"))
			{
				Accumulate(Data);
				m_dGrandState = 450;
			}
			else
			{
				Accumulate(Data);

				//				if (g_bLeakDetExcessive)
				//				{
				//					m_dGrandState = 800;
				//					m_bGRANDFreeRun = true;
				//					PostMessage(GOTGRANDVALUE,GOTGRANDVALUE,0);
				//					break;
				//				}
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{	
				//post time out error to isr window
				PostToGrand("Timeout on ECHO-UP-ECHO-DOWN response");
				m_Grand_TicCount = 0;
				m_dGrandState = 300;
			}
			else
			{
				if (m_dMessCount >= 2)
					m_dGrandState = 300;
			}
		}
		break;

	case 450://grand III only
		m_pGrandCom->Transmit(-1,"UP\r");
		PostToGrand("UP 1");
		m_dGrandState = 520;
	case 500://grand III only
		m_pGrandCom->Transmit(-1,"UP\r");
		PostToGrand("UP 2");
	case 510://grand III only
		m_pGrandCom->Transmit(-1,"UP\r");
		PostToGrand("UP 3");
	case 520://grand III only
		m_pGrandCom->Transmit(-1,"ECHO\r");
		PostToGrand("Read Gross Gammas");
		m_dGrandState = 530;
		m_Grand_TicCount = 0;
		break;
	case 530://grand III only
		if (WParam == MESSAGE)
		{
			//post to grand window
			if (strstr(Data,"GROSS GAMMAS:"))
			{
				PostToGrand(Data);
				if (!Accumulate(Data))
				{
					m_dGrandState = 0;
					break;
				}

				//				if (g_bLeakDetExcessive)
				//				{
				//					m_dGrandState = 800;
				//					m_bGRANDFreeRun = true;
				//					PostMessage(GOTGRANDVALUE,GOTGRANDVALUE,0);
				//					break;
				//				}

				m_dGrandState = 540;
			}
			else if (strstr(Data,"NET GAMMA REL DOSE"))
			{
				m_dGrandState = 500;
			}
			else if (strstr(Data,"NEUTRON BKGND SUBTRACT"))
			{
				m_dGrandState = 510;
			}
			else if (strstr(Data,"EXT A, EXT B"))
			{
				m_pGrandCom->Transmit(-1,"DOWN\rDOWN\r");
				m_dGrandState = 520;
				PostMessage(TIMER_GRAND_TIC,0,0);
			}
			else if (strstr(Data,"TESTA,"))
			{
				m_pGrandCom->Transmit(-1,"DOWN\rDOWN\r");
				m_dGrandState = 520;
				PostMessage(TIMER_GRAND_TIC,0,0);
			}
			else if (strstr(Data,"GROSS NEUTRON"))
			{
				m_pGrandCom->Transmit(-1,"DOWN\r");
				m_dGrandState = 520;
				PostMessage(TIMER_GRAND_TIC,0,0);
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				//post time out error to isr window
				PostToGrand("Timeout on Read Gross Gammas");
				//send command again
				m_dGrandState = 520;
				m_Grand_TicCount = 0;
			}
		}
		break;
	case 540://grand III only
		m_pGrandCom->Transmit(-1,"UP\r");
		PostToGrand("UP 4");
		m_dGrandState = 550;
	case 550://grand III only
		m_pGrandCom->Transmit(-1,"ECHO\r");
		PostToGrand("Read Gross Neutron Counts");
		m_dGrandState = 600;
		break;
	case 600://grand III only
		if (WParam == MESSAGE)
		{
			//post to Grand window
			PostToGrand(Data);
			PostMessage(GOTGRANDVALUE,GOTGRANDVALUE,0);

			if (m_dStepIncrement >= m_iCycles)  // bg cycle measurements done
			{			
				m_dGrandState = 700;
			}
			else
			{
				m_dGrandState = 00;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				//post time out error to isr window
				PostToGrand("Timeout on Read Gross Gammas");
				//send command again
				m_dGrandState = 455;
				m_Grand_TicCount = 0;
			}
		}
		else if (WParam == GRANDSHUTDOWN)
		{
			m_dGrandState = 800;
		}
		break;

	case 700://grand III only
		m_pGrandCom->Transmit(-1,"NEXT\r");
		PostToGrand("NEXT");
		m_dGrandState = 800;
		PostMessage(TIMER_GRAND_TIC,0,0);
		break;
	case 800://grand III and mini-grand
		if (m_bGRANDFreeRun)
		{
			if (m_bMiniGrand)
				m_pGrandCom->Transmit(-1,"Q\r");
			m_dGrandState = 900;
		}
		//wait here until command to go to 200 or 0
		break;
	case 900:// grand III and mini-grand
		if (m_bMiniGrand)
		{
			m_pGrandCom->Transmit(-1,"01 1\r");
			m_pGrandCom->Transmit(-1,"87 1\r");
			m_pGrandCom->Transmit(-1,"8f\r");
			m_dGrandState = 1900;
			break;
		}
		m_pGrandCom->Transmit(-1,"END\rNEUT\rDOWN\r");
		PostToGrand("END\r\nNEUT\r\nDOWN");
		m_dGrandState = 940;
	case 930:
		m_pGrandCom->Transmit(-1,"ECHO\r");
		PostToGrand("ECHO");
		m_dGrandState = 940;
		break;
	case 940:
		if (WParam == MESSAGE)
		{
			//post to Grand window
			m_Grand_TicCount = 0;
			Accumulate(Data);

			PostToGrand(Data);
			m_dGrandState = 930;
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				//post time out error to isr window
				PostToGrand("Timeout on Read Neutron Counter Rate");
				//send command again
				m_dGrandState = 800;
				m_Grand_TicCount = 0;
			}
		}
		break;

	case 1000:
		sprintf(buf,"\r01 %d\r",m_bGRANDFreeRun?1:m_dGrandCycleTime);
		m_pGrandCom->Transmit(-1,buf);
		sprintf(buf,"Set time to %d",m_bGRANDFreeRun?1:m_dGrandCycleTime);
		PostToGrand(buf);
		SetDlgItemInt(IDC_GRAND_COUNTTIME,m_bGRANDFreeRun?1:m_dGrandCycleTime);
		ClearAccumulator();
		m_dGrandTime = m_dGrandCycleTime;
		m_dGrandState = 1048;
		break;

	case 1048:
		if (m_psMeasurementData) // bkg not doing it
		{
			sprintf(buf,"\r86 %d\r",m_psMeasurementData->iIonHV);
			m_pGrandCom->Transmit(-1,buf);
			sprintf(buf,"Set Ion HV to %d",m_psMeasurementData->iIonHV);
			PostToGrand(buf);
		}
		m_dGrandState = 1050;
		break;


	case 1050:
		m_pGrandCom->Transmit(-1,"87 1\r");
		PostToGrand("Send 87 1");
		m_Grand_TicCount = 0;
		m_dMessCount = 0;
		m_dGrandState = 1051;
		break;

	case 1051:
		if (m_psMeasurementData) // bkg not doing it
		{
			sprintf(buf,"\r05 %d\r",m_psMeasurementData->iHeHV);
			m_pGrandCom->Transmit(-1,buf);
			m_pGrandCom->Transmit(-1,"1f 1\r");
			sprintf(buf,"Set He HV to %d",m_psMeasurementData->iHeHV);
			PostToGrand(buf);
		}
		m_dGrandState = 1100;
		break;

	case 1100:
		//set to push data to us as received
		m_pGrandCom->Transmit(-1,"8f\r");
		PostToGrand("Send 8f");
		m_Grand_TicCount = 0;
		m_dMessCount = 0;
		m_dGrandState = (m_bGRANDFreeRun)?1900:1200;
		break;

	case 1200:
		if (m_bGRANDFreeRun)
		{
			m_pGrandCom->Transmit(-1,"Q");
			m_dGrandState = 1000;
			break;
		}

		if (WParam == MESSAGE)
		{
			if (((Data[0]=='A')||(Data[0]=='0')) && Data[1]==':')
			{
				PostToGrand(Data);
				Accumulate(Data);
				m_Grand_TicCount = 0;
				if (Data[0]=='0')
				{
					PostMessage(GOTGRANDVALUE,GOTGRANDVALUE,0);
					m_pProgress1.SetPos(m_dGrandCycleTime*(m_dStepIncrement+1));
				}
			}
		}

		if (WParam == TIMER_GRAND_TIC)
		{

			m_pProgress1.SetPos(m_dGrandCycleTime*m_dStepIncrement + m_Grand_TicCount);

			char buff[32];
			sprintf(buff,"%d sec",max(m_dGrandCycleTime - m_Grand_TicCount,0));
			PostToGrand(buff);
			TRACE1("CDoAssay::GrandStateMachine() TICK_GRAND_TIC State 1200, %s\n",buff);
			m_Grand_TicCount++;
			if ((unsigned)(m_Grand_TicCount * TIC_TIMER_TIME) > (unsigned)(2 * m_dGrandCycleTime * 1000))
			{
				PostToGrand("Timeout on Read Current Values");
				m_dGrandState = 1100;
			}
		}
		break;

	case 1800:
		if (m_bGRANDFreeRun)
		{
			m_dGrandState = 1900;
			m_pGrandCom->Transmit(-1,"8f\r");
			PostToGrand("Start Freerun with 8f");
		}
		if (WParam == MESSAGE)
		{
			m_pGrandCom->Transmit(-1,"Q");
			PostToGrand("Sent Q");
		}
		break;

	case 1900:
		//free run		
		if (WParam == MESSAGE)
		{
			if (Data[0]=='A' && Data[1]==':')
			{
				PostToGrand(Data);
				Accumulate(Data);
				m_Grand_TicCount = 0;
				//				if (g_bLeakDetExcessive)
				//				{
				//					PostMessage(GOTGRANDVALUE,GOTGRANDVALUE,0);
				//					break;
				//				}
			}
			m_Grand_TicCount = 0;
		}

		if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if ((unsigned)(m_Grand_TicCount * TIC_TIMER_TIME) > (unsigned)(2 * m_dGrandCycleTime * 1000))
			{
				m_Grand_TicCount = 0;
				PostToGrand("Timeout on FreeRun Read Values");
				m_pGrandCom->Transmit(-1,"Q");
				PostToGrand("Send Q");
				m_dGrandState = 800;
			}
		}
		break;

	case 3000:
		if (m_bMiniGrand)
		{
			m_dGrandState = 3100;
		}
		else
		{
			m_dGrandState = 3600;
		}

		memset(m_iGainOffsetInternal,NULL,sizeof(m_iGainOffsetInternal));
		memset(m_iGainOffsetRemote,NULL,sizeof(m_iGainOffsetRemote));

		{
			char buff[128];
			sprintf(buff,"Beginning Internal Offset Measurements\r\nPlease wait approximately %d seconds...", m_iOffsetMaxTime);
			PostComment(buff);
		}
		m_bOffsetFail = false;
		break;

	case 3100://minigrand
		//send quit
		m_pGrandCom->Transmit(-1,"\r");
		PostToGrand("Sent \\r");
		m_dGrandState = 3110;

	case 3110:
		//send set offsets type 0
		m_pGrandCom->Transmit(-1,"81 0\r80\r");
		PostToGrand("Sent take internal offsets");
		m_Start = COleDateTime::GetCurrentTime();
		m_Grand_TicCount = 0;
		m_dGrandState = 3111;
		break;

	case 3111:
		//wait for response
		//crack response
		if (WParam == MESSAGE)
		{
			PostToGrand(Data);
			m_Grand_TicCount=0;
			if (strstr(Data,"channel 0:"))
			{
				m_iReceivingOffsetChan = 0;
			} 
			else if (strstr(Data,"channel 1:"))
			{
				m_iReceivingOffsetChan = 1;
			} 
			else
			{
				int iBase = -1;
				if (strstr(Data,"0:") && strstr(Data,"1:") && strstr(Data,"2:") && strstr(Data,"3:"))
					iBase = 0;
				else if (strstr(Data,"4:") && strstr(Data,"5:") && strstr(Data,"6:") && strstr(Data,"7:"))
					iBase = 4;
				else if (strstr(Data,"8:") && strstr(Data,"9:") && strstr(Data,"10:") && strstr(Data,"11:"))
					iBase = 8;

				if ((iBase >= 0) && (m_iReceivingOffsetChan >= 0) && (m_iReceivingOffsetChan < 12))
				{
					CrackGains(Data,&m_iGainOffsetInternal[m_iReceivingOffsetChan][iBase + 0],
						&m_iGainOffsetInternal[m_iReceivingOffsetChan][iBase + 1],
						&m_iGainOffsetInternal[m_iReceivingOffsetChan][iBase + 2],
						&m_iGainOffsetInternal[m_iReceivingOffsetChan][iBase + 3]);
				}
				if (iBase == 8 && m_iReceivingOffsetChan == 1)
				{
					m_iReceivingOffsetChan = -1;
					m_dGrandState = 3120;
				}
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			COleDateTime Now = COleDateTime::GetCurrentTime();
			COleDateTimeSpan Time = Now - m_Start;

			CString s;
			s.Format("Offset time %3.lf of %d", Time.GetTotalSeconds(), m_iOffsetMaxTime);
			PostToActions(s.GetBuffer());

			if (Time.GetTotalSeconds() > m_iOffsetMaxTime)
			{
				PostToGrand("Timeout on Internal Offset");
				m_dGrandState = 3120;
				m_bOffsetFail = true;
			}
		}
		break;

	case 3120:
		if (mbWantRemote)
		{
			//send do take offsets type 1
			m_pGrandCom->Transmit(-1,"81 1\r80\r");
			PostToGrand("Sent take remote offsets");
			{
				char buff[128];
				sprintf(buff,"Beginning Remote Offset Measurements\r\nPlease wait approximately %d seconds...", m_iOffsetMaxTime);
				PostComment(buff);
			}
			m_Start = COleDateTime::GetCurrentTime();
			m_Grand_TicCount = 0;
			m_dGrandState = 3121;
		}
		else
		{
			PostComment("Skipping Remote Offset Measurements");
			m_dGrandState = 3800;
		}
		break;

	case 3121:
		//wait for response
		//crack response
		//if timeout send fail message
		if (WParam == MESSAGE)
		{
			PostToGrand(Data);
			if (strstr(Data,"channel 0:"))
			{
				m_iReceivingOffsetChan = 0;
			} 
			else if (strstr(Data,"channel 1:"))
			{
				m_iReceivingOffsetChan = 1;
			} 
			else if (strcmp(Data,"80")==0)
			{
				m_iReceivingOffsetChan = -1;
				m_dGrandState = 3800;
			}
			else
			{
				int iBase = -1;
				if (strstr(Data,"0:") && strstr(Data,"1:") && strstr(Data,"2:") && strstr(Data,"3:"))
					iBase = 0;
				else if (strstr(Data,"4:") && strstr(Data,"5:") && strstr(Data,"6:") && strstr(Data,"7:"))
					iBase = 4;
				else if (strstr(Data,"8:") && strstr(Data,"9:") && strstr(Data,"10:") && strstr(Data,"11:"))
					iBase = 8;

				if ((iBase >= 0) && (m_iReceivingOffsetChan >= 0) && (m_iReceivingOffsetChan < 12))
				{
					CrackGains(Data,&m_iGainOffsetRemote[m_iReceivingOffsetChan][iBase + 0],
						&m_iGainOffsetRemote[m_iReceivingOffsetChan][iBase + 1],
						&m_iGainOffsetRemote[m_iReceivingOffsetChan][iBase + 2],
						&m_iGainOffsetRemote[m_iReceivingOffsetChan][iBase + 3]);
				}
				if (iBase == 8 && m_iReceivingOffsetChan == 1)
				{
					m_iReceivingOffsetChan = -1;
					m_dGrandState = 3800;
				}
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			COleDateTime Now = COleDateTime::GetCurrentTime();
			COleDateTimeSpan Time = Now - m_Start;

			CString s;
			s.Format("Remote offset time %3.lf of %d", Time.GetTotalSeconds(), m_iOffsetMaxTime);
			PostToActions(s.GetBuffer());

			if (Time.GetTotalSeconds() > m_iOffsetMaxTime)
			{
				PostToGrand("Timeout on Remote Offset");
				m_dGrandState = 3800;
				m_bOffsetFail = true;
			}
		}
		break;

	case 3600://grand III
		//send end
		//send offset
		//send down
		//send echo
		m_pGrandCom->Transmit(-1,"END\rOFFSET\rDOWN\rECHO\r");
		PostToGrand("END\r\nOFFSET\r\nDOWN\r\nECHO");
		m_dGrandState = 3605;
		m_Grand_TicCount = 0;
		break;

	case 3605:
		if (WParam == MESSAGE)
		{
			// do the internal measurement
			//if "USE REMOTE" send no yes
			PostToGrand(Data);
			if (strstr(Data,"USE REMOTE"))
			{
				m_pGrandCom->Transmit(-1,"NO\rYES\rUP\rYES\r");
				PostToGrand("No\r\nYes\r\nUp\r\nYes");
				PostToGrand("Sent take internal offsets");
				m_Start = COleDateTime::GetCurrentTime();
				m_dGrandState = 3610;
			}
			else if (strstr(Data,"USE INTERNAL"))
			{
				m_pGrandCom->Transmit(-1,"YES\rYES\rUP\rYES\r");
				PostToGrand("Yes\r\nYes\r\nUp\r\nYes");
				PostToGrand("Sent take internal offsets");
				m_Start = COleDateTime::GetCurrentTime();
				m_dGrandState = 3610;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Start Internal Offset");
				m_dGrandState = 3600;
			}
		}
		break;

	case 3610:
		m_Grand_TicCount = 0;
		m_pGrandCom->Transmit(-1,"ECHO\r");
		PostToGrand("Echo");
		m_dGrandState = 3615;
		break;

	case 3615:
		if (WParam == MESSAGE)
		{
			//ECHO until "TAKE NEW OFFSETS"
			PostToGrand(Data);
			if (strstr(Data,"TAKE NEW OFFSETS?"))
			{
				//send no yes
				//send echo 
				m_pGrandCom->Transmit(-1,"NO\rYES\rECHO\r");
				PostToGrand("No\r\nYes\r\nEcho");
				m_Grand_TicCount = 0;
				m_dGrandState = 3620;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > 5)
				m_dGrandState = 3610;		
			COleDateTime Now = COleDateTime::GetCurrentTime();
			COleDateTimeSpan Time = Now - m_Start;
			if (Time.GetTotalSeconds() > m_iOffsetMaxTime)
			{
				m_bOffsetFail = true;
				m_dGrandState = 3640;
			}
		}
		break;

	case 3620:
		if (WParam == MESSAGE)
		{
			//crack message
			//G GAIN   G1   G2
			//      1  xxxx xxxx
			//      4  xxxx xxxx
			//     16  xxxx xxxx v
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				//crack message
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetInternal[0][0],&m_iGainOffsetInternal[1][0],
					&m_iGainOffsetInternal[0][1],&m_iGainOffsetInternal[1][1],
					&m_iGainOffsetInternal[0][2],&m_iGainOffsetInternal[1][2]);

				//send down echo
				m_pGrandCom->Transmit(-1,"DOWN\rECHO\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3625;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Internal Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3640;
			}
		}

		break;

	case 3625:
		if (WParam == MESSAGE)
		{
			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetInternal[0][3],&m_iGainOffsetInternal[1][3],
					&m_iGainOffsetInternal[0][4],&m_iGainOffsetInternal[1][4],
					&m_iGainOffsetInternal[0][5],&m_iGainOffsetInternal[1][5]);

				//send down echo
				m_pGrandCom->Transmit(-1,"DOWN\rECHO\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3630;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Internal Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3640;
			}
		}
		break;

	case 3630:
		if (WParam == MESSAGE)
		{
			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetInternal[0][6],&m_iGainOffsetInternal[1][6],
					&m_iGainOffsetInternal[0][7],&m_iGainOffsetInternal[1][7],
					&m_iGainOffsetInternal[0][8],&m_iGainOffsetInternal[1][8]);

				//send down echo
				m_pGrandCom->Transmit(-1,"DOWN\rECHO\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3635;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Internal Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3640;
			}
		}
		break;

	case 3635:
		if (WParam == MESSAGE)
		{
			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetInternal[0][9],&m_iGainOffsetInternal[1][9],
					&m_iGainOffsetInternal[0][10],&m_iGainOffsetInternal[1][10],
					&m_iGainOffsetInternal[0][11],&m_iGainOffsetInternal[1][11]);

				//send end
				m_pGrandCom->Transmit(-1,"END\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3640;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Internal Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3640;
			}
		}
		break;

	case 3640://grand III
		//send end
		//send offset
		//send down
		//send echo
		m_pGrandCom->Transmit(-1,"END\r");
		PostToGrand("END");
		if (mbWantRemote)
			m_dGrandState = 3641;
		else
			m_dGrandState = 3800;

		m_Grand_TicCount = 0;
		break;
	case 3641://grand III
		//send end
		//send offset
		//send down
		//send echo
		m_pGrandCom->Transmit(-1,"OFFSET\r");
		PostToGrand("OFFSET");
		m_dGrandState = 3642;
		m_Grand_TicCount = 0;
		break;
	case 3642://grand III
		//send end
		//send offset
		//send down
		//send echo
		m_pGrandCom->Transmit(-1,"DOWN\r");
		PostToGrand("DOWN");
		m_dGrandState = 3643;
		m_Grand_TicCount = 0;
		break;
	case 3643://grand III
		//send end
		//send offset
		//send down
		//send echo
		m_pGrandCom->Transmit(-1,"ECHO\r");
		PostToGrand("ECHO");
		m_dGrandState = 3645;
		m_Grand_TicCount = 0;
		break;

	case 3645:
		if (WParam == MESSAGE)
		{
			//if "USE REMOTE" send no yes
			PostToGrand(Data);
			{
				char buff[128];
				sprintf(buff,"Beginning Remote Offset Measurements\r\nPlease wait approximately %d seconds...", m_iOffsetMaxTime);
				PostComment(buff);
			}
			if (strstr(Data,"USE INTERNAL"))
			{
				m_pGrandCom->Transmit(-1,"NO\rYES\rUP\rYES\r");
				PostToGrand("No\r\nYes\r\nUp\r\nYes");
				PostToGrand("Sent take remote offsets");
				m_Start = COleDateTime::GetCurrentTime();
				m_dGrandState = 3650;
			}
			else if (strstr(Data,"USE REMOTE"))
			{
				m_pGrandCom->Transmit(-1,"YES\rYES\rUP\rYES\r");
				PostToGrand("Yes\r\nYes\r\nUp\r\nYes");
				PostToGrand("Sent take internal offsets");
				m_Start = COleDateTime::GetCurrentTime();
				m_dGrandState = 3650;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > (m_dGrandCmdTimeOut))
			{
				PostToGrand("Timeout on Start Remote Offset");
				m_bOffsetFail = true;
				m_dGrandState = 3800;
			}
		}
		break;

	case 3650:
		m_Grand_TicCount = 0;
		m_pGrandCom->Transmit(-1,"ECHO\r");
		PostToGrand("Echo");
		m_dGrandState = 3655;
		break;

	case 3655:
		if (WParam == MESSAGE)
		{
			//ECHO until "TAKE NEW OFFSETS"
			PostToGrand(Data);
			if (strstr(Data,"TAKE NEW OFFSETS?"))
			{
				//send no yes
				//send echo 
				m_pGrandCom->Transmit(-1,"NO\rYES\rECHO\r");
				PostToGrand("No\r\nYes\r\nEcho");
				m_Grand_TicCount = 0;
				m_dGrandState = 3660;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > 5)
				m_dGrandState = 3650;		
			COleDateTime Now = COleDateTime::GetCurrentTime();
			COleDateTimeSpan Time = Now - m_Start;
			if (Time.GetTotalSeconds() > m_iOffsetMaxTime)
			{
				m_bOffsetFail = true;
				m_dGrandState = 3800;
			}
		}
		break;

	case 3660:
		if (WParam == MESSAGE)
		{
			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				//crack message
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetRemote[0][0],&m_iGainOffsetRemote[1][0],
					&m_iGainOffsetRemote[0][1],&m_iGainOffsetRemote[1][1],
					&m_iGainOffsetRemote[0][2],&m_iGainOffsetRemote[1][2]);

				//send down echo
				m_pGrandCom->Transmit(-1,"DOWN\rECHO\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3665;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Remote Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3800;
			}
		}
		break;

	case 3665:
		if (WParam == MESSAGE)
		{

			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetRemote[0][3],&m_iGainOffsetRemote[1][3],
					&m_iGainOffsetRemote[0][4],&m_iGainOffsetRemote[1][4],
					&m_iGainOffsetRemote[0][5],&m_iGainOffsetRemote[1][5]);

				//send down echo
				m_pGrandCom->Transmit(-1,"DOWN\rECHO\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3670;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Remote Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3800;
			}
		}
		break;

	case 3670:
		if (WParam == MESSAGE)
		{
			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetRemote[0][6],&m_iGainOffsetRemote[1][6],
					&m_iGainOffsetRemote[0][7],&m_iGainOffsetRemote[1][7],
					&m_iGainOffsetRemote[0][8],&m_iGainOffsetRemote[1][8]);

				//send down echo
				m_pGrandCom->Transmit(-1,"DOWN\rECHO\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3675;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Remote Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3800;
			}
		}
		break;

	case 3675:
		if (WParam == MESSAGE)
		{
			//crack message
			PostToGrand(Data);
			if (strstr(Data,"G GAIN   G1   G2")!=0)
			{
				sscanf(Data,"%*s %*s %*s %*s %*d %d %d %*d %d %d %*d %d %d",
					&m_iGainOffsetRemote[0][9],&m_iGainOffsetRemote[1][9],
					&m_iGainOffsetRemote[0][10],&m_iGainOffsetRemote[1][10],
					&m_iGainOffsetRemote[0][11],&m_iGainOffsetRemote[1][11]);

				//send end
				m_pGrandCom->Transmit(-1,"END\r");
				m_Grand_TicCount = 0;
				//go to next
				m_dGrandState = 3800;
			}
		}
		else if (WParam == TIMER_GRAND_TIC)
		{
			m_Grand_TicCount++;
			if (m_Grand_TicCount > m_dGrandCmdTimeOut)
			{
				PostToGrand("Timeout on Remote Offset Message");
				m_bOffsetFail = true;
				m_dGrandState = 3800;
			}
		}
		break;

	case 3800:
		//analyse results
		{
			bool bOffsetsOOBOk = true;
			for (int iChan = 0; iChan < 2; iChan++)
				for(int iGain = 0; iGain < 12; iGain++)
				{
					if ((m_bOffset && m_iGainOffsetInternal[iChan][iGain] > m_iOffsetMax) || 
						(m_iGainOffsetInternal[iChan][iGain] < m_iOffsetMin))
						bOffsetsOOBOk = false;

					if (mbWantRemote &&
						((m_iGainOffsetRemote[iChan][iGain] > m_iOffsetMax) ||
						(m_iGainOffsetRemote[iChan][iGain] < m_iOffsetMin)))
						bOffsetsOOBOk = false;
				}

				//post appropriate messages to state machine
				PostMessage(NEXTSTATE,(bOffsetsOOBOk)?HAVEGRANDOFFSETS:GRANDOFFSETOOB,(m_bOffsetFail)?1:0);
				//go to wait state
				m_dGrandState = 3900;
		}
		break;

	case 3900:
		//wait here
		break;
	}

	if (WParam == MESSAGE)
	{
		if ((char*)LParam != NULL)
			free((char*)LParam);
	}
	return 0;

}

LRESULT CDoAssay::GrandCmdAccumulator(WPARAM/* WParam*/, LPARAM LParam)
{
	ComData* ReceiveData = (ComData*)LParam;
	for (int i = 0; i < ReceiveData->Size; i++)
	{
		if (ReceiveData->Data[i] == '\n')
		{}
		else
			if (ReceiveData->Data[i] == '\r')
			{
				char *ToSend = (char *)malloc(strlen(m_szGrandCmdBuffer)+1);
				strcpy(ToSend,m_szGrandCmdBuffer);
				PostMessage(RECEIVE_GRAND_DAT,MESSAGE,(LPARAM)ToSend);
				m_szGrandCmdBuffer[0] = NULL;
			}
			else
			{
				strncat(m_szGrandCmdBuffer,&(ReceiveData->Data[i]),1);
			}
	}
	free(ReceiveData->Data);  // allocated with malloc
	free((ComData*)LParam);
	return 0;

}

LRESULT CDoAssay::StateMachine(WPARAM WParam, LPARAM LParam)
{

#ifdef _DEBUG	
	if (true)
#else
	if (m_pActivityLog->bGo)
#endif
	{
		CString cTemp;
		cTemp.Format("%d",m_dCurrentState);
		PostToActions((LPSTR)LPCSTR(cTemp));
	}

	if (WParam == SHUTDOWN)
		m_dCurrentState = 9100;

	//	if (m_dCurrentState != 9200 && m_dCurrentState != 9210 && g_bLeakDetExcessive)
	//	{
	//		CBOk->EnableWindow(false);
	//		m_dCurrentState = 9200;
	//		m_bGRANDFreeRun = true;
	//		m_dGrandState = 800;
	//	}
	//

	if (WParam == GOTGRANDVALUE)
	{
		TRACE1("DoAssay::StateMachine() GOTGRANDVALUE message.  State:%d\n",m_dCurrentState);
		PostToActions(m_bMiniGrand?"Received Mini-GRAND Data":"Received GRAND Data");
		m_bHaveGrand = true;
		//		m_dISRSubCount++;
	}

	switch (m_dCurrentState)
	{
	case 0:
		if (WParam == 0)
		{
			if (strlen(m_szSound))
			{
				char temp[_MAX_PATH];
				strcpy(temp,m_szSound);
				strcat(temp,"Sound2.wav");
				//				PlaySound(temp,NULL,SND_ASYNC);
			}
			PostToActions("Verify Item ID");
			CBOk->EnableWindow(true);
			PostInstruction("Position Item to verify ID.\nClick on the \"ACCOMPLISHED\" button below when ready to continue.");
			PostComment("Item identification MUST be verified!");
		}

		if (WParam == ACCOMPLISHED)
		{
			if (strlen(m_szSound))
			{
				char temp[_MAX_PATH];
				strcpy(temp,m_szSound);
				strcat(temp,"Sound3.wav");
				//				PlaySound(temp,NULL,SND_ASYNC);
			}
			m_dCurrentState = 100;
			PostInstruction("Enter the verified Item ID.");
			CBOk->EnableWindow(false);
			PostMessage(NEXTSTATE,0,0);
		}
		break;

	case 100:
		if (WParam == 0)
		{
			CPrompt Prompt(NULL,false);
			Prompt.m_StaticInstructions = "Please verify and enter the Item ID.";
			Prompt.m_Title = "Verify Item ID";
			if (Prompt.DoModal() == IDOK)
			{
				if (strcmp(Prompt.m_EditString,m_cID)==0)
				{
					switch (m_iMode) {
					case eBackgroundAssay://background -- should never happen
						m_dCurrentState = 300;
						break;
					case eNormalAssay://PASSIVE
					case eBaselineAssay://baseline
					case eReVerificationAssay://reverification
						m_dCurrentState = 1300;
					}

					strcpy(m_pText,"ID verified as: ");
					strcat(m_pText,Prompt.m_EditString);
					PostToActions(m_pText);
				}
				else
				{
					sprintf(m_pText,"Verify Item ID failed. Selected: %s Input: %s",
						m_cID,
						Prompt.m_EditString);
					PostToActions(m_pText);
					PostComment("Verify Item ID failed.");
					m_dCurrentState = 9400;
				}
			}
			else
			{
				sprintf(m_pText,"Verify Item ID canceled. Selected: %s Input: %s",
					m_cID,
					Prompt.m_EditString);
				PostToActions(m_pText);
				PostComment("Verify Item ID canceled.");
				m_dCurrentState = 9400;
			}

			//			g_hWnd = m_hWnd;
			PostMessage(NEXTSTATE,0,0);

			//SetActiveWindow();
			//::SetWindowPos(this->m_hWnd, 
			//	HWND_TOP, -1,-1,-1,-1,SWP_NOMOVE | SWP_NOSIZE);
			//::EnableWindow(this->m_hWnd, TRUE);

		}
		break;

		//300 background
	case 300:
		if (!m_bOffset)
		{
			m_dCurrentState = 399;
			PostMessage(NEXTSTATE,0,0);
			PostToActions("Bypassing Offset Measurements");
			PostComment("Bypassing Offset Measurements");
		}
		else
		{
			m_dCurrentState = 310;
			CBOk->EnableWindow(false);
			PostMessage(NEXTSTATE,0,0);
			PostInstruction("Wait");
			PostToActions("Beginning Offset Measurements");
			PostComment("Beginning Offset Measurements");
		}
		break;

	case 310:
		m_dGrandState = 3000;
		m_dCurrentState = 315;
		break;

	case 315:
		if (WParam == HAVEGRANDOFFSETS)
		{
			m_dCurrentState = 399;
			PostMessage(NEXTSTATE,0,0);
			if (LParam == 1)
			{
				PostComment("Take Offset Measurements failed.\n"
					"Return and retry background measurement!");
				m_bPass = false;
				m_dCurrentState = 9110;
			}
			else
				PostComment("Received Good Offset Measurements.");
			m_dGrandState = 300;
		}
		else if (WParam == GRANDOFFSETOOB)
		{
			m_dCurrentState = 399;
			PostMessage(NEXTSTATE,0,0);
			if (LParam == 1)
			{
				PostComment("Take Offset Measurements failed.\n"
					"Return and retry background measurement!");
				m_bPass = false;
				m_dCurrentState = 9110;
			}
			else
			{
				PostComment("One or more Offset Measurements OUT OF BOUNDS!\n"
					"Investigate and Fix GRAND offset problem!");
				m_bGRANDFreeRun = true;
				//if failed ISR ten
				m_bPass = false;
				//abort
				m_dCurrentState = 9110;
				PostMessage(NEXTSTATE,0,0);
			}
			m_dGrandState = 300;
		}

		break;

	case 399:
		m_dCurrentState = 400;
		CBOk->EnableWindow(false);
		PostMessage(NEXTSTATE,0,0);
		PostToActions("Beginning Background Measurements");
		PostComment("Beginning Background Measurements");
		{
			CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
			m_iTotalSeconds = m_iCycleTime*m_iCycles;
			pProgressCtrl->SetRange32(0,m_iTotalSeconds);
			pProgressCtrl->SetPos(0);
		}
		break;

	case 400:
		//setup values 
		if (WParam != 0)
			break;
		m_dStepIncrement = 0;	
		m_dCurrentState = 401;
		//fall through
	case 401:
		if (WParam == 0)
		{
			if ((m_dStepIncrement >= 10) || 
				(gl_DataRecord.ChPos[(m_dStepIncrement>=10)?9:m_dStepIncrement] == ENDFLAG))
			{
				m_dStepIncrement = 0;
				m_dCurrentState = 8000;
				PostMessage(NEXTSTATE,0,0);

			}
			else //if (m_bDoingBackground)
			{
				if (strlen(m_szSound))
				{
					char temp[_MAX_PATH];
					strcpy(temp,m_szSound);
					strcat(temp,"Sound7.wav");
					//					PlaySound(temp,NULL,SND_ASYNC);
				}
				m_dCurrentState = 405;
				//restart grand
				m_dGrandState = 0;

				PostMessage(NEXTSTATE,0,0);
			}
		}
		break;

	case 404:
		if (WParam == GRANDDATA)
		{
#ifdef _DEBUG	
			CString cTemp;
			cTemp.Format("%d GOT GRANDDATA",m_dCurrentState);
			PostToActions((LPSTR)LPCSTR(cTemp));
#endif
			//			m_dISRSubCount++;
			//			if (m_dISRSubCount > 6)
			//			{
			////				m_bKillingINCC = false;
			//				m_dCurrentState = 405;
			//				PostMessage(NEXTSTATE,ACCOMPLISHED,0);
			//			}
		}
		break;

	case 405:
		//comment window
		PostComment("Background Assay\nStarting assay cycle.");
		//no instructions
		PostInstruction("Wait");
		//don't let grand continue past each measurement
		m_bGRANDFreeRun = false;

		m_dGrandCycleTime = m_iCycleTime;
		m_fCurrentCountTime = (float)m_iCycleTime;
		
		if (bDemoVersion)
		{
			CString cString;
			cString.Format("P%d",m_dStepIncrement+1);
			UpdateParams((LPSTR)LPCSTR(cString));//"P1");
		}
		//		//restart grand
		//		m_dGrandState = 0;

		//clear the grand data
		//on every cycle, cycle data is collected separately in g_sScanRecs and averaged by the "controlling authority" on termination
		ClearAccumulator();

		//go to next state
		m_dCurrentState = 407;

		//not waiting for accomplish so disable button
		CBOk->EnableWindow(false);

		//trigger next state now
		PostMessage(NEXTSTATE,0,0);

		break;

	case 407:
		m_bHaveGrand = false;
		m_dCurrentState = 410;
		PostToActions("Assay Started");
		PostMessage(NEXTSTATE,0,0);
		break;

	case 410:
		m_dCurrentState = 420;
		sprintf(m_szTemp,"Background Scan %d.",
			m_dStepIncrement+1);
		PostToActions(m_szTemp);
		strcat(m_szTemp,"\nWait for completion.");
		PostComment(m_szTemp);
		PostMessage(NEXTSTATE,0,0);
		break;

	case 417:
		m_dCurrentState = 420;  // next state will be data collection

		if (m_dStepIncrement + 1 < m_psMeasurementData->iPoints)
		{
			sprintf(m_szTemp,"Completing Background cycle %d of %d, prior to early termination.\n"
				"Please wait for the current cycle to complete.",
				m_dStepIncrement + 1,
				(short)m_psMeasurementData->iPoints);
		}
		else
		{
			sprintf(m_szTemp,"Completing last Background cycle %d of %d.\n"
				"Please wait for the current cycle to complete.",
				m_dStepIncrement + 1,
				(short)m_psMeasurementData->iPoints);
		}

		PostComment(m_szTemp);

		break;
	case 420:
		//wait for assay value
		//then go to next state
		if (m_bHaveGrand)
		{
			//if we just now got the grand value 
			//then free run it
			if (WParam == GOTGRANDVALUE)
			{
				m_bGRANDFreeRun = true;
				//store all the values
				if (m_dStepIncrement >= 0)
				{
					//COleDateTime Now = COleDateTime::GetCurrentTime();
					//strcpy(g_sScanRecs[m_dStepIncrement].szYMD,LPCSTR(Now.Format("%Y/%m/%d")));
					//strcpy(g_sScanRecs[m_dStepIncrement].szHMS,LPCSTR(Now.Format("%H:%M:%S")));
					//g_sScanRecs[m_dStepIncrement].iTimeSec			= m_dGrandTime;
					g_sScanRecs[m_dStepIncrement].dGrossGammaAve1	= m_fGrossGammaAverage1;
					g_sScanRecs[m_dStepIncrement].dGrossGammaAve2	= m_fGrossGammaAverage2;
					g_sScanRecs[m_dStepIncrement].dNeutronA			= m_fNeutronA;
					g_sScanRecs[m_dStepIncrement].dNeutronB			= m_fNeutronB;
					g_sScanRecs[m_dStepIncrement].dNeutronC			= m_fNeutronC;
				}

				//set next step
				m_dStepIncrement++;
				g_dScanRecMax = m_dStepIncrement;
				//set message to tell user
				PostToGrand("Measurement Completed");
				PostToActions("Measurement Completed");
				if (strlen(m_szSound))
				{
					char temp[_MAX_PATH];
					strcpy(temp,m_szSound);
					strcat(temp,"Sound8.wav");
					//					PlaySound(temp,NULL,SND_SYNC);
				}
				if (m_dStepIncrement >= m_iCycles)
				{
					m_dStepIncrement = 0;
					m_dCurrentState = 8000;
					PostMessage(NEXTSTATE,0,0);
				}
				else
				{
					m_dCurrentState = 405;
					PostMessage(NEXTSTATE,0,0);
				}
			}
		}
		break;

		//1300 Normalization, baseline, and reverification
	case 1300:
		m_dCurrentState = 1400;
		CBOk->EnableWindow(false);

		switch (m_iMode) {
	case eBackgroundAssay: //should never happen
		PostToActions("ERROR: Beginning Background Measurements");
		PostComment("ERROR: Beginning Background Measurements");
		break;
	case eNormalAssay:
		PostToActions("Beginning Normal Measurements");
		PostComment("Beginning Normal Measurements");
		break;
	case eBaselineAssay:
		PostToActions("Beginning Baseline Measurements");
		PostComment("Beginning Baseline Measurements");
		break;
	case eReVerificationAssay:
		PostToActions("Beginning Re-Verification Measurements");
		PostComment("Beginning Re-Verification Measurements");
		break;
		}

		PostMessage(NEXTSTATE,0,0);
		m_dStepIncrement = 0;
		break;

	case 1400:
		//setup values 
		if (WParam != 0)
			break;
		m_dCurrentState = 1401;
		m_bGRANDFreeRun = true;
		//fall through

	case 1401:
		if (WParam == 0)
		{
			if ((m_dStepIncrement >= 10) || 
				(gl_DataRecord.ChPos[(m_dStepIncrement>=10)?9:m_dStepIncrement] == ENDFLAG))
			{
				m_dCurrentState = 8000;
				PostMessage(NEXTSTATE,0,0);
			}
			else
			{
				if (strlen(m_szSound))
				{
					char temp[_MAX_PATH];
					strcpy(temp,m_szSound);
					strcat(temp,"Sound5.wav");
					//					PlaySound(temp,NULL,SND_ASYNC);
				}

				m_pProgress1.SetPos(0);
				m_pProgress2.SetPos(m_dStepIncrement);

				/*sprintf(m_szTemp,"Neutron Assay #%d\nPosition Item at %+d %s now.",
				m_dStepIncrement+1,
				gl_DataRecord.ChPos[m_dStepIncrement],
				m_szScale);*/
				//sprintf(m_szTemp, "Correct assembly measurement position.");
				sprintf(m_szTemp,"\nClick on the \"ACCOMPLISHED\" button below when in position and ready to begin measurement.");
				PostInstruction(m_szTemp);
				sprintf(m_szTemp,"Neutron Assay, cycle: %d",// at %+d %s",
					m_dStepIncrement+1 //,
					//gl_DataRecord.ChPos[m_dStepIncrement],
					//m_szScale
					);
				PostToActions(m_szTemp);
				CBOk->EnableWindow(true);

				PostComment("Neutron Assay\nPosition of Item MUST be verified!");
				m_dCurrentState = 1405;
			}
		}
		break;

	case 1404:
		if (WParam == GRANDDATA)
		{
#ifdef _DEBUG	
			CString cTemp;
			cTemp.Format("%d GOT GRANDDATA",m_dCurrentState);
			PostToActions((LPSTR)LPCSTR(cTemp));
#endif
		}
		break;

	case 1405:
		if (WParam == ACCOMPLISHED || (m_dStepIncrement > 0))  // adjusted for new cycling assay, force ack only on first cycle start
		{
			//comment window
			PostComment("Neutron Assay\nStarting assay cycle.");
			//no instructions
			PostInstruction("Wait");
			if (strlen(m_szSound))
			{
				char temp[_MAX_PATH];
				strcpy(temp,m_szSound);
				if (m_iMode == eBackgroundAssay)
					strcat(temp, "Sound7.wav");
				else if (m_iMode == eNormalAssay)
					strcat(temp, "Sound16.wav");
				else if (m_iMode == eBaselineAssay)
					strcat(temp,"Sound14.wav");
				else if (m_iMode == eReVerificationAssay)
					strcat(temp,"Sound15.wav");
				else 
					strcat(temp,"Sound7.wav");
				//				PlaySound(temp,NULL,SND_ASYNC);
			}
			//don't let grand continue past each measurement
			m_bGRANDFreeRun = false;
			//setup to receive data from isr & grand

			//			m_dGrandCycleTime = GETPROFINT(eCYCLETIME);
			//			m_fCurrentCountTime = (float)m_dGrandCycleTime;
			if (bDemoVersion)
			{
				sprintf(m_szTemp,"P%d",m_dStepIncrement+1);
				UpdateParams(m_szTemp);
			}

			//restart grand
			m_dGrandState = 0;
			if (m_bMiniGrand)
				m_pGrandCom->Transmit(-1,"\rQ\r");

			//clear grand data
			//on every cycle, cycle data is collected separately in g_sScanRecs and averaged by the "controlling authority" on termination
			ClearAccumulator();
			//go to next state
			m_dCurrentState = 1407;
			//not waiting for accomplish so disable button
			CBOk->EnableWindow(false);
			//trigger next state now
			PostMessage(NEXTSTATE,0,0);
		}
		break;

	case 1407:
		m_bHaveGrand = false;
		m_dCurrentState = 1410;
		PostToActions("Assay Started");
		PostMessage(NEXTSTATE,0,0);
		break;

	case 1410:
		m_dCurrentState = 1420;
		m_dGrandState = 0;
		sprintf(m_szTemp,"Passive Neutron Assay cycle %d of %d\n" // at %+d %s.\n"
			//			"Doing assay %d of up to %d.\nWait for completion.",
			"Wait for completion.",
			m_dStepIncrement + 1,
			(short)m_psMeasurementData->iPoints
			//gl_DataRecord.ChPos[m_dStepIncrement],
			//m_szScale
			//			min(m_dISRSubCount,INCCInput.dMaxNumRun),
			//			INCCInput.dMaxNumRun
			);

		PostComment(m_szTemp);
		break;
	case 1417:
		m_dCurrentState = 1420;  // next state will be data collection

		if (m_dStepIncrement + 1 < m_psMeasurementData->iPoints)
		{
			sprintf(m_szTemp,"Completing cycle %d of %d, prior to early termination.\n"
				"Please wait for the current cycle to complete.",
				m_dStepIncrement + 1,
				(short)m_psMeasurementData->iPoints);
		}
		else
		{
			sprintf(m_szTemp,"Completing last cycle %d of %d.\n"
				"Please wait for the current cycle to complete.",
				m_dStepIncrement + 1,
				(short)m_psMeasurementData->iPoints);
		}

		PostComment(m_szTemp);

		break;
	case 1420:
		//wait for assay value
		//then go to next state
		if (m_bHaveGrand)
		{
#ifdef _DEBUG	
			CString cTemp;
			cTemp.Format("%d m_bHaveGrand", m_dCurrentState);
			PostToActions((LPSTR)LPCSTR(cTemp));
#endif
			m_dCurrentState = 1401;
			//store all the values
			if (m_dStepIncrement >= 0)
			{
				//COleDateTime Now = COleDateTime::GetCurrentTime();
				//strcpy(g_sScanRecs[m_dStepIncrement].szYMD,LPCSTR(Now.Format("%Y/%m/%d")));
				//strcpy(g_sScanRecs[m_dStepIncrement].szHMS,LPCSTR(Now.Format("%H:%M:%S")));
				//g_sScanRecs[m_dStepIncrement].iTimeSec			= m_dGrandTime;
				g_sScanRecs[m_dStepIncrement].dGrossGammaAve1	= m_fGrossGammaAverage1;
				g_sScanRecs[m_dStepIncrement].dGrossGammaAve2	= m_fGrossGammaAverage2;
				g_sScanRecs[m_dStepIncrement].dNeutronA			= m_fNeutronA;
				g_sScanRecs[m_dStepIncrement].dNeutronB			= m_fNeutronB;
				g_sScanRecs[m_dStepIncrement].dNeutronC			= m_fNeutronC;
			}

			//set next step
			m_dStepIncrement++;
			g_dScanRecMax = m_dStepIncrement;
			//reset hv
			//set message to tell user
			PostToGrand("Measurement Completed");
			PostToActions("Measurement Completed");
			if (strlen(m_szSound))
			{
				char temp[_MAX_PATH];
				strcpy(temp,m_szSound);
				strcat(temp,"Sound8.wav");
				//				PlaySound(temp,NULL,SND_ASYNC);
			}
			/****

			m_dGrandState = 800;
			m_bGRANDFreeRun = true;

			*****/
			if (m_dStepIncrement >= m_iCycles) 
			{
				m_dStepIncrement = 0;
				m_dCurrentState = 8000;
			}
			else
			{
				m_dCurrentState = 1405;
			}
			PostMessage(NEXTSTATE,0,0);
		}
		break;

	case 8000:
		PostComment("Assay completed.");
		KillTimer(TIMER_GRAND_TIC);
		PostToActions("Doing Computations...");
		CBOk->EnableWindow(false);
		PostInstruction("Wait");
		PostComment("Wait.\nDoing computations now...");
		switch (m_iMode) {
	case eBackgroundAssay: //background
	case eNormalAssay://normalization
	case eBaselineAssay://baseline
	case eReVerificationAssay://re-verification
		m_bPass = true;
		break;
	default:
		m_bPass = false;
		}	
		m_dCurrentState = 9000;
		PostMessage(NEXTSTATE,0,0);
		break;

	case 9000:
		m_bShutDown = true;
		SetDlgItemText(IDCANCEL,"\r\nRETURN\r\n F12");
		PostInstruction("Click on the \"RETURN\" button.");
		PostToActions("Terminus (9000)");

		TerminateDetectorMeasurement();// not in 9000, only in 9110 , why? m_pGrandCom->Transmit(-1,"87 0\r");

		m_bGRANDFreeRun = false;
		m_dGrandState = (m_bMiniGrand)?1800:800;
		CDialog::OnOK();
		break;

	case 9100:
		PostComment("Assay aborted due to high Gamma value");
		PostToActions("Assay aborted due to high Gamma value");
		m_dCurrentState = 9110;
		PostMessage(NEXTSTATE,0,0);
		break;

	case 9110://wait for user to click on return
		m_bShutDown = true;
		SetDlgItemText(IDCANCEL,"\r\nRETURN\r\n F12");
		PostInstruction("Click on the \"RETURN\" button.");
		PostToActions("Terminus (9110)");
		if (m_bMiniGrand)
		{
			m_pGrandCom->Transmit(-1,"Q");
			m_pGrandCom->Transmit(-1,"87 0\r");
		}
		m_pGrandCom->UnRegister(GRAND_NODE);
		m_bGRANDFreeRun = false;
		m_dGrandState = (m_bMiniGrand)?1800:800;
		break;

		//	case 9200:
		//		if (strlen(m_szSound))
		//		{
		//			char temp[_MAX_PATH];
		//			strcpy(temp,m_szSound);
		//			strcat(temp,"Sound13.wav");
		//			PlaySound(temp,NULL,SND_ASYNC);
		//		}
		//		m_bPass = false;
		//
		//		PostComment("Assay aborted do to detector leak");
		//		PostToActions("Assay aborted. Detector is leaking!");
		//		m_bShutDown = true;
		//		SetDlgItemText(IDCANCEL,"RETURN\r\n F12");
		//		PostInstruction("WARNING: Detector is leaking!\nTurn off all electronics and remove counter from water!\nClick on the \"RETURN\" button.");
		//		m_dGrandState = 800;
		//		m_bGRANDFreeRun = true;
		//		m_dCurrentState = 9210;
		//		PostMessage(NEXTSTATE,0,0);
		//		break;

	case 9210:
		PostToActions("Terminus (9210)");
		break;

	case 9300:
		m_dCurrentState = 9110;
		PostMessage(NEXTSTATE,0,0);
		m_bPass = false;
		break;

	case 9400:
		PostComment("Verify Item failed.\nAssay aborted!");
		PostToActions("Verify Item failed.");
		m_bPass = false;
		KillTimer(TIMER_GRAND_TIC);
		CDialog::OnOK();
		break;

	default: 
		m_dCurrentState = 9000;
		PostComment("State Machine Failure");
		PostToActions("State Machine Failure");
		m_dCurrentState = 9110;
		PostMessage(NEXTSTATE,0,0);
	}

	if (WParam == MESSAGE)
	{
		try 
		{
			delete [] (char*)LParam;
		}
		catch(...)
		{
		}
	}

	return 0;
}

int CDoAssay::DoModal(int AssayIndex) 
{
	if (AssayIndex == -1)
	{
		m_bDoingBackground = true;
		m_dAssayIndex = 0;
		m_dCurrentState = 300;
	}
	else
	{
		m_bDoingBackground = false;
		m_dAssayIndex = AssayIndex;
	}

	return CDialog::DoModal();
}

int CDoAssay::DoModal(CString cString) 
{
	m_bDoingBackground = false;
	//m_dAssayIndex = 0;
	m_cID = cString;
	return CDialog::DoModal();
}





void CDoAssay::OnWatchButton() 
{
	//toggle showing the bottom half of the dialog box
	RECT rect, rect2;
	::GetWindowRect(m_hWnd,&rect);

	::GetWindowRect(GetDlgItem(IDC_STATIC_WATCH)->m_hWnd,&rect2);

	SetWindowPos(&wndTop,
		rect.left,
		rect.top,
		rect.right-rect.left,
		m_bMinimal?(m_Rect.bottom-m_Rect.top):(rect2.top-rect.top),
		SWP_FRAMECHANGED  );
	m_bMinimal = !m_bMinimal;
	CBWatch->SetWindowText(m_bMinimal? WATCHBUTTONTEXT:NOWATCHBUTTONTEXT);
}

void CDoAssay::OnClose() 
{
	CDialog::OnClose();
}

BOOL CDoAssay::DestroyWindow() 
{
	TerminateDetectorMeasurement();
	return CDialog::DestroyWindow();
}

BOOL CDoAssay::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_F4)
		{
			OnWatchButton();
			return TRUE;
		}
		if (pMsg->wParam == VK_F8)
		{
			if (GetDlgItem(IDOK)->IsWindowEnabled())
				OnOK();
			return TRUE;
		}
		if (pMsg->wParam == VK_F12)
		{
			OnCancel();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDoAssay::OnOK() 
{
	PostMessage(NEXTSTATE,ACCOMPLISHED,0);	
}

void CDoAssay::TerminateDetectorMeasurement()
{
	if (m_bMiniGrand)
		m_pGrandCom->Transmit(-1,"Q");
	m_pGrandCom->UnRegister(GRAND_NODE);

}

bool CDoAssay::bHasAbortableStatus() const
{
	int checkstate;
	switch (m_iMode)
	{
	case eBackgroundAssay://background 
		checkstate = 400;
		break;
	case eNormalAssay://PASSIVE
	case eBaselineAssay://baseline
	case eReVerificationAssay://reverification
	default:
		checkstate = 1406;  // have not acknowledged source placement yet
	}
	if (m_dCurrentState < checkstate || m_dStepIncrement < 0)  // measurement cycle never started
	{
		return false;
	}
	else
		return true;

}
void CDoAssay::OnCancel() 
{
	if (m_bShutDown)
	{
		m_bPass = false;
		TerminateDetectorMeasurement();
		CDialog::OnOK();
	}
	else
	{
		// Enable the abort button when it is possible to abort rather than just stop

		// the other issue is that the incremental progress bar gets screwed up by the abort technique
		CAbort Abort(this);
		INT_PTR r = Abort.DoModal();
		if (r==IDOK)
		{
			m_bPass = false;
			TerminateDetectorMeasurement();
			CDialog::OnCancel();
		}
		else if (r==CAbort::AbortButKeepWhatWeHave)
		{

			// if a measurement cycle has not yet begun, e.g. background
			// offset being taken, or not yet started first cycle of standard measurement,
			// then need to do the same thing as IDOK above

			int checkstate, nextstate;
			switch (m_iMode)
			{
			case eBackgroundAssay://background 
				checkstate = 400;
				nextstate = 417;
				break;
			case eNormalAssay://PASSIVE
			case eBaselineAssay://baseline
			case eReVerificationAssay://reverification
			default:
				checkstate = 1406;  // have not acknowledged source placement yet
				nextstate = 1417;
			}
			if (m_dCurrentState < checkstate || m_dStepIncrement < 0)  // measurement cycle never started
			{
				m_bPass = false;
				TerminateDetectorMeasurement();
				CDialog::OnCancel();
			}
			else
			{
				// A measurement cycle has already begun, set #cycles to be 1 more than unfinished current counter.
				// This allows the state mahcine to finish current measurement (state 1420/420)
				m_dCurrentState = nextstate;

				m_iCycles = m_dStepIncrement + 1; // force termination by this trick

				PostMessage(NEXTSTATE,0,0);
			}
		}
		//		g_hWnd = m_hWnd;
	}
}

void CDoAssay::OnAck() 
{

	PostMessage(NEXTSTATE,ACCOMPLISHED,0);	

}

void CDoAssay::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);

}
