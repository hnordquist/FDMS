//  $History: testdrawView3.CPP $
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
// implementation of the CGraphNeutVGammaView class
//

#include "stdafx.h"
#include "FDMS.h"
#include <afxdisp.h>

#include "Graph.h"
#include "GraphSeries.h"
#include "math.h"
#include "float.h"
#include "resource.h"

#include "testdrawView3.h"


#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGraphNeutVGammaView

IMPLEMENT_DYNCREATE(CGraphNeutVGammaView, CFDMSView)

BEGIN_MESSAGE_MAP(CGraphNeutVGammaView, CFDMSView)
	//{{AFX_MSG_MAP(CGraphNeutVGammaView)
	ON_COMMAND(ID_DRAW_SCATTER, OnDrawScatter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CGraphNeutVGammaView::OnGraphHelp(WPARAM , LPARAM )
{
	m_GraphHelp->ShowWindow(SW_SHOW);
}

CGraphNeutVGammaView::CGraphNeutVGammaView()
{
	TRACE("CGraphNeutVGammaView::CGraphNeutVGammaView\n");
	g_bPlot3 = TRUE;

}

CGraphNeutVGammaView::~CGraphNeutVGammaView()
{
	TRACE("CGraphNeutVGammaView::~CGraphNeutVGammaView\n");
	g_bPlot3 = FALSE;



}

BOOL CGraphNeutVGammaView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CFDMSView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphNeutVGammaView drawing
void CGraphNeutVGammaView::OnMessageGraphHelp(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_GraphHelp)
		m_GraphHelp->ToggleDisplay();
}

void CGraphNeutVGammaView::OnDraw(CDC* pDC)
{


	if(graphComplete)
		testGraph->DrawGraph(pDC);

	UpdateWindow();

}

/////////////////////////////////////////////////////////////////////////////
// CGraphNeutVGammaView diagnostics

#ifdef _DEBUG
void CGraphNeutVGammaView::AssertValid() const
{
	CFDMSView::AssertValid();
}

void CGraphNeutVGammaView::Dump(CDumpContext& dc) const
{
	CFDMSView::Dump(dc);
}

CDocument* CGraphNeutVGammaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
	return (CDocument*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphNeutVGammaView message handlers

void CGraphNeutVGammaView::OnInitialUpdate() 
{
	CFDMSView::OnInitialUpdate();
	graphComplete = FALSE;



	OnDrawScatter();
	//	CFDMSDoc* pDoc = (CFDMSDoc*)GetDocument();
	//	ASSERT_VALID(pDoc);
	//	pDoc->SetTitle(TITLE3);

	m_pDocument->SetTitle(TITLE3);
	UpdateGraphData();
	GetOwner()->SetWindowText(TITLE3);
}

void CGraphNeutVGammaView::UpdateGraphData()
{
	TRACE("CGraphNeutVGammaView::UpdateGraphData\n");
	if (testGraph)
		testGraph->RemoveAllData();
	OnDrawScatter();

	double dMinX = DBL_MAX; 
	double dMaxX = -DBL_MAX;
	double dMinY = DBL_MAX; 
	double dMaxY = -DBL_MAX;

	int iGroup = 0;
	int i;
	if (g_iGrayCount3)
	{
		TRACE1("CGraphNeutVGammaView::UpdateGraphData GrayCount=%d\n",g_iGrayCount3);
		CGraphSeries* dataGray;
		dataGray = new CGraphSeries(iGroup);
		dataGray->SetDoLines(false);
		dataGray->SetStyle(1);// cross
		dataGray->SetColor(DARK_GRAY);
		dataGray->SetGroup(iGroup);
		dataGray->SetLabel("Database Entries");
		dataGray->SetLabelX("Gtc");
		dataGray->SetLabelY("Ntc");
		testGraph->SetLegend(iGroup,"Database Entries",DARK_GRAY,1);
		for (i = 0; i < g_iGrayCount3; i++)
		{
			double X, Y;
			X = pow(10.0,(double)g_fGrayDataX3[i]);
			Y = pow(10.0,(double)g_fGrayDataY3[i]);
			dataGray->SetData(X, Y);
			dMinX = min(dMinX,(double)g_fGrayDataX3[i]);
			dMaxX = max(dMaxX,(double)g_fGrayDataX3[i]);
			dMinY = min(dMinY,(double)g_fGrayDataY3[i]);
			dMaxY = max(dMaxY,(double)g_fGrayDataY3[i]);
		}
		testGraph->AddSeries(dataGray);
		iGroup++;
	}
	if (g_iBlueCount3)
	{
		TRACE1("CGraphNeutVGammaView::UpdateGraphData BlueCount=%d\n",g_iBlueCount3);
		CGraphSeries* dataBlue;
		dataBlue = new CGraphSeries(iGroup);
		dataBlue->SetDoLines(false);
		dataBlue->SetStyle(1);// cross
		dataBlue->SetColor(BLUE);
		dataBlue->SetGroup(iGroup);
		dataBlue->SetLabel("Fitted Database Entries");
		dataBlue->SetLabelX("Gtc");
		dataBlue->SetLabelY("Ntc");
		testGraph->SetLegend(iGroup,"Fitted Database Entries",BLUE,1);
		for (i = 0; i < g_iBlueCount3; i++)
		{
			double X, Y;
			X = pow(10.0,(double)g_fBlueDataX3[i]);
			Y = pow(10.0,(double)g_fBlueDataY3[i]);
			dataBlue->SetData(X, Y);
			dMinX = min(dMinX,(double)g_fBlueDataX3[i]);
			dMaxX = max(dMaxX,(double)g_fBlueDataX3[i]);
			dMinY = min(dMinY,(double)g_fBlueDataY3[i]);
			dMaxY = max(dMaxY,(double)g_fBlueDataY3[i]);
		}
		testGraph->AddSeries(dataBlue);
		iGroup++;
	}

	if (g_iGoodCount)
	{
		TRACE1("CGraphNeutVGammaView::UpdateGraphData GoodCount=%d\n",g_iGoodCount);
		CGraphSeries* dataGood;
		dataGood = new CGraphSeries(iGroup);
		dataGood->SetDoLines(false);
		dataGood->SetStyle(5);// diamond
		dataGood->SetColor(FOREST_GREEN);
		dataGood->SetGroup(iGroup);
		dataGood->SetLabel("Good Data");
		dataGood->SetLabelX("Gtc");
		dataGood->SetLabelY("Ntc");
		testGraph->SetLegend(iGroup,"Good Data",FOREST_GREEN,5);
		for (i = 0; i < g_iGoodCount; i++)
		{
			double X, Y;
			X = pow(10.0,(double)g_fMPGoodDataX3[i]);
			Y = pow(10.0,(double)g_fMPGoodDataY3[i]);
			dataGood->SetData(X, Y);
			dMinX = min(dMinX,(double)g_fMPGoodDataX3[i]);
			dMaxX = max(dMaxX,(double)g_fMPGoodDataX3[i]);
			dMinY = min(dMinY,(double)g_fMPGoodDataY3[i]);
			dMaxY = max(dMaxY,(double)g_fMPGoodDataY3[i]);
		}
		testGraph->AddSeries(dataGood);
		iGroup++;
	}

	if (g_iNeutralCount)
	{
		TRACE1("CGraphNeutVGammaView::UpdateGraphData NeutralCount=%d\n",g_iNeutralCount);
		CGraphSeries* dataNeutral;
		dataNeutral = new CGraphSeries(iGroup);
		dataNeutral->SetDoLines(false);
		dataNeutral->SetStyle(5);// diamond
		dataNeutral->SetColor(PINK);
		dataNeutral->SetGroup(iGroup);
		dataNeutral->SetLabel("Neutral Data");
		dataNeutral->SetLabelX("Gtc");
		dataNeutral->SetLabelY("Ntc");
		testGraph->SetLegend(iGroup,"Neutral Data",PINK,5);
		for (i = 0; i < g_iNeutralCount; i++)
		{
			double X, Y;
			X = pow(10.0,(double)g_fMPNeutralDataX3[i]);
			Y = pow(10.0,(double)g_fMPNeutralDataY3[i]);
			dataNeutral->SetData(X, Y);
			dMinX = min(dMinX,(double)g_fMPNeutralDataX3[i]);
			dMaxX = max(dMaxX,(double)g_fMPNeutralDataX3[i]);
			dMinY = min(dMinY,(double)g_fMPNeutralDataY3[i]);
			dMaxY = max(dMaxY,(double)g_fMPNeutralDataY3[i]);
		}
		testGraph->AddSeries(dataNeutral);
		iGroup++;
	}

	if (g_iSuspectCount)
	{
		TRACE1("CGraphNeutVGammaView::UpdateGraphData SuspectCount=%d\n",g_iSuspectCount);
		CGraphSeries* dataSuspect;
		dataSuspect = new CGraphSeries(iGroup);
		dataSuspect->SetDoLines(false);
		dataSuspect->SetStyle(5);// diamond
		dataSuspect->SetColor(RED);
		dataSuspect->SetGroup(iGroup);
		dataSuspect->SetLabel("Suspect Data");
		dataSuspect->SetLabelX("Gtc");
		dataSuspect->SetLabelY("Ntc");

		testGraph->SetLegend(iGroup,"Suspect Data",RED,5);
		for (i = 0; i < g_iSuspectCount; i++)
		{
			double X, Y;
			X = pow(10.0,(double)g_fMPSuspectDataX3[i]);
			Y = pow(10.0,(double)g_fMPSuspectDataY3[i]);
			dataSuspect->SetData(X, Y);
			dMinX = min(dMinX,(double)g_fMPSuspectDataX3[i]);
			dMaxX = max(dMaxX,(double)g_fMPSuspectDataX3[i]);
			dMinY = min(dMinY,(double)g_fMPSuspectDataY3[i]);
			dMaxY = max(dMaxY,(double)g_fMPSuspectDataY3[i]);
		}
		testGraph->AddSeries(dataSuspect);
		iGroup++;
	}

	if (g_bPlotMostRecent && (g_iMPIndexMostRecentMeasurement > 0))
	{
		TRACE("CGraphNeutVGammaView::UpdateGraphData PlotMostRecent\n");
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
		//		dataMostRecent->SetLabel("Most Recent");
		dataMostRecent->SetLabelX("Gtc");
		dataMostRecent->SetLabelY("Ntc");
		testGraph->SetLegend(iGroup,"Most Recent",cColor,4);
		dataMostRecent->SetData(pow(10.0,(double)g_fMostRecentDataX3), 
			pow(10.0,(double)g_fMostRecentDataY3));
		dMinX = min(dMinX,(double)g_fMostRecentDataX3);
		dMaxX = max(dMaxX,(double)g_fMostRecentDataX3);
		dMinY = min(dMinY,(double)g_fMostRecentDataY3);
		dMaxY = max(dMaxY,(double)g_fMostRecentDataY3);
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
		testGraph->SetNLCycles(iCycX,iMinX,iCycY,iMinY,0);

		if (g_bFit3Possible && g_iBlueCount3)
		{
			//double dPointX1 = pow(10.,(int)dMinX);
			//double dPointX2 = pow(10.,(int)dMaxX + 1);
			//double dPointY1 = pow(10.,g_fSlope3*log10(dPointX1)+g_fIntercept3);
			//double dPointY2 = pow(10.,g_fSlope3*log10(dPointX2)+g_fIntercept3);

			double dTempX[20];
			double dTempY0[20];
			double dTempY1[20];
			double dTempY2[20];

			double dStep = ((int)dMaxX+1-iMinX)/19.0;
			double dIndex;
			for (dIndex = iMinX, i = 0; i < 20; i++, dIndex += dStep)
			{
				dTempX[i] = pow(10.,dIndex);
				double dPhi = 1.0/g_iBlueCount3 + ((dIndex-g_fMeanX3)*(dIndex-g_fMeanX3))/((g_iBlueCount3-1)*g_fStatX3);
				double dLogTemp = g_fSlope3*log10(dTempX[i])+g_fIntercept3;
				double dError = 3.0*sqrt(g_fStatY3*(1.0+dPhi));
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



void CGraphNeutVGammaView::OnDrawScatter() 
{
	if (testGraph)
		delete testGraph;
	testGraph = new CGraph(SCATTER_GRAPH);
	testGraph->SetGraphTitle("Neutron v. Gamma");
	testGraph->SetXTickLimits(0,100,1);
	testGraph->SetYTickLimits(0,6500,1);

	testGraph->SetXAxisLabel("Time Corrected Gammas");
	testGraph->SetYAxisLabel("Time Corrected Neutrons");
	testGraph->SetLinear(false,false);
	testGraph->SetMarkerSize(3);

	//Number, Base of X axis cycles, Number, Base of Y axis cycles 
	//(int iXCycles, int iXBaseCycles, int iYCycles, int iYBaseCycles, int iYRBaseCycles = NULL) 
	testGraph->SetLCycles(11,0,10,1,3);
	//(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
	testGraph->SetNLCycles(1,2,3,1,3);

	testGraph->SetHGridLines(FALSE);
	testGraph->SetVGridLines(FALSE);

	graphComplete = TRUE;
	Invalidate(TRUE);	
}



