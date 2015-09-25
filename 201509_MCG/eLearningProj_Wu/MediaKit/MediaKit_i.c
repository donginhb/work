/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed May 30 10:28:26 2012
 */
/* Compiler settings for C:\MCG5.0\eLearningProj_Wu\MediaKit\MediaKit.idl:
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

const IID IID_IWMXUtility = {0xB86F4B76,0xBE82,0x4952,{0xA2,0x6B,0x91,0xF8,0x38,0x66,0xAD,0x4C}};


const IID LIBID_MEDIAKITLib = {0x35069362,0xA436,0x41EB,{0xA9,0x54,0xA9,0x0B,0xF9,0x0F,0xB1,0x90}};


const CLSID CLSID_WMXUtility = {0x3FD8AA90,0x1BB1,0x4FCB,{0x90,0xA3,0x0B,0xB1,0x1A,0xB9,0xDA,0x9A}};


#ifdef __cplusplus
}
#endif

