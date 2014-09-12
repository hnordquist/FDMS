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
// $History: GrandCom.cpp $
// 
// *****************  Version 5  *****************
// User: Longo        Date: 12/21/04   Time: 4:14p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GrandCom.h"
#include "Resource.h"
#include "dconfig.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// GrandCom

#define EOL1  (0x0a)
#define	EOL2  (0x0d)

double		g_dDemoA;
double		g_dDemoB;
double		g_dDemoC;
double	g_fDemoG1;
double	g_fDemoG2;
double	g_fDemoSingles;
double	g_fDemoSinglesError;
double	g_fDemoDoubles;
double	g_fDemoDoublesError;

extern char		g_cIniFile[_MAX_PATH];//the path/name for .ini file


/////////////////////////////////////////////////////////////////////////////
// GrandCom
// simple serial port communications

IMPLEMENT_DYNCREATE(GrandCom, DetectorThread)

//constructor
GrandCom::GrandCom()
{
//init with no data in command accumulator
	m_pCommandAcc[0] = NULL;
	m_dCommandAccIndex = 0;
	//initial grand state
	m_dGrandState = 0;
	//initial seconds for acquire
	m_dNumber0 = 20;
	m_dNumber1 = 20;
	m_dNumber2 = 0;
	m_bMiniGrand = false;
	m_bIn8f = false;
	m_bInInternal = true;
	m_iTickCount = 0;
	m_iSetTime = 10;
}


GrandCom::~GrandCom()
{
	//free up the memory used
}


//initialize the serial port
//from information in the INI file
bool
GrandCom::SetUpPort(char * name, unsigned /*BaudRate*/, 
					  unsigned /*DataBits*/, unsigned /*StopBits*/, 
					  char* /*Parity*/, char* /*Port*/, bool /*LongBreak*/)
{
	strcpy(m_pName,name);
	return true;
}

unsigned long GrandCom::Transmit(int Length, char* Output, int /*Node*/)
{
	TRACE2("GrandCom::Transmit(%d,%s)\n",Length,Output);
	//output Output to file\com.
	unsigned long BytesWritten;
	if (Length == ISZSTRING)
		BytesWritten = strlen(Output);
	else
		BytesWritten = Length;
		for (unsigned i = 0; i < BytesWritten; i++)
	{
		if ((Output[i] == EOL1) || (Output[i] == EOL2))
		{
			//send what we got iff we have something to send
//			if (m_dCommandAccIndex)
//			{
				char * temp = (char*)malloc(m_dCommandAccIndex+1);
				memmove(temp,m_pCommandAcc,m_dCommandAccIndex);
				temp[m_dCommandAccIndex] = NULL;
				PostThreadMessage(IDC_RECEIVE,BytesWritten,(LPARAM)temp);
//			}
			//reset for next command
			m_pCommandAcc[0] = NULL;
			m_dCommandAccIndex = 0;
		}
		else
			m_pCommandAcc[m_dCommandAccIndex++] = Output[i];
	}
	if (m_bIn8f)
	{
		m_pCommandAcc[m_dCommandAccIndex] = NULL;
		char *cTemp;
		if ((cTemp = strchr(m_pCommandAcc,'Q'))!=NULL)
		{
			char * temp = (char*)malloc(2*sizeof(char));
			temp[0] = 'Q'; temp[1] = NULL;
			PostThreadMessage(IDC_RECEIVE,1,(LPARAM)temp);
			temp = m_pCommandAcc;
			cTemp++;
			m_dCommandAccIndex = 0;
			while (*cTemp != NULL) 
			{
				*temp++ = *cTemp++; 
				m_dCommandAccIndex++;
				if (*temp == '\r')
				{
					char *temp = (char*)malloc(m_dCommandAccIndex+1);
					memmove(temp,m_pCommandAcc,m_dCommandAccIndex);
					temp[m_dCommandAccIndex] = NULL;
					PostThreadMessage(IDC_RECEIVE,m_dCommandAccIndex,(LPARAM)temp);
					m_dCommandAccIndex = 0;
					temp = m_pCommandAcc;
				}
			}
		}
	}

	return BytesWritten;
}



BEGIN_MESSAGE_MAP(GrandCom, DetectorThread)
	//{{AFX_MSG_MAP(GrandCom)
	ON_THREAD_MESSAGE(IDC_RECEIVE,DoRespond)
	ON_THREAD_MESSAGE(TIMER_ALL_TIC,DoRespond)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GrandCom message handlers


void GrandCom::SendBack(char *Message)
{
	TRACE1("GrandCom::SendBack(%s)\n",Message);
	if (m_pReceiver)
	{
		ComData* ReceiveData = (ComData*)malloc(sizeof(ComData));
		ReceiveData->Size = strlen(Message);
		ReceiveData->Data = (char*)malloc(strlen(Message)+1);
		ReceiveData->Data[0] = NULL;
		strcpy(ReceiveData->Data,Message);
		m_pReceiver->PostMessage(IDC_RECEIVE,m_dNode,(LPARAM)ReceiveData);
	}
}


enum {CEND,CUSER,CYES,CNO,CUP,CDOWN,CECHO,CACQUIRE,COFFSET,CNEXT,CNEUT,CNUMB,CERROR,CTICK,C8F,C81,C81_0,C81_1,C80,CQ,C1,C01};

void GrandCom::DoRespond(WPARAM /*Length*/, LPARAM LWord)
{	
	char * Command = (char*)LWord;
TRACE2("GrandCom::DoRespond(%s) State:%d\n",Command,m_dGrandState);
	int iCommand;
	if (m_bMiniGrand)
	{
		if		(strcmp(Command,"END")==0)  iCommand = CEND;
		else if (strcmp(Command,"USER")==0) iCommand = CUSER;
		else if (strcmp(Command,"YES")==0)  iCommand = CYES;
		else if (strcmp(Command,"NO")==0)	iCommand = CNO;
		else if (strcmp(Command,"UP")==0)	iCommand = CUP;
		else if (strcmp(Command,"DOWN")==0) iCommand = CDOWN;
		else if (strcmp(Command,"ECHO")==0) iCommand = CECHO;
		else if (strcmp(Command,"ACQUIRE")==0) iCommand = CACQUIRE;
//		else if (strcmp(Command,"OFFSET")==0) iCommand = C80;
		else if (strcmp(Command,"NEXT")==0) iCommand = CNEXT;
		else if (strcmp(Command,"NEUT")==0) iCommand = CNEUT;
		else if (strcmp(Command,"TICK")==0) iCommand = CTICK;
		else if (strcmp(Command,"8f")==0)	iCommand = C8F;
		else if (strcmp(Command,"81 1")==0) iCommand = C81_1;
		else if (strcmp(Command,"81 0")==0) iCommand = C81_0;
		else if (strcmp(Command,"81")==0)	iCommand = C81;
		else if (strcmp(Command,"80")==0)   iCommand = C80;
		else if (strcmp(Command,"Q")==0)	iCommand = CQ;
		else if (strcmp(Command,"1")==0)	iCommand = C1;
		else if (strstr(Command,"01")==Command)	iCommand = C01;
		else iCommand = CERROR;
		if ((m_dGrandState == 2) && (iCommand != CTICK))
		{
			m_BackedUpCommands += Command;
			m_BackedUpCommands += ",";
		}
		switch (m_dGrandState) {
		case 0://Setup mode
			switch (iCommand) {
			case CEND:
				if (!m_bIn8f)
					SendBack("?? (END)\r");
				break;
			case CUSER:
				if (!m_bIn8f)
				{
					m_dGrandState = 1;
					SendBack("MG Monitor 3.73 2.01\rtpc=0 ic=0\r");
				}
				break;
			case CYES:
				if (!m_bIn8f)
					SendBack("?? (YES)\r");
				break;
			case CNO:
				if (!m_bIn8f)
					SendBack("?? (NO)\r");
				break;
			case CUP:
				if (!m_bIn8f)
					SendBack("?? (UP)\r");
				break;
			case CDOWN:
				if (!m_bIn8f)
					SendBack("?? (DOWN)\r");
				break;
			case CECHO:
				if (!m_bIn8f)
					SendBack("?? (ECHO)\r");
				break;
			case CACQUIRE:
				if (!m_bIn8f)
					SendBack("?? (ACQUIRE)\r");
				break;
			case CNEXT:
				if (!m_bIn8f)
					SendBack("?? (NEXT)\r");
				break;
			case CNEUT:
				if (!m_bIn8f)
					SendBack("?? (NEUT)\r");
				break;
			case CERROR:
				if (!m_bIn8f)
					SendBack("?? ()\r");
				break;
			case CTICK:
				if (m_bIn8f)
				{
					m_iTickCount++;
					if (m_iTickCount >= m_iSetTime)
					{
						m_iTickCount = 0;
						char buff[128];
						sprintf(buff,"A:%9.2f/%9.2f  B:%9.2f/%9.2f  C:%9.2f/%9.2f\r\n",(float)g_dDemoA,0.0,(float)g_dDemoB,0.0,(float)g_dDemoC,0.0);
						SendBack(buff);
						sprintf(buff,"0:%.4e/%.4e  1:%.4e/%.4e\r\n",g_fDemoG1,0.0,g_fDemoG2,0.0);
						SendBack(buff);
					}
				}
				break;
			case C8F:
				if (!m_bIn8f)
				{	
					m_bIn8f = true;
					SendBack("Press Q to terminate acquisitions\r");
					m_iTickCount = 0;
				}
				break;
			case C81:
				if (m_bInInternal)
					SendBack("81 0\r");
				else
					SendBack("81 1\r");
				break;
			case C81_0:
				m_bInInternal = true;
				SendBack("81 0\r");
				break;
			case C81_1:
				m_bInInternal = false;
				SendBack("81 1\r");
				break;
			case C80:
				m_iTickCount = 0;
				m_dGrandState = 2;
				m_szBackedUpCommands[0] = NULL;
				break;
			case CQ:
				if (m_bIn8f)
				{
					m_bIn8f = false;
					SendBack("8f\r");
				}
				else
					SendBack("?? (Q)\r");
				break;
			case C1:
				if (!m_bIn8f)
					SendBack("?? (1)\r");
				break;
			case C01:
				int iCmd;
				sscanf(Command,"%d %d",&iCmd,&m_iSetTime);
				break;
			}
			break;
		case 1://Monitor mode
			switch (iCommand) {
			case CEND:
				SendBack("MG Setup 1.73 2/01\r");
				m_bIn8f = false;
				m_iTickCount = 0;
				m_dGrandState = 0;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CNO:
				break;
			case CUP:
				break;
			case CDOWN:
				break;
			case CECHO:
				SendBack("\rTIME =           UNITS\r");
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			case CTICK:
				break;
			case C8F:
				SendBack("12         0A01F10485761048576  1.0  0  0  0  0   0.0   0.011.94.9411.1-11.52F\r");
				SendBack("131515369600FF   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0 4\r");
				SendBack("1A         0C6   0.0AA4194304419430410485761048576IA4\r");
				SendBack("1B         055A003.73777777700001 96008ND11.94.9411.1-11.50000CF\r");
				SendBack("1C         003.73   107050   100.0   100.0   100.0     100.0     100.0 33.0 31.9\r");
				SendBack("31         001681921510.2000.2000.200  0.020  0.020 0.01001043030 25037\r");
				SendBack("61097\r");
				break;
			case C81_0:
				break;
			case C81_1:
				break;
			case C80:
				break;
			case CQ:
				break;
			case C1:
				SendBack("MG Setup 1.73 2/01\r");
				m_dGrandState = 0;
				m_bIn8f = false;
				m_iTickCount = 0;
				break;
			case C01:
				SendBack("MG Setup 1.73 2/01\r");
				m_dGrandState = 0;
				m_bIn8f = false;
				m_iTickCount = 0;
				break;
			}
			break;
		case 2://taking offsets
			if (iCommand == CTICK)
			{
				m_iTickCount++;
				//if past 20 seconds
				if (m_iTickCount > 20)
				{
					//read from demo.ini what to send back
					int iOffsets[2][12];
					//send message back
					char szMsgBuff[128];

	TRACE1("sending back offset data (%d)\n",m_iTickCount);


					for(int iChan = 0; iChan < 2; iChan ++)
					{
						sprintf(szMsgBuff,"channel %d:\r",iChan);
						SendBack(szMsgBuff);

						int it = (m_bInInternal ? eOFFSET_INTERNAL_CH0 : eOFFSET_REMOTE_CH0);
						it = it + iChan;
						GETPROFSTR((tIniNames)it, szMsgBuff, sizeof(szMsgBuff));
						
						sscanf(szMsgBuff,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
							&iOffsets[iChan][0],&iOffsets[iChan][1],&iOffsets[iChan][2],
							&iOffsets[iChan][3],&iOffsets[iChan][4],&iOffsets[iChan][5],
							&iOffsets[iChan][6],&iOffsets[iChan][7],&iOffsets[iChan][8],
							&iOffsets[iChan][9],&iOffsets[iChan][10],&iOffsets[iChan][11]);
						
						for (int iGainSet = 0; iGainSet < 12; iGainSet = iGainSet + 4)
						{
							sprintf(szMsgBuff,"%3d:%5d%5d:%5d%5d:%5d%5d:%5d\r",
								iGainSet,   iOffsets[iChan][iGainSet],
								iGainSet+1, iOffsets[iChan][iGainSet+1],
								iGainSet+2, iOffsets[iChan][iGainSet+2],
								iGainSet+3, iOffsets[iChan][iGainSet+3]);
							SendBack(szMsgBuff);
						}
					}
					SendBack("80\r");

//channel 0:
//  0: 1003    1: 1005    2: 1014    3: 1051
//  4: 1003    5: 1005    6: 1014    7: 1051
//  8: 1002    9: 1003   10: 1012   11: 1056
//channel 1:
//  0: 1010    1: 1011    2: 1016    3: 1037
//  4: 1010    5: 1011    6: 1016    7: 1037
//  8: 1009    9: 1009   10: 1015   11: 1045
//80					
					//set mode back to setup
					m_dGrandState = 0;
					//post all of the backed up messages out
					char *cAt = strtok(m_szBackedUpCommands,",");
					while (cAt!=NULL)
					{
						SendBack(cAt);
						cAt = strtok(NULL,",");
					}
				}
			}
			break;
		}
	}
	else
	{
		if		(strcmp(Command,"END")==0)  iCommand = CEND;
		else if (strcmp(Command,"USER")==0) iCommand = CUSER;
		else if (strcmp(Command,"YES")==0)  iCommand = CYES;
		else if (strcmp(Command,"NO")==0)	iCommand = CNO;
		else if (strcmp(Command,"UP")==0)	iCommand = CUP;
		else if (strcmp(Command,"DOWN")==0) iCommand = CDOWN;
		else if (strcmp(Command,"ECHO")==0) iCommand = CECHO;
		else if (strcmp(Command,"ACQUIRE")==0) iCommand = CACQUIRE;
		else if (strcmp(Command,"OFFSET")==0) iCommand = COFFSET;
		else if (strcmp(Command,"NEXT")==0) iCommand = CNEXT;
		else if (strcmp(Command,"NEUT")==0) iCommand = CNEUT;
		else if (strcmp(Command,"TICK")==0)  iCommand = CTICK;
		else {
			if (isdigit(Command[0]))
				iCommand = CNUMB;
			else
				iCommand = CERROR;
		}

		switch (m_dGrandState) {
		case 0: //monitor mode screen 2 of 1,2,3
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CNO:
				break;
			case CUP:
				m_dGrandState = 10;
				break;
			case CDOWN:
				m_dGrandState = 20;
				break;
			case CECHO:
				sprintf(m_pEcho,
						//12345678901234567890
						 "TIME = %5d        "
						 "G1=0.000E+1 R UNITS "
						 "G2=0.000E-5 R UNITS "
						 "                   x\r",m_dNumber2);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case COFFSET:
				break;
			case CNEXT:
				m_dGrandState = 2000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			case CTICK:
				m_dNumber1--;
				m_dNumber2++;
				if (m_dNumber1 <= 0)
				{
					m_dNumber1 = m_dNumber0;
					m_dNumber2 = 0;
				}
				break;
			}
			break;
		case 10: //monitor mode screen 1 of 1,2,3
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: //invalid
				break;
			case CDOWN:
				m_dGrandState = 20;
				break;
			case CECHO:
				sprintf(m_pEcho,
						//12345678901234567890
	//					 "TIME = %5d        "
	//					 "A =        0 CTS    "
	//					 "B =        0 CTS    "
	//					 "C =        0 CTS   v\r",m_dNumber2);
						 "TIME = %5d        "
						 "A = %8d CTS    "
						 "B = %8d CTS    "
						 "C = %8d CTS   v\r",
						 m_dNumber1,m_dACnts,m_dBCnts,m_dCCnts);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case COFFSET:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			case CTICK:
				m_dNumber1--;
				m_dNumber2++;
				if (m_dNumber1 <= 0)
				{
					m_dNumber1 = m_dNumber0;
					m_dNumber2 = 0;
				}
				break;
			}
			break;
		case 20: //monitor mode screen 3 of 1,2,3
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 10;
				break;
			case CDOWN:
				break;
			case CECHO:
				Now = COleDateTime::GetCurrentTime();
									//12345678901234567890
				strcpy(m_pEcho,LPCSTR(Now.Format("TIME =    DATE =    "
									 "%H:%M:%S  %y-%m-%d  "
									 "                   ^\r")));
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 1000:
			switch (iCommand) {
			case CEND:
				break;
			case CUSER:
				m_dGrandState = 2000;
				break;
			case CYES:
				break;
			case CUP: //invalid
				break;
			case CDOWN:
				break;
			case CECHO:   
				Now = COleDateTime::GetCurrentTime();
									//12345678901234567890
				strcpy(m_pEcho,LPCSTR(Now.Format(
									 "GRAND UNIT 20       "
									 "%H:%M:%S  %y-%m-%d  "
									 "PUSH KEY CHOICE     \r"
									 "80\r")));
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				m_dGrandState = 3000;
				break;
			case COFFSET:
				m_dGrandState = 4000;
				m_iPromptPosition = 16;
				break;
			case CNEXT:
				break;
			case CNEUT:
				m_dGrandState = 1010;
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 1010:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				m_dGrandState = 2000;
				break;
			case CYES:
				break;
			case CUP: //invalid
				break;
			case CDOWN:
				m_dGrandState = 1020;
				break;
			case CECHO:   
				SendBack("NEUTRONS INPUTS NOW="
						 " EXT A, EXT B,EXT C "
						 "CHANGE INPUTS TO    "
						 "EXT A, EXT B,EXT C ?@\r"
						 "78\r");
				break;
			case CACQUIRE:
				m_dGrandState = 3000;
				break;
			case CNEXT:
				break;
			case CNEUT:
				m_dGrandState = 1010;
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 1020:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				m_dGrandState = 2000;
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 1010;
				break;
			case CDOWN:
				break;
			case CECHO:   
	//			SendBack("NEUTRON COUNTER RATE"
	//					 "A=          0 C/S   "
	//					 "B=          0 C/S   "
	//					 "C=          0 C/S   ^\r");
				sprintf(m_pEcho,
						 "NEUTRON COUNTER RATE"
						 "A=%9.2fC/S   "
						 "B=%11dC/S   "
						 "C=%11dC/S   ^\r",
						 g_dDemoA,g_dDemoB,g_dDemoC);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 2000:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				m_dGrandState = 0;
				break;
			case CUP: //invalid
				break;
			case CDOWN:
				m_dGrandState = 2010;
				break;
			case CECHO:	//12345678901234567890
				SendBack("BEGIN VERSION 1.600 "
						 "GRAND MONITOR?      "
						 "                    "
						 "                   v\r");
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 2010:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 2000;
				break;
			case CDOWN:
				m_dGrandState = 2010;
				break;
			case CECHO:	//12345678901234567890
				SendBack("CHANGE/EXAMINE      "
						 "GRAND MONITOR       "
						 "PARAMETERS?         "
						 "                   x\r");
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 3000:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 2000;
				break;
			case CDOWN:
				m_dGrandState = 2010;
				break;
			case CECHO:	
				sprintf(m_pEcho,
						//12345678901234567890
						 "END WHEN TIME=%5dS"
						 "A = 16777215 CTS    "
						 "B = 16777215 CTS    "
						 "C = 16777215 CTS   n\r",m_dNumber1);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dACnts = 0;
				m_dBCnts = 0;
				m_dCCnts = 0;
				m_dGrandState = 3010;
				break;
			case CNEUT:
				m_dGrandState = 0;
				break;
			case CNUMB:
				//get number of seconds and save it
				m_dNumber0 = atoi(Command);
				m_dNumber1 = m_dNumber0;
				m_dNumber2 = 0;
				break;
			case CERROR:
				break;
			}
			break;

		case 3010:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				break;
			case CDOWN:
				m_dGrandState = 3020;
				break;
			case CECHO:	//12345678901234567890//counting down
				sprintf(m_pEcho,
						//12345678901234567890
						 "TIME = %5d        "
						 "A = %8d CTS    "
						 "B = %8d CTS    "
						 "C = %8d CTS   v\r",
						 m_dNumber1,m_dACnts,m_dBCnts,m_dCCnts);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			case CTICK:
				m_dNumber1--;
				m_dNumber2++;
				m_dACnts += g_dDemoA;
				m_dBCnts += g_dDemoB;
				m_dCCnts += g_dDemoC;
				if (m_dNumber1 <= 0)
					m_dGrandState = 3030;
				break;
			}
			break;

		case 3020:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 3010;
				break;
			case CDOWN:
				break;
			case CECHO:	
				sprintf(m_pEcho,
						//12345678901234567890
						 "TIME = %5d        "
						 "G1=%10.3lER UNITS"
						 "G2=%10.3lER UNITS"
						 "                   ^\r",
						 m_dNumber2, g_fDemoG1, g_fDemoG2);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			case CTICK:
				m_dNumber1--;
				m_dNumber2++;
				m_dACnts += g_dDemoA;
				m_dBCnts += g_dDemoB;
				m_dCCnts += g_dDemoC;
				if (m_dNumber1 <= 0)
					m_dGrandState = 3030;
				break;
			}
			break;
		case 3030:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 3040;
				break;
			case CDOWN:
				break;
			case CECHO:	
						//12345678901234567890
	//			SendBack("NET NEUT RATES (C/S)"
	//					 "A=123456.00 x0.00   "
	//					 "B=     0.00 x0.00   "
	//					 "C=     0.00 x0.00 n^\r");
				sprintf(m_pEcho,
						 "NET NEUT RATES (C/S)"
						 "A=%9.2f6.00 x0.00   "
						 "B=%9.2f6.00 x0.00   "
						 "C=%9.2f6.00 x0.00 n^\r",
						 g_dDemoA,g_dDemoB,g_dDemoC);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dGrandState = 3000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;
		case 3040:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 3050;
				break;
			case CDOWN:
				m_dGrandState = 3030;
				break;
			case CECHO:	
						//12345678901234567890
				SendBack("NET GAMMA REL DOSE  "
						 "G1=0.000E+1 x1.91E-5"
						 "G2=0.000E+1 x1.91E-5"
						 "                  nx\r");
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dGrandState = 3000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;
		case 3050:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 3060;
				break;
			case CDOWN:
				m_dGrandState = 3040;
				break;
			case CECHO:	
						//12345678901234567890
				SendBack("NEUT BKGND SUBTRACT "
						 "A=        0 x0.00   "
						 "B=        0 x0.00   "
						 "C=        0 x0.00 nx\r");
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dGrandState = 3000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 3060:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 3070;
				break;
			case CDOWN:
				m_dGrandState = 3050;
				break;
			case CECHO:	
						//12345678901234567890
				sprintf(m_pEcho,
						 "GROSS GAMMAS:       "
	//					 "G1=0.000E+1 X1.91E-5"
	//					 "G2=0.000E+1 X1.91e-5"
	//					 "                  nx\r");
						 "G1=%10.3lE .00E+0"
						 "G2=%10.3lE .00E+0"
						 "                  nx\r",
						 g_fDemoG1, g_fDemoG2);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dGrandState = 3000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 3070:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CUP: 
				m_dGrandState = 3080;
				break;
			case CDOWN:
				m_dGrandState = 3060;
				break;
			case CECHO:	
						//12345678901234567890
				sprintf(m_pEcho,
	//			SendBack("GROSS NEUTRON COUNTS"
	//					 "A=123456789 X0.00   "
	//					 "B=        0 X0.00   "
	//					 "C=        0 X0.00 nx\r");
						 "GROSS NEUTRON COUNTS"
						 "A=%9d X0.00   "
						 "B=%9d X0.00   "
						 "C=%9d X0.00 nx\r",
						 m_dACnts,m_dBCnts,m_dCCnts);
				SendBack(m_pEcho);
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dGrandState = 3000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;
		case 3080:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				break;
			case CNO:
				break;
			case CUP: 
				m_dGrandState = 3080;
				break;
			case CDOWN:
				m_dGrandState = 3070;
				break;
			case CECHO:	
						//12345678901234567890
				SendBack("EXT A, EXT B,EXT C  "
						 "TIME =    20 S      "
						 "NEUT BD ID=         "
						 "                  nx\r");
				break;
			case CACQUIRE:
				break;
			case CNEXT:
				m_dGrandState = 3000;
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CERROR:
				break;
			}
			break;

		case 4000:
			switch (iCommand) {
			case CEND:
				m_dGrandState = 1000;
				break;
			case CUSER:
				break;
			case CYES:
				if (m_iPromptPosition==16)
				{
					m_dGrandState = 4010;//do the take offsets
					strcpy(m_szMsg1, "G GAIN   G1   G2    ");
					strcpy(m_szMsg2, "                    ");
					strcpy(m_szMsg3, "                    ");
					strcpy(m_szMsg4, "                   ^\r");
					m_iTickCount = 0;
				}
				else
				{
					m_dGrandState = 4020;//show offsets
					m_iOffsetPage = 0;
				}
				break;
			case CNO:
				if (m_iPromptPosition==16)
					m_iPromptPosition=57;
				else
					m_iPromptPosition=16;
				break;
			case CECHO:
				SendBack("TAKE NEW OFFSETS?   "
						 "                    "
						 "SHOW LAST OFFSETS?  "
						 "                   @\r");
				if (m_iPromptPosition == 16)
					SendBack("16\r");
				else
					SendBack("57\r");
				 
				break;
			case CACQUIRE:
				break;
			case CNEUT:
				break;
			case CNUMB:
				break;
			case CDOWN:
				m_dGrandState = 4030;//internal or remote
				if (m_bInInternal) 
				{
					strcpy(m_szMsg1,"DISCONNECT=INTERNAL ");
					strcpy(m_szMsg2,"USE INTERNAL?       ");
				}
				else
				{
					strcpy(m_szMsg1,"DISCONNECT=REMOTE   ");
					strcpy(m_szMsg2,"USE REMOTE  ?       ");
				}
				break;
			case CERROR:
				break;
			}
			break;

		case 4010://do the take offsets
			switch (iCommand) {
			case CECHO:
				{
					char szMsg[128];
					strcpy(szMsg,m_szMsg1);
					strcat(szMsg,m_szMsg2);
					strcat(szMsg,m_szMsg3);
					strcat(szMsg,m_szMsg4);
					SendBack(szMsg);
					SendBack("80\r");
				}
				break;

			case CTICK:
				if (m_iTickCount == 0)
				{
					char szMsgBuff[1024];	
					for (int iChan = 0; iChan < 2; iChan++)
					{

						int it = (m_bInInternal ? eOFFSET_INTERNAL_CH0 : eOFFSET_REMOTE_CH0);
						it = it + iChan;
						GETPROFSTR((tIniNames)it, szMsgBuff, sizeof(szMsgBuff));

						sscanf(szMsgBuff,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
							&m_iOffsets[iChan][0],&m_iOffsets[iChan][1],&m_iOffsets[iChan][2],
							&m_iOffsets[iChan][3],&m_iOffsets[iChan][4],&m_iOffsets[iChan][5],
							&m_iOffsets[iChan][6],&m_iOffsets[iChan][7],&m_iOffsets[iChan][8],
							&m_iOffsets[iChan][9],&m_iOffsets[iChan][10],&m_iOffsets[iChan][11]);
					}
				}
				m_iTickCount++;
				if (m_iTickCount > 12)
					m_dGrandState = 4000;

				if (m_iTickCount == 1)
					sprintf(m_szMsg2,"      1 %5d %5d  ",  m_iOffsets[0][0],m_iOffsets[1][0]);
				else if (m_iTickCount == 2)
					sprintf(m_szMsg3,"      4 %5d %5d  ",  m_iOffsets[0][1],m_iOffsets[1][1]);
				else if (m_iTickCount == 3)
					sprintf(m_szMsg4,"     16 %5d %5d @\r",m_iOffsets[0][2],m_iOffsets[1][2]);
				else if (m_iTickCount == 4)
					sprintf(m_szMsg2,"     64 %5d %5d  ",  m_iOffsets[0][3],m_iOffsets[1][3]);
				else if (m_iTickCount == 5)
					sprintf(m_szMsg3,"    256 %5d %5d  ",  m_iOffsets[0][4],m_iOffsets[1][4]);
				else if (m_iTickCount == 6)
					sprintf(m_szMsg4,"   1024 %5d %5d ]\r",m_iOffsets[0][5],m_iOffsets[1][5]);
				else if (m_iTickCount == 7)
					sprintf(m_szMsg2,"   4096 %5d %5d  ",  m_iOffsets[0][6],m_iOffsets[1][6]);
				else if (m_iTickCount == 8)
					sprintf(m_szMsg3,"  16384 %5d %5d  ",  m_iOffsets[0][7],m_iOffsets[1][7]);
				else if (m_iTickCount == 9)
					sprintf(m_szMsg4,"  65536 %5d %5d ]\r",m_iOffsets[0][8],m_iOffsets[1][8]);
				else if (m_iTickCount == 10)
					sprintf(m_szMsg2," 262144 %5d %5d  ",  m_iOffsets[0][9],m_iOffsets[1][9]);
				else if (m_iTickCount == 11)
					sprintf(m_szMsg3,"1048576 %5d %5d  ",  m_iOffsets[0][10],m_iOffsets[1][10]);
				else if (m_iTickCount == 12)
					sprintf(m_szMsg4,"4194304 %5d %5d ]\r",m_iOffsets[0][11],m_iOffsets[1][11]);
				break;
			}
			break;

		case 4020://show offsets
			switch (iCommand) {
			case CECHO:
				{
					char szMsg[128];
					if (m_iOffsetPage == 0)
					{
						strcpy(m_szMsg1, "G GAIN   G1   G2    ");
						sprintf(m_szMsg2,"      1 %5d %5d  ",  m_iOffsets[0][0],m_iOffsets[1][0]);
						sprintf(m_szMsg3,"      4 %5d %5d  ",  m_iOffsets[0][1],m_iOffsets[1][1]);
						sprintf(m_szMsg4,"     16 %5d %5d @\r",m_iOffsets[0][2],m_iOffsets[1][2]);
					}
					else if (m_iOffsetPage == 1)
					{
						strcpy(m_szMsg1, "G GAIN   G1   G2    ");
						sprintf(m_szMsg2,"     64 %5d %5d  ",  m_iOffsets[0][3],m_iOffsets[1][3]);
						sprintf(m_szMsg3,"    256 %5d %5d  ",  m_iOffsets[0][4],m_iOffsets[1][4]);
						sprintf(m_szMsg4,"   1024 %5d %5d ]\r",m_iOffsets[0][5],m_iOffsets[1][5]);
					}
					else if (m_iOffsetPage == 2)
					{
						strcpy(m_szMsg1, "G GAIN   G1   G2    ");
						sprintf(m_szMsg2,"   4096 %5d %5d  ",  m_iOffsets[0][6],m_iOffsets[1][6]);
						sprintf(m_szMsg3,"  16384 %5d %5d  ",  m_iOffsets[0][7],m_iOffsets[1][7]);
						sprintf(m_szMsg4,"  65536 %5d %5d ]\r",m_iOffsets[0][8],m_iOffsets[1][8]);
					}
					else if (m_iOffsetPage == 3)
					{
						strcpy(m_szMsg1, "G GAIN   G1   G2    ");
						sprintf(m_szMsg2," 262144 %5d %5d  ",  m_iOffsets[0][9],m_iOffsets[1][9]);
						sprintf(m_szMsg3,"1048576 %5d %5d  ",  m_iOffsets[0][10],m_iOffsets[1][10]);
						sprintf(m_szMsg4,"4194304 %5d %5d ]\r",m_iOffsets[0][11],m_iOffsets[1][11]);
					}
					else if (m_iOffsetPage == 4)
					{
						strcpy(m_szMsg1,"UNIT 20 OFFSET      ");
						strcpy(m_szMsg2,"12:36:12  101-08-24 ");
						strcpy(m_szMsg3,"                    ");
						strcpy(m_szMsg4,"                   ^\r");
					}
					strcpy(szMsg,m_szMsg1);
					strcat(szMsg,m_szMsg2);
					strcat(szMsg,m_szMsg3);
					strcat(szMsg,m_szMsg4);
					SendBack(szMsg);
					SendBack("80\r");
				}
				break;

			case CDOWN:
				m_iOffsetPage++;
				if (m_iOffsetPage > 4)
					m_iOffsetPage = 4;
				break;
			case CUP:
				m_iOffsetPage--;
				if (m_iOffsetPage < 0) 
					m_iOffsetPage = 0;
				break;
			case CEND:
				m_dGrandState = 1000;
				break;
			}
			break;
		case 4030://ask disconnect=REMOTE or INTERNAL
			switch (iCommand) {
			case CECHO:
				{
					char szMsg[128];
					strcpy(szMsg,m_szMsg1);
					strcat(szMsg,m_szMsg2);
					strcat(szMsg,"                    ");
					strcat(szMsg,"                   ^\r");
					SendBack(szMsg);
					SendBack("32\r");
				}
				break;

			case CYES:
				if (strstr(m_szMsg2,"INTERNAL"))
				{
					strcpy(m_szMsg1,"DISCONNECT=INTERNAL ");
					m_bInInternal = true;
				}
				else
				{
					strcpy(m_szMsg1,"DISCONNECT=REMOTE   ");
					m_bInInternal = false;
				}
				break;
			case CNO:
				if (strstr(m_szMsg2,"INTERNAL"))
					strcpy(m_szMsg2,"USE REMOTE  ?");
				else
					strcpy(m_szMsg2,"USE INTERNAL?");
				break;
			case CDOWN:
				break;
			case CUP:
				m_dGrandState = 4000;
				break;
			case CEND:
				m_dGrandState = 1000;
				break;
			}
			break;

		}
	}
	free(Command); // The callers of DoRespond ALL use malloc
}
