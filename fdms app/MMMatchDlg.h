#pragma once
#include "afxwin.h"


// MMMatchDlg dialog

class MMMatchDlg : public CDialog
{
private:
	using CDialog::Create;

public:
	MMMatchDlg(CWnd* pParent);   // standard constructor
	BOOL Create();

	void SetXText(LPCSTR ps);

// Dialog Data
	//{{AFX_DATA(MMMatchDlg)
	enum { IDD = IDD_MMMATCHDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MMMatchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	CWnd* m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(MMMatchDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
