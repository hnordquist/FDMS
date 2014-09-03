// GraphLegendSet.cpp : implementation file
//

#include "stdafx.h"
#include "GraphLegendSet.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphLegendSet

CGraphLegendSet::CGraphLegendSet()
{
	legendId = 0;
	legendLabel = "";
	m_bDoShow = true;
}

CGraphLegendSet::~CGraphLegendSet()
{
}


BEGIN_MESSAGE_MAP(CGraphLegendSet, CStatic)
	//{{AFX_MSG_MAP(CGraphLegendSet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphLegendSet message handlers

void CGraphLegendSet::SetLegendLabel(int id, CString legendText, COLORREF cColor, int iStyle, bool bDoShow)
{
	legendId = id;
	legendLabel = legendText;
	m_cColor = cColor;
	m_iStyle = iStyle;
	m_bDoShow = bDoShow;
}

COLORREF CGraphLegendSet::GetLegendColor()
{
	return m_cColor;
}

int CGraphLegendSet::GetLegendStyle()
{
	return m_iStyle;
}

CString CGraphLegendSet::GetLegendLabel()
{
	return legendLabel;
}

bool CGraphLegendSet::GetDoShow()
{
	return m_bDoShow;
}