#if !defined(AFX_GRAPHHELP_H__450D7A78_5282_417A_B2F9_350CE15058E5__INCLUDED_)
#define AFX_GRAPHHELP_H__450D7A78_5282_417A_B2F9_350CE15058E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphHelp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphHelp dialog

class CGraphHelp : public CDialog
{
// Construction
public:
	CGraphHelp(CWnd* pParent = NULL);   // standard constructor
	void ToggleDisplay();
// Dialog Data
	//{{AFX_DATA(CGraphHelp)
	enum { IDD = IDD_DIALOG_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphHelp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	bool m_bVisible;
	// Generated message map functions
	//{{AFX_MSG(CGraphHelp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHHELP_H__450D7A78_5282_417A_B2F9_350CE15058E5__INCLUDED_)
