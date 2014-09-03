#include "stdafx.h"

#include "FDMS.h"
#include ".\cycleframe.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCycleFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CCycleFrame, CChildFrame)
	//{{AFX_MSG_MAP(CCycleFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CCycleFrame::CCycleFrame(void)
{
}

CCycleFrame::~CCycleFrame(void)
{
}

bool CCycleFrame::m_bMenuEnhanced = false;

void CCycleFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	CChildFrame::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);

	if (!m_bMenuEnhanced)
	{
		TRACE("CCycleFrame::OnUpdateFrameMenu");
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->AddUserInfoToMenus(	this->GetMDIFrame(), eCycleFrame );
		pApp->CreateToolMenu(GetMDIFrame());
		m_bMenuEnhanced = true;
	}
	else if (pActiveWnd && bActive)
	{
			CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
			pApp->CheckCustomMenuItems(this->GetMDIFrame(), true);
	}
}


///////////////////



IMPLEMENT_DYNCREATE(CGBUvTFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CGBUvTFrame, CChildFrame)
	//{{AFX_MSG_MAP(CGBUvTFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CGBUvTFrame::CGBUvTFrame(void)
{
}

CGBUvTFrame::~CGBUvTFrame(void)
{
}

bool CGBUvTFrame::m_bMenuEnhanced = false;

void CGBUvTFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	CChildFrame::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);

	if (!m_bMenuEnhanced)
	{
		TRACE("CGBUvTFrame::OnUpdateFrameMenu");
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->AddUserInfoToMenus(	this->GetMDIFrame(), eGBUvTCycle );
		pApp->CreateToolMenu(GetMDIFrame());
		m_bMenuEnhanced = true;
	}
	else if (pActiveWnd && bActive)
	{
			CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
			pApp->CheckCustomMenuItems(this->GetMDIFrame(), true);

	}
}
////////////////////// CNvBUFrame
IMPLEMENT_DYNCREATE(CNvBUFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CNvBUFrame, CChildFrame)
	//{{AFX_MSG_MAP(CNvBUFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CNvBUFrame::CNvBUFrame(void)
{
}

CNvBUFrame::~CNvBUFrame(void)
{
}

bool CNvBUFrame::m_bMenuEnhanced = false;

void CNvBUFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	CChildFrame::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);

	if (!m_bMenuEnhanced)
	{
		TRACE("CNvBUFrame::OnUpdateFrameMenu");
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->AddUserInfoToMenus(	GetMDIFrame(), eNvBUCycle );
		pApp->CreateToolMenu(GetMDIFrame());
		m_bMenuEnhanced = true;
	}
	else if (pActiveWnd && bActive)
	{
			CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
			pApp->CheckCustomMenuItems(this->GetMDIFrame(), true);
	}
}
//////////////////// CNvGFrame
IMPLEMENT_DYNCREATE(CNvGFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CNvGFrame, CChildFrame)
	//{{AFX_MSG_MAP(CNvGFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CNvGFrame::CNvGFrame(void)
{
}

CNvGFrame::~CNvGFrame(void)
{
}

bool CNvGFrame::m_bMenuEnhanced = false;

void CNvGFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	CChildFrame::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);

	if (!m_bMenuEnhanced)
	{
		TRACE("CChildFrame::OnUpdateFrameMenu");
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->AddUserInfoToMenus(	this->GetMDIFrame(), eNvGFCycle );
		pApp->CreateToolMenu(GetMDIFrame());
		m_bMenuEnhanced = true;
	}
	else if (pActiveWnd && bActive)
	{
			CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
			pApp->CheckCustomMenuItems(this->GetMDIFrame(), true);
	}
}
