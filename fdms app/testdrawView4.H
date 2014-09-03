// testdrawView3.h : interface of the CtestdrawView3 class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTDRAWVIEW4_H__3A3D2F8F_6568_428C_AFC7_9476C22FE4C3__INCLUDED_)
#define AFX_TESTDRAWVIEW4_H__3A3D2F8F_6568_428C_AFC7_9476C22FE4C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "fdmsview.h"
#define TITLE4 "Cycle Verification"
class CGraphCycleVerifView : public CFDMSView
{
protected: // create from serialization only
	CGraphCycleVerifView();
	DECLARE_DYNCREATE(CGraphCycleVerifView)

	void UpdateGraphData();
// Attributes
public:
	CDocument* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphCycleVerifView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphCycleVerifView();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphCycleVerifView)
	afx_msg void OnDrawScatter();
	//}}AFX_MSG
	afx_msg void OnMessageGraphHelp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGraphHelp(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in testdrawView4.cpp
inline CDocument* CGraphCycleVerifView::GetDocument()
   { return (CDocument*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDRAWVIEW4_H__3A3D2F8F_6568_428C_AFC7_9476C22FE4C3__INCLUDED_)
