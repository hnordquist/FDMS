#pragma once

#define IDC_RECEIVE		5000
#define IDC_COM_START	5001
#define IDC_COM_STOP	5002
#define IDC_COM_AFXTERMINATE	5009
#define ISZSTRING		(-1)

class DetectorThread : public CWinThread
{
	DECLARE_DYNCREATE(DetectorThread )
protected:
	DetectorThread();           // protected constructor used by dynamic creation
	char		m_pName[32];
	bool		m_bDone;
	CWnd*		m_pReceiver;
	CWnd*		m_pWatch;

// Attributes
public:
	bool	m_bMiniGrand;
	virtual void TerminateMe(WPARAM, LPARAM);
	virtual void DoStart(WPARAM,LPARAM);
	virtual void DoShutdown(WPARAM,LPARAM);
	virtual void DoStop();
	virtual char *GetName();
	virtual unsigned long Transmit(int bytes,char *,int node = 0);

// Operations
public:
	virtual bool SetUpPort(char * name, unsigned BaudRate, 
		unsigned Databits, unsigned StopBits, char* Parity, 
		char* Port, bool LongBreak);
	virtual bool Register(CWnd *, int);
	virtual bool UnRegister(int);
	virtual bool RegisterWatch(CWnd *, int);
	virtual bool UnRegisterWatch(int);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DetectorThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	virtual ~DetectorThread();
protected:
	int m_dNode;

	// Generated message map functions
	//{{AFX_MSG(DetectorThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
