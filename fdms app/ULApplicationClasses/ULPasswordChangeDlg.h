#if !defined(AFX_ULPASSWORDCHANGEDLG_H__863A23C3_AC94_4A8E_8D98_2493B293C604__INCLUDED_)
#define AFX_ULPASSWORDCHANGEDLG_H__863A23C3_AC94_4A8E_8D98_2493B293C604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULPasswordChangeDlg.h : header file
// $History: ULPasswordChangeDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
#include "..\resource.h"
/////////////////////////////////////////////////////////////////////////////
// ULPasswordChangeDlg dialog

class ULPasswordChangeDlg : public CDialog
{
// Construction
public:
	bool DoThePasswordsMatch(CString Password1, CString Password2);
	ULPasswordChangeDlg(CString Title, CString Password1, CString Password2, 
		int IconResID = 0,	CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULPasswordChangeDlg)
	enum { IDD = IDD_SEB_PASSWORD_CHANGE_BOX };
	CString	m_Password1;
	CString	m_Password2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULPasswordChangeDlg)
	public:
	virtual int DoModal(CString& NewPassword);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULPasswordChangeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_Title;
	int m_IconID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULPASSWORDCHANGEDLG_H__863A23C3_AC94_4A8E_8D98_2493B293C604__INCLUDED_)
