#if !defined(AFX_ULTITLEDLG_H__05B35863_D8A5_4C3D_8990_225F3D1317FF__INCLUDED_)
#define AFX_ULTITLEDLG_H__05B35863_D8A5_4C3D_8990_225F3D1317FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULTitleDlg.h : header file
// $History: ULTitleDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULTitleDlg dialog

class ULTitleDlg : public CDialog
{
// Construction
public:
	ULTitleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULTitleDlg)
	enum { IDD = IDD_RT_TITLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULTitleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULTitleDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULTITLEDLG_H__05B35863_D8A5_4C3D_8990_225F3D1317FF__INCLUDED_)
