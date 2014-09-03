
#pragma once


#define LONGBREAKSET	"LONGBREAKSET"
#define LOCALLONGBREAK	"LOCALLONGBREAK"
#define LONGBREAKCLR	"LONGBREAKCLR"


//#define TICKLE			5003
/////////////////////////////////////////////////////////////////////////////
// CSerialCom thread


#include "DetectorThread.h"

class CSerialCom : public DetectorThread
{
	DECLARE_DYNCREATE(CSerialCom )
protected:
	CSerialCom();           // protected constructor used by dynamic creation

	OVERLAPPED	m_OverlapR;
	OVERLAPPED	m_OverlapT;
	OVERLAPPED	m_OverlapDump;
	
	char		m_pPort[16];

	int			m_dBaudRate;
	int			m_dDataBits;
	int			m_dStopBits;

	bool		m_bLongBreak;
	bool		m_bInLocalBreak;
	char		m_pParity[16];

	HANDLE		m_hComm;
	DCB *		lpdcb;
	LPCOMMPROP	lpCommProp;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialCom)
	public:

	//}}AFX_VIRTUAL
	virtual bool SetUpPort(char * name, unsigned BaudRate, 
		unsigned Databits, unsigned StopBits, char* Parity, 
		char* Port, bool LongBreak);
// Implementation
	virtual ~CSerialCom();
	virtual int ExitInstance();
	virtual void DoStop();
	virtual unsigned long Transmit(int bytes,char *,int node = 0);
	virtual void DoStart(WPARAM,LPARAM);
	virtual void DoShutdown(WPARAM,LPARAM);
	virtual void EscapeCommFunction(DWORD dwFunc);


protected:
	int m_dNode;

	// Generated message map functions
	//{{AFX_MSG(CSerialCom)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

