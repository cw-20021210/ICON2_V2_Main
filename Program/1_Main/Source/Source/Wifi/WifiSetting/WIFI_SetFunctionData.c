
/*******************************************************************************
&Description :
    A1010 ~ A101x
    Function Table
  
&Histiry : 
    ShinHM 2021.04.13 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetFunctionData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxFunc ( U16 mId, I8 *pBuf );
static void EvtWifiRxFunc ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
#ifdef MODEL_NO_1
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0024_HEART_TIME4,             NULL,           NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_003A_FREEZE_BURST,            EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0044_FILTER_STATUS2,          EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      NULL,           NULL            },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           NULL,           NULL            },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        NULL,           NULL            },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,	        NULL	        }
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }
};
#endif

#ifdef MODEL_NO_2
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,  		    NULL   		    },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,  		    NULL   		    },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};
#endif

#ifdef MODEL_NO_3
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1  // icon 2.0 rx function ??????_ssg
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
        //-------------------------------------------20260223--------------------------JHKO
    //{   WIFI_FUNC_003B_HOT_TEMP_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,  EvtWifiRxFunc   },
        //-------------------------------------------20260223--------------------------JHKO
        
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
            //-------------------------------------------20260223--------------------------JHKO
    //{   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      NULL,  EvtWifiRxFunc   },
            //-------------------------------------------20260223--------------------------JHKO
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           NULL,           NULL            },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
                //-------------------------------------------20260223--------------------------JHKO
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        NULL,  EvtWifiRxFunc   },
       // {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
                //-------------------------------------------20260223--------------------------JHKO
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,           NULL            },
    {	WIFI_FUNC_0058_ICE_SIZE_SEL,	        NULL,           NULL            },
    {	WIFI_FUNC_0059_ICE_ONOFF_SEL,	        NULL,           NULL            },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    //-------------------------------------------20260223--------------------------JHKO
   {   WIFI_FUNC_00B5_FILTER_FLUSHING,     EvtWifiTxFunc,  NULL   }, //
   {   WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,     EvtWifiTxFunc,  NULL   }, //
    //-------------------------------------------20260201--------------------------
    {   WIFI_FUNC_0073_WATER_TEMP_SORT_SEL,     EvtWifiTxFunc,  EvtWifiRxFunc   }, //Set hot water temperature alignment (upward order : 0, descending order : 1)
    {   WIFI_FUNC_007F_DEFAULT_TEMP_SET,        EvtWifiTxFunc,  EvtWifiRxFunc   }, //Default hot water temperature setting status (temperature)
    {   WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //45℃ Enable Settings
    {   WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //50℃ Enable Settings
    {   WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //55℃ Enable Settings
    {   WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //60℃ Enable Settings
    {   WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //65℃ Enable Settings
    {   WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //70℃ Enable Settings
    {   WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //75℃ Enable Settings
    {   WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //80℃ Enable Settings
    {   WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //85℃ Enable Settings
    {   WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //90℃ Enable Settings
    {   WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //95℃ Enable Settings
    {   WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,	EvtWifiTxFunc,  EvtWifiRxFunc   }, //100℃ Enable Settings
    {   WIFI_FUNC_011E_HOT_TEMP_MIN_COUNT,	EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0120_HOT_TEMP_MAX_COUNT, 	EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0129_AMOUNT_SLOT1_ENABLE,	EvtWifiTxFunc,  EvtWifiRxFunc   },   //Capacity Setting Slots 1 On/Off
    {   WIFI_FUNC_012A_AMOUNT_SLOT2_ENABLE,	EvtWifiTxFunc,  EvtWifiRxFunc   },   //Capacity Setting Slots 2 On/Off
    {   WIFI_FUNC_012B_AMOUNT_SLOT3_ENABLE,	EvtWifiTxFunc,  EvtWifiRxFunc   },   //Capacity Setting Slots 3 On/Off
    {   WIFI_FUNC_0132_START_WATER_QUANTITY_SLOT, EvtWifiTxFunc,           EvtWifiRxFunc    },
    {   WIFI_FUNC_0133_START_WATER_QUANTITY_SEL_1,EvtWifiTxFunc,  EvtWifiRxFunc   }, //Capacity Setting Slots 120 ~ 1000(ml)
    {   WIFI_FUNC_0134_START_WATER_QUANTITY_SEL_2,EvtWifiTxFunc,  EvtWifiRxFunc   }, //Capacity Setting Slots 120 ~ 1000(ml)
    {   WIFI_FUNC_0135_START_WATER_QUANTITY_SEL_3,EvtWifiTxFunc,  EvtWifiRxFunc   },  //Capacity Setting Slots 120 ~ 1000(ml)
    {   WIFI_FUNC_013C_START_WATER_QUANTITY_SEL_10,EvtWifiTxFunc,  EvtWifiRxFunc   }  //Capacity Setting Slots 120 ~ 1000(ml)
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
            //-------------------------------------------20260223--------------------------JHKO
    //{   WIFI_FUNC_003B_HOT_TEMP_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,  EvtWifiRxFunc   },
        //-------------------------------------------20260223--------------------------JHKO
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      NULL,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           NULL,           NULL            },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        NULL,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,           NULL            },
    {	WIFI_FUNC_0058_ICE_SIZE_SEL,	        NULL,           NULL            },
    {	WIFI_FUNC_0059_ICE_ONOFF_SEL,	        NULL,           NULL            },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      EvtWifiTxFunc,  EvtWifiRxFunc   },
	    //-------------------------------------------20260223--------------------------JHKO
   {   WIFI_FUNC_00B5_FILTER_FLUSHING,     EvtWifiTxFunc,  NULL   }, //
   {   WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,     EvtWifiTxFunc,  NULL   }, //
     //-------------------------------------------20260201--------------------------
    {   WIFI_FUNC_0129_AMOUNT_SLOT1_ENABLE,	EvtWifiTxFunc,  EvtWifiRxFunc            },   //Capacity Setting Slots 1 On/Off
    {   WIFI_FUNC_012A_AMOUNT_SLOT2_ENABLE,	EvtWifiTxFunc,  EvtWifiRxFunc            },   //Capacity Setting Slots 2 On/Off
    {   WIFI_FUNC_012B_AMOUNT_SLOT3_ENABLE,	EvtWifiTxFunc,  EvtWifiRxFunc            },   //Capacity Setting Slots 3 On/Off
    {   WIFI_FUNC_0132_START_WATER_QUANTITY_SLOT, EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0133_START_WATER_QUANTITY_SEL_1,EvtWifiTxFunc,  EvtWifiRxFunc   }, //Capacity Setting Slots 120 ~ 1000(ml)
    {   WIFI_FUNC_0134_START_WATER_QUANTITY_SEL_2,EvtWifiTxFunc,  EvtWifiRxFunc   }, //Capacity Setting Slots 120 ~ 1000(ml)
    {   WIFI_FUNC_0135_START_WATER_QUANTITY_SEL_3,EvtWifiTxFunc,  EvtWifiRxFunc   },  //Capacity Setting Slots 120 ~ 1000(ml)
    {   WIFI_FUNC_013C_START_WATER_QUANTITY_SEL_10,EvtWifiTxFunc,  EvtWifiRxFunc   }  //Capacity Setting Slots 120 ~ 1000(ml)
};
#endif

#ifdef MODEL_NO_4
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET       NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};
#endif

#define SZ_FN_LIST ( sizeof(WifiFuncList) / sizeof(WifiTxFuncList_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiFuncEventList ( void )
{
    if ( IsUserModelCHP() == TRUE )
    {
        return WifiFuncList;
    }
    else
    {
        return WifiFuncListCp;
    }
}

U16 GetWifiFuncListSize ( void )
{
    return SZ_FN_LIST;
}

// Request ID 재사용 예외처리
U8 IsExceptId ( U16 mId )
{
    if ( mId == WIFI_FUNC_0040_SMART_CHECK_RQST )
    {
        return TRUE;
    }
    return FALSE;
}


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxFunc ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];

    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf) );

    ConvertDataHexToAscii( TYPE_FUNC_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}

static void EvtWifiRxFunc ( U16 mId, I8 *pBuf )
{
    ApplyFunctionData( mId, pBuf );
}

