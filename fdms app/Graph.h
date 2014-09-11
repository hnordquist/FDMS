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
//Graph.h - Version 3.0 (Brian Convery, May, 2001)

#if !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
#define AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graph.h : header file
//
#define ID_DRAW_SCATTER	5000
#define ID_SPECTRUM_GET	5001
#define ID_SPECTRUM_CLEAR 5002


#define ID_GRAPH_DATA	(WM_USER + 1000)
#define ID_GRAPH_HELP	(WM_USER + 1001)

enum GraphCommands {SET_LINEAR,				
					SET_NL_CYCLES,			SET_L_CYCLES,
					SET_MARKER_SIZE,
					SET_X_TICK_FONT_SIZE,	SET_Y_TICK_FONT_SIZE,
					SET_LEGEND_FONT_SIZE,	
					SET_RIGHT_AXIS,
					SET_X_AXIS_LABEL,		
					SET_Y_AXIS_LABEL,	SET_YR_AXIS_LABEL,
					SET_Y_AXIS_LABEL_COLOR,	SET_YR_AXIS_LABEL_COLOR,
					SET_GRAPH_TYPE,
					SET_LEGEND,				
					SET_GRAPH_TITLE,		SET_GRAPH_TITLE_LINE2,	
					SET_H_GRIDLINES,		SET_V_GRIDLINES,
					SET_X_TEXT_LINES,
					SET_X_TICK_LIMITS,		SET_Y_TICK_LIMITS, SET_YR_TICK_LIMITS,
					REMOVE_SERIES,			REMOVE_ALL_SERIES,
					REMOVE_LEGEND,
					REMOVE_ALL_LEGEND,
					SET_X_AXIS_LEGEND,		
					REMOVE_ALL_DATA,		
					ADD_SERIES,				ADD_RIGHT_SERIES,
					ADD_SERIES_DATA,		ADD_RIGHT_SERIES_DATA,
					MOUSE_CONTROL,
					SHOW_WINDOW,
					INVALIDATE,
					TOGGLE_DISPLAY_GROUP
					};

#define GRAPHMOUSE_GRID		0x00000001	/*toggle which grid lines display	*/
#define GRAPHMOUSE_DATA		0x00000002	/*display selected data				*/
#define GRAPHMOUSE_LCYCLE	0x00000004	/*Y axis number of cycles from left	*/
#define GRAPHMOUSE_RCYCLE	0x00000008	/*Y axis number of cycles from right*/
#define GRAPHMOUSE_XCYCLE	0x00000010	/*X axis number of cycles			*/
#define GRAPHMOUSE_XLINEAR	0x00000020	/*X axis linear						*/
#define GRAPHMOUSE_YLINEAR	0x00000040	/*Y axis linear						*/
#define GRAPHMOUSE_LBCYCLE	0x00000080	/*Y axis base cycle left side		*/
#define GRAPHMOUSE_RBCYCLE	0x00000100	/*Y axis base cycle right side		*/
#define GRAPHMOUSE_XBCYCLE  0x00000200	/*X axis base cycle					*/
#define GRAPHMOUSE_LEGEND	0x00000400	/*toggle legend display				*/

//graph type definitions
#define BAR_GRAPH				0
#define LINE_GRAPH				1	//lines between plotted points
#define PIE_GRAPH				2
#define SCATTER_GRAPH			3

#define VERTICAL_ALIGN		1
#define HORIZONTAL_ALIGN	0

/////////////////////////////////////////////////////////////////////////////
// CGraph window

#include "GraphSeries.h"
#include "GraphLegend.h"
//#include "UWNCCDlg.h"

extern HWND g_hWndGraph;

class CGraph : public CStatic
{
// Construction
public:
	CGraph();
	CGraph(int type);
// Attributes
public:
	BOOL graphHasLegend;
	bool m_bInDraw;
private:
	double tickXSpace;
	double tickYSpace;
	double tickYRSpace;
	double m_tickStep;
	double m_XtickStepValue;
	double m_minXTickValue, m_maxXTickValue, m_XtickStep;
	double m_minYTickValue, m_maxYTickValue, m_YtickStep;
	double m_minYRTickValue, m_maxYRTickValue, m_YRtickStep;
	CObList* graphSeries;
	CObList* graphRSeries;
	int graphType;	//0 - bar graph, 1 - line graph, 2 - pie
	int graphAlignment;	//0 - horizontal, 1 - vertical
	int maxHeight;
	int maxWidth;
	CObList* colorList;
	CString axisYLabel;
	COLORREF axisYLabelColor;
	CString axisYRLabel;
	COLORREF axisYRLabelColor;
	COLORREF m_cColorTable[10];
	CString axisXLabel;
	CRect graphRect;
	CRect m_cClipRect;
	int m_xFontSize;
	int m_yFontSize;
	int yApexPoint;	//yApex is the number of pixels from top to draw the x axis(bottom of y axis line)
	int xApexPoint;	//xApex is number of pixels from left to draw y axis (leftmost point of x axis line)
	int yAxisHeight;
	int xAxisWidth;
	int yTickFontSize;
	int xTickFontSize;
	int legendFontSize;
	double numXTicks, numYTicks;
	double minXTick, maxXTick;
	double minYTick, maxYTick;
	double minYRTick, maxYRTick;
	int iXMarkerSize, iYMarkerSize;
	int seriesSize;	//number of data elements to map per series
	CGraphLegend graphLegend;
	CGraphLegend graphXAxisLegend;
	int legendWidth;
	int xAxisAlign;	//# - degree rotation (0 = horizontal, 90 = vertical, etc)
	int xAxisLabelLength;
	int yAxisLabelLength;	//in case I ever need it
	int yRAxisLabelLength;
	CString graphTitle;
	CString graphTitleLine2;
	BOOL graphHasVGridLines;
	BOOL graphHasHGridLines;
	int legendMaxText;
	int topYTick;
	int rightXTick;
	int depth;	//for 3D graphs
	double depthRatio;
	int graphQuadType;
	BOOL quadSetManually;
	BOOL inRedraw;

	//print settings
	int topMargin;
	int bottomMargin;
	int leftMargin;
	int rightMargin;
//	int pGraphL, pGraphT, pGraphB, pGraphR;
	int p_topYTick;
	int p_rightXTick;
	bool m_bSelectedData;
	double m_dSelectedX;
	double m_dSelectedY;
	CString m_cSelectedLabel;
	CString m_cSelectedLabelX;
	CString m_cSelectedLabelY;
	bool bXAxisLinear;
	bool bYAxisLinear;
	int iXLNumCycles;
	int iYLNumCycles;
	int iXLBaseCycle;
	int iYLBaseCycle;
	int iYRLBaseCycle;
	int iXNLNumCycles;
	int iYNLNumCycles;
	int iXNLBaseCycle;
	int iYNLBaseCycle;
	int iYRNLBaseCycle;
	int iMarkerSize;
	int m_iXTextLines;
	bool bHasLegendInfo;
	bool m_bHasRightAxis;

	CRect cLegendRect;
	CRect cXAxisRect;
	CRect cYAxisRect;
	CRect cYRAxisRect;
	CRect cXYAxisRect;
	CRect cPlotRect;
//	CRect cHelpRect;
//	CRect cSendRect;
//	CRect cEditRect;
//	CRect cSpinRect;
//	CRect cHistRect;
//	CRect cSpecRect;
	struct SLegendCont {
		bool bValid;
		CRect cRect;
		int iGroup;
		bool bDoDraw;
	} cLegendContents[100];

// Operations
public:
//	void SetAutoScale(bool bAuto) { bAutoScale = bAuto;};

	void GetPlotBox  (CRect *cRect){memcpy(cRect,cPlotRect,sizeof(CRect));};
	void GetLegendBox(CRect *cRect){memcpy(cRect,cLegendRect,sizeof(CRect));};
	bool GetLegendContentsBox(int iIndex, CRect *cRect)
	{
		if (cLegendContents[iIndex].bValid)
			memcpy(cRect,cLegendContents[iIndex].cRect,sizeof(CRect));
		return cLegendContents[iIndex].bValid;
	};
	void ToggleDisplayEqGroup(int iIndex);
	void SaveFile(CString cString);
	void LoadFile(CString cString);
//	void GetHistBox  (CRect *cRect){memcpy(cRect,cHistRect,sizeof(CRect));};
//	void GetSpinBox  (CRect *cRect){memcpy(cRect,cSpinRect,sizeof(CRect));};
//	void GetEditBox  (CRect *cRect){memcpy(cRect,cEditRect,sizeof(CRect));};
//	void GetHelpBox  (CRect *cRect){memcpy(cRect,cHelpRect,sizeof(CRect));};
//	void GetSendBox  (CRect *cRect){memcpy(cRect,cSendRect,sizeof(CRect));};
//	void GetSpecBox  (CRect *cRect){memcpy(cRect,cSpecRect,sizeof(CRect));};
	void GetXAxisBox (CRect *cRect){memcpy(cRect,cXAxisRect,sizeof(CRect));};
	void GetYAxisBox (CRect *cRect){memcpy(cRect,cYAxisRect,sizeof(CRect));};
	void GetYRAxisBox(CRect *cRect){memcpy(cRect,cYRAxisRect,sizeof(CRect));};
	void GetXYAxisBox(CRect *cRect){memcpy(cRect,cXYAxisRect,sizeof(CRect));};
	bool GetDataItemAt(CPoint cPoint, double* dX, double *dY);
	void SetRightAxis(bool bRight){m_bHasRightAxis = bRight;};
	BOOL GetHasLegendInfo() {return bHasLegendInfo;};
	void SetHasLegendInfo(bool bLegend) {bHasLegendInfo = bLegend;};
	CString GetGraphTitle();
	CString GetGraphTitle2();

	void SetXAxisTextLines(int iLines);
	void SetLinear(bool bXAxis, bool bYAxis);
	void GetLinear(bool *bXAxis, bool *bYAxis) { *bXAxis = bXAxisLinear; *bYAxis = bYAxisLinear;};
	void SetNLCycles(int iXCycles, int iXBaseCycle, int iYCycles, int iYBaseCycle, int iYRBaseCycle = NULL) 
	{
		iXNLNumCycles = max(1,min(iXCycles, 1200));
		iXNLBaseCycle = iXBaseCycle;
		iYNLNumCycles = max(1,min(iYCycles,1200));
		iYNLBaseCycle = iYBaseCycle;
		iYRNLBaseCycle = iYRBaseCycle;
	};

	void SetLCycles(int iXCycles, int iXBaseCycles, int iYCycles, int iYBaseCycles, int iYRBaseCycles = NULL) 
	{
		iXLNumCycles = max(1,iXCycles);
		iXLBaseCycle = iXBaseCycles;
		iYLNumCycles = max(1,iYCycles);
		iYLBaseCycle = iYBaseCycles;
		iYRLBaseCycle = iYRBaseCycles;
	};
	void GetNLCycles(int *iXCycles, int *iXBaseCycles, int *iYCycles, int *iYBaseCycles, int *iYRBaseCycles = NULL)
	{
		 *iXCycles = iXNLNumCycles;
		 *iXBaseCycles = iXNLBaseCycle;
		 *iYCycles = iYNLNumCycles;
		 *iYBaseCycles = iYNLBaseCycle;
		 if (iYRBaseCycles)
			 *iYRBaseCycles = iYRNLBaseCycle;
	}

	void CGraph::GetXTickLimits(double* pminTickValue, double *pmaxTickValue, double* ptickStep)
	{
		*pminTickValue = m_minXTickValue;
		*pmaxTickValue = m_maxXTickValue;
		*ptickStep = m_XtickStep;
	}

	void CGraph::GetYTickLimits(double*pminTickValue, double*pmaxTickValue, double*ptickStep)
	{
		*pminTickValue = m_minYTickValue;
		*pmaxTickValue = m_maxYTickValue;
		*ptickStep = m_tickStep;
	}
	void CGraph::GetYRTickLimits(double*pminTickValue, double*pmaxTickValue, double*ptickStep)
	{
		*pminTickValue = m_minYRTickValue;
		*pmaxTickValue = m_maxYRTickValue;
		*ptickStep = m_tickStep;
	}

	void GetLCycles(int *iXCycles, int *iXBaseCycles, int *iYCycles, int *iYBaseCycles, int *iYRBaseCycles = NULL)
	{
		 *iXCycles = iXLNumCycles;
		 *iXBaseCycles = iXLBaseCycle;
		 *iYCycles = iYLNumCycles;
		 *iYBaseCycles = iYLBaseCycle;
		 if (iYRBaseCycles)
			 *iYRBaseCycles = iYRLBaseCycle;
	}

	void SetMarkerSize(int iSize) {iMarkerSize = iSize;};
	bool IsXLinear(){return bXAxisLinear;};
	bool IsYLinear(){return bYAxisLinear;};

	void SetXTickFontSize(int size);
	void SetYTickFontSize(int size);
	void SetLegendFontSize(int size);
	void DrawGraph(CDC* pDC);
	void AddSeries(CGraphSeries* dataSet, bool bToRight = false);
	void SetXAxisLabel(CString label);
	void SetYAxisLabel(CString label, COLORREF cColor = NULL);
	void SetYRAxisLabel(CString label, COLORREF cColor = NULL);
	void SetLegend(int datagroup, CString label, COLORREF cColor = NULL, int iStyle=0, bool bDoDraw = true);
	void SetXAxisLegend(CString label, COLORREF cColor = NULL);
	void SetXAxisAlignment(int alignValue);
	void SetGraphType(int gType);
	void SetGraphTitle(CString title);
	void SetGraphTitleLine2(CString title);
	int  PrintGraph(CDC *pDC, CPrintInfo* pInfo);
	void SetMargins(int top, int bottom, int left, int right, int graphTop);
	void SetGridLines(BOOL hasGridLines);
	void SetHGridLines(BOOL hasGridLines);
	void SetVGridLines(BOOL hasGridLines);
	BOOL GetVGridLines(){return graphHasVGridLines;};
	BOOL GetHGridLines(){return graphHasHGridLines;};
	void SetGraphAlignment(int alignment);  //0 - vertical, 1 horizontal
	int  GetGraphAlignment(){return graphAlignment;};
	void SetTickLimits(int minTick, int maxTick, int tickStep);
	void SetTickLimits(double minTick, double maxTick, double tickStep);
	void SetXTickLimits(int minTick, int maxTick, int tickStep);
	void SetYTickLimits(int minTick, int maxTick, int tickStep);
	void SetYRTickLimits(int minTick, int maxTick, int tickStep);
	void SetXTickLimits(double minTick, double maxTick, double tickStep);
	void SetYTickLimits(double minTick, double maxTick, double tickStep);
	void SetYRTickLimits(double minTick, double maxTick, double tickStep);
	void SetColor(int dataGroup, COLORREF groupColor);
	void RemoveSeries(CString label);
	void RemoveColor(int dataGroup);
	void RemoveAllSeries();
	void RemoveAllColors();
	void RemoveLegend(int dataGroup);
	void RemoveAllLegends();
	void RemoveAllData();	//removes all series, legends and colors
	void SetGraphQuadType(int quads);

private:
	void DrawMarker(CDC* pDC, int iX, int iDX, int iY, int iDY, int iStyle, CBrush* wbrush);
	void FinishInit();
	void DrawAxis(CDC* pDC);
	void DrawSeries(CDC* pDC);
	int  DrawLegend(CDC* pDC);
	COLORREF GetColor(int dataGroup);
	int GetXAxisAlignment();

	void DrawBarSeries(CDC* pDC);
	void DrawLineSeries(CDC* pDC);
	void DrawPieSeries(CDC* pDC);
	void DrawScatterSeries(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraph)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
