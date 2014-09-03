#if !defined(AFX_ULMODELESSBOXDLG_H__45A16F35_8058_4362_A62E_19E2A679C781__INCLUDED_)
#define AFX_ULMODELESSBOXDLG_H__45A16F35_8058_4362_A62E_19E2A679C781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULModelessBoxDlg.h : header file
// $History: ULModelessBoxDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULModelessBoxDlg dialog

class ULModelessBoxDlg : public CDialog
{
// Construction
public:
	ULModelessBoxDlg(CString Title, CString Message, int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULModelessBoxDlg)
	enum { IDD = IDD_SEB_MODELESS_BOX_LARGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULModelessBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULModelessBoxDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_Title;
	CString m_Message;
	int m_IconID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULMODELESSBOXDLG_H__45A16F35_8058_4362_A62E_19E2A679C781__INCLUDED_)
