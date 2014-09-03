// C:\dev\FDMS\fdmsNET\xlc\XCalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\FDMS.h"
#include ".\XCalDlg.h"


// XCalDlg dialog

IMPLEMENT_DYNAMIC(XCalDlg, CDialog)
XCalDlg::XCalDlg(LONG x, LONG y, const CString& title,  CWnd* pParent /*=NULL*/)
	: m_x(x), m_y(y), m_bChange(false), m_sTitle(title), CDialog(XCalDlg::IDD, pParent)
	, m_time(COleDateTime::GetCurrentTime())
{
}

XCalDlg::~XCalDlg()
{
}

void XCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER, m_date);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER, m_time);
}


BEGIN_MESSAGE_MAP(XCalDlg, CDialog)
	ON_WM_CREATE()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER, OnDtnDatetimechangeTimepicker)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER, OnDtnDatetimechangeDatepicker)
END_MESSAGE_MAP()


// XCalDlg message handlers

BOOL XCalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_sTitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int XCalDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->x = m_x;
	lpCreateStruct->y = m_y;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void XCalDlg::OnDtnDatetimechangeTimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_bChange = true;
	GetDlgItem(IDOK)->EnableWindow(true);
	*pResult = 0;
}

void XCalDlg::OnDtnDatetimechangeDatepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_bChange = true;
	GetDlgItem(IDOK)->EnableWindow(true);
	*pResult = 0;
}
