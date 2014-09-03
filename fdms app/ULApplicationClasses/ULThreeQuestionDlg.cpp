// ULThreeQuestionDlg.cpp : implementation file
// $History: ULThreeQuestionDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULThreeQuestionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULThreeQuestionDlg dialog


ULThreeQuestionDlg::ULThreeQuestionDlg(CString Title, CString Message, CString Button1Txt, CString Button2Txt, CString Button3Txt, 
		int IconResID /*= 0,*/, CWnd* pParent /*=NULL*/)
	: CDialog(ULThreeQuestionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULThreeQuestionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Title = Title;
	m_Message = Message;
	m_Button1Txt = Button1Txt;
	m_Button2Txt = Button2Txt;
	m_Button3Txt = Button3Txt;
	m_IconID = IconResID;
}


void ULThreeQuestionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULThreeQuestionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULThreeQuestionDlg, CDialog)
	//{{AFX_MSG_MAP(ULThreeQuestionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULThreeQuestionDlg message handlers

BOOL ULThreeQuestionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_Title);
	((CStatic*)GetDlgItem(IDC_SEB_THREE_BOX_MESSAGE))->SetWindowText(m_Message);
	((CButton*)GetDlgItem(IDC_SEB_THREE_BOX_BUTTON_1))->SetWindowText(m_Button1Txt);
	((CButton*)GetDlgItem(IDC_SEB_THREE_BOX_BUTTON_2))->SetWindowText(m_Button2Txt);
	((CButton*)GetDlgItem(IDC_SEB_THREE_BOX_BUTTON_3))->SetWindowText(m_Button3Txt);

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ULThreeQuestionDlg::DisableAllButtons()
{
	((CButton*)GetDlgItem(IDC_SEB_THREE_BOX_BUTTON_1))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_SEB_THREE_BOX_BUTTON_2))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_SEB_THREE_BOX_BUTTON_3))->EnableWindow(false);

}
