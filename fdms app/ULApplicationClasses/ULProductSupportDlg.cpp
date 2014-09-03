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
