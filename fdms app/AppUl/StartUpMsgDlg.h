#if !defined(AFX_STARTUPMSGDLG_H__C05C3B18_4F46_4F4F_8D85_CC68D8B84310__INCLUDED_)
#define AFX_STARTUPMSGDLG_H__C05C3B18_4F46_4F4F_8D85_CC68D8B84310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartUpMsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartUpMsgDlg dialog

class CStartUpMsgDlg : public CDialog
{
// Construction
public:
	CStartUpMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartUpMsgDlg)
	enum { IDD = IDD_STARTUP_MSG_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartUpMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartUpMsgDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTUPMSGDLG_H__C05C3B18_4F46_4F4F_8D85_CC68D8B84310__INCLUDED_)
