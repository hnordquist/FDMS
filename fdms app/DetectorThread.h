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
	int			m_dNode;

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

	// Generated message map functions
	//{{AFX_MSG(DetectorThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
