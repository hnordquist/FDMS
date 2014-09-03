//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: GrandCom.h $
// 
// *****************  Version 4  *****************
// User: Longo        Date: 12/21/04   Time: 4:14p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <afxdisp.h>
#include "DetectorThread.h"

/////////////////////////////////////////////////////////////////////////////
// GrandCom thread
extern double	g_dDemoA;
extern double	g_dDemoB;
extern double	g_dDemoC;
extern double	g_fDemoG1;
extern double	g_fDemoG2;
extern double	g_fDemoSingles;
extern double	g_fDemoSinglesError;
extern double	g_fDemoDoubles;
extern double	g_fDemoDoublesError;

class GrandCom : public DetectorThread
{
	DECLARE_DYNCREATE(GrandCom )
protected:
	GrandCom();           // protected constructor used by dynamic creation
	char		m_pEcho[128];

	char		m_pCommandAcc[64];
	CString		m_BackedUpCommands;
	int			m_dCommandAccIndex;
	int			m_dGrandState;

	int			m_dNumber0;
	int			m_dNumber1;
	int			m_dNumber2;

	double		m_dACnts;
	double		m_dBCnts;
	double		m_dCCnts;
	int			m_iTickCount;
	int			m_iSetTime;
	int			m_iPromptPosition;
	int			m_iOffsetPage;
	char		m_szMsg1[32];
	char		m_szMsg2[32];
	char		m_szMsg3[32];
	char		m_szMsg4[32];

	COleDateTime Now;

// Attributes
public:


	void DoRespond(WPARAM,LPARAM);

	virtual unsigned long Transmit(int bytes,char *,int node = 0);

// Operations
public:
	virtual bool SetUpPort(char * name, unsigned BaudRate, 
		unsigned Databits, unsigned StopBits, char* Parity, 
		char* Port, bool LongBreak);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GrandCom)
	public:
	//}}AFX_VIRTUAL

// Implementation
	virtual ~GrandCom();
protected:
	bool m_bIn8f;
	bool m_bInInternal;
	void SendBack(char*);
	char m_szBackedUpCommands[1024];
	int  m_iOffsets[2][12];
	// Generated message map functions
	//{{AFX_MSG(GrandCom)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


