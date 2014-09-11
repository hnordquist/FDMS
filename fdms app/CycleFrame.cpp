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
