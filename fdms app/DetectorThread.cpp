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

