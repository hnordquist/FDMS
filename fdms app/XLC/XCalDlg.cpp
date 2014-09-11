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
// C:\dev\FDMS\fdmsNET\xlc\XCalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\FDMS.h"
#include ".\XCalDlg.h"


// XCalDlg dialog

IMPLEMENT_DYNAMIC(XCalDlg, CDialog)
XCalDlg::XCalDlg(LONG x, LONG y, const CString& title,  CWnd* pParent /*=NULL*/)
	: m_x(x), m_y(y), m_bChange(false), m_sTitle(title), CDialog(XCalDlg::IDD, pParent)
	, m_time(COleDateTime::GetCurrentTime())
{
}

XCalDlg::~XCalDlg()
{
}

void XCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER, m_date);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER, m_time);
}


BEGIN_MESSAGE_MAP(XCalDlg, CDialog)
	ON_WM_CREATE()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER, OnDtnDatetimechangeTimepicker)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER, OnDtnDatetimechangeDatepicker)
END_MESSAGE_MAP()


// XCalDlg message handlers

BOOL XCalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_sTitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int XCalDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->x = m_x;
	lpCreateStruct->y = m_y;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void XCalDlg::OnDtnDatetimechangeTimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_bChange = true;
	GetDlgItem(IDOK)->EnableWindow(true);
	*pResult = 0;
}

void XCalDlg::OnDtnDatetimechangeDatepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_bChange = true;
	GetDlgItem(IDOK)->EnableWindow(true);
	*pResult = 0;
}
