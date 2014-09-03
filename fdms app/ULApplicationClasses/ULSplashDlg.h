#if !defined(AFX_ULSPLASHDLG_H__3E58FD00_1EDC_4A20_93C3_08B8B6852B71__INCLUDED_)
#define AFX_ULSPLASHDLG_H__3E58FD00_1EDC_4A20_93C3_08B8B6852B71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULSplashDlg.h : header file
// $History: ULSplashDlg.h $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// compiler warning removal
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULSplashDlg dialog

class ULSplashDlg : public CDialog
{
	// Construction
public:
	ULSplashDlg(CString Version, CString ProgrammerEmail, UINT DisplayDurationSecs, int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(ULSplashDlg)
	enum { IDD = IDD_RT_SPLASH };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULSplashDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULSplashDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void HideSplashScreen();

private:
	int m_IconID;
	CString m_Version;
	CString m_ProgrammerEmail;
	UINT m_DisplayDurationSecs;
	//		UINT m_StartTime;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULSPLASHDLG_H__3E58FD00_1EDC_4A20_93C3_08B8B6852B71__INCLUDED_)
