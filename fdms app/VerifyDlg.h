#if !defined(AFX_VERIFYDLG_H__38F18639_5590_4116_B7F2_07E85420D5E2__INCLUDED_)
#define AFX_VERIFYDLG_H__38F18639_5590_4116_B7F2_07E85420D5E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerifyDlg.h : header file
//
#include "CCButton.h"
/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog
extern HWND g_hVerifyDlg;

class CVerifyDlg : public CDialog
{
// Construction
public:
	CVerifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVerifyDlg)
	enum { IDD = IDD_DIALOG1 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CColorButton m_cButtonCycleVerify;
	CColorButton m_cButtonAssemblyVerify;
	CColorButton m_cButtonItemReverify;
	CColorButton m_cButtonReturn;
	bool FindGraph(CString cTitle);

	// Generated message map functions
	//{{AFX_MSG(CVerifyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCycleVer();
	afx_msg void OnAssemVer();
	afx_msg void OnItemVer();
	afx_msg void OnReturn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERIFYDLG_H__38F18639_5590_4116_B7F2_07E85420D5E2__INCLUDED_)
