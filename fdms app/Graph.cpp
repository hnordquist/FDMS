//Graph.cpp - Version 3.0 (Brian Convery, May, 2001)
// $History: Graph.cpp $ 
// 
// *****************  Version 8  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 7  *****************
// User: Longo        Date: 12/18/07   Time: 12:32p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 11/14/05   Time: 3:43p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 5  *****************
// User: Longo        Date: 12/10/04   Time: 4:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// warnign removal
// 
// *****************  Version 4  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/24/04    Time: 3:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// bitmap deallocation error fixed

#include "stdafx.h"
#include "afxtempl.h"
#include "GraphSeries.h"
#include "GraphLegend.h"
#include "math.h"
#include "Graph.h"
#include "GraphDataColor.h"
#include "GraphPieLabel.h"	//for pie labels

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraph

CGraph::CGraph()
{
	TRACE0("CGraph::CGraph\n");

	graphType = 0;
	graphQuadType = 1;
	quadSetManually = FALSE;

	FinishInit();
}

CGraph::CGraph(int type)
{
	TRACE1("CGraph::CGraph %d\n",type);

	colorList = new CObList();
	graphType = type;
	if(type != BAR_GRAPH)
		quadSetManually = TRUE;
	else
		quadSetManually = FALSE;


	FinishInit();
}

void CGraph::FinishInit()
{
	TRACE0("CGraph::FinishInit()\n");
	m_bInDraw = false;

	graphSeries = new CObList();
	graphRSeries = new CObList();
	seriesSize = 0;
	minXTick = 0;
	minYTick = 0;
	maxXTick = 100;
	maxYTick = 100;
	maxYRTick = 100;
	numXTicks = 10;
	numYTicks = 10;
	tickXSpace = 10;
	tickYSpace = 10;
	tickYRSpace = 10;
	m_iXTextLines = 1;

	m_bSelectedData = false;

	graphHasLegend = FALSE;
	legendWidth = 0;
	legendMaxText= 0;
	graphAlignment = VERTICAL_ALIGN;  

	xAxisAlign = 0;  //in degrees == horizontal
	xAxisLabelLength = 0;

	xTickFontSize = 12;
	yTickFontSize = 12;
	legendFontSize = 12;

	depth = 0;
	depthRatio = 0.05;

	//	line3DXBase = 0;
	//	line3DYBase = 0;

	SetGridLines(FALSE);
	iXLNumCycles = 1;
	iYLNumCycles = 1;
	iXNLNumCycles = 1;
	iYNLNumCycles = 1;

	graphQuadType = 1;
	bXAxisLinear = true;
	bYAxisLinear = true;
	iMarkerSize = 2;

	bHasLegendInfo = false;
	m_bHasRightAxis = false;

	m_cColorTable[0] = RGB(128, 128, 128);     // Grey
	m_cColorTable[1] = RGB(  0,   0,   0);     // Black
	m_cColorTable[2] = RGB(  0, 255,   0);     // Green
	m_cColorTable[3] = RGB(  0,   0, 255);     // Blue
	m_cColorTable[4] = RGB(255,   0, 255);     // Magenta
	m_cColorTable[5] = RGB(  0, 255, 255);     // Cyan
	m_cColorTable[6] = RGB(255, 255,   0);     // Yellow            
	m_cColorTable[7] = RGB(128, 128,   0);     // Brown
	m_cColorTable[8] = RGB(128,   0, 128);     // Purple
	m_cColorTable[9] = RGB(255,   0,   0);     // Red
}


CGraph::~CGraph()
{
	TRACE0("CGraph::~CGraph()\n");

	POSITION pos;
	for(pos = graphSeries->GetHeadPosition(); pos != NULL; )
	{
		try
		{
			TRACE0("CGraph::~CGraph() delete graphSeries->GetNext\n");
			delete (CGraphSeries*) graphSeries->GetNext( pos );
		}
		catch(...)
		{
		}
	}
	graphSeries->RemoveAll();
	try
	{
		TRACE0("CGraph::~CGraph() delete graphSeries\n");
		delete graphSeries;
	}
	catch(...)
	{
	}

	for(pos = graphRSeries->GetHeadPosition(); pos != NULL;)
	{
		try
		{
			TRACE0("CGraph::~CGraph() delete graphRSeries->GetNext\n");
			delete (CGraphSeries*) graphRSeries->GetNext( pos);
		}
		catch(...)
		{
		}
	}
	try
	{
		TRACE0("CGraph::~CGraph() graphRSeries->RemoveAll\n");
		graphRSeries->RemoveAll();
	}
	catch(...)
	{
	}
	try
	{
		TRACE0("CGraph::~CGraph() delete graphRSeries\n");
		delete graphRSeries;
	}
	catch(...)
	{
	}

	for( pos = colorList->GetHeadPosition(); pos != NULL; )
	{
		try
		{
			TRACE0("CGraph::~CGraph() delete colorList->GetNext\n");
			delete (CGraphDataColor*) colorList->GetNext( pos );
		}
		catch(...)
		{
		}
	}

	try
	{
		colorList->RemoveAll();
		TRACE0("CGraph::~CGraph() colorList->RemoveAll()\n");
	}
	catch(...)
	{
	}

	try 
	{
		delete colorList;
		TRACE0("CGraph::~CGraph() delete colorList\n");
	}
	catch(...)
	{
	}

	try
	{
		graphLegend.RemoveAll();
		TRACE0("CGraph::~CGraph() graphLegend.RemoveAll()\n");
	}
	catch(...)
	{
	}
	//	try
	//	{
	//		delete graphLegend;
	//		TRACE0("CGraph::~CGraph() delete GraphLegend\n");
	//	}
	//	catch(...)
	//	{
	//	}


	TRACE0("CGraph::~CGraph() end\n");
}

BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers

void CGraph::SetGraphType(int gType)
{
	TRACE0("CGraph::SetGraphType\n");

	graphType = gType;
}

void CGraph::SetXAxisAlignment(int alignValue)
{
	TRACE1("CGraph::SetXAxisAlignment\n",alignValue);

	xAxisAlign = alignValue;
}

int CGraph::GetXAxisAlignment()
{
	TRACE1("CGraph::GetXAxisAlignment() %d\n",xAxisAlign);

	return xAxisAlign;
}

void CGraph::SetXAxisTextLines(int iLines)
{
	TRACE1("CGraph::SetXAxisTextLines(%d)\n",iLines);

	if (iLines < 1 ) iLines = 1;
	m_iXTextLines = iLines;
}

void CGraph::SetColor(int dataGroup, COLORREF groupColor)
{
	TRACE2("CGraph::SetColor( %d, %d)\n", dataGroup, groupColor);

	CGraphDataColor *dataColor = new CGraphDataColor(dataGroup, groupColor);
	colorList->AddTail(dataColor);
}

COLORREF CGraph::GetColor(int dataGroup)
{
	TRACE1("CGraph::GetColor(%d)\n",dataGroup);

	POSITION pos;
	if((colorList->GetCount() <= dataGroup) || (colorList->GetCount() == 0))
		return BLACK;

	CGraphDataColor* dataColor;
	pos = colorList->GetHeadPosition();
	for(int i = 0; i < dataGroup; i++)
		colorList->GetNext(pos);

	dataColor = (CGraphDataColor*) colorList->GetAt( pos );
	return dataColor->GetColor();	
}

void CGraph::SetLinear(bool bXAxis, bool bYAxis) 
{
	TRACE2("CGraph::SetLinear(%d, %d)\n", bXAxis, bYAxis);

	switch (graphType) 
	{
	case BAR_GRAPH:
	case SCATTER_GRAPH:
		bXAxisLinear = bXAxis;
		break;
	case LINE_GRAPH:
		bXAxisLinear = bXAxis;
		break;
	case PIE_GRAPH:
		bXAxisLinear = true;
		break;
	}

	bYAxisLinear = bYAxis;
}

void CGraph::DrawGraph(CDC* pDCFinal)
{
	TRACE0("CGraph::DrawGraph\n");

	if (m_bInDraw)
	{
		TRACE("CGraph::DrawGraph shortstop\n");
		return;
	}
	m_bInDraw = true;

	if (pDCFinal->IsPrinting())
	{
		int iWidth = pDCFinal->GetDeviceCaps(HORZRES);
		int iHeight = pDCFinal->GetDeviceCaps(VERTRES);
		graphRect.SetRect(0,0,iWidth,iHeight);
		pDCFinal->SetBkMode(TRANSPARENT);
	}
	else
	{
		CWnd* graphWnd = pDCFinal->GetWindow();
		if (graphWnd)
			graphWnd->GetClientRect(&graphRect);
		else
			graphRect=CRect(0,0,1024,768);
	}

	CDC* pDC;
	CDC dcMemory;
	CBitmap *oldBM = NULL;
	CBitmap cBitmap;
	if (!pDCFinal->IsPrinting())
	{
		pDC = &dcMemory;
		dcMemory.CreateCompatibleDC(pDCFinal);
		cBitmap.CreateCompatibleBitmap(pDCFinal, graphRect.Width(), graphRect.Height());
		oldBM = (CBitmap*)pDC->SelectObject(&cBitmap);
	}
	else
		pDC = pDCFinal;
	pDC->SetBkMode(TRANSPARENT);

	CString tickLabel;

	//reset graph to be clear background
	COLORREF backColor;
	backColor = RGB(255,255,255);  //replace with desired background color
	CBrush backBrush (backColor);
	CBrush* pOldBackBrush;
	pOldBackBrush = pDC->SelectObject(&backBrush);
	pDC->Rectangle(0, 0, graphRect.Width(), graphRect.Height());
	pDC->SelectObject(pOldBackBrush);

	maxHeight = (int)((double)graphRect.Height()*98.0/100.0);//leave a little white space on the right 
	maxWidth = (int)((double)graphRect.Width()*98.0/100.0);//and on the bottom

	//draw graph title
	CFont titleFont;
	CFont titleFontLine2;
	m_xFontSize = (maxWidth/60);
	m_yFontSize = (maxHeight/25);//20
	titleFont.CreateFont(m_yFontSize,m_xFontSize, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");

	CFont* pOldFont = (CFont*) pDC->SelectObject(&titleFont);

	CSize cSize = pDC->GetTextExtent(graphTitle);
	pDC->TextOut((graphRect.Width()-cSize.cx)/2,1,graphTitle);
	if (graphTitleLine2.GetLength())
	{
		titleFontLine2.CreateFont(m_yFontSize*7/10,m_xFontSize*7/10, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&titleFontLine2);
		CSize cSizeLine2 = pDC->GetTextExtent(graphTitleLine2);
		pDC->TextOut((graphRect.Width()-cSizeLine2.cx)/2,cSize.cy+2,graphTitleLine2);
		cSize.cy += cSizeLine2.cy;
	}
	pDC->SelectObject(pOldFont);
	cSize.cy += 2;
	m_xFontSize = m_xFontSize * 15 / 10;
	if(graphType == PIE_GRAPH)// || (graphType == PIE_GRAPH_3D))
	{

		//since pie does not have axis lines, set to full size minus 5 on each side
		//these are needed for legend to plot itself
		xAxisWidth = maxWidth - 10;
		yAxisHeight = maxHeight - 50;  //10 buffer, 20 for title, and 20 for series label
		xApexPoint = 5;
		yApexPoint = maxHeight - 5;
	}
	else
	{
		//compute label size for axis alignment
		CFont axisLabelFont, tickLabelFont;

		//Y axis label
		axisLabelFont.CreateFont(0, m_xFontSize*5/8, 900, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");

		//Y tick label
		tickLabelFont.CreateFont(m_xFontSize*5/8,m_yFontSize*5/8,0,0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");


		CFont* pOldFont = (CFont*) pDC->SelectObject(&axisLabelFont);
		CSize cAxisLabelSize = pDC->GetTextExtent(axisYLabel);


		if (bYAxisLinear)
			tickLabel.Format("%d", maxYTick*iYLNumCycles);
		else
			tickLabel.Format("0.000e-000");

		pDC->SelectObject(&tickLabelFont);
		CSize cTickLabelSize = pDC->GetTextExtent(tickLabel);

		pDC->SelectObject(pOldFont);

		//determine axis specifications 
		if(graphAlignment)
		{
			xApexPoint = maxWidth*90/1000;
			//			xApexPoint = cAxisLabelSize.cy + cTickLabelSize.cx;
			xAxisWidth = maxWidth - xApexPoint;
			if (m_bHasRightAxis)
				xAxisWidth -= xApexPoint;
			int iTemp = cTickLabelSize.cy * m_iXTextLines;
			yApexPoint = maxHeight * 97 / 100 - iTemp;
			yAxisHeight = yApexPoint - cSize.cy;
		}
		else
		{
			xApexPoint = 5 + cAxisLabelSize.cx + cTickLabelSize.cx;
			yApexPoint = (maxHeight - 15) - cSize.cy;
			yAxisHeight = yApexPoint - 40;
			xAxisWidth = (maxWidth - 5) - xApexPoint;
		}
	}

	//draw legend
	if(bHasLegendInfo)
	{
		int legendLeft = DrawLegend(pDC);
		xAxisWidth = legendLeft - 10 - xApexPoint;
		if (m_bHasRightAxis)
			xAxisWidth -= xApexPoint;
	}

	iXMarkerSize = xAxisWidth * iMarkerSize / 400;
	iYMarkerSize = yAxisHeight* iMarkerSize / 400;
	iXMarkerSize = max(2,iXMarkerSize);
	iYMarkerSize = max(2,iYMarkerSize);

	maxXTick = m_maxXTickValue;
	minXTick = m_minXTickValue;
	tickXSpace = m_XtickStep;

	maxYTick = m_maxYTickValue;
	minYTick = m_minYTickValue;
	tickYSpace = m_YtickStep;

	maxYRTick = m_maxYRTickValue;
	minYRTick = m_minYRTickValue;
	tickYRSpace = m_YRtickStep;

	if (bXAxisLinear)
		numXTicks = m_XtickStep;
	else
		numXTicks = 10;

	if (bYAxisLinear)
		numYTicks = (maxYTick - minYTick) / m_YtickStep;
	else
		numYTicks = 10;

	if((!quadSetManually) && (graphType == BAR_GRAPH))
	{
		//computer number of quadrants needed based on data
		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries;
		int minXValue = 0;
		int minYValue = 0;
		for(int x = 1; x <= graphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			for(int s = 0; s < seriesSize; s++)
			{
				//to allow scalability (height may be less than tickRange)
				int curXValue = tmpSeries->GetXDataValue();
				int curYValue = tmpSeries->GetYDataValue();
				if(curXValue < minXValue)
					minXValue = curXValue;
				if(curYValue < minYValue)
					minYValue = curYValue;
			}
		}
		graphQuadType = 1;
		if((minXValue < 0) && (minYValue < 0))
			graphQuadType = 4;
		if((minXValue < 0) && (minYValue >= 0))
			graphQuadType = 2;
		if((minXValue >= 0) && (minYValue < 0))
			graphQuadType = 3;
	}

	if(graphType == BAR_GRAPH)
	{
		switch(graphQuadType)
		{
		case 2 :
			xApexPoint += xAxisWidth / 2;
			break;
		case 3 :
			yApexPoint -= yAxisHeight / 2;
			break;
		case 4 :
			xApexPoint += xAxisWidth / 2;
			yApexPoint -= yAxisHeight / 2;
			break;
		}
	}

	if((graphType != 2) && (graphType != 32))  //pie & 3d pie
	{
		inRedraw = FALSE;
		//draw axis lines
		DrawAxis(pDC);
	}

	//draw series data and labels
	DrawSeries(pDC);

	// Copy the bits from the in-memory DC into the on-
	// screen DC to actually do the painting. Use the centerpoint
	// we computed for the target offset.

	if (!pDCFinal->IsPrinting())
	{
		pDCFinal->BitBlt(0,0,graphRect.Width(),graphRect.Height(),&dcMemory,0,0,SRCCOPY);
		pDC->SelectObject(oldBM);
	}
	m_bInDraw = false;
}

void CGraph::DrawAxis(CDC* pDC)
{
	TRACE0("CGraph::DrawAxis\n");

	TEXTMETRIC	tm;
	pDC->SetTextColor(BLACK);
	CFont sideFont, axisFont, dummyFont;
	int charWidth;
	double ytickScale;
	double xtickScale;
	int tickXLocation = 0;
	int tickYLocation = 0;
	double seriesSpace;
	CFont* pOldFont;
	int x, y;
	int iXAxisTickLength = yAxisHeight/75;
	int iYAxisTickLength = xAxisWidth/100;

	COLORREF axisLineColor;
	axisLineColor = BLACK;
	CPen axisPen (PS_SOLID, (pDC->IsPrinting())?4:2, axisLineColor);
	pOldFont = pDC->GetCurrentFont();
	switch(graphType)
	{
	case BAR_GRAPH :
	case LINE_GRAPH :
	case SCATTER_GRAPH :
		CPen* pOldAxisPen;
		pOldAxisPen = pDC->SelectObject(&axisPen);
		//			switch(graphQuadType)
		//			{
		//				case 1 :
		//draw y axis
		/*
		pDC->MoveTo(xApexPoint, yApexPoint);  
		pDC->LineTo(xApexPoint, yApexPoint - yAxisHeight);
		//					for (int i = 0; i < 200; i= i + 10)
		//					{
		//						pDC->MoveTo(xApexPoint+i, yApexPoint);  
		//						pDC->LineTo(xApexPoint+i, yApexPoint - yAxisHeight);
		//					}

		if (m_bHasRightAxis)
		{
		pDC->MoveTo(xApexPoint+xAxisWidth,yApexPoint);
		pDC->LineTo(xApexPoint+xAxisWidth,yApexPoint - yAxisHeight);
		}

		//draw low x axis
		pDC->MoveTo(xApexPoint, yApexPoint);  
		pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint);
		*/					
		pDC->Rectangle(xApexPoint, yApexPoint, xApexPoint+xAxisWidth, yApexPoint-yAxisHeight);
		m_cClipRect = CRect(xApexPoint, yApexPoint, xApexPoint+xAxisWidth, yApexPoint-yAxisHeight);

		/*					break;

		case 2 :
		//draw y axis
		pDC->MoveTo(xApexPoint, yApexPoint);  
		pDC->LineTo(xApexPoint, yApexPoint - yAxisHeight);

		if (m_bHasRightAxis)
		{
		pDC->MoveTo(xApexPoint+xAxisWidth,yApexPoint);
		pDC->LineTo(xApexPoint+xAxisWidth,yApexPoint - yAxisHeight);
		}

		//draw x axis
		pDC->MoveTo(xApexPoint - (xAxisWidth / 2), yApexPoint);  
		pDC->LineTo(xApexPoint + (xAxisWidth / 2), yApexPoint);
		break;
		case 3 :
		//draw y axis
		pDC->MoveTo(xApexPoint, yApexPoint + (yAxisHeight / 2));  
		pDC->LineTo(xApexPoint, yApexPoint - (yAxisHeight / 2));

		//draw x axis
		pDC->MoveTo(xApexPoint, yApexPoint);  
		pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint);
		break;
		case 4 :
		//draw y axis
		pDC->MoveTo(xApexPoint, yApexPoint + (yAxisHeight / 2));  
		pDC->LineTo(xApexPoint, yApexPoint - (yAxisHeight / 2));

		//draw x axis
		pDC->MoveTo(xApexPoint - (xAxisWidth / 2), yApexPoint);  
		pDC->LineTo(xApexPoint + (xAxisWidth / 2), yApexPoint);
		break;
		}
		*/
		pDC->SelectObject(pOldAxisPen);

		//draw labels
		axisFont.CreateFont((int)(maxHeight*13.0/500),(int)(maxWidth*13.0/1000), 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		sideFont.CreateFont((int)(maxWidth*13.0/500),(int)(maxHeight*13.0/1001), 900, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");

		pDC->SelectObject(&sideFont);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;

		if(graphAlignment)
		{
			CSize cSize = pDC->GetTextExtent(axisYLabel);
			CSize cSizeRight = pDC->GetTextExtent(axisYRLabel);
			COLORREF cPreviousColor = pDC->SetTextColor(axisYLabelColor);
			pDC->TextOut(maxWidth*1/100, yApexPoint - yAxisHeight/2 + cSize.cx/2, axisYLabel);
			if (m_bHasRightAxis)
			{
				pDC->SetTextColor(axisYRLabelColor);
				pDC->TextOut(xApexPoint + xAxisWidth + xApexPoint - cSizeRight.cy, yApexPoint - yAxisHeight/2 + cSizeRight.cx/2,axisYRLabel);
			}
			pDC->SetTextColor(cPreviousColor);

			pDC->SelectObject(&axisFont);

			cSize = pDC->GetTextExtent(axisXLabel);
			pDC->TextOut(xApexPoint + (xAxisWidth - cSize.cx)/2, maxHeight-cSize.cy/2,  axisXLabel);

			pDC->SelectObject(pOldFont);

			double yTempCycleHeight;
			double xTempCycleWidth;

			if (bYAxisLinear)
			{
				yTempCycleHeight = (yAxisHeight-10)/(double)iYLNumCycles;
				ytickScale = yTempCycleHeight / numYTicks;
			}
			else
			{
				yTempCycleHeight = (yAxisHeight-10)/(double)iYNLNumCycles;
				ytickScale = yTempCycleHeight / numYTicks;
			}

			if (bXAxisLinear)
			{
				xTempCycleWidth = (xAxisWidth - 10)/(double)iXLNumCycles;
				xtickScale = xTempCycleWidth / numXTicks;
			}
			else
			{
				xTempCycleWidth = (xAxisWidth - 10)/(double)iXNLNumCycles;
				xtickScale = xTempCycleWidth / numXTicks;
			}

			double yTempApexPoint = yApexPoint;
			double xTempApexPoint = xApexPoint;

			double yTempHeight = yApexPoint;

			double ytickTempSpace,yRtickTempSpace;
			double xtickTempSpace;

			double yBaseCycle = pow(10.0,(bYAxisLinear)?iYLBaseCycle:iYNLBaseCycle);
			double yRBaseCycle = pow(10.0,(bYAxisLinear)?iYRLBaseCycle:iYRNLBaseCycle);
			double xBaseCycle = pow(10.0,(bXAxisLinear)?iXLBaseCycle:iXNLBaseCycle);

			if (bYAxisLinear)
			{
				ytickTempSpace = tickYSpace;
				yRtickTempSpace = tickYRSpace;
			}
			else
			{
				ytickTempSpace = yBaseCycle;
				yRtickTempSpace = yRBaseCycle;
			}

			if (bXAxisLinear)
				xtickTempSpace = tickXSpace;
			else
				xtickTempSpace = xBaseCycle;

			//draw y axis ticks
			if(!inRedraw)
			{
				switch(graphQuadType)
				{
				case 1 :
					{
						CFont yFont;
						yFont.CreateFont(m_yFontSize*5/8, m_xFontSize*4/8, 0, 0, 700, FALSE, FALSE, 0,
							ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
						int lasttickYLocation = INT_MAX;
						double dY = m_minYTickValue + ytickTempSpace;
						double dYR = m_minYRTickValue + yRtickTempSpace;

						COLORREF gridLineColor;
						gridLineColor = DARK_GRAY;
						CPen gridPen (PS_SOLID, 1, gridLineColor);

						for (int iCycle = 0; iCycle < ((bYAxisLinear)?iYLNumCycles:iYNLNumCycles); iCycle++)
						{
							dY -= ytickTempSpace;
							dYR -= yRtickTempSpace;
							yTempHeight += yTempCycleHeight;
							int iBase;
							int iMax;
							if (bYAxisLinear)
							{
								iBase = 0;
								iMax = (int)numYTicks;
							}
							else
							{
								numYTicks = 10;
								iBase = 1;
								iMax = (int)numYTicks;
							}

							for(y = iBase; y <= iMax; y++) 
							{
								if (bYAxisLinear)
									tickYLocation = (int)(yTempApexPoint - (y * ytickScale));
								else
									tickYLocation = (int)(yTempApexPoint - log10((double)y)*ytickScale*10);


								if (graphHasHGridLines)
								{
									CPen* pOldPen;
									pOldPen = pDC->SelectObject(&gridPen);
									pDC->MoveTo(xApexPoint, tickYLocation);
									pDC->LineTo(xAxisWidth+xApexPoint, tickYLocation);
									pDC->SelectObject(pOldPen);
								}
								//draw tick mark
								int iTemp = (y==iMax)?iYAxisTickLength*3/2:iYAxisTickLength;
								pDC->MoveTo(xApexPoint,tickYLocation);
								pDC->LineTo(xApexPoint+iTemp,tickYLocation);
								pDC->MoveTo(xApexPoint+xAxisWidth-iTemp,tickYLocation);
								pDC->LineTo(xApexPoint+xAxisWidth, tickYLocation);

								//draw tick label
								CString tickLabel;
								CString tickLabelRight;
								if (bYAxisLinear)
								{
									if (fabs(dY) < 1.0e-12) dY = 0.0;
									tickLabel.Format("%g", dY);
									dY += ytickTempSpace;

									if (m_bHasRightAxis)
									{
										if (fabs(dYR) < 1.0e-12) dYR = 0.0;
										tickLabelRight.Format("%g",dYR);
										dYR += yRtickTempSpace;
									}

								}
								else
								{
									tickLabel.Format("%.4g", (double)y * ytickTempSpace);
									if (m_bHasRightAxis)
										tickLabelRight.Format("%.4g",(double)y * yRtickTempSpace);
								}

								pDC->SelectObject(&yFont);

								if (bYAxisLinear || y == 1 || y == numYTicks || (((bYAxisLinear)?iYLNumCycles:iYNLNumCycles) < 2 ))
								{
									CSize cSize = pDC->GetTextExtent(tickLabel);
									if (!bYAxisLinear || ((lasttickYLocation-tickYLocation)>cSize.cy))
									{
										COLORREF cPreviousColor = pDC->SetTextColor(axisYLabelColor);
										pDC->TextOut(xApexPoint - iYAxisTickLength/2 - cSize.cx,
											tickYLocation - cSize.cy/2,
											tickLabel);
										lasttickYLocation = tickYLocation;
										if (m_bHasRightAxis)
										{
											pDC->SetTextColor(axisYRLabelColor);
											pDC->TextOut(xApexPoint+xAxisWidth+iYAxisTickLength/2,
												tickYLocation - cSize.cy/2,
												tickLabelRight);
										}
										pDC->SetTextColor(cPreviousColor);
									}
								}

								pDC->SelectObject(pOldFont);
								topYTick = tickYLocation;
							}
							yTempApexPoint -= yTempCycleHeight; 
							if (!bYAxisLinear)
							{
								ytickTempSpace *= 10;
								yRtickTempSpace *= 10;
							}
						}
					}
					break;

				case 2 :

					if(graphType == SCATTER_GRAPH)
					{
						double oldTickScale;
						double oldNumYTicks;
						oldNumYTicks = numYTicks;
						oldTickScale = ytickScale;

						numYTicks /= 2;
						ytickScale *= 2;
						COLORREF gridLineColor;
						gridLineColor = DARK_GRAY;
						CPen gridPen (PS_SOLID, 1, gridLineColor);
						CFont yFont;
						yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
							ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
						for(y = 1; y <= numYTicks; y++)  
						{
							tickYLocation = (int)(yApexPoint - (y * ytickScale));

							//draw tick mark
							pDC->MoveTo(xApexPoint - 3, tickYLocation);
							pDC->LineTo(xApexPoint + 3, tickYLocation);

							if(graphHasHGridLines)
							{
								//draw grid lines
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
								pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
								pDC->SelectObject(pOldPen);
							}

							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", minYTick + (y * tickYSpace));

							pDC->SelectObject(&yFont);

							pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
							pDC->SelectObject(pOldFont);
							topYTick = tickYLocation;
						}
						//draw the 0
						pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");

						//reset numYTicks and tickScale back to normal
						ytickScale = oldTickScale;
						numYTicks = oldNumYTicks;
					}
					else
					{
						COLORREF gridLineColor;
						gridLineColor = DARK_GRAY;
						CPen gridPen (PS_SOLID, 1, gridLineColor);
						CFont yFont;
						yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
							ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
						for(y = 1; y <= numYTicks; y++)  
						{
							tickYLocation = (int)(yApexPoint - (y * ytickScale));

							//draw tick mark
							pDC->MoveTo(xApexPoint - 3, tickYLocation);
							pDC->LineTo(xApexPoint + 3, tickYLocation);

							if(graphHasHGridLines)
							{
								//draw grid lines
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
								pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
								pDC->SelectObject(pOldPen);
							}

							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", minYTick + (y * tickYSpace));

							pDC->SelectObject(&yFont);

							pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
							topYTick = tickYLocation;

						}
						//draw the 0
						pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");
					}
					pDC->SelectObject(pOldFont);
					break;
				case 3 :
					{
						COLORREF gridLineColor;
						gridLineColor = DARK_GRAY;
						CPen gridPen (PS_SOLID, 1, gridLineColor);
						CFont yFont;
						yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
							ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
						for(y = 0; y <= numYTicks / 2; y++) 
						{
							tickYLocation = (int)(yApexPoint + 5 - (yAxisHeight / 2) + (y * ytickScale));

							//draw tick mark
							pDC->MoveTo(xApexPoint - 3, tickYLocation);
							pDC->LineTo(xApexPoint + 3, tickYLocation);

							if(graphHasHGridLines)
							{
								//draw grid lines
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(xApexPoint, tickYLocation);
								pDC->LineTo(xApexPoint, tickYLocation);
								pDC->SelectObject(pOldPen);
							}

							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", maxYTick - (y * tickYSpace));

							pDC->SelectObject(&yFont);

							pDC->TextOut(xApexPoint - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
							topYTick = tickYLocation;

						}
						//draw the 0
						pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");

						for(; y <= numYTicks; y++) 
						{
							tickYLocation = (int)(yApexPoint + 5 - (yAxisHeight / 2) + (y * ytickScale));

							//draw tick mark
							pDC->MoveTo(xApexPoint - 3, tickYLocation);
							pDC->LineTo(xApexPoint + 3, tickYLocation);

							if(graphHasHGridLines)
							{
								//draw grid lines
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(xApexPoint, tickYLocation);
								pDC->LineTo(xApexPoint, tickYLocation);
								pDC->SelectObject(pOldPen);
							}

							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", minYTick + ((numYTicks - y) * tickYSpace));

							pDC->TextOut(xApexPoint - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
							topYTick = tickYLocation;

						}
						pDC->SelectObject(pOldFont);
						break;
					}
				case 4 :
					{
						COLORREF gridLineColor;
						gridLineColor = DARK_GRAY;
						CPen gridPen (PS_SOLID, 1, gridLineColor);
						CFont yFont;
						yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
							ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
						for(y = 1; y <= numYTicks / 2; y++) 
						{
							tickYLocation = (int)(yApexPoint - (y * ytickScale));

							//draw tick mark
							pDC->MoveTo(xApexPoint - 3, tickYLocation);
							pDC->LineTo(xApexPoint + 3, tickYLocation);

							if(graphHasHGridLines)
							{
								//draw grid lines
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
								pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
								pDC->SelectObject(pOldPen);
							}

							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", minYTick + (y * tickYSpace));

							pDC->SelectObject(&yFont);

							pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
							topYTick = tickYLocation;

						}
						//draw the 0
						pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");

						for(; y <= numYTicks; y++)  
						{
							tickYLocation = (int)(yApexPoint - (yAxisHeight / 2) + (y * ytickScale));

							//draw tick mark
							pDC->MoveTo(xApexPoint - 3, tickYLocation);
							pDC->LineTo(xApexPoint + 3, tickYLocation);

							if(graphHasHGridLines)
							{
								//draw grid lines
								//									COLORREF gridLineColor;
								//									gridLineColor = DARK_GRAY;
								//									CPen gridPen (PS_SOLID, 1, gridLineColor);
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
								pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
								pDC->SelectObject(pOldPen);
							}

							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", minYTick + ((numYTicks - y) * tickYSpace));

							pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
							topYTick = tickYLocation;

						}
						pDC->SelectObject(pOldFont);
					}
					break;
				}

			}

			//draw X axis tick marks
			switch(graphQuadType)
			{
			case 1 :
				if(graphType != SCATTER_GRAPH)
				{
					POSITION pos;
					pos = graphSeries->GetHeadPosition();
					CFont xFont;
					xFont.CreateFont(m_yFontSize*4/8, m_xFontSize*4/8, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					CFont sideFont2;
					sideFont2.CreateFont(m_yFontSize*3/8, m_xFontSize*3/8, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);

					int iMarkerCount;
					if (graphType == LINE_GRAPH)
						iMarkerCount = graphXAxisLegend.GetLegendCount();
					else
						iMarkerCount = graphSeries->GetCount();

					for(int x = 1; x <= iMarkerCount; x++)
					{
						seriesSpace = (double)xAxisWidth / iMarkerCount;
						tickXLocation = (int)(xApexPoint + ((x * seriesSpace) - (seriesSpace / 2.0)));

						pDC->MoveTo(tickXLocation,yApexPoint - iXAxisTickLength);
						pDC->LineTo(tickXLocation,yApexPoint + iXAxisTickLength);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(tickXLocation, yApexPoint);  
							pDC->LineTo(tickXLocation, yApexPoint - yAxisHeight);
							pDC->SelectObject(pOldPen);
						}

						//draw tick label
						CString tickLabel;
						pDC->SelectObject(&xFont);
						tickLabel = graphXAxisLegend.GetLegendText(x-1);
						pDC->SetTextColor(graphXAxisLegend.GetLegendColor(x-1));
						if(!xAxisAlign)  //horizontal
							pDC->DrawText(tickLabel,
							CRect(tickXLocation - ((tickLabel.GetLength() * 4) / 2), yApexPoint + 8,1,1),
							DT_NOCLIP | DT_LEFT);

						else
						{
							pDC->SelectObject(&sideFont2);
							if(xAxisAlign < 180)
								pDC->DrawText(tickLabel,
								CRect(tickXLocation - 8, yApexPoint + iXAxisTickLength + iXAxisTickLength + (xAxisLabelLength * charWidth),500,500),
								DT_NOCLIP | DT_LEFT);
							else
								pDC->TextOut(tickXLocation + 2, yApexPoint + iXAxisTickLength + iXAxisTickLength, tickLabel);

							pDC->SelectObject(pOldFont);
						}
						rightXTick = tickXLocation;
					}
				}

				else
				{
					CFont xFont;

					xFont.CreateFont(m_yFontSize*5/8,m_xFontSize*4/8,0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					seriesSpace = (xAxisWidth - 20) / (numXTicks * ((bXAxisLinear)?iXLNumCycles:iXNLNumCycles));
					double dX = minXTick;
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);


					tickXLocation = (int)(xApexPoint);
					rightXTick = tickXLocation;
					pDC->SelectObject(pOldFont);


					for (int iCycle = 0; iCycle < ((bXAxisLinear)?iXLNumCycles:iXNLNumCycles); iCycle++)
					{
						for(int x = 1; x <= numXTicks; x++)  
						{
							if (bXAxisLinear)
								tickXLocation = (int)(xApexPoint + (x * seriesSpace) + (iCycle * seriesSpace * numXTicks));
							else
								tickXLocation = (int)(xTempApexPoint + log10((double)x)*xtickScale*10);

							if (graphHasVGridLines)
							{
								CPen* pOldPen;
								pOldPen = pDC->SelectObject(&gridPen);
								pDC->MoveTo(tickXLocation, yApexPoint);  
								pDC->LineTo(tickXLocation, yApexPoint - yAxisHeight);
								pDC->SelectObject(pOldPen);
							}

							//draw tick mark
							int iTemp = (x == numXTicks)?iXAxisTickLength*3/2:iXAxisTickLength;
							pDC->MoveTo(tickXLocation, yApexPoint - iTemp);
							pDC->LineTo(tickXLocation, yApexPoint);
							pDC->MoveTo(tickXLocation, yApexPoint - yAxisHeight + iTemp);
							pDC->LineTo(tickXLocation, yApexPoint - yAxisHeight);

							//draw tick label
							CString tickLabel;
							if (bXAxisLinear)
							{
								dX += m_XtickStepValue;
								tickLabel.Format("%d", (int)dX);
							}
							else
								tickLabel.Format("%.4g", (double)x * (double)xtickTempSpace);

							pDC->SelectObject(&xFont);
							int iCycles = (bXAxisLinear)?iXLNumCycles:iXNLNumCycles;
							if  (
								//(bXAxisLinear && (iCycles < 2))||  //if linear and 1 or 2 cycles do all
								x == 1         || //if first do it
								x == numXTicks || //if last do it
								//iCycles == 1   || //if 1 cycle then do all
								bXAxisLinear||
								(((x/2)*2 == x) && (iCycles < 2))//if even and 1,2,or3 cycles
								)
							{
								//if (((x/2)*2 == x ) || x == 1)
								{
									CSize cSize = pDC->GetTextExtent(tickLabel);
									pDC->TextOut(tickXLocation - (cSize.cx / 2), yApexPoint + iXAxisTickLength/2, tickLabel);
								}
							}

							rightXTick = tickXLocation;
							pDC->SelectObject(pOldFont);
						}
						xTempApexPoint += xTempCycleWidth;
						if (!bXAxisLinear)
							xtickTempSpace *= 10;
					}
				}

				break;
			case 2 :
				if(graphType != SCATTER_GRAPH)
				{
					POSITION pos;
					pos = graphSeries->GetHeadPosition();
					CGraphSeries* tmpSeries;
					seriesSpace = xAxisWidth / graphSeries->GetCount();
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);
					CFont xFont;
					xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					CFont sideFont2;
					sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					for(x = 1; x <= graphSeries->GetCount() / 2; x++)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}

						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
					}
					if((graphSeries->GetCount() % 2) > 0)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						tickXLocation = xApexPoint;

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							//									COLORREF gridLineColor;
							//									gridLineColor = DARK_GRAY;
							//									CPen gridPen (PS_SOLID, 1, gridLineColor);
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								//										CFont xFont;
								//										xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								//										CFont sideFont2;
								//										sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
						x++;
					}
					for(; x <= graphSeries->GetCount(); x++)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							//									COLORREF gridLineColor;
							//									gridLineColor = DARK_GRAY;
							//									CPen gridPen (PS_SOLID, 1, gridLineColor);
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								//										CFont xFont;
								//										xFont.CreateFont(m_yFontSize/4, m_xFontSize/4,0, 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								//										CFont sideFont2;
								//										sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
					}
				}
				else
				{
					//scatter graphs will use the same tick marking on both axis lines
					seriesSpace = xAxisWidth / numXTicks;
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);
					CFont xFont;
					xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					for(x = 0; x <= numXTicks / 2; x++)  
					{
						tickXLocation = (int)(xApexPoint + 5 - (xAxisWidth / 2.0) + (x * seriesSpace));

						//draw tick mark
						pDC->MoveTo(tickXLocation, yApexPoint - 3);
						pDC->LineTo(tickXLocation, yApexPoint + 3);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", minXTick + (x * tickXSpace));

							pDC->SelectObject(&xFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;

							pDC->TextOut(tickXLocation - (tickLabel.GetLength() * charWidth / 2), yApexPoint + 8, tickLabel);
						}
						rightXTick = tickXLocation;
						pDC->SelectObject(pOldFont);
					}
					//draw the 0
					tickXLocation = xApexPoint;
					if(!inRedraw)
					{
						//draw tick label

						//								CFont xFont;
						//								xFont.CreateFont(m_yFontSize/4, m_xFontSize/4,0, 0, 700, FALSE, FALSE, 0,
						//									ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						//									DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
						pDC->SelectObject(&xFont);
						pDC->GetTextMetrics(&tm);
						charWidth = tm.tmAveCharWidth;

						pDC->TextOut(tickXLocation - (charWidth / 2), yApexPoint + 8, "0");
					}

					for(; x <= numXTicks; x++)  
					{
						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + (x * seriesSpace));

						//draw tick mark
						pDC->MoveTo(tickXLocation, yApexPoint - 3);
						pDC->LineTo(tickXLocation, yApexPoint + 3);

						if (graphHasVGridLines)
						{
							//									COLORREF gridLineColor;
							//									gridLineColor = DARK_GRAY;
							//									CPen gridPen (PS_SOLID, 1, gridLineColor);
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", (x / 2) * tickXSpace);

							//									CFont xFont;
							//									xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
							//										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							//										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
							pDC->SelectObject(&xFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;

							pDC->TextOut(tickXLocation - (tickLabel.GetLength() * charWidth / 2), yApexPoint + 8, tickLabel);
						}
						rightXTick = tickXLocation;
						pDC->SelectObject(pOldFont);
					}
				}
				break;
			case 3 :
				if(graphType != SCATTER_GRAPH)
				{
					POSITION pos;
					pos = graphSeries->GetHeadPosition();
					CGraphSeries* tmpSeries;
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);
					CFont xFont;
					xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					CFont sideFont2;
					sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					for(int x = 1; x <= graphSeries->GetCount(); x++)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						seriesSpace = xAxisWidth / graphSeries->GetCount();
						tickXLocation = (int)(xApexPoint + ((x * seriesSpace) - (seriesSpace / 2.0)));

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
					}
				}
				else
				{
					//scatter graphs will use the same tick marking on both axis lines
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);
					CFont xFont;
					xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					seriesSpace = (xAxisWidth - 20) / numXTicks;
					for(int x = 1; x <= numXTicks; x++)  
					{
						tickXLocation = (int)(xApexPoint + (x * seriesSpace));

						//draw tick mark
						pDC->MoveTo(tickXLocation, yApexPoint - 3);
						pDC->LineTo(tickXLocation, yApexPoint + 3);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", x * tickXSpace);

							pDC->SelectObject(&xFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;

							pDC->TextOut(tickXLocation - (tickLabel.GetLength() * charWidth / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
						}
						rightXTick = tickXLocation;
						pDC->SelectObject(pOldFont);
					}

				}
				break;
			case 4 :
				if(graphType != SCATTER_GRAPH)
				{
					POSITION pos;
					pos = graphSeries->GetHeadPosition();
					CGraphSeries* tmpSeries;
					seriesSpace = xAxisWidth / graphSeries->GetCount();
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);
					CFont xFont;
					xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					CFont sideFont2;
					sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					for(x = 1; x <= graphSeries->GetCount() / 2; x++)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
					}
					if((graphSeries->GetCount() % 2) > 0)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						tickXLocation = xApexPoint;

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							//									COLORREF gridLineColor;
							//									gridLineColor = DARK_GRAY;
							//									CPen gridPen (PS_SOLID, 1, gridLineColor);
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								//										CFont xFont;
								//										xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								//										CFont sideFont2;
								//										sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
						x++;
					}
					for(; x <= graphSeries->GetCount(); x++)
					{
						tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

						pDC->MoveTo(tickXLocation,yApexPoint - 3);
						pDC->LineTo(tickXLocation,yApexPoint + 3);

						if (graphHasVGridLines)
						{
							//									COLORREF gridLineColor;
							//									gridLineColor = DARK_GRAY;
							//									CPen gridPen (PS_SOLID, 1, gridLineColor);
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel = tmpSeries->GetLabel();
							if(!xAxisAlign)  //horizontal
							{
								//										CFont xFont;
								//										xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&xFont);
								pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
								pDC->SelectObject(pOldFont);
							}
							else
							{
								//										CFont sideFont2;
								//										sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
								//											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								//											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
								pDC->SelectObject(&sideFont2);
								pDC->GetTextMetrics(&tm);
								charWidth = tm.tmAveCharWidth;
								if(xAxisAlign < 180)
									pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
								else
									pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

								pDC->SelectObject(pOldFont);
							}
						}
						rightXTick = tickXLocation;
					}
				}
				else
				{
					//scatter graphs will use the same tick marking on both axis lines
					seriesSpace = (xAxisWidth - 20) / numXTicks;
					COLORREF gridLineColor;
					gridLineColor = DARK_GRAY;
					CPen gridPen (PS_SOLID, 1, gridLineColor);

					CFont xFont;
					xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
					for(x = 1; x <= numXTicks / 2; x++)  
					{
						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + (x * seriesSpace));

						//draw tick mark
						pDC->MoveTo(tickXLocation, yApexPoint + (yAxisHeight / 2) - 3);
						pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2) + 3);

						if (graphHasVGridLines)
						{
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", x * tickXSpace);
							pDC->SelectObject(&xFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;

							pDC->TextOut(tickXLocation - (tickLabel.GetLength() * charWidth / 2), yApexPoint + 8, tickLabel);
						}
						rightXTick = tickXLocation;
						pDC->SelectObject(pOldFont);
					}
					//draw the 0

					for(; x <= numXTicks; x++)  
					{
						tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + (x * seriesSpace));

						//draw tick mark
						pDC->MoveTo(tickXLocation, yApexPoint + (yAxisHeight / 2) - 3);
						pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2) + 3);

						if (graphHasVGridLines)
						{
							//									COLORREF gridLineColor;
							//									gridLineColor = DARK_GRAY;
							//									CPen gridPen (PS_SOLID, 1, gridLineColor);
							CPen* pOldPen;
							pOldPen = pDC->SelectObject(&gridPen);
							pDC->MoveTo(yApexPoint, tickXLocation);
							pDC->LineTo(yApexPoint + xAxisWidth, tickXLocation);
							pDC->SelectObject(pOldPen);
						}
						if(!inRedraw)
						{
							//draw tick label
							CString tickLabel;
							tickLabel.Format("%d", x * tickXSpace);

							//									CFont xFont;
							//									xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
							//										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							//										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
							pDC->SelectObject(&xFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;

							pDC->TextOut(tickXLocation - (tickLabel.GetLength() * charWidth / 2), yApexPoint + 8, tickLabel);
						}
						rightXTick = tickXLocation;
						pDC->SelectObject(pOldFont);
					}
				}
				break;
			}
		}
		/*
		else
		{
		//reverse above stuff and treat as horizontal graph
		switch(graphQuadType)
		{
		case 1 :
		pDC->TextOut(10, (yApexPoint - (yAxisHeight / 2)) + (axisYLabel.GetLength() / 2), axisXLabel);

		pDC->SelectObject(&axisFont);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		labelHeight = tm.tmHeight + 10;
		if(axisYLabel.GetLength() == 0)
		labelHeight = 0;
		pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);

		pDC->SelectObject(pOldFont);

		//to allow scalability (height may be less than tickRange)
		//ytickScale = 0.00;

		//draw y axis ticks
		ytickScale = (yAxisHeight - 10) / seriesSize;
		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries;
		for(y = 1; y <= graphSeries->GetCount(); y++)  
		{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		seriesSpace = yAxisHeight / graphSeries->GetCount();
		tickYLocation = (int)(yApexPoint - ((y * seriesSpace) - (seriesSpace / 2.0)));

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}

		//draw X axis tick marks
		xtickScale = (xAxisWidth - 10) / numXTicks;
		for(x = 1; x <= numXTicks; x++)
		{
		tickXLocation = xApexPoint + (int)(x * xtickScale);

		pDC->MoveTo(tickXLocation,yApexPoint - 3);
		pDC->LineTo(tickXLocation,yApexPoint + 3);

		if(!inRedraw)
		{
		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint);
		pDC->LineTo(tickXLocation, yApexPoint - yAxisHeight);
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", minXTick + (x * tickXSpace));
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + 8, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

		pDC->SelectObject(pOldFont);
		}
		}
		rightXTick = tickXLocation;
		}
		break;

		case 2 :
		pDC->TextOut(10, (yApexPoint - (yAxisHeight / 2)) + (axisYLabel.GetLength() / 2), axisXLabel);

		pDC->SelectObject(&axisFont);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		labelHeight = tm.tmHeight + 10;
		if(axisYLabel.GetLength() == 0)
		labelHeight = 0;
		pDC->TextOut(xApexPoint - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);

		pDC->SelectObject(pOldFont);

		//to allow scalability (height may be less than tickRange)
		ytickScale = 0.00;
		seriesSpace = yAxisHeight / graphSeries->GetCount();

		//draw y axis ticks
		if(graphType != SCATTER_GRAPH)
		{
		ytickScale = (yAxisHeight - 10) / seriesSize;
		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries;
		for(y = 1; y <= graphSeries->GetCount(); y++)  
		{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		tickYLocation = (int)(yApexPoint - ((y * seriesSpace) - (seriesSpace / 2.0)));

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		else
		{
		//scatter graphs will use the same tick marking on both axis lines
		double oldNumYTicks;
		oldNumYTicks = numYTicks;
		numYTicks /= 2;

		for(y = 0; y <= numYTicks; y++)  
		{
		ytickScale = (yAxisHeight - 10) / numYTicks;
		tickYLocation = yApexPoint - (int)(y * ytickScale);

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", (y * tickYSpace));

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);

		pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (tickLabel.GetLength() * (yTickFontSize / 2)) - 6, tickYLocation - 6, tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		numYTicks = oldNumYTicks;
		}

		//draw X axis tick marks
		xtickScale = (xAxisWidth - 10) / numXTicks;
		for(x = 0; x <= numXTicks / 2; x++)
		{
		tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * xtickScale) + 5;

		if(!inRedraw)
		{
		pDC->MoveTo(tickXLocation,yApexPoint - 3);
		pDC->LineTo(tickXLocation,yApexPoint + 3);

		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint - yAxisHeight);
		pDC->LineTo(tickXLocation, yApexPoint);
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", minXTick + (x * tickXSpace));
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + 8, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

		pDC->SelectObject(pOldFont);
		}
		}
		rightXTick = tickXLocation;
		}
		//draw the 0 location
		tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * xtickScale);
		if(!inRedraw)
		{
		//draw tick label
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - (charWidth / 2), yApexPoint + 8, "0");
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), "0");
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + 8, "0");

		pDC->SelectObject(pOldFont);
		}
		}

		for(; x <= numXTicks; x++)
		{
		tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * xtickScale);

		pDC->MoveTo(tickXLocation,yApexPoint - 3);
		pDC->LineTo(tickXLocation,yApexPoint + 3);

		if(!inRedraw)
		{
		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint - yAxisHeight);
		pDC->LineTo(tickXLocation, yApexPoint);
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", minXTick + (x * tickXSpace));
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + 8, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

		pDC->SelectObject(pOldFont);
		}
		}
		rightXTick = tickXLocation;
		}
		break;
		case 3 :
		pDC->TextOut(10, yApexPoint - (axisYLabel.GetLength() / 2), axisXLabel);

		pDC->SelectObject(&axisFont);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		labelHeight = tm.tmHeight + 10;
		if(axisYLabel.GetLength() == 0)
		labelHeight = 0;
		pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);

		pDC->SelectObject(pOldFont);

		//to allow scalability (height may be less than tickRange)
		ytickScale = 0.00;
		seriesSpace = yAxisHeight / graphSeries->GetCount();

		//draw y axis ticks
		if(graphType != SCATTER_GRAPH)
		{
		ytickScale = (yAxisHeight - 10) / seriesSize;
		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries;
		for(y = 1; y <= graphSeries->GetCount() / 2; y++)  
		{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - 5 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		//draw the 0 location
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
		tickYLocation = yApexPoint;
		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - 5 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		y++;

		for(; y <= graphSeries->GetCount(); y++)  
		{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - 5 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		}
		else
		{
		//scatter graphs will use the same tick marking on both axis lines
		for(y = 0; y < numYTicks / 2; y++)  
		{
		ytickScale = (yAxisHeight - 10) / numYTicks;
		tickYLocation = yApexPoint - (yAxisHeight / 2) + (int)(y * ytickScale) + 5;

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", maxYTick - (y * tickYSpace));

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);

		pDC->TextOut(xApexPoint - (tickLabel.GetLength() * (yTickFontSize / 2)) - 8, tickYLocation - 6, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		}
		//draw the 0 location
		tickYLocation = yApexPoint;
		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);

		pDC->TextOut(xApexPoint - (yTickFontSize / 2) - 8, tickYLocation - 6, "0");
		pDC->SelectObject(pOldFont);
		}
		y++;

		for(; y <= numYTicks; y++)  
		{
		ytickScale = (yAxisHeight - 10) / numYTicks;
		tickYLocation = yApexPoint - (yAxisHeight / 2) + (int)(y * ytickScale) + 5;

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", maxYTick - (y * tickYSpace));

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);

		pDC->TextOut(xApexPoint - (tickLabel.GetLength() * (yTickFontSize / 2)) - 5, tickYLocation - 6, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		topYTick = tickYLocation;
		}
		}

		//draw X axis tick marks
		double oldNumXTicks;
		oldNumXTicks = numXTicks;
		numXTicks /= 2;
		xtickScale = (xAxisWidth - 10) / numXTicks;

		for(x = 0; x <= numXTicks; x++)
		{
		tickXLocation = xApexPoint + (int)(x * xtickScale);

		if(!inRedraw)
		{
		pDC->MoveTo(tickXLocation,yApexPoint - 3);
		pDC->LineTo(tickXLocation,yApexPoint + 3);

		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
		pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", (x * tickXSpace * 2));
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4,(xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

		pDC->SelectObject(pOldFont);
		}
		}
		}
		numXTicks = oldNumXTicks;
		rightXTick = tickXLocation;
		break;
		case 4 :
		pDC->TextOut(10, yApexPoint - (axisYLabel.GetLength() / 2), axisXLabel);

		pDC->SelectObject(&axisFont);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		labelHeight = tm.tmHeight + 10;
		if(axisYLabel.GetLength() == 0)
		labelHeight = 0;
		pDC->TextOut(xApexPoint - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);

		pDC->SelectObject(pOldFont);

		//to allow scalability (height may be less than tickRange)
		ytickScale = 0.00;
		seriesSpace = yAxisHeight / graphSeries->GetCount();

		//draw y axis ticks
		if(graphType != SCATTER_GRAPH)
		{
		ytickScale = (yAxisHeight - 10) / seriesSize;
		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries;
		for(y = 1; y <= graphSeries->GetCount() / 2; y++)  
		{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		//draw the 0 location
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
		tickYLocation = yApexPoint;
		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		y++;

		for(; y <= graphSeries->GetCount(); y++)  
		{
		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

		tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);
		charWidth = tm.tmAveCharWidth;

		pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		}
		else
		{
		//scatter graphs will use the same tick marking on both axis lines
		for(y = 1; y <= numYTicks; y++)  
		{
		ytickScale = (yAxisHeight - 10) / numYTicks;
		tickYLocation = yApexPoint - (yAxisHeight / 2) + (int)(y * ytickScale);

		//draw tick mark
		pDC->MoveTo(xApexPoint - 3, tickYLocation);
		pDC->LineTo(xApexPoint + 3, tickYLocation);

		if(!inRedraw)
		{
		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", y * tickYSpace);

		CFont yFont;
		yFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&yFont);

		pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
		}
		topYTick = tickYLocation;
		pDC->SelectObject(pOldFont);
		}
		}

		//draw X axis tick marks
		xtickScale = (xAxisWidth - 10) / numXTicks;
		for(x = 0; x <= numXTicks / 2; x++)
		{
		tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * xtickScale) + 5;

		if(!inRedraw)
		{
		pDC->MoveTo(tickXLocation,yApexPoint - 3);
		pDC->LineTo(tickXLocation,yApexPoint + 3);

		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
		pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", minXTick + (x * tickXSpace));
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

		pDC->SelectObject(pOldFont);
		}
		}
		rightXTick = tickXLocation;
		}
		//draw the 0 location
		tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * xtickScale);
		if(!inRedraw)
		{
		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
		pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - (charWidth / 2), yApexPoint + (yAxisHeight / 2) + 8, "0");
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), "0");
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, "0");

		pDC->SelectObject(pOldFont);
		}
		}
		x++;

		for(; x <= numXTicks; x++)
		{
		tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * xtickScale);

		pDC->MoveTo(tickXLocation,yApexPoint - 3);
		pDC->LineTo(tickXLocation,yApexPoint + 3);

		if(!inRedraw)
		{
		if(graphHasHGridLines)
		{
		//draw grid lines
		COLORREF gridLineColor;
		gridLineColor = DARK_GRAY;
		CPen gridPen (PS_SOLID, 1, gridLineColor);
		CPen* pOldPen;
		pOldPen = pDC->SelectObject(&gridPen);
		pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
		pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
		pDC->SelectObject(pOldPen);
		}

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", (x / 2) * tickXSpace);
		if(!xAxisAlign)  //horizontal
		{
		CFont xFont;
		xFont.CreateFont(m_yFontSize/4, m_xFontSize/4, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&xFont);
		charWidth = tm.tmAveCharWidth;
		pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
		pDC->SelectObject(pOldFont);
		}
		else
		{
		CFont sideFont2;
		sideFont2.CreateFont(m_yFontSize/4, m_xFontSize/4, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		pDC->SelectObject(&sideFont2);
		pDC->GetTextMetrics(&tm);
		charWidth = tm.tmAveCharWidth;
		if(xAxisAlign < 180)
		pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
		else
		pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

		pDC->SelectObject(pOldFont);
		}
		}
		rightXTick = tickXLocation;
		}
		break;
		}
		}
		break;
		*/
	}

	iXMarkerSize = max(2,iXMarkerSize);
	iYMarkerSize = max(2,iYMarkerSize);

	cXAxisRect =  CRect(xApexPoint, yApexPoint+iYMarkerSize, graphRect.right, graphRect.bottom);
	cYAxisRect =  CRect(0,0,xApexPoint-iXMarkerSize/2,yApexPoint);
	if (m_bHasRightAxis)
		cYRAxisRect = CRect(xApexPoint+xAxisWidth,0,xApexPoint+xAxisWidth+xApexPoint,yApexPoint);
	else
		cYRAxisRect = CRect(0,0,0,0);
	cXYAxisRect = CRect(0,yApexPoint,xApexPoint,graphRect.bottom);
	cPlotRect =   CRect(xApexPoint-iXMarkerSize-iXMarkerSize,yApexPoint-yAxisHeight-iYMarkerSize-iYMarkerSize,
		xApexPoint+xAxisWidth+iYMarkerSize+iYMarkerSize,yApexPoint+iYMarkerSize+iYMarkerSize);

}

void CGraph::LoadFile(CString cFileName)
{
	TRACE1("CGraph::Load File(%s)\n",cFileName);
	FILE* sInFile;
	sInFile = fopen(cFileName,"rt");
	int iMin, iMax;
	if (sInFile)
	{
		int iVal[10];
		int iIndex;
		CString cTitles[10];
		char szString[256];
		szString[0] = NULL;
		int i = 0;

		//get the title(s)
		for (i = 0; i < 10; i++)
			cTitles[i] = "";
		fscanf(sInFile,"%[^\n]",szString);
		char *szAt = szString;
		szAt = strtok(szString,",");
		while (szAt && (i < 10))
		{
			cTitles[i++] = szAt;
			szAt = strtok(NULL,",");
		}

		//		if (strlen(szString) < 50)
		//			SetGraphTitleLine2(szString);
		//		else
		//			return;

		//get the data
		CGraphSeries* data[10];
		for (i = 0; i < 10; i++)
		{
			data[i] = new CGraphSeries(i);
			data[i]->SetDoLines(true);
			data[i]->SetStyle(0);  // pixels
			data[i]->SetColor(m_cColorTable[i]);
			data[i]->SetGroup(i+1);
			iVal[i] = 0;
		}

		iIndex = 0;
		iMin = INT_MAX; 
		iMax = INT_MIN;

		fgetc(sInFile);
		fscanf(sInFile,"%[^\n]",szString);
		int iMaxSpectCount=0;
		int iSpectCount;
		while((( iSpectCount = sscanf(szString,"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			&iVal[0], &iVal[1], &iVal[2], &iVal[3], &iVal[4], 
			&iVal[5], &iVal[6], &iVal[7], &iVal[8], &iVal[9])) > 0) && (iIndex < 1024))
		{
			iMaxSpectCount = max(iMaxSpectCount, iSpectCount);
			for (int i = 0; i < iSpectCount; i++)
			{
				iMin = min(iMin, iVal[i]);
				iMax = max(iMax, iVal[i]);
				data[i]->SetData((double)iIndex, (double)iVal[i]);
			}
			iIndex++;
			fgetc(sInFile);
			fscanf(sInFile,"%[^\n]",szString);
		}
		fclose(sInFile);
		for (i = 0; i < 10; i++)
		{
			if (i < iSpectCount)
			{
				data[i]->SetLabel(cTitles[i]);
				//int datagroup, CString label, COLORREF cColor = NULL, int iStyle=0, bool bDoDraw = true);
				SetLegend(i,cTitles[i],m_cColorTable[i]);
				AddSeries(data[i]);
			}
			else
				delete data[i];
		}

		if		(iMax <    10) iMax = 1;
		else if (iMax <    20) iMax = 2;
		else if (iMax <    30) iMax = 3;
		else if (iMax <    40) iMax = 4;
		else if (iMax <    50) iMax = 5;
		else if (iMax <    60) iMax = 6;
		else if (iMax <    70) iMax = 7;
		else if (iMax <    80) iMax = 8;
		else if (iMax <    90) iMax = 9;
		else if (iMax <   100) iMax = 10;
		else if (iMax <   200) iMax = 20;
		else if (iMax <   300) iMax = 30;
		else if (iMax <   400) iMax = 40;
		else if (iMax <   500) iMax = 50;
		else if (iMax <   600) iMax = 60;
		else if (iMax <   700) iMax = 70;
		else if (iMax <   800) iMax = 80;
		else if (iMax <   900) iMax = 90;
		else if (iMax <  1000) iMax = 100;
		else if (iMax <  2000) iMax = 200;
		else if (iMax <  3000) iMax = 300;
		else if (iMax <  4000) iMax = 400;
		else if (iMax <  5000) iMax = 500;
		else if (iMax <  6000) iMax = 600;
		else if (iMax <  7000) iMax = 700;
		else if (iMax <  8000) iMax = 800;
		else if (iMax <  9000) iMax = 900;
		else if (iMax < 10000) iMax = 1000;
		else if (iMax < 15000) iMax = 1500;
		else if (iMax < 20000) iMax = 2000;
		else if (iMax < 25000) iMax = 2500;
		else if (iMax < 30000) iMax = 3000;
		else if (iMax < 35000) iMax = 3500;
		else if (iMax < 40000) iMax = 4000;
		else if (iMax < 45000) iMax = 4500;
		else if (iMax < 50000) iMax = 5000;
		else if (iMax < 55000) iMax = 5500;
		else if (iMax < 60000) iMax = 6000;
		else iMax = 7000;

		int iTempMin = 0;
		while (iTempMin < iMin) 
		{
			iTempMin += iMax;
		}
		if (iMin > 0)
			iTempMin -= iMax;


		SetXTickLimits(0,100,1);
		SetYTickLimits(iTempMin,iMax,1);

		SetLinear(true,true);
		SetMarkerSize(3);

		//Number, Base of X axis cycles, Number, Base of Y axis cycles 
		//(int iXCycles, int iXBaseCycles, int iYCycles, int iYBaseCycles, int iYRBaseCycles = NULL) 
		SetLCycles(11,0,10,1,3);
		//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
		SetNLCycles(3,0,5,0,3);

	}
}

void CGraph::SaveFile(CString cFileName)
{
	TRACE1("CGraph::Save File(%s)\n",cFileName);
	FILE* sOutFile;
	sOutFile = fopen(cFileName,"wt");
	if (sOutFile)
	{
		int iValues[1024][10];
		int iSpectCount = 0;
		CString cLabel[10];
		//		fprintf(sOutFile,"%s\n",graphTitleLine2);
		ZeroMemory(iValues,sizeof(iValues));
		POSITION pos;
		for(pos = graphSeries->GetHeadPosition(); pos != NULL; graphSeries->GetNext(pos))
		{
			CGraphSeries* tmpSeries = (CGraphSeries*)graphSeries->GetAt(pos);
			cLabel[iSpectCount] = tmpSeries->GetLabel();
			int iDataPoints = tmpSeries->GetDataCount();
			for (int i = 0; i < iDataPoints; i++)
			{
				int iXValue, iYValue;
				double dXValue, dYValue;
				tmpSeries->IterateGet(&dXValue,&dYValue);
				iXValue = (int)dXValue;
				iYValue = (int)dYValue;
				if ((iXValue >=0) && (iXValue < 1024))
					iValues[iXValue][iSpectCount]= iYValue;
			}
			if (iSpectCount <= 9)
				iSpectCount++;
		}
		int iIndex;
		for (iIndex = 0; iIndex < iSpectCount; iIndex++)
		{
			fprintf(sOutFile,"%s",cLabel[iIndex]);
			if (iIndex == (iSpectCount-1))
				fprintf(sOutFile,"\n");
			else
				fprintf(sOutFile,",");
		}

		for (iIndex = 0; iIndex < 1024; iIndex++)
		{
			for (int iSpect = 0; iSpect < iSpectCount; iSpect++)
			{
				fprintf(sOutFile,"%d",iValues[iIndex][iSpect]);
				if (iSpect == (iSpectCount-1)) 
					fprintf(sOutFile,"\n");
				else
					fprintf(sOutFile,",");
			}
		}
		fclose(sOutFile);
	}
}

bool CGraph::GetDataItemAt(CPoint cPoint, double*dX,double*dY)
{

	TRACE2("CGraph::GetDataItemAt(%d, %d)\n",cPoint.x, cPoint.y);

	POSITION pos;
	CGraphSeries* tmpSeries;
	double yAxisScale = 0.0;
	double yRAxisScale = 0.0;
	double xAxisScale = 0.0;
	int dataXLocation = 0;
	int dataYLocation = 0;

	if (bYAxisLinear)
	{
		yAxisScale = ((yApexPoint - topYTick) / ((double)(maxYTick-minYTick)*iYLNumCycles));
		if (m_bHasRightAxis)
			yRAxisScale = ((yApexPoint - topYTick) / ((double)(maxYRTick-minYRTick)*iYLNumCycles));
	}
	else 
		yAxisScale = ((yApexPoint - topYTick) / (double)iYNLNumCycles);

	if (bXAxisLinear)
		xAxisScale = ((rightXTick - xApexPoint) / ((double)(maxXTick-minXTick)*iXLNumCycles));
	else
		xAxisScale = ((rightXTick - xApexPoint) / (double)iXNLNumCycles);

	m_bSelectedData = false;

	CObList* pSeries;
	for (int iIndex = 0; iIndex < 2; iIndex++)
	{
		if (iIndex == 0)
			pSeries = graphSeries;
		else
			pSeries = graphRSeries;
		for(pos = pSeries->GetHeadPosition(); pos != NULL; pSeries->GetNext(pos))
		{
			tmpSeries = (CGraphSeries*)pSeries->GetAt(pos);
			if (tmpSeries->GetDoDisplay() && tmpSeries->GetSelectable())
			{

				int iDataPoints = tmpSeries->GetDataCount();
				CString cLabel = tmpSeries->GetLabel();
				CString cLabelX = tmpSeries->GetLabelX();
				CString cLabelY = tmpSeries->GetLabelY();

				for (int i = 0; i < iDataPoints; i++)
				{ 
					double xValue, yValue;
					double dOriginalX, dOriginalY;
					tmpSeries->IterateGet(&xValue,&yValue);
					dOriginalX = xValue;
					dOriginalY = yValue;

					double seriesSpace;
					if (graphType==LINE_GRAPH)
					{
						seriesSpace = (double)xAxisWidth / graphXAxisLegend.GetLegendCount();
						dataXLocation = (int)(xApexPoint + ((xValue+1) * seriesSpace) - (seriesSpace /2));
					}
					else
					{
						if (bXAxisLinear)
						{
							xValue -= minXTick;
							dataXLocation = (int)(xValue * xAxisScale) + xApexPoint;
						}
						else
						{
							double tmpValue = log10(xValue)-iXNLBaseCycle;
							dataXLocation = xApexPoint + (int)(tmpValue * xAxisScale);
						}
					}

					if (bYAxisLinear)
					{
						if (iIndex == 0)
							dataYLocation = yApexPoint - (int)((yValue-minYTick) * yAxisScale+1);
						else
							dataYLocation = yApexPoint - (int)((yValue-minYRTick) * yRAxisScale+1);
					}
					else
					{
						double tmpValue;
						if (iIndex == 0)
							tmpValue = log10(yValue)-iYNLBaseCycle;
						else
							tmpValue = log10(yValue)-iYRNLBaseCycle;
						dataYLocation = yApexPoint - (int)(tmpValue * yAxisScale);
						xValue = pow(10.0,(double)iXNLBaseCycle);
					}

					if (dataXLocation > xAxisWidth + xApexPoint)
						dataXLocation = xAxisWidth + xApexPoint + iXMarkerSize;
					else	 
					{
						if (bXAxisLinear)
						{
							if (dataXLocation < xApexPoint) 
								dataXLocation = xApexPoint - iXMarkerSize;
						}
						else
						{
							if ((dataXLocation < xApexPoint) || (xValue < pow(10.0,(double)iXNLBaseCycle-1)))
							{
								dataXLocation = xApexPoint - iXMarkerSize;
							}
						}
					}

					if (dataYLocation > yApexPoint)
					{
						dataYLocation = yApexPoint + iYMarkerSize;
						//bDoRed = true;
					}
					else
					{
						//check bottom of graph
						if (bYAxisLinear)
						{
							if (dataYLocation > yApexPoint)
							{
								dataYLocation = yApexPoint + iYMarkerSize;
								//bDoRed = true;
							}
						}
						else
						{
							if (iIndex == 0)
							{
								if (yValue < pow(10.0,(double)iYNLBaseCycle-1))
								{
									dataYLocation = yApexPoint + iYMarkerSize;
									//bDoRed = true;
								}
							}
							else
							{
								if (yValue < pow(10.0,(double)iYRNLBaseCycle-1))
								{
									dataYLocation = yApexPoint + iYMarkerSize;
									//bDoRed = true;
								}
							}
						}
						if (dataYLocation < (topYTick-1))
						{
							dataYLocation = topYTick - iYMarkerSize;
							//bDoRed = true;
						}
					}

					CRect cRect(dataXLocation-iXMarkerSize, dataYLocation-iYMarkerSize, 
						dataXLocation+iXMarkerSize, dataYLocation+iYMarkerSize);

					if (cRect.PtInRect(cPoint))
					{
						*dX = m_dSelectedX = dOriginalX;
						*dY = m_dSelectedY = dOriginalY;
						m_cSelectedLabel = cLabel;
						m_cSelectedLabelX = cLabelX;
						m_cSelectedLabelY = cLabelY;

						m_bSelectedData = true;
					}
					//					if (cPoint.x == dataXLocation)
					//					{
					//						*dX = m_dSelectedX = dOriginalX;
					//						*dY = m_dSelectedY = dOriginalY;
					//						m_cSelectedLabel = cLabel;
					//						m_bSelectedData = true;
					//					}
				}
			}
		}
	}
	TRACE("CGraph::DrawAxis end\n");
	return m_bSelectedData;
}

void CGraph::AddSeries(CGraphSeries* dataSet, bool bToRight/*=false*/)
{
	TRACE("CGraph::AddSeries\n");

	if (bToRight)
		graphRSeries->AddTail(dataSet);
	else
	{
		int numData = 0;
		numData = dataSet->GetDataCount();

		if(numData > seriesSize)
			seriesSize = numData;
		graphSeries->AddTail(dataSet);
	}
}

void CGraph::ToggleDisplayEqGroup(int iGroup)
{
	TRACE0("CGraph::ToggleDisplayEqGroup\n");

	//step through each data series and call its toggledisplayeqgroup
	CObList* pSeries;
	for (int iIndex = 0; iIndex < 2; iIndex++)
	{
		if (iIndex == 0)
			pSeries = graphSeries;
		else
			pSeries = graphRSeries;
		POSITION pos;
		CGraphSeries* tmpSeries;
		for(pos = pSeries->GetHeadPosition(); pos != NULL; pSeries->GetNext(pos))
		{
			tmpSeries = (CGraphSeries*)pSeries->GetAt(pos);
			tmpSeries->ToggleDisplayOnEqGroup(iGroup);
		}
	}

	graphLegend.ToggleLegendDoDraw(iGroup);
}

void CGraph::SetXAxisLabel(CString label)
{
	TRACE1("CGraph::SetXAxisLabel(%s)\n",label);

	axisXLabel = label;
	xAxisLabelLength = axisXLabel.GetLength();
}

void CGraph::SetYAxisLabel(CString label, COLORREF cColor)
{
	TRACE2("CGraph::SetYAxisLabel(%s, %d)\n",label, cColor);

	axisYLabel = label;
	axisYLabelColor = cColor;
	yAxisLabelLength = axisYLabel.GetLength();
}

void CGraph::SetYRAxisLabel(CString label, COLORREF cColor)
{
	TRACE2("CGraph::SetYRAxisLabel(%s, %d)\n",label, cColor);

	axisYRLabel = label;
	axisYRLabelColor = cColor;
	yRAxisLabelLength = axisYRLabel.GetLength();
}

void CGraph::DrawSeries(CDC* pDC)
{
	TRACE1("CGraph::DrawSeries %d\n",graphType);
	switch(graphType)
	{
	case BAR_GRAPH :
		DrawBarSeries(pDC);
		break;
	case LINE_GRAPH :
		DrawLineSeries(pDC);
		break;
	case PIE_GRAPH :
		DrawPieSeries(pDC);
		break;
	case SCATTER_GRAPH :
		DrawScatterSeries(pDC);
		break;
	default :
		AfxMessageBox("No graph type to display");
		break;
	}
}

void CGraph::SetLegend(int datagroup, CString label, COLORREF cColor, int iStyle, bool bDoShow)
{
	TRACE3("CGraph::SetLegend(%d, %s, %d, ",datagroup, label, cColor);
	TRACE2("%d, %d)\n",iStyle, bDoShow);

	graphLegend.SetLegendText(datagroup, label, cColor, iStyle, bDoShow);
	graphHasLegend = TRUE;
	bHasLegendInfo = true;
	if(legendMaxText < label.GetLength())
		legendMaxText = label.GetLength();
}

void CGraph::SetXAxisLegend(CString label, COLORREF cColor)
{
	TRACE2("CGraph::SetXAxisLegend(%s, %d)\n", label, cColor);

	graphXAxisLegend.SetLegendText(0,label, cColor, 0);
}

int CGraph::DrawLegend(CDC* pDC)
{
	TRACE0("CGraph::DrawLegend\n");

	//determine size of legend
	int legendL, legendT, legendR, legendB;
	if (graphHasLegend)
	{
		CFont legendTitleFont, legendTextFont;
		legendTitleFont.CreateFont(-m_yFontSize/2, m_xFontSize/2, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
		legendTextFont.CreateFont(-m_yFontSize*3/8, m_xFontSize*3/8, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");

		CFont* pOldFont = (CFont*) pDC->SelectObject(&legendTextFont);
		int iTotalHeight;
		int iMaxWidth = 0;
		CString cText;
		CSize cSize;
		int i;
		for (i = 0; i < graphLegend.GetLegendCount(); i++)
		{
			cText = graphLegend.GetLegendText(i);		
			cSize = pDC->GetTextExtent(cText);
			iMaxWidth = max(iMaxWidth, cSize.cx);
		}

		pDC->SelectObject(&legendTitleFont);
		cSize = pDC->GetTextExtent(" Legend ");
		iMaxWidth = max(iMaxWidth, cSize.cx);
		cSize = pDC->GetTextExtent("Selected");
		iMaxWidth = max(iMaxWidth, cSize.cx);
		iMaxWidth += maxWidth*2/100;
		iTotalHeight = cSize.cy * (graphLegend.GetLegendCount() + 2);

		legendT = (maxHeight - iTotalHeight) / 2;
		legendB = legendT + iTotalHeight;
		legendR = maxWidth;
		legendL = legendR - iMaxWidth*10/9;
		legendWidth = legendR - legendL;


		pDC->Rectangle(legendL, legendT, legendR, legendB);
		cLegendRect = CRect(legendL, legendT, legendR, legendB);
		cSize = pDC->GetTextExtent(" Legend ");
		pDC->TextOut(legendL + (legendWidth - cSize.cx) / 2,legendT+1," Legend ");
		COleDateTime cNow = COleDateTime::GetCurrentTime();
		CString cString = cNow.Format("%Y.%m.%d");

		pDC->SelectObject(legendTextFont);
		CSize cSizex = pDC->GetTextExtent(cString);
		pDC->TextOut(legendL + (legendWidth - cSizex.cx) / 2, legendT+1+cSize.cy,cString);

		CBrush wbrush(RGB(255,255,255));

		CSize cSizeMarker;
		cSizeMarker.cx = maxWidth/100;
		cSizeMarker.cy = maxHeight/100;
		for(i = 0; i < 100; i++) cLegendContents[i].bValid = false;
		COLORREF bgColor = pDC->GetBkColor();
		for(i = 0; i < graphLegend.GetLegendCount(); i++)
		{
			COLORREF cOldColor = pDC->GetTextColor();
			if (graphLegend.GetLegendDoDraw(i))
			{
				pDC->SetBkColor(bgColor);
			}
			else
			{
				pDC->SetBkColor(GRAY);
				pDC->SetTextColor(GRAY);
			}

			pDC->TextOut(legendL+maxWidth/200+1, legendT + ((i+2)*cSize.cy),graphLegend.GetLegendText(i));
			pDC->SetTextColor(cOldColor);

			if (i < 100)
			{
				cLegendContents[i].cRect = CRect(
					legendL, 
					legendT + ((i+2)*cSize.cy),
					legendR,
					legendT + ((i+3)*cSize.cy));
				cLegendContents[i].bValid = true;
			}

			//draw marker
			COLORREF barColor;
			barColor = graphLegend.GetLegendColor(i);
			CBrush brush (barColor);
			CPen pen(PS_SOLID,1,barColor);
			CBrush* pOldBrush;
			CPen* pOldPen;
			pOldBrush = pDC->SelectObject(&brush);
			pOldPen = pDC->SelectObject(&pen);
			DrawMarker(pDC, legendR - cSizeMarker.cx-2, cSizeMarker.cx, legendT + (i+2)*cSize.cy+cSize.cy/2, cSizeMarker.cy, graphLegend.GetLegendStyle(i), &wbrush);

			pDC->SelectObject(pOldBrush);
			pDC->SelectObject(pOldPen);
		}

		pDC->SetBkColor(bgColor);

		if (m_bSelectedData)
		{
			int iLeftText = legendL+maxWidth/200+1;
			pDC->Rectangle(legendL,legendB+2,legendR,legendB+4*cSize.cy+2);
			CString cText;
			pDC->SelectObject(legendTitleFont);
			cSize = pDC->GetTextExtent("Selected");
			pDC->TextOut(legendL + (legendWidth - cSize.cx) / 2, legendB + 3,"Selected");
			pDC->SelectObject(legendTextFont);
			cSize = pDC->GetTextExtent("MyMy");
			i = 2;
			if (!m_cSelectedLabel.IsEmpty())
			{
				pDC->TextOut(iLeftText, legendB + cSize.cy*i+3,m_cSelectedLabel);
				i++;
			}
			if (!m_cSelectedLabelX.IsEmpty())
				cText.Format("%s:%g",m_cSelectedLabelX,m_dSelectedX);
			else
				cText.Format("Bin:%g",m_dSelectedX);
			pDC->TextOut(iLeftText, legendB + cSize.cy*i+3,cText);
			i++;
			if (!m_cSelectedLabelY.IsEmpty())
				cText.Format("%s:%g",m_cSelectedLabelY,m_dSelectedY);
			else
				cText.Format("Count:%g",m_dSelectedY);
			pDC->TextOut(iLeftText, legendB + cSize.cy*i+3,cText);

		}
		pDC->SelectObject(pOldFont);
	}
	else
	{
		legendL = maxWidth;
		legendR = graphRect.right;
		legendT = 0;
		legendB = graphRect.bottom;
		cLegendRect = CRect(legendL-maxWidth/100, legendT, legendR, legendB);
		//		cHelpRect = CRect(0,0,0,0);
		TRACE0("CGraph::DrawLegend else\n");
		return legendL;
	}
	TRACE0("CGraph::DrawLegend end\n");
	return legendL - maxWidth/100;
}

void CGraph::SetGraphTitle(CString title)
{
	TRACE1("CGraph::SetGraphTitle(%s)\n",title);

	graphTitle = title;
}

CString CGraph::GetGraphTitle()
{
	TRACE0("CGraph::GetGraphTitle\n");

	return graphTitle;
}
CString CGraph::GetGraphTitle2()
{
	TRACE0("CGraph::GetGraphTitle2\n");

	return graphTitleLine2;
}

void CGraph::SetGraphTitleLine2(CString title)
{
	TRACE1("CGraph::SetGraphTitleLine2(%s)\n",title);

	graphTitleLine2 = title;
}

void CGraph::SetXTickFontSize(int size)
{
	TRACE1("CGraph::SetXTickFontSize(%d)\n",size);	

	xTickFontSize = size;
}

void CGraph::SetYTickFontSize(int size)
{
	TRACE1("CGraph::SetYTickFontSize(%d)\n",size);

	yTickFontSize = size;
}

void CGraph::SetLegendFontSize(int size)
{
	TRACE1("CGraph::SetLegendFontSize(%d)\n",size);

	legendFontSize = size;
}


int CGraph::PrintGraph(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	TRACE0("CGraph::PrintGraph\n");

	DrawGraph(pDC);
	return 0;
}

void CGraph::SetMargins(int top, int bottom, int left, int right, int graphTop)
{
	TRACE3("CGraph::SetMargins(%d, %d, %d, ",top, bottom, left);
	TRACE2("%d, %d\n", right, graphTop);

	//page margins for portrait or landscape distinction
	topMargin = top;
	bottomMargin = bottom;
	leftMargin = left;
	rightMargin = right;

}

void CGraph::SetHGridLines(BOOL hasHGridLines)
{
	TRACE1("CGraph::SetHGridLines(%d)\n",hasHGridLines);

	graphHasHGridLines = hasHGridLines;
}

void CGraph::SetVGridLines(BOOL hasVGridLines)
{
	TRACE1("CGraph::SetVGridLines(%d)\n",hasVGridLines);

	graphHasVGridLines = hasVGridLines;
}

void CGraph::SetGridLines(BOOL hasGridLines)
{
	TRACE1("CGraph::SetGridLines(%d)\n",hasGridLines);

	//	graphHasGridLines = hasGridLines;
	graphHasHGridLines = hasGridLines;
	graphHasVGridLines = hasGridLines;
}

void CGraph::SetGraphAlignment(int alignment)
{
	TRACE1("CGraph::SetGraphAlignment(%d)\n",alignment);

	graphAlignment = alignment;
}

void CGraph::SetTickLimits(double minTickValue, double maxTickValue, double tickStep)
{
	TRACE3("CGraph::SetTickLimits(%lf, %lf, %lf)\n", minTickValue, maxTickValue, tickStep);

	m_minXTickValue = m_minYTickValue = m_minYRTickValue = minTickValue;
	m_maxXTickValue = m_maxYTickValue = m_maxYRTickValue = maxTickValue;
	m_XtickStep = tickStep;
	m_YtickStep = m_YRtickStep = (maxTickValue-minTickValue)/tickStep;
}

void CGraph::SetTickLimits(int minTickValue, int maxTickValue, int tickStep)
{
	TRACE3("CGraph::SetTickLimits(%d, %d, %d)\n", minTickValue, maxTickValue, tickStep);

	m_minXTickValue = m_minYTickValue = m_minYRTickValue = (double)minTickValue;
	m_maxXTickValue = m_maxYTickValue = m_maxYRTickValue = (double)maxTickValue;
	m_tickStep = tickStep;
	m_XtickStep = tickStep;
	m_YtickStep = m_YRtickStep = ((double)maxTickValue-(double)minTickValue)/(double)tickStep;
}

void CGraph::SetXTickLimits(double minTickValue, double maxTickValue, double tickStep)
{
	TRACE3("CGraph::SetXTickLimits(%lf, %lf, %lf)\n", minTickValue, maxTickValue, tickStep);

	m_minXTickValue = minTickValue;
	m_maxXTickValue = maxTickValue;
	//	m_XtickStep = (maxTickValue-minTickValue)/tickStep;
	m_XtickStep = tickStep;
	m_XtickStepValue = (maxTickValue-minTickValue)/tickStep;
}

void CGraph::SetXTickLimits(int minTickValue, int maxTickValue, int tickStep)
{
	TRACE3("CGraph::SetXTickLimits(%d, %d, %d)\n", minTickValue, maxTickValue, tickStep);

	m_minXTickValue = minTickValue;
	m_maxXTickValue = maxTickValue;
	//	m_XtickStep = (maxTickValue-minTickValue)/tickStep;
	m_XtickStep = tickStep;
	m_XtickStepValue = (maxTickValue-minTickValue)/tickStep;
}

void CGraph::SetYTickLimits(double minTickValue, double maxTickValue, double tickStep)
{
	TRACE3("CGraph::SetYTickLimits(%lf, %lf, %lf)\n", minTickValue, maxTickValue, tickStep);

	m_minYTickValue = minTickValue;
	m_maxYTickValue = maxTickValue;
	m_tickStep = tickStep;
	m_YtickStep = (maxTickValue-minTickValue)/tickStep;
	// !! ADDED JFL
	m_minYRTickValue = minTickValue;
	m_maxYRTickValue = maxTickValue;
	// !! ADDED JFL
	m_YRtickStep = (m_maxYRTickValue-m_minYRTickValue)/tickStep;
}

void CGraph::SetYTickLimits(int minTickValue, int maxTickValue, int tickStep)
{
	TRACE3("CGraph::SetYTickLimits(%d, %d, %d)\n", minTickValue, maxTickValue, tickStep);

	m_minYTickValue = minTickValue;
	m_maxYTickValue = maxTickValue;
	m_tickStep = tickStep;
	m_YtickStep = (maxTickValue-minTickValue)/tickStep;
	// !! ADDED JFL
	m_minYRTickValue = minTickValue;
	m_maxYRTickValue = maxTickValue;
	// !! ADDED JFL
	m_YRtickStep = (m_maxYRTickValue-m_minYRTickValue)/tickStep;
}
void CGraph::SetYRTickLimits(double minTickValue, double maxTickValue, double tickStep)
{
	TRACE2("CGraph::SetYRTickLimits(%lf, %lf)\n",minTickValue, maxTickValue);

	UNREFERENCED_PARAMETER(tickStep);
	m_minYRTickValue = minTickValue;
	m_maxYRTickValue = maxTickValue;
	m_YRtickStep = (maxTickValue-minTickValue)/m_tickStep;
}

void CGraph::SetYRTickLimits(int minTickValue, int maxTickValue, int tickStep)
{
	TRACE2("CGraph::SetYRTickLimits(%d, %d\n",minTickValue, maxTickValue);

	UNREFERENCED_PARAMETER(tickStep);
	m_minYRTickValue = minTickValue;
	m_maxYRTickValue = maxTickValue;
	m_YRtickStep = (maxTickValue-minTickValue)/m_tickStep;
}

void CGraph::DrawBarSeries(CDC* pDC)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int barL, barT, barR, barB;
	int tickXLocation, tickYLocation;
	double seriesSpace;
	int barHeight;
	POSITION pos;
	CGraphSeries* tmpSeries;
	int x, y, s;

	if(graphAlignment)
	{
		//determine width of barchart data blocks
		seriesSpace = (double)xAxisWidth / graphSeries->GetCount();
		barWidth = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barWidth;

		pos = graphSeries->GetHeadPosition();

		switch(graphQuadType)
		{
		case 1 :
			for(x = 1; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = (int)(xApexPoint + ((x * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					if(tmpSeries->GetData(s) > -1)
					{
						double dataScale = 0.00;
						dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
						barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minYTick) > 0) && (barHeight < 1))
							barHeight = (int)dataScale;

						barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
						if(barHeight > yAxisHeight)
							barT = yApexPoint - yAxisHeight;
						else
							barT = yApexPoint - barHeight;
						barR = barL + barWidth;
						barB = yApexPoint;

						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
			}
			break;
		case 2 :
			for(x = 1; x <= graphSeries->GetCount() / 2; x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);
					if(barHeight < 1)
						barHeight = (int)dataScale;

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight > (yAxisHeight / 2))
						barT = yApexPoint - (yAxisHeight / 2);
					else
						barT = yApexPoint - barHeight;
					barR = barL + barWidth;
					barB = yApexPoint;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			//draw at the 0 location
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			tickXLocation = xApexPoint;
			for(s = 0; s < seriesSize; s++)
			{
				double dataScale = 0.00;
				dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
				barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);
				if(((tmpSeries->GetData(s) - minYTick) > 0) && (barHeight < 1))
					barHeight = (int)dataScale;

				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
				if(barHeight > yAxisHeight)
					barT = yApexPoint - yAxisHeight;
				else
					barT = yApexPoint - barHeight;
				barR = barL + barWidth;
				barB = yApexPoint;

				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(barL, barT, barR, barB);
				pDC->SelectObject(pOldBrush);
			}
			x++;

			for(; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minYTick) > 0) && (barHeight < 1))
						barHeight = (int)dataScale;

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight > (yAxisHeight / 2))
						barT = yApexPoint - (yAxisHeight / 2);
					else
						barT = yApexPoint - barHeight;
					barR = barL + barWidth;
					barB = yApexPoint;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		case 3 :
			for(x = 1; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = (int)(xApexPoint + ((x * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight < 0)
					{
						if((0 - barHeight) > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					else
					{
						if(barHeight > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					barR = barL + barWidth;
					barB = yApexPoint;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		case 4 :
			for(x = 1; x <= graphSeries->GetCount() / 2; x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight < 0)
					{
						if((0 - barHeight) > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					else
					{
						if(barHeight > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					barR = barL + barWidth;
					barB = yApexPoint;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			//draw at the 0 location
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			tickXLocation = xApexPoint;
			for(s = 0; s < seriesSize; s++)
			{
				double dataScale = 0.00;
				dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
				barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);
				if(((tmpSeries->GetData(s) - minYTick) > 0) && (barHeight < 1))
					barHeight = (int)dataScale;

				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
				if(barHeight < 0)
				{
					if((0 - barHeight) > (yAxisHeight / 2))
						barT = yApexPoint - (yAxisHeight / 2);
					else
						barT = yApexPoint - barHeight;
				}
				else
				{
					if(barHeight > (yAxisHeight / 2))
						barT = yApexPoint - (yAxisHeight / 2);
					else
						barT = yApexPoint - barHeight;
				}
				barR = barL + barWidth;
				barB = yApexPoint;

				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(barL, barT, barR, barB);
				pDC->SelectObject(pOldBrush);
			}
			x++;

			for(; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickXLocation = (int)(xApexPoint - (xAxisWidth / 2.0) + ((x * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxYTick - minYTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minYTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minYTick) > 0) && (barHeight < 1))
						barHeight = (int)dataScale;

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight < 0)
					{
						if((0 - barHeight) > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					else
					{
						if(barHeight > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					barR = barL + barWidth;
					barB = yApexPoint;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		}

	}
	else
	{
		//determine width of barchart data blocks
		seriesSpace = yAxisHeight / graphSeries->GetCount();
		barHeight = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barHeight;

		pos = graphSeries->GetHeadPosition();

		switch(graphQuadType)
		{
		case 1 :
			for(y = 1; y <= graphSeries->GetCount(); y++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickYLocation = (int)(yApexPoint - ((y * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((xAxisWidth - 10) * 1.00) / (maxXTick - minXTick);

					barWidth = (int)(((tmpSeries->GetData(s) - minXTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minXTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					barL = xApexPoint;
					barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
					barB = barT + barHeight;
					if(barWidth > xAxisWidth)
						barR = xApexPoint + xAxisWidth;
					else
						barR = xApexPoint + barWidth;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		case 2 :
			for(y = 1; y <= graphSeries->GetCount(); y++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickYLocation = (int)(yApexPoint - ((y * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((xAxisWidth - 10) * 1.00) / (maxXTick - minXTick);

					barWidth = (int)(((tmpSeries->GetData(s) - minXTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minXTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					barL = xApexPoint;
					barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
					barB = barT + barHeight;
					if(barWidth < 0)
					{
						if((0 - barWidth) > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}
					else
					{
						if(barWidth > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		case 3 :
			for(y = 1; y <= graphSeries->GetCount(); y++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((xAxisWidth - 10) * 1.00) / (maxXTick - minXTick);

					barWidth = (int)(((tmpSeries->GetData(s) - minXTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minXTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					barL = xApexPoint;
					barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
					barB = barT + barHeight;
					if(barWidth > xAxisWidth)
						barR = xApexPoint + xAxisWidth;
					else
						barR = xApexPoint + barWidth;

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		case 4 :
			for(y = 1; y <= graphSeries->GetCount() / 2; y++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((xAxisWidth - 10) * 1.00) / (maxXTick - minXTick);

					barWidth = (int)(((tmpSeries->GetData(s) - minXTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minXTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					barL = xApexPoint;
					barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
					barB = barT + barHeight;
					if(barWidth < 0)
					{
						if((0 - barWidth) > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}
					else
					{
						if(barWidth > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			//draw the 0 location
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			tickYLocation = yApexPoint;
			for(s = 0; s < seriesSize; s++)
			{
				double dataScale = 0.00;
				dataScale = ((xAxisWidth - 10) * 1.00) / (maxXTick - minXTick);

				barWidth = (int)(((tmpSeries->GetData(s) - minXTick) * 1.00) * dataScale);
				if(((tmpSeries->GetData(s) - minXTick) > 0) && (barWidth < 1))
					barWidth = (int)dataScale;

				barL = xApexPoint;
				barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
				barB = barT + barHeight;
				if(barWidth < 0)
				{
					if((0 - barWidth) > (xAxisWidth / 2))
						barR = xApexPoint + (xAxisWidth / 2);
					else
						barR = xApexPoint + barWidth;
				}
				else
				{
					if(barWidth > (xAxisWidth / 2))
						barR = xApexPoint + (xAxisWidth / 2);
					else
						barR = xApexPoint + barWidth;
				}

				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(barL, barT, barR, barB);
				pDC->SelectObject(pOldBrush);
			}
			y++;

			for(; y <= graphSeries->GetCount(); y++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				tickYLocation = (int)(yApexPoint - (yAxisHeight / 2.0) + ((y * seriesSpace) - (seriesSpace / 2.0)));

				for(s = 0; s < seriesSize; s++)
				{
					double dataScale = 0.00;
					dataScale = ((xAxisWidth - 10) * 1.00) / (maxXTick - minXTick);

					barWidth = (int)(((tmpSeries->GetData(s) - minXTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minXTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					barL = xApexPoint;
					barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
					barB = barT + barHeight;
					if(barWidth < 0)
					{
						if((0 - barWidth) > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}
					else
					{
						if(barWidth > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
			}
			break;
		}

	}
}

void CGraph::DrawLineSeries(CDC* pDC)
{
	TRACE("CGraph::DrawLineSeries\n");

	POSITION pos;
	CGraphSeries* tmpSeries;
	double yAxisScale = 0.0;
	double yRAxisScale = 0.0;
	double xAxisScale = 0.0;
	int dataXLocation = 0;
	int dataYLocation = 0;
	if (bYAxisLinear)
	{
		yAxisScale = ((yApexPoint - topYTick) / ((double)(maxYTick-minYTick)*iYLNumCycles));
		yRAxisScale = ((yApexPoint - topYTick) / ((double)(maxYRTick-minYRTick)*iYLNumCycles));
	}
	else 
		yAxisScale = ((yApexPoint - topYTick) / (double)iYNLNumCycles);

	xAxisScale = ((rightXTick - xApexPoint) / ((double)maxXTick*iXLNumCycles));
	CBrush wBrush(WHITE);
	CBrush bBrush(BLACK);
	CBrush rBrush(RED);
	CBrush* pOldBrush;
	CBrush* pPrevBrush;
	pPrevBrush = &wBrush;
	CPen* pOldPen;

	CPen rLinePen (PS_SOLID, 1, RED);
	CPen bLinePen (PS_SOLID, 1, BLACK);

	pOldBrush = pDC->SelectObject(&bBrush);
	pOldPen = pDC->SelectObject(&bLinePen);

	CObList* pSeries;
	for (int iIndex = 0; iIndex < 2; iIndex++)
	{
		if (iIndex == 0)
			pSeries = graphSeries;
		else 
			pSeries = graphRSeries;

		for(pos = pSeries->GetHeadPosition(); pos != NULL; pSeries->GetNext(pos))
		{
			tmpSeries = (CGraphSeries*)pSeries->GetAt(pos);

			int iStyle = tmpSeries->GetStyle();
			int iDataPoints = tmpSeries->GetDataCount();
			bool bDoLines = tmpSeries->GetDoLines();
			CString cLabel = tmpSeries->GetLabel();
			COLORREF cColor = tmpSeries->GetColor();

			bool bDoDisplay = tmpSeries->GetDoDisplay();

			//			CPen cPen(PS_SOLID,1,cColor);
			//now have x and y location of center of ellipse
			CBrush Brush(cColor);
			CBrush RedBrush(RED);
			CPen LinePen(PS_SOLID, 1, cColor);
			CPen RedLinePen(PS_SOLID, 1, RED);

			int dataPrevXLocation = 0;
			int dataPrevYLocation = 0;

			if (bDoDisplay)
			{
				bool bPrevDoRed = false;

				for (int i = 0; i < iDataPoints; i++)
				{
					double xValue, yValue;
					tmpSeries->IterateGet(&xValue,&yValue);

					double seriesSpace = (double)xAxisWidth / graphXAxisLegend.GetLegendCount();
					dataXLocation = (int)(xApexPoint + ((xValue+1) * seriesSpace) - (seriesSpace /2));
					if (bYAxisLinear)
					{
						if (iIndex==0)
							dataYLocation = yApexPoint - (int)((yValue-minYTick) * yAxisScale+1);
						else
							dataYLocation = yApexPoint - (int)((yValue-minYRTick) * yRAxisScale+1);
					}
					else
					{
						double tmpValue;
						if (iIndex==0)
							tmpValue = log10(yValue)-iYNLBaseCycle;
						else
							tmpValue = log10(yValue)-iYRNLBaseCycle;

						dataYLocation = yApexPoint - (int)(tmpValue * yAxisScale+1);
						xValue = pow(10.0,(double)iXNLBaseCycle);
					}

					bool bDoRed = false;

					if (dataXLocation > (xAxisWidth + xApexPoint))
					{
						dataXLocation = xAxisWidth + iXMarkerSize;
						bDoRed = true;
					}
					else 
					{
						if (bXAxisLinear)
						{
							if (dataXLocation < xApexPoint) 
							{
								dataXLocation = xApexPoint - iXMarkerSize;
								bDoRed = true;
							}
						}
						else
						{
							if (xValue < pow(10.0,(double)iXNLBaseCycle-1))
							{
								dataXLocation = xApexPoint - iXMarkerSize;
								bDoRed = true;
							}
						}
					}

					if (dataYLocation > yApexPoint)
					{
						dataYLocation = yApexPoint + iYMarkerSize;
						bDoRed = true;
					}
					else
					{
						//check bottom of graph
						if (bYAxisLinear)
						{
							if (dataYLocation > yApexPoint)
							{
								dataYLocation = yApexPoint + iYMarkerSize;
								bDoRed = true;
							}
						}
						else
						{
							if (iIndex == 0)
							{
								if (yValue < pow(10.0,(double)iYNLBaseCycle-1))
								{
									dataYLocation = yApexPoint + iYMarkerSize;
									bDoRed = true;
								}
							}
							else
							{
								if (yValue < pow(10.0,(double)iYRNLBaseCycle-1))
								{
									dataYLocation = yApexPoint + iYMarkerSize;
									bDoRed = true;
								}
							}
						}
						if (dataYLocation < (topYTick-1))
						{
							dataYLocation = topYTick - iYMarkerSize;			
							bDoRed = true;
						}

					}


					//					//now have x and y location of center of ellipse
					//					CBrush Brush((bDoRed)?RED:cColor);
					//					CPen LinePen(PS_SOLID, 1, (bDoRed)?RED:cColor);

					pDC->SelectObject(&Brush);
					pDC->SelectObject(&LinePen);

					if(i != 0 && bDoLines)
					{
						if (bPrevDoRed)
						{
							//							CBrush Brush(RED);
							//							CPen LinePen(PS_SOLID, 1, RED);
							pDC->SelectObject(&RedBrush);
							pDC->SelectObject(&RedLinePen);
							pDC->MoveTo(dataPrevXLocation, dataPrevYLocation);
							pDC->LineTo(dataXLocation, dataYLocation);
							DrawMarker(pDC, dataPrevXLocation, iXMarkerSize, dataPrevYLocation, iYMarkerSize, iStyle, &Brush);							
						}
						else
						{
							pDC->MoveTo(dataPrevXLocation, dataPrevYLocation);
							pDC->LineTo(dataXLocation, dataYLocation);
							//							CBrush Brush(cColor);
							//							CPen LinePen(PS_SOLID, 1, cColor);
							//							pDC->SelectObject(&Brush);
							//							pDC->SelectObject(&LinePen);
							DrawMarker(pDC, dataPrevXLocation, iXMarkerSize, dataPrevYLocation, iYMarkerSize, iStyle, &Brush);
						}
						if (bDoRed)
						{
							pDC->SelectObject(&RedBrush);
							pDC->SelectObject(&RedLinePen);
						}
						else
						{
							pDC->SelectObject(&Brush);
							pDC->SelectObject(&LinePen);
						}
					}
					//now draw marker
					dataPrevXLocation = dataXLocation;
					dataPrevYLocation = dataYLocation;
					pPrevBrush = &Brush;
					bPrevDoRed = bDoRed;

					DrawMarker(pDC,dataXLocation,iXMarkerSize,dataYLocation,iYMarkerSize,tmpSeries->GetStyle(),&Brush);
					pDC->SelectObject(pOldPen);
					pDC->SelectObject(pOldBrush);

				}
			}
		}
	}
}

void CGraph::DrawPieSeries(CDC* pDC)
{
	double dataSum = 0.00;  //for storing cumulative sum
	double labelData = 0.00;
	int lastXLocation = 0;
	int lastYLocation = 0;
	int newXLocation = 0;
	int newYLocation = 0;
	int labelXLocation = 0;
	int labelYLocation = 0;
	double percent = 0.00;
	double labelPercent = 0.00;
	int degrees;
	int labelDegrees;
	double totalSum = 0.00;
	int deltaXY;
	int labelDeltaXY;
	int radius;
	POSITION pos;
	CGraphSeries* tmpSeries;
	double seriesSpace;
	int labelLineXStart = 0;
	int labelLineYStart = 0;
	int labelLineXEnd = 0;
	int labelLineYEnd = 0;
	int maxLabelWidth;
	int maxLabelHeight;
	TEXTMETRIC tm;

	lastXLocation = 0;
	lastYLocation = 0;

	pDC->GetTextMetrics(&tm);
	maxLabelWidth = tm.tmMaxCharWidth + 10;
	maxLabelHeight = tm.tmHeight + 6;

	//pie labels will be stored in a list and drawn after entire pie
	//is completed.
	CObList *pieLabels;
	CGraphPieLabel *pieLabel;

	//determine width of pie display area
	if(xAxisWidth > yAxisHeight)
		seriesSpace = yAxisHeight / graphSeries->GetCount();
	else
		seriesSpace = xAxisWidth / graphSeries->GetCount();
	seriesSpace -= (3 * maxLabelWidth);  //allows text like 25%  (3 chars)

	//to plot a pie plus labels inside of series space, use the following :
	//(3 * radius) + (2 * label width) = series space 
	//so, 3*radius = series space - (2 * label width)
	// 1 radius = (series space - (2 * label width)) / 3
	radius = (int)(seriesSpace / 3.0);  //pie needs 2 radius, + 1 extra for line to labels = 3 for my divisor

	int centerYPie = (yAxisHeight + 60) / 2;

	pos = graphSeries->GetHeadPosition();
	for(int x = 1; x <= graphSeries->GetCount(); x++)
	{
		pieLabels = new CObList();

		tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
		totalSum = 0;
		for(int s = 0; s < seriesSize; s++)
			totalSum += tmpSeries->GetData(s);
		int pieLeft, pieRight;
		if(graphSeries->GetCount() == 1)
		{
			pieLeft = xApexPoint + (xAxisWidth / 2) - radius;
		}
		else
		{
			pieLeft = xApexPoint + 15 + ((x * 2 - 1) * ((xAxisWidth / graphSeries->GetCount()) / 2)) - radius;
		}
		pieRight = pieLeft + (2 * radius);
		CRect pieRect (pieLeft, 
			centerYPie - radius,
			pieRight, 
			centerYPie + radius);
		int centerXPie = pieLeft + radius;

		//plot series label
		pDC->TextOut(centerXPie - ((tmpSeries->GetLabel().GetLength() * 8) / 2),
			centerYPie + (int)(radius * 1.5) + maxLabelHeight + 15, tmpSeries->GetLabel());
		lastXLocation = pieLeft;
		lastYLocation = centerYPie;

		dataSum = 0;
		for(int s = 0; s < seriesSize; s++)
		{
			if(tmpSeries->GetData(s) > 0)
			{
				int seriesDataValue;
				seriesDataValue = tmpSeries->GetData(s);
				labelData = seriesDataValue / 2;
				dataSum += seriesDataValue;
				percent = (dataSum / totalSum) * 100;
				labelPercent = ((dataSum - labelData) / totalSum) * 100;
				degrees = (int)((360 * percent) / 100);
				labelDegrees = (int)((360 * labelPercent) / 100);

				deltaXY = (degrees * radius) / 90;
				labelDeltaXY = (labelDegrees * radius) / 90;
				//deltaXY is change from start point of pie 0
				//this value is total change.  so if radius is 300
				//and degrees is 270, delta is 300.  The change 
				//would move both x and y 300 pixels.  For x, 100
				//to right is center, another 100 to right edge,
				//100 back to center.  For y, 100 to bottom, 100
				//back to center, 100 to top. thus gives 270 degree
				//rotation.

				//determine destination quadrant...
				//and set newXLocation and newYLocation values...
				//degress / 90 will never exceed 4.
				//this can tell us the quadrant of destination
				int quadrant = degrees / 90;  //truncates decimal
				int labelQuadrant = labelDegrees / 90;

				switch(quadrant)
				{
					//in the computations below, the remarked line is
					//the original computation.  The line above it, for
					//actual use, is the simplified line, and gives the
					//exact same result
				case 0 : newXLocation = pieLeft + deltaXY;
					newYLocation = centerYPie + deltaXY;
					break;
				case 1 : newXLocation = pieLeft + deltaXY;
					newYLocation = centerYPie + (2 * radius) - deltaXY;
					break;
				case 2 : newXLocation = pieLeft + (4 * radius) - deltaXY;
					newYLocation = centerYPie + (2 * radius) - deltaXY;
					break;
				case 3 : newXLocation = pieLeft + (4 * radius) - deltaXY;
					newYLocation = centerYPie - (4 * radius) + deltaXY;
					break;
				case 4 : newXLocation = pieLeft;
					newYLocation = centerYPie - 1;
					break;
				}

				switch(labelQuadrant)
				{
					//after getting X & Y location for label, we take
					//1/2 the delta between x y locations and center pie
				case 0 : labelXLocation = pieLeft + labelDeltaXY;
					labelYLocation = centerYPie + labelDeltaXY;
					labelLineXStart = labelXLocation + ((centerXPie - labelXLocation) / 2);
					labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
					labelLineXEnd = labelXLocation - (radius / 2);
					labelLineYEnd = labelYLocation + (radius / 2);
					break;
				case 1 : labelXLocation = pieLeft + labelDeltaXY;
					labelYLocation = centerYPie + (2 * radius) - labelDeltaXY;
					labelLineXStart = labelXLocation - ((labelXLocation - centerXPie) / 2);
					labelLineYStart = labelYLocation - ((labelYLocation - centerYPie) / 2);
					labelLineXEnd = labelXLocation + (radius / 2);
					labelLineYEnd = labelYLocation + (radius / 2);
					break;
				case 2 : labelXLocation = pieLeft + (4 * radius) - labelDeltaXY;
					labelYLocation = centerYPie + (2 * radius) - labelDeltaXY;
					labelLineXStart = labelXLocation - ((labelXLocation - centerXPie) / 2);
					labelLineYStart = labelYLocation + ((centerYPie - labelYLocation) / 2);
					labelLineXEnd = labelXLocation + (radius / 2);
					labelLineYEnd = labelYLocation - (radius / 2);
					break;
				case 3 : labelXLocation = pieLeft + (4 * radius) - labelDeltaXY;
					labelYLocation = centerYPie - (4 * radius) + labelDeltaXY;
					labelLineXStart = labelXLocation + ((centerXPie - labelXLocation) / 2);
					labelLineYStart = labelYLocation + ((centerYPie - labelYLocation) / 2);
					labelLineXEnd = labelXLocation - (radius / 2);
					labelLineYEnd = labelYLocation - (radius / 2);
					break;
				}

				pieLabel = new CGraphPieLabel();

				pieLabel->lineXStart = labelLineXStart;
				pieLabel->lineYStart = labelLineYStart;
				pieLabel->lineXEnd = labelLineXEnd;
				pieLabel->lineYEnd = labelLineYEnd;
				switch(labelQuadrant)
				{
				case 0 : //label to left of line
					pieLabel->topLeftX = labelLineXEnd - maxLabelWidth;
					pieLabel->topLeftY = labelLineYEnd;
					break;
				case 1 : //label to right of line
					pieLabel->topLeftX = labelLineXEnd + 5;
					pieLabel->topLeftY = labelLineYEnd;
					break;
				case 2 : //label to right of line
					pieLabel->topLeftX = labelLineXEnd + 5;
					pieLabel->topLeftY = labelLineYEnd - maxLabelHeight;
					break;
				case 3 : //label to left of line
					pieLabel->topLeftX = labelLineXEnd - maxLabelWidth;
					pieLabel->topLeftY = labelLineYEnd - maxLabelHeight;
					break;
				}
				pieLabel->labelQuadrant = labelQuadrant;
				int roundPercent = 0;
				roundPercent = (int)((seriesDataValue * 100) / totalSum);
				pieLabel->pieLabel.Format("%d%%", roundPercent);
				pieLabels->AddTail(pieLabel);

				if(s == 0)
					lastYLocation -= 1;

				CPoint p1 (lastXLocation, lastYLocation);
				CPoint p2 (newXLocation, newYLocation);
				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				CPen piePen(PS_SOLID, 1, barColor);
				CPen* pOldPen;
				pOldBrush = pDC->SelectObject(&brush);
				pOldPen = pDC->SelectObject(&piePen);
				pDC->Pie(pieRect, p1, p2); 
				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);

				lastXLocation = newXLocation;
				lastYLocation = newYLocation;
			}
		}
		//draw lines and labels for pies slices
		POSITION labelPos;

		CBrush lineBrush (BLACK);
		CBrush* pOldBrush;
		pOldBrush = pDC->SelectObject(&lineBrush);
		POSITION mainLinePos, checkLinePos;
		mainLinePos = pieLabels->GetHeadPosition();
		int numLinesDrawn = 0;
		CGraphPieLabel* currentLabel;
		CGraphPieLabel* tmpLabel;
		while(mainLinePos != NULL)
		{
			currentLabel = (CGraphPieLabel*)pieLabels->GetNext(mainLinePos);
			int r = 0;
			checkLinePos = pieLabels->GetHeadPosition();
			while(r < numLinesDrawn)
			{
				//check if any overlap in label areas
				tmpLabel = (CGraphPieLabel*)pieLabels->GetAt(checkLinePos);
				if((currentLabel->topLeftX > tmpLabel->topLeftX) &&
					(currentLabel->topLeftX < (tmpLabel->topLeftX + maxLabelWidth)) &&
					(currentLabel->topLeftY > tmpLabel->topLeftY) &&
					(currentLabel->topLeftY < (tmpLabel->topLeftY + maxLabelHeight)))
				{
					//OVERLAP !!!
					//move current label top left position up or down
					//depending on its quadrant
					if(currentLabel->labelQuadrant < 2)
					{
						//move label down to tmplabel topleft + height
						currentLabel->topLeftY = tmpLabel->topLeftY + maxLabelHeight;
						currentLabel->lineYEnd = tmpLabel->lineYEnd + maxLabelHeight;
					}
					else
					{
						//move label up to tmpLabel topleft - height
						currentLabel->topLeftY = tmpLabel->topLeftY - maxLabelHeight;
						currentLabel->lineYEnd = tmpLabel->lineYEnd - maxLabelHeight;
					}
					//reset r value to 0 so it starts over, just in
					//case we moved the label and it overlaps another
					r = 0;
					checkLinePos = pieLabels->GetHeadPosition();
				}
				else
				{
					r++;
					pieLabels->GetNext(checkLinePos);
				}

			}
			//draw the line and label
			pDC->MoveTo(currentLabel->lineXStart, currentLabel->lineYStart);
			pDC->LineTo(currentLabel->lineXEnd, currentLabel->lineYEnd);

			//write the label
			pDC->TextOut(currentLabel->topLeftX, currentLabel->topLeftY,
				currentLabel->pieLabel);
			numLinesDrawn++;
		}

		pDC->SelectObject(pOldBrush);

		//now done, remove everything inside the label list
		labelPos = pieLabels->GetHeadPosition();
		while(labelPos != NULL)
		{
			pieLabel = (CGraphPieLabel*)pieLabels->GetNext(labelPos);
			delete pieLabel;
		}
		delete pieLabels;
	}
}

void CGraph::DrawScatterSeries(CDC* pDC)
{
	//rightXTick and topYTick contain the outer bounds of the axis ticks.
	//So, if maxTick is 100 on both axis lines, then 100, 100 would be top
	//right.  We will use the bounds to see spacing from apex points and then
	//scale so any point can be plotted inside
	TRACE("CGraph::DrawScatterSeries\n");
	double yAxisScale = 0.0;
	double yRAxisScale = 0.0;
	double xAxisScale = 0.0;
	int dataXLocation = 0;
	int dataYLocation = 0;

	if (bYAxisLinear)
	{
		yAxisScale = ((yApexPoint - topYTick) / ((double)(maxYTick-minYTick)*iYLNumCycles));
		if (m_bHasRightAxis)
			yRAxisScale = ((yApexPoint - topYTick) / ((double)(maxYRTick-minYRTick)*iYLNumCycles));
	}
	else 
		yAxisScale = ((yApexPoint - topYTick) / (double)iYNLNumCycles);

	if (bXAxisLinear)
		xAxisScale = ((rightXTick - xApexPoint) / ((double)(maxXTick-minXTick)*iXLNumCycles));
	else
		xAxisScale = ((rightXTick - xApexPoint) / (double)iXNLNumCycles);
	TRACE("CGraph::DrawScatterSeries 2\n");

	CBrush bBrush (BLACK);
	CBrush rBrush(RED);
	CBrush wBrush(WHITE);
	CBrush *pPrevBrush;
	pPrevBrush = &wBrush;

	CPen rLinePen(PS_SOLID,1,RED);
	CPen bLinePen(PS_SOLID,1,BLACK);

	iXMarkerSize = xAxisWidth * iMarkerSize / 400;
	iYMarkerSize = yAxisHeight* iMarkerSize / 400;
	CPen bpen(PS_SOLID,1,BLACK);
	CPen* pOldPen = NULL;
	CBrush* pOldBrush;
	pOldBrush = pDC->SelectObject(&bBrush);
	ASSERT(pOldBrush!=NULL);
	pOldPen = pDC->SelectObject(&bLinePen);
	ASSERT(pOldPen!=NULL);

	POSITION pos;
	CGraphSeries* tmpSeries;
	iXMarkerSize = max(2,iXMarkerSize);
	iYMarkerSize = max(2,iYMarkerSize);

	CObList* pSeries;

	//hack to do left then right
	for (int iIndex = 0; iIndex < 2; iIndex++)
	{
		if (iIndex == 0)
			pSeries = graphSeries;
		else
			pSeries = graphRSeries;
		for(pos = pSeries->GetHeadPosition(); pos != NULL; pSeries->GetNext(pos))
		{
			tmpSeries = (CGraphSeries*)pSeries->GetAt(pos);
			int iStyle = tmpSeries->GetStyle();
			int iDataPoints = tmpSeries->GetDataCount();
			bool bDoLines = tmpSeries->GetDoLines();
			bool bDoClip = tmpSeries->GetClip();
			bool bDoDisplay = tmpSeries->GetDoDisplay();
			COLORREF cColor = tmpSeries->GetColor();
			//CPen cPen(PS_SOLID,1,cColor);
			CBrush Brush(cColor);
			CPen LinePen(tmpSeries->GetLineStyle(), tmpSeries->GetLineWidth(), cColor);

			int dataPrevXLocation = 0;
			int dataPrevYLocation = 0;

			if (bDoDisplay)
			{
				//				bool bPrevDoRed = false;

				for (int i = 0; i < iDataPoints; i++)
				{
					double xValue, yValue;
					tmpSeries->IterateGet(&xValue,&yValue);

					if (bXAxisLinear)
					{
						xValue -= minXTick;
						dataXLocation = (int)(xValue * xAxisScale) + xApexPoint;
					}
					else
					{
						double tmpValue = log10(xValue)-iXNLBaseCycle;
						dataXLocation = xApexPoint + (int)(tmpValue * xAxisScale);
					}

					if (bYAxisLinear)
					{
						if (iIndex == 0)
							dataYLocation = yApexPoint - (int)((yValue - minYTick) * yAxisScale+1);
						else
							dataYLocation = yApexPoint - (int)((yValue - minYRTick) * yRAxisScale+1);
					}
					else
					{
						double tmpValue;
						if (yValue > 0.0) 
						{
							if (iIndex == 0)
								tmpValue = log10(yValue)-iYNLBaseCycle;
							else
								tmpValue = log10(yValue)-iYRNLBaseCycle;
						}
						else
							tmpValue = 0.0;

						dataYLocation = yApexPoint - (int)(tmpValue * yAxisScale+1);
					}

					//select color for out-of-bounds
					bool bDoRed = false;

					if (dataXLocation > (xAxisWidth + xApexPoint))
					{
						if (!bDoClip)
						{
							dataXLocation = xAxisWidth + xApexPoint + iXMarkerSize;
							bDoRed = true;
						}
					}
					else	 
					{
						if (bXAxisLinear)
						{
							if (dataXLocation < xApexPoint) 
							{
								if (!bDoClip)
								{
									dataXLocation = xApexPoint - iXMarkerSize;
									bDoRed = true;
								}
							}
						}
						else
						{
							if ((dataXLocation < xApexPoint) || (xValue < pow(10.0,(double)iXNLBaseCycle-1)))
							{
								if (!bDoClip)
								{
									dataXLocation = xApexPoint - iXMarkerSize;
									bDoRed = true;
								}
							}
						}
					}

					if (!bDoClip)
					{
						if (dataYLocation >= yApexPoint)
						{
							dataYLocation = yApexPoint+iYMarkerSize;			
							bDoRed = true;
						}
						else if (dataYLocation < topYTick)
						{
							dataYLocation = topYTick - iYMarkerSize;			
							bDoRed = true;
						}
					}

					if (bDoRed)
					{
						pDC->SelectObject(&rBrush);
						pDC->SelectObject(&rLinePen);
					}
					else
					{
						pDC->SelectObject(&Brush);
						pDC->SelectObject(&LinePen);
					}

					CRect cRect;
					pDC->GetClipBox(&cRect);
					if (bDoClip)
					{
						CRgn   rgn;
						BOOL bSucceeded = rgn.CreateRectRgn( m_cClipRect.left, m_cClipRect.top, m_cClipRect.right, m_cClipRect.bottom );
						ASSERT( bSucceeded == TRUE );
						pDC->SelectClipRgn( &rgn );
					}
					{
						if (i != 0 && bDoLines)
						{
							pDC->MoveTo(dataPrevXLocation, dataPrevYLocation);
							pDC->LineTo(dataXLocation, dataYLocation);

							DrawMarker(pDC, dataPrevXLocation, iXMarkerSize, dataPrevYLocation, iYMarkerSize, iStyle, &Brush);
						}
						dataPrevXLocation = dataXLocation;
						dataPrevYLocation = dataYLocation;
						pPrevBrush = &wBrush;
					}

					DrawMarker(pDC, dataXLocation, iXMarkerSize, dataYLocation, iYMarkerSize, iStyle, &Brush);
					pDC->SelectObject(pOldPen);
					pDC->SelectObject(pOldBrush);
					CRgn   rgn;
					rgn.CreateRectRgn(cRect.left,cRect.top,cRect.right,cRect.bottom);
					pDC->SelectClipRgn(&rgn);
				}
			}
		}
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	TRACE("CGraph::DrawSeries end\n");
}

void CGraph::DrawMarker(CDC* pDC, int iX, int iDX, int iY, int iDY, int iStyle, CBrush* wbrush)
{
	switch (iStyle) {
	case 0:
		{
			LOGPEN LogPen;
			CPen* cPen = pDC->GetCurrentPen(); 
			cPen->GetLogPen(&LogPen);
			pDC->SetPixelV( iX, iY,  LogPen.lopnColor);
		}
		break;
	case 1:// cross
		pDC->MoveTo(iX - iDX, iY);
		pDC->LineTo(iX + iDX, iY);
		pDC->MoveTo(iX, iY - iDY);
		pDC->LineTo(iX, iY + iDY);
		break;
	case 2:
		pDC->MoveTo(iX - iDX, iY - iDY);
		pDC->LineTo(iX + iDX, iY + iDY);
		pDC->MoveTo(iX - iDX, iY + iDY);
		pDC->LineTo(iX + iDX, iY - iDY);
		break;
	case 3:
		pDC->Ellipse(iX - iDX, iY - iDY,
			iX + iDX, iY + iDY);
		break;
	case 4:// square
		pDC->Rectangle(iX - iDX, iY - iDY,
			iX + iDX, iY + iDY);
		break;
	case 5:// diamond
		{

			CPoint pts[4];
			pts[0].x = iX - iDX;
			pts[0].y = iY;

			pts[1].x = iX;
			pts[1].y = iY - iDY;

			pts[2].x = iX + iDX;
			pts[2].y = iY;

			pts[3].x = iX;
			pts[3].y = iY + iDY;

			pDC->Polygon(pts, 4);
		}
		break;
	case 6:
		pDC->SelectObject(wbrush);
		pDC->Arc(iX - iDX, iY - iDY,
			iX + iDX, iY + iDY,iX-iDX,iY-iDY,iX-iDX,iY-iDY);
		break;
	case 7:
		pDC->MoveTo(iX - iDX, iY - iDY);
		pDC->LineTo(iX + iDX, iY - iDY);
		pDC->LineTo(iX + iDX, iY + iDY);
		pDC->LineTo(iX - iDX, iY + iDY);
		pDC->LineTo(iX - iDX, iY - iDY);
		break;
	case 8:
		pDC->MoveTo(iX,       iY - iDY);
		pDC->LineTo(iX + iDX, iY);
		pDC->LineTo(iX,       iY + iDY);
		pDC->LineTo(iX - iDX, iY);
		pDC->LineTo(iX,       iY - iDY);
		break;
	case 9:
		pDC->MoveTo(iX + iDX, iY - iDY);
		pDC->LineTo(iX,       iY);
		pDC->LineTo(iX + iDX, iY + iDY);
		break;
	case 10:
		pDC->MoveTo(iX - iDX, iY + iDY);
		pDC->LineTo(iX,       iY);
		pDC->LineTo(iX + iDX, iY + iDY);
		break;
	case 11:
		pDC->MoveTo(iX - iDX, iY + iDY);
		pDC->LineTo(iX,       iY);
		pDC->LineTo(iX - iDX, iY - iDY);
		break;
	case 12:
		pDC->MoveTo(iX - iDX, iY - iDY);
		pDC->LineTo(iX,       iY);
		pDC->LineTo(iX + iDX, iY - iDY);
		break;
	case 13:
		{
			CPoint pts[3];
			pts[0].x = iX + iDX;
			pts[0].y = iY - iDY;

			pts[1].x = iX;
			pts[1].y = iY;

			pts[2].x = iX + iDX;
			pts[2].y = iY + iDY;
			pDC->Polygon(pts, 3);
		}
		break;
	case 14:
		{
			CPoint pts[3];
			pts[0].x = iX - iDX;
			pts[0].y = iY + iDY;

			pts[1].x = iX;
			pts[1].y = iY;

			pts[2].x = iX + iDX;
			pts[2].y = iY + iDY;
			pDC->Polygon(pts, 3);
		}
		break;
	case 15:
		{
			CPoint pts[3];
			pts[0].x = iX - iDX;
			pts[0].y = iY + iDY;

			pts[1].x = iX;
			pts[1].y = iY;

			pts[2].x = iX - iDX;
			pts[2].y = iY - iDY;
			pDC->Polygon(pts, 3);
		}
		break;
	case 16:
		{
			CPoint pts[3];
			pts[0].x = iX - iDX;
			pts[0].y = iY - iDY;

			pts[1].x = iX;
			pts[1].y = iY;

			pts[2].x = iX + iDX;
			pts[2].y = iY - iDY;
			pDC->Polygon(pts, 3);
		}
		break;
	default:
		//do nothing
		break;
	}

}

void CGraph::RemoveSeries(CString label)
{
	TRACE1("CGraph::RemoveSeries(%s)\n",label);

	POSITION pos;
	CGraphSeries* pSeries;
	pos = graphSeries->GetHeadPosition();
	while(pos != NULL)
	{
		pSeries = (CGraphSeries*)graphSeries->GetAt(pos);
		if(pSeries->GetLabel().Compare(label) == 0)
		{
			graphSeries->RemoveAt(pos);
			delete pSeries;
			break;
		}
		graphSeries->GetNext(pos);
	}
}

void CGraph::RemoveColor(int dataGroup)
{
	TRACE1("CGraph::RemoveColor(%d)\n",dataGroup);

	POSITION pos;
	CGraphDataColor* dataColor = NULL;
	pos = colorList->GetHeadPosition();
	for(int i = 0; i < dataGroup; i++)
	{
		dataColor = (CGraphDataColor*)colorList->GetNext(pos);
	}
	colorList->RemoveAt(pos);
	delete dataColor;
}

void CGraph::RemoveLegend(int dataGroup)
{
	TRACE1("CGraph::RemoveLegend(%d)\n",dataGroup);

	graphLegend.Remove(dataGroup);
}

void CGraph::RemoveAllSeries()
{
	TRACE0("CGraph::RemoveAllSeries\n");

	POSITION pos1, pos2;
	CGraphSeries* pSeries;
	for( pos1 = graphSeries->GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		graphSeries->GetNext(pos1);
		pSeries = (CGraphSeries*)graphSeries->GetAt( pos2 ); // Save the old pointer for
		//deletion.
		graphSeries->RemoveAt( pos2 );
		delete pSeries; // Deletion avoids memory leak.
	}

}

void CGraph::RemoveAllColors()
{
	TRACE0("CGraph::RemoveAllColors\n");

	POSITION pos;
	CGraphDataColor* dataColor;
	pos = colorList->GetHeadPosition();
	while(pos != NULL)
	{
		dataColor = (CGraphDataColor*) colorList->GetAt( pos );
		colorList->RemoveAt(pos);
		delete dataColor;
	}

	colorList->RemoveAll();
}

void CGraph::RemoveAllLegends()
{
	TRACE0("CGraph::RemoveAllLegends\n");

	graphLegend.RemoveAll();
}

void CGraph::RemoveAllData()
{
	TRACE0("CGraph::RemoveAllData\n");

	RemoveAllSeries();
	RemoveAllLegends();
	RemoveAllColors();
}

void CGraph::SetGraphQuadType(int quads)
{
	TRACE1("CGraph::SetGraphQuadType(%d)\n",quads);

	if(graphType == BAR_GRAPH)
	{
		if((quads < 1) || (quads > 4))
			graphQuadType = 1;
		else
		{
			graphQuadType = quads;
			quadSetManually = TRUE;
		}
	}
}

void CGraph::OnLButtonDown(UINT nFlags, CPoint point) 
{


	CStatic::OnLButtonDown(nFlags, point);
}

