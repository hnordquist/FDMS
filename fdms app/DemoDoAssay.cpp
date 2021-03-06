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
�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
�	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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
// $History: DemoDoAssay.cpp $
// 
// *****************  Version 3  *****************
// User: Longo        Date: 12/21/04   Time: 4:13p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/10/04   Time: 6:23p
// Updated in $/FDMS/FDMS Application/FDMSNET
// merged demo and live works now, checkpoint for code
// 
// *****************  Version 1  *****************
// User: Longo        Date: 12/10/04   Time: 4:48p
// Created in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FDMS.h"
#include "DoAssay.h"

#include "dconfig.h"

#include ".\DemoDoAssay.h"

#include ".\GrandCom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define BACKGROUND_SYM			"BACKGROUND"
#define PASSIVE_SYM			"PASSIVE"
#define BASELINE			"BASELINE"
#define REVERIFY			"REVERIFY"

IMPLEMENT_DYNAMIC(DemoDoAssay, CDoAssay)


DemoDoAssay::DemoDoAssay(CWnd* pParent /*=NULL*/)
: CDoAssay(pParent)
{
	m_cMode[0] = NULL;
}


DemoDoAssay::~DemoDoAssay()
{
}

void DemoDoAssay::CreateTheThread()
{
		m_pGrandCom = (GrandCom*)AfxBeginThread(RUNTIME_CLASS( GrandCom));
}

BEGIN_MESSAGE_MAP(DemoDoAssay, CDoAssay)
END_MESSAGE_MAP()

void DemoDoAssay::SetModeForUpdateParams()
{
	switch (m_iMode) {
	case eBackgroundAssay:
		strcpy(m_cMode,BACKGROUND_SYM);
		break;
	case eNormalAssay:
		strcpy(m_cMode,PASSIVE_SYM);
		break;
	case eBaselineAssay:
		strcpy(m_cMode,BASELINE);
		break;
	case eReVerificationAssay:
		strcpy(m_cMode,REVERIFY);
		break;
	}
}

void
DemoDoAssay::UpdateParams(char* Phase)
{

	TRACE("DemoDoAssay::UpdateParams(%s), m_cMode:%s m_dAssayIndex:%d\n",Phase,m_cMode,m_dAssayIndex);

	if (m_dAssayIndex < 0)
	{

		char temp[64];
		char szTemp[64];
		/*!*/GetPrivateProfileString(m_cMode,Phase,"",temp,sizeof(temp),CIniFileProcessor::DefaultSourceFullPath);
		if ((m_iMode == eNormalAssay)||(m_iMode == eBaselineAssay)||(m_iMode == eReVerificationAssay))
		{
			if (strlen(temp) == 0)
			{
				sprintf(szTemp,"%s_%d",Phase,g_iDemoCount);
				/*!*/GetPrivateProfileString(m_cMode,szTemp,"",temp,sizeof(temp),CIniFileProcessor::DefaultSourceFullPath);
				if (strlen(temp)==0)
				{
					g_iDemoCount = 1;
					sprintf(szTemp,"%s_%d",Phase,g_iDemoCount);
					/*!*/GetPrivateProfileString(m_cMode,szTemp,"",temp,sizeof(temp),CIniFileProcessor::DefaultSourceFullPath);
				}
			}
		}

		if (strlen(temp)==0)
			strcpy(temp,"0,0,0,0.0,0.0,0.0,0.0,0.0,0.0");

		sscanf(temp,"%9.2f,9.2f,9.2f,%lf,%lf,%lf,%lf,%lf,%lf",
			&g_dDemoA,
			&g_dDemoB,
			&g_dDemoC,
			&g_fDemoG1,
			&g_fDemoG2,
			&g_fDemoSingles,
			&g_fDemoSinglesError,
			&g_fDemoDoubles,
			&g_fDemoDoublesError);
	}
	else
	{
		// demo mode, might be background
		if (m_bDoingBackground)
		{
			char szString[128];
			GETPROFSTR(eBACKGROUND_A,szString,sizeof(szString));
			g_dDemoA = (float)atof(szString);
			GETPROFSTR(eBACKGROUND_B,szString,sizeof(szString));
			g_dDemoB = (float)atof(szString);
			GETPROFSTR(eBACKGROUND_C,szString,sizeof(szString));
			g_dDemoC = (float)atof(szString);
			GETPROFSTR(eBACKGROUND_1,szString,sizeof(szString));
			g_fDemoG1 = (float)atof(szString);
			GETPROFSTR(eBACKGROUND_2,szString,sizeof(szString));
			g_fDemoG2 = (float)atof(szString);
		}
		else
		{
			g_dDemoA			= g_fMPNChanA[m_dAssayIndex];
			g_dDemoB			= g_fMPNChanB[m_dAssayIndex];
			g_dDemoC			= g_fMPNChanC[m_dAssayIndex];
			g_fDemoG1			= g_fMPGDose1[m_dAssayIndex];
			g_fDemoG2			= g_fMPGDose2[m_dAssayIndex];
		}
		g_fDemoSingles		= 0.0;
		g_fDemoSinglesError	= 0.0;
		g_fDemoDoubles		= 0.0;
		g_fDemoDoublesError	= 0.0;
	}
}
