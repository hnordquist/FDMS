#pragma once

typedef enum
{
	edcStatic,
	edcEditable,
	edcEditAdvisable,
	edcMustEdit,
	edcGoodValues,
	edcEnumCount
} tDataColouring;

typedef struct
{
	COLORREF text[edcEnumCount];
	COLORREF bkg[edcEnumCount];
} CELLCOLOURING;

typedef struct
{
	COLORREF text[18];  // array's here are keyed to the BMP . . . these are values for entire rows AFTER computation and acceptance
	COLORREF bkg[18];
} ROWCOLOURING;

/*
18 indicators, 8 unique colors, need light and dark for each of the 8

BLUE +

ORANGE s
BLACK s
GREEN s
RED s

ORANGE d
GREEN d
RED d

ORANGE o
GREEN o
RED o
YELLOW o
BLACK o
CYAN o

GRAY +
GREEN +
BLACK +
ORANGE +

*/


// TODO 447/448 
// 1: create global with RGB colours mapping to the graphs bitmap colors: RGB		g_MPStatusColour[COUNT]; count them in IDB_BMP_ALL, add to DataColouring.h
// 2: create global enum mapping the named states into the IDB_BMP_ALL indexed images (maybe this already exists somewhere?)
// 3: set list BKG color for each row in code based on cMyLIstCtrl and EventDlg, take into account the alternate shading used

// TODO: 447/448 this class must be changed to support status-based coloring or rows

class DataColouring
{
public:
	DataColouring(void);
	~DataColouring(void);

	static COLORREF StaticBkg(int light)
	{
		return m_CellColours[light].bkg[edcStatic];
	}
	static COLORREF EditableBkg(int light)
	{
		return m_CellColours[light].bkg[edcEditable];
	}
	static COLORREF EditAdvisableBkg(int light)
	{
		return m_CellColours[light].bkg[edcEditAdvisable];
	}
	static COLORREF MustEditBkg(int light)
	{
		return m_CellColours[light].bkg[edcMustEdit];
	}
	static COLORREF GoodValuesBkg(int light)
	{
		return m_CellColours[light].bkg[edcGoodValues];
	}

	static COLORREF StaticText(int light)
	{
		return m_CellColours[light].text[edcStatic];
	}
	static COLORREF EditableText(int light)
	{
		return m_CellColours[light].text[edcEditable];
	}
	static COLORREF EditAdvisableText(int light)
	{
		return m_CellColours[light].text[edcEditAdvisable];
	}
	static COLORREF MustEditText(int light)
	{
		return m_CellColours[light].text[edcMustEdit];
	}
	static COLORREF GoodValuesText(int light)
	{
		return m_CellColours[light].text[edcGoodValues];
	}

	static COLORREF AnalyzedRowText(int light, int status)  // status is one of the 18 possible indicator values (indexed into the BMP)
	{
		return m_RowColours[light].text[status];
	}
	static COLORREF AnalyzedRowBkg(int light, int status)  // status is one of the 18 possible indicator values (indexed into the BMP)
	{
		if (status < 0) status = 0;
		else if (status >= 18) status = 17;
		return m_RowColours[light].bkg[status];
	}

private:

	static CELLCOLOURING m_CellColours[2];  // 0 is light, 1 is dark, for alternating list rows
	static ROWCOLOURING m_RowColours[2];  // 0 is light, 1 is dark, for alternating list rows
};
