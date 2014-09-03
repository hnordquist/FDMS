#if !defined(AFX_ULPROGRESSBARDLG_H__2901366B_654B_4262_BD60_5826DD07CFF4__INCLUDED_)
#define AFX_ULPROGRESSBARDLG_H__2901366B_654B_4262_BD60_5826DD07CFF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULProgressBarDlg.h : header file
// $History: ULProgressBarDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULProgressBarDlg dialog

class ULProgressBarDlg : public CDialog
{
// Construction
public:
	void SetMessage(CString Message);
	void ProgressBarSetValue(double PercentComplete);
	ULProgressBarDlg(CString Title, CString Message, bool WithCancelBtn, int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULProgressBarDlg)
	enum { IDD = IDD_SEB_PROGRESS_BAR };
	CScrollBar	m_ProgressBar;
	CButton	m_CancelBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULProgressBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULProgressBarDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_Title;
	CString m_Message;
	int m_IconID;
	bool m_WithCancelBtn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULPROGRESSBARDLG_H__2901366B_654B_4262_BD60_5826DD07CFF4__INCLUDED_)
