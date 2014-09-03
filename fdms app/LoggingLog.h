#pragma once


// LoggingLog dialog

class LoggingLog : public CDialog
{
private:
	using CDialog::Create;

public:
	LoggingLog(CWnd* pParent);   // standard constructor
	BOOL Create();


// Dialog Data
	enum { IDD = IDD_DIALOG4 };

		CListBox m_OList;

		void AddLine(LPCTSTR str)
		{
			m_OList.AddString(str);
		}

// Implementation
protected:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	CWnd* m_pParent;
	int m_nID;

	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	bool CutToClipboard(bool bAll);
	bool CopyToClipboard(bool bAll, bool bHeaders = false);
	bool CopyFromClipboard(bool bReplaceSelected);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
