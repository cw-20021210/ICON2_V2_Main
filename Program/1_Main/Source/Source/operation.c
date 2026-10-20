/// @file     watermeter.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    유량센서 동작 제어 file
#define OPERATION_C
#include "include/_include.h"
#include "operation.h"

static void FirstWaterSelectOperation(void);
//static void WaterSelectOperation(void);
static void Operation_Common(void);
static void Water_Cup_SelectTimer(U8* address);
static void Cold_ONOFF_Setting(void);
static void Hot_Lock_Setting(void);
static void PureWater_Button(void);
static void HotWater_Button(void);
static void ColdWater_Button(void);
static void Flushing_Check(void);


Operation_StepTable Operation_Step[Z_OperationStepNum_Max] =
{
	//	스텝 No		Segment			추출LED			온수LED			정수LED			냉수LED			용량LED			온수잠금LED		CUP LED			냉수온도LED		WIFI LED		파우셋 LED		필터 교체 LED		mL LED			℃ LED			살균 LED      	UV LED			Step Func.
	{ Z_OperationStep_00, 	SEG_BOOT,		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT,		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT, 		LED_BOOT,		LED_BOOT,		OperationStep_00},
	{ Z_OperationStep_01, 	SEG_VER,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_01},
#ifndef _AFTER_LPP_IMDCHANGE_
	{ Z_OperationStep_02, 	SEG_OFF,		LED_FADE_F, 		LED_OFF, 		LED_OFF, 		LED_FLU_READY, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_FADE_F,		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_02},
	{ Z_OperationStep_03, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_ON, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_03},
	{ Z_OperationStep_04, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_ON, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_04},
	{ Z_OperationStep_05, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_ON, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_05},
#else
	{ Z_OperationStep_02, 	SEG_OFF,		LED_FADE_F, 		LED_OFF, 		LED_OFF, 		LED_FLU_READY, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_02},
//@033	{ Z_OperationStep_03, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_03},
	{ Z_OperationStep_03, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_WIFI_READY, 	LED_WIFI_READY, 	LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_03},		//@033
//@033	{ Z_OperationStep_04, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_04},
	{ Z_OperationStep_04, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_WIFI_READY, 	LED_WIFI_READY, 	LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_04},		//@033
//@033	{ Z_OperationStep_05, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_05},
	{ Z_OperationStep_05, 	SEG_FLUSHING,		LED_FADE_F_S, 		LED_OFF, 		LED_WIFI_READY, 	LED_WIFI_READY, 	LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_05},		//@033
#endif
//	{ Z_OperationStep_10, 	SEG_ML,			LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 	LED_STATUS,		LED_STATUS,		LED_STATUS,		LED_forSEG, 	LED_forSEG, 	LED_OFF,		LED_OFF,		OperationStep_10},
//	{ Z_OperationStep_15, 	SEG_ON_FLIC,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_FADE_F, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_FADE_F,	 	LED_STATUS,		LED_STATUS, 	LED_STATUS, 	LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_15},
//	{ Z_OperationStep_16, 	SEG_OFF_DISP,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_OFF_DISP, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_STATUS, 	LED_STATUS, 	LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_16},
//	{ Z_OperationStep_17, 	SEG_ONOFF_MOVE,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_STATUS, 	LED_STATUS,		LED_STATUS, 	LED_STATUS, 	LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_17},
//	{ Z_OperationStep_18, 	SEG_OFF_FLIC,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_FADE_F, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_STATUS, 	LED_STATUS, 	LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_18},
	{ Z_OperationStep_10, 	SEG_ML,			LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS,		LED_STATUS,		LED_STATUS,		LED_forSEG, 	LED_forSEG, 	LED_OFF,		LED_OFF,		OperationStep_10},
	{ Z_OperationStep_15, 	SEG_ON_FLIC,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_FADE_F, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_FADE_F,	 	LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_15},
	{ Z_OperationStep_16, 	SEG_OFF_DISP,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_OFF_DISP, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_16},
	{ Z_OperationStep_17, 	SEG_ONOFF_MOVE,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_STATUS, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_17},
	{ Z_OperationStep_18, 	SEG_OFF_FLIC,		LED_ON, 		LED_STATUS, 		LED_STATUS, 		LED_FADE_F, 		LED_STATUS, 		LED_STATUS, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_18},
	{ Z_OperationStep_20, 	SEG_OFF,		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_20},
	{ Z_OperationStep_30, 	SEG_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_ON,			LED_UVCARE1,	OperationStep_30},
	{ Z_OperationStep_31, 	SEG_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_FADE_F_S,	LED_UVCARE2,	OperationStep_31},
	{ Z_OperationStep_40, 	SEG_OFF,		LED_DIMM, 		LED_ALL_LOCK, 		LED_ALL_LOCK,		LED_DIMM, 		LED_DIMM, 		LED_ALL_LOCK,		LED_OFF, 		LED_STATUS, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_40},
	{ Z_OperationStep_41, 	SEG_SOUND,		LED_OFF, 		LED_SOUND, 		LED_DIMM,		LED_DIMM, 		LED_SOUND, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_VOLUME,		OperationStep_41},
	{ Z_OperationStep_42, 	SEG_MANAGE,		LED_ON, 		LED_DIMM, 		LED_DIMM,		LED_DIMM, 		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_42},
	{ Z_OperationStep_50, 	SEG_EFF,		LED_EFF, 		LED_EFF, 		LED_EFF, 		LED_EFF, 		LED_STATUS, 		LED_STATUS, 		LED_EFF, 		LED_EFF, 		LED_STATUS,		LED_STATUS,		LED_STATUS, 	LED_forSEG, 	LED_forSEG,		LED_OFF,		LED_STATUS,		OperationStep_50},
	{ Z_OperationStep_70, 	SEG_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_FOTA,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		LED_OFF,		OperationStep_70},
	{ Z_OperationStep_80, 	SEG_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		LED_SMART,		OperationStep_80},
	{ Z_OperationStep_90, 	SEG_LINETEST,		LED_LINETEST, 		LED_LINETEST, 		LED_LINETEST, 		LED_LINETEST, 		LED_LINETEST, 	LED_LINETEST, 	LED_LINETEST, 	LED_LINETEST, 	LED_LINETEST,	LED_LINETEST, 		LED_LINETEST, 	LED_LINETEST, 	LED_LINETEST,	LED_LINETEST,	LED_LINETEST,	OperationStep_90},
	{ Z_OperationStep_91, 	SEG_PBATEST,		LED_PBATEST, 		LED_PBATEST, 		LED_PBATEST, 		LED_PBATEST, 		LED_PBATEST, 	LED_PBATEST, 	LED_PBATEST, 	LED_PBATEST, 	LED_PBATEST,	LED_PBATEST, 		LED_PBATEST, 	LED_PBATEST, 	LED_PBATEST,	LED_PBATEST,	LED_PBATEST,	OperationStep_91},
	{ Z_OperationStep_92, 	SEG_FRONTTEST,		LED_FRONTTEST, 		LED_FRONTTEST, 		LED_FRONTTEST, 		LED_FRONTTEST, 		LED_FRONTTEST, 	LED_FRONTTEST, 	LED_FRONTTEST, 	LED_FRONTTEST, 	LED_FRONTTEST,	LED_FRONTTEST, 	LED_FRONTTEST, 	LED_FRONTTEST, 	LED_FRONTTEST,	LED_FRONTTEST,	LED_FRONTTEST,	OperationStep_92},
	{ Z_OperationStep_93, 	SEG_LINETEST2,		LED_LINETEST2,		LED_LINETEST2, 		LED_LINETEST2, 		LED_LINETEST2, 		LED_LINETEST2, 	LED_LINETEST2, 	LED_LINETEST2, 	LED_LINETEST2, 	LED_LINETEST2, 	LED_LINETEST2,	LED_LINETEST2, 	LED_LINETEST2, 	LED_LINETEST2, 	LED_LINETEST2,	LED_LINETEST2,	OperationStep_93},
	{ Z_OperationStep_A0, 	SEG_COLDTIME,		LED_OFF, 		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_A0},
	{ Z_OperationStep_B0, 	SEG_TOPCOVER,		LED_OFF, 		LED_TOPCOVER,		LED_TOPCOVER, 		LED_TOPCOVER, 		LED_TOPCOVER, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_TOPCOVER,		LED_TOPCOVER, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_B0},
	{ Z_OperationStep_C0, 	SEG_OFF,		LED_ON, 		LED_DIMM, 		LED_OFF, 		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_COLDTANK,		OperationStep_C0},
	{ Z_OperationStep_D0, 	SEG_TH,			LED_FADE_F, 		LED_FADE_F,		LED_FADE_F, 		LED_FADE_F, 		LED_FADE_F, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_D0},
	{ Z_OperationStep_D1, 	SEG_MYSET,		LED_FADE_F, 		LED_DIMM,		LED_OFF, 		LED_OFF, 		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_D1},
	{ Z_OperationStep_E0, 	SEG_ERR_FLIC,		LED_ON, 		LED_DIMM, 		LED_DIMM, 		LED_DIMM, 		LED_DIMM, 		LED_STATUS, 		LED_OFF, 		LED_STATUS, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_E0},
	{ Z_OperationStep_E1, 	SEG_ERR_HIS,		LED_ON, 		LED_DIMM, 		LED_DIMM, 		LED_DIMM, 		LED_DIMM, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_E1},
	{ Z_OperationStep_F0, 	SEG_STE,		LED_FADE_F_S, 		LED_OFF, 		LED_STEMODE, 		LED_STEMODE, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_STATUS,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_F0},
	{ Z_OperationStep_F1, 	SEG_AD_FEEDBACK,	LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF, 		LED_OFF,		LED_OFF,		OperationStep_F1},
};

/// @brief
/// @param    void
/// @return   void
void Operation_Initialize(void)
{
	U8 i;

	gu16_CupSize = CUPSIZE_HALFCUP;      			//
	gu8_WaterSelect = COLD_WATER;      				//
	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
	}
	else											//	일본향
	{
		gu8_HotTempSetup = HOT_TEMP_SETUP_KOR70_JP70;
		gu8_DefaultHotTempSetup = HOT_TEMP_SETUP_KOR70_JP70;
	}

//	if (gu8_ModelType_forNational == MODEL_JAPAN)
//	{
//		F_HotLock = SET;
//	}

	F_CupSize_MySet = CLEAR;
	gu16_CupSize_My = MY_CUPSIZE_CC;

	gu8_TimerUV_6h = Z_TimerUV_6h;

	F_FirstFlushing = CLEAR;
	F_AllLock = CLEAR;
	F_HotLock = CLEAR;
	F_WIFI_First_Connecting = CLEAR;		//@033 플러싱 중 wifi 최초 1회 자동 연결 시도 여부 Flag CLear(미시도)

	gu8_TimerSleepMode = Z_TimerSleepMode;
	gu8_TimerReedSWCheck = Z_TimerReedSWCheck;
	gu16_TimerSilverCare = Z_TimerSilverCare;		//	실버케어 48시간

	gu8_Sound_Type = SOUND_TYPE_3;

	for (i = 0 ; i < 3 ; i++)
	{
		gu16_Effluent_Count[i] = 0;
		gu16_Effluent_Amount[i] = 0;
	}
#ifdef _EFF_Continue_Heater_OFF
    F_Eff_Continue_Heater_Off = CLEAR;
#endif
	Ini_OperationStep_00();
}

/// @brief
/// @param    void
/// @return   void
void Operation(void)
{
	if (gu8_OperationStepNum < Z_OperationStepNum_Max)	//	스텝 허용 범위 이내
	{
#ifdef _FOR_HOTWATER_1CUP_EFF_30s_DELAY_
		if ((gu8_ErrorCode == 0) && (KEY_EFFLUENT_SHORT == SET))
		{
			F_HotWater_1CUP_Eff ^= 1;
			gu8_TimerHotWater_1CUP_EFF = 3;
			Play_Voice(VOICE_BEEP_ON);
		}

		if ((F_HotWater_1CUP_Eff == SET) && (gu8_TimerHotWater_1CUP_EFF == 0) && (gu8_OperationStep != Z_OperationStep_50))
		{
			gu8_TimerHotWater_1CUP_EFF = 30;
			gu8_CupSize = CUPSIZE_1CUP;
			gu8_WaterSelect = HOT_WATER;
			Ini_OperationStep_50();
			Play_Voice(VOICE_EFFLUENT_START);
		}
		if (gu8_OperationStep == Z_OperationStep_50)
		{
			gu8_TimerHotWater_1CUP_EFF = 30;
		}
#endif

#ifdef _FOR_PURE_EFF_30s_REPEAT_
		if ((gu8_ErrorCode == 0) && (KEY_EFFLUENT_SHORT == SET))
		{
			F_HotWater_1CUP_Eff ^= 1;
			gu8_Timer_forDSC_RepeatEff_30min = 1;
			Play_Voice(VOICE_BEEP_ON);
		}

		if ((F_HotWater_1CUP_Eff == SET) && (gu8_Timer_forDSC_RepeatEff_30min == 0))
		{
			gu8_Timer_forDSC_RepeatEff_30min = 60;
			gu8_WaterSelect = PURE_WATER;
			KEY_EFFLUENT_SHORT = SET;
			gu8_CupSize = CUPSIZE_CONTINUE;
		}
		else if ((F_HotWater_1CUP_Eff == SET) && (gu8_Timer_forDSC_RepeatEff_30min < 30) && (gu8_OperationStep != Z_OperationStep_10))
		{
			Ini_OperationStep_10();
			KEY_EFFLUENT_SHORT = SET;
		}
		else
		{

		}
#endif

#ifdef _FOR_REPEAT_EFF_ENDURANCE_TEST_
		F_HotLock = CLEAR;
		if (F_Repeat_Eff == SET)
		{
#ifndef _FOR_WQA_
#ifdef _FOR_100DEGREE_TEST_
			if ((gu8_WaterSelect == HOT_WATER) && (gu8_HotStep_hi != 0x00))
			{
				gu16_CupSize = CUPSIZE_CONTINUE;
				gu8_DefaultCupSize = CUPSIZE_FIVE;
			}
			else
			{
				gu16_CupSize = CUPSIZE_HALFCUP;
				gu8_DefaultCupSize = CUPSIZE_ONE;
			}
#else
			gu8_CupSize = CUPSIZE_HALFCUP;
			gu8_DefaultCupSize = CUPSIZE_ONE;
#endif
#else
			gu16_CupSize = CUPSIZE_CONTINUE;
			gu8_DefaultCupSize = CUPSIZE_FIVE;
#endif
			gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;

			if ((gu8_OperationStep != Z_OperationStep_50) && (gu16_Timer_Repeat_Eff == 0) && (F_Repeat_StepChange == CLEAR))
			{
				if (gu8_ModelType_forCHP == MODEL_CHP)
				{
					if (++gu8_Counter_Repeat_Eff > 2)		//	0:냉수 1:정수 2:온수
					{
#ifdef _FOR_COLD_ONOFF_TEST_
						if (gu8_CoolingStep >= Z_CoolingStep_21)
						{
							gu8_Counter_Repeat_Eff = 0;
						}
						else
						{
							gu8_Counter_Repeat_Eff = 1;
						}
#else
						gu8_Counter_Repeat_Eff = 0;
#endif
					}
				}
				else
				{
					if (++gu8_Counter_Repeat_Eff > 1)		//	0:냉수 1:정수
					{
#ifdef _FOR_COLD_ONOFF_TEST_
						if (gu8_CoolingStep >= Z_CoolingStep_21)
						{
							gu8_Counter_Repeat_Eff = 0;
						}
						else
						{
							gu8_Counter_Repeat_Eff = 1;
						}
#else
						gu8_Counter_Repeat_Eff = 0;
#endif
					}
				}
				switch (gu8_Counter_Repeat_Eff)
				{
					case 0:								//	냉수
//					case 1:								//	냉수
						KEY_COLDWATER_SHORT = SET;
						break;
					case 2:								//	온수
						KEY_HOTWATER_SHORT = SET;
						break;
					case 1:								//	정수
//					case 4:								//	정수
						KEY_PUREWATER_SHORT = SET;
						break;
					default:
						break;
				}
				gu16_Timer_Repeat_Delay = 10;	//	1초
				F_Repeat_StepChange = SET;
			}

			if ((gu8_OperationStep != Z_OperationStep_50) && (gu16_Timer_Repeat_Eff == 0) && (gu16_Timer_Repeat_Delay == 0))
			{
				F_Repeat_StepChange = CLEAR;
				KEY_EFFLUENT_SHORT = SET;
			}
		}
#endif

		//@003	시작
		if (F_Repeat_Eff_Mode_forMP == SET)
		{
			if (F_Repeat_Eff_forMP == SET)
			{
				gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;

				if ((gu8_OperationStep != Z_OperationStep_50) && (gu16_Timer_Repeat_Eff_forMP == 0) && (F_Repeat_StepChange_forMP == CLEAR))
				{
					switch (gu8_Counter_Repeat_Eff_forMP)
					{
						case 0:								//	냉수
							KEY_COLDWATER_SHORT = SET;
							break;
						case 2:								//	온수
							KEY_HOTWATER_SHORT = SET;
							break;
						case 1:								//	정수
							KEY_PUREWATER_SHORT = SET;
							break;
						default:
							break;
					}
					gu16_Timer_Repeat_Delay_forMP = 15;	//	1.5초
					F_Repeat_StepChange_forMP = SET;

					if (gu8_ModelType_forCHP == MODEL_CHP)
					{
						if (++gu8_Counter_Repeat_Eff_forMP > 2)		//	0:냉수 1:정수 2:온수
						{
							gu8_Counter_Repeat_Eff_forMP = 0;
						}
					}
					else
					{
						if (++gu8_Counter_Repeat_Eff_forMP > 1)		//	0:냉수 1:정수
						{
							gu8_Counter_Repeat_Eff_forMP = 0;
						}
					}
				}

				if ((gu8_OperationStep != Z_OperationStep_50) && (gu16_Timer_Repeat_Eff_forMP == 0) && (gu16_Timer_Repeat_Delay_forMP == 0))
				{
					F_Repeat_StepChange_forMP = CLEAR;
					KEY_EFFLUENT_SHORT = SET;
				}
			}
		}
		//@003	끝

		(Operation_Step[gu8_OperationStepNum].exe)();	//		해당 스텝 함수 실행

#ifndef _FRONT_JIG_PROGRAM_
		if ((F_TestMode.byte == 0x00) && (gu8_TimerPBATest_Join != 0))	//		테스트모드가 아니고 전원 인가 후 30초 이내
		{
			if ((KEY_LINETEST_MANAGE_LONG == SET) && (INPUT_REED_SW_COVER == CLEAR))
			{
				if (ADC_RESULT_LEAKSENSOR <= Z_LeakSensor_Error)
				{
					F_EEPROM_Forced_Initialize = SET;	//		EEPROM 초기화 (라인 검사 중에 잘못 조작으로 검사모드 아닌 경우에 에러 발생한 경우에 에러이력 저장한 것을 지우기 위함)
					gu8_TimerPBATest_Join = 0;			//		Timer Clr
					F_TestMode_LineFront = SET;			//		테스트모드 설정
					gu16_TimerUV_Oper = Z_TimerUV_Oper;
					Play_Voice(VOICE_SPECIAL_FUNCTION);
					Ini_OperationStep_00();				//		부팅스텝
					WifiControlProcess(WIFI_TIME_FACTORY);
				}
				else
				{
					Play_Voice(VOICE_BEEP_WARNING);
				}
			}
			else if ((KEY_COLDTANK_PBATEST_LONG == SET) && (INPUT_REED_SW_COVER == CLEAR))
			{
				F_EEPROM_Forced_Initialize = SET;	//		EEPROM 초기화 (라인 검사 중에 잘못 조작으로 검사모드 아닌 경우에 에러 발생한 경우에 에러이력 저장한 것을 지우기 위함)
				gu8_TimerPBATest_Join = 0;			//		Timer Clr
				F_TestMode_PBA = SET;				//		테스트모드 설정
				Play_Voice(VOICE_SPECIAL_FUNCTION);
				Ini_OperationStep_00();				//		부팅스텝
				WifiControlProcess(WIFI_TIME_FACTORY);
			}
			else if (KEY_ERROR_HIS_LONG == SET)	//	에러 이력
			{
				gu8_TimerPBATest_Join = 0;			//		Timer Clr
				Play_Voice(VOICE_SPECIAL_FUNCTION);
				Ini_OperationStep_E1();
			}
			else
			{

			}
		}
		if (F_TestMode.byte == 0x00)
		{
			if  ((INPUT_REED_SW_COVER == SET) && (INPUT_REED_SW_FILTER == SET) && (INPUT_REED_SW_FAUCET == SET))
			{
				gu8_TimerReedSWCheck = Z_TimerReedSWCheck;
			}

			if ((F_SmartCheck == SET) && (gu8_OperationStep != Z_OperationStep_80)) 	//	자가진단
			{
				if ((gu8_OperationStep != Z_OperationStep_03) && (gu8_OperationStep != Z_OperationStep_04) && (gu8_OperationStep != Z_OperationStep_05)
					&& (gu8_OperationStep_hi != 0x50))
				{
					Play_Voice(VOICE_SMART_CHECK_START);
					Ini_OperationStep_80();
				}
				else
				{
					F_SmartCheck_Prohibit = SET;
					WifiSendData(WIFI_DATA_FUNCTION);
					Play_Voice(VOICE_BEEP_WARNING);
				}
			}
			else if ((gu8_OperationStep != Z_OperationStep_B0)	//	커버 열림 상태
				&& (gu8_OperationStep != Z_OperationStep_70)	//	포타 상태
				&& (F_FOTA_Display == SET))						//	포타 상태 인 경우
			{
				Ini_OperationStep_70();
			}
			else if ((gu8_OperationStep >= Z_OperationStep_02)
				&& (gu8_OperationStep != Z_OperationStep_80)
				&& (gu8_OperationStep != Z_OperationStep_B0)
				&& (gu8_OperationStep != Z_OperationStep_E0)	// 삭제해야하나 말아야하나
				&& (gu8_OperationStep != Z_OperationStep_F0)
				&& (gu8_OperationStep_hi != 0x90)	//	테스트모드
				&& (gu8_TimerReedSWCheck == 0))
			{
				if (INPUT_REED_SW_COVER == CLEAR)
				{
					Play_Voice(VOICE_TOP_COVER_WARNING_B);
				}
				else if (INPUT_REED_SW_FILTER == CLEAR)
				{
					Play_Voice(VOICE_FILTER_CONNECT_CHECK_B);
				}
				else
				{
					Play_Voice(VOICE_FLOWFAUCET_CONNECT_CHECK);
				}
				Ini_OperationStep_B0();
				gu16_HeartServiceTime = 0;
				WifiSendData(WIFI_DATA_FUNCTION);
			}
			else
			{

			}
		}
#endif

#ifdef _FOR_HEATER_ENDURANCE_TEST_
		if (gu8_ErrorCode != 0)
		{
			F_Heater_Endurance_Test = 0;
		}
		if ((F_Heater_Endurance_Test == 1) && (gu8_OperationStep != Z_OperationStep_30))
		{
			gu8_WaterSelect = HOT_WATER;
		}
#endif

		DisplayControl_Data[Z_SEGMENT].Operation_CMD = Operation_Step[gu8_OperationStepNum].SEGMENT;
		DisplayControl_Data[Z_LED_EFFLUENT].Operation_CMD = Operation_Step[gu8_OperationStepNum].EFFLUENT;
		DisplayControl_Data[Z_LED_HOT].Operation_CMD = Operation_Step[gu8_OperationStepNum].HOT_LED;
		DisplayControl_Data[Z_LED_PURE].Operation_CMD = Operation_Step[gu8_OperationStepNum].PURE_LED;
		DisplayControl_Data[Z_LED_COLD].Operation_CMD = Operation_Step[gu8_OperationStepNum].COLD_LED;
		DisplayControl_Data[Z_LED_AMOUNT].Operation_CMD = Operation_Step[gu8_OperationStepNum].AMOUNT_LED;
		DisplayControl_Data[Z_LED_HOTLOCK].Operation_CMD = Operation_Step[gu8_OperationStepNum].HOT_LOCK;
		DisplayControl_Data[Z_LED_COLD_BLUE].Operation_CMD = Operation_Step[gu8_OperationStepNum].COLD_TEMP_LED;
		DisplayControl_Data[Z_LED_COLD_WHITE].Operation_CMD = Operation_Step[gu8_OperationStepNum].COLD_TEMP_LED;
		DisplayControl_Data[Z_LED_2CUP].Operation_CMD = Operation_Step[gu8_OperationStepNum].CUP_LED;
		DisplayControl_Data[Z_LED_1CUP].Operation_CMD = Operation_Step[gu8_OperationStepNum].CUP_LED;
		DisplayControl_Data[Z_LED_CON].Operation_CMD = Operation_Step[gu8_OperationStepNum].CUP_LED;
		DisplayControl_Data[Z_LED_HALFCUP].Operation_CMD = Operation_Step[gu8_OperationStepNum].CUP_LED;
		if (gu8_ModelType_forNational == MODEL_JAPAN)	//	일본향은 %로 사용
		{
			if ((gu8_OperationStep >= Z_OperationStep_03) && (gu8_OperationStep <= Z_OperationStep_05))
			{
				DisplayControl_Data[Z_LED_WIFI].Operation_CMD = LED_forSEG;
			}
			else if (gu8_OperationStep != Z_OperationStep_00)
			{
				DisplayControl_Data[Z_LED_WIFI].Operation_CMD = LED_OFF;
			}
			else
			{

			}
		}
		else
		{
			DisplayControl_Data[Z_LED_WIFI].Operation_CMD = Operation_Step[gu8_OperationStepNum].WIFI_LED;
		}
		DisplayControl_Data[Z_LED_FILTER_CHANGE].Operation_CMD = Operation_Step[gu8_OperationStepNum].FILTER_CLEAN_LED;
		DisplayControl_Data[Z_LED_FAUCET].Operation_CMD = Operation_Step[gu8_OperationStepNum].CHANGE_ALRAM_LED;
		DisplayControl_Data[Z_LED_mL].Operation_CMD = Operation_Step[gu8_OperationStepNum].ML_LED;
		DisplayControl_Data[Z_LED_DEGREE].Operation_CMD = Operation_Step[gu8_OperationStepNum].DEGREE_LED;
		DisplayControl_Data[Z_LED_STE].Operation_CMD = Operation_Step[gu8_OperationStepNum].STE_LED;
		DisplayControl_Data[Z_LED_UV1].Operation_CMD = Operation_Step[gu8_OperationStepNum].UV_LED;
		DisplayControl_Data[Z_LED_UV2].Operation_CMD = Operation_Step[gu8_OperationStepNum].UV_LED;
		DisplayControl_Data[Z_LED_UV3].Operation_CMD = Operation_Step[gu8_OperationStepNum].UV_LED;
		DisplayControl_Data[Z_LED_UV4].Operation_CMD = Operation_Step[gu8_OperationStepNum].UV_LED;
		DisplayControl_Data[Z_LED_UV5].Operation_CMD = Operation_Step[gu8_OperationStepNum].UV_LED;

		gu8_DigitalInput_Key1.OffEdge.byte = 0x00;      //      Key data clear
		gu8_DigitalInput_Key1.OnEdge.byte = 0x00;       //      Key data clear

//#ifdef _100DEGREE_SET_
//		F_HotWater_100Degree = SET;
//#endif

#ifndef _FOR_DEVELOP_
	if ((F_HotWater_DegreeDisplay == SET) && (gu8_TimerHotWater_DegreeDisplay == 0))
	{
		F_HotWater_DegreeDisplay = CLEAR;
	}
#endif
	}
	else											//	스텝 허용범위 외
	{
		SafetyLoop();								//		안전정지 루프
	}
}

/// @brief    초기 전원 인가 (EEPROM 대기 및 전체 점멸)
/// @param    void
/// @return   void
void OperationStep_00(void)
{
#ifdef _FRONT_JIG_PROGRAM_
	if (F_ModelDecide_Complete == SET)
	{
		Ini_OperationStep_92();
	}
#else
	if (F_FirstEEPROM_Read == SET)
	{
		if ((F_Boot_forBuzzer == CLEAR) && (F_FOTA_Setup == CLEAR))
		{
#ifdef _RBK_VOICE_JIG_PROGRAM_
//			Play_Voice(VOICE_CHANGE_FLOWFAUCET);
			Play_Voice(VOICE_HOTWATER_FIRST);
#else
			Play_Voice(VOICE_POWER_ON);
#endif
		}
		F_Boot_forBuzzer = SET;
		F_FOTA_Setup = CLEAR;
	}
	else
	{
		gu16_TimerBootDisplay = Z_TimerBootDisplay;
	}

	if (gu16_TimerBootDisplay == 0)
	{       
		HotTemp_Setting();
		
		if (F_TestMode_LineFront == SET)
		{
			Ini_OperationStep_90();
		}
		else
		{
			Ini_OperationStep_01();
		}
	}
#endif
}
void Ini_OperationStep_00(void)
{
	if ((F_TestMode_LineFront == SET) || (F_TestMode_PBA == SET))
	{
		gu16_TimerBootDisplay = (Z_TimerBootDisplay/2);		//	부팅 타이머 설정
	}
	else
	{
		gu16_TimerBootDisplay = Z_TimerBootDisplay;		//	부팅 타이머 설정
	}

	OperationStepNoSet(Z_OperationStepNum_00);
}

/// @brief    부팅 중  (버전 표시)
/// @param    void
/// @return   void
void OperationStep_01(void)
{
	if (gu16_TimerVersionDisplay == 0)					//	부팅타이머 경과 시
	{
		if (F_TestMode_PBA == SET)
		{
			Ini_OperationStep_91();
		}
		else
		{
#ifdef _NO_FLUSHING_
			F_FirstFlushing = SET;
			F_Heater_Fill = SET;
			F_ColdTank_Fill = SET;
#endif
			if ((F_FirstFlushing == CLEAR) || (F_Heater_Fill == CLEAR) || (F_ColdTank_Fill == CLEAR))			//		플러싱 필요 라면
			{
				F_FirstFlushing = CLEAR;
				F_Heater_Fill = CLEAR;
				F_ColdTank_Fill = CLEAR;
				F_First_Set = CLEAR;
				Play_Voice(VOICE_INSTALL);
				Ini_OperationStep_02();				//			플러싱 대기
			}
//			if (F_First_Set == CLEAR)
//			{
//				F_FirstFlushing = CLEAR;
//				F_Heater_Fill = CLEAR;
//				F_ColdTank_Fill = CLEAR;
//				Play_Voice(VOICE_INSTALL);
//				Ini_OperationStep_02();				//			플러싱 대기
//			}
			else									//		플러싱 완료라면
			{
				if (F_AllLock == SET)
				{
					Ini_OperationStep_40();
				}
				else
				{
					FirstWaterSelectOperation();
				}
			}
		}
	}
}
void Ini_OperationStep_01(void)
{
	if (F_TestMode_PBA == SET)
	{
		gu16_TimerVersionDisplay = Z_TimerPBATestDisplay;
	}
	else
	{
		gu16_TimerVersionDisplay = Z_TimerVersionDisplay;
	}

	if (gu8_ModelType_forNational == MODEL_JAPAN)
	{
		F_HotLock = SET;
	}

	if ((gu8_ModelType_forNational == MODEL_KOREA) && (F_HotWater_100Degree == CLEAR))
	{
		gu8_HotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
		gu8_DefaultHotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
	}

	OperationStepNoSet(Z_OperationStepNum_01);
}

/// @brief    'FL'표시
/// @param    void
/// @return   void
void OperationStep_02(void)
{
	U8 mu8_WifiDisp = 0;

#ifdef _NO_FLUSHING_
		F_FirstFlushing = SET;
		F_ColdTank_Fill = SET;
#endif

	gu8_TimerSleepMode = Z_TimerSleepMode;
	gu8_TimerUV_6h = Z_TimerUV_6h;
	gu8_TimerUV_Delay = Z_TimerUV_Delay;

	if (KEY_FLU_CAN_TH_CON_LONG == SET)
	{
		F_FirstFlushing = SET;
		F_Heater_Fill = SET;
		F_ColdTank_Fill = SET;
		F_FilterChange_Detect = CLEAR;
		Play_Voice(VOICE_FLUSHING_COMPLETE);
		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_DONE_FOR_WIFI;  //JHKO
		WifiSendData(WIFI_DATA_FUNCTION);  //JHKO
	}
	else if (gu8_ErrorCode != 0)							//	에러 발생 시
	{
		Ini_OperationStep_E0();						//
	}
	else if ((F_FirstFlushing == SET) && (F_Heater_Fill == SET) && (F_ColdTank_Fill == SET)
			&& (F_FilterChange_Detect == CLEAR) && (F_FaucetChange_Detect == CLEAR))
	{
		Ini_OperationStep_10();						//
	}
	else if (KEY_EFFLUENT_SHORT == SET)
	{
		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_ON_FOR_WIFI;

		if (F_FilterChange_Detect == SET)
		{
			F_FirstFlushing = CLEAR;
		}

		if (F_FirstFlushing == CLEAR)
		{
			if (F_FilterChange_Detect == SET)
			{
				if (F_FaucetChange_Detect == SET)
				{
					Play_Voice(VOICE_FILTER_FLOWFAUCET_FLUSHING);
				}
				else
				{
					Play_Voice(VOICE_FILTER_FLUSHING);
				}
			}
			else
			{
				Play_Voice(VOICE_FLUSHING_START);
			}
			F_SmartCheck_Prohibit = SET;
			Ini_OperationStep_03();

		}
		else if (F_Heater_Fill == CLEAR)
		{
			F_SmartCheck_Prohibit = SET;
			Play_Voice(VOICE_FLUSHING_START);
			Ini_OperationStep_04();
		}
		else if (F_ColdTank_Fill == CLEAR)
		{
			F_SmartCheck_Prohibit = SET;
			Play_Voice(VOICE_FLUSHING_START);
			Ini_OperationStep_05();
		}
		else
		{
			Play_Voice(VOICE_BEEP_ON);
			Ini_OperationStep_10();
		}

		if (F_FaucetChange_Detect == SET)
		{
			F_FaucetChange_Detect = CLEAR;
			gu16_DayCount_forFaucet = CLEAR;
			F_FaucetChange_Display = CLEAR;
			mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_DISP);	//	Wi-Fi 연결상태 확인
			if (mu8_WifiDisp == WIFI_DISP_CONNECT)
			{
				F_Faucet_Reset = SET;
			}
		}

		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (((F_FilterChange_Detect == SET) || (F_FaucetChange_Detect == SET)) && (KEY_COLDWATER_SHORT == SET))
	{
		if (F_First_Set == SET)						//	초기 설치가 완료 된 경우만 set
		{
			F_FirstFlushing = SET;
		}
		F_FilterChange_Detect = CLEAR;
		F_FaucetChange_Detect = CLEAR;
		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_OFF_FOR_WIFI;		//	플러싱 취소시.......항상 송신.
		Ini_OperationStep_10();
		Play_Voice(VOICE_BEEP_ON);
	}
	else if (KEY_COLDTANK_PBATEST_VERYLONG == SET)
	{
		Ini_OperationStep_C0();
		Play_Voice(VOICE_BEEP_ON);
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
//		Play_Voice(VOICE_BEEP_WARNING);
		if (F_FilterChange_Detect == SET)
		{
			Play_Voice(VOICE_CHANGE_FILTER);
		}
		else if (F_FaucetChange_Detect == SET)
		{
			Play_Voice(VOICE_CHANGE_FLOWFAUCET);
		}
		else
		{
			Play_Voice(VOICE_INSTALL);
		}
	}
	else
	{

	}
}
void Ini_OperationStep_02(void)
{
	gu8_WaterSelect = PURE_WATER;
	OperationStepNoSet(Z_OperationStepNum_02);

	gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_ON_FOR_WIFI;     //	플러싱 대기에서도 ON 전송
}

/// @brief    플러싱 중
/// @param    void
/// @return   void
void OperationStep_03(void)
{
	static U8 mu8_FlusingVoice = CLEAR;
	U8 mu8_WifiDisp = 0;
//	static U8 F_FirstFlushing_AirPurge = CLEAR;

	if (gu8_ModelType_forCHP == MODEL_CP)
	{
		F_Heater_Fill = SET;
	}

//@033	if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_Timer10sAfter_WifiConnect_forFlushing == 0) && (F_First_Set == CLEAR) && (GetWifiStatusValue(WIFI_STATUS_DISP) == WIFI_DISP_OFF))	//	미연결 상태
	if ( (gu8_ModelType_forNational == MODEL_KOREA) && (gu8_Timer10sAfter_WifiConnect_forFlushing == 0) &&
	     (F_First_Set == CLEAR) && (GetWifiStatusValue(WIFI_STATUS_DISP) == WIFI_DISP_OFF) && (F_WIFI_First_Connecting == CLEAR) )	//@033	미연결 상태
	{
		WifiKey(WIFI_KEY_BLE);
		F_WIFI_First_Connecting = SET;	//@033 플러싱 중 wifi 최초 1회 자동 연결 시도 여부 Flag Set(1회 시도 완료)
	}

	if (F_First_Set == CLEAR)
	{
		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_GOING_FOR_WIFI;  //JHKO
		if ((gu8_TimerFirstFlushing_AirPurge != 0) && (F_FirstFlushing_AirPurge == CLEAR))	//	초기 설치 플러싱 시, 내부 잔여 질소로 인해 수압이 밀어내면서 유량센서가 빠르게 흘러 20%가 그냥 Pass되는 것을 방지하기 위함
		{
//			gu16_Effluent_Hz = (U16)((U32)Z_Flushing_Litter * 10000 / gu16_FlowMeter_Standard);	//	3L (플러싱 용량)
//			gu16_Flushing_Hz_forDisplay = gu16_Flushing_TotalHz_forDisplay;
		}
		else
		{
			F_FirstFlushing_AirPurge = SET;
		}
	}

	if (gu16_Flushing_Hz_forDisplay <= (gu16_Flushing_TotalHz_forDisplay/2))	//	플러싱 진행 50% 이상 경과 시
	{
		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_GOING_FOR_WIFI;  //JHKO
		if (mu8_FlusingVoice == CLEAR)
		{
			mu8_FlusingVoice = SET;
			if (F_First_Set == CLEAR)
			{
				Play_Voice(VOICE_FLUSHING_START);
			}
			else
			{
				Play_Voice(VOICE_FILTER_FLUSHING);
			}
		}
	}
	else
	{
		mu8_FlusingVoice = CLEAR;
	}

	if (gu16_Effluent_Hz == 0)						//	플러싱 완료
	{
		F_FirstFlushing = SET;
		if ((gu8_ModelType_forCHP == MODEL_CHP) && (F_Heater_Fill == CLEAR))
		{
			Ini_OperationStep_04();
		}
		else if (F_ColdTank_Fill == CLEAR)
		{
			Ini_OperationStep_05();
		}
		else
		{
			Ini_OperationStep_10();
			if (F_FilterChange_Detect == SET)
			{
				Play_Voice(VOICE_FILTER_SYSTEM_READY);
			}
			else
			{
				Play_Voice(VOICE_FLUSHING_COMPLETE);
			}
			gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_DONE_FOR_WIFI;  
		}
		gu16_DayCount_forFilter = CLEAR;
		F_FilterChange_Display = CLEAR;
		F_FilterChange_Detect = CLEAR;
		mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_DISP);	//	Wi-Fi 연결상태 확인
		if (mu8_WifiDisp == WIFI_DISP_CONNECT)
		{
			F_Filter_Reset = SET;
		}
	}
	else if (KEY_EFFLUENT_SHORT == SET)
	{
		Ini_OperationStep_02();
		StepMotor_ZeroBack_Open();
		Play_Voice(VOICE_EFFLUENT_COMPLETE);
	}
	else if (KEY_PUREWATER_SHORT == SET)												//@033
	{                                                                       			//@033
	/* 하기 Wifi 페어링을 위한 키 입력시 Play_Voice(VOICE_BEEP_WARNING) 출력 방지를 위해 조건문 추가 */ 	//@033
	}                                                                      	 			//@033
	else if (KEY_COLDWATER_SHORT == SET)                                    			//@033
	{                                                                       			//@033
	/* 하기 Wifi 페어링을 위한 키 입력시 Play_Voice(VOICE_BEEP_WARNING) 출력 방지를 위해 조건문 추가 */	//@033
	}                                                                       			//@033
	else if (KEY_WIFI_AP_PAIRING_LONG == SET)								//@033
	{                                                                       //@033
		if (gu8_ModelType_forNational == MODEL_KOREA)                       //@033
		{                                                                   //@033
			WifiKey(WIFI_KEY_AP);                                           //@033
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);          //@033
			                                                                //@033
			if (gu8_WifiError != 0)                                         //@033
			{                                                               //@033
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;        //@033
			}                                                               //@033
		}                                                                   //@033
		else                                                                //@033
		{                                                                   //@033
			Play_Voice(VOICE_BEEP_WARNING);                                 //@033
		}                                                                   //@033
	}                                                                       //@033
	else if (KEY_PUREWATER_LONG == SET)										//@033
	{                                                                       //@033
		if (gu8_ModelType_forNational == MODEL_KOREA)                       //@033
		{                                                                   //@033
			WifiKey(WIFI_KEY_BLE);                                          //@033
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);          //@033
			                                                                //@033
			if (gu8_WifiError != 0)                                         //@033
			{                                                               //@033
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;        //@033
			}                                                               //@033
		}                                                                   //@033
		else                                                                //@033
		{                                                                   //@033
			Play_Voice(VOICE_BEEP_WARNING);                                 //@033
		}                                                                   //@033
	}                                                                       //@033
	else if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else if (gu8_ErrorCode != 0)					//	에러 중인 경우
	{
		Ini_OperationStep_E0();
	}
	else
	{

	}
}
void Ini_OperationStep_03(void)
{
//@033	U16 buf = 0;

//@033	if (gu8_ModelType_forCHP == MODEL_CHP)
//@033	{
//@033		buf = (U16)((U32)(Z_HeaterFill_Litter+Z_ColdTankFill_Litter) * 10000 / gu16_FlowMeter_Standard);
//@033	}
//@033	else
//@033	{
//@033		buf = (U16)((U32)(Z_ColdTankFill_Litter) * 10000 / gu16_FlowMeter_Standard);
//@033	}

	gu8_TimerFlicker = Z_TimerFlicker;
	gu8_TimerEffluent_After_2s = Z_TimerEffluent_After_2s;

	if (gu16_Effluent_Hz == 0)
	{
//@029		gu16_Effluent_Hz = (U16)((U32)Z_Flushing_Litter * 10000 / gu16_FlowMeter_Standard);	//	3L (플러싱 용량)
		if (F_FilterChange_Detect == CLEAR)
		{
			gu16_Effluent_Hz = (U16)((U32)Z_Flushing_Litter_FirstSet * 10000 / gu16_FlowMeter_Standard);	//@029	2L (플러싱 용량)

			if (gu8_ModelType_forCHP == MODEL_CHP)
			{
//@033			gu16_Flushing_TotalHz_forDisplay = (U16)((U32)(Z_Flushing_Litter+Z_HeaterFill_Litter+Z_ColdTankFill_Litter) * 10000 / gu16_FlowMeter_Standard);	//	총 플러싱 용량 계산

				if(gu8_ModelType_forNational == MODEL_KOREA ){																														//@033 국내향인 경우
					gu16_Flushing_TotalHz_forDisplay = (U16)((U32)(Z_Flushing_Litter_FirstSet + Z_HeaterFill_Litter + Z_ColdTankFill_Litter_KR) * 10000 / gu16_FlowMeter_Standard);	//@033 총 플러싱 용량 계산 (냉수탱크5L)
				}																																									//@033
				else{																																								//@033 일본향인 경우
					gu16_Flushing_TotalHz_forDisplay = (U16)((U32)(Z_Flushing_Litter_FirstSet + Z_HeaterFill_Litter + Z_ColdTankFill_Litter_JP) * 10000 / gu16_FlowMeter_Standard);	//@033 총 플러싱 용량 계산 (냉수탱크4L)
				}																																									//@033
			}
			else
			{
//@033			gu16_Flushing_TotalHz_forDisplay = (U16)((U32)(Z_Flushing_Litter+Z_ColdTankFill_Litter) * 10000 / gu16_FlowMeter_Standard);	//	총 플러싱 용량 계산

				if(gu8_ModelType_forNational == MODEL_KOREA ){																									//@033 국내향인 경우
					gu16_Flushing_TotalHz_forDisplay = (U16)((U32)(Z_Flushing_Litter_FirstSet + Z_ColdTankFill_Litter_KR) * 10000 / gu16_FlowMeter_Standard);	//@033 총 플러싱 용량 계산 (냉수탱크5L)
				}																																				//@033
				else{																																			//@033 일본향인 경우
					gu16_Flushing_TotalHz_forDisplay = (U16)((U32)(Z_Flushing_Litter_FirstSet + Z_ColdTankFill_Litter_JP) * 10000 / gu16_FlowMeter_Standard);	//@033 총 플러싱 용량 계산 (냉수탱크4L)
				}																																				//@033
			}
		}
		else										//	필터 교체로 진입 된 경우
		{
			gu16_Effluent_Hz = (U16)((U32)Z_Flushing_Litter * 10000 / gu16_FlowMeter_Standard);	//@029	3L (플러싱 용량)
			gu16_Flushing_TotalHz_forDisplay = gu16_Effluent_Hz;	//	 플러싱 용량만 저장
		}
		gu16_Flushing_Hz_forDisplay = gu16_Flushing_TotalHz_forDisplay;
	}
	gu8_TimerFirstFlushing_AirPurge = Z_TimerFirstFlushing_AirPurge;	//	초기 설치 플러싱 시, 내부 잔여 질소로 인해 수압이 밀어내면서 유량센서가 빠르게 흘러 20%가 그냥 Pass되는 것을 방지하기 위함
	gu8_WaterSelect = PURE_WATER;
	gu8_Timer10sAfter_WifiConnect_forFlushing = Z_Timer10sAfter_WifiConnect_forFlushing;
	OperationStepNoSet(Z_OperationStepNum_03);
}

/// @brief    히터 물 채우기 및 전력체크
/// @param    void
/// @return   void
void OperationStep_04(void)
{
	if (gu16_Effluent_Hz == 0)		//	히터 물채우기 완료
	{
		F_Heater_Fill = SET;
		F_FirstHotEffluent = SET;
		Ini_OperationStep_05();
	}
	else if (F_Heater_Fill == SET)
	{
		Ini_OperationStep_05();
	}
	else if (KEY_EFFLUENT_SHORT == SET)
	{
		Ini_OperationStep_02();
		StepMotor_ZeroBack_Open();
		Play_Voice(VOICE_EFFLUENT_COMPLETE);
	}
	else if (KEY_PUREWATER_SHORT == SET)												//@033
	{                                                                       			//@033
	/* 하기 Wifi 페어링을 위한 키 입력시 Play_Voice(VOICE_BEEP_WARNING) 출력 방지를 위해 조건문 추가 */ 	//@033
	}                                                                      	 			//@033
	else if (KEY_COLDWATER_SHORT == SET)                                    			//@033
	{                                                                       			//@033
	/* 하기 Wifi 페어링을 위한 키 입력시 Play_Voice(VOICE_BEEP_WARNING) 출력 방지를 위해 조건문 추가 */	//@033
	}                                                                       			//@033
	else if (KEY_WIFI_AP_PAIRING_LONG == SET)								//@033
	{                                                                       //@033
		if (gu8_ModelType_forNational == MODEL_KOREA)                       //@033
		{                                                                   //@033
			WifiKey(WIFI_KEY_AP);                                           //@033
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);          //@033
			                                                                //@033
			if (gu8_WifiError != 0)                                         //@033
			{                                                               //@033
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;        //@033
			}                                                               //@033
		}                                                                   //@033
		else                                                                //@033
		{                                                                   //@033
			Play_Voice(VOICE_BEEP_WARNING);                                 //@033
		}                                                                   //@033
	}                                                                       //@033
	else if (KEY_PUREWATER_LONG == SET)										//@033
	{                                                                       //@033
		if (gu8_ModelType_forNational == MODEL_KOREA)                       //@033
		{                                                                   //@033
			WifiKey(WIFI_KEY_BLE);                                          //@033
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);          //@033
			                                                                //@033
			if (gu8_WifiError != 0)                                         //@033
			{                                                               //@033
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;        //@033
			}                                                               //@033
		}                                                                   //@033
		else                                                                //@033
		{                                                                   //@033
			Play_Voice(VOICE_BEEP_WARNING);                                 //@033
		}                                                                   //@033
	}                                                                       //@033
	else if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else if (gu8_ErrorCode != 0)					//	에러 중인 경우
	{
		Ini_OperationStep_E0();
	}
	else
	{

	}
}
void Ini_OperationStep_04(void)
{
	if (gu8_TimerFlicker == 0)
	{
		gu8_TimerFlicker = Z_TimerFlicker;
	}
	gu8_TimerEffluent_After_2s = Z_TimerEffluent_After_2s;

	gu8_WaterSelect = HOT_WATER;
	gu16_Effluent_Hz = (U16)((U32)Z_HeaterFill_Litter * 10000 / gu16_FlowMeter_Standard);	//

	F_WIFI_First_Connecting = CLEAR;	//@033 플러싱 중 wifi 최초 1회 자동 연결 시도 여부 Flag CLear(미시도)

	OperationStepNoSet(Z_OperationStepNum_04);
}

/// @brief    냉수 탱크 물 채우기
/// @param    void
/// @return   void
void OperationStep_05(void)
{
	if (gu16_Effluent_Hz == 0)		//	냉수탱크 물채우기 완료
	{
		F_ColdTank_Fill = SET;
		Ini_OperationStep_10();
		Play_Voice(VOICE_FLUSHING_COMPLETE);

		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_DONE_FOR_WIFI;   //  @004    설치 플러싱 완료
	}
	else if (F_ColdTank_Fill == SET)
	{
		Ini_OperationStep_10();
		Play_Voice(VOICE_FLUSHING_COMPLETE);
	}
	else if (KEY_EFFLUENT_SHORT == SET)
	{
		Ini_OperationStep_02();
		StepMotor_ZeroBack_Open();
		Play_Voice(VOICE_EFFLUENT_COMPLETE);
	}
	else if (KEY_PUREWATER_SHORT == SET)												//@033
	{                                                                       			//@033
	/* 하기 Wifi 페어링을 위한 키 입력시 Play_Voice(VOICE_BEEP_WARNING) 출력 방지를 위해 조건문 추가 */ 	//@033
	}                                                                      	 			//@033
	else if (KEY_COLDWATER_SHORT == SET)                                    			//@033
	{                                                                       			//@033
	/* 하기 Wifi 페어링을 위한 키 입력시 Play_Voice(VOICE_BEEP_WARNING) 출력 방지를 위해 조건문 추가 */	//@033
	}                                                                       			//@033
	else if (KEY_WIFI_AP_PAIRING_LONG == SET)								//@033
	{                                                                       //@033
		if (gu8_ModelType_forNational == MODEL_KOREA)                       //@033
		{                                                                   //@033
			WifiKey(WIFI_KEY_AP);                                           //@033
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);          //@033
			                                                                //@033
			if (gu8_WifiError != 0)                                         //@033
			{                                                               //@033
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;        //@033
			}                                                               //@033
		}                                                                   //@033
		else                                                                //@033
		{                                                                   //@033
			Play_Voice(VOICE_BEEP_WARNING);                                 //@033
		}                                                                   //@033
	}                                                                       //@033
	else if (KEY_PUREWATER_LONG == SET)										//@033
	{                                                                       //@033
		if (gu8_ModelType_forNational == MODEL_KOREA)                       //@033
		{                                                                   //@033
			WifiKey(WIFI_KEY_BLE);                                          //@033
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);          //@033
			                                                                //@033
			if (gu8_WifiError != 0)                                         //@033
			{                                                               //@033
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;        //@033
			}                                                               //@033
		}                                                                   //@033
		else                                                                //@033
		{                                                                   //@033
			Play_Voice(VOICE_BEEP_WARNING);                                 //@033
		}                                                                   //@033
	}                                                                       //@033
	else if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else if (gu8_ErrorCode != 0)					//	에러 중인 경우
	{
		Ini_OperationStep_E0();
	}
	else
	{

	}
}
void Ini_OperationStep_05(void)
{
	if (gu8_TimerFlicker == 0)
	{
		gu8_TimerFlicker = Z_TimerFlicker;
	}
	gu8_TimerEffluent_After_2s = Z_TimerEffluent_After_2s;

	gu8_WaterSelect = COLD_WATER;
	if (gu16_Effluent_Hz == 0)
	{
//@033	gu16_Effluent_Hz = (U16)((U32)Z_ColdTankFill_Litter * 10000 / gu16_FlowMeter_Standard);	//

		if(gu8_ModelType_forNational == MODEL_KOREA ){													//@033 국내향인 경우
			gu16_Effluent_Hz = (U16)((U32)Z_ColdTankFill_Litter_KR * 10000 / gu16_FlowMeter_Standard);	//@033 냉수탱크 플러싱 용량 (5L) 확정
		}																								//@033
		else{																							//@033 일본향인 경우
			gu16_Effluent_Hz = (U16)((U32)Z_ColdTankFill_Litter_JP * 10000 / gu16_FlowMeter_Standard);	//@033 냉수탱크 플러싱 용량 (4L) 확정
		}
	}
	OperationStepNoSet(Z_OperationStepNum_05);
}

/// @brief    정수 선택
/// @param    void
/// @return   void
void OperationStep_10(void)
{
	Operation_Common();
}
void Ini_OperationStep_10(void)
{
	F_First_Set = SET;
	F_KITMode = CLEAR;
//#ifndef _FOR_JAPAN_90DEGREE_CONTINUE
//	if ((gu8_ModelType_forNational == MODEL_JAPAN) && (gu8_WaterSelect == HOT_WATER) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))	//	일본향 90도는 반컵만 설정 가능
//	{
//		gu8_CupSize = CUPSIZE_HALFCUP;
//	}
//#endif
	Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
	OperationStepNoSet(Z_OperationStepNum_10);
}

/// @brief    ON 표시
/// @param    void
/// @return   void
void OperationStep_15(void)
{
	if (gu8_TimerOperation_100ms == 0)
	{
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		Ini_OperationStep_10();
	}
	else
	{

	}
}
void Ini_OperationStep_15(void)
{
	gu8_TimerOperation_100ms = Z_TimerOperation_100ms_2s;
	OperationStepNoSet(Z_OperationStepNum_15);
}

/// @brief    OFF 표시
/// @param    void
/// @return   void
void OperationStep_16(void)
{
	Operation_Common();

	if (gu8_TimerOperation_100ms == 0)
	{
		gu8_WaterSelect = PURE_WATER;
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		if (gu8_OperationStep == Z_OperationStep_16)
		{
			if (KEY_COLDWATER_SHORT == SET)
			{
				Ini_OperationStep_16();
			}
			else
			{
				Ini_OperationStep_10();
			}
		}
	}
}
void Ini_OperationStep_16(void)
{
	gu8_TimerOFF_Flic = Z_TimerOFF_Flic;
	gu8_TimerOperation_100ms = Z_TimerOperation_100ms_7s;
	OperationStepNoSet(Z_OperationStepNum_16);
}

/// @brief    무브 ON/OFF 설정 표시
/// @param    void
/// @return   void
void OperationStep_17(void)
{
	if (gu16_TimerMoveDisplay == 0)
	{
		F_SleepMode_Display = CLEAR;
		F_ColdOff_Display = CLEAR;
		Ini_OperationStep_10();
	}
	else
	{

	}
}
void Ini_OperationStep_17(void)
{
	gu16_TimerMoveDisplay = Z_TimerMoveDisplay;
	OperationStepNoSet(Z_OperationStepNum_17);
}

/// @brief    OFF 표시
/// @param    void
/// @return   void
void OperationStep_18(void)
{
	if (gu8_TimerOperation_100ms == 0)
	{
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		Ini_OperationStep_10();
	}
	else
	{

	}
}
void Ini_OperationStep_18(void)
{
	gu8_TimerOperation_100ms = Z_TimerOperation_100ms_2s;
	OperationStepNoSet(Z_OperationStepNum_18);
}

/// @brief    슬립모드
/// @param    void
/// @return   void
void OperationStep_20(void)
{
	if (gu8_Key_Short.byte != 0x00U)
	{
		gu8_TimerSleepMode = Z_TimerSleepMode;
		Ini_OperationStep_10();
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (gu8_TimerUV_6h == 0)
	{
		Ini_OperationStep_31();
	}
	else
	{

	}
}
void Ini_OperationStep_20(void)
{
	OperationStepNoSet(Z_OperationStepNum_20);
}

/// @brief    UV케어
/// @param    void
/// @return   void
void OperationStep_30(void)
{
	if (gu16_TimerUV_Oper == 0)
	{
//		Play_Voice(VOICE_STERIZATION_END_ICON22);
		Ini_OperationStep_10();
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (gu8_Key_Short.byte != 0x00U)
	{
		gu16_TimerUV_Oper = 0;
		gu8_TimerSleepMode = Z_TimerSleepMode;
		Ini_OperationStep_10();
		WifiSendData(WIFI_DATA_FUNCTION);
		Play_Voice(VOICE_BEEP_SELECT);
	}
//	if ((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_HOTWATER_LONG == SET))
//	{
//		Hot_Lock_Setting();
//	}
//	else if (KEY_COLDWATER_LONG == SET)
//	{
//		Cold_ONOFF_Setting();
//	}
//	else if (((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_ALL_LOCK_LONG == SET))
//			|| ((gu8_ModelType_forCHP == MODEL_CP) && (KEY_HOTWATER_LONG == SET)))
//	{
//		F_AllLock = SET;
//		Play_Voice(VOICE_ALL_LOCK_SETTING_A);
//		Ini_OperationStep_40();
//	}
//	else if (KEY_EFFLUENT_SHORT == SET)
//	{
//		Ini_OperationStep_50();
//		Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
//		if (gu8_WaterSelect == HOT_WATER)
//		{
//			if (F_FirstHotEffluent == CLEAR)
//			{
//				Play_Voice(VOICE_HOTWATER_DRAIN);
//			}
//			else
//			{
//				Play_Voice(VOICE_HOTWATER_EFFLUENT);
//			}
//		}
//		else
//		{
//			if (gu8_CupSize == CUPSIZE_CONTINUE)	//		연속 추출 인 경우
//			{
//				Play_Voice(VOICE_CONTIUNE_EFFLUENT);
//			}
//			else
//			{
//				Play_Voice(VOICE_EFFLUENT_START);
//			}
//		}
//	}
//	else if (KEY_COLDWATER_SHORT == SET)
//	{
//		ColdWater_Button();
//		Ini_OperationStep_10();
//	}
//	else if (KEY_HOTWATER_SHORT == SET)
//	{
//		if (gu8_ModelType_forCHP == MODEL_CHP)
//		{
//			HotWater_Button();
//		}
//		else
//		{
//			Play_Voice(VOICE_BEEP_SELECT);
//		}
//		Ini_OperationStep_10();
//	}
//	else if (KEY_PUREWATER_SHORT == SET)
//	{
//		PureWater_Button();
//		Ini_OperationStep_10();
//	}
//	else if (KEY_AMOUNT_SHORT == SET)
//	{
//		Ini_OperationStep_10();
//		Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
//		Water_Cup_SelectTimer(&gu8_TimerCupSelect);
//		Play_Voice(VOICE_BEEP_SELECT);
//	}
}
void Ini_OperationStep_30(void)
{
	OperationStepNoSet(Z_OperationStepNum_30);
	gu16_TimerUV_Oper = Z_TimerUV_Oper;
	F_UVCare_Start_forWIFI = SET;
	WifiSendData(WIFI_DATA_FUNCTION);
}

/// @brief    UV 동작 15분
/// @param    void
/// @return   void
void OperationStep_31(void)
{
	if (gu16_TimerUV_Oper == 0)
	{
		Ini_OperationStep_10();
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (gu8_Key_Short.byte != 0x00U)
	{
		gu16_TimerUV_Oper = 0;
		gu8_TimerSleepMode = Z_TimerSleepMode;
		Ini_OperationStep_10();
		WifiSendData(WIFI_DATA_FUNCTION);
		Play_Voice(VOICE_BEEP_SELECT);
#ifdef _EFF_Continue_Heater_OFF
		if (gu8_WaterSelect == HOT_WATER)
        {
            if ((gu16_CupSize == CUPSIZE_ONE_BOTTLE) || (gu16_CupSize == CUPSIZE_CONTINUE))
            {
                F_Eff_Continue_Heater_Off = SET;
            }
		}
#endif
	}
	else
	{

	}
}
void Ini_OperationStep_31(void)
{
	gu8_TimerUV_6h = Z_TimerUV_6h; 
	OperationStepNoSet(Z_OperationStepNum_31);
	gu16_TimerUV_Oper = Z_TimerUV_Oper_15min;
	F_UVCare_Start_forWIFI = SET;
	WifiSendData(WIFI_DATA_FUNCTION);
}

/// @brief    전체 잠금
/// @param    void
/// @return   void
void OperationStep_40(void)
{
	if (gu8_ErrorCode != 0)
	{
		Ini_OperationStep_E0();
	}
	else if (((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_ALL_LOCK_LONG == SET))
			|| ((gu8_ModelType_forCHP == MODEL_CP) && (KEY_HOTWATER_LONG == SET)))
	{
		F_VeryLongKey_Rejection = SET;
		F_AllLock = CLEAR;
		Play_Voice(VOICE_ALL_LOCK_CLEAR);
		WifiSendData(WIFI_DATA_FUNCTION);
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		gu8_TimerAllLockDisplay = Z_TimerAllLockDisplay;
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			Play_Voice(VOICE_ALL_LOCK_ICON22);
		}
		else
		{
			Play_Voice(VOICE_ALL_LOCK_CHECK_CP);
		}
	}
	else if (F_AllLock == CLEAR)
	{
		Ini_OperationStep_10();
	}
	else if (gu8_TimerUV_6h == 0)
	{
		Ini_OperationStep_31();
	}
	else
	{

	}
}
void Ini_OperationStep_40(void)
{
	gu8_WaterSelect = PURE_WATER;
	OperationStepNoSet(Z_OperationStepNum_40);
}

/// @brief    음성 변경
/// @param    void
/// @return   void
void OperationStep_41(void)
{
	if ((gu8_Key_Short.byte != 0) || (gu8_DigitalInput_Key1.Level.byte != 0))
	{
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_7s;
	}

	if (gu16_TimerOperation_100ms == 0)
	{
		Play_Voice(VOICE_BEEP_ON);
		Ini_OperationStep_10();
	}
	else if (KEY_SOUND_TYPE_LONG == SET)
	{
		if (gu8_Sound_Type == 1)
		{
			gu8_Sound_Type = 3;
		}
		else
		{
			gu8_Sound_Type--;
		}

		switch (gu8_Sound_Type)
		{
			case 1:
			default:
				Play_Voice(VOICE_SETTING_SILENT);
				break;
			case 2:
				Play_Voice(VOICE_SETTING_MELODY);
				break;
			case 3:
				Play_Voice(VOICE_SETTING_VOICE);
				break;
		}
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (KEY_HOTWATER_SHORT == SET)
	{
		if (gu8_Sound_Type > 1)
		{
			if (gu8_VoiceVolumeLevel > 0)
			{
				gu8_VoiceVolumeLevel--;
			}
			Play_Voice(VOICE_BEEP_SELECT);
			gu8_TimerSoundChange = Z_TimerSoundChange;
			WifiSendData(WIFI_DATA_FUNCTION);
		}
	}
	else if (KEY_AMOUNT_SHORT == SET)
	{
		if (gu8_Sound_Type > 1)
		{
			if (gu8_VoiceVolumeLevel < 4)
			{
				gu8_VoiceVolumeLevel++;
			}
			Play_Voice(VOICE_BEEP_SELECT);
			gu8_TimerSoundChange = Z_TimerSoundChange;
			WifiSendData(WIFI_DATA_FUNCTION);
		}
	}
	else if (KEY_EFFLUENT_SHORT == SET)
	{
		Play_Voice(VOICE_BEEP_ON);
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else
	{

	}
}
void Ini_OperationStep_41(void)
{
	gu16_TimerOperation_100ms = Z_TimerOperation_100ms_7s;
	if (gu8_Sound_Type == 1)
	{
		gu8_Sound_Type = 3;
	}
	else
	{
		gu8_Sound_Type--;
	}
	switch (gu8_Sound_Type)
	{
		case 1:
		default:
			Play_Voice(VOICE_SETTING_SILENT);
			break;
		case 2:
			Play_Voice(VOICE_SETTING_MELODY);
			break;
		case 3:
			Play_Voice(VOICE_SETTING_VOICE);
			break;
	}
	WifiSendData(WIFI_DATA_FUNCTION);
	OperationStepNoSet(Z_OperationStepNum_41);
}

/// @brief    관리모드 변경 및 표시
/// @param    void
/// @return   void
void OperationStep_42(void)
{
	if (gu16_TimerOperation_100ms == 0)
	{
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		Ini_OperationStep_10();
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_LINETEST_MANAGE_VERYLONG == SET)
	{
		Ini_OperationStep_42();
		Play_Voice(VOICE_BEEP_ON);
	}
	else
	{

	}
}
void Ini_OperationStep_42(void)
{
	F_SelfService ^= 1;								//	관리모드 전환
	if (F_SelfService == CLEAR)
	{
		F_FilterChange_Display = CLEAR;
		F_FaucetChange_Display = CLEAR;
	}
	gu16_TimerOperation_100ms = Z_TimerOperation_100ms_6s;
	WifiSendData(WIFI_DATA_FUNCTION);
	OperationStepNoSet(Z_OperationStepNum_42);
}

/// @brief    추출 중
/// @param    void
/// @return   void
void OperationStep_50(void)
{
	Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
	gu8_TimerChangeDisplay = Z_TimerChangeDisplay;

	if (gu8_ErrorCode != 0)							//	에러발생한 경우
	{
		if (gu16_CupSize == CUPSIZE_CONTINUE)		//		연속 추출인 경우
		{
			return_default_cup();		//			Default Cup
		}
		Ini_OperationStep_E0();						//		에러표시스텝 이행
	}
	else if (KEY_EFFLUENT_SHORT == SET)				//	추출 버튼
	{

#ifdef _FOR_REPEAT_EFF_ENDURANCE_TEST_
		F_Repeat_Eff = CLEAR;
#endif
		if (F_Repeat_Eff_Mode_forMP == SET)			//@003
		{
			F_Repeat_Eff_forMP = CLEAR;				//@003
		}

		if (gu16_CupSize == CUPSIZE_CONTINUE)		//		연속 추출인 경우
		{
			return_default_cup();		//			Default Cup
		}
		else
		{
			if (gu16_Effluent_Amount_SegDisplay >= 10)//		추출 된 물양이 10ml 이상
			{
				if (gu8_HotStep != Z_HotStep_02)
				{
					gu8_TimerEffluentAmount_Display = Z_TimerEffluentAmount_Display;	//	2초 점등
				}
			}
		}
		Play_Voice(VOICE_EFFLUENT_COMPLETE);
		Ini_OperationStep_10();
	}
	else if ((gu8_ModelType_forNational == MODEL_KOREA) && (KEY_EFFLUENT_LONG == SET) && (gu16_CupSize != CUPSIZE_CONTINUE) && (gu8_HotStep != Z_HotStep_02))
	{
		gu16_CupSize = CUPSIZE_CONTINUE;
		Effluent_Hz_Decide();
		gu8_TimerContSeg = Z_TimerContSeg;			//	연속 추출 휠 타이머
		gu8_CounterContSeg = 0;
		Play_Voice(VOICE_CONTIUNE_EFFLUENT);
	}
	else if ((gu8_ModelType_forNational == MODEL_KOREA) && (KEY_EFFLUENT_VERYLONG == SET))			//	추출 버튼 8초 연속 입력 시 강제 추출 종료
	{
		Play_Voice(VOICE_EFFLUENT_COMPLETE);
		Ini_OperationStep_10();
	}
	else if (((gu16_CupSize != CUPSIZE_CONTINUE) && (gu16_Effluent_Hz == 0))
			|| ((gu16_CupSize == CUPSIZE_CONTINUE) && (gu16_TimerEffluent == 0)))	//	추출 완료
	{
//		if (gu8_CupSize == CUPSIZE_CONTINUE)
//		{
//			gu8_CupSize = gu8DefaultCupSize;
//		}
#ifdef _FOR_REPEAT_EFF_ENDURANCE_TEST_
#ifndef _FOR_WQA_
		switch (gu8_Counter_Repeat_Eff)
		{
			case 0:								//	냉수 끝나고
				gu16_Timer_Repeat_Eff = 10;		//	단위 1초
				break;
			case 1:								//	정수 끝나고
				gu16_Timer_Repeat_Eff = 10;
				break;
			case 2:								//	온수 끝나고
				gu16_Timer_Repeat_Eff = 10;
				break;
//			case 3:								//	정수 끝나고
//			case 4:								//	정수 끝나고
//				gu16_Timer_Repeat_Eff = 10;
//				break;
			default:
				break;
		}
#else
		switch (gu8_Counter_Repeat_Eff)
		{
			case 0:								//	냉수 끝나고
				gu16_Timer_Repeat_Eff = 2;		//	단위 1초
				break;
			case 1:								//	정수 끝나고
				gu16_Timer_Repeat_Eff = 2;
				break;
			case 2:								//	온수 끝나고
				gu16_Timer_Repeat_Eff = 2;
				break;
			default:
				break;
		}
#endif
#endif
		//@003	시작
		if (F_Repeat_Eff_Mode_forMP == SET)
		{
			switch (gu8_Counter_Repeat_Eff_forMP)
			{
				case 0:								//	냉수 끝나고
					gu16_Timer_Repeat_Eff_forMP = 5;		//	단위 1초
					break;
				case 1:								//	정수 끝나고
					gu16_Timer_Repeat_Eff_forMP = 5;
					break;
				case 2:								//	온수 끝나고
					gu16_Timer_Repeat_Eff_forMP = 5;
					break;
				default:
					break;
			}
		}
		//@003	끝

		if (gu16_CupSize == CUPSIZE_CONTINUE)		//		연속 추출인 경우
		{
			return_default_cup();		//			Default Cup
		}
		else
		{
			if (gu16_Effluent_Amount_SegDisplay >= 10)//		추출 된 물양이 10ml 이상
			{
				if (gu8_HotStep != Z_HotStep_02)
				{
					gu8_TimerEffluentAmount_Display = Z_TimerEffluentAmount_Display;	//	2초 점등
				}
			}
		}
		Play_Voice(VOICE_EFFLUENT_COMPLETE);
		Ini_OperationStep_10();
	}
	else if (gu8_Key_Short.byte != 0x00)		//
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else
	{

	}
}
void Ini_OperationStep_50(void)
{
	OperationStepNoSet(Z_OperationStepNum_50);

	gu8_TimerContSeg = Z_TimerContSeg;
	gu8_CounterContSeg = 0;
	gu16_FlowDecide_W_Ave = 0;

	gu16_Effluent_Amount_SegDisplay = 0;
	gu16_Effluent_Hz_forSegDisplay = 0;				//

	gu8_TimerEffluent_After_2s = Z_TimerEffluent_After_2s;

	F_SmartCheck_Prohibit = SET;
	WifiSendData(WIFI_DATA_FUNCTION);

	Effluent_Hz_Decide();					//	(다른스텝에서 준비과정떄문에 Hz결정이 안될 때)	추출유량 결정

#ifdef _AVATAR_
	gu16_Effluent_Count_forAVATAR[gu8_WaterSelect]++;
#endif

#ifdef _FOR_DEVELOP_
	if (gu16_Effluent_Count[gu8_WaterSelect] < 1999)
	{
		gu16_Effluent_Count[gu8_WaterSelect]++;
		if ((gu16_Effluent_Count[gu8_WaterSelect] == 1999) || ((gu16_Effluent_Count[gu8_WaterSelect] % 10) == 0))		//	EEPROM 수명으로 10회마다 저장
		{
			switch (gu8_WaterSelect)
			{
				case COLD_WATER:
//					EEPROM_ConfirmWrite(EEPROM_ADDR_COLD_EFF_COUNT_LOW, (U8)(gu16_Effluent_Count[gu8_WaterSelect] & 0x00FF));
					if (gu16_Effluent_Count[gu8_WaterSelect] >= 256)
					{
//						EEPROM_ConfirmWrite(EEPROM_ADDR_COLD_EFF_COUNT_HIGH, (U8)((gu16_Effluent_Count[gu8_WaterSelect] & 0xFF00)>>4));
					}
					break;
				case HOT_WATER:
//					EEPROM_ConfirmWrite(EEPROM_ADDR_HOT_EFF_COUNT_LOW, (U8)(gu16_Effluent_Count[gu8_WaterSelect] & 0x00FF));
					if (gu16_Effluent_Count[gu8_WaterSelect] >= 256)
					{
//						EEPROM_ConfirmWrite(EEPROM_ADDR_HOT_EFF_COUNT_HIGH, (U8)((gu16_Effluent_Count[gu8_WaterSelect] & 0xFF00)>>4));
					}
					break;
				case PURE_WATER:
//					EEPROM_ConfirmWrite(EEPROM_ADDR_PURE_EFF_COUNT_LOW, (U8)(gu16_Effluent_Count[gu8_WaterSelect] & 0x00FF));
					if (gu16_Effluent_Count[gu8_WaterSelect] >= 256)
					{
//						EEPROM_ConfirmWrite(EEPROM_ADDR_PURE_EFF_COUNT_HIGH, (U8)((gu16_Effluent_Count[gu8_WaterSelect] & 0xFF00)>>4));
					}
					break;
				default:
					break;
			}
		}
	}
#endif
}

/// @brief	  FOTA 표시
/// @param    void
/// @return   void
static void OperationStep_70(void)
{
	if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
}
static void Ini_OperationStep_70(void)
{
	gu8_TimerFOTA_Display = Z_TimerFOTA_Display;
	F_FOTA_Display_forWIFI_LED_ONOFF = SET;
	OperationStepNoSet(Z_OperationStepNum_70);
}

/// @brief    자가진단
/// @param    void
/// @return   void
void OperationStep_80(void)
{
	U8 mu8_WifiDisp;

	if (gu8_Key_Short.byte != 0x00)					//	적용 대기
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else if (F_SmartCheck_Complete == SET)
	{
		Ini_OperationStep_10();
		mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_DISP);	//	Wi-Fi 연결상태 확인

		if (mu8_WifiDisp != WIFI_DISP_CONNECT)
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
	}
	else if (F_SmartCheck == CLEAR)					//	서버에서 정지 신호 시
	{
		Ini_OperationStep_10();
		gu16_SmartCheck_Result_HeaterTriac_ADC = 0xFFFFU;
		gu16_SmartCheck_Result_HeaterRelay_ADC = 0xFFFFU;
		gu16_SmartCheck_Result_Pure_VV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_HotIn_VV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_HotOut_VV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_ColdIn_VV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_ColdOut_VV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_1stNos_VV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_Fan_mA = 0xFFFFU;
		gu16_SmartCheck_Result_TEM_mA = 0xFFFFU;
		gu16_SmartCheck_Result_FlowMeter = 0xFFFFU;
		gu16_SmartCheck_Result_UV_mA = 0xFFFFU;
		gu16_SmartCheck_Result_SteppingMotor_mA = 0xFFFFU;
		Play_Voice(VOICE_BEEP_OFF);
	}
	else
	{

	}
}
void Ini_OperationStep_80(void)
{
	F_SmartCheck_Start = SET;
	F_SmartCheck_Complete = CLEAR;
	OperationStepNoSet(Z_OperationStepNum_80);
}

/// @brief    Line Test (FCT)
/// @param    void
/// @return   void
void OperationStep_90(void)
{
//	static U8 gu8_HeaterRetry = 0;
	static U16 leaksonsor_before = 0;				//@027

//	if ((F_LineFrontTest_Amount_Complete == SET) &&	(F_LineFrontTest_Hot_Complete == SET) && (F_LineFrontTest_Cold_Complete == SET)
//		&& (F_LineFrontTest_Pure_Complete == SET) && (F_LineFrontTest_Effluent_Complete == SET)
//		&& (INPUT_REED_SW_COVER == CLEAR)
//		&& (INPUT_REED_SW_FAUCET == SET)
//		&& (INPUT_REED_SW_FILTER == SET)
//		&& (REED_SW_COVER_Ready == SET)
//		&& (REED_SW_FILTER_Ready == SET)
//		&& (REED_SW_WATERFLOW_Ready == SET)
//		&& (ADC_RESULT_LEAKSENSOR > Z_LeakSensor_Error))
//	{
	if (ADC_RESULT_LEAKSENSOR > Z_LeakSensor_Error)																							//@027
	{                                                                                                                                       //@027
		if ((F_LineFrontTest_Amount_Complete == SET) &&	(F_LineFrontTest_Hot_Complete == SET) && (F_LineFrontTest_Cold_Complete == SET)     //@027
			&& (F_LineFrontTest_Pure_Complete == SET) && (F_LineFrontTest_Effluent_Complete == SET)                                         //@027
			&& (INPUT_REED_SW_COVER == CLEAR)                                                                                               //@027
			&& (INPUT_REED_SW_FAUCET == SET)                                                                                                //@027
			&& (INPUT_REED_SW_FILTER == SET)                                                                                                //@027
			&& (REED_SW_COVER_Ready == SET)                                                                                                 //@027
			&& (REED_SW_FILTER_Ready == SET)                                                                                                //@027
			&& (REED_SW_WATERFLOW_Ready == SET))	                                                                                        //@027
		{                                                                                                                                   //@027
//		if (gu8_ModelType_forCHP == MODEL_CP)
//		{
			F_LineAuto_Ready = SET;
		}
		else									//@027
		{										//@027
			if (leaksonsor_before < (Z_LeakSensor_Error/2))	//@027
			{
				Play_Voice(VOICE_BEEP_WARNING);	//@027
				gu16_TimerFCT_Incomplete_Display = Z_TimerFCT_Incomplete_Display;	//@027
			}
		}										//@027
//		}
//		else										//	CHP
//		{
//			if (gu16_Heater_FB_forLineTest != 0)
//			{
//				F_LineAuto_Ready = SET;				//
//			}
//		}
	}

	leaksonsor_before = ADC_RESULT_LEAKSENSOR;		//@027

	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		if ((F_WiFi_Start == CLEAR) && (gu8_TimerWiFiTest_Start == 0))
		{
			F_WiFi_Start = SET;
			WifiKey(WIFI_KEY_TEST);
		}
	}
//@006	else											//	일본향
//@006	{
		if ((F_LineFrontTest_Effluent_Oper == SET) && (gu8_TimerLineFrontTest_EffLimit == 0)
		&& ((F_LineFrontTest_Cold_Select == SET) || (F_LineFrontTest_Pure_Select == SET)))
		{
			F_LineFrontTest_Effluent_Oper = CLEAR;	//	저유량센서 고속 회전(열발생)으로 추출 x초 제한
			F_LineFrontTest_1stNos_VV = CLEAR;		//@023
			Play_Voice(VOICE_EFFLUENT_COMPLETE);
		}
//@006	}

	if ((INPUT_REED_SW_COVER_Before == CLEAR) && (INPUT_REED_SW_COVER == SET))
	{
		REED_SW_COVER_Ready = SET;
		Play_Voice(VOICE_BEEP_ON);
	}
	if ((INPUT_REED_SW_COVER == CLEAR) && (INPUT_REED_SW_COVER_Before == SET))	//@024
	{                                                                           //@024
		Play_Voice(VOICE_BEEP_OFF);                                             //@024
	}                                                                           //@024

	if ((INPUT_REED_SW_FILTER_Before == CLEAR) && (INPUT_REED_SW_FILTER == SET))
	{
		REED_SW_FILTER_Ready = SET;
		Play_Voice(VOICE_BEEP_ON);
	}
	if ((INPUT_REED_SW_FILTER == CLEAR) && (INPUT_REED_SW_FILTER_Before == SET))//@024
	{                                                                           //@024
		Play_Voice(VOICE_BEEP_OFF);                                             //@024
	}                                                                           //@024

	if ((INPUT_REED_SW_FAUCET_Before == CLEAR) && (INPUT_REED_SW_FAUCET == SET))
	{
		REED_SW_WATERFLOW_Ready = SET;
		Play_Voice(VOICE_BEEP_ON);
	}
	if ((INPUT_REED_SW_FAUCET == CLEAR) && (INPUT_REED_SW_FAUCET_Before == SET))//@024
	{                                                                           //@024
		Play_Voice(VOICE_BEEP_OFF);                                             //@024
	}                                                                           //@024

	if (gu16_TimerUV_Oper == 0)
	{
		if (gu16_Front_UV_FB_forLineTest < 10)
		{
			F_LineTest_UV_FBError = SET;
		}
	}

	if ((gu8_ModelType_forCHP == MODEL_CHP) && (F_LineTest_Heater == SET) && (gu16_Heater_FB_forLineTest < 800)	//	히터 검사 완료 시에 FB Data가 기준치 미달인 경우
		&& (gu8_TimerLineTest_Heater_Delay == 0) && (gu8_TimerLineTest_Heater == 0)	&& (gu8_HeaterRetry < 3))
	{
		gu8_HeaterRetry++;
		gu8_TimerLineTest_Heater = Z_TimerLineTest_Heater;
		gu8_TimerLineTest_Heater_Delay = Z_TimerLineTest_Heater_Delay;
	}

	if (F_TestMode_LineAuto == CLEAR)
	{
//@023		if (KEY_LINETEST_MANAGE_LONG == SET)
		if (KEY_FLU_CAN_TH_CON_FCTLONG == SET)	//@023
		{
			Play_Voice(VOICE_SPECIAL_FUNCTION);
			Ini_OperationStep_93();				//
		}
		//@023 시작
		else if (KEY_LINETEST_MANAGE_FCTLONG == SET)
		{
			if ((F_LineFrontTest_Amount_Complete == SET) &&	(F_LineFrontTest_Hot_Complete == SET) && (F_LineFrontTest_Cold_Complete == SET)
				&& (F_LineFrontTest_Pure_Complete == SET) && (F_LineFrontTest_Effluent_Complete == SET)
				&& (INPUT_REED_SW_COVER == CLEAR)
				&& (INPUT_REED_SW_FAUCET == SET)
				&& (INPUT_REED_SW_FILTER == SET)
				&& (REED_SW_COVER_Ready == SET)
				&& (REED_SW_FILTER_Ready == SET)
				&& (REED_SW_WATERFLOW_Ready == SET)
				&& (F_WIFI_Pairing_forLineTest == SET)
				&& (gu16_FlowDecide_Hz_forLineAuto != 0)
				&& (F_LineFrontTest_1stNos_VV_Complete == SET))
			{
				Play_Voice(VOICE_SPECIAL_FUNCTION);
				Ini_OperationStep_93();				//
			}
			else
			{
				gu16_TimerFCT_Incomplete_Display = Z_TimerFCT_Incomplete_Display;	//@027
				gu16_TimerFCT_Incomplete_Display_forPASECO = Z_TimerFCT_Incomplete_Display;	//@027
				Play_Voice(VOICE_BEEP_WARNING);
			}
		}
		//@023 끝
		else if (KEY_HOTWATER_SHORT == SET)
		{
			gu16_TimerFCT_Incomplete_Display = 0;	//@027
			gu16_TimerFCT_Incomplete_Display_forPASECO = 0;	//@027
			if (gu8_ModelType_forCHP == MODEL_CP)
			{
				F_LineFrontTest_Hot_Complete = SET;
			}
			else
			{
				if (F_LineTest_Heater == CLEAR)
				{
					if (gi16_HeatSinkSensor.DEGREE < Z_TEMP55)
					{
						F_LineTest_Heater = SET;
						gu8_HeaterRetry++;
						gu8_TimerLineTest_Heater = Z_TimerLineTest_Heater;
						gu8_TimerLineTest_Heater_Delay = Z_TimerLineTest_Heater_Delay;
					}
				}
			}

			Play_Voice(VOICE_BEEP_SELECT);

			F_LineFrontTest_Operation.byte = 0x00U;
			F_LineFrontTest_Pure_Select	= CLEAR;
			F_LineFrontTest_Cold_Select = CLEAR;
			F_LineFrontTest_Hot_Select ^= 1;
		}
		else if (KEY_PUREWATER_SHORT == SET)
		{
			gu16_TimerFCT_Incomplete_Display = 0;	//@027
			Play_Voice(VOICE_BEEP_SELECT);

			F_LineFrontTest_Operation.byte = 0x00U;
			F_LineFrontTest_Hot_Select	= CLEAR;
			F_LineFrontTest_Cold_Select = CLEAR;
			F_LineFrontTest_Pure_Select ^= 1;
		}
		else if (KEY_COLDWATER_SHORT == SET)
		{
			gu16_TimerFCT_Incomplete_Display = 0;	//@027
			gu16_TimerFCT_Incomplete_Display_forPASECO = 0;	//@027
			Play_Voice(VOICE_BEEP_SELECT);

			F_LineFrontTest_Operation.byte = 0x00U;
			F_LineFrontTest_Pure_Select	= CLEAR;
			F_LineFrontTest_Hot_Select = CLEAR;
			F_LineFrontTest_Cold_Select ^= 1;
		}
		else if (KEY_AMOUNT_SHORT == SET)
		{
			gu16_TimerFCT_Incomplete_Display = 0;	//@027
			gu16_TimerFCT_Incomplete_Display_forPASECO = 0;	//@027
			Play_Voice(VOICE_BEEP_SELECT);
			F_LineFrontTest_Amount_Select = 1;
			if (++gu8_LineFrontTest_Amount > 3)
			{
				gu8_LineFrontTest_Amount = 0;
			}
			F_LineFrontTest_Amount_Complete = SET;
		}
//		else if (KEY_EFFLUENT_SHORT == SET)
//		{
//			F_LineFrontTest_Effluent_Complete = SET;
//
//			if ((F_LineFrontTest_Effluent_Oper == CLEAR)
//				&& ((F_LineFrontTest_Cold_Select == SET) || (F_LineFrontTest_Hot_Select == SET) || (F_LineFrontTest_Pure_Select == SET)))
//			{
//				if ((gu8_ModelType_forCHP == MODEL_CP) && (F_LineFrontTest_Hot_Select == SET))
//				{
//					Play_Voice(VOICE_BEEP_WARNING);
//					return;
//				}
//				F_LineFrontTest_Effluent_Oper = SET;
//				if (F_LineFrontTest_Cold_Select == SET)
//				{
//					F_LineFrontTest_Cold_Complete = SET;
//				}
//				else if (F_LineFrontTest_Hot_Select == SET)
//				{
//					F_LineFrontTest_Hot_Complete = SET;
//					F_LineFrontTest_SteppingMotor_Oper = SET;
//				}
//				else if (F_LineFrontTest_Pure_Select == SET)
//				{
//					F_LineFrontTest_Pure_Complete = SET;
//				}
//				else
//				{
//
//				}
//				Play_Voice(VOICE_EFFLUENT_START);
//			}
//			else
//			{
//				Play_Voice(VOICE_BEEP_WARNING);
//			}
//		}
//		else if ((F_LineFrontTest_Effluent_Oper == SET) && (KEY_EFFLUENT_LOW_EDGE == SET))
//		{
//			F_LineFrontTest_Effluent_Oper = CLEAR;
//			F_LineFrontTest_SteppingMotor_Oper = CLEAR;
//			Play_Voice(VOICE_EFFLUENT_COMPLETE);
//		}
		else if (KEY_EFFLUENT_SHORT == SET)
		{
			gu16_TimerFCT_Incomplete_Display = 0;	//@027
			gu16_TimerFCT_Incomplete_Display_forPASECO = 0;	//@027
			gu8_TimerFrontTest_1stNosDisplay = 0;	//@030
			if (F_LineFrontTest_Effluent_Oper == CLEAR)
			{
				F_LineFrontTest_Effluent_Complete = SET;

				if ((F_LineFrontTest_Effluent_Oper == CLEAR)
					&& ((F_LineFrontTest_Cold_Select == SET) || (F_LineFrontTest_Hot_Select == SET) || (F_LineFrontTest_Pure_Select == SET)))
				{
					if ((gu8_ModelType_forCHP == MODEL_CP) && (F_LineFrontTest_Hot_Select == SET))
					{
						Play_Voice(VOICE_BEEP_WARNING);
						return;
					}
					F_LineFrontTest_Effluent_Oper = SET;
					gu8_TimerLineFrontTest_EffLimit = Z_TimerLineFrontTest_EffLimit;
//@030					if (F_LineFrontTest_Cold_Select == SET)
//@030					{
//@030						F_LineFrontTest_Cold_Complete = SET;
//@030					}
//@030					else if (F_LineFrontTest_Hot_Select == SET)
//@030					{
//@030						F_LineFrontTest_Hot_Complete = SET;
//@030						F_LineFrontTest_SteppingMotor_Oper = SET;
//@030					}
//@030					else if (F_LineFrontTest_Pure_Select == SET)
//@030					{
//@030						F_LineFrontTest_Pure_Complete = SET;
//@030					}
//@030					else
//@030					{
//@030
//@030					}
					if (F_LineFrontTest_Hot_Select == SET)			//@030
					{                                               //@030
						F_LineFrontTest_SteppingMotor_Oper = SET;   //@030
					}                                               //@030
					Play_Voice(VOICE_EFFLUENT_START);
				}
				else
				{
					Play_Voice(VOICE_BEEP_WARNING);
				}
			}
			else
			{
				F_LineFrontTest_Effluent_Oper = CLEAR;
				F_LineFrontTest_1stNos_VV = CLEAR;		//@023
				if (F_LineFrontTest_Hot_Select == SET)
				{
					F_LineFrontTest_SteppingMotor_Oper = CLEAR;
					StepMotor_ZeroBack_Open();			//				열림 초기화
				}
				Play_Voice(VOICE_EFFLUENT_COMPLETE);
			}
		}
		else if (KEY_WIFI_AP_PAIRING_FCTLONG == SET)//@023
		{											//@023
			gu16_TimerFCT_Incomplete_Display = 0;	//@027
			gu16_TimerFCT_Incomplete_Display_forPASECO = 0;	//@027
			F_LineFrontTest_1stNos_VV ^= 1;			//@023
			gu8_TimerFrontTest_1stNosDisplay = Z_TimerFrontTest_1stNosDisplay;	//@023
//@030			F_LineFrontTest_1stNos_VV_Complete = SET;	//@023
			F_LineFrontTest_Cold_Select = SET;		//@023
			F_LineFrontTest_Pure_Select = CLEAR;	//@023
			F_LineFrontTest_Hot_Select = CLEAR;		//@023
			if (F_LineFrontTest_1stNos_VV == SET)	//@023
			{                                       //@023
				Play_Voice(VOICE_BEEP_ON);          //@023
			}                                       //@023
			else                                    //@023
			{                                       //@023
				Play_Voice(VOICE_BEEP_OFF);         //@023
			}										//@023
		}											//@023
		else
		{

		}
	}
	else
	{
		if (gu8_Key_Short.byte != 0x00)
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}

	if (F_LineFrontTest_Effluent_Oper == SET)		//@030
	{                                               //@030
		if (gu16_FlowDecide_Hz == 0)					//@030
		{											//@030
			if (F_LineFrontTest_1stNos_VV == SET)	//@030
			{										//@030
				F_LineFrontTest_1stNos_VV_Complete = SET;	//@030
			}										//@030
		}											//@030
		else										//@030
		{											//@030
			if (F_LineFrontTest_Cold_Select == SET)        	//@030
			{                                               //@030
				F_LineFrontTest_Cold_Complete = SET;        //@030
			}                                               //@030
			else if (F_LineFrontTest_Hot_Select == SET)     //@030
			{                                               //@030
				F_LineFrontTest_Hot_Complete = SET;         //@030
			}                                               //@030
			else if (F_LineFrontTest_Pure_Select == SET)    //@030
			{                                               //@030
				F_LineFrontTest_Pure_Complete = SET;        //@030
			}                                               //@030
			else                                            //@030
			{                                               //@030
															//@030
			}                                               //@030
		}													//@030
	}                                                       //@030

	INPUT_REED_SW_COVER_Before = INPUT_REED_SW_COVER;
	INPUT_REED_SW_FILTER_Before = INPUT_REED_SW_FILTER;
	INPUT_REED_SW_FAUCET_Before = INPUT_REED_SW_FAUCET;
}
void Ini_OperationStep_90(void)
{
	gu8_TimerWiFiTest_Start = Z_TimerWiFiTest_Start;
	F_WiFi_Start = CLEAR;

	INPUT_REED_SW_COVER_Before = INPUT_REED_SW_COVER;
	INPUT_REED_SW_FILTER_Before = INPUT_REED_SW_FILTER;
	INPUT_REED_SW_FAUCET_Before = INPUT_REED_SW_FAUCET;
	OperationStepNoSet(Z_OperationStepNum_90);
}

/// @brief    PBA Test
/// @param    void
/// @return   void
void OperationStep_91(void)
{
	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		if ((F_WiFi_Start == CLEAR) && (gu8_TimerWiFiTest_Start == 0))
		{
			F_WiFi_Start = SET;
			WifiKey(WIFI_KEY_TEST);
		}
	}
}
void Ini_OperationStep_91(void)
{
	gu8_TimerWiFiTest_Start = Z_TimerWiFiTest_Start;
	F_WiFi_Start = CLEAR;

	OperationStepNoSet(Z_OperationStepNum_91);
}

/// @brief    프론트 테스트
/// @param    void
/// @return   void
void OperationStep_92(void)
{
	static U8 VoiceTest_forRBK = CLEAR;

#ifdef _FRONT_JIG_PROGRAM_
	gu8_VoiceVolumeLevel = 4;

//#ifdef _RBK_VOICE_JIG_PROGRAM_
////	if ((gu8_TimerVoiceTest_forRBK == 0) && (VoiceTest_forRBK == CLEAR))
////	{
////		VoiceTest_forRBK = SET;
////		Play_Voice(0xC9);
////	}
//	if (gu8_TimerVoiceTest_forRBK == 0)
//	{
//		VoiceTest_forRBK ^= 1;
//		gu8_TimerVoiceTest_forRBK = 20;
//		if (VoiceTest_forRBK == 1)
//		{
//			Play_Voice(0xCF);
//		}
//		else
//		{
//			Play_Voice(VOICE_POWER_ON);
//		}
//	}
//#endif

	if (KEY_HOTWATER_SHORT == SET)
	{
#ifdef _RBK_VOICE_JIG_PROGRAM_
		Play_Voice(VOICE_CUP_SENSOR_ERROR);
#else
		Play_Voice(VOICE_BEEP_SELECT);
		gu8_FrontTest_HotButton_Counter++;
#endif
	}
	else if (KEY_PUREWATER_SHORT == SET)
	{
#ifdef _RBK_VOICE_JIG_PROGRAM_
		Play_Voice(VOICE_HOTWATER_FIRST);
#else
		Play_Voice(VOICE_BEEP_SELECT);
		gu8_FrontTest_PureButton_Counter++;
#endif
	}
	else if (KEY_COLDWATER_SHORT == SET)
	{
#ifdef _RBK_VOICE_JIG_PROGRAM_
//		Play_Voice(0xC9);
		Play_Voice(0xCF);
#else
		Play_Voice(VOICE_BEEP_SELECT);
		gu8_FrontTest_ColdButton_Counter++;
#endif
	}
	else if (KEY_AMOUNT_SHORT == SET)
	{
#ifdef _RBK_VOICE_JIG_PROGRAM_
		Play_Voice(VOICE_FLOWFAUCET_ON);
#else
		Play_Voice(VOICE_BEEP_SELECT);
		gu8_FrontTest_AmountButton_Counter++;
#endif
	}
	else if (KEY_EFFLUENT_SHORT== SET)
	{
#ifdef _RBK_VOICE_JIG_PROGRAM_
		Play_Voice(VOICE_BEEP_WARNING);
#else
		PSM5 ^= 1;
		Play_Voice(VOICE_BEEP_SELECT);
//		gu8_FrontTest_EffluentButton_Counter++;
		F_FrontTest_EffluentButton = SET;
		gu8_Timer_FrontTest_UV_FBCheck = Z_Timer_FrontTest_UV_FBCheck;
		gu16_Timer_FrontTest_Seg = Z_Timer_FrontTest_Seg;
#endif
	}
	else
	{

	}
#else
	Ini_OperationStep_10();
#endif
}
void Ini_OperationStep_92(void)
{
//	Play_Voice(VOICE_SPECIAL_FUNCTION);				//	국내향/일본향 구분을 위해 음성 출력
	gu8_VoiceVolumeLevel = 4;
#ifndef _RBK_VOICE_JIG_PROGRAM_
	if (gu8_ModelType_forCHP == MODEL_CHP)
	{
		Play_Voice(VOICE_HOTWATER_FIRST);				//	icon 2.0
	}
	else
	{
		Play_Voice(VOICE_COLD_ON_SETTING);
	}
#else
//	Play_Voice(0xCF);								//	icon AIS 4.0
//	Play_Voice(VOICE_POWER_ON);						//	icon AIS 4.0 & Noble
	gu8_TimerVoiceTest_forRBK = 20;
#endif
	OperationStepNoSet(Z_OperationStepNum_92);
}

/// @brief    공장 단축 검사
/// @param    void
/// @return   void
void OperationStep_93(void)
{
//	if ((gu8_TimerLineTest2_Heater != 0) && (gu8_TimerLineTest2_Heater <= (Z_TimerLineTest_Heater2/2)))
	if ((gu8_TimerLineTest2_Heater != 0) && (gu8_TimerLineTest2_Heater <= (Z_TimerLineTest_Heater/2)))
	{
		if (ADC_RESULT_HEATER_FB >= 200)
		{
			F_LineTest2_HeaterResult_OK = SET;
		}
		else
		{
			F_LineTest2_HeaterResult_NG = SET;
		}
	}

	if (F_LineTest2_Cooling == SET)
	{
		if (gu8_TimerLineTest2_Cooling != 0)
		{
//			if (abs(gi16_LineTest2_HeatSinkSensor_Backup-gi16_HeatSinkSensor.DEGREE) >= Z_TEMP2)
//			{
//				F_LineTest2_CoolingResult_OK = SET;
//			}
			if ((gi16_HeatSinkSensor.DEGREE > gi16_LineTest2_HeatSinkSensor_Backup) && ((gi16_HeatSinkSensor.DEGREE-gi16_LineTest2_HeatSinkSensor_Backup) >= Z_TEMP2))
			{
				F_LineTest2_CoolingResult_OK = SET;
			}
		}
	}

	if (ADC_RESULT_LEAKSENSOR > Z_LeakSensor_Error)
	{
		F_LineTest2_Purge = SET;
	}
	else
	{
		F_LineTest2_Purge = CLEAR;
	}

	if ((gu8_ErrorCode != 0) && (gu8_Key_Short.byte != 0x00U))
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else if (KEY_HOTWATER_SHORT == SET)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			if (F_LineTest2_Heater == CLEAR)
			{
//				gu8_TimerLineTest2_Heater = Z_TimerLineTest_Heater2;
				if (gu8_ModelType_forNational == MODEL_KOREA)	//@023
				{                                               //@023
					if (gu16_Heater_FB_forLineTest >= 2000)     //@023
					{                                           //@023
						F_LineTest2_HeaterResult_OK = SET;      //@023
					}                                           //@023
					else										//@023
					{											//@023
						gu8_TimerLineTest2_Heater = Z_TimerLineTest_Heater;	//@023
					}											//@023
				}                                               //@023
				else                                            //@023
				{                                               //@023
					if (gu16_Heater_FB_forLineTest >= 1000)     //@023
					{                                           //@023
						F_LineTest2_HeaterResult_OK = SET;      //@023
					}                                           //@023
					else										//@023
					{											//@023
						gu8_TimerLineTest2_Heater = Z_TimerLineTest_Heater;	//@023
					}											//@023
				}                                               //@023
//@023				gu8_TimerLineTest2_Heater = Z_TimerLineTest_Heater;
				F_LineTest2_Heater = SET;
				Play_Voice(VOICE_BEEP_ON);
			}
			else
			{
				Play_Voice(VOICE_BEEP_WARNING);
			}
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
	else if (KEY_PUREWATER_SHORT == SET)
	{
		F_LineTest2_Valve ^= 1;
		if (F_LineTest2_Valve == SET)
		{
			Play_Voice(VOICE_BEEP_ON);
		}
		else
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
	}
	else if (KEY_COLDWATER_SHORT == SET)
	{
		if (F_LineTest2_Cooling == CLEAR)
		{
			F_LineTest2_Cooling = SET;
			gu8_TimerLineTest2_Cooling = Z_TimerLineTest2_Cooling;
			gi16_LineTest2_HeatSinkSensor_Backup = gi16_HeatSinkSensor.DEGREE;
			Play_Voice(VOICE_BEEP_ON);
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
}
void Ini_OperationStep_93(void)
{
	F_TestMode_LineTest2 = SET;
	gu16_TimerFCT_Incomplete_Display = 0;			//@030
	gu8_ErrorCode = CLEAR;							//@023
	OperationStepNoSet(Z_OperationStepNum_93);
}

/// @brief    냉각PI 제어 변경 적용 @022
/// @param    void
/// @return   void
void OperationStep_A0(void)
{
	if (KEY_EFFLUENT_SHORT == SET)
	{
		Ini_OperationStep_10();
		Play_Voice(VOICE_BEEP_ON);
	}
//@022 #ifdef _FOR_DEVELOP_
	else if (KEY_HOTWATER_SHORT == SET)			//@022
	{                                           //@022
		if (gu16_Cooling_TargetDegree > Z_TEMP2)//@022
		{                                       //@022
			gu16_Cooling_TargetDegree -= Z_TEMP0_5;     //@022
			Play_Voice(VOICE_BEEP_SELECT);      //@022
		}                                       //@022
		else                                    //@022
		{                                       //@022
			Play_Voice(VOICE_BEEP_WARNING);     //@022
		}                                       //@022

//		if (gu16_ColdTimeData > 60)
//		{
//			gu16_ColdTimeData = gu16_ColdTimeData - 60; 		//	1분 감소
//			Play_Voice(VOICE_BEEP_SELECT);
//		}
//		else
//		{
//			Play_Voice(VOICE_BEEP_WARNING);
//		}
	}
	else if (KEY_AMOUNT_SHORT == SET)           //@022
	{                                           //@022
		if (gu16_Cooling_TargetDegree < Z_TEMP6)//@022
		{                                       //@022
			gu16_Cooling_TargetDegree += Z_TEMP0_5; //@022
			Play_Voice(VOICE_BEEP_SELECT);      //@022
		}                                       //@022
		else                                    //@022
		{                                       //@022
			Play_Voice(VOICE_BEEP_WARNING);     //@022
		}                                       //@022

//		if (gu16_ColdTimeData < 59000)
//		{
//			gu16_ColdTimeData = gu16_ColdTimeData + 60; 		//	1분 증가
//			Play_Voice(VOICE_BEEP_SELECT);
//		}
//		else
//		{
//			Play_Voice(VOICE_BEEP_WARNING);
//		}
	}
//@022 #endif
	else if (gu16_TimerOperation_100ms == 0)
	{
		Ini_OperationStep_10();
	}
	else
	{

	}
}
void Ini_OperationStep_A0(void)
{
	gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	gu8_Timer_SEG_TH = Z_Timer_SEG_TH;
	gu8_Counter_SEG_TH = 0;
	OperationStepNoSet(Z_OperationStepNum_A0);
}

/// @brief
/// @param    void
/// @return   void
void OperationStep_B0(void)
{
	if (INPUT_REED_SW_FILTER == SET)
	{
		gu8_TimerFilterChange = Z_TimerFilterChange;
	}

	if ((gu8_TimerFilterChange == 0) && (F_First_Set == SET))	//	초기 설치 이후에 필터 교체가 이루어진 경우
	{
		gu16_Effluent_Hz = 0;
		F_FilterChange_Detect = SET;
	}

	if (INPUT_REED_SW_FAUCET == SET)
	{
		gu8_TimerWaterFlowChange = Z_TimerWaterFlowChange;
	}

	if ((gu8_TimerWaterFlowChange == 0) && (F_First_Set == SET))//	초기 설치 이후에 교체가 이루어진 경우
	{
		F_FaucetChange_Detect = SET;
	}

	if ((INPUT_REED_SW_COVER == CLEAR) || (INPUT_REED_SW_FILTER == CLEAR) || (INPUT_REED_SW_FAUCET == CLEAR))
	{
		gu8_TimerReedSWCheck_B0 = Z_TimerReedSWCheck;
	}

	if (gu8_TimerReedSWCheck_B0 == 0)
	{
//		if (F_FaucetChange_Detect == SET)
//		{
//			F_FaucetChange_Detect = CLEAR;
//			gu16_DayCount_forWaterFlow = CLEAR;
//			F_WaterFlow_Reset = SET;
//		}

//		if ((F_FilterChange_Detect == SET) && (F_KITMode == CLEAR))
//		{
//			Play_Voice(VOICE_CHANGE_FILTER);
//			Ini_OperationStep_02();
//		}
//		else
//		{
//			F_FilterChange_Detect = CLEAR;
//			Play_Voice(VOICE_BEEP_ON);
//			Ini_OperationStep_10();
//		}
		if ((F_FirstFlushing == CLEAR) || (F_Heater_Fill == CLEAR) || (F_ColdTank_Fill == CLEAR))
		{
			Play_Voice(VOICE_BEEP_ON);
			Ini_OperationStep_02();
		}
		else if ((F_FilterChange_Detect == SET) && (F_FaucetChange_Detect == SET))
		{
			Play_Voice(VOICE_CHANGE_FILTER_FLOWFAUCET);
			Ini_OperationStep_02();
		}
		else if (F_FilterChange_Detect == SET)
		{
//			F_FirstFlushing = CLEAR;
			Play_Voice(VOICE_CHANGE_FILTER);
			Ini_OperationStep_02();
		}
		else if (F_FaucetChange_Detect == SET)
		{
			Play_Voice(VOICE_CHANGE_FLOWFAUCET);
			Ini_OperationStep_02();
		}
		else
		{
			Play_Voice(VOICE_BEEP_ON);
			Ini_OperationStep_10();
		}
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		if (INPUT_REED_SW_COVER == CLEAR)
		{
			Play_Voice(VOICE_TOP_COVER_CHECK);
		}
		else if (INPUT_REED_SW_FILTER == CLEAR)
		{
			Play_Voice(VOICE_FILTER_CONNECT_CHECK_B);
		}
		else
		{
			Play_Voice(VOICE_FLOWFAUCET_CONNECT_CHECK);
		}
	}
	else if (KEY_STE_KITMODE_LONG == SET)
	{
		Play_Voice(VOICE_STERIZATION_START_ICON22);
		Ini_OperationStep_F0();
	}
//	else if ((INPUT_REED_SW_COVER_Before != INPUT_REED_SW_COVER)
//			|| (INPUT_REED_SW_FILTER_Before != INPUT_REED_SW_FILTER)
//			|| (INPUT_REED_SW_WATERFLOW_Before != INPUT_REED_SW_WATERFLOW))
//	{
//		if ((INPUT_REED_SW_COVER_Before == CLEAR) && (INPUT_REED_SW_COVER == SET))
//		{
//			if (INPUT_REED_SW_FILTER == CLEAR)
//			{
//				Play_Voice(VOICE_FILTER_CONNECT_CHECK_B);
//			}
//			else if (INPUT_REED_SW_WATERFLOW == CLEAR)
//			{
//				Play_Voice(VOICE_FLOWFAUCET_CONNECT_CHECK);
//			}
//		}
//		else
//		{
//			Play_Voice(VOICE_BEEP_ON);
//		}
//	}
	else if ((INPUT_REED_SW_COVER_Before != INPUT_REED_SW_COVER))
	{
		if (INPUT_REED_SW_COVER == SET)
		{
			if (INPUT_REED_SW_FILTER == CLEAR)
			{
				Play_Voice(VOICE_FILTER_CONNECT_CHECK_B);
			}
			else if (INPUT_REED_SW_FAUCET == CLEAR)
			{
				Play_Voice(VOICE_FLOWFAUCET_CONNECT_CHECK);
			}
			else
			{

			}
		}
		if ((INPUT_REED_SW_COVER == SET) && (INPUT_REED_SW_FILTER == SET) && (INPUT_REED_SW_FAUCET == SET))
		{

		}
		else
		{
			WifiSendData(WIFI_DATA_FUNCTION);
		}
	}
	else if ((INPUT_REED_SW_FILTER_Before != INPUT_REED_SW_FILTER))
	{
		if (INPUT_REED_SW_FILTER == SET)			//	필터 결합 행위
		{
			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				Play_Voice(VOICE_BEEP_ON);
			}
			else									//	일본향
			{
				Play_Voice(VOICE_FILTER_ON);
			}
		}
		else
		{
			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				Play_Voice(VOICE_BEEP_OFF);
			}
			else									//	일본향
			{
				Play_Voice(VOICE_FILTER_OFF);
			}
		}
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if ((INPUT_REED_SW_FAUCET_Before != INPUT_REED_SW_FAUCET))
	{
		if (INPUT_REED_SW_FAUCET == SET)			//	파우셋 결합 행위
		{
			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				Play_Voice(VOICE_BEEP_ON);
			}
			else									//	일본향
			{
				Play_Voice(VOICE_FLOWFAUCET_ON);
			}
		}
		else
		{
			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				Play_Voice(VOICE_BEEP_OFF);
			}
			else									//	일본향
			{
				Play_Voice(VOICE_FLOWFAUCET_OFF);
			}
		}
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else
	{

	}

	INPUT_REED_SW_COVER_Before = INPUT_REED_SW_COVER;
	INPUT_REED_SW_FILTER_Before = INPUT_REED_SW_FILTER;
	INPUT_REED_SW_FAUCET_Before = INPUT_REED_SW_FAUCET;
}
void Ini_OperationStep_B0(void)
{
	gu8_TimerSleepMode = Z_TimerSleepMode;
	gu8_TimerUV_6h = Z_TimerUV_6h;
	gu8_WaterSelect = gu8_DefaultWaterSelect;
	gu8_TimerFilterChange = Z_TimerFilterChange;
	gu8_TimerWaterFlowChange = Z_TimerWaterFlowChange;
	INPUT_REED_SW_COVER_Before = INPUT_REED_SW_COVER;
	INPUT_REED_SW_FILTER_Before = INPUT_REED_SW_FILTER;
	INPUT_REED_SW_FAUCET_Before = INPUT_REED_SW_FAUCET;
	gu8_TimerReedSWCheck_B0 = Z_TimerReedSWCheck;
	OperationStepNoSet(Z_OperationStepNum_B0);
}

/// @brief    냉수탱크 진공해제
/// @param    void
/// @return   void
void OperationStep_C0(void)
{
	if ((KEY_COLDTANK_PBATEST_VERYLONG == SET) || (gu16_TimerColdTank_Clear == 0) || (KEY_EFFLUENT_SHORT == SET))
	{
		Flushing_Check();
		Play_Voice(VOICE_BEEP_OFF);
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else
	{

	}
}
void Ini_OperationStep_C0(void)
{
	gu8_TimerColdTankClearDisplay = Z_TimerColdTankClearDisplay;
	gu16_TimerColdTank_Clear = Z_TimerColdTank_Clear;
	OperationStepNoSet(Z_OperationStepNum_C0);
}

/// @brief    TH 확인
/// @param    void
/// @return   void
void OperationStep_D0(void)
{
	if (KEY_EFFLUENT_SHORT == SET)
	{
		Ini_OperationStep_10();
		Play_Voice(VOICE_BEEP_ON);
	}
	else if (KEY_FLU_CAN_TH_CON_LONG == SET)		//	냉각시간 변경
	{  
	       
//#ifdef _FOR_DEVELOP_
		F_Cooling_PID ^= 1;                         //@022
		if (F_Cooling_PID == SET)                   //@022
		{                                           //@022
			Ini_OperationStep_A0();                 //@022
			Play_Voice(VOICE_SPECIAL_FUNCTION);     //@022
		}                                           //@022
		else                                        //@022
		{                                           //@022
			gu8_TimerPIOff_Display = Z_TimerPIOff_Display;	//032
			Play_Voice(VOICE_BEEP_OFF);             //@022
		}                                           //@022
//#else
//		Ini_OperationStep_10();
//		Play_Voice(VOICE_BEEP_ON);
//#endif 
          
	}
	else if (KEY_VALUE_TEM_SMPS_TEST_LONG == SET)		//	(KEY_VALUE_HOTWATER | KEY_VALUE_COLDWATER | KEY_VALUE_AMOUNT)
	{  
	        F_TEMP_A_SMPS_Voltage_Display ^= 1;
		if(F_TEMP_A_SMPS_Voltage_Display == SET)                                         
		{
			Play_Voice(VOICE_BEEP_ON);              //@013
			Ini_OperationStep_F1();
		}
		else
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
	}
	else if (KEY_COLDWATER_LONG == SET)
	{
//@013		F_FAN_NoiseClear ^= 1;
//@013		if (F_FAN_NoiseClear == SET)
//@013		{
//@013			Play_Voice(VOICE_BEEP_ON);
//@013		}
//@013		else
//@013		{
//@013			Play_Voice(VOICE_BEEP_OFF);
//@013		}
		Play_Voice(VOICE_BEEP_WARNING);				//@013
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	else if (KEY_COLDTANK_PBATEST_LONG == SET)		//@013
	{                                               //@013
		F_FAN_NoiseClear ^= 1;                      //@013
		if (F_FAN_NoiseClear == SET)                //@013
		{                                           //@013
			Play_Voice(VOICE_BEEP_ON);              //@013
		}                                           //@013
		else                                        //@013
		{                                           //@013
			Play_Voice(VOICE_BEEP_OFF);             //@013
		}                                           //@013
		gu8_TimerDisplayFanChange = Z_TimerDisplayFanChange;	//@016
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}                                               //@013
	else if (KEY_HOTWATER_LONG == SET)
	{
		if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_ModelType_forCHP == MODEL_CHP))
		{
			F_HotWater_100Degree ^= 1;
			if (F_HotWater_100Degree == SET)
			{
				gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
				gu8_DefaultHotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
				Play_Voice(VOICE_BEEP_ON);
			}
			else
			{
				gu8_HotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
				gu8_DefaultHotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
				Play_Voice(VOICE_BEEP_OFF);
			}
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	else if (KEY_ALL_LOCK_LONG == SET)
	{
		F_HotWater_DegreeDisplay ^= 1;
		gu8_TimerHotWater_DegreeDisplay = 60;			//	60분
		if (F_HotWater_DegreeDisplay == SET)
		{
			Play_Voice(VOICE_BEEP_ON);
		}
		else
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	else if (KEY_LINETEST_MANAGE_LONG == SET)
	{
		if (F_HotWater_DegreeDisplay == SET)
		{
			F_HotWater_DegreeDisplay_TH ^= 1;
			if (F_HotWater_DegreeDisplay_TH == SET)
			{
				Play_Voice(VOICE_BEEP_ON);
			}
			else
			{
				Play_Voice(VOICE_BEEP_OFF);
			}
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	else if (KEY_WIFI_ONOFF_LONG == SET)
	{
//@013		F_Cooling_Up ^= 1;
//@013		if (F_Cooling_Up == SET)
//@013		{
//@013			Play_Voice(VOICE_BEEP_ON);
//@013		}
//@013		else
//@013		{
//@013			Play_Voice(VOICE_BEEP_OFF);
//@013		}
//@013		gu8_TimerDisplayCoolingChange = Z_TimerDisplayCoolingChange;	//@016
//@013		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
		Play_Voice(VOICE_BEEP_WARNING);				//@013
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	else if (KEY_SOUND_TYPE_LONG == SET)			//@013
	{                                               //@013
		F_Cooling_Up ^= 1;                          //@013
		if (F_Cooling_Up == SET)                    //@013
		{                                           //@013
			Play_Voice(VOICE_BEEP_ON);              //@013
		}                                           //@013
		else                                        //@013
		{                                           //@013
			Play_Voice(VOICE_BEEP_OFF);             //@013
		}                                           //@013
		gu8_TimerDisplayCoolingChange = Z_TimerDisplayCoolingChange;	//@013
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;	//@013
	}
	else if (KEY_WIFI_AP_PAIRING_LONG == SET)		//@011
	{                                               //@011
		F_CoolingEcoMode_Disable ^= 1;              //@011
		if (F_CoolingEcoMode_Disable == SET)        //@011
		{                                           //@011
			Play_Voice(VOICE_BEEP_ON);              //@011
		}                                           //@011
		else                                        //@011
		{                                           //@011
			Play_Voice(VOICE_BEEP_OFF);             //@011
		}                                           //@011
		gu8_TimerDisplayEcoMode = Z_TimerDisplayEcoMode;	//@016
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}                                               //@011
	else if (KEY_PUREWATER_LONG == SET)
	{
		F_EffluentLED_OFF ^= 1;
		if (F_EffluentLED_OFF == CLEAR)
		{
			Play_Voice(VOICE_BEEP_ON);
		}
		else
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
		// [추가] 추출표시등 변경 시 앱과 즉시 상태 동기화 (A1010 전송)  JHKO
        WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (KEY_AMOUNT_LONG == SET)
	{
		F_CupSize_MySet ^= 1;
		if (F_CupSize_MySet == SET)
		{
			Ini_OperationStep_D1();
			Play_Voice(VOICE_BEEP_ON);
		}
		else
		{
    		if (gu8_DefaultCupSize == CUPSIZE_MY)
    		{
    			gu8_DefaultCupSize = CUPSIZE_ONE;
    		}
			Play_Voice(VOICE_BEEP_OFF);
		}
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	//@003	시작
	else if (KEY_SLEEPMODE_LONG == SET)
	{
		F_Repeat_Eff_Mode_forMP ^= 1;
		if (F_Repeat_Eff_Mode_forMP == CLEAR)
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
		else
		{
			Play_Voice(VOICE_BEEP_ON);
		}
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}
	//@003	끝
	else if (KEY_STE_KITMODE_LONG == SET)			//@015
	{                                               //@015
		Ini_OperationStep_31();                     //@015
		Play_Voice(VOICE_EW_START);                 //@015
	}                                               //@015
	else if (gu16_TimerOperation_100ms == 0)
	{
		Ini_OperationStep_10();
	}
	else
	{

	}
}
void Ini_OperationStep_D0(void)
{
	gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	gu8_Timer_SEG_TH = Z_Timer_SEG_TH;
	gu8_Counter_SEG_TH = 0;
	OperationStepNoSet(Z_OperationStepNum_D0);
}

/// @brief    My용량 변경
/// @param    void
/// @return   void
void OperationStep_D1(void)
{
	if (gu8_Key_Short.byte != 0x00U)
	{
		gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	}

	if (KEY_EFFLUENT_SHORT == SET)
	{
		Ini_OperationStep_10();
		Play_Voice(VOICE_MYCUP_SET);
	}
	else if (KEY_HOTWATER_HIGH_EDGE == SET)
	{
		gu8_TimerOperation_100ms = Z_TimerOperation_100ms_1s;
		if (gu16_CupSize_My > 130)
		{
			gu16_CupSize_My -= 10;
			Play_Voice(VOICE_BEEP_SELECT);
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
	else if (KEY_AMOUNT_HIGH_EDGE == SET)
	{
		gu8_TimerOperation_100ms = Z_TimerOperation_100ms_1s;
		if (gu16_CupSize_My < 990)
		{
			gu16_CupSize_My += 10;
			Play_Voice(VOICE_BEEP_SELECT);
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
	else if (gu16_TimerOperation_100ms == 0)
	{
		Ini_OperationStep_10();
		Play_Voice(VOICE_MYCUP_SET);
	}
	else
	{

	}

	if ((KEY_HOTWATER_LEVEL == LOW) && (KEY_AMOUNT_LEVEL == LOW))
	{
		gu8_TimerOperation_100ms = Z_TimerOperation_100ms_1s;
	}

	if ((KEY_HOTWATER_LEVEL == HIGH) && (gu8_TimerOperation_100ms == 0))
	{
		if (gu8_TimerOperation_10ms == 0)
		{
			if (gu16_CupSize_My > 130)
			{
				gu16_CupSize_My -= 10;
			}
			gu8_TimerOperation_10ms = Z_TimerOperation_10ms_200ms;
		}
	}

	if ((KEY_AMOUNT_LEVEL == HIGH) && (gu8_TimerOperation_100ms == 0))
	{
		if (gu8_TimerOperation_10ms == 0)
		{
			if (gu16_CupSize_My < 990)
			{
				gu16_CupSize_My += 10;
			}
			gu8_TimerOperation_10ms = Z_TimerOperation_10ms_200ms;
		}
	}
}
void Ini_OperationStep_D1(void)
{
	gu8_TimerOperation_100ms = Z_TimerOperation_100ms_1s;
	gu16_TimerOperation_100ms = Z_TimerOperation_100ms_60s;
	OperationStepNoSet(Z_OperationStepNum_D1);
}

/// @brief    에러
/// @param    void
/// @return   void
void OperationStep_E0(void)
{
	if (gu8_Key_Short.byte != 0x00)					//	키 입력이 있을 경우
	{
		gu8_TimerSleepMode = Z_TimerSleepMode;
	}

	if (((gu8_ErrorCode == Z_Ecode01)			//	누수에러 (ADC)
		|| (gu8_ErrorCode == Z_Ecode02))		//	누수에러 (Flow Sensor)
		&& (F_TestMode_PBA == CLEAR)
		&& (gu8_Key_Short.byte != 0x00))		//	테스트모드에서는 해제 가능
	{
		Play_Voice(VOICE_BEEP_WARNING);			//			에러음 발생
	}
	else if ((F_TestMode_PBA == SET) && (gu8_ErrorCode != 0x00)
			&& (gu8_ErrorCode != Z_Ecode08) && (gu8_ErrorCode != Z_Ecode09)
			&& (gu8_ErrorCode != Z_Ecode01) && (gu8_ErrorCode != Z_Ecode02)
			&& (gu8_Key_Short.byte != 0x00))	//	검사모드에서 에러발생 시 에러 해제 불가능 (누수,단수에러만 예외)
	{												//
		Play_Voice(VOICE_BEEP_WARNING);				//			에러음 발생
	}												//
	else
	{
		if ((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_HOTWATER_LONG == SET))
		{
			Hot_Lock_Setting();
		}
		else if (KEY_COLDWATER_LONG == SET)
		{
			Cold_ONOFF_Setting();
		}
		else if (((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_ALL_LOCK_LONG == SET))
				|| ((gu8_ModelType_forCHP == MODEL_CP) && (KEY_HOTWATER_LONG == SET)))
		{
			F_AllLock = SET;
			Play_Voice(VOICE_ALL_LOCK_SETTING_A);
			Ini_OperationStep_40();
			ErrorClear();
			WifiSendData(WIFI_DATA_FUNCTION);
		}
		else if (KEY_EFFLUENT_SHORT == SET)
		{
			Flushing_Check();
			Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
			Play_Voice(VOICE_BEEP_SELECT);
			ErrorClear();
		}
		else if (KEY_COLDWATER_SHORT == SET)
		{
			ColdWater_Button();
			ErrorClear();
			Flushing_Check();
		}
		else if (KEY_HOTWATER_SHORT == SET)
		{
			if (gu8_ModelType_forCHP == MODEL_CHP)
			{
				HotWater_Button();
			}
			else
			{
				Play_Voice(VOICE_BEEP_SELECT);
			}
			ErrorClear();
			Flushing_Check();
		}
		else if (KEY_PUREWATER_SHORT == SET)
		{
			PureWater_Button();
			Flushing_Check();
			ErrorClear();
		}
		else if (KEY_AMOUNT_SHORT == SET)
		{
			Flushing_Check();
			Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
			Water_Cup_SelectTimer(&gu8_TimerCupSelect);
			Play_Voice(VOICE_BEEP_SELECT);
			ErrorClear();
		}
		else if (gu8_ErrorCode == CLEAR)
		{
			Flushing_Check();
			ErrorClear();
		}
		else
		{

		}
	}
}
void Ini_OperationStep_E0(void)
{
#ifdef _FOR_REPEAT_EFF_ENDURANCE_TEST_
	F_Repeat_Eff = CLEAR;
	gu8_Counter_Repeat_Eff = 0;
#endif

	F_Repeat_Eff_forMP = CLEAR;						//@003

	F_UVCare_Oper = CLEAR;
	OperationStepNoSet(Z_OperationStepNum_E0);

	StepMotor_ZeroBack_Open();

	gu8_WaterSelect = gu8_DefaultWaterSelect;

	switch (gu8_ErrorCode)
	{
		case Z_Ecode01:
		case Z_Ecode02:
			Play_Voice(VOICE_LEAKAGE_ERROR);
			break;
		case Z_Ecode08:
		case Z_Ecode09:
			Play_Voice(VOICE_SHORTAGE_ERROR);
			break;
		case Z_Ecode29:
			Play_Voice(VOICE_CLEAN_DUST_FILTER);
			break;
		case Z_Ecode40:
			Play_Voice(VOICE_HOT_WATER_ERROR);
			break;
		case Z_Ecode43:
			Play_Voice(VOICE_BEEP_WARNING);
			break;
		case Z_Ecode25:
		case Z_Ecode44:
			Play_Voice(VOICE_COLD_WATER_ERROR);
			break;
		case Z_Ecode46:
		case Z_Ecode47:
		case Z_Ecode48:
			Play_Voice(VOICE_HOT_WATER_ERROR);
			break;
		case Z_EcodeA0:
			Play_Voice(VOICE_BEEP_WARNING);
			break;
		default:
			Play_Voice(VOICE_BEEP_WARNING);
			break;
	}

}

/// @brief    에러이력
/// @param    void
/// @return   void
void OperationStep_E1(void)
{
	U8 i = 0;
	if (gu8_Key_Short.byte != 0x00)
	{
		Flushing_Check();
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_ERROR_HIS_LONG == SET)
	{
//		RamClear();
//		F_EEPROM_Forced_Initialize = SET;
//		Ini_OperationStep_10();
		for (i = 0 ; i < Z_ErrorHistory ; i++)
		{
			gu8_ErrorHistory[i] = CLEAR;
		}
		Flushing_Check();
		Play_Voice(VOICE_BEEP_ON);
	}
	else if (gu16_TimerOperation_100ms == 0)
	{
		Flushing_Check();
	}
	else
	{

	}
}
void Ini_OperationStep_E1(void)
{
	gu16_TimerOperation_100ms = Z_TimerOperation_100ms_120s;
	gu8_TimerErrorHistory = Z_TimerErrorHistory;
	gu8_CounterErrorHistory = 0;
	OperationStepNoSet(Z_OperationStepNum_E1);
}

/// @brief    살균키트 모드 (정수살균 30초 → 냉수살균 2분 → 냉수헹굼 2분 → 정수헹굼 30초)
/// @param    void
/// @return   void
void OperationStep_F0(void)
{
	if ((F_KITMode_VoiceRinse == CLEAR) && (gu16_TimerKITMode < (Z_TimerKITMode/2)))
	{
		F_KITMode_VoiceRinse = SET;
		Play_Voice(VOICE_STERIZATION_RINSE_ICON22);
	}

	if (gu8_ErrorCode != 0)
	{
		Ini_OperationStep_E0();
	}
	else if (gu16_TimerKITMode == 0)
	{
		Play_Voice(VOICE_STERIZATION_END_ICON22);
		Ini_OperationStep_B0();
	}
	else if (KEY_STE_KITMODE_LONG == SET)
	{
		Play_Voice(VOICE_BEEP_OFF);
		Ini_OperationStep_B0();
	}
	else if (gu8_Key_Short.byte != 0x00)
	{
		Play_Voice(VOICE_BEEP_WARNING);
	}
	else
	{

	}
}
void Ini_OperationStep_F0(void)
{
	F_KITMode = SET;
	F_KITMode_VoiceRinse = CLEAR;
	gu8_WaterSelect = PURE_WATER;
	gu16_TimerKITMode = Z_TimerKITMode;
	OperationStepNoSet(Z_OperationStepNum_F0);
}

/// @brief    전압, 전류 피드백 확인 모드
/// @param    void
/// @return   void
void OperationStep_F1(void)
{
	ad_feedback_mode_ad_sensing();

	if(gu8_Timer_AdFeedback_Mode_Exit == 0)
	{
		Play_Voice(VOICE_BEEP_OFF);
		gu8_f_ad_feedback_mode = CLEAR;
		gu8_Timer_Forced_TemOutput = 0;
		Ini_OperationStep_10();
	}
}
void Ini_OperationStep_F1(void)
{
	gu8_Timer_AdFeedback_Mode_Exit = 100;
	gu8_Timer_AdFeedback_Loading = Z_TIMER_AD_FEEDBACK_DISPLAY;
	gu8_f_ad_feedback_mode = SET;
	gu8_Timer_Forced_TemOutput = 50;
	OperationStepNoSet(Z_OperationStepNum_F1);
}

/// @brief    운전스텝 설정
/// @param    void
/// @return   void
void OperationStepNoSet(U8 data)
{
	static const U8 DT_OperationNumber[]
	= {
		Z_OperationStep_00,
		Z_OperationStep_01,
		Z_OperationStep_02,
		Z_OperationStep_03,
		Z_OperationStep_04,
		Z_OperationStep_05,
		Z_OperationStep_10,
		Z_OperationStep_15,
		Z_OperationStep_16,
		Z_OperationStep_17,
		Z_OperationStep_18,
		Z_OperationStep_20,
		Z_OperationStep_30,
		Z_OperationStep_31,
		Z_OperationStep_40,
		Z_OperationStep_41,
		Z_OperationStep_42,
		Z_OperationStep_50,
		Z_OperationStep_70,
		Z_OperationStep_80,
		Z_OperationStep_90,
		Z_OperationStep_91,
		Z_OperationStep_92,
		Z_OperationStep_93,
		Z_OperationStep_A0,
		Z_OperationStep_B0,
		Z_OperationStep_C0,
		Z_OperationStep_D0,
		Z_OperationStep_D1,
		Z_OperationStep_E0,
		Z_OperationStep_E1,
		Z_OperationStep_F0,
		Z_OperationStep_F1,
	};
	/************************************************/
	gu8_OperationStepNum = data; /*	일련 번호 set*/

	if (gu8_OperationStep == Z_OperationStep_50)	//	추출 상태에서 다른 상태로 이행하는 경우 (스마트체크 가능 전송)
	{
		WifiSendData(WIFI_DATA_FUNCTION);
	}

	if (((gu8_OperationStep == Z_OperationStep_03) || (gu8_OperationStep == Z_OperationStep_04) || (gu8_OperationStep == Z_OperationStep_05))
		&& ((gu8_OperationStepNum == Z_OperationStepNum_02) || (gu8_OperationStepNum == Z_OperationStepNum_10) || (gu8_OperationStepNum == Z_OperationStepNum_E0)))	//	플러싱 상태에서 다른 상태로 이행하는 경우
	{												//	 (스마트체크 가능 전송)
		WifiSendData(WIFI_DATA_FUNCTION);
	}

	if ((gu8_OperationStep == Z_OperationStep_50) && (DT_OperationNumber[gu8_OperationStepNum] == Z_OperationStep_10))	//	추출 상태에서 대기모드로 가는 경우
	{
		F_UVCare_Oper = SET;
		gu8_TimerUV_Delay = Z_TimerUV_Delay;
		gu8_TimerEcode43_Display = Z_TimerEcode43_Display;
	}

	if ((gu8_OperationStep == Z_OperationStep_10) && (DT_OperationNumber[gu8_OperationStepNum] == Z_OperationStep_50))	//	추출 상태로 이행하는 경우
	{
		gu16_Effluent_Hz_Amount_forWIFI = 0;
//		gu16_Effluent_Hz_Amount = 0;
		gu16_TimerSilverCare = Z_TimerSilverCare;	//	실버케어 48시간
		F_SilverCare_Detect = CLEAR;
	}

	if ((gu8_OperationStep == Z_OperationStep_10) && (DT_OperationNumber[gu8_OperationStepNum] != Z_OperationStep_10))	//	대기 상태에서 다른 상태로 이행하는 경우
	{
		F_UVCare_Oper = CLEAR;
	}

	gu8_OperationStep = DT_OperationNumber[gu8_OperationStepNum]; /*	상태 mode No. set*/
}

/// @brief	   전원 인가시 최초 물 선택
/// @param    void
/// @return   void
static void FirstWaterSelectOperation(void)
{
	if (F_ColdOff == CLEAR)						//	냉각 ON인 경우
	{
		gu8_WaterSelect = COLD_WATER;			//		냉수
		gu8_DefaultWaterSelect = COLD_WATER;
	}
	else
	{
		gu8_WaterSelect = PURE_WATER;
		gu8_DefaultWaterSelect = PURE_WATER;
	}
	Ini_OperationStep_10();
}

/// @brief	  설정된 워터로 이행
/// @param    void
/// @return   void
//static void WaterSelectOperation(void)
//{
//
//}

/// @brief	  운전상태 공통처리
/// @param    void
/// @return   void
static void Operation_Common(void)
{
	U8 search_ok = 0;
	U8 mu8_i = 0;
	U8 mu8_start_index = 0;


	// ==========================================  JHKO_260417
	U8 mu8_WifiStatus = GetWifiStatusValue(WIFI_STATUS_DISP);

	// ==========================================
	// [추가] Wi-Fi 연결 성공 시 최초 1회 페어링 이력 기록
	if (mu8_WifiStatus == WIFI_DISP_CONNECT)
	{
		if (gu8_WifiPairingHistory == 0) // 아직 기록이 없다면
		{
			gu8_WifiPairingHistory = 1;
			//EEPROM_ConfirmWrite(EEPROM_ADDR_WIFI_PAIRING_HISTORY, gu8_WifiPairingHistory);
		}
	}

	// [수정] Wi-Fi 미연결 상태이면서 + '최초 페어링 전'일 때만 강제 활성화
	// 한번이라도 페어링 되었다면 이 로직은 건너뜁니다. (앱 설정 유지)
	if ((mu8_WifiStatus == WIFI_DISP_OFF) && (gu8_WifiPairingHistory == 0))
	{
		for (mu8_i = 0; mu8_i <= CUPSIZE_FIVE; mu8_i++) {
			gu8_Cup_size_Array[mu8_i].enable = 1;
		}
	}
	// ==========================================JHKO_260417
	
	
#ifdef _FRONT_JIG_PROGRAM_

#else
	if (gu8_TimerCupSelect == 0)
	{
		//gu16_CupSize = gu8_DefaultCupSize;
		return_default_cup();
	}
#ifndef _HOT_TEST_
#ifndef _FOR_REPEAT_EFF_ENDURANCE_TEST_
	if (gu8_TimerWaterSelect == 0)
	{
		if (gu8_ModelType_forNational == MODEL_JAPAN)
		{
			F_HotLock = SET;
		}
		//gu8_HotTempSetup = gu8_DefaultHotTempSetup;
		//gu8_WaterSelect = gu8_DefaultWaterSelect;
		#if 0
		gu8_HotTempSetup = gu8_DefaultHotTempSetup;
		#endif
		return_default_hot_temp_setup();	
		gu8_WaterSelect = gu8_DefaultWaterSelect;

	}

	if (gu8_WaterSelect != HOT_WATER)
	{
		//gu8_HotTempSetup = gu8_DefaultHotTempSetup;
		#if 0
		gu8_HotTempSetup = gu8_DefaultHotTempSetup;
		#endif
		return_default_hot_temp_setup();
	}
#endif
#endif

#ifdef _FOR_COLD_ONOFF_TEST_
	if (F_Repeat_Eff == SET)
	{
		if (F_ColdOff == CLEAR)
		{
			if ((gu8_CoolingStep == Z_CoolingStep_20) && (gu8_TimerTestColdOff == 0) && (F_CoolingRapid_Complete == CLEAR))
			{
				gu8_TimerTestColdOff = 300;	//	300초
				gu8_TimerTestColdOn = 10;	//	10초
				KEY_COLDWATER_LONG = SET;	//	냉각 OFF
			}
		}
		else
		{
			if (gu8_TimerTestColdOn == 0)
			{
				KEY_COLDWATER_LONG = SET;	//	냉각 ON
			}
		}
	}
	else
	{
		gu8_TimerTestColdOff = 300;	//	300초
	}
#endif

	if (gu16_TimerSilverCare == 0)					//	실버케어 타이머 경과 시
	{
		if (F_SilverCare_Detect == CLEAR)			//		실버케어 감지 unset인 경우
		{
			F_SilverCare_Notice = SET;				//			WIFI 실버케어 알림
			WifiSendData(WIFI_DATA_FUNCTION);
		}
		F_SilverCare_Detect = SET;					//			실버케어 상태 flag set
	}
	

	if (KEY_FLU_CAN_TH_CON_LONG == SET)
	{
		Ini_OperationStep_D0();
		Play_Voice(VOICE_SPECIAL_FUNCTION);
	}
	else if (gu8_ErrorCode != 0)					//	에러 발생인 경우
	
	
	//if (gu8_ErrorCode != 0)					//	에러 발생인 경우
	{
		Ini_OperationStep_E0();
	}
	else if (F_AllLock == SET)						//	전체잠금 인 경우
	{
		Ini_OperationStep_40();
	}
	else if (KEY_EFFLUENT_SHORT == SET)				//	 	추출버튼
	{
#ifdef _FOR_REPEAT_EFF_ENDURANCE_TEST_
		if (gu16_Timer_Repeat_Eff != 0)
		{
			if (F_Repeat_Eff == SET)
			{
				F_Repeat_Eff = CLEAR;
				Play_Voice(VOICE_EFFLUENT_COMPLETE);
				return;
			}
		}
		else
		{
			F_ColdOff = CLEAR;
			F_HotLock = CLEAR;
			F_Repeat_Eff = SET;
#ifdef _FOR_WQA_
			gu8_CupSize = CUPSIZE_CONTINUE;
			//gu8_DefaultCupSize = CUPSIZE_CONTINUE;
			gu8_DefaultCupSize = CUPSIZE_FIVE;
#endif
		}
#endif
#ifdef _FOR_HOTWATER_1CUP_EFF_30s_DELAY_
		gu8_CupSize = CUPSIZE_1CUP;
		gu8_WaterSelect = HOT_WATER;
#endif
#ifdef _FOR_FOREVER_EFF_
		gu8_CupSize = CUPSIZE_CONTINUE;
#endif
		///////////////////20260310
		#if 1
	        gu16_HeatingTime = 0;
	        gu16_HotEffluentTime = 0;
	    	#endif
		///////////////
		//@003	시작
		if ((F_Repeat_Eff_Mode_forMP == SET) && (F_Repeat_Eff_forMP == CLEAR))
		{
			F_Repeat_Eff_forMP = SET;
			gu16_Timer_Repeat_Delay_forMP = 15;	//	1.5초
			gu16_Timer_Repeat_Eff_forMP = 3;	//	3초
			Play_Voice(VOICE_BEEP_ON);
			return;
		}
		//@003	끝

		Ini_OperationStep_50();
		if (gu8_WaterSelect == HOT_WATER)
		{
			if (F_FirstHotEffluent == CLEAR)		//	첫잔 미 완료 시
			{
#ifndef _KOREA_NEW_VOICE_
				Play_Voice(VOICE_HOTWATER_READY);
#else
				Play_Voice(VOICE_HOTWATER_FIRST);	//
#endif
			}
//			else
//			{
			//	(※ 온수는 HotStep에서 음성 출력. 일본향은 온수에열시간이 길기에 온수온도를 맞추고 있다라는 음성떄문에 이리 처리함)
//				Play_Voice(VOICE_HOTWATER_EFFLUENT_A);	// 일본향은 0x3F 음성 활용 (온수 온도를 맞추고 있습니다.)
//			}
		}
		else
		{
			if (gu16_CupSize == CUPSIZE_CONTINUE)	//		연속 추출 인 경우
			{
				Play_Voice(VOICE_CONTIUNE_EFFLUENT);
			}
			else
			{
				Play_Voice(VOICE_EFFLUENT_START);
			}
		}
#ifdef _EFF_Continue_Heater_OFF
		if ((gu16_CupSize == CUPSIZE_ONE_BOTTLE) || (gu16_CupSize == CUPSIZE_CONTINUE))
		{
			F_Eff_Continue_Heater_Off = SET;
		}
#endif
	}
	else if (KEY_COLDWATER_SHORT == SET)		//	냉수버튼
	{
		ColdWater_Button();
	}
	else if (KEY_COLDWATER_LONG == SET)
	{
		Cold_ONOFF_Setting();
	}
	else if (KEY_HOTWATER_SHORT == SET)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			if (F_HotLock == SET)
			{
				gu8_TimerHotLock_Display = Z_TimerHotLock_Display;
				Play_Voice(VOICE_HOT_LOCK_CHECK);
			}
			else
			{
				HotWater_Button();
			}
		}
		else
		{
			Play_Voice(VOICE_BEEP_SELECT);
		}
	}
	else if ((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_HOTWATER_LONG == SET))
	{
		Hot_Lock_Setting();
	}
	else if (KEY_PUREWATER_SHORT == SET)
	{
		PureWater_Button();
	}
	else if (KEY_AMOUNT_SHORT == SET)
	{
		gu8_TimerDefaultCupSize_Set_LEDFlic = 0;	//		디폴트설정 점멸타이머 clear

		if (gu8_TimerSleepMode == 0)
		{
			Play_Voice(VOICE_BEEP_SELECT);
		}
		else
		{
			if ((gu8_WaterSelect == HOT_WATER) && (F_FirstHotEffluent == CLEAR))
			{
				#if 0
				gu16_CupSize = CUPSIZE_HALFCUP;
				#endif
				Play_Voice(VOICE_BEEP_WARNING);
			}
			else
			{
				if (gu8_ModelType_forNational == MODEL_KOREA)
				{
					if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET))
					{
						F_HotDegreeDisplay = CLEAR;
					}
					else
					{
						#if 0					
						if (F_CupSize_MySet == CLEAR)
						{
							gu16_CupSize++;

							while (gu16_Setting_Amount_Value[gu16_CupSize] == 0)
							{
								gu16_CupSize++;

								if (gu16_CupSize > CUPSIZE_CONTINUE)
								{
									gu16_CupSize = CUPSIZE_HALFCUP;
								}
							}
						}
						else
						{
#ifdef _AFTER_LPP_IMDCHANGE_
							if (++gu16_CupSize > CUPSIZE_MY)
							{
								gu16_CupSize = CUPSIZE_HALFCUP;
							}
#else
							switch (gu16_CupSize)
							{
								case CUPSIZE_HALFCUP:
									gu16_CupSize = CUPSIZE_1CUP;
									break;
								case CUPSIZE_1CUP:
									gu16_CupSize = CUPSIZE_2CUP;
									break;
								case CUPSIZE_2CUP:
									gu16_CupSize = CUPSIZE_MY;
									break;
								case CUPSIZE_CONTINUE:
								default:
									gu16_CupSize = CUPSIZE_HALFCUP;
									break;
								case CUPSIZE_MY:
									gu16_CupSize = CUPSIZE_CONTINUE;
									break;
							}
#endif
						}
						#endif
						
							// if (++gu8_CupIndex > CUPSIZE_FIVE)
							// {
							// 	gu8_CupIndex = CUPSIZE_ONE;
							// }
							#if 0
							for(mu8_i = 0; mu8_i < CUPSIZE_FIVE; mu8_i++)  // 최대 5번만 검색
							{
								if (++gu8_CupIndex > CUPSIZE_FIVE)
								{
									gu8_CupIndex = CUPSIZE_ONE;
								}
								
								if(gu8_Cup_size_Array[gu8_CupIndex].enable == 1)
								{
									search_ok = 1;
									break;
								}
							}
							#endif
							// 먼저 증가 (현재 값 건너뛰기)
							if (++gu8_CupIndex > CUPSIZE_FIVE)
							{
								gu8_CupIndex = CUPSIZE_ONE;
							}
							mu8_start_index = gu8_CupIndex;  // 시작 인덱스 저장

							// 5번 순차 탐색
							for(mu8_i = 0; mu8_i < CUPSIZE_FIVE; mu8_i++)
							{
								// 현재 값이 enable인지 체크
								if(gu8_Cup_size_Array[gu8_CupIndex].enable == 1)
								{
									search_ok = 1;
									break;
								}
								
								// 다음 값으로 이동
								if (++gu8_CupIndex > CUPSIZE_FIVE)
								{
									gu8_CupIndex = CUPSIZE_ONE;
								}
								
								// 시작점으로 돌아왔으면 종료 (5번 체크 완료)
								if(gu8_CupIndex == mu8_start_index)
								{
									break;
								}
							}
							
							#if 0
							if(search_ok == 0)
							{
								gu8_CupIndex = 0;
							}
							#endif

							
							if(gu8_CupIndex != CUPSIZE_FIVE)		//	연속 아니면..
							{
								gu16_CupSize = (U16)gu8_Cup_size_Array[gu8_CupIndex].amount_volume * 10;		//	x 10
							}
							else
							{
								gu16_CupSize = CUPSIZE_CONTINUE;			//	연속은 0xAA로...
							}

							#if 0
							//	flowmeter.c에서 120, 250, 500, 1000 제외 계산하기 위함...
							if((gu16_CupSize != 120) && (gu16_CupSize != 250)
							&& (gu16_CupSize != 500) && (gu16_CupSize != 1000))
							{
								gu8_F_CupSize_FineAdjust = SET;					
							}
							else
							{
								gu8_F_CupSize_FineAdjust = CLEAR;
							}
							#endif
							gu8_F_CupSize_FineAdjust = SET;
					}
					Play_Voice(VOICE_BEEP_SELECT);
				}
				else										//	일본향
				{
//#ifndef _FOR_JAPAN_90DEGREE_CONTINUE
//					if ((gu8_WaterSelect == HOT_WATER) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))
//					{
//						gu8_CupSize = CUPSIZE_HALFCUP;
//						gu8_TimerCupSize_LEDFlic_forJAPAN_Limit = Z_TimerCupSize_LEDFlic_forJAPAN_Limit;
//						Play_Voice(VOICE_BEEP_WARNING);
//					}
//					else
//					{
//#endif
						gu8_TimerCupSize_LEDFlic_forJAPAN_Limit = 0;
						if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET))
						{
							F_HotDegreeDisplay = CLEAR;
						}
						else
						{
//							if (++gu8_CupSize > CUPSIZE_CONTINUE)
//							{
//								gu8_CupSize = CUPSIZE_HALFCUP;
//							}
							if (F_CupSize_MySet == CLEAR)
							{
								if (++gu16_CupSize > CUPSIZE_CONTINUE)
								{
									gu16_CupSize = CUPSIZE_HALFCUP;
								}
							}
							else
							{
								//if (++gu8_CupSize > CUPSIZE_MY)
								if (++gu16_CupSize > CUPSIZE_MY)
								{
									//gu8_CupSize = CUPSIZE_HALFCUP;
									gu16_CupSize = CUPSIZE_HALFCUP;
								}
							}
						}
						Play_Voice(VOICE_BEEP_SELECT);
//#ifndef _FOR_JAPAN_90DEGREE_CONTINUE
//					}
//#endif
				}
			}
		}
		Water_Cup_SelectTimer(&gu8_TimerCupSelect);
	}
	else if (KEY_AMOUNT_LONG == SET)
	{
		//if (gu8_CupSize != CUPSIZE_CONTINUE)
		if (gu16_CupSize != CUPSIZE_CONTINUE)
		{
			//gu8_DefaultCupSize = gu8_CupSize;
			gu8_DefaultCupSize = gu8_CupIndex;
			gu8_TimerDefaultCupSize_Set_LEDFlic = Z_TimerDefaultCupSize_Set_LEDFlic;	//			물양 설정 점멸
			gu8_TimerCupSelect = Z_TimerCupSelect;    //			물양 설정 시 7-Seg 점등을 위하여 타이머 set
			Play_Voice(VOICE_BEEP_ON);
			WifiSendData(WIFI_DATA_FUNCTION);
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
	else if (((gu8_ModelType_forCHP == MODEL_CHP) && (KEY_ALL_LOCK_LONG == SET))
			|| ((gu8_ModelType_forCHP == MODEL_CP) && (KEY_HOTWATER_LONG == SET)))
	{
		F_VeryLongKey_Rejection = SET;
		F_AllLock = SET;
		WifiSendData(WIFI_DATA_FUNCTION);
		Play_Voice(VOICE_ALL_LOCK_SETTING_A);
		Ini_OperationStep_40();
	}
	else if ((F_FirstFlushing == CLEAR) || (F_Heater_Fill == CLEAR) || (F_ColdTank_Fill == CLEAR))
	{
		Ini_OperationStep_02();
	}
	else if (gu8_TimerUV_6h == 0)
	{
		Ini_OperationStep_31();
	}
	else if (KEY_WIFI_ONOFF_LONG == SET)
	{
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
//			Play_Voice(VOICE_WIFI_OFF);
			WifiKey(WIFI_KEY_ONOFF);
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
	else if (KEY_WIFI_AP_PAIRING_LONG == SET)
	{
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
//			Play_Voice(VOICE_WIFI_START_A);
			WifiKey(WIFI_KEY_AP);
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);
			if (gu8_WifiError != 0)
			{
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;
			}
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
	}
	else if (KEY_PUREWATER_LONG == SET)
	{
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
//			Play_Voice(VOICE_WIFI_START_A);
			WifiKey(WIFI_KEY_BLE);
			gu8_WifiError = GetWifiStatusValue(WIFI_STATUS_ERROR);
			if (gu8_WifiError != 0)
			{
				gu8_TimerWifiErrorDisplay = Z_TimerWifiErrorDisplay;
			}
		}
		else
		{
			Play_Voice(VOICE_BEEP_WARNING);
		}
		WifiSendData(WIFI_DATA_FUNCTION);  //JHKO
	}
	else if ((F_SleepMode == SET) && (gu8_TimerSleepMode == 0))
	{
		Ini_OperationStep_20();
	}
	else if (KEY_SLEEPMODE_LONG == SET)
	{
		F_SleepMode ^= 1;
		F_SleepMode_Display = SET;
		Ini_OperationStep_17();
		if (F_SleepMode == SET)
		{
			Play_Voice(VOICE_BEEP_ON);
		}
		else
		{
			Play_Voice(VOICE_BEEP_OFF);
		}
		WifiSendData(WIFI_DATA_FUNCTION);
	}
	else if (KEY_SOUND_TYPE_LONG == SET)
	{
		Ini_OperationStep_41();
	}
	else if (KEY_COLDTANK_PBATEST_VERYLONG == SET)
	{
		Ini_OperationStep_C0();
		Play_Voice(VOICE_BEEP_ON);
	}
	else if (KEY_LINETEST_MANAGE_VERYLONG == SET)
	{
		Ini_OperationStep_42();
		Play_Voice(VOICE_BEEP_ON);
	}
	else
	{

	}

	if (gu8_Key_Short.byte != 0x00)			//	키 입력이 있을 경우 추출유량 타이머 해지
	{
		gu8_TimerEffluentAmount_Display = 0;
		gu8_TimerSleepMode = Z_TimerSleepMode;
		gu8_TimerUV_6h = Z_TimerUV_6h;
		gu8_TimerUV_Delay = Z_TimerUV_Delay;
		if (KEY_HOTWATER_SHORT != SET)
		{
			gu8_TimerOverHeatDisplay = 0;
		}
	}
	if (gu32_LongKeyArray.LONG != 0x0000U)
	{
		gu8_TimerSleepMode = Z_TimerSleepMode;
		gu8_TimerUV_6h = Z_TimerUV_6h;
		gu8_TimerUV_Delay = Z_TimerUV_Delay;
	}
#endif
}

/// @brief	    물 및 컵 설정 타이머 관리
/// @param    void
/// @return   void
static void Water_Cup_SelectTimer(U8* address)
{
	if (address == &gu8_TimerWaterSelect)			//	물 선택 타이머인 경우
	{
		gu8_TimerWaterSelect = Z_TimerWaterSelect;	//		물 선택 타이머 set
		if (gu8_TimerCupSelect != 0)				//		컵 설정 중 7초 이내인 경우
		{
			gu8_TimerCupSelect = Z_TimerCupSelect;	//			7초 reset
		}
	}
	else											//	컵 선택 타이머인 경우
	{
		gu8_TimerCupSelect = Z_TimerCupSelect;		//		컵 선택 타이머 set
		if (gu8_TimerWaterSelect != 0)				//		물 선택 중 7초 이내인 경우
		{
			gu8_TimerWaterSelect = Z_TimerWaterSelect;	//			7초 reset
		}
	}
}

/// @brief
/// @param    void
/// @return   void
static void Cold_ONOFF_Setting(void)
{
	F_ColdOff ^= 1;
//	F_ColdOff_Display = SET;
//	Ini_OperationStep_17();
	if (F_ColdOff == CLEAR)
	{
		Ini_OperationStep_15();
//		if (gu8_ModelType_forNational == MODEL_KOREA)
//		{
			Play_Voice(VOICE_COLD_ON_SETTING);
//		}
//		else
//		{
//			Play_Voice(VOICE_COLD_ON_SETTING_JP);
//		}
	}
	else
	{
		Ini_OperationStep_18();
		gu8_WaterSelect = PURE_WATER;
		gu8_DefaultWaterSelect = PURE_WATER;
		Play_Voice(VOICE_COLD_OFF_SETTING);
	}
	WifiSendData(WIFI_DATA_FUNCTION);
}

/// @brief
/// @param    void
/// @return   void
static void Hot_Lock_Setting(void)
{
	F_HotLock ^= 1;
	if (F_HotLock == CLEAR)					//			온수 잠금상태가 아니면
	{
//		if (gu8_ModelType_forNational == MODEL_KOREA)	//	일본향은 해제해도 온수선택 안되도록
//		{
			gu8_WaterSelect = HOT_WATER;
//		}
		F_HotDegreeDisplay = SET;
		return_default_hot_temp_setup();
		Play_Voice(VOICE_HOT_LOCK_CLEAR);
	}
	else
	{
		gu8_WaterSelect = PURE_WATER;
		Play_Voice(VOICE_HOT_LOCK_SETTING);
	}
	Water_Cup_SelectTimer(&gu8_TimerWaterSelect);    //			물선택 타이머 set
	WifiSendData(WIFI_DATA_FUNCTION);
}

/// @brief
/// @param    void
/// @return   void
#define HOT_TEMP_INVALID 0
static void HotWater_Button(void)
{
	if (F_OverHeatError == CLEAR)
	{
		if (gu8_WaterSelect == HOT_WATER)
		{
			if (F_FirstHotEffluent == SET)
			{
#ifndef _HOT_TEMP_4STEP_
				if (++gu8_HotTempSetup > HOT_TEMP_SETUP_KOR45_JPX)
				{
					gu8_HotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
				}
				else
				{

				}
#else
				if (F_HotDegreeDisplay == CLEAR)
				{
					F_HotDegreeDisplay = SET;
				}
				else
				{
					if (gu8_ModelType_forNational == MODEL_KOREA)	//	국내향
					{
						if (F_HotWater_100Degree == SET)  //HotTemp_Setting
						//if (selected_hot_temp[0] != HOT_TEMP_INVALID)  
						{       /*
							if (++gu8_HotTempSetup > HOT_TEMP_SETUP_KOR45_JPX)
							{
								gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
							}
							*/
							if(gu8_f_hot_temp_sorting == Z_HOT_TEMP_ASCENDING)				//	오름 차순		
							{
								//cbr_hot_test 2025-09-03
								gu8_hot_water_select_index++;

								if(gu8_hot_water_select_index > (gu8_select_hot_temp_total_count - 1))		//	최대 갯수
								{
									gu8_hot_water_select_index = 0;
								}
								else{}

								gu8_HotTempSetup = selected_hot_temp[gu8_hot_water_select_index];
							}
							else																//	내림차순
							{
								if(gu8_hot_water_select_index != 0)
								{
									gu8_hot_water_select_index--;
								}	
								else
								{
									gu8_hot_water_select_index = (gu8_select_hot_temp_total_count - 1);
								}

								gu8_HotTempSetup = selected_hot_temp[gu8_hot_water_select_index];
							}
							
						}
						else
						{
							if (++gu8_HotTempSetup > HOT_TEMP_SETUP_KOR45_JPX)
							{
								gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
							}
						}
					}
					else							//	일본향
					{
//						if (++gu8_HotTempSetup > HOT_TEMP_SETUP_KOR70_JP70)
//						{
//							gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
//						}
						if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
						{
							gu8_HotTempSetup = HOT_TEMP_SETUP_KOR70_JP70;
						}
						else
						{
							gu8_HotTempSetup--;
						}
					}
				}
#endif
				Play_Voice(VOICE_BEEP_SELECT);

//#ifndef _FOR_JAPAN_90DEGREE_CONTINUE
//				if ((gu8_ModelType_forNational == MODEL_JAPAN) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))	//	일본향 90도 반컵만 설정 가능
//				{
//					gu8_CupSize = CUPSIZE_HALFCUP;
//				}
//#endif
			}
			else									//	온수 선택 중에 and 첫잔 미완료 상태에서 온수버튼 누를 경우
			{
#ifndef _KOREA_NEW_VOICE_
				Play_Voice(VOICE_BEEP_WARNING);
#else
				Play_Voice(VOICE_HOTWATER_FIRST);	//
#endif
			}
		}
		else
		{
			gu8_WaterSelect = HOT_WATER;
			F_HotDegreeDisplay = SET;
			//cbr_hot_test 2025-09-03
			#if 0
			gu8_hot_water_select_index = 0;
			#endif
			return_default_hot_temp_setup();
//			if ((gu8_ModelType_forNational == MODEL_JAPAN) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))
//			{
//				gu8_CupSize = CUPSIZE_HALFCUP;
//			}
			Play_Voice(VOICE_BEEP_SELECT);
		}
		Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
	}
	else
	{
		gu8_TimerOverHeatDisplay = Z_TimerOverHeatDisplay;
		Play_Voice(VOICE_BEEP_WARNING);
	}
//#ifdef _HOT_TEST_
	gu8_TimerHotTest_DegreeDisplay = 0;
//#endif
}

/// @brief
/// @param    void
/// @return   void
static void PureWater_Button(void)
{
	U8 mu8_i =0;
	F_HotDegreeDisplay = CLEAR;
	gu8_WaterSelect = PURE_WATER;
	gu8_DefaultWaterSelect = PURE_WATER;

	Water_Cup_SelectTimer(&gu8_TimerWaterSelect);
	Play_Voice(VOICE_BEEP_SELECT);
}

/// @brief
/// @param    void
/// @return   void
static void ColdWater_Button(void)
{
	U8 mu8_i =0;
	F_HotDegreeDisplay = CLEAR;
	if (F_ColdOff == SET)					//		냉각 OFF라면
	{
		Ini_OperationStep_16();				//			OFF표시
		Play_Voice(VOICE_COLD_OFF_CHECK);
	}
	else									//		냉각 ON
	{
		gu8_DefaultWaterSelect = COLD_WATER;
		Play_Voice(VOICE_BEEP_SELECT);
	}
	gu8_WaterSelect = COLD_WATER;

	Water_Cup_SelectTimer(&gu8_TimerWaterSelect);    //			물선택 타이머 set
}

/// @brief
/// @param    void
/// @return   void
static void Flushing_Check(void)
{
	if ((F_FirstFlushing == CLEAR) || (F_ColdTank_Fill == CLEAR) || (F_Heater_Fill == CLEAR))
	{
		Ini_OperationStep_02();
	}
	else
	{
		Ini_OperationStep_10();
	}
}

/// @brief
/// @param    void
/// @return   void
void return_default_cup(void)
{
	gu8_CupIndex = gu8_DefaultCupSize;

	if((U16)gu8_Cup_size_Array[gu8_DefaultCupSize].amount_volume == CUPSIZE_CONTINUE)
	{
		gu16_CupSize = CUPSIZE_CONTINUE;
	}
	else
	{
		gu16_CupSize = (U16)gu8_Cup_size_Array[gu8_DefaultCupSize].amount_volume * 10;	
	}
}

/// @brief
/// @param    void
/// @return   void
void return_default_hot_temp_setup(void)
{
	gu8_hot_water_select_index = gu8_DefaultHotTempSetup;
	gu8_HotTempSetup = selected_hot_temp[gu8_hot_water_select_index];
}

/// @brief	WiFi에서 선택한 용량이 비활성화된 경우 다음 활성화된 용량을 찾아 반환하는
/// @param    void
/// @return   void
void Update_DefaultCupSize_If_Disabled(void)
{
    U8 i = 0;
    U8 min_volume_index = CUPSIZE_ONE;
    U8 min_volume_value = 0xFF;
    U8 found_active = 0;

    // 연속 모드(CUPSIZE_FIVE)는 default 물양으로 선택되지 않도록 제외
    if(gu8_DefaultCupSize == CUPSIZE_FIVE)
    {
        // 연속 모드로 설정되어 있으면 가장 작은 용량으로 변경
        found_active = 0;
    }
    else
    {
        // 현재 기본 컵사이즈가 활성화되어 있으면 그대로 유지
        if(gu8_Cup_size_Array[gu8_DefaultCupSize].enable == 1)
        {
            return;  // 변경 불필요
        }
        
        // 비활성화되어 있으면 가장 작은 용량 찾기
        found_active = 0;
    }
    
    // 활성화된 용량 중 가장 작은 용량 찾기 (연속 모드 제외)
    for(i = CUPSIZE_ONE; i <= CUPSIZE_FOUR; i++)
    {
        // 활성화된 용량이고 연속 모드가 아니면
        if((gu8_Cup_size_Array[i].enable == 1) && (i != CUPSIZE_FIVE))
        {
            found_active = 1;
            // 가장 작은 용량 찾기
            if(gu8_Cup_size_Array[i].amount_volume < min_volume_value)
            {
                min_volume_value = gu8_Cup_size_Array[i].amount_volume;
                min_volume_index = i;
            }
        }
    }
    
    // 활성화된 용량이 있으면 가장 작은 용량으로 설정
    if(found_active == 1)
    {
        gu8_DefaultCupSize = min_volume_index;
    }
    else
    {
        // 모든 용량이 비활성화된 경우 기본값으로
        gu8_DefaultCupSize = CUPSIZE_ONE;
    }
}

/// @brief    현재 선택된 용량이 비활성화되었을 때 활성화된 다른 용량으로 변경
/// @param    void
/// @return   void
void Update_CurrentCupSize_If_Disabled(void)
{
    U8 i = 0;
    U8 mu8_start_index = 0;
    U8 mu8_i = 0;
    U8 search_ok = 0;
    
    // 현재 선택된 용량이 활성화되어 있으면 변경 불필요
    if(gu8_CupIndex <= CUPSIZE_FIVE)
    {
        if(gu8_Cup_size_Array[gu8_CupIndex].enable == 1)
        {
            return;  // 변경 불필요
        }
    }
    
    // 현재 선택된 용량이 비활성화되었거나 범위를 벗어났으면 활성화된 용량 찾기
    // 먼저 증가 (현재 값 건너뛰기)
    if (++gu8_CupIndex > CUPSIZE_FIVE)
    {
        gu8_CupIndex = CUPSIZE_ONE;
    }
    mu8_start_index = gu8_CupIndex;  // 시작 인덱스 저장

    // 5번 순차 탐색
    for(mu8_i = 0; mu8_i < CUPSIZE_FIVE; mu8_i++)
    {
        // 현재 값이 enable인지 체크
        if(gu8_Cup_size_Array[gu8_CupIndex].enable == 1)
        {
            search_ok = 1;
            break;
        }
        
        // 다음 값으로 이동
        if (++gu8_CupIndex > CUPSIZE_FIVE)
        {
            gu8_CupIndex = CUPSIZE_ONE;
        }
        
        // 시작점으로 돌아왔으면 종료 (5번 체크 완료)
        if(gu8_CupIndex == mu8_start_index)
        {
            break;
        }
    }

    if(search_ok == 0)
    {
        gu8_CupIndex = CUPSIZE_ONE;
    }

    // gu16_CupSize 업데이트
    if(gu8_CupIndex != CUPSIZE_FIVE)		//	연속 아니면..
    {
        gu16_CupSize = (U16)gu8_Cup_size_Array[gu8_CupIndex].amount_volume * 10;		//	x 10
    }
    else
    {
        gu16_CupSize = CUPSIZE_CONTINUE;			//	연속은 0xAA로...
    }
}
