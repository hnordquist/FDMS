#if !defined(AFX_ABORT_H__8AA34D02_732D_11D2_B42B_006008A70A6A__INCLUDED_)
#define AFX_ABORT_H__8AA34D02_732D_11D2_B42B_006008A70A6A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Abort.h : header file
//
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAbort dialog

class CAbort : public CDialog
{
// Construction
public:
	CAbort(CWnd* pParent = NULL);   // standard constructor
	~CAbort();

	CString m_cTitle;
// Dialog Data
	//{{AFX_DATA(CAbort)
	enum { IDD = IDD_DIALOG_QUIT };
	CString	m_cInstructions;
	//}}AFX_DATA

	typedef enum {AbortMeasurement = 9 ,AbortButKeepWhatWeHave = 66, ContinueOnward = 11} fromnatz;
	fromnatz result;

	bool bLastState;
	void SetControlStates();
	void OnAbortRetain();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbort)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CWnd* pAbortItem;
	CWnd* pCancelItem;
	// Generated message map functions
	//{{AFX_MSG(CAbort)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAbortok();
	afx_msg void OnTimer(UINT nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABORT_H__8AA34D02_732D_11D2_B42B_006008A70A6A__INCLUDED_)
