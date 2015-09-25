#include <afxwin.h>
#include "HelperFree.h"

BOOL ParseDrawUnit(long lWidth, long lHeight, LPBYTE pBuffer, DWORD cbBuffer, DWORD &dwOffset, DRAWUNIT *punit)
{
	memset(punit, 0, sizeof(DRAWUNIT));

	punit->lWidth = lWidth;
	punit->lHeight = lHeight;

	if(dwOffset+4>cbBuffer) return FALSE;
	punit->lUnitID = *(long*)(pBuffer+dwOffset);
	dwOffset += 4; 

	if(dwOffset+4>cbBuffer) return FALSE;
	punit->crPen = *(COLORREF*)(pBuffer+dwOffset);
	dwOffset += 4; 

	if(dwOffset+4>cbBuffer) return FALSE;
	punit->lPenWidth  = *(long*)(pBuffer+dwOffset);
	dwOffset += 4;

	if(dwOffset+4>cbBuffer) return FALSE;
	punit->lPointCount = *(long*)(pBuffer+dwOffset);
	dwOffset += 4;

	punit->Points=(POINTUNIT *)malloc(punit->lPointCount*sizeof(POINTUNIT));
	if(punit->Points==NULL) return FALSE;
	memset(punit->Points, 0, punit->lPointCount*sizeof(POINTUNIT));
	
	for(long ii=0; ii<punit->lPointCount; ii++) {
		if(dwOffset+4>cbBuffer) return FALSE;
		punit->Points[ii].lTime = *(long*)(pBuffer+dwOffset);
		dwOffset += 4;
		punit->Points[ii].xx = *(WORD*)(pBuffer+dwOffset);
		dwOffset += 2;
		punit->Points[ii].yy = *(WORD*)(pBuffer+dwOffset);
		dwOffset += 2;

		if(ii==0) { 
			punit->lTimeMin = punit->Points[0].lTime;
			punit->lTimeMax = punit->Points[0].lTime;
		}
		else {
			if(punit->Points[ii].lTime<punit->lTimeMin) punit->lTimeMin = punit->Points[ii].lTime;
			if(punit->Points[ii].lTime>punit->lTimeMax) punit->lTimeMax = punit->Points[ii].lTime;
		}
	}

	return TRUE;
}

BOOL ParseBuffer(LPBYTE pBuffer, DWORD cbBuffer, CPtrArray &DrawUnits)
{
	Helper_DrawUnitsFree(DrawUnits);

	DWORD dwOffset=0;
	if(cbBuffer<4) return FALSE;
	long lVer = *(long*)(pBuffer+dwOffset);
	dwOffset += 4;
	long lWidth=*(long*)(pBuffer+dwOffset);
	dwOffset += 4;
	long lHeight=*(long*)(pBuffer+dwOffset);
	dwOffset += 4;

	if((lWidth==0)||(lHeight==0))
		return FALSE;

	BOOL fOK=FALSE;	
	while(dwOffset<cbBuffer) {				
		DRAWUNIT	*punit = (DRAWUNIT*)malloc(sizeof(DRAWUNIT));
		if(punit==NULL)	{
			fOK = FALSE;
			break;
		}

		fOK = ParseDrawUnit(lWidth, lHeight, pBuffer, cbBuffer, dwOffset, punit);		
		if(!fOK) {
			free(punit);
			break;
		}

		DrawUnits.Add(punit);
	}

	if(!fOK) Helper_DrawUnitsFree(DrawUnits);

	return fOK;
}

BOOL ParseFile(CFile &cf, CPtrArray &DrawUnits)
{	
	DWORD cbBuffer = cf.GetLength();
	LPBYTE pBuffer = (LPBYTE)malloc(cbBuffer);
	if(pBuffer==NULL)
		return FALSE;
	
	BOOL fOK = (cbBuffer==cf.Read(pBuffer, cbBuffer));
	if(fOK) {
		fOK = ParseBuffer(pBuffer, cbBuffer, DrawUnits);
	}

	free(pBuffer);

	return fOK;
}

BOOL Helper_ParseFile2DrawUnits(LPSTR szFile, CPtrArray &DrawUnits)
{
	CFile cf;
	if(!cf.Open(szFile, CFile::modeRead|CFile::shareDenyWrite, NULL))
		return FALSE;

	BOOL fOK = ParseFile(cf, DrawUnits);

	cf.Close();	

	return fOK;
}

void Helper_DrawUnitsFree(CPtrArray &DrawUnits)
{
	for(long ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue;
		if(punit->Points==NULL) continue;

		free(punit->Points);
		free(punit);
	}
	DrawUnits.RemoveAll();
}

BOOL Helper_DrawUnitsDumpJScript(CPtrArray &DrawUnits, long lmsCutTime, LPSTR szJSFile)
{
	char szLF[3] = { 13, 10, 0 };

	CFile	cf;
	if(!cf.Open(szJSFile, CFile::modeCreate|CFile::modeWrite, NULL)) {
		Helper_DrawUnitsFree(DrawUnits);
		return FALSE;
	}

	CString cst, cstCond, cstTemp;
// dump global variables
	cst = "var g_usepen = \"\";"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_param = \"\";"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_timerID = -1;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_time = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

	cst = "var g_nSL = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_nST = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_nSW = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_nSH = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_nXOff = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst = "var g_nYOff = 0;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	
	HDC hdc = GetDC(NULL);
	if(hdc==NULL) return FALSE;
	long lWidth = GetDeviceCaps(hdc, HORZRES);
	long lHeight = GetDeviceCaps(hdc, VERTRES);
	ReleaseDC(NULL, hdc);
	
	cst.Format("var g_nScrWidth = %ld", lWidth); cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst.Format("var g_nScrHeight = %ld", lHeight);; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

// dump all freehand drawunits
	int ii=0;
	for(ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue; // empty allowed!!

		cst.Format("function fh_%d() {", punit->lUnitID); cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		cst.Format("    g_usepen = \"%d,%d\";", punit->crPen, punit->lPenWidth); cst += szLF;		
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		cst.Format("    if(g_time>%d) fh_main_stop();", (punit->lTimeMax-punit->lTimeMin+lmsCutTime-1)/lmsCutTime*lmsCutTime); cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		cst =      "    else {"; cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
		
		long lTimeBase=lmsCutTime;
		CString cstCond, cstParam, cstCachePre, cstCacheNow;

		BOOL fNeedDump=FALSE;
		cstCond = "";
		int jj=0;
		while(jj<punit->lPointCount) {
			fNeedDump = FALSE;
			if(punit->Points[jj].lTime<=lTimeBase) {
				cstCacheNow.Format("%0.4f,%0.4f", (double)punit->Points[jj].xx/punit->lWidth, (double)punit->Points[jj].yy/punit->lHeight);
				if(cstParam!="") cstParam += ",";
				cstParam += cstCacheNow;
				jj++;

				if(jj==punit->lPointCount) 
					fNeedDump = TRUE;
			}
			else fNeedDump = TRUE;

			if(fNeedDump) {
				if(cstParam!="") {
					cst = "        ";
					if(cstCond!="") cst += "else ";				
					cstCond.Format("if(g_time<=%d) ", lTimeBase);
					if(jj!=punit->lPointCount) cst += cstCond;
					cst += "g_param = \"";
					if(cstCachePre!="") {
						cst += cstCachePre;
						cst += ",";					
					}
					cstCachePre = cstCacheNow;
					cst += cstParam;
					cst += "\";"; cst += szLF;
					cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
				}
				lTimeBase += lmsCutTime;
				cstParam = "";
			}
		}
		
		cst = "        document.DrawUtility.DrawFreeHand(g_usepen, g_param, g_nSL, g_nST, g_nSW, g_nSH, g_nXOff, g_nYOff);"; cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		cst = "    }"; cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		
		cst.Format("    g_time = g_time + %d;", lmsCutTime); cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		cst = "}"; cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	}

// dump fh_main_stop
	cst.Format("function fh_main_stop() {");	cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst.Format("    if(g_timerID!=-1) clearInterval(g_timerID);");	cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst.Format("    g_timerID = -1; g_time = 0;");	cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	cst.Format("}"); cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

// dump fh_main_start
	cst.Format("function fh_main_start(id, nSL, nST, nSW, nSH, nXOff, nYOff) {");	cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

	cst = "    g_nSL = nSL; g_nST = nST; g_nSW = nSW; g_nSH = nSH; g_nXOff = nXOff; g_nYOff = nYOff;"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

	cst.Format("    fh_main_stop();");	cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

	cstCond = "";
	for(ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue; // empty allowed!!
		
		cst = "    ";
		if(cstCond!="") cst += "else ";
		cstCond.Format("if(id==%d) g_timerID = setInterval(\"fh_%d()\", %d);", punit->lUnitID, punit->lUnitID, lmsCutTime);
		cst += cstCond; cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
	}
	cst = "}"; cst += szLF;
	cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

	cf.Close();

	return TRUE;
}

BOOL Helper_DrawUnitsDumpText(CPtrArray &DrawUnits, LPSTR szTextFile)
{
/*
	char szLF[3] = { 13, 10, 0 };

	CFile	cf;
	if(!cf.Open(szTextFile, CFile::modeCreate|CFile::modeWrite, NULL)) {
		Helper_DrawUnitsFree(DrawUnits);
		return FALSE;
	}

	CString cst, cstTemp;
	for(long ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue;
		cst.Format("PAGE:%d COLOR:%d, PEN:%d, RINGS:%d", punit->lPageNo, punit->crPen, punit->lPenWidth, punit->lPointCount);
		cst += szLF;
		cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());

		cst = "";
		if(punit->Points==NULL) continue;
		for(long jj=0; jj<punit->lPointCount; jj++) {
			POINTUNIT *punit=punit->Points+jj;
			if(punit==NULL) continue;
			cst.Format("POINTUNIT:%d, POINTS:%d", punit->ID, punit->lPoint);
			cst += szLF;
			cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
			if(punit->Points==NULL) continue;

			cst = "";
			for(long kk=0; kk<punit->lPoint; kk++) {
				cstTemp.Format("%d,%d", LOWORD(punit->Points[kk]), HIWORD(punit->Points[kk]));
				if(kk!=punit->lPoint-1) 
					cstTemp += ",";
				else 
					cstTemp += szLF;
				cst += cstTemp;
			}
			cf.Write((LPSTR)(LPCSTR)cst, cst.GetLength());
		}
	}

	cf.Close();
*/
	return TRUE;
}

BOOL Helper_DrawUnitsPaint(HDC hdc, CPtrArray &DrawUnits)
{
	for(long ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue;
		if(punit->Points==NULL) continue;
		
		for(long kk=0; kk<punit->lPointCount; kk++) {				
			if(kk==0) {
				POINT pt;
				MoveToEx(hdc, punit->Points[kk].xx, punit->Points[kk].yy, &pt);
			}
			else {
				LineTo(hdc, punit->Points[kk].xx, punit->Points[kk].yy);
			}
		}
	}

	return TRUE;
}

BOOL Helper_DrawUnitsRefine(CPtrArray &DrawUnits)
{
	for(long ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue;
		if(punit->Points==NULL) continue;

		long lUnit=punit->lPenWidth + 6;
		long l1_2Unit=(lUnit+1)/2;
		long l3_4Unit=(lUnit*3/4);

		CDWordArray times;
		CDWordArray pts;

		pts.Add(MAKELONG(punit->Points[0].xx, punit->Points[0].yy));
		times.Add(punit->Points[0].lTime);
		int xx=punit->Points[0].xx, yy = punit->Points[0].yy;
		int xTemp=0, yTemp=0;
		for(long kk=1; kk<punit->lPointCount; kk++) {
			xTemp = xx - punit->Points[kk].xx;
			yTemp = yy - punit->Points[kk].yy;
				
			if(xTemp<0) xTemp = -xTemp;
			if(yTemp<0) yTemp = -yTemp;

			if( (xTemp>=lUnit)||(yTemp>=lUnit)||
				((xTemp>=l3_4Unit)&&(yTemp>=l1_2Unit))||
				((yTemp>=l3_4Unit)&&(xTemp>=l1_2Unit))||
				(kk==punit->lPointCount-1)) {

				xx = punit->Points[kk].xx;
				yy = punit->Points[kk].yy;
					
				pts.Add(MAKELONG(punit->Points[kk].xx, punit->Points[kk].yy));
				times.Add(punit->Points[kk].lTime);
			}
		}

		punit->lPointCount = pts.GetSize();
		for(long pp=0; pp<punit->lPointCount; pp++) {
			punit->Points[pp].xx = LOWORD(pts.GetAt(pp));
			punit->Points[pp].yy = HIWORD(pts.GetAt(pp));
			punit->Points[pp].lTime = times.GetAt(pp);
		}
	}

	return TRUE;
}

BOOL Helper_DrawUnitsRemoveID(CPtrArray &DrawUnits, long ID)
{
	BOOL fOK=FALSE;
	for(long ii=0; ii<DrawUnits.GetSize(); ii++) {
		DRAWUNIT *punit= (DRAWUNIT *)DrawUnits.GetAt(ii);
		if(punit==NULL) continue;

		if(punit->lUnitID==ID) {
			if(punit->Points!=NULL) free(punit->Points);
			DrawUnits.RemoveAt(ii);
			break;
		}
	}

	return fOK;
}
