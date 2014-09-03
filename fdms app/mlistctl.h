//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: mlistctl.h $
// 
// *****************  Version 4  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 3  *****************
// User: Longo        Date: 2/10/05    Time: 3:47p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 2  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
//////////////////////////////////////////////////////////////////////////

// CMyListCtrl window
#ifndef INC_MYLISTCTRL_H
#define INC_MYLISTCTRL_H

#pragma once


class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyListCtrl();
	int HitTestEx(CPoint &point, int *col) const;
	int HitTestRow(CPoint &point) const;
	void SetROColumns(bool* pRWColumnMap);

	void SelectRow(int index);
	void SelectAll(bool on);

	// Generated message map functions
protected:

	bool* pColumnData;

	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll( UINT, UINT, CWnd* );
	afx_msg void OnVScroll( UINT, UINT, CWnd* );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:

	bool CutToClipboard(bool bAll);
	bool CopyToClipboard(bool bAll, bool bHeaders = false);
	bool CopyFromClipboard(bool bReplaceSelected);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////
#endif
