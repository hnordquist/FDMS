#if !defined(AFX_WARNING_H__8AA34D03_732D_11D2_B42B_006008A70A6A__INCLUDED_)
#define AFX_WARNING_H__8AA34D03_732D_11D2_B42B_006008A70A6A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Warning.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CWarning dialog

class CWarning : public CDialog
{
// Construction
public:
	CWarning(CWnd* pParent = NULL);   // standard constructor
	CString m_cTitle;

// Dialog Data
	//{{AFX_DATA(CWarning)
	enum { IDD = IDD_DIALOG2 };
	CString	m_szMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarning)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	HACCEL m_hAccel;
	// Generated message map functions
	//{{AFX_MSG(CWarning)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARNING_H__8AA34D03_732D_11D2_B42B_006008A70A6A__INCLUDED_)
