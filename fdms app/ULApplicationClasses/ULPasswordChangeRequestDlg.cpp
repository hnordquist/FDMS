// ULPasswordChangeRequestDlg.cpp : implementation file
// $History: ULPasswordChangeRequestDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULPasswordChangeRequestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULPasswordChangeRequestDlg dialog


ULPasswordChangeRequestDlg::ULPasswordChangeRequestDlg(CString Title, CString Password, 
													   int IconResID /*= 0*/,CWnd* pParent /*=NULL*/)
	: CDialog(ULPasswordChangeRequestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULPasswordChangeRequestDlg)
	m_EnteredPassword = _T("");
	//}}AFX_DATA_INIT
	m_Title = Title;
	m_IconID = IconResID;
	m_Password = Password;
}


void ULPasswordChangeRequestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULPasswordChangeRequestDlg)
	DDX_Text(pDX, IDC_SEB_PASSWORD_REQUEST_PASSWORD, m_EnteredPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULPasswordChangeRequestDlg, CDialog)
	//{{AFX_MSG_MAP(ULPasswordChangeRequestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULPasswordChangeRequestDlg message handlers
void ULPasswordChangeRequestDlg::OnOK() 
{
	//Get the data from the dialog box.
	UpdateData(true);

	//Error check the passwords.
	if(m_Password == m_EnteredPassword)
	{
		//Update the password in the file and exit.
		CDialog::OnOK();
	}
	else //passwords did not match.  Issue a warning to the user and force a retry.
	{
		((CStatic*)GetDlgItem(IDC_SEB_PASSWORD_REQUEST_MESSAGE))->SetWindowText("Invalid password.  Please enter the password to continue.");
	}
	
}

BOOL ULPasswordChangeRequestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
