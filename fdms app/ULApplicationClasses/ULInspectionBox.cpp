// ULInspectionBox.cpp : implementation file
// $History: ULInspectionBox.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h" //REQT:  Change this to use the new application header.
#include "ULInspectionBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULInspectionBox dialog


ULInspectionBox::ULInspectionBox(CWnd* pParent /*=NULL*/)
	: CDialog(ULInspectionBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULInspectionBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ULInspectionBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULInspectionBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULInspectionBox, CDialog)
	//{{AFX_MSG_MAP(ULInspectionBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULInspectionBox message handlers
