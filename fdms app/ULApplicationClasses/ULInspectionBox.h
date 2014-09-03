#if !defined(AFX_ULINSPECTIONBOX_H__6EAD21AB_2E5F_4C04_8EB1_A9106AF9B5B1__INCLUDED_)
#define AFX_ULINSPECTIONBOX_H__6EAD21AB_2E5F_4C04_8EB1_A9106AF9B5B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULInspectionBox.h : header file
// $History: ULInspectionBox.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULInspectionBox dialog

class ULInspectionBox : public CDialog
{
// Construction
public:
	ULInspectionBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULInspectionBox)
	enum { IDD = IDD_SEB_IAEA_INSPECTION_BOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULInspectionBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULInspectionBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULINSPECTIONBOX_H__6EAD21AB_2E5F_4C04_8EB1_A9106AF9B5B1__INCLUDED_)
