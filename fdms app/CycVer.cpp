//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: CycVer.cpp $
// 
// *****************  Version 6  *****************
// User: 186846       Date: 2/26/08    Time: 8:55a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 5  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "fdms.h"
#include "CycVer.h"
#include "fdmsDoc.h"
#include "testdrawview4.h"
#include "selectmeasurement.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCycVer dialog
HWND g_hCycVer;


CCycVer::CCycVer(CWnd* pParent /*=NULL*/)
: CDialog(CCycVer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCycVer)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//	m_bIncludeGood = false;
	//	m_bIncludeSuspect = false;
	m_preIndex = -1;
}


void CCycVer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCycVer)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCycVer, CDialog)
	//{{AFX_MSG_MAP(CCycVer)
	ON_BN_CLICKED(ID_BUTTON_AO_MEASURE,OnMeasure)
	ON_BN_CLICKED(ID_BUTTON_AO_IG,OnIncludeGood)
	ON_BN_CLICKED(ID_BUTTON_AO_IS,OnIncludeSuspect)
	ON_BN_CLICKED(ID_BUTTON_AO_AG,OnAcceptGood)
	ON_BN_CLICKED(ID_BUTTON_AO_RETURN,OnReturn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MY_SHOW,OnMyShow)
	ON_MESSAGE(MY_SELECT,OnMySelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCycVer message handlers
void CCycVer::OnMeasure()
{

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	if (pApp->GetMeasurementDialogPtr())
		return;

	ShowWindow(SW_HIDE);
	CSelectMeasurement cSelectMeasurement;
	cSelectMeasurement.m_iSubsetSelector = CycleVerification;
	cSelectMeasurement.m_preIndex = m_preIndex;
	cSelectMeasurement.DoModal();
	if (::IsWindow(m_hWnd))
		ShowWindow(SW_SHOW);
	m_preIndex = -1;
}

void CCycVer::OpenSelectMeasurement(int iSelectItem)
{
	m_preIndex = iSelectItem;
	PostMessage(WM_COMMAND,MAKEWPARAM(ID_BUTTON_AO_MEASURE,BN_CLICKED),NULL);
}

LRESULT CCycVer::OnMySelect(WPARAM w, LPARAM )
{
	int index = w;

	g_iMPItemIndices[index];


	return 0;
}

LRESULT CCycVer::OnMyShow(WPARAM , LPARAM )
{
	ShowWindow(SW_SHOW);
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();

	return 0;

}

void CCycVer::OnIncludeGood()
{
	g_bIncludeGood = !g_bIncludeGood;
	m_cButtonIncludeGood.SetWindowText(g_bIncludeGood?"Exclude Good":"Include Good");
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CCycVer::OnIncludeSuspect()
{
	//	m_bIncludeSuspect = !m_bIncludeSuspect;
	g_bIncludeSuspect = g_bIncludeSuspect;
	m_cButtonIncludeSuspect.SetWindowText(g_bIncludeSuspect?"Exclude Suspect":"Include Suspect");
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CCycVer::OnAcceptGood()
{
	g_bAcceptGood = true;
	//	m_cButtonAcceptGood.SetWindowText(m_bAcceptGood?"Exclude Good":"Accept Good");
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
	g_bAcceptGood = false;
}

bool CCycVer::FindGraph(CString cTitle)
{
	bool bReturn = false;
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

	CObList  pDocList;
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	while(pos)
	{
		CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos); 
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
		CFDMSDoc* pFDMSDoc = (CFDMSDoc*)pDocList.GetNext(pos);
		pFDMSDoc->UpdateAllViews(NULL);
		if (pFDMSDoc->GetTitle() == cTitle)
			bReturn = true;
	}
	return bReturn;
}


void CCycVer::OnReturn()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->m_pDocTemplate4->CloseAllDocuments(FALSE);
	g_bPlot4 = FALSE;

	ShowWindow(SW_HIDE);
	::ShowWindow(g_hVerifyDlg,SW_SHOW);
}


BOOL CCycVer::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCycVer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect cRect;

	GetDlgItem(IDC_STATIC_1)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonMeasure.Create("Measure",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_AO_MEASURE);
	m_cButtonMeasure.SetFaceColor(CYAN,false);
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);

	/*GetDlgItem(IDC_STATIC_2)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonIncludeGood.Create("Include Good",
	WS_CHILD | WS_TABSTOP | WS_VISIBLE |
	BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
	cRect,this,ID_BUTTON_AO_IG);
	m_cButtonIncludeGood.SetFaceColor(YELLOW,false);
	GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_3)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonIncludeSuspect.Create("Include Suspect",
	WS_CHILD | WS_TABSTOP | WS_VISIBLE |
	BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
	cRect,this,ID_BUTTON_AO_IS);
	m_cButtonIncludeSuspect.SetFaceColor(YELLOW,false);
	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_4)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonAcceptGood.Create("Accept Good",
	WS_CHILD | WS_TABSTOP |WS_VISIBLE |
	BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
	cRect,this,ID_BUTTON_AO_AG);
	m_cButtonAcceptGood.SetFaceColor(YELLOW,false);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_HIDE);*/

	GetDlgItem(IDC_STATIC_5)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonReturn.Create("Return",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_AO_RETURN);
	m_cButtonReturn.SetFaceColor(CYAN,false);
	GetDlgItem(IDC_STATIC_5)->ShowWindow(SW_HIDE);


	CMenu* cMenu = GetSystemMenu(FALSE);
	if (cMenu)
		cMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
