#pragma once

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	static bool m_bDoneGot;
	static CString m_sVer;
	static CString m_sTime;
	static BOOL GetAppInfo();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static BOOL GetLastWriteTime(PTCHAR pszFullPath, CString& lpszString);
public:
	virtual BOOL OnInitDialog();
};