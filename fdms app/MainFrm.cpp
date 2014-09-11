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
// $History: MainFrm.cpp $
// 
// *****************  Version 10  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 9  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 8  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 7  *****************
// User: Longo        Date: 1/05/05    Time: 5:30p
// Updated in $/FDMS/FDMS Application/FDMSNET
// import button added to toolbar
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FDMS.h"

#include ".\mainfrm.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLCOMBOBOXID, OnMenuSelection)
	ON_COMMAND(ID_TOOLCOMBOBOXGO, OnGoButton)
	ON_COMMAND(ID_TOOLBAR_IMPORT, OnImportButton)
	ON_COMMAND(ID_TOOLS_IMPORT, OnImportButton)
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	bNeedsMenuUpdate = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_MeasureDlg.Create(IDD_DIALOG_MEASURE,this);
	m_MeasureDlg.ShowWindow(SW_SHOW);
	g_hMeasureDlg = m_MeasureDlg.m_hWnd;

	m_VerifyDlg.Create(IDD_DIALOG1,this);
	m_VerifyDlg.ShowWindow(SW_HIDE);
	g_hVerifyDlg = m_VerifyDlg.m_hWnd;

	m_AllOtherDlg.Create(IDD_DIALOG_ALLOTHER,this);
	m_AllOtherDlg.ShowWindow(SW_HIDE);
	g_hAllOtherDlg = m_AllOtherDlg.m_hWnd;

	m_CycVer.Create(IDD_DIALOG_CYCVER,this);
	m_CycVer.ShowWindow(SW_HIDE);
	g_hCycVer = m_CycVer.m_hWnd;

	m_wndToolBar.InitControlWithULInfo(this);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);

	if (!m_wndToolBar.m_bULToolsAvailable)
		m_wndToolBar.ShowWindow(SW_HIDE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_MAXIMIZE;
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	CMDIFrameWnd::OnUpdateFrameMenu(hMenuAlt);

/****
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	if (pApp->m_bMenusConstructed[0])
	{
		for (int mid = ID_EDIT_BACKGROUNDCYCLES; mid <= ID_EDIT_DETECTOR_ID; mid += MENU_SPAN)
		{
			CMenu* pMenu = pApp->GetPopUpForPref(this, mid);
			pApp->CheckCustomMenuItems(pMenu, mid);
		}
	}
****/

}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	if (!bMinimized && nState == WA_ACTIVE) 
	{
		if (bNeedsMenuUpdate)
		{
			CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
			pApp->CheckCustomMenuItems(this, false);
			bNeedsMenuUpdate = false;
		}
	}
}
void CMainFrame::OnUpdateImportButton(CCmdUI *pCmdUI)
{
    if (m_wndToolBar.m_bImportSourceAvailable)
		pCmdUI->Enable(); 
	else
		pCmdUI->Enable(FALSE);
}
void CMainFrame::OnGoButton()
{
	m_wndToolBar.StartTheTool();
}

void CMainFrame::OnImportButton()
{
	m_wndToolBar.ImportFromTool();
}
void CMainFrame::OnMenuSelection()
{
	// This application class doesn't do anything with button presses but a command handler
	// needs to be present for MFC to enable the buttons
}

