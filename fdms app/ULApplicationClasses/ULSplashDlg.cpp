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
// ULSplashDlg.cpp : implementation file
// $History: ULSplashDlg.cpp $ 
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULSplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULSplashDlg dialog


ULSplashDlg::ULSplashDlg(CString Version, CString ProgrammerEmail, UINT DisplayDurationSecs, int IconResID/* = 0*/,
						 CWnd* pParent /*=NULL*/)
						 : CDialog(ULSplashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULSplashDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Version = Version;
	m_ProgrammerEmail = ProgrammerEmail;
	m_IconID = IconResID;
	m_DisplayDurationSecs = DisplayDurationSecs;
}


void ULSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULSplashDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULSplashDlg, CDialog)
	//{{AFX_MSG_MAP(ULSplashDlg)
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULSplashDlg message handlers

void ULSplashDlg::OnTimer(UINT ) 
{
	static UINT StartTime = 0;
	if(StartTime == 0)
	{
		StartTime = GetTickCount();

	}

	// Destroy the window, and update the mainframe.
	// Destroy the splash screen window.
	UINT CurrentTime = GetTickCount();
	if(CurrentTime - StartTime > (m_DisplayDurationSecs * 1000))
	{
		KillTimer(1);
		HideSplashScreen();
	}
	else
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
		CenterWindow();
		UpdateWindow();
	}

}

BOOL ULSplashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);


	// Center the window.
	CenterWindow();
	SetForegroundWindow();

	// Set a timer to destroy the splash screen.
	//UINT RetVal = 
	SetTimer(1, m_DisplayDurationSecs * 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


int ULSplashDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
	CenterWindow();

	return 0;
}

void ULSplashDlg::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	//	EndDialog(IDOK);
	DestroyWindow();

	AfxGetMainWnd()->Invalidate(true); //blackout
}




