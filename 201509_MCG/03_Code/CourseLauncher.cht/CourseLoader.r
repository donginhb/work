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
CAPTION "关于课程出版工具"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "课程出版工具 版本 3.6.2",IDC_STATIC,40,10,119,8,
                    SS_NOPREFIX
    LTEXT           "版权 (C) 2005",IDC_STATIC,40,25,119,8
    DEFPUSHBUTTON   "OK",IDOK,178,7,50,14,WS_GROUP
    ICON            IDR_MAINFRAME,IDC_STATIC,7,7,20,20
END

IDD_COURSELOADER_DIALOG DIALOGEX 0, 0, 383, 320
STYLE DS_MODALFRAME | DS_3DLOOK | WS_POPUP | WS_VISIBLE | WS_CAPTION | 
    WS_SYSMENU
EXSTYLE WS_EX_APPWINDOW
CAPTION "课程出版工具"
MENU IDR_MAIN
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    LISTBOX         IDC_LIST_COURSE,7,25,180,79,LBS_OWNERDRAWFIXED | 
                    LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | 
                    WS_TABSTOP
    PUSHBUTTON      "输入 SCORM 资讯",IDC_BUTTON_INPUT_INFO,7,110,76,15
    PUSHBUTTON      "全部选取",IDC_BUTTON_SELECT_ALL,93,110,45,15
    PUSHBUTTON      "全不选取",IDC_BUTTON_DESELECT_ALL,141,110,45,15
    PUSHBUTTON      "设定出版类别",IDC_BUTTON_SET_TYPE,307,90,69,15
    CONTROL         "出版 SCORM 1.2.7 标准 课程至右列资料夹，以供平台介面汇入:",
                    IDC_RADIO_SCORM,"Button",BS_AUTORADIOBUTTON | WS_GROUP,
                    16,146,205,13
    CONTROL         "出版 非SCORM标准 课程:",IDC_RADIO_NO_SCORM,"Button",
                    BS_AUTORADIOBUTTON,16,193,202,13
    CONTROL         "将媒体档案直接出版至 FTP伺服器",IDC_RADIO_SCORM_FTP,
                    "Button",BS_AUTORADIOBUTTON | WS_GROUP,31,161,202,13
    CONTROL         "将媒体档案出版至上列资料夹，供日后以手动汇入伺服器:",
                    IDC_RADIO_SCORM_LOCAL,"Button",BS_AUTORADIOBUTTON,31,175,
                    191,13
    CONTROL         "出版至预先设置的伺服器，提供使用者直接于网路上浏览课程",
                    IDC_RADIO_FTP,"Button",BS_AUTORADIOBUTTON | WS_GROUP,31,
                    206,287,13
    CONTROL         "出版课程至右列资料夹，供直接浏览 (毋需伺服器支援):",
                    IDC_RADIO_FTP_LOCAL,"Button",BS_AUTORADIOBUTTON,30,221,
                    186,13
    CONTROL         "出版课程至右列资料夹，供日后以手动汇入伺服器:",
                    IDC_RADIO_FTP_MANUAL,"Button",BS_AUTORADIOBUTTON,30,237,
                    182,13
    EDITTEXT        IDC_EDIT_PATH_SCORM,222,147,145,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_PATH_DIRECT,222,219,145,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_PATH_LOAD_LATER,222,236,145,12,ES_AUTOHSCROLL
    PUSHBUTTON      "开始出版",IDC_LOAD,255,298,57,15
    PUSHBUTTON      "关闭",IDCANCEL,319,298,57,15
    PUSHBUTTON      "Button1",IDOK,0,128,6,7,NOT WS_VISIBLE
    LTEXT           "出版课程:",IDC_STATIC,7,11,47,9
    LTEXT           "出版类别:",IDC_STATIC,192,11,47,9
    CONTROL         "",IDC_STATIC_GRID,"Static",SS_BLACKRECT | NOT 
                    WS_VISIBLE,196,25,180,62
    GROUPBOX        "出版目的地",IDC_STATIC,7,133,369,124
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
CAPTION "媒体伺服器"
FONT 9, "Arial"
BEGIN
    RTEXT           "FTP伺服器:",IDC_STATIC,22,23,53,11
    RTEXT           "登入帐号:",IDC_STATIC,22,40,53,11
    RTEXT           "登入密码:",IDC_STATIC,22,57,53,11
    EDITTEXT        IDC_EDIT_MSRV_IP,83,21,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_MSRV_LOGIN_NAME,83,38,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_MSRV_PASSWORD,83,55,132,13,ES_PASSWORD | 
                    ES_AUTOHSCROLL
    RTEXT           "子路径:",IDC_STATIC,22,74,53,11
    EDITTEXT        IDC_EDIT_MSRV_DIRECTORY,83,72,132,13,ES_AUTOHSCROLL
    GROUPBOX        "FTP",IDC_STATIC,8,8,219,103
    LTEXT           "影片档位置:",IDC_STATIC,8,122,61,10
    EDITTEXT        IDC_EDIT_MSRV_ALIAS,104,120,123,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_MMS,73,120,27,12,ES_RIGHT | ES_READONLY
    RTEXT           "连接埠:",IDC_STATIC,15,91,60,11
    EDITTEXT        IDC_EDIT_MSRV_PORT,83,89,132,13,ES_AUTOHSCROLL
END

IDD_DIALOG_WEB_SRV_FTP DIALOG DISCARDABLE  0, 0, 235, 140
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Web伺服器"
FONT 9, "Arial"
BEGIN
    RTEXT           "FTP伺服器:",IDC_STATIC,22,23,53,11
    RTEXT           "登入帐号:",IDC_STATIC,22,40,53,11
    RTEXT           "登入密码:",IDC_STATIC,16,57,59,11
    EDITTEXT        IDC_EDIT_WEBSRV_IP,83,21,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_WEBSRV_LOGIN_NAME,83,38,132,13,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_WEBSRV_PASSWORD,83,55,132,13,ES_PASSWORD | 
                    ES_AUTOHSCROLL
    RTEXT           "子路径:",IDC_STATIC,22,74,53,11
    EDITTEXT        IDC_EDIT_WEBSRV_DIRECTORY,83,72,132,13,ES_AUTOHSCROLL
    GROUPBOX        "FTP",IDC_STATIC,8,8,220,102
    LTEXT           "课程URL:",IDC_STATIC,8,119,49,10
    EDITTEXT        IDC_EDIT_COURSE_URL,58,118,104,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_COURSENAME,166,118,62,12,ES_READONLY
    RTEXT           "连接埠:",IDC_STATIC,15,91,60,11
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
    PUSHBUTTON      "取消",IDCANCEL,129,33,48,14
END

IDD_DIALOG_SQL DIALOGEX 0, 0, 248, 188
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Dialog"
FONT 10, "System", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,191,167,50,14
    EDITTEXT        IDC_EDITSQL,7,7,234,147,ES_MULTILINE | ES_WANTRETURN | 
                    WS_VSCROLL,0,HIDC_EDITSQL
END

IDD_DIALOG_TRANS_COURSE DIALOG DISCARDABLE  0, 0, 255, 53
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "传送未传送完成之课程"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,200,31,48,14
    PUSHBUTTON      "确定",IDOK,147,31,48,14
    LTEXT           "欲传送之课程:",IDC_STATIC,7,10,67,13
    COMBOBOX        IDC_COMBO_COURSE,79,8,170,93,CBS_DROPDOWNLIST | CBS_SORT | 
                    WS_VSCROLL | WS_TABSTOP
END

IDD_DIALOG_ASK_COURSE DIALOGEX 0, 0, 209, 116
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "FTP伺服器存在同名之课程"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,154,94,48,14
    RTEXT           "出版课程",ID_STATIC,7,9,37,11,0,0,HID_STATIC
    PUSHBUTTON      "确定",IDOK,97,94,48,14
    CONTROL         "覆写已存在之课程",IDC_RADIO_OVERWRITE,"Button",
                    BS_AUTORADIOBUTTON,22,41,180,13
    CONTROL         "以下列名称出版:",IDC_RADIO_NEW_NAME,"Button",
                    BS_AUTORADIOBUTTON,22,58,164,11
    EDITTEXT        IDC_EDIT_NAME,50,72,152,12,ES_AUTOHSCROLL
    LTEXT           "已存在于FTP伺服器上，请输入您的选择:",IDC_STATIC_EXIST,
                    7,23,195,11,0,0,HIDC_STATIC_EXIST
    EDITTEXT        IDC_EDIT_COURSE,51,7,151,12,ES_AUTOHSCROLL | ES_READONLY
END

IDD_DIALOG_OPTION DIALOG DISCARDABLE  0, 0, 187, 122
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "选项设定"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,130,102,50,14
    DEFPUSHBUTTON   "确定",IDOK,73,102,50,14
    RTEXT           "连结文字:",IDC_STATIC,5,29,49,13
    EDITTEXT        IDC_EDIT_LINK,58,27,122,15,ES_AUTOHSCROLL
    CONTROL         "转换荧幕撷取影音档",IDC_CHECK_CONV_DEMO,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,7,66,173,11
    CONTROL         "FTP 使用 PASSIVE 模式传输",IDC_CHECK_PASSIVE,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,7,82,186,11
    RTEXT           "预设公司名称:",IDC_STATIC,5,9,49,13
    EDITTEXT        IDC_EDIT_ORG,58,7,122,15,ES_AUTOHSCROLL
    CONTROL         "预设以 SCORM 1.2 标准出版",IDC_CHECK_DEFAULT_SCORN,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,7,50,173,11
END

IDD_DIALOG_LAUNCH_OK DIALOGEX 0, 0, 178, 58
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "通知"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "关闭",IDCANCEL,102,36,48,14
    PUSHBUTTON      "浏览结果",ID_VIEW_RESULT,31,36,60,14
    LTEXT           "课程已成功出版",IDC_STATIC_EXIST,40,12,130,11,0,0,
                    HIDC_STATIC_EXIST
    ICON            IDI_INFO,IDC_STATIC,7,7,20,20
END

IDD_DIALOG_REGEN_INDEX DIALOG DISCARDABLE  0, 0, 185, 49
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "索引档已存在"
FONT 9, "Arial"
BEGIN
    LTEXT           "远端伺服器上已存在索引档，请按下您的选择:",IDC_STATIC,7,
                    7,171,10
    PUSHBUTTON      "保留原档",IDC_BUTTON_RESERVE,7,28,54,14
    PUSHBUTTON      "直接覆写",IDC_BUTTON_OVERWRITE,65,28,54,14
    PUSHBUTTON      "选取后再覆写",IDC_BUTTON_SELECT,124,28,54,14
END

IDD_DIALOG_TYPE_SELECTING DIALOG DISCARDABLE  0, 0, 199, 142
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "出版类别选择"
FONT 9, "Arial"
BEGIN
    DEFPUSHBUTTON   "确定",IDOK,85,117,50,16
    PUSHBUTTON      "取消",IDCANCEL,142,117,50,16
    LTEXT           "请选择出版类别:",IDC_STATIC,7,7,185,12
    LISTBOX         IDC_LIST_TYPE,7,21,185,83,LBS_OWNERDRAWFIXED | 
                    LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | 
                    WS_TABSTOP
    PUSHBUTTON      "全选",IDC_BUTTON_SELECTALL,7,117,53,16
END

IDD_DIALOG_COURSE_DUP DIALOGEX 0, 0, 209, 116
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "目的资料夹存在同名之课程"
FONT 9, "Arial", 0, 0, 0x1
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,154,94,48,14
    RTEXT           "出版课程",ID_STATIC,7,9,37,11,0,0,HID_STATIC
    PUSHBUTTON      "确定",IDOK,97,94,48,14
    CONTROL         "覆写已存在之课程",IDC_RADIO_OVERWRITE,"Button",
                    BS_AUTORADIOBUTTON,22,41,180,13
    CONTROL         "以下列名称出版:",IDC_RADIO_NEW_NAME,"Button",
                    BS_AUTORADIOBUTTON,22,58,164,11
    EDITTEXT        IDC_EDIT_NAME,50,72,152,12,ES_AUTOHSCROLL
    LTEXT           "已存在于目的资料夹上，请输入您的选择:",IDC_STATIC_EXIST,
                    7,23,195,11,0,0,HIDC_STATIC_EXIST
    EDITTEXT        IDC_EDIT_COURSE,51,7,151,12,ES_AUTOHSCROLL | ES_READONLY
END

IDD_DIALOG_VIEW_SEL DIALOG DISCARDABLE  0, 0, 223, 54
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "课程浏览"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "关闭",IDCANCEL,113,32,48,14
    PUSHBUTTON      "浏览",IDVIEW,54,32,48,14
    LTEXT           "请选择要浏览的课程:",IDC_STATIC,7,7,73,11
    COMBOBOX        IDC_COMBO_COURSE,83,7,133,135,CBS_DROPDOWNLIST | 
                    CBS_SORT | WS_VSCROLL | WS_TABSTOP
END

IDD_DIALOG_TYPE_SET DIALOG DISCARDABLE  0, 0, 319, 270
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "设定出版类别"
FONT 9, "Arial"
BEGIN
    COMBOBOX        IDC_COMBO_TYPE,50,7,185,147,CBS_DROPDOWNLIST | 
                    WS_VSCROLL | WS_TABSTOP
    PUSHBUTTON      "新增",IDC_BUTTON_NEW_TYPE,242,7,31,13
    PUSHBUTTON      "更名",IDC_BUTTON_RENAME_TYPE,279,7,31,13
    EDITTEXT        IDC_EDIT_DEFAULT_PROFILE,76,83,220,12,ES_AUTOHSCROLL | 
                    ES_READONLY
    PUSHBUTTON      "编码更名",IDC_BUTTON_RENAME,21,207,52,14
    PUSHBUTTON      "删除编码",IDC_BUTTON_DELETE,78,207,52,14
    PUSHBUTTON      "关闭",IDCANCEL,262,246,48,14
    RTEXT           "编码方式:",IDC_STATIC_SCREEN_TYPE,23,84,47,11
    CONTROL         "",IDC_STATIC_GRID,"Static",SS_BLACKRECT | NOT 
                    WS_VISIBLE,21,103,276,96
    LTEXT           "类别名称:",IDC_STATIC,8,9,37,11
    CONTROL         "不经转换，使用课程原来的影音档",IDC_RADIO_ORIGINAL,
                    "Button",BS_AUTORADIOBUTTON,8,28,130,9
    CONTROL         "使用以下的设定转换各影音档",IDC_RADIO_CONVERT,"Button",
                    BS_AUTORADIOBUTTON,154,28,131,9
    PUSHBUTTON      "删除类别",IDC_BUTTON_DEL_TYPE,8,246,46,14
    COMBOBOX        IDC_COMBO_CODEC,76,65,220,53,CBS_DROPDOWNLIST | 
                    WS_VSCROLL | WS_TABSTOP
    RTEXT           "声音 Codec:",IDC_STATIC_CODEC,21,67,47,11
    EDITTEXT        IDC_EDIT_SCREEN_PROFILE,76,83,220,12,ES_AUTOHSCROLL | 
                    ES_READONLY
    CONTROL         "Tab1",IDC_TAB,"SysTabControl32",0x0,8,47,303,188
END

IDD_DIALOG_ASK_TYPE DIALOG DISCARDABLE  0, 0, 210, 51
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "输入出版类别"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,155,29,48,14
    PUSHBUTTON      "确定",IDOK,97,29,48,14
    EDITTEXT        IDC_EDIT_TYPE,48,7,155,12,ES_AUTOHSCROLL
    LTEXT           "出版类别:",IDC_STATIC,7,9,38,12
END

IDD_DIALOG_INPUT_PROFILE DIALOG DISCARDABLE  0, 0, 187, 50
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "请输入编码名称"
FONT 9, "Arial"
BEGIN
    DEFPUSHBUTTON   "确定",IDOK,80,29,48,14
    PUSHBUTTON      "取消",IDCANCEL,132,29,48,14
    LTEXT           "编码名称:",IDC_STATIC,7,9,36,11
    EDITTEXT        IDC_EDIT_PROFILE,46,7,134,12,ES_AUTOHSCROLL
END

IDD_DIALOG_INPUT_INFO DIALOG DISCARDABLE  0, 0, 223, 220
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "输入 SCORM 资讯"
FONT 9, "Arial"
BEGIN
    PUSHBUTTON      "取消",IDCANCEL,166,199,50,14
    LISTBOX         IDC_LIST_COURSE,7,19,209,64,LBS_SORT | 
                    LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP
    LTEXT           "课程名称:",IDC_STATIC,7,7,87,10
    GROUPBOX        "SCORM 资讯 (目前不支援中文)",IDC_STATIC,7,91,209,102
    RTEXT           "课程标题:",IDC_STATIC,12,108,38,8
    EDITTEXT        IDC_EDIT_TITLE,56,106,146,12,ES_AUTOHSCROLL
    RTEXT           "课程说明:",IDC_STATIC,12,176,38,8
    RTEXT           "子标题:",IDC_STATIC,12,124,38,8
    EDITTEXT        IDC_EDIT_SUB_TITLE,56,122,146,12,ES_AUTOHSCROLL
    RTEXT           "出版单位:",IDC_STATIC,12,141,38,8
    EDITTEXT        IDC_EDIT_ORG,56,139,146,12,ES_AUTOHSCROLL
    EDITTEXT        IDC_EDIT_DESC,56,174,153,12,ES_AUTOHSCROLL
    RTEXT           "关键字:",IDC_STATIC,12,159,38,8
    EDITTEXT        IDC_EDIT_KEYWORD,56,157,146,12,ES_AUTOHSCROLL
    PUSHBUTTON      "确定",IDOnOK,105,199,50,14
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
            VALUE "CompanyName", "智展科技股份有限公司\0"
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
    POPUP "课程(&C)"
    BEGIN
        MENUITEM "续传课程...",                 ID_TRANSMIT
        MENUITEM "预览最近出版之课程",          ID_VIEW_COURSE
        MENUITEM SEPARATOR
        MENUITEM "离开(&E)",                    ID_EXIT
    END
    POPUP "设定(&S)"
    BEGIN
        MENUITEM "出版类别设定(&P)",            ID_TYPE_SETTING
        MENUITEM "伺服器设定(&S)",              ID_SERVER_SETTING
        MENUITEM "选项设定(&O)",                ID_OPTION
    END
    POPUP "说明(&H)"
    BEGIN
        MENUITEM "关于...(&A)",                 ID_ABOUT
    END
END


/////////////////////////////////////////////////////////////////////////////
//
// String Table
//

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_ABOUTBOX            "&关于课程出版工具..."
    IDS_FAIL_TO_COPY        "复制档案自 <%s> 至 <%s> 失败 "
    IDS_FAIL_TO_CREATE_DIR  "产生资料夹 <%s> 失败"
    IDS_SURE_TO_LAUNCH      "出版课程可能会花费很多时间，确定要继续?"
    IDS_SEL_COURSE_FIRST    "请先选择课程"
    IDS_INVALID_URL         "FTP伺服器之位址 <%s> 不合法!"
    IDS_EXCEPTION           "尝试连结FTP伺服器失败，请先检查网际网路环境是否正常。"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_COURSE_DUP          "您输入的课程已存在，请重新输入。"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_FAIL_TO_CONVERT_ASF "转换档案自 <%s> 至 <%s> 失败"
    IDS_SRV_SETTING_CAPTION "伺服器设定"
    IDS_SET_MEDIA_SRV_ALIAS "在实际出版课程前，请先设定<影音档位置>"
    IDS_APPNAME             "课程出版工具"
    IDS_FAIL_TO_CONNECT_INTERNET 
                            "尝试连结FTP伺服器失败，请先检查网际网路环境是否正常。"
    IDS_FAIL_TO_CHANGE_DIR  "在FTP伺服器上 <%s> 改变目录至 <%s> 失败"
    IDS_FAIL_TO_CREATE_FTP_DIR "在FTP伺服器上 <%s> 产生目录 <%s> 失败"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_ERROR_COM_EASY_UTIL "无法找到 <IEasyUtility>"
    IDS_ERROR_COM_WMX_UTILITY "无法找到 <IWMXUtility>"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_CANCEL              "取消"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_GEN_MEDIA_FILE      "产生课程<%s>影音档"
    IDS_PROCESSING          "处理中..."
    IDS_COPY_FILE           "复制档案 <%s>..."
    IDS_CONVERT_FILE        "转换档案 <%s>..."
    IDS_CONNECT_TO_FTP      "连结至FTP伺服器 <%s>"
    IDS_COPY_FILE_TO_FTP    "传送档案 <%s>"
    IDS_TRANSMIT_FILE       "传送课程 <%s>"
    IDS_FAIL_TO_FIND_FILE   "找不到档案 <%s>"
    IDS_FAIL_TO_WRITE_DB    "Fail to write the entry to DB."
    IDS_CHECK_FREQ_FIRST    "请先检查<视讯频宽>及<声音频宽>。"
    IDS_DESCRIPTION         "说明"
    IDS_VIDEO_FREQ          "视讯频宽"
    IDS_AUDIO_FREQ          "声音频宽"
    IDS_FRAME_RATE          "每秒框数"
    IDS_WEB_FTP_NOT_SET     "Web伺服器尚未设定，请先设定之"
    IDS_MEDIA_FTP_NOT_SET   "媒体伺服器尚未设定，请先设定之"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_FAIL_TO_PUT_FILE    "将档案放至FTP伺服器 <%s> 失败。"
    IDS_SURE_TO_EXIT        "确定要中断处理流程?"
    IDS_TYPE_DESC0          "宽频影音"
    IDS_TYPE_DESC1          "窄频影音"
    IDS_TYPE_DESC2          "仅含声音"
    IDS_NOT_NULL            "资料格 (%d,%d) 不能是空的"
    IDS_COURSE_TRANSMITTED  "课程已传送完成"
    IDS_SELECT_ROW_FIRST    "请选择要插入之位置"
    IDS_DEFAULT_LINK        "连结"
    IDS_NOT_TRANS_ANY_COURSE "您尚未传送任何课程"
    IDS_SET_COURSE_URL_FIRST "请先输入<课程URL>"
    ID_BTN_OPEN_FTP         "选择资料夹"
    ID_BTN_OPEN_FILE        "选择资料夹"
    IDS_INPUT_DEST_FIRST    "请先输入目的资料夹"
    IDS_DEST_NOT_EXIST      "目的资料夹 <%s> 不存在"
    IDS_COURSE_PUBLISHED    "课程出版完毕"
END

STRINGTABLE DISCARDABLE 
BEGIN
    AFX_IDS_APP_TITLE       "课程出版工具"
    IDS_VIDEO_CAPTURE       "一般影音档"
    IDS_SCREEN_CAPTURE      "荧幕撷取档"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_PROFILE_NAME        "编码名称"
    IDS_LABEL_ATTRIB_VALUE  "属性\\输入值"
    IDS_MONO                "单音"
    IDS_UNTITLE             "未命名"
    IDS_STEREO              "立体音"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_VIDEO_BANDWIDTH     "视讯频宽"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_VIDEO_FPS           "视讯框/秒"
    IDS_VIDEO_KEY_PER_SEC   "关键框间隔"
    IDS_VIDEO_QUALITY       "画面品质"
    IDS_AUDIO_BANDWIDTH     "声音频宽"
    IDS_AUDIO_SAMPLE_RATE   "取样率"
    IDS_EMPTY_TYPE          "请输入类别名称"
    IDS_DUP_TYPE            "类别名称已存在，请重新输入。"
    IDS_SURE_DEL            "是否确定删除?"
    IDS_SEL_DEL_FIRST       "请先选取删除列"
    IDS_SYSTEM_DEFAULT_NOT_DEL "本编码为系统预设，无法删除。"
    IDS_SURE_DEL_PROFILE    "您是否确定删除编码名称 <%s>?"
    IDS_SEL_RENAME_FIRST    "请先选取更名列"
    IDS_SYSTEM_DEFAULT_NOT_REN "本编码为系统预设，无法更名。"
    IDS_PROFILE_NAME_NOT_NULL "编码名称不得为空白，请重新输入。"
    IDS_SYSTEM_DEFAULT_NOT_ALLOWED 
                            "系统预设的编码才允许两边以<>标记，请删除两边的<>符号。"
    IDS_PROFILE_NAME_EXIST  "编码名称已经存在，请重新输入。"
END

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_DEFAULT_PROFILE_NOT_FOUND 
                            "原先设定的<一般影音档>编码名称无法在系统序列找到，建议您重新设定。"
    IDS_SCREEN_PROFILE_NOT_FOUND 
                            "原先设定的<荧幕撷取档>编码名称无法在系统序列找到，建议您重新设定。"
    IDS_PROFILE_LOW_BAND    "<AM音质,低画质(56K)>"
    IDS_PROFILE_NO_SOUND    "<仅含声音,近CD音质(56K)>"
    IDS_COURSE_MODIFIED     "课程<%s>影音档已被修改过但尚未储存，请回到主程式储存后再出版。"
    IDS_ERROR_CONVERT       "档案转换失败, 课程无法成功出版! 请确认转换频宽是否正确 (如课程仅含声音, 却转换视讯)"
    ID_BTN_OPEN_SCORM       "开启 SCORM 目录"
    IDS_SCORM_LANGUAGE      "zh-tw"
    IDS_NOT_ALL_FILLED      "未填完所有必要栏位，请更正后再试。"
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

