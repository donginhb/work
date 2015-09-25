#ifndef _HELPERFREE_H_
#define _HELPERFREE_H_

typedef struct {
	long	lTime;
	WORD	xx, yy;
} POINTUNIT;

typedef struct  {
	long		lWidth, lHeight;

	long		lUnitID;	
	long		lTimeMin, lTimeMax;
	COLORREF	crPen;
	long		lPenWidth;	
	long		lPointCount;
	POINTUNIT	*Points;
} DRAWUNIT;

BOOL Helper_ParseFile2DrawUnits(LPSTR szFile, CPtrArray &DrawUnits);
BOOL Helper_DrawUnitsDumpJScript(CPtrArray &DrawUnits, long lmsCutTime, LPSTR szJSFile);
BOOL Helper_DrawUnitsDumpText(CPtrArray &DrawUnits, LPSTR szTextFile);
BOOL Helper_DrawUnitsRemoveID(CPtrArray &DrawUnits, long ID);

BOOL Helper_DrawUnitsRefine(CPtrArray &DrawUnits);
void Helper_DrawUnitsFree(CPtrArray &DrawUnits);	
	
BOOL Helper_DrawUnitsPaint(HDC hdc, CPtrArray &DrawUnits);
#endif