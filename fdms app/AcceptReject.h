#if !defined(AFX_ACCEPTREJECT_H__600C7BBE_A89A_4183_A4A4_5C5CC942FC40__INCLUDED_)
#define AFX_ACCEPTREJECT_H__600C7BBE_A89A_4183_A4A4_5C5CC942FC40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcceptReject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAcceptReject dialog

class CAcceptReject : public CDialog
{
// Construction
public:
	CAcceptReject(CWnd* pParent = NULL);   // standard constructor
	CString m_cDialogLabel;

// Dialog Data
	//{{AFX_DATA(CAcceptReject)
	enum { IDD = IDD_DIALOG_ACCEPT };
	CString	m_cLabel[MaxChannels];
	CString	m_cLabelNew;
	CString	m_cLabelPrev;
	CString	m_cNew[MaxChannels];
	CString	m_cPrev[MaxChannels];
	CString	m_cLabelTitle;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcceptReject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAcceptReject)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCEPTREJECT_H__600C7BBE_A89A_4183_A4A4_5C5CC942FC40__INCLUDED_)
