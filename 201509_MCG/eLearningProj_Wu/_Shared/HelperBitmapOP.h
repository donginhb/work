#ifndef _HELPERBMPOP_H_
#define _HELPERBMPOP_H_

#define REDMASK_565		0xF800
#define GREENMASK_565	0x07E0
#define BLUEMASK_565	0x001F

#define REDMASK_555		0x7C00
#define GREENMASK_555	0x03E0
#define BLUEMASK_555	0x001F

#define REDMASK			0x

#define COLORREF_GETRED(rgb)        ((rgb) & 0xff)
#define COLORREF_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)
#define COLORREF_GETBLUE(rgb)       (((rgb) >> 16) & 0xff)
#define COLORREF_MAKE(r, g, b)      ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#define RGB555_GETRED(rgb555)		(((rgb555) & REDMASK_555) >> 7) // >> 10 << 3 = >> 7
#define RGB555_GETGREEN(rgb555)		(((rgb555) & GREENMASK_555) >> 2) // >> 5 << 3 = >> 2
#define RGB555_GETBLUE(rgb555)		(((rgb555) & BLUEMASK_555) << 3) // << 3 = << 2

#define RGB565_GETRED(rgb56)		(((rgb565) & REDMASK_565) >> 8) // >> 11 << 3 = >> 8
#define RGB565_GETGREEN(rgb565)		(((rgb565) & GREENMASK_565) >> 3) // >> 5 << 2 = >> 3
#define RGB565_GETBLUE(rgb565)		(((rgb565) & BLUEMASK_565) << 3) // << 3 = << 3

#define RGB555_MAKE(rr, gg, bb)		((((int)rr>>3)<<10)+(((int)gg>>3)<<5)+((int)bb>>3))
#define RGB565_MAKE(rr, gg, bb)		((((int)rr>>3)<<11)+(((int)gg>>2)<<5)+((int)bb>>3))

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((DWORD)(bi).biWidth * (DWORD)(bi).biBitCount)
#define _DIBSIZE(bi) (DIBWIDTHBYTES(bi) * (DWORD)(bi).biHeight)
#define DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(_DIBSIZE(bi)) : _DIBSIZE(bi))

#endif