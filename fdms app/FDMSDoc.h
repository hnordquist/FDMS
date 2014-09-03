// FDMSDoc.h : interface of the CFDMSDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FDMSDOC_H__1EFF89D8_AB07_4118_828E_61CD2012A458__INCLUDED_)
#define AFX_FDMSDOC_H__1EFF89D8_AB07_4118_828E_61CD2012A458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFDMSDoc : public CDocument
{
protected: // create from serialization only
	CFDMSDoc();
	DECLARE_DYNCREATE(CFDMSDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDMSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFDMSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFDMSDoc)
	afx_msg void OnUpdateMenuItems(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDMSDOC_H__1EFF89D8_AB07_4118_828E_61CD2012A458__INCLUDED_)
