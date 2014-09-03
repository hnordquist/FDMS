#if !defined(AFX_ULPASSWORDCHANGEREQUESTDLG_H__BA09FA5A_B4DE_46FA_BEE8_75C9C10BF27E__INCLUDED_)
#define AFX_ULPASSWORDCHANGEREQUESTDLG_H__BA09FA5A_B4DE_46FA_BEE8_75C9C10BF27E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULPasswordChangeRequestDlg.h : header file
// $History: ULPasswordChangeRequestDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
#include "..\resource.h"
/////////////////////////////////////////////////////////////////////////////
// ULPasswordChangeRequestDlg dialog

class ULPasswordChangeRequestDlg : public CDialog
{
// Construction
public:
	ULPasswordChangeRequestDlg(CString Title, CString Password, int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULPasswordChangeRequestDlg)
	enum { IDD = IDD_SEB_PASSWORD_REQUEST_BOX };
	CString	m_EnteredPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULPasswordChangeRequestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULPasswordChangeRequestDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_Title;
	int m_IconID;
	CString m_Password;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULPASSWORDCHANGEREQUESTDLG_H__BA09FA5A_B4DE_46FA_BEE8_75C9C10BF27E__INCLUDED_)
