//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: MainFrm.h $
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 6  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 5  *****************
// User: Longo        Date: 1/05/05    Time: 5:30p
// Updated in $/FDMS/FDMS Application/FDMSNET
// import button added to toolbar
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_MAINFRM_H__86DB230B_2B86_4A66_987F_0F29373F087E__INCLUDED_)
#define AFX_MAINFRM_H__86DB230B_2B86_4A66_987F_0F29373F087E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MeasureDlg.h"
#include "AllOtherDlg.h"
#include "VerifyDlg.h"
#include "CycVer.h"
#include "ULTooToolBar.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  // control bar embedded members
	CStatusBar	 m_wndStatusBar;
	CMeasureDlg	 m_MeasureDlg;
	CVerifyDlg	 m_VerifyDlg;
	CAllOtherDlg m_AllOtherDlg;
	CCycVer		 m_CycVer;
	ULTooToolBar    m_wndToolBar;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGoButton();
	afx_msg void OnMenuSelection();
	afx_msg void OnUpdateImportButton(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdateFrameMenu(HMENU hMenuAlt);

public:
	afx_msg void OnImportButton();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void CreateToolMenu();
	bool bNeedsMenuUpdate;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__86DB230B_2B86_4A66_987F_0F29373F087E__INCLUDED_)
