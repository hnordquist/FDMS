//GraphDataSet.h - Version 3.0 

// GraphDataSet.cpp : implementation file
//

#include "stdafx.h"
#include "GraphDataSet.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet

CGraphDataSet::CGraphDataSet()
{
	xPosition = 0;
	xdPosition = 0.0;
	ydValue = 0.0;
	yValue = 0;
	bInteger = true;
}

CGraphDataSet::~CGraphDataSet()
{
}


BEGIN_MESSAGE_MAP(CGraphDataSet, CStatic)
	//{{AFX_MSG_MAP(CGraphDataSet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet message handlers

void CGraphDataSet::SetXPosition(int x)
{
	xPosition = x;
	xdPosition = (double)x;
}

void CGraphDataSet::SetYValue(int y)
{
	yValue = y;
	ydValue = (double)y;
}

void CGraphDataSet::SetXPosition(double x)
{
	xPosition = (int)x;
	xdPosition = x;
}

void CGraphDataSet::SetYValue(double y)
{
	yValue = (int)y;
	ydValue = y;
}

int CGraphDataSet::GetXData()
{
	return xPosition;
}

int CGraphDataSet::GetYData()
{
	return yValue;
}

double CGraphDataSet::GetdXData()
{
	return xdPosition;
}

double CGraphDataSet::GetdYData()
{
	return ydValue;
}
