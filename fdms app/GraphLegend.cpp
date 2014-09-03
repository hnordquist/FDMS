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
