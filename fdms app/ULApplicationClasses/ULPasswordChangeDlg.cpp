// ULPasswordChangeDlg.cpp : implementation file
// $History: ULPasswordChangeDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULPasswordChangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULPasswordChangeDlg dialog


ULPasswordChangeDlg::ULPasswordChangeDlg(CString Title, CString Password1, CString Password2, 
										 int IconResID /* =0 */, CWnd* pParent /*=NULL*/)
	: CDialog(ULPasswordChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULPasswordChangeDlg)
	m_Password1 = Password1; //_T("");
	m_Password2 = Password2; //_T("");
	//}}AFX_DATA_INIT
	m_Title = Title;
	m_IconID  = IconResID;

}


void ULPasswordChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULPasswordChangeDlg)
	DDX_Text(pDX, IDC_SEB_PASSWORD_CHANGE_PASSWORD1, m_Password1);
	DDX_Text(pDX, IDC_SEB_PASSWORD_CHANGE_PASSWORD2, m_Password2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULPasswordChangeDlg, CDialog)
	//{{AFX_MSG_MAP(ULPasswordChangeDlg)
//	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULPasswordChangeDlg message handlers

bool ULPasswordChangeDlg::DoThePasswordsMatch(CString Password1, CString Password2)
{
	return Password1 == Password2;
}

void ULPasswordChangeDlg::OnOK() 
{
	//Get the data from the dialog box.
	UpdateData(true);

	//Error check the passwords.
	if(m_Password1 == m_Password2)
	{
		//Update the password in the file and exit.
		CDialog::OnOK();
	}
	else //passwords did not match.  Issue a warning to the user and force a retry.
	{
		((CStatic*)GetDlgItem(IDC_SEB_PASSWORD_CHANGE_MESSAGE))->SetWindowText("Passwords do not match.  Please re-enter the password twice.");
	}
	
}

BOOL ULPasswordChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	SetWindowText(m_Title);
	
	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int ULPasswordChangeDlg::DoModal(CString& NewPassword) 
{
	
	int RetVal = CDialog::DoModal();
	UpdateData(true);

	if(RetVal == IDOK)
		NewPassword = m_Password1;
	
	return RetVal;

}

