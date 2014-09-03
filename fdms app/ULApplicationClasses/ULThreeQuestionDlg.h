#if !defined(AFX_ULTHREEQUESTIONDLG_H__D5749F09_D807_45FF_B5DF_857025BCDE59__INCLUDED_)
#define AFX_ULTHREEQUESTIONDLG_H__D5749F09_D807_45FF_B5DF_857025BCDE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULThreeQuestionDlg.h : header file
// $History: ULThreeQuestionDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

/////////////////////////////////////////////////////////////////////////////
// ULThreeQuestionDlg dialog

class ULThreeQuestionDlg : public CDialog
{
// Construction
public:
	ULThreeQuestionDlg(CString Title, CString Message, CString Button1Txt, CString Button2Txt, CString Button3Txt, 
		int IconResID = 0, CWnd* pParent = NULL);   // standard constructor
	void DisableAllButtons();

// Dialog Data
	//{{AFX_DATA(ULThreeQuestionDlg)
	enum { IDD = IDD_SEB_THREE_QUESTION_BOX_LARGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULThreeQuestionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULThreeQuestionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_Title;
	CString m_Message;
	CString m_Button1Txt;
	CString m_Button2Txt;
	CString m_Button3Txt;
	int m_IconID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULTHREEQUESTIONDLG_H__D5749F09_D807_45FF_B5DF_857025BCDE59__INCLUDED_)
