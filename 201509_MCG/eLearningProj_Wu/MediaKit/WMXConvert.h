#ifndef _WMXCONVERT_H_
#define _WMXCONVERT_H_

#include "WMXDumper.h"
class CWMXConvert : public CWMXDumper {
public:
	CWMXConvert();
	~CWMXConvert();
public:
	BOOL MakeConvert(BSTR bstrWMX, BSTR bstrProfile, BOOL fCopyHeader, BOOL fCopyScript, BOOL fCopyMarker, BSTR bstrOutput);
};
#endif