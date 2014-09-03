#if !defined(AFX_PROMPT_H__85A86B62_4E71_11D2_B3EC_006008A70A6A__INCLUDED_)
#define AFX_PROMPT_H__85A86B62_4E71_11D2_B3EC_006008A70A6A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Prompt.h : header file
//
//#include "UWNCCDlg.h"
#include "resource.h"
////////////////////////////////////////////////////////////////////////////
// CPrompt dialog

class CPrompt : public CDialog
{
// Construction
public:
	CPrompt(CWnd* pParent = NULL, bool Large = false, bool Cyrillic = false);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CPrompt)
	enum { IDD = IDD_DIALOG3 };
	CStatic	m_StaticInst;
	CEdit	m_pEditString;
	CString	m_EditString;
	CString	m_StaticInstructions;
	CString m_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrompt)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bPassword;
protected:
	bool m_bLarge;
	bool m_bCyrillic;
//	HACCEL m_hAccel;
//	CFont EditFont;

	// Generated message map functions
	//{{AFX_MSG(CPrompt)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton0();
	afx_msg void OnButtonsign();
	afx_msg void OnButtonce();
	afx_msg void OnButtonca();
	afx_msg void OnButtondp();
	afx_msg void OnChangeEdit();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMPT_H__85A86B62_4E71_11D2_B3EC_006008A70A6A__INCLUDED_)
