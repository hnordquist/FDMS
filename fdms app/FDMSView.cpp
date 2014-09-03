//  $History: FDMSView.cpp $
// 
// *****************  Version 4  *****************
// User: Longo        Date: 7/27/05    Time: 3:53p
// Updated in $/FDMS/FDMS Application/FDMSNET
// attempting to constrain pathological Y axis display
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 2  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
// implementation file
//

#include "stdafx.h"
#include "FDMS.h"
#include "FDMSView.h"


// CFDMSView

IMPLEMENT_DYNCREATE(CFDMSView, CView)

CFDMSView::CFDMSView()
{
	testGraph	= NULL;
	m_GraphHelp = NULL;

	m_iMouseControl =	GRAPHMOUSE_GRID	| GRAPHMOUSE_DATA | 
		GRAPHMOUSE_LCYCLE | GRAPHMOUSE_RCYCLE | GRAPHMOUSE_XCYCLE |
		//						GRAPHMOUSE_XLINEAR | GRAPHMOUSE_YLINEAR | 
		GRAPHMOUSE_LBCYCLE | GRAPHMOUSE_RBCYCLE | GRAPHMOUSE_XBCYCLE |
		GRAPHMOUSE_LEGEND;


}

CFDMSView::~CFDMSView()
{
	if (testGraph)
		delete testGraph;
	if (m_GraphHelp)
		delete m_GraphHelp;
}

BEGIN_MESSAGE_MAP(CFDMSView, CView)

	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()


// CFDMSView drawing

void CFDMSView::OnDraw(CDC* )
{

}


// CFDMSView diagnostics

#ifdef _DEBUG
void CFDMSView::AssertValid() const
{
	CView::AssertValid();
}

void CFDMSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CFDMSView message handlers
void CFDMSView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{


	CView::OnBeginPrinting(pDC, pInfo);
}

void CFDMSView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{


	CView::OnEndPrinting(pDC, pInfo);
	Invalidate();
}

BOOL CFDMSView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);
	pInfo->m_nNumPreviewPages = 1;
	return DoPreparePrinting(pInfo);
}

void CFDMSView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{

	if(graphComplete)
	{
		testGraph->PrintGraph(pDC, pInfo);
	}
	//	CView::OnPrint(pDC, pInfo);
}


void CFDMSView::OnLButtonDown(UINT nFlags, CPoint point) 
{

	if (testGraph)
	{
		CRect cRect(0,0,0,0);

		testGraph->GetPlotBox(&cRect);
		if (cRect.PtInRect(point) && (m_iMouseControl & GRAPHMOUSE_DATA))
		{
			double dX, dY;
			testGraph->GetDataItemAt(point,&dX,&dY);
			Invalidate();
		}

		testGraph->GetXYAxisBox(&cRect);
		if (cRect.PtInRect(point) && (m_iMouseControl & GRAPHMOUSE_GRID))
		{
			BOOL bVGrid, bHGrid;
			int iBinVector = 0;
			bHGrid = testGraph->GetHGridLines();
			bVGrid = testGraph->GetVGridLines();
			if (bHGrid)
				iBinVector  = iBinVector | 0x01;
			if (bVGrid)
				iBinVector  = iBinVector | 0x10;
			iBinVector--;
			if (iBinVector & 0x01)
				testGraph->SetHGridLines(TRUE);
			else
				testGraph->SetHGridLines(FALSE);
			if (iBinVector & 0x10)
				testGraph->SetVGridLines(TRUE);
			else
				testGraph->SetVGridLines(FALSE);
			Invalidate(TRUE);
		}

		testGraph->GetXAxisBox(&cRect);
		if (cRect.PtInRect(point))
		{
			bool bInvalidate = true;
			int iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle;
			if ((nFlags & MK_CONTROL)&&(m_iMouseControl & GRAPHMOUSE_XBCYCLE))
			{
				if (testGraph->IsXLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetXTickLimits(&minTickValue,&maxTickValue,&tickStep);
					testGraph->SetXTickLimits(
						(3.0*minTickValue-maxTickValue)/2.0,
						(minTickValue+maxTickValue)/2.0,
						tickStep);
				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle-1,iYNumCycles,iYBaseCycle,iYRBaseCycle);
				}

			}
			else if ((nFlags & MK_SHIFT)&&(m_iMouseControl & GRAPHMOUSE_XBCYCLE))
			{
				if (testGraph->IsXLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetXTickLimits(&minTickValue,&maxTickValue,&tickStep);
					testGraph->SetXTickLimits(
						(minTickValue+maxTickValue)/2.0, 
						(3.0*maxTickValue-minTickValue)/2.0, 
						tickStep);

				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle+1,iYNumCycles,iYBaseCycle,iYRBaseCycle);
				}
			}
			else if (m_iMouseControl & GRAPHMOUSE_XLINEAR)
			{
				bool bYLinear, bXLinear;
				testGraph->GetLinear(&bXLinear, &bYLinear);
				testGraph->SetLinear(!bXLinear,bYLinear);
			}
			else 
				bInvalidate = false;
			if (bInvalidate)
				Invalidate(TRUE);
		}

		testGraph->GetYAxisBox(&cRect);
		if (cRect.PtInRect(point))
		{
			int iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle;
			bool bInvalidate = true;
			if ((nFlags & MK_CONTROL) && (m_iMouseControl & GRAPHMOUSE_LBCYCLE))
			{
				if (testGraph->IsYLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetYTickLimits(&minTickValue,&maxTickValue,&tickStep);
					testGraph->SetYTickLimits((3.0*minTickValue-maxTickValue)/2.0,(minTickValue+maxTickValue)/2.0,tickStep);
				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle-1,iYRBaseCycle);
				}
			}
			else if ((nFlags & MK_SHIFT) && (m_iMouseControl & GRAPHMOUSE_LBCYCLE))
			{
				if (testGraph->IsYLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetYTickLimits(&minTickValue,&maxTickValue,&tickStep);
					testGraph->SetYTickLimits((minTickValue+maxTickValue), 
						(3.0*maxTickValue-minTickValue), tickStep);
				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle+1,iYRBaseCycle);
				}
			}
			else if ((nFlags & MK_MBUTTON) && (m_iMouseControl & GRAPHMOUSE_YLINEAR))
			{
				bool bYLinear, bXLinear;
				testGraph->GetLinear(&bXLinear, &bYLinear);
				testGraph->SetLinear(bXLinear,!bYLinear);
				if (testGraph->IsYLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetYTickLimits(&minTickValue,&maxTickValue,&tickStep);

					// find min and mx .. on pgraohseries Oblist

				//	testGraph->SetYTickLimits((minTickValue+maxTickValue)/2.0, 
				//		(3.0*maxTickValue-minTickValue)/2.0, tickStep);
				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle+1,iYRBaseCycle);
				}

			}
			else
				bInvalidate = false;
			if (bInvalidate)
				Invalidate(TRUE);
		}

		testGraph->GetYRAxisBox(&cRect);
		if (cRect.PtInRect(point))
		{
			int iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle;
			bool bInvalidate = true;
			if ((nFlags & MK_CONTROL ) && (m_iMouseControl & GRAPHMOUSE_RBCYCLE))
			{
				if (testGraph->IsYLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetYRTickLimits(&minTickValue,&maxTickValue,&tickStep);
					testGraph->SetYRTickLimits((3.0*minTickValue-maxTickValue)/2.0,(minTickValue+maxTickValue)/2.0,tickStep);
				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle-1);
				}
			}
			else if ((nFlags & MK_SHIFT) && (m_iMouseControl & GRAPHMOUSE_RBCYCLE))
			{
				if (testGraph->IsYLinear())
				{
					double minTickValue,maxTickValue,tickStep;
					testGraph->GetYRTickLimits(&minTickValue,&maxTickValue,&tickStep);
					testGraph->SetYRTickLimits((minTickValue+maxTickValue)/2.0, 
						(3.0*maxTickValue-minTickValue)/2.0, tickStep);
				}
				else
				{
					testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
					testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle+1);
				}
			}
			else if (m_iMouseControl & GRAPHMOUSE_YLINEAR)
			{
				bool bYLinear, bXLinear;
				testGraph->GetLinear(&bXLinear, &bYLinear);
				testGraph->SetLinear(bXLinear,!bYLinear);
			}
			else
				bInvalidate = false;
			if (bInvalidate)
				Invalidate(TRUE);
		}

		if (testGraph->GetHasLegendInfo() && 
			(m_iMouseControl & GRAPHMOUSE_LEGEND))
		{
			testGraph->GetLegendBox(&cRect);
			if (cRect.PtInRect(point))
			{
				bool bFound = false;
				int iIndex = 0;
				if (testGraph->graphHasLegend)
				{
					while (testGraph->GetLegendContentsBox(iIndex,&cRect) && iIndex < 100)
					{
						if (cRect.PtInRect(point))
						{
							bFound = true;
							testGraph->ToggleDisplayEqGroup(iIndex);
						}
						iIndex++;
					}
				}
				if (!bFound)
					testGraph->graphHasLegend = !testGraph->graphHasLegend;

				Invalidate(TRUE);
			}
		}

	}
	CView::OnLButtonDown(nFlags, point);
}

BOOL CFDMSView::OnEraseBkgnd(CDC* pDC) 
{

	if (testGraph)
		return FALSE;	
	else
		return CView::OnEraseBkgnd(pDC);
}

void CFDMSView::OnRButtonDown(UINT nFlags, CPoint point) 
{

	if (testGraph)
	{
		CRect cRect(0,0,0,0);
		testGraph->GetXYAxisBox(&cRect);
		if (cRect.PtInRect(point) && (m_iMouseControl & GRAPHMOUSE_GRID))
		{
			BOOL bVGrid, bHGrid;
			int iBinVector = 0;
			bHGrid = testGraph->GetHGridLines();
			bVGrid = testGraph->GetVGridLines();
			if (bHGrid)
				iBinVector  = iBinVector | 0x01;
			if (bVGrid)
				iBinVector  = iBinVector | 0x10;
			iBinVector--;
			if (iBinVector & 0x01)
				testGraph->SetHGridLines(TRUE);
			else
				testGraph->SetHGridLines(FALSE);
			if (iBinVector & 0x10)
				testGraph->SetVGridLines(TRUE);
			else
				testGraph->SetVGridLines(FALSE);
			Invalidate(TRUE);
		}

		testGraph->GetXAxisBox(&cRect);
		if (cRect.PtInRect(point) && (m_iMouseControl & GRAPHMOUSE_XCYCLE))
		{
			int iBump = 1;
			if (nFlags & MK_CONTROL) iBump = -1;

			int iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle;
			if (testGraph->IsXLinear())
			{
				testGraph->GetLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
				testGraph->SetLCycles(iXNumCycles+iBump,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle);
			}
			else
			{
				testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
				testGraph->SetNLCycles(iXNumCycles+iBump,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle);
			}
			Invalidate(TRUE);
		}

		testGraph->GetYAxisBox(&cRect);
		if (cRect.PtInRect(point) && (m_iMouseControl & GRAPHMOUSE_LCYCLE))
		{
			int iBump = 1;
			if (nFlags & MK_CONTROL) iBump = -1;

			int iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle;
			if (testGraph->IsYLinear())
			{
				testGraph->GetLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
				testGraph->SetLCycles(iXNumCycles,iXBaseCycle,iYNumCycles+iBump,iYBaseCycle,iYRBaseCycle);
			}
			else
			{
				testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
				testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles+iBump,iYBaseCycle,iYRBaseCycle);
			}
			Invalidate(TRUE);
		}

		testGraph->GetYRAxisBox(&cRect);
		if (cRect.PtInRect(point) && (m_iMouseControl & GRAPHMOUSE_RCYCLE))
		{
			int iBump = 1;
			if (nFlags & MK_CONTROL) iBump = -1;

			int iXNumCycles,iXBaseCycle,iYNumCycles,iYBaseCycle,iYRBaseCycle;
			if (testGraph->IsYLinear())
			{
				testGraph->GetLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
				testGraph->SetLCycles(iXNumCycles,iXBaseCycle,iYNumCycles+iBump,iYBaseCycle,iYRBaseCycle);
			}
			else
			{
				testGraph->GetNLCycles(&iXNumCycles,&iXBaseCycle,&iYNumCycles,&iYBaseCycle,&iYRBaseCycle);
				testGraph->SetNLCycles(iXNumCycles,iXBaseCycle,iYNumCycles+iBump,iYBaseCycle,iYRBaseCycle);
			}
			Invalidate(TRUE);
		}

		if (testGraph->GetHasLegendInfo() && (m_iMouseControl & GRAPHMOUSE_LEGEND))
		{
			testGraph->GetLegendBox(&cRect);
			if (cRect.PtInRect(point))
			{
				testGraph->graphHasLegend = !testGraph->graphHasLegend;
				Invalidate(TRUE);
			}
		}
	}

	CView::OnRButtonDown(nFlags, point);
}


void CFDMSView::OnTimer(UINT nIDEvent) 
{

	//	PostMessage(STATE_TICKLE,STATE_TICKLE,0);
	CView::OnTimer(nIDEvent);
}

void CFDMSView::OnUpdate(CView* , LPARAM , CObject* ) 
{
	UpdateGraphData();
}

void CFDMSView::OnEditCopy() 
{

	if (testGraph)
	{
		CMetaFileDC * pMetaDC = new CMetaFileDC();
		pMetaDC->CreateEnhanced(GetDC(),NULL,NULL,testGraph->GetGraphTitle());
		//draw meta file

		//do what ever you want to do: bitmaps, lines, text...
		if (testGraph)
			testGraph->DrawGraph(pMetaDC);
		//close meta file dc and prepare for clipboard;
		HENHMETAFILE hMF = pMetaDC->CloseEnhanced();

		//copy to clipboard
		OpenClipboard();
		EmptyClipboard();
		::SetClipboardData(CF_ENHMETAFILE,hMF);
		CloseClipboard();
		//DeleteMetaFile(hMF);
		delete pMetaDC;
	}
}

void CFDMSView::UpdateGraphData()
{
}


