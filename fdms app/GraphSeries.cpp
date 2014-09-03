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
