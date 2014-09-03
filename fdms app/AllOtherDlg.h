//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: AllOtherDlg.h $ 
// 
// *****************  Version 4  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/18/05    Time: 9:43a
// Created in $/FDMS/FDMS Application/FDMSNET/FDMSNET
// new list ctrl code
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CCButton.h"
/////////////////////////////////////////////////////////////////////////////
// CAllOtherDlg dialog
extern HWND g_hAllOtherDlg;

class CAllOtherDlg : public CDialog
{
// Construction
public:
	CAllOtherDlg(CWnd* pParent = NULL);   // standard constructor

	void OpenSelectMeasurement(int iSelectItem);

// Dialog Data
	//{{AFX_DATA(CAllOtherDlg)
	enum { IDD = IDD_DIALOG_ALLOTHER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllOtherDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool FindGraph(CString cString);
	CColorButton m_cButtonMeasure;
	CColorButton m_cButtonIncludeGood;
	CColorButton m_cButtonIncludeSuspect;
	CColorButton m_cButtonAcceptGood;
	CColorButton m_cButtonNvsBU;
	CColorButton m_cButtonGBUvsTime;
	CColorButton m_cButtonNvsG;
	CColorButton m_cButtonReturn;
	bool m_bIncludeGood;
	bool m_bIncludeSuspect;
	int m_preIndex;
public:
	// Generated message map functions
	//{{AFX_MSG(CAllOtherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasure();
	afx_msg void OnIncludeGood();
	afx_msg void OnIncludeSuspect();
	afx_msg void OnAcceptGood();
	afx_msg void OnNeutronVsBU();
	afx_msg void OnGammaBUVsTime();
	afx_msg void OnNeutronVsGamma();
	afx_msg void OnReturn();
	afx_msg LRESULT OnMyShow(WPARAM WParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnMySelect(WPARAM WParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


