// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "FDMS.h"

#include "ChildFrm.h"
#include "FDMSView.h"
#include ".\childfrm.h"
#include ".\askoncebox.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_RESETACKNOWLEDGEMENTS, OnEditResetacknowledgements)
	ON_COMMAND(ID_EDIT_CONFIG_O_RAMA_C, OnEditConfigDUMP_O_RAMA_Current)
	ON_COMMAND(ID_EDIT_CONFIG_O_RAMA_D, OnEditConfigDUMP_O_RAMA_Default)
	ON_COMMAND_RANGE(ID_EDIT_BACKGROUNDCYCLES, ID_EDIT_BACKGROUNDCYCLES + (MENU_SPAN -1),  OnEditPrefBGC)
	ON_COMMAND_RANGE(ID_EDIT_BACKGROUNDCYCLETIME, ID_EDIT_BACKGROUNDCYCLETIME  + (MENU_SPAN -1),  OnEditPrefBGCT)
	ON_COMMAND_RANGE(ID_EDIT_DELTAENRICHMENT, ID_EDIT_DELTAENRICHMENT  + (MENU_SPAN -1),  OnEditPrefDE)
	ON_COMMAND_RANGE(ID_EDIT_DELTACOOLINGTIME, ID_EDIT_DELTACOOLINGTIME  + (MENU_SPAN -1),  OnEditPrefDCT)
	ON_COMMAND_RANGE(ID_EDIT_STANDARDMEASUREMENTCYCLES, ID_EDIT_STANDARDMEASUREMENTCYCLES  + (MENU_SPAN -1),  OnEditPrefSMC)
	ON_COMMAND_RANGE(ID_EDIT_STANDARDMEASUREMENTCYCLETIME, ID_EDIT_STANDARDMEASUREMENTCYCLETIME  + (MENU_SPAN -1),  OnEditPrefSMCT)
	ON_COMMAND_RANGE(ID_EDIT_DETECTOR_ID, ID_EDIT_DETECTOR_ID  + (MENU_SPAN -1),  OnEditPrefDID)
	ON_COMMAND_RANGE(ID_TOOLMENUID, ID_TOOLMENUID + 10,  OnToolGoose)
	ON_COMMAND(ID_TOOLS_IMPORT, OnImportButton)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROCESSINGLOG, OnUpdateImportButton)

END_MESSAGE_MAP()

void CChildFrame::OnUpdateImportButton(CCmdUI *pCmdUI)
{
//	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
  //  if ((CMainFrame*)(pApp->m_pMainWnd))->m_wndToolBar.m_bImportSourceAvailable)
	//	pCmdUI->Enable(); 
	//else
	//	pCmdUI->Enable(FALSE);
}

void CChildFrame::OnUpdateMenuItems(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(0);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{

}

CChildFrame::~CChildFrame()
{
}



void CChildFrame::OnEditPrefDCT(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_DELTACOOLINGTIME);
	pApp->NotifyChildChangedPrefs();
}

void CChildFrame::OnEditPrefDE(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_DELTAENRICHMENT);
	pApp->NotifyChildChangedPrefs();

}

void CChildFrame::OnEditPrefSMC(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_STANDARDMEASUREMENTCYCLES);
	pApp->NotifyChildChangedPrefs();
}

void CChildFrame::OnEditPrefSMCT(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_STANDARDMEASUREMENTCYCLETIME);
	pApp->NotifyChildChangedPrefs();
}

void CChildFrame::OnEditPrefBGC(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_BACKGROUNDCYCLES);
	pApp->NotifyChildChangedPrefs();
}
void CChildFrame::OnEditPrefBGCT(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_BACKGROUNDCYCLETIME);
	pApp->NotifyChildChangedPrefs();
}
void CChildFrame::OnEditPrefDID(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMenu* pMenu = pApp->GetPopUpForPref(this->GetMDIFrame(), nID);
	pApp->SetTemporaryIniValue(pMenu, nID, ID_EDIT_DETECTOR_ID);
	pApp->NotifyChildChangedPrefs();
}

void CChildFrame::OnToolGoose(UINT nID)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->OnToolGoose(nID);
}

void CChildFrame::OnImportButton()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->OnImportButton();
}

void CChildFrame::OnEditConfigDUMP_O_RAMA_Current()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->SaveFullConfig(false);

}
void CChildFrame::OnEditConfigDUMP_O_RAMA_Default()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->SaveFullConfig(true);
}


void CChildFrame::OnEditResetacknowledgements()
{
	CAskOnceBox::ResetAllAskOnceVlaues();
}



BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = //  WS_MINIMIZEBOX  | WS_MAXIMIZEBOX |WS_CAPTION | WS_SYSMENU  | FWS_ADDTOTITLE
				WS_MAXIMIZE | WS_VISIBLE |	WS_OVERLAPPEDWINDOW | 	WS_CHILD;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnClose() 
{
}


void CChildFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	CMenu* cMenu = GetSystemMenu(FALSE);
	if (cMenu)
		cMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}


void CChildFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	CMDIChildWnd::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);
	//CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	//bNeedsMenuUpdate = pApp->bNeedsMenuUpdate();
}




