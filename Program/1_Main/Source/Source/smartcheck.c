/// @file     buzzer.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    부저  동작 제어 file
#define SMARTCHECK_C
#include "include/_include.h"

static void Ini_SmartCheckStep_00(void);
static void Ini_SmartCheckStep_10(void);			//	대기
static void Ini_SmartCheckStep_20(void);			//	ALL OFF
static void Ini_SmartCheckStep_21(void);			//	히터 Triac
static void Ini_SmartCheckStep_22(void);			//	히터 Relay
static void Ini_SmartCheckStep_23(void);			//	1stNosValve
static void Ini_SmartCheckStep_24(void);			//	HotInValve
static void Ini_SmartCheckStep_25(void);			//	HotOutValve
static void Ini_SmartCheckStep_26(void);			//	ColdInValve
static void Ini_SmartCheckStep_27(void);			//	ColdOutValve
static void Ini_SmartCheckStep_28(void);			//	PureValve
static void Ini_SmartCheckStep_29(void);			//	UV
static void Ini_SmartCheckStep_2A(void);			//	Fan
static void Ini_SmartCheckStep_2B(void);			//	TEM
static void Ini_SmartCheckStep_2C(void);			//	유량센서
static void Ini_SmartCheckStep_2D(void);			//	SteppingMotor

static void SmartCheckStep_00(void);
static void SmartCheckStep_10(void);				//	대기
static void SmartCheckStep_20(void);				//	ALL OFF
static void SmartCheckStep_21(void);				//	히터 Triac
static void SmartCheckStep_22(void);				//	히터 Relay
static void SmartCheckStep_23(void);				//	1stNosValve
static void SmartCheckStep_24(void);				//	HotInValve
static void SmartCheckStep_25(void);				//	HotOutValve
static void SmartCheckStep_26(void);				//	ColdInValve
static void SmartCheckStep_27(void);				//	ColdOutValve
static void SmartCheckStep_28(void);				//	PureValve
static void SmartCheckStep_29(void);				//	UV
static void SmartCheckStep_2A(void);				//	Fan
static void SmartCheckStep_2B(void);				//	TEM
static void SmartCheckStep_2C(void);				//	유량센서
static void SmartCheckStep_2D(void);				//	SteppingMotor

static void SmartCheckStepNoSet(U8 data);

//static void Current_Decide_forCompOperCheck(U16 *data);

SmartCheck_StepTable SmartCheck_Step[Z_SmartCheckStepNum_Max] =
{
	//	스텝 No			 		Step Func.
	{ Z_SmartCheckStep_00, 		SmartCheckStep_00 },
	{ Z_SmartCheckStep_10, 		SmartCheckStep_10 },
	{ Z_SmartCheckStep_20, 		SmartCheckStep_20 },
	{ Z_SmartCheckStep_21, 		SmartCheckStep_21 },
	{ Z_SmartCheckStep_22, 		SmartCheckStep_22 },
	{ Z_SmartCheckStep_23, 		SmartCheckStep_23 },
	{ Z_SmartCheckStep_24, 		SmartCheckStep_24 },
	{ Z_SmartCheckStep_25, 		SmartCheckStep_25 },
	{ Z_SmartCheckStep_26, 		SmartCheckStep_26 },
	{ Z_SmartCheckStep_27, 		SmartCheckStep_27 },
	{ Z_SmartCheckStep_28, 		SmartCheckStep_28 },
	{ Z_SmartCheckStep_29, 		SmartCheckStep_29 },
	{ Z_SmartCheckStep_2A, 		SmartCheckStep_2A },
	{ Z_SmartCheckStep_2B, 		SmartCheckStep_2B },
	{ Z_SmartCheckStep_2C, 		SmartCheckStep_2C },
	{ Z_SmartCheckStep_2D, 		SmartCheckStep_2D },
};

/// @brief    초기화
/// @param    void
/// @return   void
void SmartCheck_Initialize(void)
{
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
	Ini_SmartCheckStep_00();
}

/// @brief    SmartCheck TestMode 제어
/// @param    void
/// @return   void
void SmartCheckControl(void)
{
	static U8 mu8_SmartCheck_Percent_Before = 0;
	if (gu8_SmartCheckStepNum < Z_SmartCheckStepNum_Max)			//	스텝 허용 범위 이내
	{
		(SmartCheck_Step[gu8_SmartCheckStepNum].exe)();			//		해당 스텝 함수 실행

		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			gu8_SmartCheck_Percent = (U8)(((gu8_SmartCheckStepNum - Z_SmartCheckStepNum_20) * 100) / ((Z_SmartCheckStepNum_Max-1) - Z_SmartCheckStepNum_20));
			gu8_SmartCheck_Percent = (U8)(gu8_SmartCheck_Percent / 10 * 10);
			if (mu8_SmartCheck_Percent_Before != gu8_SmartCheck_Percent)
			{
				WifiSendData(WIFI_DATA_FUNCTION);
			}
			mu8_SmartCheck_Percent_Before = gu8_SmartCheck_Percent;

			if (F_SmartCheck == CLEAR)					//	서버에서 정지 신호 시
			{
				F_SmartCheck_Stop = SET;
				Ini_SmartCheckStep_10();
//서버제어는 ack가 있음				WifiSendData(WIFI_DATA_FUNCTION);
			}
			else if (gu8_OperationStep != Z_OperationStep_80)
			{
				F_SmartCheck_Stop = SET;
				Ini_SmartCheckStep_10();
				WifiSendData(WIFI_DATA_FUNCTION);
			}
			else
			{

			}
		}
	}
	else											//	스텝범위 외
	{
		SafetyLoop();								//		안전정지 루프
	}
}

/// @brief    부팅
/// @param    void
/// @return   void
static void SmartCheckStep_00(void)
{
	Ini_SmartCheckStep_10();
}
static void Ini_SmartCheckStep_00(void)
{
	SmartCheckStepNoSet(Z_SmartCheckStepNum_00);
}

/// @brief    대기
/// @param    void
/// @return   void
static void SmartCheckStep_10(void)
{
	if (gu8_OperationStep == Z_OperationStep_80)
	{
		Ini_SmartCheckStep_20();
	}
}
static void Ini_SmartCheckStep_10(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_10);
}

/// @brief    ALL OFF
/// @param    void
/// @return   void
static void SmartCheckStep_20(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		Ini_SmartCheckStep_21();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_20(void)
{
	gu16_TimerSmartCheckDisplay = Z_TimerSmartCheckDisplay;

	gu16_SmartCheck_Result.Word = 0x0000U;
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	gu8_SmartCheck_Percent = 0;

	gu16_SmartCheck_Result_HeaterTriac_ADC = 0;
	gu16_SmartCheck_Result_HeaterRelay_ADC = 0;
	gu16_SmartCheck_Result_Pure_VV_mA = 0;
	gu16_SmartCheck_Result_HotIn_VV_mA = 0;
	gu16_SmartCheck_Result_HotOut_VV_mA = 0;
	gu16_SmartCheck_Result_ColdIn_VV_mA = 0;
	gu16_SmartCheck_Result_ColdOut_VV_mA = 0;
	gu16_SmartCheck_Result_1stNos_VV_mA = 0;
	gu16_SmartCheck_Result_Fan_mA = 0;
	gu16_SmartCheck_Result_TEM_mA = 0;
	gu16_SmartCheck_Result_FlowMeter = 0;
	gu16_SmartCheck_Result_UV_mA = 0;
	gu16_SmartCheck_Result_SteppingMotor_mA = 0;

	SmartCheckStepNoSet(Z_SmartCheckStepNum_20);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_21(void)
{
	if (gu8_TimerSmartCheck_10ms == 0)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			if (ADC_RESULT_HEATER_FB > gu16_SmartCheck_Result_HeaterTriac_ADC)
			{
				gu16_SmartCheck_Result_HeaterTriac_ADC = ADC_RESULT_HEATER_FB;
			}
		}
		else
		{
			gu16_SmartCheck_Result_HeaterTriac_ADC = 0;
		}
		F_SmartCheck_HeaterTriac = CLEAR;
		if (gu16_SmartCheck_Result_HeaterTriac_ADC >= 50)
		{
			F_SmartCheck_Result_HeaterTriac = SET;
		}
		if (gu8_TimerSmartCheck_100ms == 0)
		{
			Ini_SmartCheckStep_22();
		}
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_21(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_HeaterTriac = SET;
	gu8_TimerSmartCheck_10ms = Z_TimerSmartCheck_10ms_500ms;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_2s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_21);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_22(void)
{
	if (gu8_TimerSmartCheck_10ms == 0)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			if (ADC_RESULT_HEATER_FB > gu16_SmartCheck_Result_HeaterRelay_ADC)
			{
				gu16_SmartCheck_Result_HeaterRelay_ADC = ADC_RESULT_HEATER_FB;
			}
		}
		else
		{
			gu16_SmartCheck_Result_HeaterRelay_ADC = 0;
		}
		F_SmartCheck_HeaterRelay = CLEAR;
		if (gu16_SmartCheck_Result_HeaterRelay_ADC >= 50)
		{
			F_SmartCheck_Result_HeaterRelay = SET;
		}
		if (gu8_TimerSmartCheck_100ms == 0)
		{
			Ini_SmartCheckStep_23();
		}
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_22(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_HeaterRelay = SET;
	gu8_TimerSmartCheck_10ms = Z_TimerSmartCheck_10ms_500ms;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_2s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_22);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_23(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		gu16_SmartCheck_Result_1stNos_VV_mA = gu16_DC_mA_Result;
		if ((gu16_SmartCheck_Result_1stNos_VV_mA >= 100) && (gu16_SmartCheck_Result_1stNos_VV_mA <= 300))
		{
			F_SmartCheck_Result_1stNos_VV = SET;
		}
		Ini_SmartCheckStep_24();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_23(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_1stNos_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_1_5s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_23);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_24(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			if (gu16_DC_mA_Result >= gu16_SmartCheck_Result_1stNos_VV_mA)
			{
				gu16_SmartCheck_Result_HotIn_VV_mA = gu16_DC_mA_Result - gu16_SmartCheck_Result_1stNos_VV_mA;
			}
			else
			{
				gu16_SmartCheck_Result_HotIn_VV_mA = 0;
			}
		}
		else
		{
			gu16_SmartCheck_Result_HotIn_VV_mA = 0;
		}
		if ((gu16_SmartCheck_Result_HotIn_VV_mA >= 100) && (gu16_SmartCheck_Result_HotIn_VV_mA <= 300))
		{
			F_SmartCheck_Result_HotIn_VV = SET;
		}
		Ini_SmartCheckStep_25();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_24(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_1stNos_VV = SET;
	F_SmartCheck_HotIn_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_1_5s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_24);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_25(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			if (gu16_DC_mA_Result >= gu16_SmartCheck_Result_1stNos_VV_mA)
			{
				gu16_SmartCheck_Result_HotOut_VV_mA = gu16_DC_mA_Result - gu16_SmartCheck_Result_1stNos_VV_mA;
			}
			else
			{
				gu16_SmartCheck_Result_HotOut_VV_mA = 0;
			}
		}
		else
		{
			gu16_SmartCheck_Result_HotOut_VV_mA = 0;
		}
		if ((gu16_SmartCheck_Result_HotOut_VV_mA >= 100) && (gu16_SmartCheck_Result_HotOut_VV_mA <= 300))
		{
			F_SmartCheck_Result_HotOut_VV = SET;
		}
		Ini_SmartCheckStep_26();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_25(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_1stNos_VV = SET;
	F_SmartCheck_HotOut_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_1_5s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_25);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_26(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu16_DC_mA_Result >= gu16_SmartCheck_Result_1stNos_VV_mA)
		{
			gu16_SmartCheck_Result_ColdIn_VV_mA = gu16_DC_mA_Result - gu16_SmartCheck_Result_1stNos_VV_mA;
		}
		else
		{
			gu16_SmartCheck_Result_ColdIn_VV_mA = 0;
		}
		if ((gu16_SmartCheck_Result_ColdIn_VV_mA >= 100) && (gu16_SmartCheck_Result_ColdIn_VV_mA <= 300))
		{
			F_SmartCheck_Result_ColdIn_VV = SET;
		}
		Ini_SmartCheckStep_27();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_26(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_1stNos_VV = SET;
	F_SmartCheck_ColdIn_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_1_5s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_26);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_27(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu16_DC_mA_Result >= gu16_SmartCheck_Result_1stNos_VV_mA)
		{
			gu16_SmartCheck_Result_ColdOut_VV_mA = gu16_DC_mA_Result - gu16_SmartCheck_Result_1stNos_VV_mA;
		}
		else
		{
			gu16_SmartCheck_Result_ColdOut_VV_mA = 0;
		}
		if ((gu16_SmartCheck_Result_ColdOut_VV_mA >= 100) && (gu16_SmartCheck_Result_ColdOut_VV_mA <= 300))
		{
			F_SmartCheck_Result_ColdOut_VV = SET;
		}
		Ini_SmartCheckStep_28();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_27(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_1stNos_VV = SET;
	F_SmartCheck_ColdOut_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_1_5s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_27);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_28(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu16_DC_mA_Result >= gu16_SmartCheck_Result_1stNos_VV_mA)
		{
			gu16_SmartCheck_Result_Pure_VV_mA = gu16_DC_mA_Result - gu16_SmartCheck_Result_1stNos_VV_mA;
		}
		else
		{
			gu16_SmartCheck_Result_Pure_VV_mA = 0;
		}
		if ((gu16_SmartCheck_Result_Pure_VV_mA >= 100) && (gu16_SmartCheck_Result_Pure_VV_mA <= 300))
		{
			F_SmartCheck_Result_Pure_VV = SET;
		}
		Ini_SmartCheckStep_29();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_28(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_1stNos_VV = SET;
	F_SmartCheck_Pure_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_1_5s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_28);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_29(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		gu16_SmartCheck_Result_UV_mA = gu16_Front_UV_FB;
		if ((gu16_SmartCheck_Result_UV_mA >= 10) && (gu16_SmartCheck_Result_UV_mA <= 100))
		{
			F_SmartCheck_Result_UV = SET;
		}
		Ini_SmartCheckStep_2A();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_29(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_UV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_3s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_29);
}

/// @brief    팬 검사
/// @param    void
/// @return   void
static void SmartCheckStep_2A(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if ((ADC_RESULT_FAN_FB >= 5) && (ADC_RESULT_FAN_FB <= 100))
		{
			gu16_SmartCheck_Result_Fan_mA = ADC_RESULT_FAN_FB+10;
			F_SmartCheck_Result_Fan = SET;
		}
		Ini_SmartCheckStep_2B();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_2A(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_Fan = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_3s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_2A);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_2B(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		gu16_SmartCheck_Result_TEM_mA = gu16_TEM_mA_Result;
		if (gu16_SmartCheck_Result_TEM_mA > 6000)
		{
			gu16_SmartCheck_Result_TEM_mA = 6000;
		}
		if ((ADC_RESULT_TEM_FB >= 50) && (ADC_RESULT_TEM_FB <= 700))
		{
			F_SmartCheck_Result_TEM = SET;
		}
		Ini_SmartCheckStep_2C();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_2B(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_TEM = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_3s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_2B);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_2C(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu16_FlowDecide_Hz > 40)				//	4Hz(약 0.1LPM) 이상일 시
		{
			F_SmartCheck_Result_FlowMeter = SET;
		}
		gu16_SmartCheck_Result_FlowMeter = gu16_FlowDecide_W * 10;		//	[1/1cc]
		Ini_SmartCheckStep_2D();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_2C(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_Pure_VV = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_2s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_2C);
}

/// @brief
/// @param    void
/// @return   void
static void SmartCheckStep_2D(void)
{
	if (gu8_TimerSmartCheck_100ms == 0)
	{
		if (gu8_ModelType_forCHP == MODEL_CHP)
		{
			gu16_SmartCheck_Result_SteppingMotor_mA = gu16_DC_mA_Result;
		}
		else
		{
			gu16_SmartCheck_Result_SteppingMotor_mA = 0;
		}
		if (gu16_SmartCheck_Result_SteppingMotor_mA >= 50)
		{
			F_SmartCheck_Result_SteppingMotor = SET;
		}
		F_SmartCheck_Complete = SET;
		F_SmartCheck_Complete_2 = SET;
		F_SmartCheck = CLEAR;
		WifiSendData(WIFI_DATA_EXAMINE);
		Ini_SmartCheckStep_10();
	}
	else
	{

	}
}
static void Ini_SmartCheckStep_2D(void)
{
	gu16_SmartCheck_LoadCMD.Word = 0x0000U;
	F_SmartCheck_SteppingMotor = SET;
	gu8_TimerSmartCheck_100ms = Z_TimerSmartCheck_100ms_2s;
	SmartCheckStepNoSet(Z_SmartCheckStepNum_2D);
}

/// @brief    검사모드 스텝 설정
/// @param    void
/// @return   void
static void SmartCheckStepNoSet(U8 data)
{
	static const U8 DT_SmartCheckNumber[]
	= {
		Z_SmartCheckStep_00,
		Z_SmartCheckStep_10,
		Z_SmartCheckStep_20,
		Z_SmartCheckStep_21,
		Z_SmartCheckStep_22,
		Z_SmartCheckStep_23,
		Z_SmartCheckStep_24,
		Z_SmartCheckStep_25,
		Z_SmartCheckStep_26,
		Z_SmartCheckStep_27,
		Z_SmartCheckStep_28,
		Z_SmartCheckStep_29,
		Z_SmartCheckStep_2A,
		Z_SmartCheckStep_2B,
		Z_SmartCheckStep_2C,
		Z_SmartCheckStep_2D,
	};
	/************************************************/
	gu8_SmartCheckStepNum = data; 							/*	일련 번호 set*/

	gu8_SmartCheckStep = DT_SmartCheckNumber[gu8_SmartCheckStepNum]; 	/*	상태 mode No. set*/
}

///// @brief    콤프레셔 가동에 따른 전류값 결정 (가동시 그만큼 감산)
///// @param    void
///// @return   void
//static void Current_Decide_forCompOperCheck(U16 *data)
//{
//	if (gu8_COMP_CMD == COMP_ON)				//	COMP가 ON상태이면 COMP값의 전류 감산
//	{
//		if (gu16_mA_Result >= gu16_SmartCheck_Result_Comp_mA)
//		{
//			*data = gu16_mA_Result - gu16_SmartCheck_Result_Comp_mA;
//		}
//		else
//		{
//			*data = 0;
//		}
//	}
//	else
//	{
//		*data = gu16_mA_Result;
//	}
//}
