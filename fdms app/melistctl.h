//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: melistctl.h $
// 
// *****************  Version 2  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/07/05    Time: 2:26p
// Created in $/FDMS/FDMS Application/FDMSNET
// support for new import edit dialog
//////////////////////////////////////////////////////////////////////////


#pragma once
#include "mlistctl.h"

class MyEditableListCtrl : public CMyListCtrl
{
// Construction
public:
	MyEditableListCtrl();

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
	virtual ~MyEditableListCtrl();
	CEdit* EditSubLabel( int nItem, int nCol );
	// Generated message map functions
protected:

	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

