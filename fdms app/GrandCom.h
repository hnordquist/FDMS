/*
Copyright (c) 2014, Los Alamos National Security, LLC
All rights reserved.
Copyright 2014. Los Alamos National Security, LLC. This software was produced under U.S. Government contract 
DE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security, 
LLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.  
NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, 
OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works, 
such modified software should be clearly marked, so as not to confuse it with the version available from LANL.

Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided 
that the following conditions are met:
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
•	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
nor the names of its contributors may be used to endorse or promote products derived from this software without specific 
prior written permission. 
THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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


