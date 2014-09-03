#if !defined(AFX_ULPRODUCTSUPPORTDLG_H__C09B6D82_CE74_465D_9775_A2A9E81E25C3__INCLUDED_)
#define AFX_ULPRODUCTSUPPORTDLG_H__C09B6D82_CE74_465D_9775_A2A9E81E25C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULProductSupportDlg.h : header file
// $History: ULProductSupportDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULProductSupportDlg dialog

class ULProductSupportDlg : public CDialog
{
// Construction
public:
	ULProductSupportDlg(CString AppName = "" , CString ContactName = "", CString Email = "", int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULProductSupportDlg)
	enum { IDD = IDD_SEB_PRODUCT_SUPPORT_BOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULProductSupportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULProductSupportDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_ApplicationName;
	CString m_ContactName;
	CString m_Email;
	int m_IconID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULPRODUCTSUPPORTDLG_H__C09B6D82_CE74_465D_9775_A2A9E81E25C3__INCLUDED_)
