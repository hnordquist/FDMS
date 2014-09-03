//
//  $History: Prompt.cpp $
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
//

#include "stdafx.h"
#include "Prompt.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrompt dialog


CPrompt::CPrompt(CWnd* pParent /*=NULL*/, bool Large, bool Cyrillic)
	: CDialog(CPrompt::IDD, pParent)
{
	m_bLarge = Large;
	m_bCyrillic = Cyrillic;
	m_bPassword = false;
	//{{AFX_DATA_INIT(CPrompt)
	m_EditString = _T("");
	m_StaticInstructions = _T("");
	//}}AFX_DATA_INIT
//	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));

}


void CPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrompt)
	DDX_Control(pDX, IDC_STATIC_INSTRUCTIONS, m_StaticInst);
	DDX_Control(pDX, IDC_EDIT, m_pEditString);
	DDX_Text(pDX, IDC_EDIT, m_EditString);
	DDX_Text(pDX, IDC_STATIC_INSTRUCTIONS, m_StaticInstructions);
	//}}AFX_DATA_MAP
//	DDV_MaxChars(pDX, m_EditString, 40);
}


BEGIN_MESSAGE_MAP(CPrompt, CDialog)
	//{{AFX_MSG_MAP(CPrompt)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON0, OnButton0)
	ON_BN_CLICKED(IDC_BUTTONSIGN, OnButtonsign)
	ON_BN_CLICKED(IDC_BUTTONCE, OnButtonce)
	ON_BN_CLICKED(IDC_BUTTONCA, OnButtonca)
	ON_BN_CLICKED(IDC_BUTTONDP, OnButtondp)
	ON_EN_CHANGE(IDC_EDIT, OnChangeEdit)
	ON_BN_CLICKED(IDACK,OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrompt message handlers

BOOL CPrompt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(m_Title);
	if (m_bPassword)
		((CEdit*)GetDlgItem(IDC_EDIT))->SetPasswordChar( '*' );
	if (!m_bLarge)
	{
		RECT rect;
		::GetWindowRect(m_hWnd,&rect);
		RECT rect2;
		GetDlgItem(IDC_BUTTON1)->GetWindowRect(&rect2);
		SetWindowPos(&wndTop,
			rect.left,
			rect.top,
			rect.right-rect.left,
			rect2.top-rect.top,
			SWP_FRAMECHANGED  );

	}
	m_pEditString.SetLimitText(63);
	GetDlgItem(IDOK)->EnableWindow(true);
	m_pEditString.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPrompt::OnButton1() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "1";
	SetDlgItemText(IDC_EDIT,text);
}

void CPrompt::OnButton2() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "2";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton3() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "3";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton4() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "4";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton5() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "5";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton6() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "6";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton7() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "7";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton8() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "8";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton9() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "9";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButton0() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	text += "0";
	SetDlgItemText(IDC_EDIT,text);
	
}

void CPrompt::OnButtonsign() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	if (text.IsEmpty())
	{
		text = "-";
		SetDlgItemText(IDC_EDIT,text);
	}
}

void CPrompt::OnButtonce() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	if (int length=text.GetLength())
	{
		text = text.Left(length-1);
		SetDlgItemText(IDC_EDIT,text);
	}
}

void CPrompt::OnButtonca() 
{
	
	SetDlgItemText(IDC_EDIT,"");	
}

void CPrompt::OnButtondp() 
{
	
	CString text;
	GetDlgItemText(IDC_EDIT,text);
	if (text.Find('.')==-1)
	{
		text += ".";
		SetDlgItemText(IDC_EDIT,text);
	}
}

BOOL CPrompt::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_F8)
			// || (pMsg->wParam ==VK_RETURN && (CWnd::GetFocus() == GetDlgItem(IDC_EDIT))))
		{
			if (GetDlgItem(IDOK)->IsWindowEnabled())
				OnOK();
			return TRUE;
		}
		if (pMsg->wParam == VK_F12)
		{
			OnCancel();
			return TRUE;
		}
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CPrompt::OnChangeEdit() 
{
	
	CString Text;
	GetDlgItemText(IDC_EDIT,Text);
	if (Text.GetLength()==0)
		GetDlgItem(IDOK)->EnableWindow(false);
	else
		GetDlgItem(IDOK)->EnableWindow(true);
}

void CPrompt::OnOK() 
{
	if (GetDlgItem(IDOK)->IsWindowEnabled())
		CDialog::OnOK();
}

void CPrompt::OnCancel() 
{

	CDialog::OnCancel();
}
