// MMMatchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FDMS.h"
#include "MMMatchDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MMMatchDlg dialog


MMMatchDlg::MMMatchDlg(CWnd* pParent)
	: CDialog(MMMatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MMMatchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = MMMatchDlg::IDD;
}

void MMMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MMMatchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MMMatchDlg, CDialog)
	//{{AFX_MSG_MAP(MMMatchDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MMMatchDlg message handlers

void MMMatchDlg::OnOK()
{

}

void MMMatchDlg::SetXText(LPCSTR ps)
{
	CEdit* e = (CEdit*) GetDlgItem(IDC_MMMATCH);
	e->SetWindowText(ps);
}


BOOL MMMatchDlg::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void MMMatchDlg::OnCancel()
{
	DestroyWindow();
}

void MMMatchDlg::PostNcDestroy()
{
	delete this;
}
