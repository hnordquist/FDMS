#pragma once


class CLegText : public CDialog
{
public:
	CLegText();
	~CLegText() {}
		// Dialog Data
	//{{AFX_DATA(CLegText)
	enum { IDD = IDD_ABOUT1 };
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLegText)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CLegText)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString clauses[4];


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMore();

};
