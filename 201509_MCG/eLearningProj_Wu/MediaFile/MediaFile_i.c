/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed May 30 10:27:50 2012
 */
/* Compiler settings for C:\MCG5.0\eLearningProj_Wu\MediaFile\MediaFile.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IMediaConvert = {0xFD1C87EB,0x98A9,0x4D23,{0xA4,0xCE,0x20,0x3D,0x9D,0x83,0xFB,0xF2}};


const IID LIBID_MEDIAFILELib = {0x79C30566,0x4A72,0x4F6B,{0xAC,0x0D,0x17,0x85,0x8B,0xBB,0xCE,0x16}};


const CLSID CLSID_MediaConvert = {0x5C366CA7,0xC3F5,0x4383,{0x86,0x6B,0xCB,0x8E,0xCC,0xAF,0xE5,0xC3}};


#ifdef __cplusplus
}
#endif

