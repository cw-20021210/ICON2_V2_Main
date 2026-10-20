/// @file     error.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    Error 체크 동작 관련 File
#define ERROR_C
#include "include/_include.h"

static void ErrorTimerSet(void);
static void ErrorHistory(U8 mu8_ErrorData);

static U8 Error01_LeakageWater_ADC(void);
static U8 Error02_LeakageWater_FlowMeter(void);
static U8 Error08_HotCutWater(void);
static U8 Error09_CutWater(void);
static U8 Error25_HeatSinkTH(void);
static U8 Error29_TEM_OverDrive(void);
static U8 Error40_OverHeat(void);
static U8 Error43_OutDoorAirTH(void);
static U8 Error44_ColdTH(void);
static U8 Error47_HotOutTH(void);
static U8 Error46_HotInTH(void);
static U8 Error48_HeaterTH(void);
static U8 ErrorA0_FrontCommunication(void);

/// @brief    Error Flag And Counter Initialize
/// @param    vo25id
/// @return   void
void Error_Initialize(void)
{
	U8 i;

	gu8_ErrorCode = 0;
	ErrorTimerSet();

	for (i = 0 ; i < Z_ErrorHistory ; i++)			//
	{
		gu8_ErrorHistory[i] = CLEAR;				//		에러 데이터 Clear
	}
}

/// @brief    Error Check 함수(100ms)
/// @param    void
/// @return   void
void ErrorCheck(void)
{
	U8 mu8_i;
	U8 mu8_ret;

	static U8 (* const AT_Error[])(void) =
	{
		Error01_LeakageWater_ADC,     				// 	급수 누수 에러 체크 (ADC)
		Error02_LeakageWater_FlowMeter,				// 	급수 누수 에러 체크 (유량센서)
		Error08_HotCutWater,						//	온수 추출 단수 에러 체크
		Error09_CutWater,							//	추출 단수 에러 체크
		Error25_HeatSinkTH,							// 	TEM 온도 센서 에러 체크
		Error29_TEM_OverDrive,						// 	TEM 온도 센서 이상 에러 체크
		Error40_OverHeat,							//	과열 에러
		Error43_OutDoorAirTH,						// 	외기 온도 센서 에러 체크
		Error44_ColdTH,								// 	냉수 온도 센서 에러 체크
		Error46_HotInTH,							// 	온수입수 온도 센서 에러 체크
		Error47_HotOutTH,							// 	온수출수 온도 센서 에러 체크
		Error48_HeaterTH,							// 	히터 온도 센서 에러 체크
		ErrorA0_FrontCommunication,					//	프론트 통신에러
	};

	if (gu8_ErrorCode != 0)							//	에러 발생 상태면
	{
		ErrorTimerSet();							//		타이머 초기화
	}

#ifndef _NO_ERROR_
	/************************************************/
	for (mu8_i = 0; mu8_i < (sizeof(AT_Error) / 4); mu8_i++)	//	error검출처리 종료 판단
	{

		mu8_ret = (*AT_Error[mu8_i])();				//		각 error check
		if ((mu8_ret != 0) && (gu8_ErrorCode == 0x00)) 	//		error 검출한 경우
		{
			gu8_ErrorCode = mu8_ret;				//			error set
			if (F_TestMode.byte == 0x00)
			{
				WifiSendData(WIFI_DATA_ERROR);
				ErrorHistory(gu8_ErrorCode);
			}
		}
	}
#endif
}

/// @brief	    에러이력 갱신
/// @param    void
/// @return   void
static void ErrorHistory(U8 mu8_ErrorData)
{
	U8 i;

	for (i = (Z_ErrorHistory-1) ; i > 0 ; i--)			//	에러1~4
	{
		gu8_ErrorHistory[i] = gu8_ErrorHistory[i-1];	//		에러 데이터 Shift
	}
	gu8_ErrorHistory[0] = mu8_ErrorData;				//	최신발생 에러 에러1에 저장

	for (i = 0 ; i < Z_ErrorHistory ; i++)				//	EEPROM 갱신
	{
//		EEPROM_ConfirmWrite(EEPROM_ADDR_ERROR_HISTORY_1+i, gu8_ErrorHistory[i]);
	}
}

/// @brief	  Error Timer Set
/// @param    void
/// @return   void
static void ErrorTimerSet(void)
{
	gu16_TimerErr02 = Z_TimerErr02;
	gu16_TimerErr08 = Z_TimerErr08;
	gu16_TimerErr09 = Z_TimerErr09;
	gu16_TimerErr29_1 = Z_TimerErr29_1;
	gu16_TimerErr29_2 = Z_TimerErr29_2;
	gu16_TimerErr40 = Z_TimerErr40;

	if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))	//	테스트모드인 경우 (에러타이머 짧게)
	{
		gu16_TimerErr01 = Z_TimerErr01_TestMode;
		gu16_TimerErr25 = Z_TimerErr25_TestMode;
		gu16_TimerErr43 = Z_TimerErr43_TestMode;	//
		gu16_TimerErr44 = Z_TimerErr44_TestMode;
		gu16_TimerErr46 = Z_TimerErr46_TestMode;
		gu16_TimerErr47 = Z_TimerErr47_TestMode;
		gu16_TimerErr48 = Z_TimerErr48_TestMode;
		gu16_TimerErr25_Display	= Z_TimerErr25_Display_TestMode;
		gu16_TimerErr44_Display	= Z_TimerErr44_Display_TestMode;
	}
	else
	{
		gu16_TimerErr01 = Z_TimerErr01;
		gu16_TimerErr25 = Z_TimerErr25;				//		타이머 set
		gu16_TimerErr43 = Z_TimerErr43;				//
		gu16_TimerErr44 = Z_TimerErr44;
		gu16_TimerErr46 = Z_TimerErr46;
		gu16_TimerErr47 = Z_TimerErr47;
		gu16_TimerErr48 = Z_TimerErr48;
		gu16_TimerErr25_Display	= Z_TimerErr25_Display;
		gu16_TimerErr44_Display	= Z_TimerErr44_Display;
	}
}

/// @brief    누수 Error Check (ADC)
/// @param    void
/// @return   void
static U8 Error01_LeakageWater_ADC(void)
{
	if (ADC_RESULT_LEAKSENSOR <= Z_LeakSensor_Error)	//	누수감지 ADC데이터가 정상데이터 인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr01 = Z_TimerErr01_TestMode;
		}
		else
		{
			gu16_TimerErr01 = Z_TimerErr01;				//			타이머 set
		}
	}
	else
	{
		gu16_TimerErr01_Clear = Z_TimerErr01_Clear;
	}

	if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
	{
		if ((gu8_ErrorCode == Z_Ecode01) && (ADC_RESULT_LEAKSENSOR <= Z_LeakSensor_Error))	//
		{
			gu8_ErrorCode = CLEAR;					//
		}
	}

	if (gu16_TimerErr01 == 0)						//	타이머 경과 시
	{
		F_Err01_LeakWater = SET;
		return (Z_Ecode01);							//		에러 확정
	}

	if (gu16_TimerErr01_Clear == 0)
	{
		F_Err01_LeakWater = CLEAR;
		if (gu8_ErrorCode == Z_Ecode01)    			//	자동해제 타이머 경과 and 해당에러인 경우
		{
			gu8_ErrorCode = CLEAR;					//		에러 해제
		}
	}

	return (0);
}

/// @brief    누수 Error Check (유량센서)
/// @param    void
/// @return   void
static U8 Error02_LeakageWater_FlowMeter(void)
{
//@026	if ((F_TestMode_PBA == CLEAR) && ((P_NOS_V == N_OFF) || ((P_FEED_COLD_IN_V == OFF) && (P_FEED_HOTIN_V == OFF) && (P_FEED_PURE_V == OFF))))     //	검사모드가 아니고 추출중이 아닌 경우
//@026	{
//@026		if (gu16_FlowDecide_Hz >= 40)				//		유량센서 ? Hz(0.09LPM) 이상인 경우
//@026		{
//@026			gu16_TimerErr02_Clear = Z_TimerErr02_Clear;	//		자동해제 타이머 set
//@026			if (gu16_TimerErr02 == 0)				//			10초 연속
//@026			{
//@026				F_Err02_LeakWater = SET;
//@026				return (Z_Ecode02);					//				에러 확정
//@026			}
//@026		}
//@026		else										//		5Hz 미만인 경우
//@026		{
//@026			gu16_TimerErr02 = Z_TimerErr02;			//			타이머 set
//@026		}
//@026	}
//@026	else											//	밸브가 하나라도 열려있는 경우 (추출 중인 경우)
//@026	{
//@026		gu16_TimerErr02 = Z_TimerErr02;				//		타이머 set
//@026	}
//@026
//@026	if (gu16_TimerErr02_Clear == 0)
//@026	{
//@026		F_Err02_LeakWater = CLEAR;
//@026		if (gu8_ErrorCode == Z_Ecode02)    			//	자동해제 타이머 경과 and 해당에러인 경우
//@026		{
//@026			gu8_ErrorCode = CLEAR;					//		에러 해제
//@026		}
//@026	}

	F_Err02_LeakWater = CLEAR;						//@026	E02 에러 삭제로 자동 해제
	if (gu8_ErrorCode == Z_Ecode02)    				//@026	E02 에러 삭제로 자동 해제
	{												//@026
		gu8_ErrorCode = CLEAR;						//@026
	}												//@026

	return (0);
}


/// @brief    온수 단수 Error Check
/// @param    void
/// @return   void
static U8 Error08_HotCutWater(void)
{
	if ((P_NOS_V == N_ON) && (P_FEED_HOTIN_V == ON))	//	온수 입수 밸브가 열려있는 경우 (추출중인 경우)
	{
		if (gu16_FlowDecide_W < 10)					//		유량이 0.1LPM 미만인 경우
		{
			if ((gu8_WaterSelect == HOT_WATER) && (gu8_TimerFlowCheck_AfterEff != 0))	//				온수인 경우
			{
				gu16_TimerErr08 = Z_TimerErr08;		//				온수 데이터로 타이머 set
			}
//@023			if ((F_TestMode_PBA == CLEAR) && (gu8_ModelType_forCHP == MODEL_CHP) && (gu16_TimerErr08 == 0))			//			타이머 경과 시
			if ((F_TestMode_PBA == CLEAR) && (F_TestMode_LineFront == CLEAR) && (F_TestMode_LineTest2 == CLEAR) && (gu8_ModelType_forCHP == MODEL_CHP) && (gu16_TimerErr08 == 0))			//			타이머 경과 시
			{
				gu8_TimerErr08_Clear = Z_TimerErr08_Clear;	//			자동에러 해제 타이머 set
				F_FirstHotEffluent = CLEAR;
				F_Err08_HotCutWater = SET;
				return (Z_Ecode08);					//				단수에러 확정
			}
		}
		else										//		0.1LPM 이상인 경우
		{
			gu16_TimerErr08 = Z_TimerErr08;     	//			타이머 set
		}
	}
	else											//	밸브 전체 OFF
	{
		gu16_TimerErr08 = Z_TimerErr08;     		//			타이머 set
	}

	if (gu8_TimerErr08_Clear == 0)					//	단수에러 1분 뒤 자동 해제
	{
		F_Err08_HotCutWater = CLEAR;
		if (gu8_ErrorCode == Z_Ecode08)
		{
			gu8_ErrorCode = CLEAR;						//		에러해제
		}
	}

	return (0);
}

/// @brief    단수 Error Check
/// @param    void
/// @return   void
static U8 Error09_CutWater(void)
{
	if ((P_NOS_V == N_ON) && ((P_FEED_COLD_IN_V == ON) || (P_FEED_PURE_V == ON)))	//	노스밸브 열린 상태에서 정냉수 밸브가 하나라도 열려있는 경우 (추출중인 경우)
	{
		if (gu16_FlowDecide_W < 10)					//		유량이 0.1LPM 미만인 경우
		{
//@023			if ((F_TestMode_PBA == CLEAR) && (gu16_TimerErr09 == 0))			//			타이머 경과 시
			if ((F_TestMode_PBA == CLEAR) && (F_TestMode_LineFront == CLEAR) && (F_TestMode_LineTest2 == CLEAR) && (gu16_TimerErr09 == 0))	//@023			타이머 경과 시

			{
				gu8_TimerErr09_Clear = Z_TimerErr09_Clear;	//			자동에러 해제 타이머 set
				F_Err09_CutWater = SET;
				return (Z_Ecode09);					//				단수에러 확정
			}
		}
		else										//		0.1LPM 이상인 경우
		{
			if ((F_FirstFlushing == CLEAR) && (gu8_OperationStep_hi == 0x00))	//			플러싱 상태
			{
				gu16_TimerErr09 = Z_TimerErr09_forFlushing;	//		(※ 에어가 차있는 경우 유량센서 인식까지 오래걸려서 시간을 길게함)
			}
			else
			{
				gu16_TimerErr09 = Z_TimerErr09;     //			일반 데이터로 타이머 set
			}
		}
	}
	else											//	밸브 전체 OFF
	{
		if ((F_FirstFlushing == CLEAR) && (gu8_OperationStep_hi == 0x00))	//			플러싱 상태
		{
			gu16_TimerErr09 = Z_TimerErr09_forFlushing;	//		(※ 에어가 차있는 경우 유량센서 인식까지 오래걸려서 시간을 길게함)
		}
		else
		{
			gu16_TimerErr09 = Z_TimerErr09;     	//			일반 데이터로 타이머 set
		}
	}

	if (gu8_TimerErr09_Clear == 0)					//	단수에러 1분 뒤 자동 해제
	{
		F_Err09_CutWater = CLEAR;
		if (gu8_ErrorCode == Z_Ecode09)
		{
			gu8_ErrorCode = CLEAR;						//		에러해제
		}
	}

	return (0);
}

/// @brief    Tem Thermistor Error Check
//  @param    void
/// @return   void
static U8 Error25_HeatSinkTH(void)
{
	/************************************************/
	U8 mu8_OPENSHORT_Error;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HeatSinkSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크

	if (mu8_OPENSHORT_Error == 0)					//		정상인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr25 = Z_TimerErr25_TestMode;//				타이머 set
		}
		else
		{
			gu16_TimerErr25 = Z_TimerErr25;			//				타이머 set
		}
	}
	else
	{
		if (gu16_TimerErr25 == 0)					//	에러 확정인 경우
		{
			F_Err25_HeatSinkTH = SET;
//			if ((gu8_WaterSelect == COLD_WATER) && (F_ColdOff == CLEAR))	//	냉수 선택 및 냉각 ON인 경우
			if (((gu8_WaterSelect == COLD_WATER) && (F_ColdOff == CLEAR)) || (F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))	//	냉수 선택 and (냉각 ON인 경우 or 검사모드인 경우)
			{
				if (gu16_TimerErr25_Display == 0)
				{
					return (Z_Ecode25);				//		에러코드 반환
				}
			}
			else
			{
				if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))			//		테스트모드인 경우 표시 빠르게
				{
					gu16_TimerErr25_Display = Z_TimerErr25_Display_TestMode;//				타이머 set
				}
				else
				{
					gu16_TimerErr25_Display = Z_TimerErr25_Display;	//		3초 뒤 표시
				}
			}
		}
	}

	/*	에러 자동해제 타이머 */
	if (mu8_OPENSHORT_Error != 0)					//	OPEN SHORT상태인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr25_Clear = Z_TimerErr25_TestMode;//	자동해제	타이머 set
		}
		else										//
		{
			gu16_TimerErr25_Clear = Z_TimerErr25;		//		자동해제 타이머 set
		}
//		gu16_TimerErr25_Clear = Z_TimerErr25;		//		자동해제 타이머 set
	}
	if (gu16_TimerErr25_Clear == 0)
	{
		F_Err25_HeatSinkTH = CLEAR;
		if (gu8_ErrorCode == Z_Ecode25)    			//	자동해제 타이머 경과 and 해당에러인 경우
		{
			gu8_ErrorCode = CLEAR;					//		에러 해제
		}
	}

	return (0);
}

/// @brief    TEM 연속구동 에러
//  @param    void
/// @return   void
static U8 Error29_TEM_OverDrive(void)
{
	U8 mu8_OPENSHORT_Error, mu8_OPENSHORT_Error2 = 0;
	if (F_ColdOff == CLEAR)     					//  냉각기능 ON
	{
		//  #1 TEM 연속 1시간 동작 시 냉수 온도로 에러 판단 (35도 이상 1시간 유지)
		if (gu8_TEM_PWM_CMD != TEM_PWM_OFF)			//
		{
			mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_ColdSensor.TH_ADC_AVERAGE);
			if (mu8_OPENSHORT_Error != 0)			//			단선 단락인 경우 에러 확정하지 않음
			{
				gu16_TimerErr29_1 = Z_TimerErr29_1;	//				타이머 set
			}
			else if (gi16_ColdSensor.DEGREE < Z_TEMP35)	//			냉수센서가 35도 미만인 경우
			{
				gu16_TimerErr29_1 = Z_TimerErr29_1;	//				타이머 set
			}
			else
			{

			}

			if (gu16_TimerErr29_1 == 0)				//		타이머 경과 시
			{
				F_Err29_TEM_OverDrive1 = SET;		//			에러 플래그 확정 (냉각을 OFF하기 위함)
			}
		}
		else										//	상기 외
		{
			gu16_TimerErr29_1 = Z_TimerErr29_1;		//		타이머 set
		}

		//  #2 HEATSINK 온도 과열
		mu8_OPENSHORT_Error2 = TH_Open_Short_Check(gi16_HeatSinkSensor.TH_ADC_AVERAGE);	//	단선 단락 체크
		if (mu8_OPENSHORT_Error2 != 0)				//	단선 단락인 경우 에러 확정하지 않음
		{
			gu16_TimerErr29_2 = Z_TimerErr29_2;		//		타이머 set
		}
		else if (gi16_HeatSinkSensor.DEGREE < Z_Cold_HeatSinkTH_CutDegree)	//	HEATSINK 센서가 cut 온도 미만인 경우
		{
			gu16_TimerErr29_2 = Z_TimerErr29_2;		//		타이머 set
		}
		else
		{

		}

		if (gu16_TimerErr29_2 == 0)					//		타이머 경과 시
		{
			F_Err29_TEM_OverDrive2 = SET;			//			에러 플래그 확정 (냉각을 OFF하기 위함)
		}

		if ((gu8_WaterSelect == COLD_WATER) && ((F_Err29_TEM_OverDrive1 == SET) || (F_Err29_TEM_OverDrive2 == SET)))	//		냉수 선택 시에 에러 코드 표시
		{
			if (gu8_TimerErr29_Display == 0)		//	선택하자마자 에러가 발생해서 선택음과 에러음이 중첩되는 문제가 있어 0.5초 후에 에러 확정
			{
				return (Z_Ecode29);					//			에러 확정
			}
		}
		else
		{
			gu8_TimerErr29_Display = Z_Timer_Err29_Decide;
		}
	}
	else											//	냉각 OFF인 경우
	{
		gu8_TimerErr29_Display = Z_Timer_Err29_Decide;
		gu16_TimerErr29_1 = Z_TimerErr29_1;			//		타이머 set
		gu16_TimerErr29_2 = Z_TimerErr29_2;			//		타이머 set
	}

	return (0);
}

/// @brief    온수 배수막힘 에러 (※ 이 에러는 히터소손방지(히터 유로가 빠져 유량은 감지되지만 히터에 물없이 가열되는 것)도 될 수 있음)
/// @param    void
/// @return   void
//static U8 Error34_HotDrain_Cut(void)
//{
//	if (F_Err34 == SET)
//	{
//		F_Err34 = CLEAR;
//		return (Z_Ecode34);
//	}
//
//	return (0);
//}

/// @brief    과열 Error Check
/// @param    void
/// @return   void
static U8 Error40_OverHeat(void)
{
	U8 mu8_OPENSHORT_Error1, mu8_OPENSHORT_Error2 = 0;
	/************************************************/

	switch (gu8_WaterSelect)							//	물 선택
	{												//
		case HOT_WATER:								//		온수 인 경우
			mu8_OPENSHORT_Error1 = TH_Open_Short_Check(gi16_HotOutSensor.TH_ADC_AVERAGE);	//		온수출수센서 단선단락 체크
			mu8_OPENSHORT_Error2 = TH_Open_Short_Check(gi16_HeaterSensor.TH_ADC_AVERAGE);	//		히터센서 단선단락 체크
			if ((mu8_OPENSHORT_Error1 != 0) || (mu8_OPENSHORT_Error2 != 0))     //		단선 단락일 경우 에러 확정하지 않음
			{
				gu16_TimerErr40 = Z_TimerErr40;		//			타이머 set
			}
//			else if (((gi16_HotOutSensor.DEGREE < Z_TEMP105) && (gi16_HeaterSensor.DEGREE < Z_TEMP105)) || (gu8_OperationStep != Z_OperationStep_50))
			/*else if (((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR100_JP90) && (gi16_HotOutSensor.DEGREE < Z_TEMP105) && (gi16_HeaterSensor.DEGREE < Z_TEMP105))
					|| ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) && (gi16_HotOutSensor.DEGREE < Z_TEMP107) && (gi16_HeaterSensor.DEGREE < Z_TEMP107)) 	//@001
					|| (gu8_OperationStep != Z_OperationStep_50))
					*/
			else if (((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR100_JP90) && (gi16_HotOutSensor.DEGREE < Z_TEMP105) && (gi16_HeaterSensor.DEGREE < Z_TEMP105))
					|| ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) && (gi16_HotOutSensor.DEGREE < Z_TEMP107) && (gi16_HeaterSensor.DEGREE < Z_TEMP107))
					|| ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) && (gi16_HotOutSensor.DEGREE < Z_TEMP107) && (gi16_HeaterSensor.DEGREE < Z_TEMP107))
					|| ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90) && (gi16_HotOutSensor.DEGREE < Z_TEMP107) && (gi16_HeaterSensor.DEGREE < Z_TEMP107))
					|| (gu8_OperationStep != Z_OperationStep_50))
			{
				gu16_TimerErr40 = Z_TimerErr40;		//			타이머 set
			}
			else
			{

			}
			break;
		default:									//	상기이외
			gu16_TimerErr40 = Z_TimerErr40;			//		타이머set
			break;
	}

//#ifndef _FOR_HEATER_ENDURANCE_TEST_
//#ifndef _FOR_REPEAT_EFF_ENDURANCE_TEST_
/*
	if ((gu8_ModelType_forCHP == MODEL_CHP) && (gu16_TimerErr40 == 0))	//	타이머 경과 시
	{
		F_OverHeatError = SET;						//		과열에러 확정
		F_FirstHotEffluent = CLEAR;
			F_Err40_OverHeat = SET;
					WifiSendData(WIFI_DATA_ERROR);										//	WIFI 에러 해제 Data 전송
		return (Z_Ecode40);							//		과열에러 확정
	}		*/										//
//#endif
//#endif
	if ((F_OverHeatError == SET) && (gi16_HotOutSensor.DEGREE <= Z_TEMP80) && (gi16_HeaterSensor.DEGREE <= Z_TEMP80))
	{
		F_OverHeatError = CLEAR;
	}

	if ((gu8_ErrorCode == Z_Ecode40) && (F_OverHeatError == CLEAR))  //	(현재 검출된 에러 코드) & (차회 추출 가능 판단 Flag Clear) 인 경우
	{
		F_Err40_OverHeat = CLEAR;											//	에러 플러그 Clear
		gu8_ErrorCode = CLEAR;												//	에러 코드 Clear
		WifiSendData(WIFI_DATA_ERROR);										//	WIFI 에러 해제 Data 전송
	}

	return (0);
}

/// @brief    Outdoor Thermistor Error Check
/// @param    void
/// @return   void
static U8 Error43_OutDoorAirTH(void)
{
	/************************************************/
	U8 mu8_OPENSHORT_Error;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크

	if (mu8_OPENSHORT_Error == 0)					//	정상인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr43 = Z_TimerErr43_TestMode;//			타이머 set
		}
		else
		{
			gu16_TimerErr43 = Z_TimerErr43;			//			타이머 set
		}
	}
	else
	{
		if (gu16_TimerErr43 == 0)					//	에러 확정인 경우
		{
			F_Err43_OutDoorAirTH = SET;
//			if (gu8_TimerSleepMode <= (Z_TimerSleepMode/2)) //
			if (gu8_TimerEcode43_Display != 0)
			{
				return (Z_Ecode43);					//		에러코드 반환
			}
		}
	}

	/*	에러 자동해제 타이머 */
	if (mu8_OPENSHORT_Error != 0)					//	OPEN SHORT상태인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr43_Clear = Z_TimerErr43_TestMode;//	자동해제	타이머 set
		}
		else											//
		{
			gu16_TimerErr43_Clear = Z_TimerErr43;		//		자동해제 타이머 set
		}
	}
	if (gu16_TimerErr43_Clear == 0)
	{
		F_Err43_OutDoorAirTH = CLEAR;
		if (gu8_ErrorCode == Z_Ecode43)    			//	자동해제 타이머 경과 and 해당에러인 경우
		{
			gu8_ErrorCode = CLEAR;					//		에러 해제
		}
	}

	if ((gu8_ErrorCode == Z_Ecode43) && (gu8_TimerEcode43_Display == 0))	//	3초 표시 후 자동 해제
	{
		gu8_ErrorCode = CLEAR;					//		에러 해제
	}

	return (0);
}

/// @brief    Cold Water Thermistor Error Check
/// @param    void
/// @return   void
static U8 Error44_ColdTH(void)
{
	/************************************************/
	U8 mu8_OPENSHORT_Error;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_ColdSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크

	if (mu8_OPENSHORT_Error == 0)					//	정상인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr44 = Z_TimerErr44_TestMode;//			타이머 set
		}
		else
		{
			gu16_TimerErr44 = Z_TimerErr44;			//			타이머 set
		}
	}
	else
	{
		if (gu16_TimerErr44 == 0)					//	에러 확정인 경우
		{
			F_Err44_ColdTH = SET;
//			if ((gu8_WaterSelect == COLD_WATER) && (F_ColdOff == CLEAR))	//	냉수 선택 및 냉각 ON인 경우
			if (((gu8_WaterSelect == COLD_WATER) && (F_ColdOff == CLEAR)) || (F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))	//	냉수 선택 and (냉각 ON인 경우 or 검사모드인 경우)
			{
				if (gu16_TimerErr44_Display == 0)
				{
					return (Z_Ecode44);				//		에러코드 반환
				}
			}
			else
			{
				if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
				{
					gu16_TimerErr44_Display = Z_TimerErr44_Display_TestMode;//				타이머 set
				}
				else
				{
					gu16_TimerErr44_Display = Z_TimerErr44_Display;	//		3초 뒤 표시
				}
			}
		}
	}

	/*	에러 자동해제 타이머 */
	if (mu8_OPENSHORT_Error != 0)					//	OPEN SHORT상태인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr44_Clear = Z_TimerErr44_TestMode;//	자동해제	타이머 set
		}
		else										//
		{
			gu16_TimerErr44_Clear = Z_TimerErr44;		//		자동해제 타이머 set
		}
//		gu16_TimerErr44_Clear = Z_TimerErr44;		//		자동해제 타이머 set
	}
	if (gu16_TimerErr44_Clear == 0)
	{
		F_Err44_ColdTH = CLEAR;
		if (gu8_ErrorCode == Z_Ecode44)    			//	자동해제 타이머 경과 and 해당에러인 경우
		{
			gu8_ErrorCode = CLEAR;					//		에러 해제
		}
	}

	return (0);
}

/// @brief    Hot Water Thermistor Error Check
/// @param    void
/// @return   void
static U8 Error47_HotOutTH(void)
{
	/************************************************/
	U8 mu8_OPENSHORT_Error;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HotOutSensor.TH_ADC_AVERAGE);    //	OPEN SHORT 체크

	switch (gu8_WaterSelect)							//	물 선택
	{
		case HOT_WATER:								//		온수 선택인 경우
			if (mu8_OPENSHORT_Error == 0)			//			정상인 경우
			{
				gu16_TimerErr47 = Z_TimerErr47;	//				타이머 set
			}										//
			break;
		default:									//	상기이외
			if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
			{
				if (mu8_OPENSHORT_Error == 0)		//			정상인 경우
				{
					gu16_TimerErr47 = Z_TimerErr47_TestMode;//				타이머 set
				}
			}
			else
			{
				gu16_TimerErr47 = Z_TimerErr47;		//				타이머 set
			}
			break;
	}

	/*	에러 자동해제 타이머 */
	if (mu8_OPENSHORT_Error != 0)                   //	OPEN SHORT상태인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr47_Clear = Z_TimerErr47_TestMode;//	자동해제	타이머 set
		}
		else										//
		{
			gu16_TimerErr47_Clear = Z_TimerErr47;		//		자동해제 타이머 set
		}
//		gu16_TimerErr47_Clear = Z_TimerErr47_Clear;	//		자동해제 타이머 set
	}
	if (gu16_TimerErr47_Clear == 0) 				//	자동해제 타이머 경과 and 해당에러인 경우
	{
		F_Err47_HotOutTH = CLEAR;
		if (gu8_ErrorCode == Z_Ecode47)
		{
			gu8_ErrorCode = CLEAR;                      //		에러 해제
		}
	}

	if ((gu8_ModelType_forCHP == MODEL_CHP) && (gu16_TimerErr47 == 0))   //	에러 확정인 경우
	{
		F_Err47_HotOutTH = SET;
		return (Z_Ecode47);							//		에러코드 반환
	}												//
	return (0);
}

/// @brief    Hot Water Thermistor Error Check
/// @param    void
/// @return   void
static U8 Error46_HotInTH(void)
{
	/************************************************/
	U8 mu8_OPENSHORT_Error;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HotInSensor.TH_ADC_AVERAGE);    //	OPEN SHORT 체크

	switch (gu8_WaterSelect)							//	물 선택에 따라
	{												//
		case HOT_WATER:								//		온수 선택인 경우
			if (mu8_OPENSHORT_Error == 0)			//			정상인 경우
			{
				gu16_TimerErr46 = Z_TimerErr46;		//				타이머 set
			}										//
			break;
		default:									//	상기이외
			if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
			{
				if (mu8_OPENSHORT_Error == 0)		//			정상인 경우
				{
					gu16_TimerErr46 = Z_TimerErr46_TestMode;	//				타이머 set
				}
			}
			else
			{
				gu16_TimerErr46 = Z_TimerErr46;		//				타이머 set
			}
			break;
	}

	/*	에러 자동해제 타이머 */
	if (mu8_OPENSHORT_Error != 0)                   //	OPEN SHORT상태인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr46_Clear = Z_TimerErr46_TestMode;//	자동해제	타이머 set
		}
		else										//
		{
			gu16_TimerErr46_Clear = Z_TimerErr46;		//		자동해제 타이머 set
		}
//		gu16_TimerErr46_Clear = Z_TimerErr46;       //		자동해제 타이머 set
	}
	if (gu16_TimerErr46_Clear == 0)
	{
		F_Err46_HotInTH = CLEAR;
		if (gu8_ErrorCode == Z_Ecode46)
		{
			gu8_ErrorCode = CLEAR;                      //		에러 해제
		}
	}

	if ((gu8_ModelType_forCHP == MODEL_CHP) && (gu16_TimerErr46 == 0))  //	에러 확정인 경우
	{
		F_Err46_HotInTH = SET;
		return (Z_Ecode46);							//		에러코드 반환
	}												//
	return (0);
}

/// @brief    Heater Thermistor Error Check
/// @param    void
/// @return   void
static U8 Error48_HeaterTH(void)
{
	/************************************************/
	U8 mu8_OPENSHORT_Error = 0;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HeaterSensor.TH_ADC_AVERAGE);    //	OPEN SHORT 체크

	switch (gu8_WaterSelect)							//	물 선택에 따라
	{
		case HOT_WATER:								//		온수 선택인 경우
			if (mu8_OPENSHORT_Error == 0)			//			정상인 경우
			{
				gu16_TimerErr48 = Z_TimerErr48;	//				타이머 set
			}										//
			break;
		default:									//	상기이외
			if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
			{
				if (mu8_OPENSHORT_Error == 0)		//			정상인 경우
				{
					gu16_TimerErr48 = Z_TimerErr48_TestMode;	//				타이머 set
				}
			}
			else
			{
				gu16_TimerErr48 = Z_TimerErr48;		//				타이머 set
			}
			break;
	}

	/*	에러 자동해제 타이머 */
	if (mu8_OPENSHORT_Error != 0)                   //	OPEN SHORT상태인 경우
	{
		if ((F_TestMode_PBA == SET) || (F_TestMode_LineTest2 == SET))
		{
			gu16_TimerErr48_Clear = Z_TimerErr48_TestMode;//	자동해제	타이머 set
		}
		else										//
		{
			gu16_TimerErr48_Clear = Z_TimerErr48;		//		자동해제 타이머 set
		}
//		gu16_TimerErr48_Clear = Z_TimerErr48_Clear;	//		자동해제 타이머 set
	}
	if (gu16_TimerErr48_Clear == 0)
	{
		F_Err48_HeaterTH = CLEAR;
		if (gu8_ErrorCode == Z_Ecode48)
		{
			gu8_ErrorCode = CLEAR;                      //		에러 해제
		}
	}

	if ((gu8_ModelType_forCHP == MODEL_CHP) && (gu16_TimerErr48 == 0))     //	에러 확정인 경우
	{
		F_Err48_HeaterTH = SET;
		return (Z_Ecode48);							//		에러코드 반환
	}												//
	return (0);
}

/// @brief    프론트 통신 에러
/// @param    void
/// @return   void
static U8 ErrorA0_FrontCommunication(void)
{
	if (F_Front_Communication_Error == SET)			//	통신에러 확정인 경우
	{
		F_ErrA0_FrontCommunication = SET;			//		에러 플래그 set
		return (Z_EcodeA0);							//		에러코드 반환
	}

	if (F_Front_Communication_Error == CLEAR)		//	통신에러 clear
	{
		F_ErrA0_FrontCommunication = CLEAR;			//		에러 플래그 clear
		if (gu8_ErrorCode == Z_EcodeA0)				//		해다 에러코드였던 경우
		{
			gu8_ErrorCode = CLEAR;					//			코드 clear
		}
	}

	return (0);
}

/// @brief    OPEN SHORT 체크 함수
/// @param    void
/// @return   void
U8 TH_Open_Short_Check(U16* mu16_ADCAddress)
{
	U8 mu8_ret = 0;
	U8 mu8_i = 0;

	for (mu8_i = 0; mu8_i < DEGREE_SENSOR_NUM; mu8_i++)
	{
		if (TH_DataArray[mu8_i]->TH_ADC_AVERAGE == mu16_ADCAddress)     //		테이블에서 해당 어드레스 찾기
		{
			break;
		}
	}

	if (mu8_i < DEGREE_SENSOR_NUM)									//	데이터가 있는 경우
	{
		if (*mu16_ADCAddress < TH_DataArray[mu8_i]->SHORT_DATA)	//	해당 데이터의 SHORT데이터 미만인 경우
		{
			mu8_ret = 1;							//		SHORT 확정
		}

		if (*mu16_ADCAddress > TH_DataArray[mu8_i]->OPEN_DATA)	//	해당 데이터의 OPEN데이터 초과인 경우
		{
			mu8_ret = 2;							//		OPEN 확정
		}
	}

	return (mu8_ret);								//	결과 반환
}

/// @brief    에러 해제 시 데이터 클리어
/// @param    void
/// @return   void
void ErrorClear(void)
{
	gu8_ErrorCode = CLEAR;
	gu8_CounterRXDError = 0;
	ErrorTimerSet();
	F_Front_Communication_Error = CLEAR;
	F_Error_Flag.Word = 0x0000U;
}
