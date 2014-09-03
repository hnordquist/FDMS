// ULStartToolMsgDlg.cpp : implementation file
// $History: ULStartToolMsgDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULStartToolMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULStartToolMsgDlg dialog


ULStartToolMsgDlg::ULStartToolMsgDlg(CString Title, CString Message, int IconResID /*= 0*/,CWnd* pParent /*=NULL*/)
	: CDialog(ULStartToolMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULStartToolMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Message = Message;
	m_Title = Title;
	m_IconID = IconResID;
}


void ULStartToolMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULStartToolMsgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULStartToolMsgDlg, CDialog)
	//{{AFX_MSG_MAP(ULStartToolMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULStartToolMsgDlg message handlers

BOOL ULStartToolMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CStatic*)GetDlgItem(IDC_RT_START_TOOL_MESSAGE))->SetWindowText(m_Message);

	//Set the title of the dialog window.
	SetWindowText(m_Title);

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
