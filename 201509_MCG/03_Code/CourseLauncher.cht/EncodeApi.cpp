#include "stdafx.h"
#include "EncodeApi.h"
#include "math.h"

UINT Mod(double d, UINT n)
{
	int   v_leng, d_sign;
	if (d < n)
		return (UINT)d;
	d /= n;
	char *v = _fcvt(d,10,&v_leng,&d_sign);
	v[v_leng] = '\0';
	return (UINT)((d - atof(v)) * n + 0.1);
}	

UINT Encode(UINT PCode)
{
	return Mod(pow(PCode,3), 33);// C = P**e % n, n=pxq=3x11.
}

UINT Encode1(UINT PCode)
{
	return ((PCode)-37 % 256);// C = P**e % n, n=pxq=3x11.
}

void KeyEncode(LPCTSTR CodeString, LPTSTR EncodedString)
{
	EncodedString[0] = '\0';
	char HexCode[256];
	int nSize = strlen(CodeString);
	for (int i = 0; i < nSize; i++)
	{
		char Code = Encode1((UINT)CodeString[i]+Encode(i%32));

	// Never uses ASCII code as second byte of chinese code,
	// since capital ASCII code will be converted into little char by browser.
		{
			sprintf(HexCode, "%02x", (unsigned char)Code);
			strcat(EncodedString, HexCode);
		}
	}
}

UINT Decode(UINT CCode)
{
	return Mod(pow(CCode, 7), 33);	// P = C**d % n.
}

UINT Decode1(UINT CCode)
{
	return (CCode + 37 + 256) % 256;	// P = C**d % n.
}

void KeyDecode(LPCTSTR CodeString, LPTSTR DecodedString) 
{
	DecodedString[0] = '\0';
	int i;
	int nSize = strlen(CodeString);
	for (i = 0; i < nSize - 1; i++)
	{
		int code = CodeString[i] - '0';
		if (code > 9)
			code = CodeString[i] - 7;
		int cod1 = CodeString[++i] - '0';
		if (cod1 > 9)
			cod1 = CodeString[i] - 'a' + 10;
		
		char sz[2];
		sz[0] = code * 16 + cod1;
		sz[1] = '\0';
		strcat(DecodedString, sz);
	}

	char szTmp[256];
	strcpy(szTmp, DecodedString);
	DecodedString[0] = '\0';
	nSize = strlen(szTmp);
	for (i = 0; i < nSize; i++)
	{
		char sz[2];
		sz[0] = Decode1((UINT)szTmp[i] - Encode(i%37));
		sz[1] = '\0';
		strcat(DecodedString, sz);
	}
}

