//GraphSeries.h - Version 3.0

#if !defined(AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_)
#define AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphSeries.h : header file
//

#include "GraphDataSet.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries window

class CGraphSeries : public CStatic
{
// Construction
public:
	CGraphSeries();
	CGraphSeries(int iStyle, COLORREF cColor = RGB(0,0,0), bool bDoLines = true, int iGroup = 0);

// Attributes
public:


private:
	CObList *dataValues;	//list of CGraphDataSet objects
	bool bIsInteger;
	bool m_bClip;
	int m_iStyle;
	int m_iLineStyle;
	int	m_iLineWidth;
	COLORREF m_cColor;
	bool m_bDoLines;
	POSITION m_pIterateStart;
	int m_iGroup;
	bool m_bDoDisplay;
	bool m_bSelectable;
	CString seriesLabel;
	CString m_cSeriesLabelX;
	CString m_cSeriesLabelY;

// Operations
public:
	bool GetDoLines(){return m_bDoLines;};
	void SetDoLines(bool bDoLines){m_bDoLines = bDoLines;};
	COLORREF GetColor(){return m_cColor;};
	bool CGraphSeries::IterateGet(double *xValue, double *yValue);
	bool IsInteger(){return bIsInteger;};
	void SetDouble(){bIsInteger = false;};
	void SetLabel(CString label);
	void SetData(int xValue, int yValue);
	void SetData(double xValue, double yValue);
	void SetStyle(int iStyle){m_iStyle = iStyle;};
	void SetLineStyle(int iStyle, int iWidth = 1){m_iLineStyle = iStyle;m_iLineWidth = iWidth;};
	int  GetLineWidth(){return m_iLineWidth;};
	void SetColor(COLORREF cColor){m_cColor = cColor;};
	int  GetStyle(){return m_iStyle;};
	int  GetLineStyle(){return m_iLineStyle;};
	void SetGroup(int iGroup) { m_iGroup = iGroup;};
	int  GetGroup() {return m_iGroup;};
	bool GetDoDisplay(){return m_bDoDisplay;};
	bool GetSelectable(){return m_bSelectable;};
	void SetSelectable(bool bSelectable){m_bSelectable = bSelectable;};
	void SetClip(bool bClip) { m_bClip = bClip;};
	bool GetClip() {return m_bClip;};
	void ToggleDisplayOnEqGroup(int iGroup) { 
		if (iGroup == m_iGroup)
			m_bDoDisplay = !m_bDoDisplay;
	}
	CString GetLabel();
	CString GetLabelX(){return m_cSeriesLabelX;};
	CString GetLabelY(){return m_cSeriesLabelY;};
	void SetLabelX(CString cLabel){m_cSeriesLabelX = cLabel;};
	void SetLabelY(CString cLabel){m_cSeriesLabelY = cLabel;};
	int GetData(int xValue);
	double GetData(double xValue);
	int GetDataCount();
	int GetXDataValue();	//for scatter graphs only
	double GetdXDataValue();
	int GetYDataValue();	//for scatter graphs only
	double GetdYDataValue();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphSeries)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphSeries();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphSeries)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_)
