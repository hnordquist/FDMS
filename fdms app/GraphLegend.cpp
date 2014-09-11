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
//GraphLegend.cpp - Version 3.0

#include "stdafx.h"
#include "GraphLegend.h"
#include "GraphLegendSet.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend

CGraphLegend::CGraphLegend()
{
	legendLabels = new CObList();
	legendLength = 0;
	m_iCount = 0;
}

CGraphLegend::~CGraphLegend()
{
  for( POSITION pos = legendLabels->GetHeadPosition(); pos != NULL; )
  {
    delete (CGraphLegendSet*)legendLabels->GetNext(pos);
  }
  legendLabels->RemoveAll();
  delete legendLabels;
}


/*
CGraphLegend::~CGraphLegend()
{
	POSITION pos;
	CGraphLegendSet *legendSet;
	pos = legendLabels->GetHeadPosition();
	while(pos != NULL)
	{
		legendSet = (CGraphLegendSet*)legendLabels->GetAt(pos);
		legendLabels->RemoveAt(pos);
		delete legendSet;
	}
	delete legendLabels;
}
*/


BEGIN_MESSAGE_MAP(CGraphLegend, CStatic)
	//{{AFX_MSG_MAP(CGraphLegend)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend message handlers

void CGraphLegend::SetLegendText(int datagroup, CString label, COLORREF cColor, int iStyle, bool bShow)
{
	m_iCount++;
	CGraphLegendSet *legendData = new CGraphLegendSet();
	legendData->SetLegendLabel(datagroup, label, cColor, iStyle, bShow);
	legendLabels->AddTail(legendData);
	if(label.GetLength() > legendLength)
		legendLength = label.GetLength();
}

CString CGraphLegend::GetLegendText(int datagroup)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if((legendLabels->GetCount() <= datagroup) || (legendLabels->GetCount() == 0))
		return "";
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < datagroup; i++)
		legendLabels->GetNext(pos);
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	return legendData->GetLegendLabel();
}

COLORREF CGraphLegend::GetLegendColor(int datagroup)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if((legendLabels->GetCount() <= datagroup) || (legendLabels->GetCount() == 0))
		return NULL;
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < datagroup; i++)
		legendLabels->GetNext(pos);
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	return legendData->GetLegendColor();
}

bool CGraphLegend::GetLegendDoDraw(int datagroup)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if ((legendLabels->GetCount() <= datagroup) || (legendLabels->GetCount() == 0))
		return false;
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < datagroup; i++)
		legendLabels->GetNext(pos);
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	return legendData->GetDoShow();
}

void CGraphLegend::SetLegendDoDraw(int datagroup, bool bDoDraw)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if ((legendLabels->GetCount() <= datagroup) || (legendLabels->GetCount() == 0))
		return;
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < datagroup; i++)
		legendLabels->GetNext(pos);
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	legendData->SetDoShow(bDoDraw);
}

void CGraphLegend::ToggleLegendDoDraw(int datagroup)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if ((legendLabels->GetCount() <= datagroup) || (legendLabels->GetCount() == 0))
		return;
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < datagroup; i++)
		legendLabels->GetNext(pos);
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	legendData->ToggleDoShow();
}

int CGraphLegend::GetLegendStyle(int datagroup)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if((legendLabels->GetCount() <= datagroup) || (legendLabels->GetCount() == 0))
		return 0;
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < datagroup; i++)
		legendLabels->GetNext(pos);
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	return legendData->GetLegendStyle();
}

int CGraphLegend::GetLegendLength()
{
	return legendLength;
}

void CGraphLegend::Remove(int dataGroup)
{
	POSITION pos;
	CGraphLegendSet *legendSet;
	legendSet = NULL;
	pos = legendLabels->GetHeadPosition();
	for(int i = 0; i < dataGroup; i++)
	{
		legendSet = (CGraphLegendSet*)legendLabels->GetAt(pos);
		legendLabels->GetNext(pos);
	}
	legendLabels->RemoveAt(pos);
	delete legendSet;
	m_iCount--;
}

void CGraphLegend::RemoveAll()
{
	POSITION pos;
	CGraphLegendSet *legendSet;
	pos = legendLabels->GetHeadPosition();
	while(pos != NULL)
	{
		legendSet = (CGraphLegendSet*)legendLabels->GetAt(pos);
		legendLabels->RemoveAt(pos);
		delete legendSet;
		pos = legendLabels->GetHeadPosition();
	}
	m_iCount = 0;
}
