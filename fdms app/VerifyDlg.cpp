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
// $History: VerifyDlg.cpp $
// 
// *****************  Version 4  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
//

#include "stdafx.h"
#include "FDMS.h"
#include "VerifyDlg.h"
#include "testdrawView4.H"
#include "CycVer.h"
#include "FDMSDoc.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog
HWND g_hVerifyDlg;

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CVerifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVerifyDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerifyDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialog)
	//{{AFX_MSG_MAP(CVerifyDlg)
	ON_BN_CLICKED(ID_BUTTON_VER_CV,OnCycleVer)
	ON_BN_CLICKED(ID_BUTTON_VER_AV,OnAssemVer)
	ON_BN_CLICKED(ID_BUTTON_VER_IR,OnItemVer)
	ON_BN_CLICKED(ID_BUTTON_VER_RETURN,OnReturn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg message handlers
void CVerifyDlg::OnCycleVer()
{
	g_bPlotMostRecent = true;
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	ShowWindow(SW_HIDE);
	::PostMessage(g_hCycVer,MY_SHOW,0,0);
	if (!FindGraph(CString(TITLE4)))
	{
		//	pApp->m_pDocTemplate1->OpenDocumentFile(NULL);
		//	pApp->m_pDocTemplate2->OpenDocumentFile(NULL);
		//	pApp->m_pDocTemplate3->OpenDocumentFile(NULL);
		pApp->m_pDocTemplate4->OpenDocumentFile(NULL);
		//		m_cButtonNvsBU.SetFaceColor(GREEN);
		g_bPlot4 = TRUE;

		//		g_bPlot4 = FALSE;
	}
	//	else
	//	{
	//	//	pApp->m_pDocTemplate1->CloseAllDocuments(FALSE);
	//	//	pApp->m_pDocTemplate2->CloseAllDocuments(FALSE);
	//	//	pApp->m_pDocTemplate3->CloseAllDocuments(FALSE);
	//		pApp->m_pDocTemplate4->CloseAllDocuments(FALSE);
	////		m_cButtonNvsBU.SetFaceColor(RED);
	//		g_bPlot4 = FALSE;
	//	}
}

void CVerifyDlg::OnAssemVer()
{
	g_bPlotMostRecent = true;
	if (g_bPlot4)
	{
		g_bPlot4 = FALSE;
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->m_pDocTemplate4->CloseAllDocuments(FALSE);
	}
	ShowWindow(SW_HIDE);
	//	::ShowWindow(g_hAllOtherDlg,SW_SHOW);
	::PostMessage(g_hAllOtherDlg,MY_SHOW,0,0);
}
void CVerifyDlg::OnItemVer()
{
	g_bPlotMostRecent = true;
	if (g_bPlot4)
	{
		g_bPlot4 = FALSE;
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->m_pDocTemplate4->CloseAllDocuments(FALSE);
	}
}

bool CVerifyDlg::FindGraph(CString cTitle)
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
			if (pDocument)
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



void CVerifyDlg::OnReturn()
{
	if (g_bPlot4)
	{
		g_bPlot4 = false;
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->m_pDocTemplate4->CloseAllDocuments(FALSE);
	}

	ShowWindow(SW_HIDE);
	::ShowWindow(g_hMeasureDlg,SW_SHOW);
}

BOOL CVerifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect cRect;
	GetDlgItem(IDC_STATIC_1)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonCycleVerify.Create(_T("Cycle Verification"),
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_VER_CV);
	m_cButtonCycleVerify.SetFaceColor(RGB(0,255,255),false);
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_2)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonAssemblyVerify.Create(_T("Assembly Verification"),
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_VER_AV);
	m_cButtonAssemblyVerify.SetFaceColor(RGB(0,255,255),false);
	GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_3)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonItemReverify.Create(_T("Assembly Reverification"),
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_VER_IR);
	m_cButtonItemReverify.SetFaceColor(RGB(0,255,255),false);
	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_4)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonReturn.Create(_T("Return"),
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_VER_RETURN);
	m_cButtonReturn.SetFaceColor(RGB(0,255,255),false);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_HIDE);

	CMenu* cMenu = GetSystemMenu(FALSE);
	if (cMenu)
		cMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CVerifyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
