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
#define		DBMS_COM_DATA_MAX	(170)						                /* BMS�ʐM�f�[�^�̈� MAX byte  */
#define		DBMS_COM_FRAME_CTRL_DATA_MAX	(10)						    /* �t���[������f�[�^ byte  */
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
	DBMS_RES_CODE_OK = 0,							/* �ʐM����	*/
	DBMS_RES_CODE_PRO_NUM_ERR,						/* �ʐM�K�i�o�W���[���G���[	*/
	DBMS_RES_CODE_CHKSUM_ERR,						/* �`�F�b�N�T���x���t�@�C */
	DBMS_RES_CODE_LENGTH_ERR,						/* �f�[�^���G���[	*/
	DBMS_RES_CODE_CMD_NO_SUPPORT,					/* �R�����g�T�|�b�g���Ȃ�	*/
	DBMS_RES_CODE_DATA_FORMAT_ERR,					/* �f�[�^�t�H�[�}�b�g�G���[	*/
	DBMS_RES_CODE_DATA_INVALID,						/* �f�[�^����	*/
	DBMS_RES_CODE_ADR_ERR,						    /* �A�h���X�G���[	*/
	DBMS_RES_CODE_MEM_DEV_ERR,						/* �������A�N�Z�X�G���[	*/
	DBMS_RES_CODE_END						        
}EBMS_COM_RES_CODE;

typedef enum {
	DBMS_CMD_GET_BMS_MONITOR_DATA = 0x42,					/* �V�X�e�����j�^�f�[�^�擾	*/
	DBMS_CMD_GET_BMS_STATE_DATA = 0x44,						/* �V�X�e����Ԏ擾	*/
	DBMS_CMD_CTRL = 0x45,									/* �V�X�e���R���g���[��	*/
	DBMS_CMD_GET_BMS_PARAM = 0x47,							/* �V�X�e���p�����[�^�擾	*/
	DBMS_CMD_SET_BMS_PARAM = 0x49,							/* �V�X�e���p�����[�^�ݒ�	*/
	DBMS_CMD_GET_BMS_COM_PROT_VER = 0x4F,					/* �ʐM�K�i�o�W���[���擾	*/
	DBMS_CMD_GET_PRODUCT_INFO = 0x51,						/* �v���_�N�V�������擾	*/
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
    DBMS_WRN_DET_CELL_VOL_MAX = 0x00,    /* �P��:mv *//* �P�̉ߏ[���[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CELL_VOL_MAX,    /* �P��:mv */
    DBMS_ERR_DET_CELL_OV_DLY,     /* �P��:S */
    DBMS_ERR_CELL_OV_REL_VOL,    /* �P��:mv */   /* �P�̉ߏ[�ی���� */       
    DBMS_WRN_DET_CELL_VOL_MIN,    /* �P��:mv *//* �P�̉ߕ����[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CELL_VOL_MIN,    /* �P��:mv */
    DBMS_ERR_DET_CELL_UV_DLY,     /* �P��:S */
    DBMS_ERR_CELL_UV_REL_VOL,     /* �P��:mv *//* �P�̉ߕ��ی���� */
    DBMS_WRN_DET_PACK_VOL_MAX,    /* �P��:10mv *//* ���̉ߏ[���[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_PACK_VOL_MAX,    /* �P��:10mv */
    DBMS_ERR_DET_PACK_OV_DLY,     /* �P��:S */
    DBMS_ERR_PACK_OV_REL_VOL,     /* �P��:10mv *//* ���̉ߏ[�ی���� */
    DBMS_WRN_DET_PACK_VOL_MIN,    /* �P��:10mv *//* ���̉ߕ����[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_PACK_VOL_MIN,    /* �P��:10mv */
    DBMS_ERR_DET_PACK_UV_DLY,     /* �P��:S */
    DBMS_ERR_PACK_UV_REL_VOL,     /* �P��:10mv *//* ���̉ߕ��ی���� */
    DBMS_WRN_DET_CHG_CUR_MAX,    /* �P��:A *//* �[�d�d���ߍ����[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CHG_CUR_MAX,	/* �P��:A */
    DBMS_ERR_DET_CHG_OC_DLY,	/* �P��:S */
    DBMS_WRN_DET_DISCHG_CUR_MAX,    /* �P��:A *//* ���d�d���ߍ�1���[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_DISCHG_CUR_MAX,    /* �P��:A */
    DBMS_ERR_DET_DISCHG_OC_DLY, 	/* �P��:S */
    DBMS_ERR_DET_DISCHG_CUR_MAX2,      /* �P��:A */  /* ���d�d���ߍ�2�ی쌟�o */ 
    DBMS_ERR_DET_DISCHG_OC_DLY2,		/* �P��:mS */
    DBMS_ERR_OC_REL_DLY,				 /* �P��:S */     /* �d���ی�����f�B���C */  
    DBMS_ERR_DET_CUR_LOCK_CNT,			/* �P��: 0.1�� */ /* �d���ی샍�b�N�� */
    DBMS_WRN_DET_MOS_TEMP_MAX,   		 /* �P��: 0.1�� *//* MOS���x�ߍ����[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_MOS_TEMP_MAX,   		 /* �P��: 0.1�� */
    DBMS_ERR_MOS_TEMP_REL_TEMP,        /* �P��: 0.1�� */ /* MOS���x�ߍ��ی���� */
    DBMS_WRN_DET_CELL_CHG_TEMP_MIN,         /* �P��: 0.1�� *//* �Z���[�d���x�ߒ჏�[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CELL_CHG_TEMP_MIN,    /* �P��: 0.1�� */
    DBMS_ERR_CELL_CHG_UT_REL_TEMP,		/* �P��: 0.1�� *//* �Z���[�d���x�ߒ�ی���� */
    DBMS_WRN_DET_CELL_CHG_TEMP_MAX,    /* �P��: 0.1�� */ /* �Z���[�d���x�ߍ����[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CELL_CHG_TEMP_MAX,    /* �P��: 0.1�� */
    DBMS_ERR_CELL_CHG_OT_REL_TEMP,        /* �P��: 0.1�� */ /* �Z���[�d���x�ߍ��ی���� */
    DBMS_WRN_DET_CELL_DISCHG_TEMP_MIN,      /* �P��: 0.1�� */ /* �Z�����d���x�ߒ჏�[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CELL_DISCHG_TEMP_MIN,    /* �P��: 0.1�� */
    DBMS_ERR_CELL_DISCHG_UT_REL_TEMP,       /* �P��: 0.1�� *//* �Z�����d���x�ߒ�ی���� */
    DBMS_WRN_DET_CELL_DISCHG_TEMP_MAX,    /* �P��: 0.1�� *//* �Z�����d���x�ߍ����[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_CELL_DISCHG_TEMP_MAX,    /* �P��: 0.1�� */
    DBMS_ERR_CELL_DISCHG_OT_REL_TEMP,    /* �P��: 0.1�� *//* �Z�����d���x�ߍ��ی���� */
    DBMS_WRN_DET_EN_TEMP_MIN,              /* �P��: 0.1�� */ /* �����x�ߒ჏�[�j���O�ƕی쌟�o */
    DBMS_ERR_DET_EN_TEMP_MIN,  		  /* �P��: 0.1�� */
    DBMS_ERR_EN_UT_REL_TEMP,           /* �P��: 0.1�� */ /* �����x�ߒ�ی���� */
    DBMS_WRN_DET_EN_TEMP_MAX,              /* �P��: 0.1�� */ /* �����x�ߍ����[�j���O�ƕی� */
    DBMS_ERR_DET_EN_TEMP_MAX,   		 /* �P��: 0.1�� */
    DBMS_ERR_EN_OT_REL_TEMP,            /* �P��: 0.1�� *//* �����x�ߍ��ی���� */
    DBMS_ERR_DET_CELL_MAX_MIN_DIF,   	 /* �P��:mv *//* �Z�������ی쌟�o */
    DBMS_ERR_CELL_INVAILD_REL_VOL_DIF,	/* �P��:mv *//* �Z�������ی���� */
    DBMS_WRN_DET_SOC_MIN,                   /* �P��:1% *//* SOC�ߒ჏�[�j���O���o */
    DBMS_CHG_SOC_RECOVER,				/* SOC�[�d�񕜗e��(%) */
    DBMS_BALANCE_ST_VOL,				/* �P��:mv *//* �o�����X�J�n�d�� (�P�ʁFmV�j */
    DBMS_BALANCE_ST_VOL_DIFF,			/* �P��:mv *//* �o�����X�J�n����(�P�ʁFmV�j */
    DBMS_BALANCE_END_VOL_DIFF,			/* �P��:mv *//* �o�����X�I������(�P�ʁFmV�j */
    DBMS_PACK_DESIGN_CAPACITY,			/* �P��:10mAH *//* �p�b�N���̗e��(10mAh) */
    DBMS_PACK_REMAINDER_CAP,			/* �P��:10mAH *//* �p�b�N�c���̗e��(10mAh) */
    DBMS_CELL_HEAT_ST_TH,				/* �P��: 0.1�� *//* �Z�����M�J�n���x(0.1C) */
    DBMS_CELL_HEAT_END_TH,				/* �P��: 0.1�� *//* �Z�����M��~���x(0.1C)  */
    DBMS_CHG_CUR_LIM,					/* �P��: A *//* �[�d�����l(�P�ʁF10mA�j */
    DBMS_STANDBY_TO_SLEEP_TIME,			/* �P��:H *//* �ҋ@�\>�X���b�v�̎���(h)*/
	DBMS_VAL_PAEAM_END,					/* ���l�p�����[�^�I�� */
    DBMS_VOL_FUN_SW = DBMS_VAL_PAEAM_END,					/* �d���@�\SW */
    DBMS_TH_FUN_SW1,					/* ���x�@�\SW1 */
    DBMS_TH_FUN_SW2,					/* ���x�@�\SW2 */
    DBMS_CUR_FUN_SW,					/* �d���@�\SW */
    DBMS_CAP_OTHER_FUN_SW,				/* �e�ʂƑ��̋@�\ */
    DBMS_BALANCE_FUN_SW,				/* �o�����X�@�\SW */
    DBMS_INDICATION_FUN_SW,				/* �w���@�\SW */
    DBMS_BOARD_NUM,						/* �ی�Board�ԍ� */
	DBMS_ALL_PARAM 						/* �S��param */
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
Function:���[�j���O���o�ƕی쌟�o�E�����p�����[�^�[
Date: 2017/03/10 PSDCD@Lifanghseng 
*****************************************************/
typedef struct {
	SHORT si16_val_param[DBMS_VAL_PAEAM_END];
#if 0
	/* �P�̉ߏ[���[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_cell_vol_max;			/* �Z���d���ߍ����[�j���O臒l(�P�ʁFmV�j */
	SHORT	si16_err_cell_vol_max;			/* �Z���d���ߍ��ی�臒l(�P�ʁFmV�j */
	USHORT	ui16_err_cell_ov_dly;				/* �Z���d���ߍ��ی�f�B���C臒l(�P�ʁF250ms�j*/
	/* �P�̉ߏ[���[�j���O�ƕی���� */
	SHORT	si16_err_cell_ov_rel_vol;	    /* �Z���d���ߍ��ی�����d��臒l(�P�ʁFmV�j */
	/* �P�̉ߕ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_cell_vol_min;		    /* �Z���d���ߒ჏�[�j���O臒l(�P�ʁFmV�j*/
	SHORT	si16_err_cell_vol_min;		    /* �Z���d���ߒ�ی�臒l(�P�ʁFmV�j*/
	USHORT	ui16_err_cell_uv_dly;		    /* �Z���d���ߒ�ی�f�B���C臒l (�P�ʁFs�j*/
	/* �P�̉ߕ����[�j���O�ƕی���� */
	SHORT	si16_err_cell_uv_rel_vol;	    /* �Z���d���ߒ�ی�����d��臒l(�P�ʁFmV�j */
	/* ���̉ߏ[���[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_pack_vol_max;			/* �p�b�N�d���ߍ����[�j���O臒l(�P�ʁF10mV�j*/
	SHORT	si16_err_pack_vol_max;			/* �p�b�N�d���ߍ��ی�臒l(�P�ʁF10mV�j*/
	USHORT	ui16_err_pack_ov_dly;			/* �p�b�N�d���ߍ��ی�f�B���C臒l(�P�ʁFs�j*/
	/* ���̉ߏ[�ی���� */
	SHORT	si16_err_pack_ov_rel_vol;		/* �p�b�N�d���ߍ��ی�����d��臒l(�P�ʁF10mV�j */
	/* ���̉ߕ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_pack_vol_min;			/* �p�b�N�d���ߒ჏�[�j���O臒l(�P�ʁF10mV�j */
	SHORT	si16_err_pack_vol_min;			/* �p�b�N�d���ߒ�ی�臒l(�P�ʁF10mV�j */
	USHORT	ui16_err_pack_uv_dly;			/* �p�b�N�d���ߒ�ی�f�B���C臒l (�P�ʁFs�j*/
	/* ���̉ߕ����[�j���O�ƕی���� */
	SHORT	si16_err_pack_uv_rel_vol;		/* �p�b�N�d���ߒ�ی�����d��臒l(�P�ʁF10mV�j */
	/* �[�d�d���ߍ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_chg_cur_max;			/* �[�d�d���ߍ����[�j���O臒l(�P�ʁF10mA�j*/
	SHORT	si16_err_chg_cur_max;			/* �[�d�d���ߍ��ی�臒l(�P�ʁF10mA�j*/
	USHORT	ui16_err_chg_oc_dly;			/* �[�d�d���ߍ��ی�f�B���C臒l(�P�ʁFs�j*/
	/* ���d�d���ߍ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_dischg_cur_max;		/* ���d�d���ߍ�1���[�j���O臒l(�P�ʁF10mA�j */
	SHORT	si16_err_dischg_cur_max;		/* ���d�d���ߍ�1�ی�臒l(�P�ʁF10mA�j */
	USHORT	ui16_err_dischg_oc_dly;			/* ���d�d���ߍ��ی�f�B���C臒l(�P�ʁF250ms�j*/
	/* ���d�d���ߍ�2�ی쌟�o */
	SHORT	si16_err_dischg_cur_max2;		/* ���d�d���ߍ�2�ی�臒l(�P�ʁF10mA�j */
	USHORT	ui16_err_dischg_oc_dly2;		/* ���d�d���ߍ�2�ی�f�B���C臒l(�P�ʁF10ms�j*/
	/* �d���ی�����f�B���C */
	USHORT	ui16_err_oc_rel_dly;		/* �d���ߍ��ی�����f�B���C臒l(�P�ʁFs�j*/
	/* �d���ی샍�b�N�� */
	SHORT   si16_oc_lock_cnt;
	/* MOS���x�ߍ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_mos_temp_max;			/* MOS���x�ߍ����[�j���O臒l(�P�ʁF0.1���j*/
	SHORT	si16_err_mos_temp_max;			/* MOS���x�ߍ��ی�臒l(�P�ʁF0.1���j*/
	/* MOS���x�ߍ��ی���� */
	SHORT	si16_err_mos_ot_rel_temp;		/* MOS���x�ߍ��ی�������x臒l(�P�ʁF0.1���j*/
	/* �Z���[�d���x�ߒ჏�[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_cell_chg_temp_min;     /* �Z���[�d�����x�ߒ჏�[�j���O臒l(�P�ʁF0.1���j */
	SHORT	si16_err_cell_chg_temp_min;		/* �Z���[�d�����x�ߒ�ی�臒l(�P�ʁF0.1���j */
	/* �Z���[�d���x�ߒ჏�[�j���O�ƕی���� */
	SHORT	si16_err_cell_chg_ut_rel_temp;	/* �Z���[�d�����x�ߒ�ی����臒l(�P�ʁF0.1���j */
	/* �Z���[�d���x�ߍ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_cell_chg_temp_max;		/* �Z���[�d�����x�ߍ����[�j���O臒l(�P�ʁF0.1���j */
	SHORT	si16_err_cell_chg_temp_max;		/* �Z���[�d�����x�ߍ��ی�臒l(�P�ʁF0.1���j */
	/* �Z���[�d���x�ߍ����[�j���O�ƕی���� */
	SHORT	si16_err_cell_chg_ot_rel_temp;	/* �Z���[�d�����x�ߍ��ی����臒l(�P�ʁF0.1���j */
	/* �Z�����d���x�ߒ჏�[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_cell_dischg_temp_min;  /* �Z�����d�����x�ߒ჏�[�j���O臒l(�P�ʁF0.1���j */
	SHORT	si16_err_cell_dischg_temp_min;	/* �Z�����d�����x�ߒ�ی�臒l(�P�ʁF0.1���j */
	/* �Z�����d���x�ߒ჏�[�j���O�ƕی���� */
	SHORT	si16_err_cell_dischg_ut_rel_temp;/* �Z�����d�����x�ߒ�ی����臒l(�P�ʁF0.1���j */
	/* �Z�����d���x�ߍ����[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_cell_dischg_temp_max;	 /* �Z�����d�����x�ߍ����[�j���O臒l(�P�ʁF0.1���j */
	SHORT	si16_err_cell_dischg_temp_max;   /* �Z�����d�����x�ߍ��ی�臒l(�P�ʁF0.1���j */
	/* �Z�����d���x�ߍ����[�j���O�ƕی���� */
	SHORT	si16_err_cell_dischg_ot_rel_temp;/* �Z�����d�����x�ߍ��ی����臒l(�P�ʁF0.1���j */
	/* �����x�ߒ჏�[�j���O�ƕی쌟�o */
	SHORT	si16_wrn_en_temp_min;            /* �����x�ߒ჏�[�j���O臒l(�P�ʁF0.1���j */
	SHORT	si16_err_en_temp_min;			 /* �����x�ߒ�ی�臒l(�P�ʁF0.1���j */
	/* �����x�ߒ�ی���� */
	SHORT	si16_err_en_ut_rel_temp;	/* �����x�ߒ�ی����臒l(�P�ʁF0.1���j */
	/* �����x�ߍ����[�j���O�ƕی� */
	SHORT	si16_wrn_en_temp_max;            /* �����x�ߍ����[�j���O臒l(�P�ʁF0.1���j */
	SHORT	si16_err_en_temp_max;			 /* �����x�ߍ��ی�臒l(�P�ʁF0.1���j */
	/* �����x�ߍ��ی���� */
	SHORT	si16_err_en_ot_rel_temp;	     /* �����x�ߍ��ی����臒l(�P�ʁF0.1���j */
	/* �Z�������ی쌟�o */
	SHORT	si16_err_cell_max_min_dif;		 /* �Z�������ی�臒l(�P�ʁFmV�j */
	/* �Z�������ی���� */
	SHORT	si16_err_cell_invaild_rel_vol_dif;		 /* �Z�������ی�臒l(�P�ʁFmV�j */
	/* SOC�ߒ჏�[�j���O���o */
	SHORT	si16_wrn_soc_min;                /* SOC�ߒ჏�[�j���O臒l */
	/* SOC�[�d�񕜗e�� */
	SHORT	si16_chg_soc_recover;                /* SOC�ߒ჏�[�j���O臒l */
	/* �o�����X�J�n�d��(mV) */
   	USHORT	ui16_balance_st_vol;
    /* �o�����X�J�n����(mV) */
   	USHORT	ui16_balance_st_vol_diff;
	/* �o�����X�I������(mV) */
   	USHORT	ui16_balance_end_vol_diff;
	/* �e�� */
	USHORT	ui16_pack_design_capacity;		/* �p�b�N���̗e��(10mAh) */
    /* �c�e�� */
	USHORT	ui16_pack_remainder_cap;			/* �p�b�N���̗e��(10mAh) */
	/* �Z�����M�J�n���x(0.1C) */
	SHORT   si16_cell_heat_st_th;
	/* �Z�����M��~���x(0.1C) */
	SHORT   si16_cell_heat_end_th;
	 /* �[�d�����l(A) */
	SHORT   si16_chg_limit_cur;
	 /* �ҋ@�X���b�v����(H) */
	USHORT   ui16_standby_to_shtdw_time;
#endif
	/* �d���@�\SW */
   	SBMS_VOL_FUN_SW	ui8_vol_fun_sw;
	/* ���x�@�\SW1 */
   	SBMS_TEMP_FUN_SW1	ui8_th_fun_sw1;
	/* ���x�@�\SW2 */
   	SBMS_TEMP_FUN_SW2	ui8_th_fun_sw2;
	/* �d���@�\SW */
   	SBMS_CUR_FUN_SW	ui8_cur_fun_sw;
	/* �e�ʂƑ��̋@�\ */
   	SBMS_CAP_OTHER_FUN_SW	ui8_cap_other_fun_sw;
	/* �o�����X�@�\SW */
   	SBMS_BALANCE_FUN_SW	ui8_balance_fun_sw;
	/* �w���@�\SW */
   	SBMS_INDICATION_FUN_SW	ui8_indication_fun_sw;
	/* �ی�Board�ԍ� */
   	UCHAR	ui8_board_num[DBMS_PROT_BOARD_NUM];
}SBMS_PARAM;
/****************************************************
Function:����
Date: 2017/04/10 PSDCD@Lifanghseng 
*****************************************************/
typedef struct {
	USHORT		ui16_year;  /* �N */
	UCHAR		ui8_month;  /* �� */
	UCHAR		ui8_day;      /* �� */
	UCHAR		ui8_hour;     /* ���� */
	UCHAR		ui8_minute;   /* �� */
	UCHAR		ui8_second;  /* �b */
}SBMS_DATE;
/****************************************************
Function:BMS�^�s�p�����[�^�[
Date: 2017/03/10 PSDCD@Lifanghseng 
*****************************************************/
/* BMS�Ď��f�[�^ */
typedef struct
{
	UCHAR		ui8_cell_series;
	USHORT		ui16_cell_vol[DBMS_CELL_SERIES_MAX];	/* 1mV */
	UCHAR		ui8_cell_th_cnt;
	SHORT		si16_cell_th[DBMS_CELL_TH_CNT];	/* �P�ʁF0.1K (0.1K = 0.1�� + 2731) */
	SHORT		si16_en_th;						/* �P�ʁF0.1K (0.1K = 0.1�� + 2731) */
	SHORT		si16_mos_th;					/* �P�ʁF0.1K (0.1K = 0.1�� + 2731) */
	SHORT		si16_cur;						/* 10mA */
	USHORT		ui16_pack_vol;					/* 10mV */
	USHORT		ui16_pack_remain_cap;
	USHORT		ui16_pack_cap;
	UCHAR		ui16_cyc_cnt;
}SBMS_MONITOR_DATA;
/* BMS��ԃf�[�^ */
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
/* BMS�ݔ��f�[�^ */
typedef struct
{
	UCHAR					ui8_device_info[DBMS_DEV_INFO];
	UCHAR					ui8_software_ver_info[DBMS_SW_VER_INFO];
	UCHAR					ui8_maker_info[DBMS_MAKER_INFO];
}SBMS_PRODUCT_INFO;
/* BMS LOG�L�^�f�[�^ */
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
	SHORT					si16_cell_th[DBMS_CELL_TH_CNT];	/* �P�ʁF0.1K (0.1K = 0.1�� + 2731) */
	SHORT					si16_en_th;		/* �P�ʁF0.1K (0.1K = 0.1�� + 2731) */
	SHORT					si16_mos_th;	/* �P�ʁF0.1K (0.1K = 0.1�� + 2731) */
	UCHAR					ui8_cell_series;
	USHORT					ui16_cell_vol[DBMS_CELL_SERIES_MAX];	/* 1mV */
}SBMS_LOG_DATA;
/****************************************************************************
 Prototype declaration
*****************************************************************************/
/***************************BMS�ʐM���N�G�X�g�擾***************************/
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
/***************************BMS�ʐM���X�|���X�擾***************************/
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

