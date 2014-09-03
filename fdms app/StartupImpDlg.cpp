// StartupImpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FDMS.h"
#include "StartupImpDlg.h"


// StartupImpDlg dialog

IMPLEMENT_DYNAMIC(StartupImpDlg, CDialog)
StartupImpDlg::StartupImpDlg(CWnd* pParent /*=NULL*/)
	: mCur(0), mTot(0), CDialog(StartupImpDlg::IDD, pParent)
{
}

StartupImpDlg::~StartupImpDlg()
{
}

void StartupImpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void StartupImpDlg::DoIt(PTCHAR p)
{
	mTitle = p;
	this->Create(IDD_STARTUP_MSG_IMP_DLG, NULL);
	this->SetWindowText(p);
	this->SetDlgItemText(IDC_STATIC1, p);
	this->CenterWindow();
	this->ShowWindow(SW_SHOW);
	this->RedrawWindow();
}

void StartupImpDlg::BumpCount()
{
	mCur++;
	CString mt, f;
	f.Format(" %hi of %3.hi", mCur, mTot);
	mt = mTitle + f; 
	this->SetWindowText(mt.GetBuffer());
	this->SetDlgItemText(IDC_STATIC1, mt.GetBuffer());
	this->RedrawWindow();
}

BEGIN_MESSAGE_MAP(StartupImpDlg, CDialog)
END_MESSAGE_MAP()


// StartupImpDlg message handlers
