//GraphLegendSet.h - Version 3.0

#if !defined(AFX_GRAPHLEGENDSET_H__3CBDC4C4_494F_11D5_AD40_006008198224__INCLUDED_)
#define AFX_GRAPHLEGENDSET_H__3CBDC4C4_494F_11D5_AD40_006008198224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphLegendSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphLegendSet window

class CGraphLegendSet : public CStatic
{
// Construction
public:
	CGraphLegendSet();

// Attributes
public:
	int legendId;	//only used for searching through a list of this object

private :
	CString legendLabel;
	int m_iStyle;
	COLORREF m_cColor;
	bool m_bDoShow;

// Operations
public:
	void SetLegendLabel(int id, CString legendText, COLORREF cColor=NULL, int iState = 0, bool bDoShow = true);
	CString GetLegendLabel();
	COLORREF GetLegendColor();
	int GetLegendStyle();
	bool GetDoShow();
	void ToggleDoShow() {m_bDoShow = !m_bDoShow;};
	void SetDoShow(bool bDoShow) {m_bDoShow = bDoShow;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphLegendSet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphLegendSet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphLegendSet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHLEGENDSET_H__3CBDC4C4_494F_11D5_AD40_006008198224__INCLUDED_)
