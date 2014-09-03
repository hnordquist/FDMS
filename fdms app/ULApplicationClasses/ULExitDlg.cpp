// ULExitDlg.cpp : implementation file
// $History: ULExitDlg.cpp $ 
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
#include "ULExitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULExitDlg dialog


ULExitDlg::ULExitDlg(CString AppName /* = "" */, CString ExitMsg/* = "" */,  int IconResID /*= 0*/, CWnd* pParent /*=NULL*/)
: CDialog(ULExitDlg::IDD, pParent), m_ButtonPushed (IDCANCEL)
{
	//{{AFX_DATA_INIT(ULExitDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_AppName = AppName;
	m_ExitMsg = ExitMsg;
	m_IconID = IconResID;
}


void ULExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULExitDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULExitDlg, CDialog)
	//{{AFX_MSG_MAP(ULExitDlg)
	ON_BN_CLICKED(IDC_EXIT_ALL_TOOLS_BTN, OnExitAllToolsBtn)
	ON_BN_CLICKED(IDC_EXIT_ONLY_THIS_TOOL_BTN, OnExitOnlyThisToolBtn)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULExitDlg message handlers

BOOL ULExitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	((CStatic*)GetDlgItem(IDC_EXIT_MESSAGE))->SetWindowText(m_ExitMsg);
	SetWindowText(m_AppName);

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ULExitDlg::OnExitAllToolsBtn()
{
	m_ButtonPushed = IDC_EXIT_ALL_TOOLS_BTN;	
	OnCancel();
}

void ULExitDlg::OnExitOnlyThisToolBtn()
{
	m_ButtonPushed = IDC_EXIT_ONLY_THIS_TOOL_BTN;		
	OnCancel();
}

int ULExitDlg::DoModal() 
{
	//int RetVal = 
	CDialog::DoModal();


	return m_ButtonPushed;
}
