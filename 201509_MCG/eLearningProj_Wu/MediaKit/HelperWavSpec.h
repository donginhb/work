#ifndef _HELPERWAVSPEC_H_
#define _HELPERWAVSPEC_H_

BOOL WavFileMixer(LPSTR szFile, CDWordArray &TimeArray, CStringArray &cstArray, CDWordArray &StartArray, CDWordArray &StopArray, LPSTR szOutput);
BOOL WavFileReplacer(LPSTR szFile, CDWordArray &TimeArray, CStringArray &cstArray, CDWordArray &StartArray, CDWordArray &StopArray, LPSTR szOutput);

#endif