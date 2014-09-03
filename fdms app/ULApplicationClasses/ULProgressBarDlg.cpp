// ULProgressBarDlg.cpp : implementation file
// $History: ULProgressBarDlg.cpp $ 
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
#include "ULProgressBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULProgressBarDlg dialog


ULProgressBarDlg::ULProgressBarDlg(CString Title, CString Message, bool /*WithCancelBtn*/, 
								   int IconResID /* = 0*/,   CWnd* pParent /*=NULL*/)
								   : CDialog(ULProgressBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULProgressBarDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Message = Message;
	m_IconID = IconResID;
}


void ULProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULProgressBarDlg)
	DDX_Control(pDX, IDC_SEB_PROGRESS_BAR_BAR, m_ProgressBar);
	DDX_Control(pDX, IDC_SEB_PROGRESS_BAR_CANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULProgressBarDlg, CDialog)
	//{{AFX_MSG_MAP(ULProgressBarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULProgressBarDlg message handlers

BOOL ULProgressBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_Title);
	((CStatic*)GetDlgItem(IDC_SEB_PROGRESS_BAR_MESSAGE))->SetWindowText(m_Message);

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);


	//If this without the cancel button, then hide it.
	if(!m_WithCancelBtn)
		m_CancelBtn.ShowWindow(SW_HIDE );

	m_ProgressBar.SetScrollRange(0, 100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ULProgressBarDlg::ProgressBarSetValue(double PercentComplete)
{
	m_ProgressBar.SetScrollPos((int)PercentComplete);
}

void ULProgressBarDlg::SetMessage(CString Message)
{
	((CStatic*)GetDlgItem(IDC_SEB_PROGRESS_BAR_MESSAGE))->SetWindowText(Message);

}
