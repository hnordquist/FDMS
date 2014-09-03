#if !defined(AFX_ULEXITDLG_H__EA8BD835_7F44_49D5_80CD_83C4A9E581D7__INCLUDED_)
#define AFX_ULEXITDLG_H__EA8BD835_7F44_49D5_80CD_83C4A9E581D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULExitDlg.h : header file
// $History: ULExitDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
#include "..\resource.h"
/////////////////////////////////////////////////////////////////////////////
// ULExitDlg dialog

class ULExitDlg : public CDialog
{
// Construction
public:
	ULExitDlg(CString AppName = "", CString ExitMsg = "", int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULExitDlg)
	enum { IDD = IDD_GENERIC_EXIT_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULExitDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULExitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExitAllToolsBtn();
	afx_msg void OnExitOnlyThisToolBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private: 
	int m_ButtonPushed;
	CString m_ExitMsg;
	CString m_AppName;
	int m_IconID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULEXITDLG_H__EA8BD835_7F44_49D5_80CD_83C4A9E581D7__INCLUDED_)
