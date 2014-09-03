// AcceptReject.cpp : implementation file
//

#include "stdafx.h"
#include "fdms.h"
#include "AcceptReject.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcceptReject dialog


CAcceptReject::CAcceptReject(CWnd* pParent /*=NULL*/)
	: CDialog(CAcceptReject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcceptReject)

	m_cLabelNew = _T("");
	m_cLabelPrev = _T("");
	m_cLabelTitle = _T("");
	for (int cht = NeutA; cht < MaxChannels; cht++)
	{
		m_cLabel[cht] = _T("");
		m_cNew[cht] = _T("");
		m_cPrev[cht] = _T("");
	}
	//}}AFX_DATA_INIT
}


void CAcceptReject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcceptReject)
	DDX_Text(pDX, IDC_STATIC_LABEL1, m_cLabel[NeutA]);
	DDX_Text(pDX, IDC_STATIC_LABEL2, m_cLabel[NeutB]);
	DDX_Text(pDX, IDC_STATIC_LABEL3, m_cLabel[NeutC]);
	DDX_Text(pDX, IDC_STATIC_LABEL4, m_cLabel[Gamma1]);
	DDX_Text(pDX, IDC_STATIC_LABEL5, m_cLabel[Gamma2]);
	DDX_Text(pDX, IDC_STATIC_LABELNEW, m_cLabelNew);
	DDX_Text(pDX, IDC_STATIC_LABELPREV, m_cLabelPrev);
	DDX_Text(pDX, IDC_STATIC_NEW1, m_cNew[NeutA]);
	DDX_Text(pDX, IDC_STATIC_NEW2, m_cNew[NeutB]);
	DDX_Text(pDX, IDC_STATIC_NEW3, m_cNew[NeutC]);
	DDX_Text(pDX, IDC_STATIC_NEW4, m_cNew[Gamma1]);
	DDX_Text(pDX, IDC_STATIC_NEW5, m_cNew[Gamma2]);
	DDX_Text(pDX, IDC_STATIC_PREV1, m_cPrev[NeutA]);
	DDX_Text(pDX, IDC_STATIC_PREV2, m_cPrev[NeutB]);
	DDX_Text(pDX, IDC_STATIC_PREV3, m_cPrev[NeutC]);
	DDX_Text(pDX, IDC_STATIC_PREV4, m_cPrev[Gamma1]);
	DDX_Text(pDX, IDC_STATIC_PREV5, m_cPrev[Gamma2]);
	DDX_Text(pDX, IDC_STATIC_ML, m_cLabelTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcceptReject, CDialog)
	//{{AFX_MSG_MAP(CAcceptReject)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcceptReject message handlers

BOOL CAcceptReject::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_cDialogLabel.GetLength())
		SetWindowText(m_cDialogLabel);
	return TRUE; 
}

void CAcceptReject::OnCancel() 
{
	CDialog::OnCancel();
}

void CAcceptReject::OnBnClickedOk()
{
	OnOK();
}
