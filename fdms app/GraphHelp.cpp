// GraphHelp.cpp : implementation file
//

#include "stdafx.h"
//#include "testdraw.h"
#include "FDMS.h"
//#include "msfp.H"
#include "GraphHelp.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphHelp dialog


CGraphHelp::CGraphHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphHelp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraphHelp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CGraphHelp::ToggleDisplay()
{
	ShowWindow(SW_SHOW);
}

void CGraphHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphHelp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphHelp, CDialog)
	//{{AFX_MSG_MAP(CGraphHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphHelp message handlers

BOOL CGraphHelp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	ShowWindow(SW_HIDE);
//	m_bVisible = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
