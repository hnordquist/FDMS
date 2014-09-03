// Warning.cpp : implementation file
//

#include "stdafx.h"
//#include "uwnccdlg.h"
#include "Warning.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarning dialog


CWarning::CWarning(CWnd* pParent /*=NULL*/)
	: CDialog(CWarning::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarning)
	m_szMessage = _T("");
	//}}AFX_DATA_INIT
	m_cTitle = _T("");
//	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));
}


void CWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarning)
	DDX_Text(pDX, IDC_MESSAGE, m_szMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarning, CDialog)
	//{{AFX_MSG_MAP(CWarning)
	ON_BN_CLICKED(IDACK,OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarning message handlers

BOOL CWarning::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_cTitle.GetLength())
		SetWindowText(m_cTitle);
//	g_hWnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWarning::OnOK() 
{
	CDialog::OnOK();
}

BOOL CWarning::PreTranslateMessage(MSG* pMsg) 
{
//	if (m_hAccel != NULL)
//		if (::GetActiveWindow() == m_hWnd)
//		{
//			if (::TranslateAccelerator (m_hWnd, m_hAccel, pMsg))
//				return TRUE;
//		}
//		else
//			return FALSE;
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN)
		{
			return TRUE;
		}
//		if (pMsg->wParam == VK_F4)
//		{
//			OnButton1();
//			return TRUE;
//		}
		if (pMsg->wParam == VK_F8)
		{
			if (GetDlgItem(IDOK)->IsWindowEnabled())
				OnOK();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
