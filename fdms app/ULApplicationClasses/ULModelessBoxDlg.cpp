// ULModelessBoxDlg.cpp : implementation file
// $History: ULModelessBoxDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h" //REQT:  Change this to use the new application header.
#include "ULModelessBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULModelessBoxDlg dialog


ULModelessBoxDlg::ULModelessBoxDlg(CString Title, CString Message, int IconResID /* = 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(ULModelessBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULModelessBoxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Title = Title;
	m_Message = Message;
	m_IconID = IconResID;
}


void ULModelessBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULModelessBoxDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULModelessBoxDlg, CDialog)
	//{{AFX_MSG_MAP(ULModelessBoxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULModelessBoxDlg message handlers

BOOL ULModelessBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_Title);
	((CStatic*)GetDlgItem(IDC_SEB_MODELESS_BOX_MESSAGE))->SetWindowText(m_Message);

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
