// $History: SerialCom.cpp $
// 
// *****************  Version 5  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 4  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// Simple serial communications 

#include "stdafx.h"
#include "SerialCom.h"
#include "Resource.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ASYNC_IO

/////////////////////////////////////////////////////////////////////////////
// CSerialCom
// simple serial port communications

IMPLEMENT_DYNCREATE(CSerialCom, DetectorThread)

//constructor
CSerialCom::CSerialCom()
{

	m_hComm = INVALID_HANDLE_VALUE;
	lpCommProp = NULL;
	lpdcb = NULL;

	//init not in local long break
	m_bInLocalBreak = false;

}

CSerialCom::~CSerialCom()
{
}

void
CSerialCom::EscapeCommFunction(DWORD dwFunc)
{
	if (m_hComm != INVALID_HANDLE_VALUE)
		::EscapeCommFunction(m_hComm,dwFunc);
}

unsigned long CSerialCom::Transmit(int Length, char* Output, int /*Node*/)
{
	//output Output to file\com.
	unsigned long BytesWritten = 0;
	//if command is to send a long break then do it
	if ((strncmp(Output,LONGBREAKSET,12)==0) ||
		(strncmp(Output,LOCALLONGBREAK,14)==0))
	{
		//only if comm device allows longbreak
		if (m_bLongBreak) 
		{
			m_bInLocalBreak = true;
			SetCommBreak(m_hComm);
		}
	} 
	//if command is to terminate a long break then do it
	else if (strncmp(Output,LONGBREAKCLR,12)==0)
	{
		//only if comm device allows longbreak
		if (m_bLongBreak) 
		{
			ClearCommBreak(m_hComm);
			m_bInLocalBreak = false;
		}
	}
	else
	{
		//send the command otherwise
		WriteFile (m_hComm, Output, (Length==ISZSTRING)?strlen(Output):Length,
			&BytesWritten, &m_OverlapT);
	}
	//echo the command to the watcher if there is one
	if (m_pWatch)
	{
		int dlength = (Length==ISZSTRING)?strlen(Output):Length;
		char*Data = (char*)malloc(dlength+2);
		memmove(Data,Output,dlength);
		if (Data[0]==0x0d) Data[0]= NULL;
		Data[dlength] = 0x0a;
		Data[dlength+1]=NULL;
		m_pWatch->PostMessage(IDC_RECEIVE,dlength+1,(LPARAM)Data);
	}
	return BytesWritten;
}


void CSerialCom::DoStart(WPARAM /*WWord*/, LPARAM /*LWord*/)
{

	//	BYTE EndOfLine1 = 0x0a;
	//	BYTE EndOfLine2 = 0x0d;
	//	BYTE Accumulator[1024];
	struct ReceiveStruct 
	{
		int DataLength;
		char* Data;
	};
	//	int At=0;
	BYTE inbuff[1024];
	DWORD nBytesRead, dwEvent, dwError;
	COMSTAT cs;
	m_bDone = false;
	//begin read loop here
	while (!m_bDone) {
		//hang reads here 
		if (WaitCommEvent(m_hComm, &dwEvent, NULL))
		{
			//clear the "error" flags. actually the char received
			ClearCommError(m_hComm, &dwError, &cs);
			//if the event out of waitcom says we have a char and there is one
			if ((dwEvent & EV_RXCHAR) && cs.cbInQue) 
			{
				//go get the number of characters the port says we have
				//up to the size of the buffer
				if (!ReadFile(m_hComm, inbuff, min(cs.cbInQue,sizeof(inbuff)),
					&nBytesRead, &m_OverlapR)) 
				{
					//if for some reason we can't do a read of that many 
					//characters or any other error occurs on the read
					//then do a time-out dialog box and continue
					//					if (!m_bDone)
					//					{
					//						CTimeMsgDlg MsgBox(NULL);
					//						MsgBox.DoModal(NAMELONG,"Communications Error on ReadFile");
					//					}
				}
				else 
				{
					//if we actually got some characters to read
					if (nBytesRead)
					{

						//give all of the buffer to the virtual instrument.
						if (m_pReceiver)
						{
							ComData* ReceiveData = (ComData*)malloc(sizeof(ComData));
							ReceiveData->Size = nBytesRead;
							ReceiveData->Data = (char*)malloc(nBytesRead);
							memmove(ReceiveData->Data,inbuff,nBytesRead);
							m_pReceiver->PostMessage(IDC_RECEIVE,m_dNode,(LPARAM)ReceiveData);
						}
						//give all of the buffer to the watcher if there is one
						if (m_pWatch)
						{
							char* Data = (char*)malloc(nBytesRead);
							memmove(Data,inbuff,nBytesRead);
							m_pWatch->PostMessage(IDC_RECEIVE,nBytesRead,(LPARAM)Data);
						}

					}//end if (nBytesRead
				}//if (file read ELSE
			}//end if ((dwEvent

		}//end if (WaitCommEvent
		else
		{
			//			if (!m_bDone)
			//			{
			//				CTimeMsgDlg MsgBox(NULL);
			//				MsgBox.DoModal(NAMELONG,"Communications Error on WaitCommEvent");
			//			}

		}
	}//end while not done
	PurgeComm(m_hComm, PURGE_RXCLEAR);
}


//initialize the serial port
//from information in the INI file
bool
CSerialCom::SetUpPort(char* name, unsigned BaudRate, 
					  unsigned DataBits, unsigned StopBits, 
					  char* Parity, char* Port, bool LongBreak)
{
	strcpy(m_pName,name);
	//allow long break on this port?
	char temp[32];
	m_bLongBreak = LongBreak;
	//get the baudrate, databits, stopbits, parity, 
	//and physical port from INI file
	m_dBaudRate = BaudRate;
	m_dDataBits = DataBits;
	m_dStopBits = StopBits;
	strncpy(m_pParity,Parity,sizeof(m_pParity)-1);
	strncpy(m_pPort,Port,sizeof(m_pPort)-1);

	//convert baudrate to "internal" form
	switch (m_dBaudRate) {
	case 300:	BaudRate = BAUD_075;	break;
	case 600:	BaudRate = BAUD_600;	break;
	case 1200:	BaudRate = BAUD_1200;	break;
	case 1800:	BaudRate = BAUD_1800;	break;
	case 2400:	BaudRate = BAUD_2400;	break;
	case 4800:	BaudRate = BAUD_4800;	break;
	case 7200:	BaudRate = BAUD_7200;	break;
	case 9600:	BaudRate = BAUD_9600;	break;
	case 14400:	BaudRate = BAUD_14400;	break;
	case 19200:	BaudRate = BAUD_19200;	break;
	case 38400:	BaudRate = BAUD_38400;	break;
	case 56000:	BaudRate = BAUD_56K;	break;
	case 57600:	BaudRate = BAUD_57600;	break;
	case 115200:BaudRate = BAUD_115200;	break;
	case 128000:BaudRate = BAUD_128K;	break;
	default:
		BaudRate = BAUD_9600; 
	}

	//convert databits to "internal" form
	switch (m_dDataBits) {
	case 5:		DataBits = DATABITS_5;	break;
	case 6:		DataBits = DATABITS_6;	break;
	case 7:		DataBits = DATABITS_7;	break;
	case 8:		DataBits = DATABITS_8;	break;
	case 16:	DataBits = DATABITS_16;	break;
	default:
		DataBits = DATABITS_7;
	}

	//necessay to resolve ports com10 and above
	//but also works on com1 - com9
	strcpy(temp,"\\\\.\\");
	strcat(temp,m_pPort);

	//open the physical the port by name
	if ((m_hComm = CreateFile(temp,//m_pPort,	//port name
		GENERIC_READ | GENERIC_WRITE, 	//read and write
		0,								//not shared (can't on com)
		NULL,							//default security attributes
		OPEN_EXISTING,					//open existing com port
#ifdef ASYNC_IO
		FILE_FLAG_OVERLAPPED,			//async i/o
#else
		NULL,							//sync i/o
#endif
		NULL))==INVALID_HANDLE_VALUE)	//template file -- not used on com
		return false;

	//configure the port
	//get the size of the actual commprop packet
	//need to do this twice, once to get the size of COMMPROP
	//and then to get the actual data
	lpCommProp = (LPCOMMPROP)malloc(sizeof(COMMPROP));
	lpCommProp->wPacketLength = sizeof(COMMPROP);
	GetCommProperties(m_hComm, lpCommProp);
	unsigned short dwSize = lpCommProp->wPacketLength;
	free(lpCommProp);

	//get the actual commprop
	lpCommProp = (LPCOMMPROP)malloc(dwSize);
	lpCommProp->wPacketLength = dwSize;
	GetCommProperties(m_hComm, lpCommProp);

	//verify that we have a valid packet
	if ((lpCommProp->wPacketVersion != 0xffff) && //win95
		(lpCommProp->wPacketVersion != 2))		  //NT	
	{
		free(lpCommProp);
		lpCommProp = NULL;
		return false;
	}

	//verify that we have a serial port
	if (lpCommProp->dwServiceMask != SP_SERIALCOMM)
	{
		free(lpCommProp);
		lpCommProp = NULL;
		return false;
	}
	else
		if (lpCommProp->dwProvSubType != PST_RS232)
		{
			free(lpCommProp);
			lpCommProp = NULL;
			return false;
		}

		//verify we can have the baud rate we need
		if (lpCommProp->dwMaxBaud != BAUD_USER)		//user settable?
			if (lpCommProp->dwMaxBaud != BaudRate)	//nope-is it at what we need?
			{
				free(lpCommProp);
				lpCommProp = NULL;
				return false;
			}

			//get device control block
			lpdcb = (DCB*)malloc(sizeof(DCB));
			GetCommState(m_hComm,lpdcb);

			//set baud rate
			if (lpCommProp->dwSettableParams & SP_BAUD)

				lpdcb->BaudRate = m_dBaudRate;

			//force binary
			lpdcb->fBinary = true;

			//set parity checking 
			if (lpCommProp->dwSettableParams & SP_PARITY_CHECK)
			{
				if (strcmp(m_pParity,"NONE")==0)
					lpdcb->fParity = false;
				else
					lpdcb->fParity = true;
			}

			//set clear to send and data set ready flow control off
			lpdcb->fOutxCtsFlow = false;
			lpdcb->fOutxDsrFlow = false;

			//set DTR on
			lpdcb->fDtrControl = DTR_CONTROL_ENABLE;

			//set xoff and xon to non-functioning state
			lpdcb->fOutX = false;
			lpdcb->fInX  = false;

			//set parity error replace char flag
			//	lpdcb->fErrorChar = true;	//doesn't seem to work on WIN95 -- ok on NT4.0
			lpdcb->fErrorChar = false;	//it even seems to screw up the stream

			//make sure we can receive NULL bytes
			lpdcb->fNull = false;

			//no RTS flow control
			//this seems backwards -- but was needed 
			//to get the grands to talk
			lpdcb->fRtsControl = RTS_CONTROL_ENABLE;

			//always abort on error
			lpdcb->fAbortOnError = true;

			//set data size
			if (lpCommProp->dwSettableParams & SP_DATABITS)
				lpdcb->ByteSize = (unsigned char)m_dDataBits;

			//set parity
			if (strcmp(m_pParity,"EVEN")==0)
				lpdcb->Parity = EVENPARITY;
			else if (strcmp(m_pParity,"ODD")==0)
				lpdcb->Parity = ODDPARITY;
			else if (strcmp(m_pParity,"NONE")==0)
				lpdcb->Parity = NOPARITY;
			else if (strcmp(m_pParity,"MARK")==0)
				lpdcb->Parity = MARKPARITY;
			else
				lpdcb->Parity = EVENPARITY;

			//set stop bits
			switch (m_dStopBits) {
	case 1: lpdcb->StopBits = ONESTOPBIT; break;//one
	case 2: lpdcb->StopBits = TWOSTOPBITS;break;//two
	case 15:lpdcb->StopBits = ONE5STOPBITS;break;//one and a half
	default:lpdcb->StopBits = ONESTOPBIT; break;//one
			}

			//set replacement char for parity errors
			lpdcb->ErrorChar = '#';

			//set the port to what we want
			BOOL Result = SetCommState(m_hComm, lpdcb);

			//The SetCommMask function specifies a set of events 
			//to be monitored for a communications device
			SetCommMask(m_hComm, EV_RXCHAR | EV_BREAK | EV_ERR );

			//create an event object for the com to wait on
			m_OverlapR.Internal = 0;
			m_OverlapR.InternalHigh = 0;
			m_OverlapR.Offset = 0;
			m_OverlapR.OffsetHigh = 0;
			m_OverlapR.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

			m_OverlapT.Internal = 0;
			m_OverlapT.InternalHigh = 0;
			m_OverlapT.Offset = 0;
			m_OverlapT.OffsetHigh = 0;
			m_OverlapT.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
			return Result?true:false;
}

int CSerialCom::ExitInstance()
{
	//close comm port 
	if (m_hComm != INVALID_HANDLE_VALUE)
		CloseHandle(m_hComm);
	return DetectorThread::ExitInstance();
}

void CSerialCom::DoStop()
{
	//flag dostart to exit now
	DetectorThread::DoStop();
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		PurgeComm(m_hComm,PURGE_RXCLEAR);
		PurgeComm(m_hComm,PURGE_TXCLEAR);
		CloseHandle(m_hComm);
		WaitForSingleObject(m_hComm,2000);
		m_hComm = INVALID_HANDLE_VALUE;
	}

}

void
CSerialCom::DoShutdown(WPARAM w, LPARAM l)
{
	if (lpCommProp) free(lpCommProp);
	if (lpdcb) free(lpdcb);
	DetectorThread::DoShutdown(w, l);
}

BEGIN_MESSAGE_MAP(CSerialCom, DetectorThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialCom message handlers
