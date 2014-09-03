// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__922207DF_B6CF_492A_9748_B58A835B1926__INCLUDED_)
#define AFX_CHILDFRM_H__922207DF_B6CF_492A_9748_B58A835B1926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
protected:

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
//	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//bool bNeedsMenuUpdate;
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnUpdateMenuItems(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImportButton(CCmdUI *pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEditResetacknowledgements();
	afx_msg void OnEditConfigDUMP_O_RAMA_Current();
	afx_msg void OnEditConfigDUMP_O_RAMA_Default();
	afx_msg void OnEditPrefBGC(UINT nID);
	afx_msg void OnEditPrefBGCT(UINT nID);
	afx_msg void OnEditPrefDE(UINT nID);
	afx_msg void OnEditPrefDCT(UINT nID);
	afx_msg void OnEditPrefSMC(UINT nID);
	afx_msg void OnEditPrefSMCT(UINT nID);
	afx_msg void OnEditPrefDID(UINT nID);
	afx_msg void OnToolGoose(UINT nID);
	afx_msg void OnImportButton();

public:
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__922207DF_B6CF_492A_9748_B58A835B1926__INCLUDED_)
