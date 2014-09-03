#pragma once

#include "Graph.h"
#include "GraphHelp.h"
// CFDMSView view

// ready to move all for subclass shared variables and processing into this single class

class CFDMSView : public CView
{
	DECLARE_DYNCREATE(CFDMSView)

protected:
	CFDMSView();           // protected constructor used by dynamic creation
	virtual ~CFDMSView();
	virtual void UpdateGraphData();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDMSView)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CGraph* testGraph;
	BOOL graphComplete;
	CGraphHelp* m_GraphHelp;

protected:
	int m_iMouseControl;
protected:
	//{{AFX_MSG(CGraphNeutVGammaView)

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditCopy();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


