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
//GraphSeries.cpp - Version 3.0

#include "stdafx.h"
#include "GraphSeries.h"
#include "GraphDataSet.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries

CGraphSeries::CGraphSeries()
{
	dataValues = new CObList();
	bIsInteger = true;
	m_iStyle = 0;
	m_iLineStyle = PS_SOLID;
	m_iLineWidth = 1;
	m_bDoDisplay = true;
	m_iGroup = 0;
	seriesLabel = "";
	m_bClip = false;
	m_bSelectable = true;
}
CGraphSeries::CGraphSeries(int iStyle, COLORREF cColor, bool bDoLines, int iGroup)
{
	dataValues = new CObList();
	bIsInteger = true;
	m_iStyle = iStyle;
	m_iLineWidth = 1;
	m_iLineStyle = PS_SOLID;
	m_cColor = cColor;
	m_bDoLines = bDoLines;
	m_iGroup = iGroup;
	m_bDoDisplay = true;
	seriesLabel = "";
	m_bClip = false;
	m_bSelectable = true;
}

CGraphSeries::~CGraphSeries()
{
	for(POSITION pos = dataValues->GetHeadPosition(); pos != NULL; )
	    delete (CGraphDataSet*)dataValues->GetNext(pos);
	dataValues->RemoveAll();
	delete dataValues;
}

BEGIN_MESSAGE_MAP(CGraphSeries, CStatic)
	//{{AFX_MSG_MAP(CGraphSeries)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries message handlers

void CGraphSeries::SetLabel(CString label)
{
	seriesLabel = label;
}

void CGraphSeries::SetData(int xValue, int yValue)
{
	CGraphDataSet *dataSet = new CGraphDataSet();
	if (!bIsInteger)
		dataSet->SetDouble();
	dataSet->SetXPosition(xValue);
	dataSet->SetYValue(yValue);
	dataValues->AddTail(dataSet);
}

void CGraphSeries::SetData(double xValue, double yValue)
{
	CGraphDataSet *dataSet = new CGraphDataSet();
	if (!bIsInteger)
		dataSet->SetDouble();
	dataSet->SetXPosition(xValue);
	dataSet->SetYValue(yValue);
	dataValues->AddTail(dataSet);
}

CString CGraphSeries::GetLabel()
{
	return seriesLabel;
}

int CGraphSeries::GetData(int xValue)
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	int retVal = 0;
	while(pos != NULL)
	{
		CGraphDataSet *dataSet;
		dataSet = (CGraphDataSet*)dataValues->GetAt(pos);
		if(dataSet->GetXData() == xValue)
		{
			retVal = dataSet->GetYData();
			return retVal;
		}
		dataValues->GetNext(pos);
	}
//	return 0;
	return -1;
}

double CGraphSeries::GetData(double xValue)
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	double retVal = 0.0;
	while(pos != NULL)
	{
		CGraphDataSet *dataSet;
		dataSet = (CGraphDataSet*)dataValues->GetAt(pos);
		if (dataSet->GetdXData() == xValue)
		{
			retVal = dataSet->GetdYData();
			return retVal;
		}
		dataValues->GetNext(pos);
	}
//	return 0.0;
	return -1.0;
}

int CGraphSeries::GetDataCount()
{
	m_pIterateStart = dataValues->GetHeadPosition();
	return dataValues->GetCount();
}

bool CGraphSeries::IterateGet(double *xValue, double *yValue)
{
	if (m_pIterateStart != NULL)
	{
		CGraphDataSet *dataSet;
		dataSet = (CGraphDataSet*)dataValues->GetAt(m_pIterateStart);
		*xValue = dataSet->GetdXData();
		*yValue = dataSet->GetdYData();
		dataValues->GetNext(m_pIterateStart);
		return true;
	}
	return false;
}

int CGraphSeries::GetXDataValue()
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	CGraphDataSet *retVal;
	retVal = (CGraphDataSet*)dataValues->GetAt(pos);

	return retVal->GetXData();
}

double CGraphSeries::GetdXDataValue()
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	CGraphDataSet *retVal;
	retVal = (CGraphDataSet*)dataValues->GetAt(pos);

	return retVal->GetdXData();
}

int CGraphSeries::GetYDataValue()
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	CGraphDataSet *retVal;
	retVal = (CGraphDataSet*)dataValues->GetAt(pos);

	return retVal->GetYData();
}

double CGraphSeries::GetdYDataValue()
{
	POSITION pos;
	pos = dataValues->GetHeadPosition();
	CGraphDataSet *retVal;
	retVal = (CGraphDataSet*)dataValues->GetAt(pos);

	return retVal->GetdYData();
}
