#ifndef _WMXCOPY_H_
#define _WMXCOPY_H_

#include "WMXDumper.h"
class CWMXCopy : public CWMXDumper {
public:
	CWMXCopy();
	~CWMXCopy();
public:
	BOOL MakeCopy(BSTR bstrWMX, BOOL fVideo, BOOL fScript, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput);
};
#endif