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
// Warning.cpp : implementation file
//

#include "stdafx.h"
//#include "uwnccdlg.h"
#include "Warning.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarning dialog


CWarning::CWarning(CWnd* pParent /*=NULL*/)
	: CDialog(CWarning::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarning)
	m_szMessage = _T("");
	//}}AFX_DATA_INIT
	m_cTitle = _T("");
//	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));
}


void CWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarning)
	DDX_Text(pDX, IDC_MESSAGE, m_szMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarning, CDialog)
	//{{AFX_MSG_MAP(CWarning)
	ON_BN_CLICKED(IDACK,OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarning message handlers

BOOL CWarning::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_cTitle.GetLength())
		SetWindowText(m_cTitle);
//	g_hWnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWarning::OnOK() 
{
	CDialog::OnOK();
}

BOOL CWarning::PreTranslateMessage(MSG* pMsg) 
{
//	if (m_hAccel != NULL)
//		if (::GetActiveWindow() == m_hWnd)
//		{
//			if (::TranslateAccelerator (m_hWnd, m_hAccel, pMsg))
//				return TRUE;
//		}
//		else
//			return FALSE;
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN)
		{
			return TRUE;
		}
//		if (pMsg->wParam == VK_F4)
//		{
//			OnButton1();
//			return TRUE;
//		}
		if (pMsg->wParam == VK_F8)
		{
			if (GetDlgItem(IDOK)->IsWindowEnabled())
				OnOK();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
