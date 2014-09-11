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
// $History: testdrawView2.CPP $
// 
// *****************  Version 5  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
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
// implementation of the CGraphGDBVCTView class
//

#include "stdafx.h"
#include "FDMS.h"
#include <afxdisp.h>

#include "Graph.h"
#include "GraphSeries.h"
#include "math.h"
#include "float.h"
#include "resource.h"

#include "testdrawView2.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CGraphGDBVCTView

IMPLEMENT_DYNCREATE(CGraphGDBVCTView, CFDMSView)

BEGIN_MESSAGE_MAP(CGraphGDBVCTView, CFDMSView)
	//{{AFX_MSG_MAP(CGraphGDBVCTView)
	ON_COMMAND(ID_DRAW_SCATTER, OnDrawScatter)
	//}}AFX_MSG_MAP


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CGraphGDBVCTView::OnGraphHelp(WPARAM , LPARAM )
{
	m_GraphHelp->ShowWindow(SW_SHOW);
}

CGraphGDBVCTView::CGraphGDBVCTView()
{
	TRACE("CGraphGDBVCTView::CGraphGDBVCTView\n");
	g_bPlot2 = TRUE;

}

CGraphGDBVCTView::~CGraphGDBVCTView()
{
	TRACE("CGraphGDBVCTView::~CGraphGDBVCTView\n");
	g_bPlot2 = FALSE;


}

BOOL CGraphGDBVCTView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CFDMSView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphGDBVCTView drawing
void CGraphGDBVCTView::OnMessageGraphHelp(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_GraphHelp)
		m_GraphHelp->ToggleDisplay();
}
void CGraphGDBVCTView::OnDraw(CDC* pDC)
{

	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	if(graphComplete)
		testGraph->DrawGraph(pDC);

	UpdateWindow();

}
/////////////////////////////////////////////////////////////////////////////
// CGraphGDBVCTView diagnostics

#ifdef _DEBUG
void CGraphGDBVCTView::AssertValid() const
{
	CFDMSView::AssertValid();
}

void CGraphGDBVCTView::Dump(CDumpContext& dc) const
{
	CFDMSView::Dump(dc);
}

CDocument* CGraphGDBVCTView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
	return (CDocument*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphGDBVCTView message handlers

void CGraphGDBVCTView::OnInitialUpdate() 
{
	CFDMSView::OnInitialUpdate();
	graphComplete = FALSE;


	OnDrawScatter();
	//	CFDMSDoc* pDoc = (CFDMSDoc*)GetDocument();
	//	ASSERT_VALID(pDoc);
	//	pDoc->SetTitle(TITLE2);

	m_pDocument->SetTitle(TITLE2);
	UpdateGraphData();
	GetOwner()->SetWindowText(TITLE2);
}

void CGraphGDBVCTView::UpdateGraphData()
{
	TRACE("CGraphGDBVCTView::UpdateGraphData\n");
	if (testGraph)
		testGraph->RemoveAllData();
	OnDrawScatter();

	double dMinX = DBL_MAX; 
	double dMaxX = -DBL_MAX;
	double dMinY = DBL_MAX; 
	double dMaxY = -DBL_MAX;

	int iGroup = 0;
	int i;
	if (g_iGrayCount2)
	{
		TRACE1("CGraphGDBVCTView::UpdateGraphData GrayCount=%d\n",g_iGrayCount2);
		CGraphSeries* dataGray;
		dataGray = new CGraphSeries(iGroup);
		dataGray->SetDoLines(false);
		dataGray->SetStyle(1);// cross
		dataGray->SetColor(DARK_GRAY);
		dataGray->SetGroup(iGroup);
		dataGray->SetLabel("Database Entries");
		dataGray->SetLabelX("Year");
		dataGray->SetLabelY("G/Bu");
		testGraph->SetLegend(iGroup,"Database Entries",DARK_GRAY,1);
		for (i = 0; i < g_iGrayCount2; i++)
		{
			double X1, Y1;
			X1 = pow(10.0,(double)g_fGrayDataX2[i]);
			Y1 = pow(10.0,(double)g_fGrayDataY2[i]);
			dataGray->SetData(X1, Y1);
			dMinX = min(dMinX,(double)g_fGrayDataX2[i]);
			dMaxX = max(dMaxX,(double)g_fGrayDataX2[i]);
			dMinY = min(dMinY,(double)g_fGrayDataY2[i]);
			dMaxY = max(dMaxY,(double)g_fGrayDataY2[i]);
		}
		testGraph->AddSeries(dataGray);
		iGroup++;
	}

	if (g_iBlueCount2)
	{
		TRACE1("CGraphGDBVCTView::UpdateGraphData BlueCount=%d\n",g_iBlueCount2);
		CGraphSeries* dataBlue;
		dataBlue = new CGraphSeries(iGroup);
		dataBlue->SetDoLines(false);
		dataBlue->SetStyle(1);// cross
		dataBlue->SetColor(BLUE);
		dataBlue->SetGroup(iGroup);
		dataBlue->SetLabel("Fitted Database Entries");
		dataBlue->SetLabelX("Year");
		dataBlue->SetLabelY("G/Bu");
		testGraph->SetLegend(iGroup,"Fitted Databse Entries",BLUE,1);
		for (i = 0; i < g_iBlueCount2; i++)
		{
			double X1, Y1;
			X1 = pow(10.0,(double)g_fBlueDataX2[i]);
			Y1 = pow(10.0,(double)g_fBlueDataY2[i]);
			TRACE("BlueData[%d]= %g,%g::%g,%g\n",i,X1,Y1,g_fBlueDataX2[i],g_fBlueDataY2[i]);
			dataBlue->SetData(X1, Y1);
			dMinX = min(dMinX,(double)g_fBlueDataX2[i]);
			dMaxX = max(dMaxX,(double)g_fBlueDataX2[i]);
			dMinY = min(dMinY,(double)g_fBlueDataY2[i]);
			dMaxY = max(dMaxY,(double)g_fBlueDataY2[i]);
		}
		testGraph->AddSeries(dataBlue);
		iGroup++;
	}

	if (g_iGoodCount)
	{
		TRACE1("CGraphGDBVCTView::UpdateGraphData GoodCount=%d\n",g_iGoodCount);
		CGraphSeries* dataGood;
		dataGood = new CGraphSeries(iGroup);
		dataGood->SetDoLines(false);
		dataGood->SetStyle(5);// diamond
		dataGood->SetColor(FOREST_GREEN);
		dataGood->SetGroup(iGroup);
		dataGood->SetLabel("Good Data");
		dataGood->SetLabelX("Year");
		dataGood->SetLabelY("G/Bu");
		testGraph->SetLegend(iGroup,"Good Data",FOREST_GREEN,5);
		for (i = 0; i < g_iGoodCount; i++)
		{
			double X1, Y1;
			X1 = pow(10.0,(double)g_fMPGoodDataX2[i]);
			Y1 = pow(10.0,(double)g_fMPGoodDataY2[i]);
			dataGood->SetData(X1, Y1);
			dMinX = min(dMinX,(double)g_fMPGoodDataX2[i]);
			dMaxX = max(dMaxX,(double)g_fMPGoodDataX2[i]);
			dMinY = min(dMinY,(double)g_fMPGoodDataY2[i]);
			dMaxY = max(dMaxY,(double)g_fMPGoodDataY2[i]);
		}
		testGraph->AddSeries(dataGood);
		iGroup++;
	}

	if (g_iNeutralCount)
	{
		TRACE1("CGraphGDBVCTView::UpdateGraphData NeutralCount=%d\n",g_iNeutralCount);
		CGraphSeries* dataNeutral;
		dataNeutral = new CGraphSeries(iGroup);
		dataNeutral->SetDoLines(false);
		dataNeutral->SetStyle(5);// diamond
		dataNeutral->SetColor(PINK);
		dataNeutral->SetGroup(iGroup);
		dataNeutral->SetLabel("Neutral Data");
		dataNeutral->SetLabelX("Year");
		dataNeutral->SetLabelY("G/Bu");
		testGraph->SetLegend(iGroup,"Neutral Data",PINK,5);
		for (i = 0; i < g_iNeutralCount; i++)
		{
			double X, Y;
			X = pow(10.0,(double)g_fMPNeutralDataX2[i]);
			Y = pow(10.0,(double)g_fMPNeutralDataY2[i]);
			dataNeutral->SetData(X, Y);
			dMinX = min(dMinX,(double)g_fMPNeutralDataX2[i]);
			dMaxX = max(dMaxX,(double)g_fMPNeutralDataX2[i]);
			dMinY = min(dMinY,(double)g_fMPNeutralDataY2[i]);
			dMaxY = max(dMaxY,(double)g_fMPNeutralDataY2[i]);
		}
		testGraph->AddSeries(dataNeutral);
		iGroup++;
	}

	if (g_iSuspectCount)
	{
		TRACE1("CGraphGDBVCTView::UpdateGraphData SuspectCount=%d\n",g_iSuspectCount);
		CGraphSeries* dataSuspect;
		dataSuspect = new CGraphSeries(iGroup);
		dataSuspect->SetDoLines(false);
		dataSuspect->SetStyle(5);// diamond
		dataSuspect->SetColor(RED);
		dataSuspect->SetLabel("Suspect Data");
		dataSuspect->SetLabelX("Year");
		dataSuspect->SetLabelY("G/Bu");
		dataSuspect->SetGroup(iGroup);

		testGraph->SetLegend(iGroup,"Suspect Data",RED,5);
		for (i = 0; i < g_iSuspectCount; i++)
		{
			double X1, Y1;
			X1 = pow(10.0,(double)g_fMPSuspectDataX2[i]);
			Y1 = pow(10.0,(double)g_fMPSuspectDataY2[i]);
			dataSuspect->SetData(X1, Y1);
			dMinX = min(dMinX,(double)g_fMPSuspectDataX2[i]);
			dMaxX = max(dMaxX,(double)g_fMPSuspectDataX2[i]);
			dMinY = min(dMinY,(double)g_fMPSuspectDataY2[i]);
			dMaxY = max(dMaxY,(double)g_fMPSuspectDataY2[i]);
		}
		testGraph->AddSeries(dataSuspect);
		iGroup++;
	}

	if (g_bPlotMostRecent && (g_iMPIndexMostRecentMeasurement > 0))
	{
		TRACE("CGraphGDBVCTView::UpdateGraphData PlotMostRecent\n");
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
		dataMostRecent->SetLabelX("Year");
		dataMostRecent->SetLabelY("G/Bu");
		testGraph->SetLegend(iGroup,"Most Recent",cColor,4);
		dataMostRecent->SetData(pow(10.0,(double)g_fMostRecentDataX2), 
			pow(10.0,(double)g_fMostRecentDataY2));
		dMinX = min(dMinX,(double)g_fMostRecentDataX2);
		dMaxX = max(dMaxX,(double)g_fMostRecentDataX2);
		dMinY = min(dMinY,(double)g_fMostRecentDataY2);
		dMaxY = max(dMaxY,(double)g_fMostRecentDataY2);
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
		iCycX = iMaxX - iMinX;
		iCycY = iMaxY - iMinY;
		//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
		testGraph->SetNLCycles(iCycX,iMinX,iCycY,iMinY);

		if (g_bFit2Possible && g_iBlueCount2)
		{
			//double dPointX1 = pow(10,(int)dMinX);
			//double dPointX2 = pow(10,(int)dMaxX + 1);
			//double dPointY1 = pow(10.,g_fSlope2*log10(dPointX1)+g_fIntercept2);
			//double dPointY2 = pow(10.,g_fSlope2*log10(dPointX2)+g_fIntercept2);

			double dTempX[20];
			double dTempY0[20];
			double dTempY1[20];
			double dTempY2[20];

			double dStep = ((int)dMaxX+1-iMinX)/19.0;
			double dIndex;
			for (dIndex = iMinX, i = 0; i < 20; i++, dIndex += dStep)
			{
				dTempX[i] = pow(10.,dIndex);
				double dPhi = 1.0/g_iBlueCount2 + ((dIndex-g_fMeanX2)*(dIndex-g_fMeanX2))/((g_iBlueCount2-1)*g_fStatX2);
				double dLogTemp = g_fSlope2*log10(dTempX[i])+g_fIntercept2;
				double dError = 3.0*sqrt(g_fStatY2*(1.0+dPhi));
				dTempY0[i] = pow(10.,dLogTemp);
				dTempY1[i] = pow(10.,dLogTemp-dError);
				dTempY2[i] = pow(10.,dLogTemp+dError);
			}

			CGraphSeries* dataFit;
			dataFit = new CGraphSeries(0,BLACK,true,iGroup);
			dataFit->SetLineStyle(PS_SOLID,1);
			dataFit->SetSelectable(false);
			dataFit->SetClip(true);
			for (i = 0; i < 20; i++)
				dataFit->SetData(dTempX[i],dTempY0[i]);
			testGraph->SetLegend(iGroup,"Fit Line",BLACK);
			testGraph->AddSeries(dataFit);
			iGroup++;

			dataFit = new CGraphSeries(0,BLACK,true,iGroup);
			dataFit->SetLineStyle(PS_DASH,1);
			dataFit->SetSelectable(false);
			dataFit->SetClip(true);
			for (i = 0; i < 20; i++)
				dataFit->SetData(dTempX[i],dTempY1[i]);
			testGraph->SetLegend(iGroup,"±3 sigma",BLACK);
			testGraph->AddSeries(dataFit);

			dataFit = new CGraphSeries(0,BLACK,true,iGroup);
			dataFit->SetLineStyle(PS_DASH,1);
			dataFit->SetSelectable(false);
			dataFit->SetClip(true);
			for (i = 0; i < 20; i++)
				dataFit->SetData(dTempX[i],dTempY2[i]);
			testGraph->AddSeries(dataFit);
			iGroup++;
		}
	}
}


void CGraphGDBVCTView::OnDrawScatter() 
{
	if (testGraph)
		delete testGraph;
	testGraph = new CGraph(SCATTER_GRAPH);
	testGraph->SetGraphTitle("Gamma / Declared Burnup v. Cooling Time");
	testGraph->SetXTickLimits(0,100,1);
	testGraph->SetYTickLimits(0,6500,1);

	testGraph->SetXAxisLabel("Declared Cooling Time (years)");
	testGraph->SetYAxisLabel("Gamma / Declared Burnup");
	testGraph->SetLinear(false,false);
	testGraph->SetMarkerSize(3);

	//Number, Base of X axis cycles, Number, Base of Y axis cycles 
	//(int iXCycles, int iXBaseCycles, int iYCycles, int iYBaseCycles, int iYRBaseCycles = NULL) 
	testGraph->SetLCycles(11,0,10,1,3);
	//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
	testGraph->SetNLCycles(2,0,2,0,3);

	testGraph->SetHGridLines(FALSE);
	testGraph->SetVGridLines(FALSE);

	graphComplete = TRUE;
	Invalidate(TRUE);	
}

