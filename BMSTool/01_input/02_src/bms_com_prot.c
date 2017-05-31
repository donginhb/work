/****************************************************************************
File name	: bms_com_prot.c
Content 		: the main control of the Communication with controller
Date/Author 	: 2017.03.16	PSDCD@Lifanghseng 
Change Log	: 
Copyright	: Panasonic Industrial Devices Sales( CHINA ) CO., LTD
*****************************************************************************/

/****************************************************************************
#include files
*****************************************************************************/
#include		"bms_com_prot.h"

/****************************************************************************
 define macro
*****************************************************************************/
/* frame header  */
#define	DBMS_COM_FRAME_SOI			(0x7E)
#define	DBMS_COM_FRAME_VER			(0x26)//(0x26)
#define	DBMS_COM_FRAME_ADR			(0x00)//(0x00)
#define	DBMS_COM_FRAME_CID1			(0x46)
#define	DBMS_COM_FRAME_EOI			(0x0D)

#define	DBMS_COM_SYS_CTRL_ENABLE			(0x10)
#define	DBMS_COM_SYS_CTRL_DISABLE		(0x1F)

#define DBMS_COM_MONITOR_DATA_LEN_MIN (1 + 1+ (1*2) + 1 + (1*2) + 10 + 1 + 4)

#define DBMS_COM_USER_DEFINE_STATE_CNT (9)
#define DBMS_COM_STATE_DATA_LEN_MIN (1 + 1+ (1*1) + 1 + (1*1) + 4 + 1 + DBMS_COM_USER_DEFINE_STATE_CNT)
#define DBMS_COM_PROT_VER_LEN (0)
#define DBMS_COM_GET_CURR_DATE_LEN (7)
#define DBMS_COM_BMS_PARAM_LEN_MIN	(sizeof(SBMS_PARAM))

#define	DBMS_COM_NULL		(0x00000000)
/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/
typedef union{
	UCHAR		all[DBMS_COM_FRAME_MAX];
	struct{
		UCHAR			soi;				/* Start indetify */
		UCHAR			ver;				/* 通信規格バジョーン*/
		UCHAR			adr;			    /* 設備アドレス  */
		UCHAR			cid1;			    /* コメント1  */
		UCHAR			cid2;			    /* コメント2  */
		UCHAR			lchecksum;			/* データ長のチェックサーム */
		UCHAR			lenid;			    /* データ長 */
		UCHAR			data[DBMS_COM_DATA_MAX];		/* データ */
		UCHAR			chksum_h;			/* フレームのチェックサム上位 */
		UCHAR			chksum_l;			/* フレームのチェックサム下位      */
		UCHAR			eoi;			    /* End indetify       */
	}byte;
}SBMS_COM_FORMAT;
/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/
SBMS_COM_FORMAT	sbms_sys_com_emit;						// 通信データ格納RAM
SBMS_COM_FORMAT	sbms_sys_com_rcv;						// communication receive data ram
UCHAR	UI8_prot_ver = 0x26;						// communication receive data ram
/****************************************************************************
 Prototype declaration
*****************************************************************************/
static UCHAR COM_RevFormatCheck(EBMS_COM_CMD req_cmd_id, USHORT req_len, UCHAR *req_data);
static void COM_Chksum(UCHAR *buffer_ptr, USHORT info_length, USHORT* chksum_result);
static void COM_Lchksum(USHORT linfo_length, UCHAR * lchksum_result);
static unsigned char COM_CharToHex(unsigned char bHex);
static unsigned char COM_HexToChar(unsigned char bChar);
static void	COM_req_data_common(USHORT data_size, USHORT *req_data_len, UCHAR *req_data);

/* 0x42 0x44 0x47 0x4F 0x51 0x4D */
void	BMS_COM_get_req_data_common_g(EBMS_COM_CMD bms_com_cmd, UCHAR pack_no, USHORT *req_data_len, UCHAR *req_data)
{
	USHORT ascii_data_len = 0;

	/* CMD process */
	sbms_sys_com_emit.byte.cid2 = bms_com_cmd;
	if((DBMS_CMD_GET_BMS_COM_PROT_VER == bms_com_cmd) || (DBMS_CMD_GET_PRODUCT_INFO == bms_com_cmd) || (DBMS_CMD_GET_CURR_DATE == bms_com_cmd))
	{
		ascii_data_len = 0;
	}
	else
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = pack_no;
	}
	COM_req_data_common(ascii_data_len, req_data_len, req_data);
}
/* 0x45  */
void	BMS_COM_get_req_data_of_bms_ctrl_g(UCHAR pack_no, EBMS_CTRL_TYPE ebms_ctrl_type, USHORT *req_data_len, UCHAR *req_data)
{
	USHORT ascii_data_len = 0;
	
	sbms_sys_com_emit.byte.cid2 = DBMS_CMD_CTRL;
	sbms_sys_com_emit.byte.data[ascii_data_len++] = pack_no;
	switch(ebms_ctrl_type)
	{
		case DBMS_CTRL_TYPE_DISCHG_ENABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x00;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
			break;
    	case DBMS_CTRL_TYPE_DISCHG_DISABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x00;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_DISABLE;
			break;
		case DBMS_CTRL_TYPE_CHG_ENABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x01;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
			break;
    	case DBMS_CTRL_TYPE_CHG_DISABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x01;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_DISABLE;
			break;
		case DBMS_CTRL_TYPE_CUR_LIM_ENABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x02;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
			break;
    	case DBMS_CTRL_TYPE_CUR_LIM_DISABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x02;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_DISABLE;
			break;
		case DBMS_CTRL_TYPE_HEAT_ENABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x03;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
			break;
    	case DBMS_CTRL_TYPE_HEAT_DISABLE:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x03;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_DISABLE;
			break;
		case DBMS_CTRL_TYPE_SYS_SHUTDOWN:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x04;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
			break;
		case DBMS_CTRL_TYPE_RECOVER:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x05;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
			break;
		default:
			ascii_data_len = 0;
			break;
	}
	if(0 != ascii_data_len)
	{
		COM_req_data_common(ascii_data_len, req_data_len, req_data);
	}
	else
	{
		*req_data_len = 0;
	}
}
/* 0x49  0xA1 */
void	BMS_COM_get_req_data_of_set_bms_param_g(UCHAR pack_no, SBMS_PARAM *bms_param, EBMS_PARAM_TYPE ebms_param_id, USHORT *req_data_len, UCHAR *req_data)
{
	UCHAR i;
	USHORT ascii_data_len = 0;
	
	if(DBMS_ALL_PARAM == ebms_param_id)
	{
		sbms_sys_com_emit.byte.cid2 = DBMS_CMD_SET_PRODUCE_PARAM;
	}
	else
	{
		sbms_sys_com_emit.byte.cid2 = DBMS_CMD_SET_BMS_PARAM;
	}
	sbms_sys_com_emit.byte.data[ascii_data_len++] = pack_no;
	switch(ebms_param_id)
	{
		case DBMS_WRN_DET_CELL_VOL_MAX:    /* 単位:mv *//* 単体過充ワーニングと保護検出 */
	    case DBMS_ERR_DET_CELL_VOL_MAX:    /* 単位:mv */
	    case DBMS_ERR_DET_CELL_OV_DLY:     /* 単位:S */
	    case DBMS_ERR_CELL_OV_REL_VOL:    /* 単位:mv */   /* 単体過充保護解除 */       
	    case DBMS_WRN_DET_CELL_VOL_MIN:    /* 単位:mv *//* 単体過放ワーニングと保護検出 */
	    case DBMS_ERR_DET_CELL_VOL_MIN:    /* 単位:mv */
	    case DBMS_ERR_DET_CELL_UV_DLY:     /* 単位:S */
	    case DBMS_ERR_CELL_UV_REL_VOL:     /* 単位:mv *//* 単体過放保護解除 */
	    case DBMS_WRN_DET_PACK_VOL_MAX:    /* 単位:10mv *//* 総体過充ワーニングと保護検出 */
	    case DBMS_ERR_DET_PACK_VOL_MAX:    /* 単位:10mv */
	    case DBMS_ERR_DET_PACK_OV_DLY:     /* 単位:S */
	    case DBMS_ERR_PACK_OV_REL_VOL:     /* 単位:10mv *//* 総体過充保護解除 */
	    case DBMS_WRN_DET_PACK_VOL_MIN:    /* 単位:10mv *//* 総体過放ワーニングと保護検出 */
	    case DBMS_ERR_DET_PACK_VOL_MIN:    /* 単位:10mv */
	    case DBMS_ERR_DET_PACK_UV_DLY:     /* 単位:S */
	    case DBMS_ERR_PACK_UV_REL_VOL:     /* 単位:10mv *//* 総体過放保護解除 */
	    case DBMS_WRN_DET_CHG_CUR_MAX:    /* 単位:A *//* 充電電流過高ワーニングと保護検出 */
	    case DBMS_ERR_DET_CHG_CUR_MAX:   /* 単位:A */
	    case DBMS_ERR_DET_CHG_OC_DLY:    /* 単位:S */
	    case DBMS_WRN_DET_DISCHG_CUR_MAX:    /* 単位:A *//* 放電電流過高1ワーニングと保護検出 */
	    case DBMS_ERR_DET_DISCHG_CUR_MAX:    /* 単位:A */
	    case DBMS_ERR_DET_DISCHG_OC_DLY:     /* 単位:S */
	    case DBMS_ERR_DET_DISCHG_CUR_MAX2:      /* 単位:A */  /* 放電電流過高2保護検出 */ 
	    case DBMS_ERR_DET_DISCHG_OC_DLY2:        /* 単位:mS */
	    case DBMS_ERR_OC_REL_DLY:                 /* 単位:S */     /* 電流保護解除ディレイ */  
	    case DBMS_ERR_DET_CUR_LOCK_CNT:          /* 単位: 0.1℃ */ /* 電流保護ロック回数 */
	    case DBMS_WRN_DET_MOS_TEMP_MAX:           /* 単位: 0.1℃ *//* MOS温度過高ワーニングと保護検出 */
	    case DBMS_ERR_DET_MOS_TEMP_MAX:           /* 単位: 0.1℃ */
	    case DBMS_ERR_MOS_TEMP_REL_TEMP:        /* 単位: 0.1℃ */ /* MOS温度過高保護解除 */
	    case DBMS_WRN_DET_CELL_CHG_TEMP_MIN:         /* 単位: 0.1℃ *//* セル充電温度過低ワーニングと保護検出 */
	    case DBMS_ERR_DET_CELL_CHG_TEMP_MIN:    /* 単位: 0.1℃ */
	    case DBMS_ERR_CELL_CHG_UT_REL_TEMP:      /* 単位: 0.1℃ *//* セル充電温度過低保護解除 */
	    case DBMS_WRN_DET_CELL_CHG_TEMP_MAX:    /* 単位: 0.1℃ */ /* セル充電温度過高ワーニングと保護検出 */
	    case DBMS_ERR_DET_CELL_CHG_TEMP_MAX:    /* 単位: 0.1℃ */
	    case DBMS_ERR_CELL_CHG_OT_REL_TEMP:        /* 単位: 0.1℃ */ /* セル充電温度過高保護解除 */
	    case DBMS_WRN_DET_CELL_DISCHG_TEMP_MIN:      /* 単位: 0.1℃ */ /* セル放電温度過低ワーニングと保護検出 */
	    case DBMS_ERR_DET_CELL_DISCHG_TEMP_MIN:    /* 単位: 0.1℃ */
	    case DBMS_ERR_CELL_DISCHG_UT_REL_TEMP:       /* 単位: 0.1℃ *//* セル放電温度過低保護解除 */
	    case DBMS_WRN_DET_CELL_DISCHG_TEMP_MAX:    /* 単位: 0.1℃ *//* セル放電温度過高ワーニングと保護検出 */
	    case DBMS_ERR_DET_CELL_DISCHG_TEMP_MAX:    /* 単位: 0.1℃ */
	    case DBMS_ERR_CELL_DISCHG_OT_REL_TEMP:    /* 単位: 0.1℃ *//* セル放電温度過高保護解除 */
	    case DBMS_WRN_DET_EN_TEMP_MIN:              /* 単位: 0.1℃ */ /* 環境温度過低ワーニングと保護検出 */
	    case DBMS_ERR_DET_EN_TEMP_MIN:         /* 単位: 0.1℃ */
	    case DBMS_ERR_EN_UT_REL_TEMP:           /* 単位: 0.1℃ */ /* 環境温度過低保護解除 */
	    case DBMS_WRN_DET_EN_TEMP_MAX:              /* 単位: 0.1℃ */ /* 環境温度過高ワーニングと保護 */
	    case DBMS_ERR_DET_EN_TEMP_MAX:            /* 単位: 0.1℃ */
	    case DBMS_ERR_EN_OT_REL_TEMP:            /* 単位: 0.1℃ *//* 環境温度過高保護解除 */
	    case DBMS_ERR_DET_CELL_MAX_MIN_DIF:       /* 単位:mv *//* セル失効保護検出 */
	    case DBMS_ERR_CELL_INVAILD_REL_VOL_DIF:  /* 単位:mv *//* セル失効保護解除 */
	    case DBMS_WRN_DET_SOC_MIN:                   /* 単位:1% *//* SOC過低ワーニング検出 */
	    case DBMS_CHG_SOC_RECOVER:               /* SOC充電回復容量(%) */
	    case DBMS_BALANCE_ST_VOL:                /* 単位:mv *//* バランス開始電圧 (単位：mV） */
	    case DBMS_BALANCE_ST_VOL_DIFF:           /* 単位:mv *//* バランス開始圧差(単位：mV） */
	    case DBMS_BALANCE_END_VOL_DIFF:          /* 単位:mv *//* バランス終了圧差(単位：mV） */
	    case DBMS_PACK_DESIGN_CAPACITY:          /* 単位:10mAH *//* パック公称容量(10mAh) */
	    case DBMS_PACK_REMAINDER_CAP:            /* 単位:10mAH *//* パック残公称容量(10mAh) */
	    case DBMS_CELL_HEAT_ST_TH:               /* 単位: 0.1℃ *//* セル加熱開始温度(0.1C) */
	    case DBMS_CELL_HEAT_END_TH:              /* 単位: 0.1℃ *//* セル加熱停止温度(0.1C)  */
	    case DBMS_CHG_CUR_LIM:                   /* 単位: A *//* 充電限流値(単位：10mA） */
	    case DBMS_STANDBY_TO_SLEEP_TIME:         /* 単位:H *//* 待機―>スリップの時間(h)*/
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(bms_param->si16_val_param[ebms_param_id] >> 8) & 0x00FF;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(bms_param->si16_val_param[ebms_param_id] & 0x00FF);
			break;
	    case DBMS_VOL_FUN_SW:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_vol_fun_sw.BYTE;
			break;
	    case DBMS_TH_FUN_SW1:                    /* 温度機能SW1 */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_th_fun_sw1.BYTE;
			break;
	    case DBMS_TH_FUN_SW2:                    /* 温度機能SW2 */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_th_fun_sw2.BYTE;
			break;
	    case DBMS_CUR_FUN_SW:                    /* 電流機能SW */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_cur_fun_sw.BYTE;
			break;
	    case DBMS_CAP_OTHER_FUN_SW:              /* 容量と他の機能 */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_cap_other_fun_sw.BYTE;
			break;
	    case DBMS_BALANCE_FUN_SW:                /* バランス機能SW */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_balance_fun_sw.BYTE;
			break;
	    case DBMS_INDICATION_FUN_SW:             /* 指示機能SW */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_indication_fun_sw.BYTE;
			break;
	    case DBMS_BOARD_NUM:                     /* 保護Board番号 */
			sbms_sys_com_emit.byte.data[ascii_data_len++] = ebms_param_id;
			for(i = 0; i< DBMS_PROT_BOARD_NUM; i++)
			{
				sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(bms_param->ui8_board_num[i] >> 8) & 0x00FF;
				sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(bms_param->ui8_board_num[i] & 0x00FF);
			}
			break;
	    case DBMS_ALL_PARAM:                      /* 全部param */
			for(i = 0; i< DBMS_VAL_PAEAM_END; i++)
			{
				sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(bms_param->si16_val_param[i] >> 8) & 0x00FF;
				sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(bms_param->si16_val_param[i] & 0x00FF);
			}
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_vol_fun_sw.BYTE;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_th_fun_sw1.BYTE;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_th_fun_sw2.BYTE;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_cur_fun_sw.BYTE;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_cap_other_fun_sw.BYTE;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_balance_fun_sw.BYTE;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_indication_fun_sw.BYTE;
			for(i = 0; i< DBMS_PROT_BOARD_NUM; i++)
			{
				sbms_sys_com_emit.byte.data[ascii_data_len++] = bms_param->ui8_board_num[i];
			}
			break;
		default:
			ascii_data_len = 0;
			break;
	}
	if(0 != ascii_data_len)
	{
		COM_req_data_common(ascii_data_len, req_data_len, req_data);
	}
	else
	{
		*req_data_len = 0;
	}
}

/* 0x4B  */
void	BMS_COM_get_req_data_of_get_log_data(UCHAR pack_no, EBMS_REG_LOG_TYPE reg_log_type ,USHORT *req_data_len, UCHAR *req_data)
{
	USHORT ascii_data_len = 0;
	
	sbms_sys_com_emit.byte.cid2 = DBMS_CMD_GET_LOG_DATA;
	sbms_sys_com_emit.byte.data[ascii_data_len++] = pack_no;
	if(DBMS_GET_FIRST_REG_LOG == reg_log_type)
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x00;
	}
	else if(DBMS_GET_NEXT_REG_LOG == reg_log_type)
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x01;
	}
	else
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x02;
	}
	COM_req_data_common(ascii_data_len, req_data_len, req_data);
}
/* 0x4E  */
void	BMS_COM_get_req_data_of_set_curr_date(SBMS_DATE *sbms_date ,USHORT *req_data_len, UCHAR *req_data)
{
	USHORT ascii_data_len = 0;
	
	sbms_sys_com_emit.byte.cid2 = DBMS_CMD_SET_CURR_DATE;
	if(DBMS_COM_NULL != sbms_date)
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(sbms_date->ui16_year >> 8) & 0x00FF;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(sbms_date->ui16_year & 0x00FF);
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_date->ui8_month;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_date->ui8_day;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_date->ui8_hour;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_date->ui8_minute;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_date->ui8_second;
	}
	else
	{
		ascii_data_len = 0;
	}
	if(0 != ascii_data_len)
	{
		COM_req_data_common(ascii_data_len, req_data_len, req_data);
	}
	else
	{
		*req_data_len = 0;
	}
}
/* 0xA0  */
void	BMS_COM_get_req_data_of_produce_cal_g(EBMS_CAL_TYPE ebms_cal_type ,SHORT cal_val, USHORT *req_data_len, UCHAR *req_data)
{
	USHORT ascii_data_len = 0;
	
	sbms_sys_com_emit.byte.cid2 = DBMS_CMD_PRODUCE_CAL;
	switch(ebms_cal_type)
	{
		case DBMS_CAL_CUR_0A:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x00;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(cal_val >> 8) & 0x00FF;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(cal_val & 0x00FF);
			break;
    	case DBMS_CAL_CUR_10A:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x01;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(cal_val >> 8) & 0x00FF;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(cal_val & 0x00FF);
			break;
		case DBMS_CAL_CELL_VOL:
			sbms_sys_com_emit.byte.data[ascii_data_len++] = 0x02;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(cal_val >> 8) & 0x00FF;
			sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(cal_val & 0x00FF);
			break;
		default:
			ascii_data_len = 0;
	}
	
	if(0 != ascii_data_len)
	{
		COM_req_data_common(ascii_data_len, req_data_len, req_data);
	}
	else
	{
		*req_data_len = 0;
	}
}
/* 0xA2  */
void	BMS_COM_get_req_data_of_set_rec_date_g(EBMS_REC_LOG_ST ebms_reg_log_st, SBMS_DATE *sbms_reg_start_time, SBMS_DATE *sbms_reg_end_time, USHORT reg_interval_time, USHORT *req_data_len, UCHAR *req_data)
{
	USHORT ascii_data_len = 0;
	
	sbms_sys_com_emit.byte.cid2 = DBMS_CMD_SET_REC_DATE;
	if(DBMS_REC_LOG_START == ebms_reg_log_st)
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_ENABLE;
	}
	else
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = DBMS_COM_SYS_CTRL_DISABLE;
	}
	if((DBMS_COM_NULL != sbms_reg_start_time) && (DBMS_COM_NULL != sbms_reg_end_time))
	{
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(sbms_reg_start_time->ui16_year >> 8) & 0x00FF;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(sbms_reg_start_time->ui16_year & 0x00FF);
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_start_time->ui8_month;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_start_time->ui8_day;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_start_time->ui8_hour;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_start_time->ui8_minute;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_start_time->ui8_second;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(sbms_reg_end_time->ui16_year >> 8) & 0x00FF;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(sbms_reg_end_time->ui16_year & 0x00FF);
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_end_time->ui8_month;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_end_time->ui8_day;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_end_time->ui8_hour;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_end_time->ui8_minute;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = sbms_reg_end_time->ui8_second;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(reg_interval_time >> 8) & 0x00FF;
		sbms_sys_com_emit.byte.data[ascii_data_len++] = (UCHAR)(reg_interval_time & 0x00FF);
	}
	else
	{
		ascii_data_len = 0;
	}
	if(0 != ascii_data_len)
	{
		COM_req_data_common(ascii_data_len, req_data_len, req_data);
	}
	else
	{
		*req_data_len = 0;
	}
}

static void	COM_req_data_common(USHORT ascii_data_len, USHORT *req_data_len, UCHAR *req_data)
{
	USHORT i;
	UCHAR char_temp;
	USHORT check_sum = 0;
	UCHAR lcheck_sum = 0;
	
	/* start  */
	sbms_sys_com_emit.byte.soi = DBMS_COM_FRAME_SOI;
	/* version  */
	sbms_sys_com_emit.byte.ver = UI8_prot_ver;
	/* address  */
	sbms_sys_com_emit.byte.adr = DBMS_COM_FRAME_ADR;
	/* cid1  */
	sbms_sys_com_emit.byte.cid1 = DBMS_COM_FRAME_CID1;
	/* eoi set */
	sbms_sys_com_emit.byte.eoi = DBMS_COM_FRAME_EOI;
	/* ASCII -> HEX */
	/* Length */
	sbms_sys_com_emit.byte.lchecksum = (UCHAR)(((ascii_data_len * 2) >> 8) & 0x000F);
	sbms_sys_com_emit.byte.lenid = (UCHAR)((ascii_data_len * 2) & 0x00FF);
	/* LCheckSum */
	COM_Lchksum((ascii_data_len * 2), &lcheck_sum);
	sbms_sys_com_emit.byte.lchecksum |= ((lcheck_sum << 4) & 0xF0);
	/* data transfer to send buffer */
	/* SOI */
	req_data[0] = sbms_sys_com_emit.byte.soi;
	/* version ~ data info convert */
	for(i = 1; i < (ascii_data_len + DBMS_COM_FRAME_CTRL_DATA_MAX -3) ;i++)
	{
		char_temp = (sbms_sys_com_emit.all[i] >> 4) & 0x0F;
		req_data[i*2 - 1] = COM_CharToHex(char_temp);
		char_temp = sbms_sys_com_emit.all[i] & 0x0F;
		req_data[i*2] = COM_CharToHex(char_temp);
	}
	/* Check sum */
	COM_Chksum((UCHAR*)req_data, (ascii_data_len*2), &check_sum);
	char_temp = (check_sum >> 12) & 0x000F;
	req_data[i*2 - 1] = COM_CharToHex(char_temp);
	char_temp = (check_sum >> 8) & 0x000F;
	req_data[i*2] = COM_CharToHex(char_temp);
	char_temp = (check_sum >> 4) & 0x000F;
	req_data[i*2 + 1] = COM_CharToHex(char_temp);
	char_temp = check_sum & 0x000F;
	req_data[i*2 + 2] = COM_CharToHex(char_temp);
	/* EOI */
	req_data[i*2 + 3] = sbms_sys_com_emit.byte.eoi;
	/* all data length */
	*req_data_len = i*2 + 4;
}
/***************************BMS通信レスポンス取得***************************/
/* 0x42 */
void	BMS_COM_get_bms_monitor_data_from_res_g(USHORT req_len, UCHAR *req_data, SBMS_MONITOR_DATA *bms_monitor_data, EBMS_COM_RES_CODE *res_code)
{
	UCHAR i;
	USHORT ascii_data_len = 0;
	UCHAR rec_code_temp;
	USHORT data_len;
	USHORT data_len_dst;
	UCHAR pack_id;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_BMS_MONITOR_DATA, req_len, req_data);
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < DBMS_COM_MONITOR_DATA_LEN_MIN)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* do nothing */
		}
	}

	for(pack_id = 0; (pack_id < sbms_sys_com_rcv.byte.data[0]) && (DBMS_RES_CODE_OK == rec_code_temp); pack_id++)
	{
		ascii_data_len++;
		bms_monitor_data[pack_id].ui8_cell_series = sbms_sys_com_rcv.byte.data[ascii_data_len];
		/* Battery Count check */
		if(bms_monitor_data[pack_id].ui8_cell_series > DBMS_CELL_SERIES_MAX)
		{
			bms_monitor_data[pack_id].ui8_cell_series = DBMS_CELL_SERIES_MAX;
		}
		else
		{
			/* do nothing */
		}
		data_len_dst = 1 + 1 + (bms_monitor_data[pack_id].ui8_cell_series * 2) + 1;
		if(data_len < data_len_dst)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* Cell1~13 Vol(mV) */
			for(i = 0;i < bms_monitor_data[pack_id].ui8_cell_series; i++)
			{
				/* Cell Vol */
				bms_monitor_data[pack_id].ui16_cell_vol[i] = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
				ascii_data_len += 2;
			}
			ascii_data_len += 1;
			bms_monitor_data[pack_id].ui8_cell_th_cnt = sbms_sys_com_rcv.byte.data[ascii_data_len];
			/* temp Count check */
			if(bms_monitor_data[pack_id].ui8_cell_th_cnt > DBMS_CELL_TH_CNT)
			{
				bms_monitor_data[pack_id].ui8_cell_th_cnt = DBMS_CELL_TH_CNT;
			}
			else
			{
				/* do nothing */
			}
			data_len_dst += (bms_monitor_data[pack_id].ui8_cell_th_cnt * 2) + (2*5) + 1;
		}
		if((data_len >= data_len_dst) && (DBMS_RES_CODE_OK == rec_code_temp))
		{
			/* Temp */
			for(i = 0;i < bms_monitor_data[pack_id].ui8_cell_th_cnt; i++)
			{
				/* Cell Temp */
				bms_monitor_data[pack_id].si16_cell_th[i] = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
				ascii_data_len += 2;
			}
			/* En Temp */
			bms_monitor_data[pack_id].si16_en_th = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			/* MOS Temp */
			bms_monitor_data[pack_id].si16_mos_th = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			/* Bat Current (10mA)  */
			bms_monitor_data[pack_id].si16_cur = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			/* Pack Vol (10mV) */
			bms_monitor_data[pack_id].ui16_pack_vol = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			/* Bat Remainder Capacity (10mAH) */
			bms_monitor_data[pack_id].ui16_pack_remain_cap = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			/* User define */
			ascii_data_len += 1;
			data_len_dst += sbms_sys_com_rcv.byte.data[ascii_data_len] * 2;
		}
		else
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		if((data_len >= data_len_dst) && (DBMS_RES_CODE_OK == rec_code_temp))
		{
			/* Bat Capacity (10mAH)  */
			bms_monitor_data[pack_id].ui16_pack_cap = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			/* Bat Cycle */
			bms_monitor_data[pack_id].ui16_cyc_cnt = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
		}
		else
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/* 0x44 */
void	BMS_COM_get_bms_state_data_from_res_g(USHORT req_len, UCHAR *req_data, SBMS_SYS_ST_DATA *bms_state_data, EBMS_COM_RES_CODE *res_code)
{
	UCHAR i;
	USHORT ascii_data_len = 0;
	UCHAR rec_code_temp;
	USHORT data_len;
	USHORT data_len_dst;
	UCHAR pack_id;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_BMS_STATE_DATA, req_len, req_data);
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < DBMS_COM_STATE_DATA_LEN_MIN)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* do nothing */
		}
	}

	for(pack_id = 0; (pack_id < sbms_sys_com_rcv.byte.data[0]) && (DBMS_RES_CODE_OK == rec_code_temp); pack_id++)
	{
		ascii_data_len++;
		bms_state_data[pack_id].ui8_cell_series = sbms_sys_com_rcv.byte.data[ascii_data_len];
		/* Battery Count check */
		if(bms_state_data[pack_id].ui8_cell_series > DBMS_CELL_SERIES_MAX)
		{
			bms_state_data[pack_id].ui8_cell_series = DBMS_CELL_SERIES_MAX;
		}
		else
		{
			/* do nothing */
		}
		data_len_dst = 1 + 1 + (bms_state_data[pack_id].ui8_cell_series * 1) + 1;
		if(data_len < data_len_dst)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* Cell1~13 Vol(mV) */
			for(i = 0;i < bms_state_data[pack_id].ui8_cell_series; i++)
			{
				/* Cell Vol */
				bms_state_data[pack_id].ebms_cell_vol_wrn_st[i] = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
				ascii_data_len += 1;
			}
			ascii_data_len += 1;
			bms_state_data[pack_id].ui8_cell_th_cnt = sbms_sys_com_rcv.byte.data[ascii_data_len];
			/* temp Count check */
			if(bms_state_data[pack_id].ui8_cell_th_cnt > DBMS_CELL_TH_CNT)
			{
				bms_state_data[pack_id].ui8_cell_th_cnt = DBMS_CELL_TH_CNT;
			}
			else
			{
				/* do nothing */
			}
			data_len_dst += (bms_state_data[pack_id].ui8_cell_th_cnt * 1) + (1 * 4) + 1;
		}
		if((data_len >= data_len_dst) && (DBMS_RES_CODE_OK == rec_code_temp))
		{
			/* Temp */
			for(i = 0;i < bms_state_data[pack_id].ui8_cell_th_cnt; i++)
			{
				/* Cell Temp */
				bms_state_data[pack_id].ebms_cell_th_wrn_st[i] = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
				ascii_data_len += 1;
			}
			/* En Temp */
			bms_state_data[pack_id].ebms_en_th_wrn_st = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* MOS Temp */
			bms_state_data[pack_id].ebms_mos_th_wrn_st = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* Bat Current (10mA)  */
			bms_state_data[pack_id].ebms_cur_wrn_st = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* Pack Vol (10mV) */
			bms_state_data[pack_id].ebms_pack_vol_wrn_st = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* User define */
			ascii_data_len += 1;
			data_len_dst += DBMS_COM_USER_DEFINE_STATE_CNT * 1;
		}
		else
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		if((data_len >= data_len_dst) && (DBMS_RES_CODE_OK == rec_code_temp))
		{
			/* bal event */
			bms_state_data[pack_id].sbms_bal_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* vol event */
			bms_state_data[pack_id].sbms_vol_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* temp event */
			bms_state_data[pack_id].sbms_th_evt.WORD = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1]);
			ascii_data_len += 2;
			/* current event */
			bms_state_data[pack_id].sbms_cur_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* other event */
			bms_state_data[pack_id].sbms_cap_other_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* switch event */
			bms_state_data[pack_id].sbms_sw_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			/* cb event */
			bms_state_data[pack_id].sbms_cb_evt.WORD =  (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1]);
			ascii_data_len += 2;
			/* system state */
			if(0x01 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
			{
				bms_state_data[pack_id].ebms_st = DBMS_CHG;
			}
			else if(0x02 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
			{
				bms_state_data[pack_id].ebms_st = DBMS_DISCHG;
			}
			else if(0x10 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
			{
				bms_state_data[pack_id].ebms_st = DBMS_STANDBY;
			}
			else
			{
				bms_state_data[pack_id].ebms_st = DBMS_UNDEFINE;
			}
			ascii_data_len += 1;
		}
		else
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/*  0x47 */
void	BMS_COM_get_bms_param_data_from_res_g(USHORT req_len, UCHAR *req_data, SBMS_PARAM *sbms_param, EBMS_COM_RES_CODE *res_code)
{
	UCHAR i;
	USHORT ascii_data_len = 0;
	UCHAR rec_code_temp;
	USHORT data_len;
	//USHORT data_len_dst;
	UCHAR pack_id;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_BMS_PARAM, req_len, req_data);
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < DBMS_COM_BMS_PARAM_LEN_MIN)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* do nothing */
		}
	}
	
	for(pack_id = 0; (pack_id < sbms_sys_com_rcv.byte.data[0]) && (DBMS_RES_CODE_OK == rec_code_temp); pack_id++)
	{
		for(i = 0; i< DBMS_VAL_PAEAM_END; i++)
		{
			sbms_param[pack_id].si16_val_param[i] = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
			if(6 == i)
			{
				ascii_data_len++;
			}
			else
			{
				/* do nothing */
			}
		}
		/* user define count */
		ascii_data_len++;
		/* function switch */
		ascii_data_len++;
		sbms_param[pack_id].ui8_vol_fun_sw.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		sbms_param[pack_id].ui8_th_fun_sw1.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		sbms_param[pack_id].ui8_th_fun_sw2.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		sbms_param[pack_id].ui8_cur_fun_sw.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		sbms_param[pack_id].ui8_cap_other_fun_sw.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		sbms_param[pack_id].ui8_balance_fun_sw.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		sbms_param[pack_id].ui8_indication_fun_sw.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		for(i = 0; i< DBMS_PROT_BOARD_NUM; i++)
		{
			sbms_param[pack_id].ui8_board_num[i] = sbms_sys_com_rcv.byte.data[ascii_data_len++];
		}
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/*  0x51 */
void	BMS_COM_get_prot_ver_from_res_g(USHORT req_len, UCHAR *req_data, USHORT *prot_ver, EBMS_COM_RES_CODE *res_code)
{
	UCHAR rec_code_temp;
	USHORT data_len;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_BMS_COM_PROT_VER, req_len, req_data);
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < DBMS_COM_PROT_VER_LEN)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			*prot_ver = sbms_sys_com_rcv.byte.ver;
			UI8_prot_ver = sbms_sys_com_rcv.byte.ver;
		}
	}
	else
	{
		/* do nothing */
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/*  0x4F */
void	BMS_COM_get_product_info_from_res_g(USHORT req_len, UCHAR *req_data,SBMS_PRODUCT_INFO *sbms_product_info, EBMS_COM_RES_CODE *res_code)
{
	UCHAR rec_code_temp;
	USHORT data_len;
	UCHAR i;
	USHORT ascii_data_len = 0;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_PRODUCT_INFO, req_len, req_data);
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < (DBMS_DEV_INFO + DBMS_SW_VER_INFO + DBMS_MAKER_INFO))
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			for(i = 0; i < DBMS_DEV_INFO; i++)
			{
				sbms_product_info->ui8_device_info[i] = sbms_sys_com_rcv.byte.data[ascii_data_len++];
			}
			for(i = 0; i < DBMS_SW_VER_INFO; i++)
			{
				sbms_product_info->ui8_software_ver_info[i] = sbms_sys_com_rcv.byte.data[ascii_data_len++];
			}
			for(i = 0; i < DBMS_MAKER_INFO; i++)
			{
				sbms_product_info->ui8_maker_info[i] = sbms_sys_com_rcv.byte.data[ascii_data_len++];
			}
		}
	}
	else
	{
		/* do nothing */
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/*  0x4B */
void	BMS_COM_get_log_data_from_res_g(USHORT req_len, UCHAR *req_data,SBMS_LOG_DATA *sbms_log_data, EBMS_COM_RES_CODE *res_code)
{
	UCHAR i;
	USHORT ascii_data_len = 0;
	UCHAR rec_code_temp;
//	USHORT data_len;
	//USHORT data_len_dst;
	UCHAR pack_id;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_LOG_DATA, req_len, req_data);
#if 0
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < DBMS_COM_MONITOR_DATA_LEN_MIN)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* do nothing */
		}
	}
#endif
	for(pack_id = 0; (pack_id < sbms_sys_com_rcv.byte.data[0]) && (DBMS_RES_CODE_OK == rec_code_temp); pack_id++)
	{
		/* last record log data flg */
		sbms_log_data[pack_id].ui8_next_data_exist_flg = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		/* log date */
		sbms_log_data[pack_id].st_log_reg_date.ui16_year = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
		ascii_data_len += 2;
		sbms_log_data[pack_id].st_log_reg_date.ui8_month = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		sbms_log_data[pack_id].st_log_reg_date.ui8_day = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		sbms_log_data[pack_id].st_log_reg_date.ui8_hour = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		sbms_log_data[pack_id].st_log_reg_date.ui8_minute = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		sbms_log_data[pack_id].st_log_reg_date.ui8_second = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		/* system state */
		if(0x01 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
		{
			sbms_log_data[pack_id].ebms_st = DBMS_CHG;
		}
		else if(0x02 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
		{
			sbms_log_data[pack_id].ebms_st = DBMS_DISCHG;
		}
		else if(0x10 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
		{
			sbms_log_data[pack_id].ebms_st = DBMS_STANDBY;
		}
		else
		{
			sbms_log_data[pack_id].ebms_st = DBMS_UNDEFINE;
		}
		ascii_data_len += 1;
		/* wrn len */
		ascii_data_len += 1;
		/* bal event */
		if(0x10 == sbms_sys_com_rcv.byte.data[ascii_data_len + 1])
		{
			sbms_log_data[pack_id].cell_invalid_flg = 0x01;
		}
		else
		{
			sbms_log_data[pack_id].cell_invalid_flg = 0x00;
		}
		ascii_data_len += 1;
		/* vol event */
		sbms_log_data[pack_id].sbms_vol_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		/* temp event */
		sbms_log_data[pack_id].sbms_th_evt.WORD = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1]);
		ascii_data_len += 2;
		/* current event */
		sbms_log_data[pack_id].sbms_cur_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		/* other event */
		sbms_log_data[pack_id].sbms_cap_other_evt.BYTE = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
		ascii_data_len += 1;
		/* val len */
		ascii_data_len += 1;
		/* Bat Current (10mA)  */
		sbms_log_data[pack_id].si16_cur = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
		ascii_data_len += 2;
		/* Pack Vol (10mV) */
		sbms_log_data[pack_id].ui16_pack_vol = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
		ascii_data_len += 2;
		/* Bat Remainder Capacity (10mAH) */
		sbms_log_data[pack_id].ui16_pack_remain_cap = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
		ascii_data_len += 2;
		/* temp Count */
		ascii_data_len += 1;
		sbms_log_data[pack_id].ui8_cell_th_cnt = sbms_sys_com_rcv.byte.data[ascii_data_len];
		/* temp Count check */
		if(sbms_log_data[pack_id].ui8_cell_th_cnt > DBMS_CELL_TH_CNT)
		{
			sbms_log_data[pack_id].ui8_cell_th_cnt = DBMS_CELL_TH_CNT;
		}
		else
		{
			/* do nothing */
		}
		/* Temp */
		for(i = 0;i < sbms_log_data[pack_id].ui8_cell_th_cnt; i++)
		{
			/* Cell Temp */
			sbms_log_data[pack_id].si16_cell_th[i] = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
		}
		/* En Temp */
		sbms_log_data[pack_id].si16_en_th = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
		ascii_data_len += 2;
		/* MOS Temp */
		sbms_log_data[pack_id].si16_mos_th = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
		ascii_data_len += 2;
		/* Battery Count */
		ascii_data_len += 1;
		sbms_log_data[pack_id].ui8_cell_series = sbms_sys_com_rcv.byte.data[ascii_data_len];
		/* Battery Count check */
		if(sbms_log_data[pack_id].ui8_cell_series > DBMS_CELL_SERIES_MAX)
		{
			sbms_log_data[pack_id].ui8_cell_series = DBMS_CELL_SERIES_MAX;
		}
		else
		{
			/* do nothing */
		}
		/* Cell1~13 Vol(mV) */
		for(i = 0;i < sbms_log_data[pack_id].ui8_cell_series; i++)
		{
			/* Cell Vol */
			sbms_log_data[pack_id].ui16_cell_vol[i] = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 2]);
			ascii_data_len += 2;
		}
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/*  0x4D */
void	BMS_COM_get_curr_date_from_res_g(USHORT req_len, UCHAR *req_data,SBMS_DATE *sbms_date, EBMS_COM_RES_CODE *res_code)
{
	USHORT ascii_data_len = 0;
	UCHAR rec_code_temp;
	USHORT data_len;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_GET_CURR_DATE, req_len, req_data);
	if(DBMS_RES_CODE_OK == rec_code_temp)
	{
		data_len = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		if(data_len < DBMS_COM_GET_CURR_DATE_LEN)
		{
			rec_code_temp = DBMS_RES_CODE_LENGTH_ERR;
		}
		else
		{
			/* log date */
			sbms_date->ui16_year = (((USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len] << 8) + (USHORT)sbms_sys_com_rcv.byte.data[ascii_data_len + 1]);
			ascii_data_len += 2;
			sbms_date->ui8_month = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			sbms_date->ui8_day = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			sbms_date->ui8_hour = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			sbms_date->ui8_minute = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
			sbms_date->ui8_second = sbms_sys_com_rcv.byte.data[ascii_data_len + 1];
			ascii_data_len += 1;
		}
	}
	else
	{
		/* do nothing */
	}
	/* res CODE */
	*res_code = rec_code_temp;
}
/* 0x45,0x49,0x4E,0xA0,0xA1,0xA2  */
void	BMS_COM_get_res_common_data_from_res_g(USHORT req_len, UCHAR *req_data, EBMS_COM_RES_CODE *res_code)
{
	UCHAR rec_code_temp;
	
	rec_code_temp = COM_RevFormatCheck(DBMS_CMD_CTRL, req_len, req_data);
	/* res CODE */
	*res_code = rec_code_temp;
}
/****************************************************************************
Function name	: COM_RevFormatCheck
Content 		: interpreting and executing received request
Argument		: none
Return value		: none
Date/Author		: 2017.05.16	PSDCD@Lifanghseng 
Change log		: 
*****************************************************************************/
static UCHAR COM_RevFormatCheck(EBMS_COM_CMD req_cmd_id, USHORT req_len, UCHAR *req_data)
{
	USHORT i;
	UCHAR char_temp;
	USHORT ascii_data_len;
	USHORT check_sum = 0;
	UCHAR lcheck_sum = 0;
	UCHAR rev_data_chk_ret;
	USHORT lenid;
	USHORT rev_chk_sum;
	
	rev_data_chk_ret = DBMS_RES_CODE_OK;
	/* ASCII count > FREAM size except DATA  */
	if(req_len >= ((DBMS_COM_FRAME_CTRL_DATA_MAX - 1) * 2))
	{
		/* HEX count -> ASCII count  expect SOI and EOI */
		ascii_data_len = (req_len - 2)/2 + 2;
		/* SOI */
		sbms_sys_com_rcv.byte.soi = req_data[0];
		/* version(1) ~ data info  data convert except SOI  CheckSumH CheckSumL EOI*/
		for(i = 1; i < ascii_data_len - 3 ;i++)
		{
			char_temp = COM_HexToChar(req_data[i*2 -1]);
			sbms_sys_com_rcv.all[i] = (char_temp << 4) & 0xF0;
			char_temp = COM_HexToChar(req_data[i*2]);
			sbms_sys_com_rcv.all[i] |= (char_temp & 0x0F);
		}
		/* Check sum */
		char_temp = COM_HexToChar(req_data[i*2 - 1]);
		sbms_sys_com_rcv.byte.chksum_h = (char_temp << 4) & 0xF0;
		char_temp = COM_HexToChar(req_data[i*2]);
		sbms_sys_com_rcv.byte.chksum_h |= (char_temp & 0x0F);
		char_temp = COM_HexToChar(req_data[i*2 + 1]);
		sbms_sys_com_rcv.byte.chksum_l = (char_temp << 4) & 0xF0;
		char_temp = COM_HexToChar(req_data[i*2 + 2]);
		sbms_sys_com_rcv.byte.chksum_l |= (char_temp & 0x0F);
		/* EOI */
		sbms_sys_com_rcv.byte.eoi =  req_data[req_len - 1];
		
		/* L check sum check */
		COM_Lchksum((ascii_data_len - DBMS_COM_FRAME_CTRL_DATA_MAX)*2, &lcheck_sum);
		/* info length check  */
		lenid = ((((USHORT)sbms_sys_com_rcv.byte.lchecksum) << 8) & 0x0F00) + (USHORT)sbms_sys_com_rcv.byte.lenid;
		/* check sum check */
		COM_Chksum((UCHAR*)req_data, lenid, &check_sum);
		rev_chk_sum = ((USHORT)sbms_sys_com_rcv.byte.chksum_h << 8) + (USHORT)sbms_sys_com_rcv.byte.chksum_l;

		/* start check	*/
		if(sbms_sys_com_rcv.byte.soi != DBMS_COM_FRAME_SOI)
		{	
			rev_data_chk_ret = DBMS_RES_CODE_DATA_FORMAT_ERR;
		}
		/* version check  */
		else if((DBMS_CMD_GET_BMS_COM_PROT_VER != req_cmd_id) && (sbms_sys_com_rcv.byte.ver != UI8_prot_ver))
		{ 
			rev_data_chk_ret = DBMS_RES_CODE_PRO_NUM_ERR;
		}
		/* address check  */
		else if(sbms_sys_com_rcv.byte.adr != DBMS_COM_FRAME_ADR)
		{ 
			rev_data_chk_ret = DBMS_RES_CODE_ADR_ERR;
		}
		/* cid1 check  */
		else if(sbms_sys_com_rcv.byte.cid1 != DBMS_COM_FRAME_CID1)
		{ 
			rev_data_chk_ret = DBMS_RES_CODE_DATA_FORMAT_ERR;
		}
		/* cid2 check  */
		else if(sbms_sys_com_rcv.byte.cid2 != DBMS_RES_CODE_OK)
		{ 
			rev_data_chk_ret = sbms_sys_com_rcv.byte.cid2;
		}
		/* lchksum check  */
		else if((sbms_sys_com_rcv.byte.lchecksum >> 4) != lcheck_sum)
		{ 
			rev_data_chk_ret = DBMS_RES_CODE_CHKSUM_ERR;
		}
		/* length check  */
		else if(lenid != (ascii_data_len - DBMS_COM_FRAME_CTRL_DATA_MAX)*2)
		{
			rev_data_chk_ret = DBMS_RES_CODE_LENGTH_ERR;
		}
		/* chksum check  */
		else if(rev_chk_sum != check_sum)
		{	
			rev_data_chk_ret = DBMS_RES_CODE_CHKSUM_ERR;
		}
		/* eoi check */
		else if(sbms_sys_com_rcv.byte.eoi != DBMS_COM_FRAME_EOI)
		{ 
			rev_data_chk_ret = DBMS_RES_CODE_DATA_FORMAT_ERR;
		}
		else
		{
			rev_data_chk_ret = DBMS_RES_CODE_OK;
		}
	}
	else
	{
		rev_data_chk_ret = DBMS_RES_CODE_LENGTH_ERR;
	}
	return rev_data_chk_ret;
}
/****************************************************************************
Function name	: COM_Chksum
Content			: checksum calculation
Argument		: none
Return value		: none
Date/Author		: 2017.03.16	PSDCD@Lifanghseng 
Change log		: 
*****************************************************************************/
static void COM_Chksum(UCHAR *buffer_ptr, USHORT info_length, USHORT* chksum_result)
{
	DWORD chksum_temp; //CHKSUM
	DWORD chksum_cnt; 

	chksum_temp=0;
	for(chksum_cnt = 1; chksum_cnt <= info_length + 12; chksum_cnt++)
	{
		chksum_temp+=buffer_ptr[chksum_cnt];
	}
	chksum_temp %= 65536; 
	chksum_temp = ~chksum_temp + 1; 
	*chksum_result = (USHORT)(chksum_temp & 0x0000FFFF); 
}
/****************************************************************************
Function name	: COM_Lchksum
Content			: checksum calculation
Argument		: none
Return value		: none
Date/Author		: 2017.03.16	PSDCD@Lifanghseng 
Change log		: 
*****************************************************************************/
static void COM_Lchksum(USHORT linfo_length, UCHAR * lchksum_result)
{
	USHORT lchksum_temp;
	
	lchksum_temp = (linfo_length&0x000F) + ((linfo_length>>4)&0x000F) + ( (linfo_length>>8)&0x000F );
	lchksum_temp %= 16;
	lchksum_temp = ~lchksum_temp + 1;
	*lchksum_result = (UCHAR)(lchksum_temp&0x000F);
}
/****************************************************************************
Function name	: COM_Lchksum
Content			: ACSII change to 16 hex
Argument		: none
Return value		: none
Date/Author		: 2017.05.16	PSDCD@Lifanghseng 
Change log		: 
*****************************************************************************/
static UCHAR COM_CharToHex(UCHAR bHex)
{
	if((bHex>=0)&&(bHex<=9))
	{
		bHex += 0x30;
	}
	else if((bHex>=10)&&(bHex<=15))//Capital
	{
		bHex += 0x37;
	}
	else 
	{
		bHex = 0xff;
	}
	return bHex;
}
/****************************************************************************
Function name	: COM_Lchksum
Content			: 16 hex change to ACSII 
Argument		: none
Return value		: none
Date/Author		: 2017.05.16	PSDCD@Lifanghseng 
Change log		: 
*****************************************************************************/
static UCHAR COM_HexToChar(UCHAR bChar)
{
	if((bChar>=0x30)&&(bChar<=0x39))
	{
		bChar -= 0x30;
	}
	else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
	{
		bChar -= 0x37;
	}
	else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
	{
		bChar -= 0x57;
	}
	else 
	{
		bChar = 0xff;
	}
	return bChar;
}
