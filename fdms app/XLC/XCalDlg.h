#pragma once
#include "afxdtctl.h"
#include "atlcomtime.h"


// XCalDlg dialog

class XCalDlg : public CDialog
{
	DECLARE_DYNAMIC(XCalDlg)

public:
	XCalDlg(LONG x, LONG y,  const CString& title, CWnd* pParent = NULL);   // standard constructor
	virtual ~XCalDlg();

// Dialog Data
	enum { IDD = IDD_XCAL };

	bool bDataChanged() {return m_bChange;}

protected:
	LONG m_x;
	LONG m_y;
	bool m_bChange;
	CString m_sTitle;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_date;
	COleDateTime m_time;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDtnDatetimechangeTimepicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatepicker(NMHDR *pNMHDR, LRESULT *pResult);
};
