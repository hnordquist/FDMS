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
�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
�	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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
// ULProductSupportDlg.cpp : implementation file
// $History: ULProductSupportDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULProductSupportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULProductSupportDlg dialog


ULProductSupportDlg::ULProductSupportDlg(CString AppName /* = "" */, CString ContactName/* = "" */, CString Email /* = "" */, int IconResID /* = 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(ULProductSupportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULProductSupportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ApplicationName = AppName;
	m_ContactName = ContactName;
	m_Email = Email;
	m_IconID = IconResID;
}


void ULProductSupportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULProductSupportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULProductSupportDlg, CDialog)
	//{{AFX_MSG_MAP(ULProductSupportDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULProductSupportDlg message handlers

BOOL ULProductSupportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Set the title of the dialog window.
	CString Title;
	Title.Format("%s - Product Support", m_ApplicationName);
	SetWindowText(Title); 

	//Set the text of the contact message information.
	CString ContactInfoStr;
	ContactInfoStr.Format("%s   (%s)", m_Email, m_ContactName);
	((CStatic*)GetDlgItem(IDC_SEB_PS_CONTACT))->SetWindowText(ContactInfoStr);

	//Set the icon for the dialog box.
	if(m_IconID != 0) 
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
