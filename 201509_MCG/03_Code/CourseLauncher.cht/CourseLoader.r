//Microsoft Developer Studio generated resource script.
//
#include "resource.h"

// Generated Help ID header file
#define APSTUDIO_HIDDEN_SYMBOLS
#include "resource.hm"
#undef APSTUDIO_HIDDEN_SYMBOLS

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// Chinese (P.R.C.) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
#ifdef _WIN32
LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED
#pragma code_page(936)
#endif //_WIN32

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE DISCARDABLE 
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE DISCARDABLE 
BEGIN
    "#include ""afxres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE DISCARDABLE 
BEGIN
    "#define _AFX_NO_SPLITTER_RESOURCES\r\n"
    "#define _AFX_NO_OLE_RESOURCES\r\n"
    "#define _AFX_NO_TRACKER_RESOURCES\r\n"
    "#define _AFX_NO_PROPERTY_RESOURCES\r\n"
    "\r\n"
    "#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)\r\n"
    "#ifdef _WIN32\r\n"
    "LANGUAGE 9, 1\r\n"
    "#pragma code_page(936)\r\n"
    "#endif //_WIN32\r\n"
    "#include ""res\\CourseLoader.rc2""  // non-Microsoft Visual C++ edited resources\r\n"
    "#include ""afxres.rc""         // Standard components\r\n"
    "#endif\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDR_MAINFRAME           ICON    DISCARDABLE     "res\\CourseLoader.ico"
IDI_INFO                ICON    DISCARDABLE     "res\\icon1.ico"

/////////////////////////////////////////////////////////////////////////////
//
// Bitmap
//

IDB_TOOLBAR_HOT         BITMAP  DISCARDABLE     "res\\hottoolbar.bmp"
IDB_TOOLBAR_COLD        BITMAP  DISCARDABLE     "res\\coldtoolbar.bmp"
IDB_UNKNOWN             BITMAP  DISCARDABLE     "RES\\UNKNOWN.BMP"
IDB_NOTE_DIS            BITMAP  DISCARDABLE     "RES\\NOTE_DIS.BMP"
IDB_NOTE_EN             BITMAP  DISCARDABLE     "RES\\NOTE_EN.BMP"
IDB_OPEN                BITMAP  DISCARDABLE     "res\\Open.bmp"
IDB_CHECK               BITMAP  DISCARDABLE     "res\\unknown1.bmp"
IDB_UNCHECK             BITMAP  DISCARDABLE     "res\\check1.bmp"

/////////////////////////////////////////////////////////////////////////////
//
// Cursor
//

IDC_CURSOR_DRAG_DROP    CURSOR  DISCARDABLE     "RES\\CURSOR1.CUR"
IDC_CURSOR_DRAG_SIDE    CURSOR  DISCARDABLE     "RES\\CURSOR_D.CUR"
IDC_HAND                CURSOR  DISCARDABLE     "res\\hand.cur"

/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//

IDD_ABOUTBOX DIALOG DISCARDABLE  0, 0, 235, 55
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "���ڿγ̳��湤��"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "�γ̳��湤�� �汾 3.6.2",IDC_STATIC,40,10,119,8,
                    SS_NOPREFIX
    LTEXT           "��Ȩ (C) 2005",IDC_STATIC,40,25,119,8
    DEFPUSHBUTTON   "OK",IDOK,178,7,50,14,WS_GROUP
    ICON            IDR_MAINFRAME,IDC_STATIC,7,7,20,20
END

IDD_COURSELOADER_DIALOG DIALOGEX 0, 0, 383, 320
STYLE DS_MODALFRAME | DS_3DLOOK | WS_POPUP | WS_VISIBLE | WS_CAPTION | 
    WS_SYSMENU
EXSTYLE WS_EX_APPWINDOW
CAPTION "�γ̳��湤��"
MENU IDR_MAIN
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    LISTBOX         IDC_LIST_COURSE,7,25,180,79,LBS_OWNERDRAWFIXED | 
                    LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | 
                    WS_TABSTOP
    PUSHBUTTON      "���� SCORM ��Ѷ",IDC_BUTTON_INPUT_INFO,7,110,76,15
    PUSHBUTTON      "ȫ��ѡȡ",IDC_BUTTON_SELECT_ALL,93,110,45,15
    PUSHBUTTON      "ȫ��ѡȡ",IDC_BUTTON_DESELECT_ALL,141,110,45,15
    PUSHBUTTON      "�趨�������",IDC_BUTTON_SET_TYPE,307,90,69,15
    CONTROL         "���� SCORM 1.2.7 ��׼ �γ����������ϼУ��Թ�ƽ̨�������:",
                    IDC_RADIO_SCORM,"Button",BS_AUTORADIOBUTTON | WS_GROUP,
                    16,146,205,13
    CONTROL         "���� ��SCORM��׼ �γ�:",IDC_RADIO_NO_SCORM,"Button",
                    BS_AUTORADIOBUTTON,16,193,202,13
    CONTROL         "��ý�嵵��ֱ�ӳ����� FTP�ŷ���",IDC_RADIO_SCORM_FTP,
                    "Button",BS_AUTORADIOBUTTON | WS_GROUP,31,161,202,13
    CONTROL         "��ý�嵵���������������ϼУ����պ����ֶ������ŷ���:",
                    IDC_RADIO_SCORM_LOCAL,"Button",BS_AUTORADIOBUTTON,31,175,
                    191,13
    CONTROL         "������Ԥ�����õ��ŷ������ṩʹ����ֱ������·������γ�",
                    IDC_RADIO_FTP,"Button",BS_AUTORADIOBUTTON | WS_GROUP,31,
                    206,287,13
    CONTROL         "����γ����������ϼУ���ֱ����� (�����ŷ���֧Ԯ):",
                    IDC_RADIO_FTP_LOCAL,"Button",BS_AUTORADIOBUTTON,30,221,
                    186,13
    CONTROL         "����γ����������ϼУ����պ����ֶ������ŷ���:",
                    IDC_RADIO_FTP_MANUAL,"Button",BS_AUTORADIOBUTTON,30,237,
                    182,13
    EDITTEXT        IDC_EDIT_PATH_SCORM,222,147,145,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_PATH_DIRECT,222,219,145,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_PATH_LOAD_LATER,222,236,145,12,ES_AUTOHSCROLL
    PUSHBUTTON      "��ʼ����",IDC_LOAD,255,298,57,15
    PUSHBUTTON      "�ر�",IDCANCEL,319,298,57,15
    PUSHBUTTON      "Button1",IDOK,0,128,6,7,NOT WS_VISIBLE
    LTEXT           "����γ�:",IDC_STATIC,7,11,47,9
    LTEXT           "�������:",IDC_STATIC,192,11,47,9
    CONTROL         "",IDC_STATIC_GRID,"Static",SS_BLACKRECT | NOT 
                    WS_VISIBLE,196,25,180,62
    GROUPBOX        "����Ŀ�ĵ�",IDC_STATIC,7,133,369,124
    CONTROL         "Progress1",IDC_PROGRESS1,"msctls_progress32",PBS_SMOOTH | 
                    NOT WS_VISIBLE | WS_BORDER,7,298,233,15
    GROUPBOX        "LMS",IDC_STATIC,7,260,369,35
    CONTROL         "aEnrich LMS",IDC_RAD_AENRICH,"Button",
                    BS_AUTORADIOBUTTON,46,281,55,10
    CONTROL         "Default",IDC_RAD_DEFAULT,"Button",BS_AUTORADIOBUTTON,46,
                    270,38,10
    CONTROL         "iLearning LMS v 5.x",IDC_RAD_ORACLE,"Button",
                    BS_AUTORADIOBUTTON,268,270,76,10
    CONTROL         "iLearning LMS Old",IDC_RAD_ORACLEOLD,"Button",
                    BS_AUTORADIOBUTTON,268,281,74,10
    CONTROL         "Enhance SCORM Mode",IDC_CHECK_ENHANCE,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,111,275,86,10
    GROUPBOX        "",IDC_STATIC,36,264,168,29
    GROUPBOX        "",IDC_STATIC,258,264,93,28
    GROUPBOX        "ZIP Level",IDC_STATIC,196,106,180,19
    CONTROL         "Best Speed",IDC_RAD_ZIP1,"Button",BS_AUTORADIOBUTTON | 
                    WS_GROUP,198,114,53,10
    CONTROL         "Default",IDC_RAD_ZIP6,"Button",BS_AUTORADIOBUTTON,259,
                    114,38,10
    CONTROL         "Best Compression",IDC_RAD_ZIP9,"Button",
                    BS_AUTORADIOBUTTON,304,114,72,10
END

IDD_DIALOG_MEDIA_SRV_FTP DIALOG DISCARDABLE  0, 0, 235, 140
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "ý���ŷ���"
FONT 9, "Arial"
BEGIN
    RTEXT           "FTP�ŷ���:",IDC_STATIC,22,23,53,11
    RTEXT           "�����ʺ�:",IDC_STATIC,22,40,53,11
    RTEXT           "��������:",IDC_STATIC,22,57,53,11
    EDITTEXT        IDC_EDIT_MSRV_IP,83,21,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_MSRV_LOGIN_NAME,83,38,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_MSRV_PASSWORD,83,55,132,13,ES_PASSWORD | 
                    ES_AUTOHSCROLL
    RTEXT           "��·��:",IDC_STATIC,22,74,53,11
    EDITTEXT        IDC_EDIT_MSRV_DIRECTORY,83,72,132,13,ES_AUTOHSCROLL
    GROUPBOX        "FTP",IDC_STATIC,8,8,219,103
    LTEXT           "ӰƬ��λ��:",IDC_STATIC,8,122,61,10
    EDITTEXT        IDC_EDIT_MSRV_ALIAS,104,120,123,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_MMS,73,120,27,12,ES_RIGHT | ES_READONLY
    RTEXT           "���Ӳ�:",IDC_STATIC,15,91,60,11
    EDITTEXT        IDC_EDIT_MSRV_PORT,83,89,132,13,ES_AUTOHSCROLL
END

IDD_DIALOG_WEB_SRV_FTP DIALOG DISCARDABLE  0, 0, 235, 140
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Web�ŷ���"
FONT 9, "Arial"
BEGIN
    RTEXT           "FTP�ŷ���:",IDC_STATIC,22,23,53,11
    RTEXT           "�����ʺ�:",IDC_STATIC,22,40,53,11
    RTEXT           "��������:",IDC_STATIC,16,57,59,11
    EDITTEXT        IDC_EDIT_WEBSRV_IP,83,21,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_WEBSRV_LOGIN_NAME,83,38,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_WEBSRV_PASSWORD,83,55,132,13,ES_PASSWORD | 
                    ES_AUTOHSCROLL
    RTEXT           "��·��:",IDC_STATIC,22,74,53,11
    EDITTEXT        IDC_EDIT_WEBSRV_DIRECTORY,83,72,132,13,ES_AUTOHSCROLL
    GROUPBOX        "FTP",IDC_STATIC,8,8,220,102
    LTEXT           "�γ�URL:",IDC_STATIC,8,119,49,10
    EDITTEXT        IDC_EDIT_COURSE_URL,58,118,104,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_COURSENAME,166,118,62,12,ES_READONLY
    RTEXT           "���Ӳ�:",IDC_STATIC,15,91,60,11
    EDITTEXT        IDC_EDIT_WEBSRV_PORT,83,89,132,13,ES_AUTOHSCROLL
END

IDD_DIALOG_PROGRESS DIALOGEX 0, 0, 269, 54
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION
CAPTION "Dialog"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    CONTROL         "Animate1",IDC_ANIMATE,"SysAnimate32",ACS_CENTER | 
                    ACS_AUTOPLAY | WS_TABSTOP,7,7,32,40,WS_EX_CLIENTEDGE
    CTEXT           "Static",IDC_STATIC_MSG,45,13,217,10
    PUSHBUTTON      "ȡ��",IDCANCEL,129,33,48,14
END

IDD_DIALOG_SQL DIALOGEX 0, 0, 248, 188
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Dialog"
FONT 10, "System", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,191,167,50,14
    EDITTEXT        IDC_EDITSQL,7,7,234,147,ES_MULTILINE | ES_WANTRETURN | 
                    WS_VSCROLL,0,HIDC_EDITSQL
END

IDD_DIALOG_TRANS_COURSE DIALOG DISCARDABLE  0, 0, 255, 53
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "����δ�������֮�γ�"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,200,31,48,14
    PUSHBUTTON      "ȷ��",IDOK,147,31,48,14
    LTEXT           "������֮�γ�:",IDC_STATIC,7,10,67,13
    COMBOBOX        IDC_COMBO_COURSE,79,8,170,93,CBS_DROPDOWNLIST | CBS_SORT | 
                    WS_VSCROLL | WS_TABSTOP
END

IDD_DIALOG_ASK_COURSE DIALOGEX 0, 0, 209, 116
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "FTP�ŷ�������ͬ��֮�γ�"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,154,94,48,14
    RTEXT           "����γ�",ID_STATIC,7,9,37,11,0,0,HID_STATIC
    PUSHBUTTON      "ȷ��",IDOK,97,94,48,14
    CONTROL         "��д�Ѵ���֮�γ�",IDC_RADIO_OVERWRITE,"Button",
                    BS_AUTORADIOBUTTON,22,41,180,13
    CONTROL         "���������Ƴ���:",IDC_RADIO_NEW_NAME,"Button",
                    BS_AUTORADIOBUTTON,22,58,164,11
    EDITTEXT        IDC_EDIT_NAME,50,72,152,12,ES_AUTOHSCROLL
    LTEXT           "�Ѵ�����FTP�ŷ����ϣ�����������ѡ��:",IDC_STATIC_EXIST,
                    7,23,195,11,0,0,HIDC_STATIC_EXIST
    EDITTEXT        IDC_EDIT_COURSE,51,7,151,12,ES_AUTOHSCROLL | ES_READONLY
END

IDD_DIALOG_OPTION DIALOG DISCARDABLE  0, 0, 187, 122
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "ѡ���趨"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,130,102,50,14
    DEFPUSHBUTTON   "ȷ��",IDOK,73,102,50,14
    RTEXT           "��������:",IDC_STATIC,5,29,49,13
    EDITTEXT        IDC_EDIT_LINK,58,27,122,15,ES_AUTOHSCROLL
    CONTROL         "ת��ӫĻߢȡӰ����",IDC_CHECK_CONV_DEMO,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,7,66,173,11
    CONTROL         "FTP ʹ�� PASSIVE ģʽ����",IDC_CHECK_PASSIVE,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,7,82,186,11
    RTEXT           "Ԥ�蹫˾����:",IDC_STATIC,5,9,49,13
    EDITTEXT        IDC_EDIT_ORG,58,7,122,15,ES_AUTOHSCROLL
    CONTROL         "Ԥ���� SCORM 1.2 ��׼����",IDC_CHECK_DEFAULT_SCORN,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,7,50,173,11
END

IDD_DIALOG_LAUNCH_OK DIALOGEX 0, 0, 178, 58
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "֪ͨ"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "�ر�",IDCANCEL,102,36,48,14
    PUSHBUTTON      "������",ID_VIEW_RESULT,31,36,60,14
    LTEXT           "�γ��ѳɹ�����",IDC_STATIC_EXIST,40,12,130,11,0,0,
                    HIDC_STATIC_EXIST
    ICON            IDI_INFO,IDC_STATIC,7,7,20,20
END

IDD_DIALOG_REGEN_INDEX DIALOG DISCARDABLE  0, 0, 185, 49
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "�������Ѵ���"
FONT 9, "Arial"
BEGIN
    LTEXT           "Զ���ŷ������Ѵ������������밴������ѡ��:",IDC_STATIC,7,
                    7,171,10
    PUSHBUTTON      "����ԭ��",IDC_BUTTON_RESERVE,7,28,54,14
    PUSHBUTTON      "ֱ�Ӹ�д",IDC_BUTTON_OVERWRITE,65,28,54,14
    PUSHBUTTON      "ѡȡ���ٸ�д",IDC_BUTTON_SELECT,124,28,54,14
END

IDD_DIALOG_TYPE_SELECTING DIALOG DISCARDABLE  0, 0, 199, 142
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "�������ѡ��"
FONT 9, "Arial"
BEGIN
    DEFPUSHBUTTON   "ȷ��",IDOK,85,117,50,16
    PUSHBUTTON      "ȡ��",IDCANCEL,142,117,50,16
    LTEXT           "��ѡ��������:",IDC_STATIC,7,7,185,12
    LISTBOX         IDC_LIST_TYPE,7,21,185,83,LBS_OWNERDRAWFIXED | 
                    LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | 
                    WS_TABSTOP
    PUSHBUTTON      "ȫѡ",IDC_BUTTON_SELECTALL,7,117,53,16
END

IDD_DIALOG_COURSE_DUP DIALOGEX 0, 0, 209, 116
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Ŀ�����ϼд���ͬ��֮�γ�"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,154,94,48,14
    RTEXT           "����γ�",ID_STATIC,7,9,37,11,0,0,HID_STATIC
    PUSHBUTTON      "ȷ��",IDOK,97,94,48,14
    CONTROL         "��д�Ѵ���֮�γ�",IDC_RADIO_OVERWRITE,"Button",
                    BS_AUTORADIOBUTTON,22,41,180,13
    CONTROL         "���������Ƴ���:",IDC_RADIO_NEW_NAME,"Button",
                    BS_AUTORADIOBUTTON,22,58,164,11
    EDITTEXT        IDC_EDIT_NAME,50,72,152,12,ES_AUTOHSCROLL
    LTEXT           "�Ѵ�����Ŀ�����ϼ��ϣ�����������ѡ��:",IDC_STATIC_EXIST,
                    7,23,195,11,0,0,HIDC_STATIC_EXIST
    EDITTEXT        IDC_EDIT_COURSE,51,7,151,12,ES_AUTOHSCROLL | ES_READONLY
END

IDD_DIALOG_VIEW_SEL DIALOG DISCARDABLE  0, 0, 223, 54
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "�γ����"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "�ر�",IDCANCEL,113,32,48,14
    PUSHBUTTON      "���",IDVIEW,54,32,48,14
    LTEXT           "��ѡ��Ҫ����Ŀγ�:",IDC_STATIC,7,7,73,11
    COMBOBOX        IDC_COMBO_COURSE,83,7,133,135,CBS_DROPDOWNLIST | 
                    CBS_SORT | WS_VSCROLL | WS_TABSTOP
END

IDD_DIALOG_TYPE_SET DIALOG DISCARDABLE  0, 0, 319, 270
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "�趨�������"
FONT 9, "Arial"
BEGIN
    COMBOBOX        IDC_COMBO_TYPE,50,7,185,147,CBS_DROPDOWNLIST | 
                    WS_VSCROLL | WS_TABSTOP
    PUSHBUTTON      "����",IDC_BUTTON_NEW_TYPE,242,7,31,13
    PUSHBUTTON      "����",IDC_BUTTON_RENAME_TYPE,279,7,31,13
    EDITTEXT        IDC_EDIT_DEFAULT_PROFILE,76,83,220,12,ES_AUTOHSCROLL | 
                    ES_READONLY
    PUSHBUTTON      "�������",IDC_BUTTON_RENAME,21,207,52,14
    PUSHBUTTON      "ɾ������",IDC_BUTTON_DELETE,78,207,52,14
    PUSHBUTTON      "�ر�",IDCANCEL,262,246,48,14
    RTEXT           "���뷽ʽ:",IDC_STATIC_SCREEN_TYPE,23,84,47,11
    CONTROL         "",IDC_STATIC_GRID,"Static",SS_BLACKRECT | NOT 
                    WS_VISIBLE,21,103,276,96
    LTEXT           "�������:",IDC_STATIC,8,9,37,11
    CONTROL         "����ת����ʹ�ÿγ�ԭ����Ӱ����",IDC_RADIO_ORIGINAL,
                    "Button",BS_AUTORADIOBUTTON,8,28,130,9
    CONTROL         "ʹ�����µ��趨ת����Ӱ����",IDC_RADIO_CONVERT,"Button",
                    BS_AUTORADIOBUTTON,154,28,131,9
    PUSHBUTTON      "ɾ�����",IDC_BUTTON_DEL_TYPE,8,246,46,14
    COMBOBOX        IDC_COMBO_CODEC,76,65,220,53,CBS_DROPDOWNLIST | 
                    WS_VSCROLL | WS_TABSTOP
    RTEXT           "���� Codec:",IDC_STATIC_CODEC,21,67,47,11
    EDITTEXT        IDC_EDIT_SCREEN_PROFILE,76,83,220,12,ES_AUTOHSCROLL | 
                    ES_READONLY
    CONTROL         "Tab1",IDC_TAB,"SysTabControl32",0x0,8,47,303,188
END

IDD_DIALOG_ASK_TYPE DIALOG DISCARDABLE  0, 0, 210, 51
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "����������"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,155,29,48,14
    PUSHBUTTON      "ȷ��",IDOK,97,29,48,14
    EDITTEXT        IDC_EDIT_TYPE,48,7,155,12,ES_AUTOHSCROLL
    LTEXT           "�������:",IDC_STATIC,7,9,38,12
END

IDD_DIALOG_INPUT_PROFILE DIALOG DISCARDABLE  0, 0, 187, 50
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "�������������"
FONT 9, "Arial"
BEGIN
    DEFPUSHBUTTON   "ȷ��",IDOK,80,29,48,14
    PUSHBUTTON      "ȡ��",IDCANCEL,132,29,48,14
    LTEXT           "��������:",IDC_STATIC,7,9,36,11
    EDITTEXT        IDC_EDIT_PROFILE,46,7,134,12,ES_AUTOHSCROLL
END

IDD_DIALOG_INPUT_INFO DIALOG DISCARDABLE  0, 0, 223, 220
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "���� SCORM ��Ѷ"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "ȡ��",IDCANCEL,166,199,50,14
    LISTBOX         IDC_LIST_COURSE,7,19,209,64,LBS_SORT | 
                    LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP
    LTEXT           "�γ�����:",IDC_STATIC,7,7,87,10
    GROUPBOX        "SCORM ��Ѷ (Ŀǰ��֧Ԯ����)",IDC_STATIC,7,91,209,102
    RTEXT           "�γ̱���:",IDC_STATIC,12,108,38,8
    EDITTEXT        IDC_EDIT_TITLE,56,106,146,12,ES_AUTOHSCROLL
    RTEXT           "�γ�˵��:",IDC_STATIC,12,176,38,8
    RTEXT           "�ӱ���:",IDC_STATIC,12,124,38,8
    EDITTEXT        IDC_EDIT_SUB_TITLE,56,122,146,12,ES_AUTOHSCROLL
    RTEXT           "���浥λ:",IDC_STATIC,12,141,38,8
    EDITTEXT        IDC_EDIT_ORG,56,139,146,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_DESC,56,174,153,12,ES_AUTOHSCROLL
    RTEXT           "�ؼ���:",IDC_STATIC,12,159,38,8
    EDITTEXT        IDC_EDIT_KEYWORD,56,157,146,12,ES_AUTOHSCROLL
    PUSHBUTTON      "ȷ��",IDOnOK,105,199,50,14
    CTEXT           "*",IDC_STATIC,202,161,8,8,SS_CENTERIMAGE
    CTEXT           "*",IDC_STATIC,202,144,8,8,SS_CENTERIMAGE
    CTEXT           "*",IDC_STATIC,202,110,8,8,SS_CENTERIMAGE
    CTEXT           "*",IDC_STATIC,202,127,8,8,SS_CENTERIMAGE
END


#ifndef _MAC
/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION 3,6,3,60207
 PRODUCTVERSION 3,6,3,0
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x29L
#else
 FILEFLAGS 0x28L
#endif
 FILEOS 0x4L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040404b0"
        BEGIN
            VALUE "Comments", "\0"
            VALUE "CompanyName", "��չ�Ƽ��ɷ����޹�˾\0"
            VALUE "FileDescription", "CourseLauncher\0"
            VALUE "FileVersion", "3, 6, 3, 60207\0"
            VALUE "InternalName", "CourseLauncher\0"
            VALUE "LegalCopyright", "Copyright (C) 2005\0"
            VALUE "LegalTrademarks", "\0"
            VALUE "OriginalFilename", "CourseLauncher.exe\0"
            VALUE "PrivateBuild", "06020700\0"
            VALUE "ProductName", "CourseLauncher\0"
            VALUE "ProductVersion", "3, 6, 3, 0\0"
            VALUE "SpecialBuild", "06020700\0"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x404, 1200
    END
END

#endif    // !_MAC


/////////////////////////////////////////////////////////////////////////////
//
// DESIGNINFO
//

#ifdef APSTUDIO_INVOKED
GUIDELINES DESIGNINFO DISCARDABLE 
BEGIN
    IDD_ABOUTBOX, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 228
        TOPMARGIN, 7
        BOTTOMMARGIN, 48
    END

    IDD_COURSELOADER_DIALOG, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 376
        TOPMARGIN, 11
        BOTTOMMARGIN, 313
    END

    IDD_DIALOG_MEDIA_SRV_FTP, DIALOG
    BEGIN
        LEFTMARGIN, 8
        RIGHTMARGIN, 227
        TOPMARGIN, 8
        BOTTOMMARGIN, 133
    END

    IDD_DIALOG_WEB_SRV_FTP, DIALOG
    BEGIN
        LEFTMARGIN, 8
        RIGHTMARGIN, 228
        TOPMARGIN, 8
        BOTTOMMARGIN, 133
    END

    IDD_DIALOG_PROGRESS, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 262
        TOPMARGIN, 7
        BOTTOMMARGIN, 47
    END

    IDD_DIALOG_SQL, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 241
        TOPMARGIN, 7
        BOTTOMMARGIN, 181
    END

    IDD_DIALOG_TRANS_COURSE, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 248
        TOPMARGIN, 7
        BOTTOMMARGIN, 45
    END

    IDD_DIALOG_ASK_COURSE, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 202
        TOPMARGIN, 7
        BOTTOMMARGIN, 108
    END

    IDD_DIALOG_OPTION, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 180
        TOPMARGIN, 7
        BOTTOMMARGIN, 116
    END

    IDD_DIALOG_LAUNCH_OK, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 171
        TOPMARGIN, 7
        BOTTOMMARGIN, 50
    END

    IDD_DIALOG_REGEN_INDEX, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 178
        TOPMARGIN, 7
        BOTTOMMARGIN, 42
    END

    IDD_DIALOG_TYPE_SELECTING, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 192
        TOPMARGIN, 7
        BOTTOMMARGIN, 133
    END

    IDD_DIALOG_COURSE_DUP, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 202
        TOPMARGIN, 7
        BOTTOMMARGIN, 108
    END

    IDD_DIALOG_VIEW_SEL, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 216
        TOPMARGIN, 7
        BOTTOMMARGIN, 46
    END

    IDD_DIALOG_TYPE_SET, DIALOG
    BEGIN
        LEFTMARGIN, 8
        RIGHTMARGIN, 310
        TOPMARGIN, 7
        BOTTOMMARGIN, 260
    END

    IDD_DIALOG_ASK_TYPE, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 203
        TOPMARGIN, 7
        BOTTOMMARGIN, 43
    END

    IDD_DIALOG_INPUT_PROFILE, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 180
        TOPMARGIN, 7
        BOTTOMMARGIN, 43
    END

    IDD_DIALOG_INPUT_INFO, DIALOG
    BEGIN
        LEFTMARGIN, 7
        RIGHTMARGIN, 216
        TOPMARGIN, 7
        BOTTOMMARGIN, 213
    END
END
#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Menu
//

IDR_MAIN MENU DISCARDABLE 
BEGIN
    POPUP "�γ�(&C)"
    BEGIN
        MENUITEM "�����γ�...",                 ID_TRANSMIT
        MENUITEM "Ԥ���������֮�γ�",          ID_VIEW_COURSE
        MENUITEM SEPARATOR
        MENUITEM "�뿪(&E)",                    ID_EXIT
    END
    POPUP "�趨(&S)"
    BEGIN
        MENUITEM "��������趨(&P)",            ID_TYPE_SETTING
        MENUITEM "�ŷ����趨(&S)",              ID_SERVER_SETTING
        MENUITEM "ѡ���趨(&O)",                ID_OPTION
    END
    POPUP "˵��(&H)"
    BEGIN
        MENUITEM "����...(&A)",                 ID_ABOUT
    END
END


/////////////////////////////////////////////////////////////////////////////
//
// String Table
//

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_ABOUTBOX            "&���ڿγ̳��湤��..."
    IDS_FAIL_TO_COPY        "���Ƶ����� <%s> �� <%s> ʧ�� "
    IDS_FAIL_TO_CREATE_DIR  "�������ϼ� <%s> ʧ��"
    IDS_SURE_TO_LAUNCH      "����γ̿��ܻỨ�Ѻܶ�ʱ�䣬ȷ��Ҫ����?"
    IDS_SEL_COURSE_FIRST    "����ѡ��γ�"
    IDS_INVALID_URL         "FTP�ŷ���֮λַ <%s> ���Ϸ�!"
    IDS_EXCEPTION           "��������FTP�ŷ���ʧ�ܣ����ȼ��������·�����Ƿ�������"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_COURSE_DUP          "������Ŀγ��Ѵ��ڣ����������롣"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_FAIL_TO_CONVERT_ASF "ת�������� <%s> �� <%s> ʧ��"
    IDS_SRV_SETTING_CAPTION "�ŷ����趨"
    IDS_SET_MEDIA_SRV_ALIAS "��ʵ�ʳ���γ�ǰ�������趨<Ӱ����λ��>"
    IDS_APPNAME             "�γ̳��湤��"
    IDS_FAIL_TO_CONNECT_INTERNET 
                            "��������FTP�ŷ���ʧ�ܣ����ȼ��������·�����Ƿ�������"
    IDS_FAIL_TO_CHANGE_DIR  "��FTP�ŷ����� <%s> �ı�Ŀ¼�� <%s> ʧ��"
    IDS_FAIL_TO_CREATE_FTP_DIR "��FTP�ŷ����� <%s> ����Ŀ¼ <%s> ʧ��"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_ERROR_COM_EASY_UTIL "�޷��ҵ� <IEasyUtility>"
    IDS_ERROR_COM_WMX_UTILITY "�޷��ҵ� <IWMXUtility>"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_CANCEL              "ȡ��"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_GEN_MEDIA_FILE      "�����γ�<%s>Ӱ����"
    IDS_PROCESSING          "������..."
    IDS_COPY_FILE           "���Ƶ��� <%s>..."
    IDS_CONVERT_FILE        "ת������ <%s>..."
    IDS_CONNECT_TO_FTP      "������FTP�ŷ��� <%s>"
    IDS_COPY_FILE_TO_FTP    "���͵��� <%s>"
    IDS_TRANSMIT_FILE       "���Ϳγ� <%s>"
    IDS_FAIL_TO_FIND_FILE   "�Ҳ������� <%s>"
    IDS_FAIL_TO_WRITE_DB    "Fail to write the entry to DB."
    IDS_CHECK_FREQ_FIRST    "���ȼ��<��ѶƵ��>��<����Ƶ��>��"
    IDS_DESCRIPTION         "˵��"
    IDS_VIDEO_FREQ          "��ѶƵ��"
    IDS_AUDIO_FREQ          "����Ƶ��"
    IDS_FRAME_RATE          "ÿ�����"
    IDS_WEB_FTP_NOT_SET     "Web�ŷ�����δ�趨�������趨֮"
    IDS_MEDIA_FTP_NOT_SET   "ý���ŷ�����δ�趨�������趨֮"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_FAIL_TO_PUT_FILE    "����������FTP�ŷ��� <%s> ʧ�ܡ�"
    IDS_SURE_TO_EXIT        "ȷ��Ҫ�жϴ�������?"
    IDS_TYPE_DESC0          "��ƵӰ��"
    IDS_TYPE_DESC1          "խƵӰ��"
    IDS_TYPE_DESC2          "��������"
    IDS_NOT_NULL            "���ϸ� (%d,%d) �����ǿյ�"
    IDS_COURSE_TRANSMITTED  "�γ��Ѵ������"
    IDS_SELECT_ROW_FIRST    "��ѡ��Ҫ����֮λ��"
    IDS_DEFAULT_LINK        "����"
    IDS_NOT_TRANS_ANY_COURSE "����δ�����κογ�"
    IDS_SET_COURSE_URL_FIRST "��������<�γ�URL>"
    ID_BTN_OPEN_FTP         "ѡ�����ϼ�"
    ID_BTN_OPEN_FILE        "ѡ�����ϼ�"
    IDS_INPUT_DEST_FIRST    "��������Ŀ�����ϼ�"
    IDS_DEST_NOT_EXIST      "Ŀ�����ϼ� <%s> ������"
    IDS_COURSE_PUBLISHED    "�γ̳������"
END

STRINGTABLE DISCARDABLE 
BEGIN
    AFX_IDS_APP_TITLE       "�γ̳��湤��"
    IDS_VIDEO_CAPTURE       "һ��Ӱ����"
    IDS_SCREEN_CAPTURE      "ӫĻߢȡ��"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_PROFILE_NAME        "��������"
    IDS_LABEL_ATTRIB_VALUE  "����\\����ֵ"
    IDS_MONO                "����"
    IDS_UNTITLE             "δ����"
    IDS_STEREO              "������"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_VIDEO_BANDWIDTH     "��ѶƵ��"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_VIDEO_FPS           "��Ѷ��/��"
    IDS_VIDEO_KEY_PER_SEC   "�ؼ�����"
    IDS_VIDEO_QUALITY       "����Ʒ��"
    IDS_AUDIO_BANDWIDTH     "����Ƶ��"
    IDS_AUDIO_SAMPLE_RATE   "ȡ����"
    IDS_EMPTY_TYPE          "�������������"
    IDS_DUP_TYPE            "��������Ѵ��ڣ����������롣"
    IDS_SURE_DEL            "�Ƿ�ȷ��ɾ��?"
    IDS_SEL_DEL_FIRST       "����ѡȡɾ����"
    IDS_SYSTEM_DEFAULT_NOT_DEL "������ΪϵͳԤ�裬�޷�ɾ����"
    IDS_SURE_DEL_PROFILE    "���Ƿ�ȷ��ɾ���������� <%s>?"
    IDS_SEL_RENAME_FIRST    "����ѡȡ������"
    IDS_SYSTEM_DEFAULT_NOT_REN "������ΪϵͳԤ�裬�޷�������"
    IDS_PROFILE_NAME_NOT_NULL "�������Ʋ���Ϊ�հף����������롣"
    IDS_SYSTEM_DEFAULT_NOT_ALLOWED 
                            "ϵͳԤ��ı��������������<>��ǣ���ɾ�����ߵ�<>���š�"
    IDS_PROFILE_NAME_EXIST  "���������Ѿ����ڣ����������롣"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_DEFAULT_PROFILE_NOT_FOUND 
                            "ԭ���趨��<һ��Ӱ����>���������޷���ϵͳ�����ҵ��������������趨��"
    IDS_SCREEN_PROFILE_NOT_FOUND 
                            "ԭ���趨��<ӫĻߢȡ��>���������޷���ϵͳ�����ҵ��������������趨��"
    IDS_PROFILE_LOW_BAND    "<AM����,�ͻ���(56K)>"
    IDS_PROFILE_NO_SOUND    "<��������,��CD����(56K)>"
    IDS_COURSE_MODIFIED     "�γ�<%s>Ӱ�����ѱ��޸Ĺ�����δ���棬��ص�����ʽ������ٳ��档"
    IDS_ERROR_CONVERT       "����ת��ʧ��, �γ��޷��ɹ�����! ��ȷ��ת��Ƶ���Ƿ���ȷ (��γ̽�������, ȴת����Ѷ)"
    ID_BTN_OPEN_SCORM       "���� SCORM Ŀ¼"
    IDS_SCORM_LANGUAGE      "zh-tw"
    IDS_NOT_ALL_FILLED      "δ�������б�Ҫ��λ������������ԡ�"
END

#endif    // Chinese (P.R.C.) resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//
#define _AFX_NO_SPLITTER_RESOURCES
#define _AFX_NO_OLE_RESOURCES
#define _AFX_NO_TRACKER_RESOURCES
#define _AFX_NO_PROPERTY_RESOURCES

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
#ifdef _WIN32
LANGUAGE 9, 1
#pragma code_page(936)
#endif //_WIN32
#include "res\CourseLoader.rc2"  // non-Microsoft Visual C++ edited resources
#include "afxres.rc"         // Standard components
#endif

/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED

