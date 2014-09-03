#if !defined(AFX_MEASUREDLG_H__59A19D1A_EA60_4516_B22E_0B3E3934F94E__INCLUDED_)
#define AFX_MEASUREDLG_H__59A19D1A_EA60_4516_B22E_0B3E3934F94E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureDlg.h : header file
//
#include "CCButton.h"
/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg dialog
extern HWND g_hMeasureDlg;

class CMeasureDlg : public CDialog
{
// Construction
public:
	CMeasureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasureDlg)
	enum { IDD = IDD_DIALOG_MEASURE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CColorButton m_cButtonBG;
	CColorButton m_cButtonAllOther;
	// Generated message map functions
	//{{AFX_MSG(CMeasureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBackGround( );
	afx_msg void OnAllOther( );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREDLG_H__59A19D1A_EA60_4516_B22E_0B3E3934F94E__INCLUDED_)
