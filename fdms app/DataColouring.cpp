#include "stdafx.h"
#include ".\datacolouring.h"

#define LIGHTBLUE1		RGB(221,238,255)
#define LIGHTBLUE2		RGB(193,224,255)
#define LIGHTYELLOW1	RGB(255,255,221)
#define LIGHTYELLOW2	RGB(255,255,199)
#define LIGHTORANGE1	LIGHTGREEN1
#define LIGHTORANGE2	LIGHTGREEN2
//#define LIGHTORANGE1	RGB(255,146,72)
//#define LIGHTORANGE2	RGB(255,197,157)
#define LIGHTGRAY1		RGB(210,210,210)
#define LIGHTGRAY2		RGB(165,165,165)
#define LIGHTGREEN1		RGB(221,255,221)
#define LIGHTGREEN2		RGB(199,255,199)
#define LIGHTRED1		RGB(255,125,125)
#define LIGHTRED2		RGB(255,190,190)
#define LIGHTCYAN1		RGB(170,255,255)
#define LIGHTCYAN2		RGB(210,255,255)

#define WARNINGMAGENTA	RGB(255,0,138)
#define WARNINGRED		RGB(233, 13,1)
#define WARNINGLIGHTPINK1	RGB(255,218,255)
#define WARNINGLIGHTPINK2	RGB(255,192,255)
#define WARNINGDARKPINK RGB(158,1,100)

DataColouring::DataColouring(void)
{
}

DataColouring::~DataColouring(void)
{
}



CELLCOLOURING DataColouring::m_CellColours[]
=
{
	// light
		// text
			//edcStatic,
			//edcEditable
			//edcEditAdvisable,
			//edcMustEdit,
			//edcGoodValue
		// bkg
			//edcStatic,
			//edcEditable
			//edcEditAdvisable,
			//edcMustEdit,
			//edcGoodValue
	// dark

	{ // light
		

			{BLACK,		//edcStatic, 
			 BLACK,		//edcEditable,
			 WARNINGMAGENTA, //edcEditAdvisable,
			 WARNINGMAGENTA, //edcMustEdit,
			 BLACK //edcGoodValue
			}, // text
			{WHITE, LIGHTBLUE1, LIGHTBLUE1, LIGHTBLUE1, LIGHTGREEN1}, // bkg
	
	},
	{ // dark
		
		
			{BLACK, BLACK, WARNINGRED, WARNINGRED, BLACK}, // text
			{WHITE, LIGHTBLUE2, LIGHTBLUE2, LIGHTBLUE2, LIGHTGREEN2}, // bkg
	
	}
};

/*
18 indicators, 8 unique colors, need light and dark for each of the 8

.	BLUE +

.	ORANGE s
.	BLACK s
.	GREEN s
.	RED s

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
ROWCOLOURING DataColouring::m_RowColours[] =
{
	{ // light
			{BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}, // text
			{LIGHTBLUE1, // TODO Black v. Grey
			LIGHTORANGE1, LIGHTGRAY1, LIGHTGREEN1, LIGHTRED1,
			LIGHTORANGE1, LIGHTGREEN1, LIGHTBLUE1,
			LIGHTORANGE1, LIGHTGREEN1, LIGHTRED1, LIGHTYELLOW1, LIGHTGRAY1, LIGHTCYAN1,
			LIGHTGRAY1/**/, LIGHTGREEN1, LIGHTGRAY1, LIGHTORANGE1
			}, // bkg
	
	},
	{ // dark
			{BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}, // text
			{LIGHTBLUE2, 
			LIGHTORANGE2, LIGHTGRAY2, LIGHTGREEN2, LIGHTRED2,
			LIGHTORANGE2, LIGHTGREEN2, LIGHTRED2,
			LIGHTORANGE2, LIGHTGREEN2, LIGHTRED2, LIGHTYELLOW1, LIGHTGRAY2, LIGHTCYAN2,
			LIGHTGRAY2/**/, LIGHTGREEN2, LIGHTGRAY2, LIGHTORANGE2
			}, // bkg
	
	}
};
