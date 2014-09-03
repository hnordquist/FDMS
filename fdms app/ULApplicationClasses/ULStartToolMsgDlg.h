#if !defined(AFX_ULSTARTTOOLMSGDLG_H__6A6C9706_FA4F_4136_A8D3_D23A9104B9D7__INCLUDED_)
#define AFX_ULSTARTTOOLMSGDLG_H__6A6C9706_FA4F_4136_A8D3_D23A9104B9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULStartToolMsgDlg.h : header file
// $History: ULStartToolMsgDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
#include "..\Resource.h"
/////////////////////////////////////////////////////////////////////////////
// ULStartToolMsgDlg dialog

class ULStartToolMsgDlg : public CDialog
{
// Construction
public:
	ULStartToolMsgDlg(CString Title, CString Message, int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULStartToolMsgDlg)
	enum { IDD = IDD_RT_START_TOOL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULStartToolMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULStartToolMsgDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_Message;
	int m_IconID;
	CString m_Title;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULSTARTTOOLMSGDLG_H__6A6C9706_FA4F_4136_A8D3_D23A9104B9D7__INCLUDED_)
