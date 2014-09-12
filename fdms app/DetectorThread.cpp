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
#include "stdafx.h"
#include ".\detectorthread.h"

#define LONGBREAKSET	"LONGBREAKSET"
#define LOCALLONGBREAK	"LOCALLONGBREAK"
#define LONGBREAKCLR	"LONGBREAKCLR"


IMPLEMENT_DYNCREATE(DetectorThread, CWinThread)

//constructor
DetectorThread::DetectorThread()
{
	//init with no receiver to send data to
	m_pReceiver = NULL;
	//init with no port watcher to send data to
	m_pWatch = NULL;
	//init not done receiving
	m_bDone = false;

	m_bMiniGrand = false;

	m_dNode = 0;

}

DetectorThread::~DetectorThread()
{
}

void
DetectorThread::DoShutdown(WPARAM, LPARAM)
{
	ExitThread(0);
}

//get the user provided name of this communications object
char *
DetectorThread::GetName()
{
	//if we are not trying to shut down then
	//provide the name
	return m_bDone?"ERROR":&m_pName[0];
}

//register a port watcher with this communications object
bool
DetectorThread::RegisterWatch(CWnd * pWatch, int /*node*/)
{
	//only allow one watcher
	if (m_pWatch) return false;
	m_pWatch = pWatch;
	return true;
}

//remove the port watcher
bool
DetectorThread::UnRegisterWatch(int /*node*/)
{
	if (m_pWatch != NULL) 
	{
		m_pWatch = NULL;
		return true;
	}
	return false;
}

//register the instrument's receiver
//so that we know where to send the
//serial stream
bool
DetectorThread::Register(CWnd * pReceiver, int node)
{
	if (m_pReceiver && (node == 0))return false;
	m_dNode = node;
	m_pReceiver = pReceiver;
	return true;
}


//remove/unregister the instrument's receiver 
bool
DetectorThread::UnRegister(int node)
{
	if (m_pReceiver != NULL)
	{
		m_pReceiver = NULL;
		m_dNode = node;
		return true;
	}
	return false;
}

//initialize the serial port
//from information in the INI file
bool
DetectorThread::SetUpPort(char* , unsigned , 
					  unsigned , unsigned , 
					  char* , char* , bool )
{
	return true;
}

BOOL DetectorThread::InitInstance()
{
	return TRUE;
}

int DetectorThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


void DetectorThread::DoStop()
{
	//flag dostart to exit now
	m_bDone = true;

}

unsigned long DetectorThread::Transmit(int , char* , int /*Node*/)
{
	return 0;
}

void DetectorThread::DoStart(WPARAM /*WWord*/, LPARAM /*LWord*/)
{

}


void
DetectorThread::TerminateMe(WPARAM, LPARAM)
{
	AfxEndThread(0,TRUE);
}

BEGIN_MESSAGE_MAP(DetectorThread, CWinThread)
	//{{AFX_MSG_MAP(DetectorThread)
	ON_THREAD_MESSAGE(IDC_COM_START,DoStart)
	ON_THREAD_MESSAGE(IDC_COM_STOP,DoShutdown)
	ON_THREAD_MESSAGE(IDC_COM_AFXTERMINATE,TerminateMe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

