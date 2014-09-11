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
//  $History: testdrawView4.CPP $
// 
// *****************  Version 6  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 5  *****************
// User: Longo        Date: 7/27/05    Time: 3:53p
// Updated in $/FDMS/FDMS Application/FDMSNET
// some view adjustments
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
// implementation of the CGraphCycleVerifView class
//

#include "stdafx.h"
#include "FDMS.h"
#include <afxdisp.h>

#include "Graph.h"
#include "GraphSeries.h"
#include "math.h"
#include "float.h"
#include "resource.h"

#include "testdrawView4.h"


#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGraphCycleVerifView

IMPLEMENT_DYNCREATE(CGraphCycleVerifView, CFDMSView)

BEGIN_MESSAGE_MAP(CGraphCycleVerifView, CFDMSView)
	//{{AFX_MSG_MAP(CGraphCycleVerifView)
	ON_COMMAND(ID_DRAW_SCATTER, OnDrawScatter)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////



void CGraphCycleVerifView::OnGraphHelp(WPARAM , LPARAM )
{
	m_GraphHelp->ShowWindow(SW_SHOW);
}

CGraphCycleVerifView::CGraphCycleVerifView()
{
	TRACE("CGraphCycleVerifView::CGraphCycleVerifView\n");
	g_bPlot4 = TRUE;


	m_iMouseControl =	GRAPHMOUSE_GRID	| GRAPHMOUSE_DATA | 
		GRAPHMOUSE_LCYCLE | GRAPHMOUSE_RCYCLE |		//	GRAPHMOUSE_XCYCLE |
								// GRAPHMOUSE_XLINEAR |
								GRAPHMOUSE_YLINEAR | 
		GRAPHMOUSE_LBCYCLE | GRAPHMOUSE_RBCYCLE | //GRAPHMOUSE_XBCYCLE |
		GRAPHMOUSE_LEGEND;
}

CGraphCycleVerifView::~CGraphCycleVerifView()
{
	TRACE("CGraphCycleVerifView::~CGraphCycleVerifView\n");
	g_bPlot4 = FALSE;


}

BOOL CGraphCycleVerifView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CFDMSView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphCycleVerifView drawing
void CGraphCycleVerifView::OnMessageGraphHelp(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_GraphHelp)
		m_GraphHelp->ToggleDisplay();
}

void CGraphCycleVerifView::OnDraw(CDC* pDC)
{


	if(graphComplete)
		testGraph->DrawGraph(pDC);

	UpdateWindow();

}

/////////////////////////////////////////////////////////////////////////////
// CGraphCycleVerifView diagnostics

#ifdef _DEBUG
void CGraphCycleVerifView::AssertValid() const
{
	CFDMSView::AssertValid();
}

void CGraphCycleVerifView::Dump(CDumpContext& dc) const
{
	CFDMSView::Dump(dc);
}

CDocument* CGraphCycleVerifView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
	return (CDocument*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphCycleVerifView message handlers

void CGraphCycleVerifView::OnInitialUpdate() 
{
	CFDMSView::OnInitialUpdate();
	graphComplete = FALSE;



	OnDrawScatter();
	//	CFDMSDoc* pDoc = (CFDMSDoc*)GetDocument();
	//	ASSERT_VALID(pDoc);
	//	pDoc->SetTitle(TITLE4);

	m_pDocument->SetTitle(TITLE4);
	UpdateGraphData();
	GetOwner()->SetWindowText(TITLE4);
}

void CGraphCycleVerifView::UpdateGraphData()
{
	TRACE("CGraphCycleVerifView::UpdateGraphData\n");
	if (testGraph)
		testGraph->RemoveAllData();
	OnDrawScatter();

	double dMinX = 6; 
	double dMaxX = 0;
	double dMinY = DBL_MAX; 
	double dMaxY = -DBL_MIN;

	int iGroup = 0;
	int i;

	if (g_iGrayCount4)
	{
		TRACE1("CGraphCycleVerifView::UpdateGraphData GrayCount=%d\n",g_iGrayCount4);
		CGraphSeries* dataGray;
		dataGray = new CGraphSeries(iGroup);
		dataGray->SetDoLines(false);
		dataGray->SetStyle(1);// cross
		dataGray->SetColor(DARK_GRAY);
		dataGray->SetGroup(iGroup);
		dataGray->SetLabel("Database Entries");
		dataGray->SetLabelX("Cycle");
		dataGray->SetLabelY("N/G");
		testGraph->SetLegend(iGroup,"Database Entries",DARK_GRAY,1);
		for (i = 0; i < g_iGrayCount4; i++)
		{
			double X, Y;
			X = (double)g_fGrayDataX4[i];
			Y = pow(10.0,(double)g_fGrayDataY4[i]);
			dataGray->SetData(X, Y);
			dMinX = min(dMinX,X);
			dMaxX = max(dMaxX,X);
			dMinY = min(dMinY,(double)g_fGrayDataY4[i]);
			dMaxY = max(dMaxY,(double)g_fGrayDataY4[i]);
		}
		testGraph->AddSeries(dataGray);
		iGroup++;
	}

	if (g_iBlueCount4)
	{
		TRACE1("CGraphCycleVerifView::UpdateGraphData BlueCount=%d\n",g_iBlueCount4);
		CGraphSeries* dataBlue;
		dataBlue = new CGraphSeries(iGroup);
		dataBlue->SetDoLines(false);
		dataBlue->SetStyle(1);// cross
		dataBlue->SetColor(BLUE);
		dataBlue->SetGroup(iGroup);
		dataBlue->SetLabel("Fitted Database Entries");
		dataBlue->SetLabelX("Cycle");
		dataBlue->SetLabelY("N/G");
		testGraph->SetLegend(iGroup,"Fitted Database Entries",BLUE,1);
		for (i = 0; i < g_iBlueCount4; i++)
		{
			double X, Y;
			X = (double)g_fBlueDataX4[i];
			Y = pow(10.0,(double)g_fBlueDataY4[i]);
			dataBlue->SetData(X, Y);
			dMinX = min(dMinX,X);
			dMaxX = max(dMaxX,X);
			dMinY = min(dMinY,(double)g_fBlueDataY4[i]);
			dMaxY = max(dMaxY,(double)g_fBlueDataY4[i]);
		}
		testGraph->AddSeries(dataBlue);
		iGroup++;
	}

	if (g_iGoodCount)
	{
		TRACE1("CGraphCycleVerifView::UpdateGraphData GoodCount=%d\n",g_iGoodCount);
		CGraphSeries* dataGood;
		dataGood = new CGraphSeries(iGroup);
		dataGood->SetDoLines(false);
		dataGood->SetStyle(5);// diamond
		dataGood->SetColor(FOREST_GREEN);
		dataGood->SetGroup(iGroup);
		dataGood->SetLabel("Good Data");
		dataGood->SetLabelX("Cycle");
		dataGood->SetLabelY("N/G");
		testGraph->SetLegend(iGroup,"Good Data",FOREST_GREEN,5);
		for (i = 0; i < g_iGoodCount; i++)
		{
			double X, Y;
			X = (double)g_fMPGoodDataX4[i];
			Y = pow(10.0,(double)g_fMPGoodDataY4[i]);
			dataGood->SetData(X, Y);
			dMinX = min(dMinX,X);
			dMaxX = max(dMaxX,X);
			dMinY = min(dMinY,(double)g_fMPGoodDataY4[i]);
			dMaxY = max(dMaxY,(double)g_fMPGoodDataY4[i]);
		}
		testGraph->AddSeries(dataGood);
		iGroup++;
	}

	if (g_iNeutralCount)
	{
		TRACE1("CGraphCycleVerifView::UpdateGraphData Neutral Count=%d\n",g_iNeutralCount);
		CGraphSeries* dataNeutral;
		dataNeutral = new CGraphSeries(iGroup);
		dataNeutral->SetDoLines(false);
		dataNeutral->SetStyle(5);// diamond
		dataNeutral->SetColor(PINK);
		dataNeutral->SetGroup(iGroup);
		dataNeutral->SetLabel("Neutral Data");
		dataNeutral->SetLabelX("Cycle");
		dataNeutral->SetLabelY("N/G");
		testGraph->SetLegend(iGroup,"Neutral Data",PINK,5);
		for (i = 0; i < g_iNeutralCount; i++)
		{
			double X, Y;
			X = g_fMPNeutralDataX4[i];
			Y = pow(10.0,(double)g_fMPNeutralDataY4[i]);
			dataNeutral->SetData(X, Y);
			dMinX = min(dMinX,X);
			dMaxX = max(dMaxX,X);
			dMinY = min(dMinY,(double)g_fMPNeutralDataY4[i]);
			dMaxY = max(dMaxY,(double)g_fMPNeutralDataY4[i]);
		}
		testGraph->AddSeries(dataNeutral);
		iGroup++;
	}

	if (g_iSuspectCount)
	{
		TRACE1("CGraphCycleVerifView::UpdateGraphData SuspectCount=%d\n",g_iSuspectCount);
		CGraphSeries* dataSuspect;
		dataSuspect = new CGraphSeries(iGroup);
		dataSuspect->SetDoLines(false);
		dataSuspect->SetStyle(5);// diamond
		dataSuspect->SetColor(RED);
		dataSuspect->SetGroup(iGroup);
		dataSuspect->SetLabel("Suspect Data");
		dataSuspect->SetLabelX("Cycle");
		dataSuspect->SetLabelY("N/G");

		testGraph->SetLegend(iGroup,"Suspect Data",RED,5);
		for (i = 0; i < g_iSuspectCount; i++)
		{
			double X, Y;
			X = g_fMPSuspectDataX4[i];
			Y = pow(10.0,(double)g_fMPSuspectDataY4[i]);
			dataSuspect->SetData(X, Y);
			dMinX = min(dMinX,X);
			dMaxX = max(dMaxX,X);
			dMinY = min(dMinY,(double)g_fMPSuspectDataY4[i]);
			dMaxY = max(dMaxY,(double)g_fMPSuspectDataY4[i]);
		}
		testGraph->AddSeries(dataSuspect);
		iGroup++;
	}


	if (g_bPlotMostRecent && (g_iMPIndexMostRecentMeasurement > 0))
	{
		TRACE("CGraphCycleVerifView::UpdateGraphData PlotMostRecent\n");
		CGraphSeries* dataMostRecent;
		dataMostRecent = new CGraphSeries(iGroup);
		dataMostRecent->SetDoLines(false);
		dataMostRecent->SetStyle(4);// square
		COLORREF cColor = ORANGE;
		switch (g_iMPStatus[g_iMPIndexMostRecentMeasurement-1]) {
		case eMostRecentNoFits:
			cColor = BLACK;
			break;
		case eMostRecentGood:
			cColor = GREEN;
			break;
		case eMostRecentSuspect:
			cColor = RED;
			break;
		}
		dataMostRecent->SetColor(cColor);
		dataMostRecent->SetGroup(iGroup);
		CString cString;
		cString.Format("MR:%s",g_szMPItemIDs[g_iMPIndexMostRecentMeasurement-1]);
		dataMostRecent->SetLabel(cString);
		dataMostRecent->SetLabelX("Cycle");
		dataMostRecent->SetLabelY("N/G");
		testGraph->SetLegend(iGroup,"Most Recent",cColor,4);
		dataMostRecent->SetData((double)g_fMostRecentDataX4, 
			pow(10.0,(double)g_fMostRecentDataY4));
		dMinX = min(dMinX,g_fMostRecentDataX4);
		dMaxX = max(dMaxX,g_fMostRecentDataX4);
		dMinY = min(dMinY,(double)g_fMostRecentDataY4);
		dMaxY = max(dMaxY,(double)g_fMostRecentDataY4);
		testGraph->AddSeries(dataMostRecent);
		iGroup++;
	}

	if (iGroup)
	{
		int iMinX, iCycX, iMinY, iCycY, iMaxX, iMaxY;

		iMinX = (int)floor(dMinX);
		iMaxX = (int)ceil(dMaxX);
		iMinY = (int)floor(dMinY);
		iMaxY = (int)ceil(dMaxY);
		iCycX = abs(iMaxX - iMinX);
		iCycY = abs(iMaxY - iMinY);


		//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
		testGraph->SetNLCycles(iCycX,iMinX,iCycY,iMinY);
		testGraph->SetXTickLimits(iMinX-1,iMaxX+1,iMaxX-iMinX+2);
		//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
		//		testGraph->SetNLCycles(iCycX,iMinX,iCycY,iMinY,0);

		for (i = 0; i < CV_ARRAYSIZE; i++)
		{
			double X1, X2, Y1, Y2;
			X1 = 0.40 + (double)i;
			X2 = 1.60 + (double)i;

			if (g_fCVOneSigma[i] > 0.0)
			{
				CGraphSeries* dataRectangle;
				dataRectangle = new CGraphSeries(iGroup);
				dataRectangle->SetDoLines(true);
				dataRectangle->SetStyle(1);// cross
				dataRectangle->SetColor(RGB(255,0,255));
				dataRectangle->SetGroup(iGroup);
				dataRectangle->SetLabel("±3 sigma");
				dataRectangle->SetLabelX("Cycle±0.6");
				dataRectangle->SetLabelY("N/G");
				dataRectangle->SetLineStyle(PS_SOLID,1);
				dataRectangle->SetSelectable(true);
				dataRectangle->SetClip(true);
				Y1 = pow(10.0,g_fCVAverages[i] + 3.0*g_fCVOneSigma[i]);
				Y2 = pow(10.0,g_fCVAverages[i] - 3.0*g_fCVOneSigma[i]);
				dataRectangle->SetData(X1,Y1);
				dataRectangle->SetData(X1,Y2);
				dataRectangle->SetData(X2,Y2);
				dataRectangle->SetData(X2,Y1);
				dataRectangle->SetData(X1,Y1);
				dataRectangle->SetData(X1,pow(10.0,(double)g_fCVAverages[i]));
				dataRectangle->SetData(X2,pow(10.0,(double)g_fCVAverages[i]));

				CString cString;
				cString.Format("±3 sigma cyc %d",i+1);
				testGraph->AddSeries(dataRectangle);
				testGraph->SetLegend(iGroup,cString,RGB(255,0,255),1);
				iGroup++;
			}
		}
	}
}

void CGraphCycleVerifView::OnDrawScatter() 
{
	if (testGraph)
		delete testGraph;
	testGraph = new CGraph(SCATTER_GRAPH);
	testGraph->SetGraphTitle(TITLE4);
	testGraph->SetXTickLimits(0,10,5);
	testGraph->SetYTickLimits(0,9,1);

	testGraph->SetXAxisLabel("Cycle");
	testGraph->SetYAxisLabel("Neutrons / Gamma");
	testGraph->SetLinear(true,false);
	testGraph->SetMarkerSize(3);

	//Number, Base of X axis cycles, Number, Base of Y axis cycles 
	//(int iXCycles, int iXBaseCycles, int iYCycles, int iYBaseCycles, int iYRBaseCycles = NULL) 
	testGraph->SetLCycles(1,0,10,1,3);
	//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
	testGraph->SetNLCycles(1,2,3,1,3);

	testGraph->SetHGridLines(FALSE);
	testGraph->SetVGridLines(FALSE);

	graphComplete = TRUE;
	Invalidate(TRUE);	
}



