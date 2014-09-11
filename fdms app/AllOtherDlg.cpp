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
// $History: AllOtherDlg.cpp $
// 
// *****************  Version 8  *****************
// User: 186846       Date: 2/26/08    Time: 8:55a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/18/05    Time: 9:43a
// Created in $/FDMS/FDMS Application/FDMSNET/FDMSNET
// new list ctrl code
// 
// *****************  Version 5  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
//

#include "stdafx.h"
#include "FDMS.h"
#include "AllOtherDlg.h"
#include "fdmsDoc.h"
#include "testdrawview1.h"
#include "testdrawview2.h"
#include "testdrawview3.h"
#include "selectmeasurement.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAllOtherDlg dialog
HWND g_hAllOtherDlg;

CAllOtherDlg::CAllOtherDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAllOtherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllOtherDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIncludeGood = false;
	m_bIncludeSuspect = false;
	m_preIndex = -1;
}


void CAllOtherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllOtherDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllOtherDlg, CDialog)
	//{{AFX_MSG_MAP(CAllOtherDlg)
	ON_BN_CLICKED(ID_BUTTON_AO_MEASURE,OnMeasure)
	ON_BN_CLICKED(ID_BUTTON_AO_IG,OnIncludeGood)
	ON_BN_CLICKED(ID_BUTTON_AO_IS,OnIncludeSuspect)
	ON_BN_CLICKED(ID_BUTTON_AO_AG,OnAcceptGood)
	ON_BN_CLICKED(ID_BUTTON_AO_NB,OnNeutronVsBU)
	ON_BN_CLICKED(ID_BUTTON_AO_GBUT,OnGammaBUVsTime)
	ON_BN_CLICKED(ID_BUTTON_AO_NG,OnNeutronVsGamma)
	ON_BN_CLICKED(ID_BUTTON_AO_RETURN,OnReturn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MY_SHOW,OnMyShow)
	ON_MESSAGE(MY_SELECT,OnMySelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllOtherDlg message handlers
void CAllOtherDlg::OnMeasure()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	if (pApp->GetMeasurementDialogPtr())
		return;

	ShowWindow(SW_HIDE);
	CSelectMeasurement cSelectMeasurement(this);
	cSelectMeasurement.m_iSubsetSelector = AssemblyVerification;
	cSelectMeasurement.m_preIndex = m_preIndex;
	cSelectMeasurement.DoModal();
	if (::IsWindow(m_hWnd))
		ShowWindow(SW_SHOW);
	m_preIndex = -1;
}

void CAllOtherDlg::OpenSelectMeasurement(int iSelectItem)
{
	m_preIndex = iSelectItem;
	PostMessage(WM_COMMAND,MAKEWPARAM(ID_BUTTON_AO_MEASURE,BN_CLICKED),NULL);
}


LRESULT CAllOtherDlg::OnMySelect(WPARAM w, LPARAM )
{
	int index = w;

	g_iMPItemIndices[index];

	return 0;
}

LRESULT CAllOtherDlg::OnMyShow(WPARAM , LPARAM )
{
	ShowWindow(SW_SHOW);
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

	if (m_cButtonNvsBU.GetFaceColor() == GREEN)
	{
		pApp->m_pDocTemplate1->OpenDocumentFile(NULL);
		g_bPlot1 = TRUE;
	}
	if (m_cButtonGBUvsTime.GetFaceColor() == GREEN)
	{
		pApp->m_pDocTemplate2->OpenDocumentFile(NULL);
		g_bPlot2 = TRUE;
	}
	if (m_cButtonNvsG.GetFaceColor() == GREEN)
	{
		pApp->m_pDocTemplate3->OpenDocumentFile(NULL);
		g_bPlot3 = TRUE;
	}
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
	return 0;
}

void CAllOtherDlg::OnIncludeGood()
{
	m_bIncludeGood = !m_bIncludeGood;
	g_bIncludeGood = m_bIncludeGood;
	m_cButtonIncludeGood.SetWindowText(m_bIncludeGood?"Exclude Good":"Include Good");
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CAllOtherDlg::OnIncludeSuspect()
{
	m_bIncludeSuspect = !m_bIncludeSuspect;
	g_bIncludeSuspect = m_bIncludeSuspect;
	m_cButtonIncludeSuspect.SetWindowText(m_bIncludeSuspect?"Exclude Suspect":"Include Suspect");
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CAllOtherDlg::OnAcceptGood()
{
	g_bAcceptGood = true;
	//	m_cButtonAcceptGood.SetWindowText(m_bAcceptGood?"Exclude Good":"Accept Good");
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
	g_bAcceptGood = false;
}

bool CAllOtherDlg::FindGraph(CString cTitle)
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
			CDocument* pDocument= pTemplate->GetNextDoc(pos2);
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

void CAllOtherDlg::OnNeutronVsBU()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	if (!FindGraph(CString(TITLE1)))
	{
		pApp->m_pDocTemplate1->OpenDocumentFile(NULL);
		m_cButtonNvsBU.SetFaceColor(GREEN);
		g_bPlot1 = TRUE;

	}
	else
	{
		pApp->m_pDocTemplate1->CloseAllDocuments(FALSE);
		m_cButtonNvsBU.SetFaceColor(RED);
		g_bPlot1 = FALSE;
	}
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CAllOtherDlg::OnGammaBUVsTime()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	if (!FindGraph(CString(TITLE2)))
	{
		pApp->m_pDocTemplate2->OpenDocumentFile(NULL);
		m_cButtonGBUvsTime.SetFaceColor(GREEN);
		g_bPlot2 = TRUE;
	}
	else
	{ 
		pApp->m_pDocTemplate2->CloseAllDocuments(FALSE);
		m_cButtonGBUvsTime.SetFaceColor(RED);
		g_bPlot2 = FALSE;
	}
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CAllOtherDlg::OnNeutronVsGamma()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	if (!FindGraph(CString(TITLE3)))
	{
		pApp->m_pDocTemplate3->OpenDocumentFile(NULL);
		m_cButtonNvsG.SetFaceColor(GREEN);
		g_bPlot3 = TRUE;
	}
	else
	{
		pApp->m_pDocTemplate3->CloseAllDocuments(FALSE);
		m_cButtonNvsG.SetFaceColor(RED);
		g_bPlot3 = FALSE;
	}
	pApp->UpdateGraphData();
	pApp->UpdateAllDocs();
}

void CAllOtherDlg::OnReturn()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->m_pDocTemplate1->CloseAllDocuments(FALSE);
	pApp->m_pDocTemplate2->CloseAllDocuments(FALSE);
	pApp->m_pDocTemplate3->CloseAllDocuments(FALSE);
	//	m_cButtonNvsBU.SetFaceColor(RED);
	//	m_cButtonGBUvsTime.SetFaceColor(RED);
	//	m_cButtonNvsG.SetFaceColor(RED);
	g_bPlot1 = FALSE;
	g_bPlot2 = FALSE;
	g_bPlot3 = FALSE;

	ShowWindow(SW_HIDE);
	//	::ShowWindow(g_hMeasureDlg,SW_SHOW);
	//	::ShowWindow(g_hAllOtherDlg,SW_SHOW);
	::ShowWindow(g_hVerifyDlg,SW_SHOW);
}

BOOL CAllOtherDlg::OnInitDialog() 
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
	WS_CHILD | WS_TABSTOP | WS_VISIBLE |
	BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
	cRect,this,ID_BUTTON_AO_AG);
	m_cButtonAcceptGood.SetFaceColor(YELLOW,false);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_HIDE);*/

	GetDlgItem(IDC_STATIC_5)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonNvsBU.Create("Neutron v. Burnup",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_AO_NB);
	m_cButtonNvsBU.SetFaceColor(RED,false);
	GetDlgItem(IDC_STATIC_5)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_6)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonGBUvsTime.Create("Gamma/BU v. Time",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_AO_GBUT);
	m_cButtonGBUvsTime.SetFaceColor(RED,false);
	GetDlgItem(IDC_STATIC_6)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_7)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonNvsG.Create("Neutron v. Gamma",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_AO_NG);
	m_cButtonNvsG.SetFaceColor(RED,false);
	GetDlgItem(IDC_STATIC_7)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_8)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonReturn.Create("Return",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_AO_RETURN);
	m_cButtonReturn.SetFaceColor(CYAN,false);
	GetDlgItem(IDC_STATIC_8)->ShowWindow(SW_HIDE);


	CMenu* cMenu = GetSystemMenu(FALSE);
	if (cMenu)
		cMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAllOtherDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
