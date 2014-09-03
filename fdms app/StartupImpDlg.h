#pragma once


// StartupImpDlg dialog

class StartupImpDlg : public CDialog
{
	DECLARE_DYNAMIC(StartupImpDlg)

public:
	StartupImpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~StartupImpDlg();

	void DoIt(PTCHAR p);

	void BumpCount();

	void SetCount(UINT16 n) {mTot = n;}

// Dialog Data
	enum { IDD = IDD_STARTUP_MSG_IMP_DLG };

protected:

	CString mTitle;
	UINT16 mCur, mTot;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
