// CHash.h : header file
//

#ifndef _CHASH_H
#define _CHASH_H


// Choose which algorithms you want
// Put 1s to support algorithms, else 0 to not support
#define        SUPPORT_CRC32          1
#define        SUPPORT_GOSTHASH       1
#define        SUPPORT_MD5            1
/*
#define        SUPPORT_MD2            1
#define        SUPPORT_MD4            1
#define        SUPPORT_SHA1           1
#define        SUPPORT_SHA2           1
*/
/////////////////////////////////////////////////////////////////////////////
// CHash definitions

class CHash
{
// Construction
public:
	CHash();   // Standard constructor
	CString DoHash();
	int GetHashAlgorithm();
	CString GetHashFile();
	int GetHashFormat();
	int GetHashOperation();
	CString GetHashString();
	int GetSHA2Strength();
	CString GOSTHash();
	CString MD2Hash();
	CString MD4Hash();
	CString MD5Hash();
	CString SHA1Hash();
	CString SHA2Hash();
	void SetHashAlgorithm(int Algo);
	void SetHashOperation(int Style);
	void SetHashFile(LPCSTR File);
	void SetHashFormat(int Style);
	void SetHashString(LPCSTR Hash);
	void SetSHA2Strength(int Strength);
	void SetHashFinalPassword(CString ThePassword);


// Implementation
protected:
	// Input string, algorithm, filename, formatting and such
	CString    hashString;
	int        hashAlgo;
	CString    hashFile;
	CString    m_Password;
	int        hashFormatting;
	int        hashStyle;
	int        hashOperation;
	int        sha2Strength;
	
	// Temporary working CString
	CString    tempHash;
};

// Definitions of some kind
#define        STRING_HASH            1
#define        FILE_HASH              2

#define        SIZE_OF_BUFFER         16000

// Algorithms
#define        CRC32                  1
#define        GOSTHASH               2
#define        MD2                    3
#define        MD4                    4
#define        MD5                    5
#define        SHA1                   6
#define        SHA2                   7

// Formatting options
#define        LOWERCASE_NOSPACES     1
#define        LOWERCASE_SPACES       2
#define        UPPERCASE_NOSPACES     3
#define        UPPERCASE_SPACES       4

#endif
