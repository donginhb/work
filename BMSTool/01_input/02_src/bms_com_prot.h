/****************************************************************************
File name		: bms_com_prot.c
Content 		: the main control of the Communication with controller
Date/Author 	: 2017.05.16	PSDCD@Lifanghseng 
Change Log		: 
Copyright		: Panasonic Industrial Devices Sales( CHINA ) CO., LTD
*****************************************************************************/
#ifndef __BMS_COM_PROT_H
#define __BMS_COM_PROT_H

/****************************************************************************
 define macro
*****************************************************************************/
typedef unsigned char		UCHAR;
typedef unsigned short		USHORT;
typedef unsigned long		ULONG;
typedef char				CHAR;
typedef short				SHORT;
typedef long				LONG;
typedef unsigned char		BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
#define		DBMS_COM_DATA_MAX	(170)						                /* BMS通信データ領域 MAX byte  */
#define		DBMS_COM_FRAME_CTRL_DATA_MAX	(10)						    /* フレーム制御データ byte  */
#define		DBMS_COM_FRAME_MAX	(DBMS_COM_DATA_MAX + DBMS_COM_FRAME_CTRL_DATA_MAX)	
#define		DBMS_CELL_SERIES_MAX 	(16)
#define 	DBMS_CELL_TH_CNT		(5)
#define		DBMS_DEV_INFO			(10)
#define		DBMS_SW_VER_INFO		(2)
#define		DBMS_MAKER_INFO			(20)
#define		DBMS_PROT_BOARD_NUM		(10)
/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/
typedef enum {
	DBMS_RES_CODE_OK = 0,							/* 通信正常	*/
	DBMS_RES_CODE_PRO_NUM_ERR,						/* 通信規格バジョーンエラー	*/
	DBMS_RES_CODE_CHKSUM_ERR,						/* チェックサムベリファイ */
	DBMS_RES_CODE_LENGTH_ERR,						/* データ長エラー	*/
	DBMS_RES_CODE_CMD_NO_SUPPORT,					/* コメントサポットしない	*/
	DBMS_RES_CODE_DATA_FORMAT_ERR,					/* データフォーマットエラー	*/
	DBMS_RES_CODE_DATA_INVALID,						/* データ無効	*/
	DBMS_RES_CODE_ADR_ERR,						    /* アドレスエラー	*/
	DBMS_RES_CODE_MEM_DEV_ERR,						/* メモリアクセスエラー	*/
	DBMS_RES_CODE_END						        
}EBMS_COM_RES_CODE;

typedef enum {
	DBMS_CMD_GET_BMS_MONITOR_DATA = 0x42,					/* システムモニタデータ取得	*/
	DBMS_CMD_GET_BMS_STATE_DATA = 0x44,						/* システム状態取得	*/
	DBMS_CMD_CTRL = 0x45,									/* システムコントロール	*/
	DBMS_CMD_GET_BMS_PARAM = 0x47,							/* システムパラメータ取得	*/
	DBMS_CMD_SET_BMS_PARAM = 0x49,							/* システムパラメータ設定	*/
	DBMS_CMD_GET_BMS_COM_PROT_VER = 0x4F,					/* 通信規格バジョーン取得	*/
	DBMS_CMD_GET_PRODUCT_INFO = 0x51,						/* プロダクション情報取得	*/
	DBMS_CMD_GET_LOG_DATA = 0x4B,
	DBMS_CMD_GET_CURR_DATE = 0x4D,
	DBMS_CMD_SET_CURR_DATE = 0x4E,
	DBMS_CMD_PRODUCE_CAL = 0xA0,
	DBMS_CMD_SET_PRODUCE_PARAM = 0xA1,
	DBMS_CMD_SET_REC_DATE = 0xA2
}EBMS_COM_CMD;

typedef enum
{
    DBMS_GET_FIRST_REG_LOG,
    DBMS_GET_NEXT_REG_LOG,
    DBMS_GET_LAST_REG_LOG_AGAIN
}EBMS_REG_LOG_TYPE;

typedef enum
{
    DBMS_CAL_CUR_0A,
    DBMS_CAL_CUR_10A,
	DBMS_CAL_CELL_VOL
}EBMS_CAL_TYPE;

typedef enum
{
    DBMS_WRN_NONE = 0x00,
    DBMS_WRN_UNDER,
	DBMS_WRN_OVER,
	DBMS_WRN_OTHER = 0x0F
}EBMS_WRN_TYPE;

typedef enum
{
    DBMS_CHG,
    DBMS_DISCHG,
	DBMS_STANDBY,
    DBMS_UNDEFINE
}EBMS_SYS_ST;

typedef enum{
	DBMS_CTRL_TYPE_DISCHG_ENABLE,
    DBMS_CTRL_TYPE_DISCHG_DISABLE,
	DBMS_CTRL_TYPE_CHG_ENABLE,
    DBMS_CTRL_TYPE_CHG_DISABLE,
	DBMS_CTRL_TYPE_CUR_LIM_ENABLE,
    DBMS_CTRL_TYPE_CUR_LIM_DISABLE,
	DBMS_CTRL_TYPE_HEAT_ENABLE,
    DBMS_CTRL_TYPE_HEAT_DISABLE,
	DBMS_CTRL_TYPE_SYS_SHUTDOWN,
	DBMS_CTRL_TYPE_RECOVER
}EBMS_CTRL_TYPE;

typedef enum
{
    DBMS_REC_LOG_START,
    DBMS_REC_LOG_END
}EBMS_REC_LOG_ST;

typedef enum
{
    DBMS_WRN_DET_CELL_VOL_MAX = 0x00,    /* 単位:mv *//* 単体過充ワーニングと保護検出 */
    DBMS_ERR_DET_CELL_VOL_MAX,    /* 単位:mv */
    DBMS_ERR_DET_CELL_OV_DLY,     /* 単位:S */
    DBMS_ERR_CELL_OV_REL_VOL,    /* 単位:mv */   /* 単体過充保護解除 */       
    DBMS_WRN_DET_CELL_VOL_MIN,    /* 単位:mv *//* 単体過放ワーニングと保護検出 */
    DBMS_ERR_DET_CELL_VOL_MIN,    /* 単位:mv */
    DBMS_ERR_DET_CELL_UV_DLY,     /* 単位:S */
    DBMS_ERR_CELL_UV_REL_VOL,     /* 単位:mv *//* 単体過放保護解除 */
    DBMS_WRN_DET_PACK_VOL_MAX,    /* 単位:10mv *//* 総体過充ワーニングと保護検出 */
    DBMS_ERR_DET_PACK_VOL_MAX,    /* 単位:10mv */
    DBMS_ERR_DET_PACK_OV_DLY,     /* 単位:S */
    DBMS_ERR_PACK_OV_REL_VOL,     /* 単位:10mv *//* 総体過充保護解除 */
    DBMS_WRN_DET_PACK_VOL_MIN,    /* 単位:10mv *//* 総体過放ワーニングと保護検出 */
    DBMS_ERR_DET_PACK_VOL_MIN,    /* 単位:10mv */
    DBMS_ERR_DET_PACK_UV_DLY,     /* 単位:S */
    DBMS_ERR_PACK_UV_REL_VOL,     /* 単位:10mv *//* 総体過放保護解除 */
    DBMS_WRN_DET_CHG_CUR_MAX,    /* 単位:A *//* 充電電流過高ワーニングと保護検出 */
    DBMS_ERR_DET_CHG_CUR_MAX,	/* 単位:A */
    DBMS_ERR_DET_CHG_OC_DLY,	/* 単位:S */
    DBMS_WRN_DET_DISCHG_CUR_MAX,    /* 単位:A *//* 放電電流過高1ワーニングと保護検出 */
    DBMS_ERR_DET_DISCHG_CUR_MAX,    /* 単位:A */
    DBMS_ERR_DET_DISCHG_OC_DLY, 	/* 単位:S */
    DBMS_ERR_DET_DISCHG_CUR_MAX2,      /* 単位:A */  /* 放電電流過高2保護検出 */ 
    DBMS_ERR_DET_DISCHG_OC_DLY2,		/* 単位:mS */
    DBMS_ERR_OC_REL_DLY,				 /* 単位:S */     /* 電流保護解除ディレイ */  
    DBMS_ERR_DET_CUR_LOCK_CNT,			/* 単位: 0.1℃ */ /* 電流保護ロック回数 */
    DBMS_WRN_DET_MOS_TEMP_MAX,   		 /* 単位: 0.1℃ *//* MOS温度過高ワーニングと保護検出 */
    DBMS_ERR_DET_MOS_TEMP_MAX,   		 /* 単位: 0.1℃ */
    DBMS_ERR_MOS_TEMP_REL_TEMP,        /* 単位: 0.1℃ */ /* MOS温度過高保護解除 */
    DBMS_WRN_DET_CELL_CHG_TEMP_MIN,         /* 単位: 0.1℃ *//* セル充電温度過低ワーニングと保護検出 */
    DBMS_ERR_DET_CELL_CHG_TEMP_MIN,    /* 単位: 0.1℃ */
    DBMS_ERR_CELL_CHG_UT_REL_TEMP,		/* 単位: 0.1℃ *//* セル充電温度過低保護解除 */
    DBMS_WRN_DET_CELL_CHG_TEMP_MAX,    /* 単位: 0.1℃ */ /* セル充電温度過高ワーニングと保護検出 */
    DBMS_ERR_DET_CELL_CHG_TEMP_MAX,    /* 単位: 0.1℃ */
    DBMS_ERR_CELL_CHG_OT_REL_TEMP,        /* 単位: 0.1℃ */ /* セル充電温度過高保護解除 */
    DBMS_WRN_DET_CELL_DISCHG_TEMP_MIN,      /* 単位: 0.1℃ */ /* セル放電温度過低ワーニングと保護検出 */
    DBMS_ERR_DET_CELL_DISCHG_TEMP_MIN,    /* 単位: 0.1℃ */
    DBMS_ERR_CELL_DISCHG_UT_REL_TEMP,       /* 単位: 0.1℃ *//* セル放電温度過低保護解除 */
    DBMS_WRN_DET_CELL_DISCHG_TEMP_MAX,    /* 単位: 0.1℃ *//* セル放電温度過高ワーニングと保護検出 */
    DBMS_ERR_DET_CELL_DISCHG_TEMP_MAX,    /* 単位: 0.1℃ */
    DBMS_ERR_CELL_DISCHG_OT_REL_TEMP,    /* 単位: 0.1℃ *//* セル放電温度過高保護解除 */
    DBMS_WRN_DET_EN_TEMP_MIN,              /* 単位: 0.1℃ */ /* 環境温度過低ワーニングと保護検出 */
    DBMS_ERR_DET_EN_TEMP_MIN,  		  /* 単位: 0.1℃ */
    DBMS_ERR_EN_UT_REL_TEMP,           /* 単位: 0.1℃ */ /* 環境温度過低保護解除 */
    DBMS_WRN_DET_EN_TEMP_MAX,              /* 単位: 0.1℃ */ /* 環境温度過高ワーニングと保護 */
    DBMS_ERR_DET_EN_TEMP_MAX,   		 /* 単位: 0.1℃ */
    DBMS_ERR_EN_OT_REL_TEMP,            /* 単位: 0.1℃ *//* 環境温度過高保護解除 */
    DBMS_ERR_DET_CELL_MAX_MIN_DIF,   	 /* 単位:mv *//* セル失効保護検出 */
    DBMS_ERR_CELL_INVAILD_REL_VOL_DIF,	/* 単位:mv *//* セル失効保護解除 */
    DBMS_WRN_DET_SOC_MIN,                   /* 単位:1% *//* SOC過低ワーニング検出 */
    DBMS_CHG_SOC_RECOVER,				/* SOC充電回復容量(%) */
    DBMS_BALANCE_ST_VOL,				/* 単位:mv *//* バランス開始電圧 (単位：mV） */
    DBMS_BALANCE_ST_VOL_DIFF,			/* 単位:mv *//* バランス開始圧差(単位：mV） */
    DBMS_BALANCE_END_VOL_DIFF,			/* 単位:mv *//* バランス終了圧差(単位：mV） */
    DBMS_PACK_DESIGN_CAPACITY,			/* 単位:10mAH *//* パック公称容量(10mAh) */
    DBMS_PACK_REMAINDER_CAP,			/* 単位:10mAH *//* パック残公称容量(10mAh) */
    DBMS_CELL_HEAT_ST_TH,				/* 単位: 0.1℃ *//* セル加熱開始温度(0.1C) */
    DBMS_CELL_HEAT_END_TH,				/* 単位: 0.1℃ *//* セル加熱停止温度(0.1C)  */
    DBMS_CHG_CUR_LIM,					/* 単位: A *//* 充電限流値(単位：10mA） */
    DBMS_STANDBY_TO_SLEEP_TIME,			/* 単位:H *//* 待機―>スリップの時間(h)*/
	DBMS_VAL_PAEAM_END,					/* 数値パラメータ終了 */
    DBMS_VOL_FUN_SW = DBMS_VAL_PAEAM_END,					/* 電圧機能SW */
    DBMS_TH_FUN_SW1,					/* 温度機能SW1 */
    DBMS_TH_FUN_SW2,					/* 温度機能SW2 */
    DBMS_CUR_FUN_SW,					/* 電流機能SW */
    DBMS_CAP_OTHER_FUN_SW,				/* 容量と他の機能 */
    DBMS_BALANCE_FUN_SW,				/* バランス機能SW */
    DBMS_INDICATION_FUN_SW,				/* 指示機能SW */
    DBMS_BOARD_NUM,						/* 保護Board番号 */
	DBMS_ALL_PARAM 						/* 全部param */
}EBMS_PARAM_TYPE;
/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/
typedef union {
	UCHAR		BYTE;
	struct{
		UCHAR		bal_start				:1;					
		UCHAR		dummy1					:1;
		UCHAR		dummy2					:1;
		UCHAR		dummy3					:1;
		UCHAR		cell_invaild			:1;	
		UCHAR		dummy5					:1;
		UCHAR		dummy6					:1;
		UCHAR		dummy7					:1;
	} BIT;
}SBMS_BAL_EVENT;

typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		cell_ov_wrn_det_fun		:1;	
		UCHAR		cell_ov_err_det_fun		:1;	
		UCHAR		cell_uv_wrn_det_fun		:1;	
		UCHAR		cell_uv_err_det_fun		:1;	
		UCHAR		pack_ov_wrn_det_fun		:1;	
		UCHAR		pack_ov_err_det_fun		:1;	
		UCHAR		pack_uv_wrn_det_fun		:1;	
		UCHAR		pack_uv_err_det_fun		:1;	
	} BIT;
}SBMS_VOL_EVENT;

typedef union {
	USHORT	WORD;
	struct{
	    /* BYTE 1 */
		UCHAR		chg_ot_wrn_det_fun		:1;	
		UCHAR		chg_ot_err_det_fun		:1;	
		UCHAR		chg_ut_wrn_det_fun		:1;	
		UCHAR		chg_ut_err_det_fun		:1;	
		UCHAR		dischg_ot_wrn_det_fun	:1;	
		UCHAR		dischg_ot_err_det_fun	:1;	
		UCHAR		dischg_ut_wrn_det_fun	:1;	
		UCHAR		dischg_ut_err_det_fun	:1;	
	    /* BYTE 2 */
	    UCHAR		en_ot_wrn_det_fun		:1;	
		UCHAR		en_ot_err_det_fun		:1;	
		UCHAR		en_ut_wrn_det_fun		:1;	
		UCHAR		en_ut_err_det_fun		:1;	
		UCHAR		mos_ot_wrn_det_fun		:1;	
		UCHAR		mos_ot_err_det_fun		:1;	
		UCHAR		dummy14					:1;	
		UCHAR		dummy15	                :1;
	} BIT;
}SBMS_TEMP_EVENT;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		chg_oc_wrn_det_fun		:1;	
		UCHAR		chg_oc_err_det_fun		:1;	
		UCHAR		dischg_oc_wrn_det_fun		:1;	
		UCHAR		dischg_oc_err_det_fun		:1;	
		UCHAR		dischg_oc2_err_det_fun		:1;	
		UCHAR		dischg_scd_err_det_fun		:1;	
		UCHAR		dischg_oc2_lock_fun		:1;	
		UCHAR		dischg_scd_lock_fun		:1;	
	} BIT;
}SBMS_CUR_EVENT;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		dummy0					:1;	
		UCHAR		dummy1					:1;	
		UCHAR		remain_cap_wrn_det_fun	:1;	
		UCHAR		dummy3					:1;	
		UCHAR		dummy4					:1;	
		UCHAR		dummy5					:1;	
		UCHAR		dummy6					:1;	
		UCHAR		dummy7					:1;	
	} BIT;
}SBMS_CAP_OTHER_EVENT;

typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		dischg_sw_st			:1;	
		UCHAR		chg_sw_st				:1;	
		UCHAR		lim_cur_sw_st			:1;	
		UCHAR		heat_sw_st				:1;	
		UCHAR		dummy4					:1;	
		UCHAR		dummy5					:1;	
		UCHAR		dummy6					:1;	
		UCHAR		dummy7					:1;	
	} BIT;
}SBMS_SW_EVENT;

typedef union {
	USHORT		WORD;
	struct {
		UCHAR	di_cbsel1:			1;
		UCHAR	di_cbsel2:			1;
		UCHAR	di_cbsel3:			1;
		UCHAR	di_cbsel4:			1;
		UCHAR	di_cbsel5:			1;
		UCHAR	di_cbsel6:			1;
		UCHAR	di_cbsel7:			1;
		UCHAR	di_cbsel8:			1;
		UCHAR	di_cbsel9:			1;
		UCHAR	di_cbsel10:			1;
		UCHAR	di_cbsel11:			1;
		UCHAR	di_cbsel12:			1;
		UCHAR	di_cbsel13:			1;
		UCHAR	di_cbsel14:			1;
		UCHAR	di_cbsel15:			1;
		UCHAR	di_cbsel16:			1;
	}BIT;
}SBMS_CB_EVENT;


typedef union {
	USHORT		WORD;
	struct{
		UCHAR		cell_vol_over			:1;
		UCHAR		pack_vol_over			:1;
		UCHAR		cell_vol_under			:1;
		UCHAR		pack_vol_under			:1;
		UCHAR		cur_chg_over			:1;
		UCHAR		cur_dischg_over1		:1;
		UCHAR		mos_temp_over			:1;
		UCHAR		cell_chg_temp_over		:1;
		UCHAR		cell_dischg_temp_over	:1;
		UCHAR		en_temp_over			:1;
		UCHAR		cell_chg_temp_under		:1;
		UCHAR		cell_dischg_temp_under	:1;
		UCHAR		en_temp_under			:1;
		UCHAR		soc_under				:1;	
		UCHAR		dummy1					:1;
		UCHAR		dummy2					:1;
	} BIT;
}SBMS_WRN_DET_RET;
typedef union {
	UCHAR		CHAR;
	struct{
		UCHAR		ov			:1;
		UCHAR		uv			:1;
		UCHAR		scd			:1;
		UCHAR		occ			:1;
		UCHAR		ocd			:1;
		UCHAR		dummy3		:1;
		UCHAR		dummy4		:1;
		UCHAR		dummy5		:1;
	} BIT;
}SBMS_49503_ERR_DET_RET;



typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		cell_ov_wrn_det_fun		:1;	
		UCHAR		cell_ov_err_det_fun		:1;	
		UCHAR		cell_uv_wrn_det_fun		:1;	
		UCHAR		cell_uv_err_det_fun		:1;	
		UCHAR		pack_ov_wrn_det_fun		:1;	
		UCHAR		pack_ov_err_det_fun		:1;	
		UCHAR		pack_uv_wrn_det_fun		:1;	
		UCHAR		pack_uv_err_det_fun		:1;	
	} BIT;
}SBMS_VOL_FUN_SW;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		chg_ot_wrn_det_fun		:1;	
		UCHAR		chg_ot_err_det_fun		:1;	
		UCHAR		chg_ut_wrn_det_fun		:1;	
		UCHAR		chg_ut_err_det_fun		:1;	
		UCHAR		dischg_ot_wrn_det_fun		:1;	
		UCHAR		dischg_ot_err_det_fun		:1;	
		UCHAR		dischg_ut_wrn_det_fun		:1;	
		UCHAR		dischg_ut_err_det_fun		:1;	
	} BIT;
}SBMS_TEMP_FUN_SW1;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		en_ot_wrn_det_fun		:1;	
		UCHAR		en_ot_err_det_fun		:1;	
		UCHAR		en_ut_wrn_det_fun		:1;	
		UCHAR		en_ut_err_det_fun		:1;	
		UCHAR		mos_ot_err_det_fun	:1;	
		UCHAR		cell_ut_heat_fun		:1;	
		UCHAR		dummy6					:1;	
		UCHAR		dummy7					:1;
	} BIT;
}SBMS_TEMP_FUN_SW2;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		chg_oc_wrn_det_fun		:1;	
		UCHAR		chg_oc_err_det_fun		:1;	
		UCHAR		dischg_oc_wrn_det_fun		:1;	
		UCHAR		dischg_oc_err_det_fun		:1;	
		UCHAR		dischg_oc2_err_det_fun		:1;	
		UCHAR		dischg_scd_err_det_fun		:1;	
		UCHAR		dischg_oc2_lock_fun		:1;	
		UCHAR		dischg_scd_lock_fun		:1;	
	} BIT;
}SBMS_CUR_FUN_SW;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		remain_cap_wrn_det_fun		:1;	
		UCHAR		intermittence_chg_fun			:1;	
		UCHAR		external_sw_ctrl_fun			:1;	
		UCHAR		standby_to_shutdown_fun		:1;	
		UCHAR		rec_log_fun					:1;	
		UCHAR		dummy5							:1;	
		UCHAR		dummy6							:1;	
		UCHAR		dummy7							:1;	
	} BIT;
}SBMS_CAP_OTHER_FUN_SW;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		bat_balance_fun				:1;	
		UCHAR		static_balance_fun			:1;	
		UCHAR		static_bal_timer_fun			:1;	
		UCHAR		balance_temp_ctrl_fun			:1;	
		UCHAR		cell_invalid_wrn_fun			:1;	
		UCHAR		dummy5							:1;	
		UCHAR		dummy6							:1;	
		UCHAR		dummy7							:1;	
	} BIT;
}SBMS_BALANCE_FUN_SW;
typedef union {
	unsigned char	BYTE;
	struct{
		UCHAR		buzzer_indicate_fun			:1;	
		UCHAR		lcd_indicate_fun				:1;	
		UCHAR		dummy2							:1;	
		UCHAR		dummy3							:1;	
		UCHAR		dummy4							:1;	
		UCHAR		dummy5							:1;	
		UCHAR		dummy6							:1;	
		UCHAR		dummy7							:1;	
	} BIT;
}SBMS_INDICATION_FUN_SW;
/****************************************************
Function:ワーニング検出と保護検出・解除パラメーター
Date: 2017/03/10 PSDCD@Lifanghseng 
*****************************************************/
typedef struct {
	SHORT si16_val_param[DBMS_VAL_PAEAM_END];
#if 0
	/* 単体過充ワーニングと保護検出 */
	SHORT	si16_wrn_cell_vol_max;			/* セル電圧過高ワーニング閾値(単位：mV） */
	SHORT	si16_err_cell_vol_max;			/* セル電圧過高保護閾値(単位：mV） */
	USHORT	ui16_err_cell_ov_dly;				/* セル電圧過高保護ディレイ閾値(単位：250ms）*/
	/* 単体過充ワーニングと保護解除 */
	SHORT	si16_err_cell_ov_rel_vol;	    /* セル電圧過高保護解除電圧閾値(単位：mV） */
	/* 単体過放ワーニングと保護検出 */
	SHORT	si16_wrn_cell_vol_min;		    /* セル電圧過低ワーニング閾値(単位：mV）*/
	SHORT	si16_err_cell_vol_min;		    /* セル電圧過低保護閾値(単位：mV）*/
	USHORT	ui16_err_cell_uv_dly;		    /* セル電圧過低保護ディレイ閾値 (単位：s）*/
	/* 単体過放ワーニングと保護解除 */
	SHORT	si16_err_cell_uv_rel_vol;	    /* セル電圧過低保護解除電圧閾値(単位：mV） */
	/* 総体過充ワーニングと保護検出 */
	SHORT	si16_wrn_pack_vol_max;			/* パック電圧過高ワーニング閾値(単位：10mV）*/
	SHORT	si16_err_pack_vol_max;			/* パック電圧過高保護閾値(単位：10mV）*/
	USHORT	ui16_err_pack_ov_dly;			/* パック電圧過高保護ディレイ閾値(単位：s）*/
	/* 総体過充保護解除 */
	SHORT	si16_err_pack_ov_rel_vol;		/* パック電圧過高保護解除電圧閾値(単位：10mV） */
	/* 総体過放ワーニングと保護検出 */
	SHORT	si16_wrn_pack_vol_min;			/* パック電圧過低ワーニング閾値(単位：10mV） */
	SHORT	si16_err_pack_vol_min;			/* パック電圧過低保護閾値(単位：10mV） */
	USHORT	ui16_err_pack_uv_dly;			/* パック電圧過低保護ディレイ閾値 (単位：s）*/
	/* 総体過放ワーニングと保護解除 */
	SHORT	si16_err_pack_uv_rel_vol;		/* パック電圧過低保護解除電圧閾値(単位：10mV） */
	/* 充電電流過高ワーニングと保護検出 */
	SHORT	si16_wrn_chg_cur_max;			/* 充電電流過高ワーニング閾値(単位：10mA）*/
	SHORT	si16_err_chg_cur_max;			/* 充電電流過高保護閾値(単位：10mA）*/
	USHORT	ui16_err_chg_oc_dly;			/* 充電電流過高保護ディレイ閾値(単位：s）*/
	/* 放電電流過高ワーニングと保護検出 */
	SHORT	si16_wrn_dischg_cur_max;		/* 放電電流過高1ワーニング閾値(単位：10mA） */
	SHORT	si16_err_dischg_cur_max;		/* 放電電流過高1保護閾値(単位：10mA） */
	USHORT	ui16_err_dischg_oc_dly;			/* 放電電流過高保護ディレイ閾値(単位：250ms）*/
	/* 放電電流過高2保護検出 */
	SHORT	si16_err_dischg_cur_max2;		/* 放電電流過高2保護閾値(単位：10mA） */
	USHORT	ui16_err_dischg_oc_dly2;		/* 放電電流過高2保護ディレイ閾値(単位：10ms）*/
	/* 電流保護解除ディレイ */
	USHORT	ui16_err_oc_rel_dly;		/* 電流過高保護解除ディレイ閾値(単位：s）*/
	/* 電流保護ロック回数 */
	SHORT   si16_oc_lock_cnt;
	/* MOS温度過高ワーニングと保護検出 */
	SHORT	si16_wrn_mos_temp_max;			/* MOS温度過高ワーニング閾値(単位：0.1℃）*/
	SHORT	si16_err_mos_temp_max;			/* MOS温度過高保護閾値(単位：0.1℃）*/
	/* MOS温度過高保護解除 */
	SHORT	si16_err_mos_ot_rel_temp;		/* MOS温度過高保護解除温度閾値(単位：0.1℃）*/
	/* セル充電温度過低ワーニングと保護検出 */
	SHORT	si16_wrn_cell_chg_temp_min;     /* セル充電時温度過低ワーニング閾値(単位：0.1℃） */
	SHORT	si16_err_cell_chg_temp_min;		/* セル充電時温度過低保護閾値(単位：0.1℃） */
	/* セル充電温度過低ワーニングと保護解除 */
	SHORT	si16_err_cell_chg_ut_rel_temp;	/* セル充電時温度過低保護解除閾値(単位：0.1℃） */
	/* セル充電温度過高ワーニングと保護検出 */
	SHORT	si16_wrn_cell_chg_temp_max;		/* セル充電時温度過高ワーニング閾値(単位：0.1℃） */
	SHORT	si16_err_cell_chg_temp_max;		/* セル充電時温度過高保護閾値(単位：0.1℃） */
	/* セル充電温度過高ワーニングと保護解除 */
	SHORT	si16_err_cell_chg_ot_rel_temp;	/* セル充電時温度過高保護解除閾値(単位：0.1℃） */
	/* セル放電温度過低ワーニングと保護検出 */
	SHORT	si16_wrn_cell_dischg_temp_min;  /* セル放電時温度過低ワーニング閾値(単位：0.1℃） */
	SHORT	si16_err_cell_dischg_temp_min;	/* セル放電時温度過低保護閾値(単位：0.1℃） */
	/* セル放電温度過低ワーニングと保護解除 */
	SHORT	si16_err_cell_dischg_ut_rel_temp;/* セル放電時温度過低保護解除閾値(単位：0.1℃） */
	/* セル放電温度過高ワーニングと保護検出 */
	SHORT	si16_wrn_cell_dischg_temp_max;	 /* セル放電時温度過高ワーニング閾値(単位：0.1℃） */
	SHORT	si16_err_cell_dischg_temp_max;   /* セル放電時温度過高保護閾値(単位：0.1℃） */
	/* セル放電温度過高ワーニングと保護解除 */
	SHORT	si16_err_cell_dischg_ot_rel_temp;/* セル放電時温度過高保護解除閾値(単位：0.1℃） */
	/* 環境温度過低ワーニングと保護検出 */
	SHORT	si16_wrn_en_temp_min;            /* 環境温度過低ワーニング閾値(単位：0.1℃） */
	SHORT	si16_err_en_temp_min;			 /* 環境温度過低保護閾値(単位：0.1℃） */
	/* 環境温度過低保護解除 */
	SHORT	si16_err_en_ut_rel_temp;	/* 環境温度過低保護解除閾値(単位：0.1℃） */
	/* 環境温度過高ワーニングと保護 */
	SHORT	si16_wrn_en_temp_max;            /* 環境温度過高ワーニング閾値(単位：0.1℃） */
	SHORT	si16_err_en_temp_max;			 /* 環境温度過高保護閾値(単位：0.1℃） */
	/* 環境温度過高保護解除 */
	SHORT	si16_err_en_ot_rel_temp;	     /* 環境温度過高保護解除閾値(単位：0.1℃） */
	/* セル失効保護検出 */
	SHORT	si16_err_cell_max_min_dif;		 /* セル失効保護閾値(単位：mV） */
	/* セル失効保護解除 */
	SHORT	si16_err_cell_invaild_rel_vol_dif;		 /* セル失効保護閾値(単位：mV） */
	/* SOC過低ワーニング検出 */
	SHORT	si16_wrn_soc_min;                /* SOC過低ワーニング閾値 */
	/* SOC充電回復容量 */
	SHORT	si16_chg_soc_recover;                /* SOC過低ワーニング閾値 */
	/* バランス開始電圧(mV) */
   	USHORT	ui16_balance_st_vol;
    /* バランス開始圧差(mV) */
   	USHORT	ui16_balance_st_vol_diff;
	/* バランス終了圧差(mV) */
   	USHORT	ui16_balance_end_vol_diff;
	/* 容量 */
	USHORT	ui16_pack_design_capacity;		/* パック公称容量(10mAh) */
    /* 残容量 */
	USHORT	ui16_pack_remainder_cap;			/* パック公称容量(10mAh) */
	/* セル加熱開始温度(0.1C) */
	SHORT   si16_cell_heat_st_th;
	/* セル加熱停止温度(0.1C) */
	SHORT   si16_cell_heat_end_th;
	 /* 充電限流値(A) */
	SHORT   si16_chg_limit_cur;
	 /* 待機スリップ時間(H) */
	USHORT   ui16_standby_to_shtdw_time;
#endif
	/* 電圧機能SW */
   	SBMS_VOL_FUN_SW	ui8_vol_fun_sw;
	/* 温度機能SW1 */
   	SBMS_TEMP_FUN_SW1	ui8_th_fun_sw1;
	/* 温度機能SW2 */
   	SBMS_TEMP_FUN_SW2	ui8_th_fun_sw2;
	/* 電流機能SW */
   	SBMS_CUR_FUN_SW	ui8_cur_fun_sw;
	/* 容量と他の機能 */
   	SBMS_CAP_OTHER_FUN_SW	ui8_cap_other_fun_sw;
	/* バランス機能SW */
   	SBMS_BALANCE_FUN_SW	ui8_balance_fun_sw;
	/* 指示機能SW */
   	SBMS_INDICATION_FUN_SW	ui8_indication_fun_sw;
	/* 保護Board番号 */
   	UCHAR	ui8_board_num[DBMS_PROT_BOARD_NUM];
}SBMS_PARAM;
/****************************************************
Function:時間
Date: 2017/04/10 PSDCD@Lifanghseng 
*****************************************************/
typedef struct {
	USHORT		ui16_year;  /* 年 */
	UCHAR		ui8_month;  /* 月 */
	UCHAR		ui8_day;      /* 日 */
	UCHAR		ui8_hour;     /* 時間 */
	UCHAR		ui8_minute;   /* 分 */
	UCHAR		ui8_second;  /* 秒 */
}SBMS_DATE;
/****************************************************
Function:BMS運行パラメーター
Date: 2017/03/10 PSDCD@Lifanghseng 
*****************************************************/
/* BMS監視データ */
typedef struct
{
	UCHAR		ui8_cell_series;
	USHORT		ui16_cell_vol[DBMS_CELL_SERIES_MAX];	/* 1mV */
	UCHAR		ui8_cell_th_cnt;
	SHORT		si16_cell_th[DBMS_CELL_TH_CNT];	/* 単位：0.1K (0.1K = 0.1℃ + 2731) */
	SHORT		si16_en_th;						/* 単位：0.1K (0.1K = 0.1℃ + 2731) */
	SHORT		si16_mos_th;					/* 単位：0.1K (0.1K = 0.1℃ + 2731) */
	SHORT		si16_cur;						/* 10mA */
	USHORT		ui16_pack_vol;					/* 10mV */
	USHORT		ui16_pack_remain_cap;
	USHORT		ui16_pack_cap;
	UCHAR		ui16_cyc_cnt;
}SBMS_MONITOR_DATA;
/* BMS状態データ */
typedef struct
{
	UCHAR					ui8_cell_series;
	EBMS_WRN_TYPE			ebms_cell_vol_wrn_st[DBMS_CELL_SERIES_MAX];
	UCHAR					ui8_cell_th_cnt;
	EBMS_WRN_TYPE			ebms_cell_th_wrn_st[DBMS_CELL_TH_CNT];
	EBMS_WRN_TYPE			ebms_en_th_wrn_st;
	EBMS_WRN_TYPE			ebms_mos_th_wrn_st;
	EBMS_WRN_TYPE			ebms_cur_wrn_st;
	EBMS_WRN_TYPE			ebms_pack_vol_wrn_st;
    SBMS_BAL_EVENT  		sbms_bal_evt;
	SBMS_VOL_EVENT  		sbms_vol_evt;
    SBMS_TEMP_EVENT 		sbms_th_evt;
    SBMS_CUR_EVENT 			sbms_cur_evt;
    SBMS_CAP_OTHER_EVENT	sbms_cap_other_evt;
    SBMS_SW_EVENT  			sbms_sw_evt;
    SBMS_CB_EVENT  			sbms_cb_evt;
    EBMS_SYS_ST    			ebms_st;
}SBMS_SYS_ST_DATA;
/* BMS設備データ */
typedef struct
{
	UCHAR					ui8_device_info[DBMS_DEV_INFO];
	UCHAR					ui8_software_ver_info[DBMS_SW_VER_INFO];
	UCHAR					ui8_maker_info[DBMS_MAKER_INFO];
}SBMS_PRODUCT_INFO;
/* BMS LOG記録データ */
typedef struct
{
    UCHAR                   ui8_next_data_exist_flg; /* 0x01:last record log data; 0x00:not last record log data */
    SBMS_DATE 		        st_log_reg_date;
	EBMS_SYS_ST		        ebms_st;
	UCHAR					cell_invalid_flg;  /* 0x01:cell invalid; 0x00:cell valid */
	SBMS_VOL_EVENT  		sbms_vol_evt;
    SBMS_TEMP_EVENT 		sbms_th_evt;
    SBMS_CUR_EVENT 			sbms_cur_evt;
    SBMS_CAP_OTHER_EVENT	sbms_cap_other_evt;
	SHORT					si16_cur;		/* 10mA */
	USHORT					ui16_pack_vol;	/* 10mV */
	USHORT					ui16_pack_remain_cap;
	UCHAR					ui8_cell_th_cnt;
	SHORT					si16_cell_th[DBMS_CELL_TH_CNT];	/* 単位：0.1K (0.1K = 0.1℃ + 2731) */
	SHORT					si16_en_th;		/* 単位：0.1K (0.1K = 0.1℃ + 2731) */
	SHORT					si16_mos_th;	/* 単位：0.1K (0.1K = 0.1℃ + 2731) */
	UCHAR					ui8_cell_series;
	USHORT					ui16_cell_vol[DBMS_CELL_SERIES_MAX];	/* 1mV */
}SBMS_LOG_DATA;
/****************************************************************************
 Prototype declaration
*****************************************************************************/
/***************************BMS通信リクエスト取得***************************/
/* 0x42 0x44 0x47 0x4F 0x51 0x4D */
void	BMS_COM_get_req_data_common_g(EBMS_COM_CMD bms_com_cmd, UCHAR pack_no, USHORT *req_data_len, UCHAR *req_data);
/* 0x45  */
void	BMS_COM_get_req_data_of_bms_ctrl_g(UCHAR pack_no, EBMS_CTRL_TYPE ebms_ctrl_type, USHORT *req_data_len, UCHAR *req_data);
/* 0x49  0xA1 */
void	BMS_COM_get_req_data_of_set_bms_param_g(UCHAR pack_no, SBMS_PARAM *bms_param,EBMS_PARAM_TYPE ebms_param_id, USHORT *req_data_len, UCHAR *req_data);
/* 0x4B  */
void	BMS_COM_get_req_data_of_get_log_data(UCHAR pack_no, EBMS_REG_LOG_TYPE reg_log_type ,USHORT *req_data_len, UCHAR *req_data);
/* 0x4E  */
void	BMS_COM_get_req_data_of_set_curr_date(SBMS_DATE *sbms_date ,USHORT *req_data_len, UCHAR *req_data);
/* 0xA0  */
void	BMS_COM_get_req_data_of_produce_cal_g(EBMS_CAL_TYPE ebms_cal_type ,SHORT cal_val, USHORT *req_data_len, UCHAR *req_data);
/* 0xA2  */
void	BMS_COM_get_req_data_of_set_rec_date_g(EBMS_REC_LOG_ST ebms_reg_log_st, SBMS_DATE *sbms_reg_start_time, SBMS_DATE *sbms_reg_end_time, USHORT reg_interval_time, USHORT *req_data_len, UCHAR *req_data);
/***************************BMS通信レスポンス取得***************************/
/* 0x42 */
void	BMS_COM_get_bms_monitor_data_from_res_g(USHORT req_len, UCHAR *req_data, SBMS_MONITOR_DATA *bms_monitor_data, EBMS_COM_RES_CODE *res_code);
/* 0x44 */
void	BMS_COM_get_bms_state_data_from_res_g(USHORT req_len, UCHAR *req_data, SBMS_SYS_ST_DATA *bms_state_data, EBMS_COM_RES_CODE *res_code);
/*  0x47 */
void	BMS_COM_get_bms_param_data_from_res_g(USHORT req_len, UCHAR *req_data, SBMS_PARAM *sbms_param, EBMS_COM_RES_CODE *res_code);
/*  0x51 */
void	BMS_COM_get_prot_ver_from_res_g(USHORT req_len, UCHAR *req_data, USHORT *prot_ver, EBMS_COM_RES_CODE *res_code);
/*  0x4F */
void	BMS_COM_get_product_info_from_res_g(USHORT req_len, UCHAR *req_data,SBMS_PRODUCT_INFO *sbms_product_info, EBMS_COM_RES_CODE *res_code);
/*  0x4B */
void	BMS_COM_get_log_data_from_res_g(USHORT req_len, UCHAR *req_data,SBMS_LOG_DATA *sbms_log_data, EBMS_COM_RES_CODE *res_code);
/*  0x4D */
void	BMS_COM_get_curr_date_from_res_g(USHORT req_len, UCHAR *req_data,SBMS_DATE *sbms_date, EBMS_COM_RES_CODE *res_code);
/* 0x45,0x49,0x4E,0xA0,0xA1,0xA2  */
void	BMS_COM_get_res_common_data_from_res_g(USHORT req_len, UCHAR *req_data, EBMS_COM_RES_CODE *res_code);
#endif /* __BMS_COM_PROT_H */

