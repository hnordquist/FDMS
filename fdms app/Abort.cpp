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
// Abort.cpp : implementation file
//

#include "stdafx.h"
#include ".\fdms.h"
#include ".\abort.h"
#include ".\doassay.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbort dialog


CAbort::CAbort(CWnd* pParent /*=NULL*/)
	: CDialog(CAbort::IDD, pParent)
{
	m_cTitle = _T("");
	//{{AFX_DATA_INIT(CAbort)
	m_cInstructions = _T("Are you sure you want to stop or cancel this assay?");

	//}}AFX_DATA_INIT
//	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));
}

CAbort::~CAbort()
{

}

void CAbort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbort)
	DDX_Text(pDX, IDC_STATIC_TEXT, m_cInstructions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbort, CDialog)
	//{{AFX_MSG_MAP(CAbort)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_ABORTOK, OnAbortRetain)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbort message handlers

#define TIMER_GETSTATE 62123

void CAbort::SetControlStates()
{
	if (bLastState)
	{
		pAbortItem->EnableWindow(TRUE);
		pCancelItem->EnableWindow(TRUE);
		pCancelItem->SetWindowText("       Abort.   \r\n    DISCARD any results\r\n        F8");

	}
	else
	{
		pAbortItem->EnableWindow(FALSE);
		pCancelItem->SetWindowText("      Stop.  \r\n    \r\n        F8");
	}

}

BOOL CAbort::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_cTitle.IsEmpty())
		SetWindowText(m_cTitle);

	pAbortItem = GetDlgItem(IDC_ABORTOK);
	pCancelItem = GetDlgItem(IDOK);

	CDoAssay* pWnd = (CDoAssay*)GetParent();
	bLastState = pWnd->bHasAbortableStatus();
	SetControlStates();


	SetTimer(TIMER_GETSTATE,400,0);

	return TRUE; 
}

void CAbort::OnAbortRetain() 
{
	result = AbortButKeepWhatWeHave;
	EndDialog(AbortButKeepWhatWeHave);
}

BOOL CAbort::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_F8)
		{
			if (GetDlgItem(IDOK)->IsWindowEnabled())
			{
				result = AbortMeasurement;
				OnOK();
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_F9)
		{
			OnAbortRetain();
			return TRUE;
		}
		if (pMsg->wParam == VK_F12)
		{
			CDialog::OnCancel();
			result = ContinueOnward;	

			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}



void CAbort::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == TIMER_GETSTATE)
	{
		CDoAssay* pWnd = (CDoAssay*)GetParent();
		bool bThisState= pWnd->bHasAbortableStatus();
		if (bLastState != bThisState)
		{
			bLastState = bThisState;
			SetControlStates();
		}
	}
	CDialog::OnTimer(nIDEvent);
}
