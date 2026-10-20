/// @file     cold.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    냉각 동작 제어 관련 file
#define COLD_C
#include "include/_include.h"

static void Ini_ColdEffStep_00(void);				//	부팅
static void Ini_ColdEffStep_01(void);				//	냉수탱크 물 채우기 대기
static void Ini_ColdEffStep_02(void);				//	냉수탱크 물 채우기 중
static void Ini_ColdEffStep_10(void);				//	대기
static void Ini_ColdEffStep_20(void);				//	냉수 선택
static void Ini_ColdEffStep_21(void);				//	추출 전 추출 밸브 선OPEN
static void Ini_ColdEffStep_22(void);				//	추출
static void Ini_ColdEffStep_23(void);				//	추출 종료 시 추출 밸브 후CLOSE
static void Ini_ColdEffStep_B0(void);				//	잔압 제거
static void Ini_ColdEffStep_C0(void);				//	냉수탱크 진공해제
static void Ini_ColdEffStep_E0(void);				//	냉수 관련 에러

static void ColdEffStep_00(void);					//	부팅
static void ColdEffStep_01(void);					//	냉수탱크 물 채우기 대기
static void ColdEffStep_02(void);					//	냉수탱크 물 채우기 중
static void ColdEffStep_10(void);					//  대기
static void ColdEffStep_20(void);					//  냉수 선택
static void ColdEffStep_21(void);					//  추출 전 추출 밸브 선OPEN
static void ColdEffStep_22(void);					//  추출
static void ColdEffStep_23(void);					//	추출 종료 시 입수밸브 선 CLOSE 후 추출 밸브 CLOSE
static void ColdEffStep_B0(void);					//	잔압 제거
static void ColdEffStep_C0(void);					//	냉수탱크 진공해제
static void ColdEffStep_E0(void);					//	냉수 관련 에러

static void ColdEffStepNoSet(U8 data);

static void Ini_CoolingStep_00(void);
static void Ini_CoolingStep_10(void);
static void Ini_CoolingStep_20(void);
static void Ini_CoolingStep_21(void);
static void Ini_CoolingStep_22(void);				//	에코모드 방치 14시간
static void Ini_CoolingStep_23(void);				//	에코모드 방치 14시간~냉수 추출 사이
static void Ini_CoolingStep_24(void);				//	냉수 첫 추출 후 10시간 방치
static void Ini_CoolingStep_30(void);
static void Ini_CoolingStep_40(void);
static void Ini_CoolingStep_41(void);
static void Ini_CoolingStep_E0(void);

static void CoolingStep_00(void);					//	부팅
static void CoolingStep_10(void);					//	냉각 OFF
static void CoolingStep_20(void);					//	급속모드
static void CoolingStep_21(void);					//	유지모드
static void CoolingStep_22(void);					//	(에코모드) 방치 14시간
static void CoolingStep_23(void);					//	(에코모드) 방치 14시간~냉수 추출 사이
static void CoolingStep_24(void);					//	(에코모드) 냉수 첫 추출 후 10시간 방치
static void CoolingStep_30(void);					//	온수 선택 및 추출
static void CoolingStep_40(void);					//	냉각 종료
static void CoolingStep_41(void);
static void CoolingStep_E0(void);

static void CoolingStepNoSet(U8 data);

static void Cooling_PID_Calculration(void);			//@022
static I32 TEM_Volt_Calculation_P(void);			//@022
static I32 TEM_Volt_Calculation_I(void);			//@022

#define	Z_RetainMode_TEM_Volt_Max	(210)			//@022	21V
#define	Z_RetainMode_TEM_Volt_Min	(40)			//@022	4V
#define Z_Cooling_P_Control_LimitTemp	(Z_TEMP2)	//@022	P제어 산출 리미트 온도
#define Z_Cooling_I_Control_LimitTemp	(Z_TEMP0_3)	//@022	I제어 산출 리미트 온도

ColdEff_StepTable ColdEff_Step[Z_ColdEffStepNum_Max] =
{
	//	스텝 No			냉수 입수 밸브 		냉수 추출 밸브		Step Func.
	{ Z_ColdEffStep_00,	VALVE_OFF, 		VALVE_OFF, 		ColdEffStep_00 },     //	부팅
	{ Z_ColdEffStep_01,	VALVE_OFF,	 	VALVE_OFF,	 	ColdEffStep_01 },     //	냉수탱크 물 채우기 대기
	{ Z_ColdEffStep_02,	VALVE_ON,	 	VALVE_ON,	 	ColdEffStep_02 },     //	냉수탱크 물 채우기 중
	{ Z_ColdEffStep_10,	VALVE_OFF, 		VALVE_OFF, 		ColdEffStep_10 },     //	대기
	{ Z_ColdEffStep_20,	VALVE_OFF, 		VALVE_OFF, 		ColdEffStep_20 },     //	냉수 선택
	{ Z_ColdEffStep_21,	VALVE_OFF, 		VALVE_ON, 		ColdEffStep_21 },     //	추출 전 추출 밸브 선OPEN
	{ Z_ColdEffStep_22,	VALVE_ON, 		VALVE_ON, 		ColdEffStep_22 },     //	추출
	{ Z_ColdEffStep_23,	VALVE_OFF, 		VALVE_ON, 		ColdEffStep_23 },     //	추출 종료 시 입수밸브 선 CLOSE 후 추출 밸브 CLOSE
	{ Z_ColdEffStep_B0,	VALVE_B0, 		VALVE_B0, 		ColdEffStep_B0 },     //
	{ Z_ColdEffStep_C0,	VALVE_OFF, 		VALVE_ON, 		ColdEffStep_C0 },     //
	{ Z_ColdEffStep_E0,	VALVE_OFF, 		VALVE_OFF, 		ColdEffStep_E0 },     //	냉수 관련 에러
};

Cooling_StepTable Cooling_Step[Z_CoolingStepNum_Max] =
{
	//	스텝 No				TEM Enable 		TEM PWM			FAN 18V		Step
	{ Z_CoolingStep_00,		TEM_DISABLE, 	TEM_PWM_OFF,	FAN_OFF,	CoolingStep_00 },     //
	{ Z_CoolingStep_10,		TEM_DISABLE, 	TEM_PWM_OFF,	FAN_OFF,	CoolingStep_10 },     //
	{ Z_CoolingStep_20,		TEM_ENABLE, 	TEM_PWM_RAPID,	FAN_21V_ON,	CoolingStep_20 },     //	급속모드
//@009	{ Z_CoolingStep_21,		TEM_ENABLE, 	TEM_PWM_RETAIN,	FAN_21V_ON,	CoolingStep_21 },     //	유지모드
//@009	{ Z_CoolingStep_22,		TEM_ENABLE, 	TEM_PWM_ECOMODE,FAN_21V_ON,	CoolingStep_22 },     //	에코모드 방치 14시간
//@009	{ Z_CoolingStep_23,		TEM_ENABLE, 	TEM_PWM_ECOMODE,FAN_21V_ON,	CoolingStep_23 },     //	에코모드 방치 14시간~냉수 추출 사이
//@009	{ Z_CoolingStep_24,		TEM_ENABLE, 	TEM_PWM_ECO_9V,	FAN_21V_ON,	CoolingStep_24 },     //	냉수 첫 추출 후 10시간 방치
//@009	{ Z_CoolingStep_30,		TEM_ENABLE, 	TEM_PWM_MIN,	FAN_21V_ON,	CoolingStep_30 },     //	온수 선택
	{ Z_CoolingStep_21,		TEM_ENABLE, 	TEM_PWM_RETAIN,	FAN_18V_ON,	CoolingStep_21 },     //@009	유지모드
	{ Z_CoolingStep_22,		TEM_ENABLE, 	TEM_PWM_ECOMODE,FAN_18V_ON,	CoolingStep_22 },     //@009	에코모드 방치 14시간
	{ Z_CoolingStep_23,		TEM_ENABLE, 	TEM_PWM_ECOMODE,FAN_18V_ON,	CoolingStep_23 },     //@009	에코모드 방치 14시간~냉수 추출 사이
	{ Z_CoolingStep_24,		TEM_ENABLE, 	TEM_PWM_ECO_9V,	FAN_18V_ON,	CoolingStep_24 },     //@009	냉수 첫 추출 후 10시간 방치
	{ Z_CoolingStep_30,		TEM_ENABLE, 	TEM_PWM_MIN,	FAN_18V_ON,	CoolingStep_30 },     //@009	온수 선택
	{ Z_CoolingStep_40,		TEM_DISABLE, 	TEM_PWM_OFF,	FAN_OFF,	CoolingStep_40 },     //	냉각 종료
	{ Z_CoolingStep_41,		TEM_DISABLE, 	TEM_PWM_OFF,	FAN_OFF,	CoolingStep_41 },     //	냉각 10분 정지
	{ Z_CoolingStep_E0,		TEM_DISABLE, 	TEM_PWM_OFF,	FAN_OFF,	CoolingStep_E0 },     //
};

/// @brief    냉각 제어 초기화
/// @param    void
/// @return   void
void Cold_Initialize(void)
{
	gu16_Cooling_Dp1 = 20;							//@022
	gu16_Cooling_Dp2 = 10;							//@022
	gu16_Cooling_Di1 = 3500;						//@022
	gu16_Cooling_TargetDegree = Z_TEMP3;			//@022

	gi16_TEM_Volt_I = 70;							//@022	초기 I전압 7V

	F_ColdOff = CLEAR;
	F_ColdTank_Fill = CLEAR;

#ifdef _FOR_DEVELOP_
	gu16_ColdTimeData = Z_TimerCoolingRapid_Out33Degree;
#endif

	gu16_TimerCoolingRapid_6Hour = Z_TimerCoolingRapid_6Hour;

	Ini_ColdEffStep_00();
	Ini_CoolingStep_00();
}

/// @brief    냉각 제어
/// @param    void
/// @return   void
void ColdControl(void)
{
	static U8 F_ColdWater_EffStart = CLEAR;
	U8 mu8_OPENSHORT_Error;

	if (gu8_ColdEffStepNum < Z_ColdEffStepNum_Max)		//	스텝 허용 범위인 경우
	{
		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
		if ((gu8_Key_Short.byte != 0)
		|| (mu8_OPENSHORT_Error != 0)
		|| (gi16_OutDoorAirSensor.DEGREE < Z_TEMP23)
		|| (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP29)
		|| (F_ColdOff == SET)
		|| (gu8_CoolingStep == Z_CoolingStep_E0)
		|| ((gu8_CoolingStep >= Z_CoolingStep_22) && (gu8_CoolingStep <= Z_CoolingStep_24)))
		{
			gu16_TimerColdEcoMode_24Hour = Z_TimerColdEcoMode_24Hour;
		}

		(ColdEff_Step[gu8_ColdEffStepNum].exe)();				//	해당 스텝 함수 실행
		(Cooling_Step[gu8_CoolingStepNum].exe)();				//	해당 스텝 함수 실행

		gu8_Cold_In_Valve_CMD = ColdEff_Step[gu8_ColdEffStepNum].COLD_IN_V;
		gu8_Cold_Out_Valve_CMD = ColdEff_Step[gu8_ColdEffStepNum].COLD_OUT_V;

		gu8_TEM_PWM_CMD = Cooling_Step[gu8_CoolingStepNum].TEM_PWM;
		//gu8_TEM_EN_CMD = Cooling_Step[gu8_CoolingStepNum].TEM_EN;
		gu8_FAN_CMD = Cooling_Step[gu8_CoolingStepNum].FAN;

		if ((gu8_ColdEffStep != Z_ColdEffStep_B0) && (gu8_OperationStep == Z_OperationStep_B0))	//	탑 커버 잔압 제거
		{
			Ini_ColdEffStep_B0();
		}
		else if ((gu8_ColdEffStep != Z_ColdEffStep_C0) && (gu8_OperationStep == Z_OperationStep_C0))	//	냉수탱크 진공 해제
		{
			Ini_ColdEffStep_C0();
		}
#ifndef _NO_ERROR_
		else if (((F_Err25_HeatSinkTH == SET) || (F_Err44_ColdTH == SET) || (F_Err29_TEM_OverDrive1 == SET) || (F_Err29_TEM_OverDrive2 == SET))	//	냉수 관련 에러 (냉각 불가)
			&& (gu8_ColdEffStep > Z_ColdEffStep_02) && (gu8_ColdEffStep != Z_ColdEffStep_E0) && (gu8_ColdEffStep != Z_ColdEffStep_B0) && (gu8_ColdEffStep != Z_ColdEffStep_C0) && (F_ColdOff == CLEAR))
		{
			Ini_ColdEffStep_E0();							//		E0 이행
		}
		else
		{

		}

//@018		if ((gu8_CoolingStep == Z_CoolingStep_10) || (gu8_CoolingStep == Z_CoolingStep_21) || (gu8_CoolingStep == Z_CoolingStep_40))
		if ((gu8_CoolingStep == Z_CoolingStep_10) || ((gu8_CoolingStep != Z_CoolingStep_20) && (gu8_CoolingStep_hi == 0x20)) || (gu8_CoolingStep == Z_CoolingStep_40))	//@018
		{
			gu16_TimerCoolingRapid_6Hour = Z_TimerCoolingRapid_6Hour;
		}

		if (((F_Err01_LeakWater == SET) || (F_Err02_LeakWater == SET) || (F_Err25_HeatSinkTH == SET) || (F_Err44_ColdTH == SET) || (F_Err29_TEM_OverDrive1 == SET) || (F_Err29_TEM_OverDrive2 == SET))	//	냉수 관련 에러 (냉각 불가)
			&& (gu8_CoolingStep != Z_CoolingStep_E0) && (F_ColdOff == CLEAR))
		{
			Ini_CoolingStep_E0();							//		E0 이행
		}
#endif
	}
	else											//	허용범위 외
	{
		SafetyLoop();								//		안전정지 루프
	}
}

/// @brief    부팅 후
/// @param    void
/// @return   void
static void ColdEffStep_00(void)
{
#ifndef _NO_FLUSHING_
	if (F_FirstFlushing == SET)						//	초기 플러싱이 완료 된 경우
	{
		Ini_ColdEffStep_10();						//		대기 이행
	}
	else
	{
		Ini_ColdEffStep_01();						//		냉수탱크 채움 대기 이행
	}
#else
	Ini_ColdEffStep_10();							//		냉각 대기 이행
#endif
}
static void Ini_ColdEffStep_00(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_00);
}

/// @brief    냉수탱크 물 채우기 대기
/// @param    void
/// @return   void
static void ColdEffStep_01(void)
{
	if (gu8_OperationStep == Z_OperationStep_05)
	{
		Ini_ColdEffStep_02();
	}
	else if (gu8_OperationStep_hi != 0x00)
	{
		Ini_ColdEffStep_10();
	}
	else if (F_ColdTank_Fill == SET)
	{
		Ini_ColdEffStep_10();
	}
	else
	{

	}
}
static void Ini_ColdEffStep_01(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_01);
}

/// @brief    냉수탱크 물 채우기
/// @param    void
/// @return   void
static void ColdEffStep_02(void)
{
	if (F_ColdTank_Fill == SET)						//	추출 완료 시
	{
		Ini_ColdEffStep_10();
	}
	else if (gu8_OperationStep != Z_OperationStep_05)	//	탱크 물 채우기 중이 아닌 경우
	{
		Ini_ColdEffStep_01();							//		플러싱 대기로 이행
	}
	else
	{

	}
}
static void Ini_ColdEffStep_02(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_02);
}

/// @brief    대기
/// @param    void
/// @return   void
static void ColdEffStep_10(void)
{
	if (gu8_WaterSelect == COLD_WATER)
	{
		Ini_ColdEffStep_20();
	}
	else if (F_ColdTank_Fill == CLEAR)
	{
		Ini_ColdEffStep_01();
	}
	else
	{

	}
}
static void Ini_ColdEffStep_10(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_10);
}

/// @brief    냉수 선택
/// @param    void
/// @return   void
static void ColdEffStep_20(void)
{
	if (gu8_WaterSelect != COLD_WATER)
	{
		Ini_ColdEffStep_10();
	}
	else if (gu8_OperationStep == Z_OperationStep_50)
	{
		Ini_ColdEffStep_21();
	}
	else if (F_ColdTank_Fill == CLEAR)
	{
		Ini_ColdEffStep_01();
	}
	else
	{

	}
}
static void Ini_ColdEffStep_20(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_20);
}

/// @brief    냉수 추출 전 냉수출수밸브 선 OPEN
/// @param    void
/// @return   void
static void ColdEffStep_21(void)
{
	if (gu8_TimerColdEffStep_10ms == 0)
	{
		Ini_ColdEffStep_22();
	}
	else if (gu8_OperationStep != Z_OperationStep_50)
	{
		Ini_ColdEffStep_10();
	}
	else
	{

	}
}
static void Ini_ColdEffStep_21(void)
{
	Effluent_Hz_Decide();
	gu8_TimerColdEffStep_10ms = Z_TimerColdEffStep_10ms_100ms;
	if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP30)
	{
		gu8_TimerColdEff_FirstCup = Z_TimerColdEff_FirstCup;
	}
	ColdEffStepNoSet(Z_ColdEffStepNum_21);
}

/// @brief    냉수 추출
/// @param    void
/// @return   void
static void ColdEffStep_22(void)
{
	if (gu8_OperationStep != Z_OperationStep_50)
	{
//		if (F_Cooling_PID == SET)					//@022	★★미적용
//		{                                           //@022
//			if (gu16_CountColdEff_Sec > 3)			//@022	3초 이상 추출 시
//			{										//@022
//				gu16_TimerForced_RapidCooling = gu16_TimerForced_RapidCooling + (gu16_CountColdEff_Sec * 10);	//@022	잔여 시간 + 추출 시간
//			}										//@022
//		}                                           //@022
		Ini_ColdEffStep_23();
	}
}
static void Ini_ColdEffStep_22(void)
{
	gu16_CountColdEff_Sec = 0; 						//@022
	ColdEffStepNoSet(Z_ColdEffStepNum_22);
}

/// @brief    냉수 추출 종료 시 냉수입수밸브  선 CLOSE
/// @param    void
/// @return   void
static void ColdEffStep_23(void)
{
	if (gu8_TimerColdEffStep_10ms == 0)
	{
		Ini_ColdEffStep_10();
	}
}
static void Ini_ColdEffStep_23(void)
{
	gu8_TimerColdEffStep_10ms = Z_TimerColdEffStep_10ms_100ms;
	ColdEffStepNoSet(Z_ColdEffStepNum_23);
}

/// @brief    내부 잔압 제거
/// @param    void
/// @return   void
static void ColdEffStep_B0(void)
{
	if (gu8_TimerPressureClear_ColdVV_Delay != 0)
	{
		gu8_TimerPressureClear_ColdVV = Z_TimerPressureClear_ColdVV;
	}

	if (gu8_OperationStep != Z_OperationStep_B0)
	{
		if (F_ColdTank_Fill == CLEAR)
		{
			Ini_ColdEffStep_01();
		}
		else
		{
			Ini_ColdEffStep_10();
		}
	}
}
static void Ini_ColdEffStep_B0(void)
{
	gu8_TimerPressureClear_ColdVV_Delay = Z_TimerPressureClear_ColdVV_Delay;
	gu8_TimerPressureClear_ColdVV = Z_TimerPressureClear_ColdVV;
	ColdEffStepNoSet(Z_ColdEffStepNum_B0);
}

/// @brief    냉수탱크 진공해제
/// @param    void
/// @return   void
static void ColdEffStep_C0(void)
{
	if (gu8_OperationStep != Z_OperationStep_C0)
	{
		if (F_ColdTank_Fill == CLEAR)
		{
			Ini_ColdEffStep_01();
		}
		else
		{
			Ini_ColdEffStep_10();
		}
	}
}
static void Ini_ColdEffStep_C0(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_C0);
}

/// @brief    냉각 관련 에러 상태 (냉각 금지)
/// @param    void
/// @return   void
static void ColdEffStep_E0(void)
{
	if ((F_Err25_HeatSinkTH == CLEAR) && (F_Err44_ColdTH == CLEAR) && (F_Err29_TEM_OverDrive1 == CLEAR) && (F_Err29_TEM_OverDrive2 == CLEAR))
	{
		Ini_ColdEffStep_10();
	}
}
static void Ini_ColdEffStep_E0(void)
{
	ColdEffStepNoSet(Z_ColdEffStepNum_E0);
}

/// @brief    냉각 스텝 설정
/// @param    void
/// @return   void
static void ColdEffStepNoSet(U8 data)
{
	static const U8 DT_ColdNumber[] =
	{
		Z_ColdEffStep_00,
		Z_ColdEffStep_01,
		Z_ColdEffStep_02,
		Z_ColdEffStep_10,
		Z_ColdEffStep_20,
		Z_ColdEffStep_21,
		Z_ColdEffStep_22,
		Z_ColdEffStep_23,
		Z_ColdEffStep_B0,
		Z_ColdEffStep_C0,
		Z_ColdEffStep_E0,
	};
	/************************************************/
	gu8_ColdEffStepNum = data; 							/*	일련 번호 set*/

	gu8_ColdEffStep = DT_ColdNumber[gu8_ColdEffStepNum];	/*	상태 mode No. set*/
}

/// @brief    부팅
/// @param    void
/// @return   void
static void CoolingStep_00(void)
{
	if (gu8_OperationStep >= Z_OperationStep_02)
	{
		Ini_CoolingStep_10();
	}
}
static void Ini_CoolingStep_00(void)
{
	CoolingStepNoSet(Z_CoolingStepNum_00);
}

/// @brief    냉각 OFF
/// @param    void
/// @return   void
static void CoolingStep_10(void)
{
	if (gi16_HeatSinkSensor.DEGREE >= Z_Cold_HeatSinkTH_CutDegree)
	{
		gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	}

	if ((F_ColdOff == CLEAR) && (gu8_TimerCoolingStep_100ms_3 == 0))
	{
//@012		Ini_CoolingStep_20();
		if (gi16_ColdSensor.DEGREE >= RAPID_OPERATE_DEGREE)	//@012
		{                                                   //@012
			Ini_CoolingStep_20();                           //@012
		}                                                   //@012
		else                                                //@012
		{                                                   //@012
			Ini_CoolingStep_21();                           //@012
		}                                                   //@012
	}
}
static void Ini_CoolingStep_10(void)
{
	F_CoolingRapid_Complete = CLEAR;
	gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	CoolingStepNoSet(Z_CoolingStepNum_10);
}

/// @brief    급속모드
/// @param    void
/// @return   void

//#define COLD_COMPLETE_DEGREE		(Z_TEMP1)		//	냉각 완료 온도
//#define RETAIN_JOIN_DEGREE		(Z_TEMP3_5)		//	급속모드 중 유지모드 진입 온도
//#define RAPID_COMPLETE_DEGREE		(Z_TEMP6_5)		//	급속모드 완료 온도
//#define RAPID_OPERATE_DEGREE		(Z_TEMP7_5)		//	급속모드 진입 온도
//#define COLD_REOPERATE_DEGREE		(Z_TEMP2_5)		//	냉각 종료 중 냉각 재가동 온도
static void CoolingStep_20(void)
{
	U8 mu8_OPENSHORT_Error = 0;

	if (gi16_ColdSensor.DEGREE >= COLD_COMPLETE_DEGREE)			//	냉각 종료 조건 체크
	{
		gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gi16_ColdSensor.DEGREE >= RETAIN_JOIN_DEGREE)		//	유지모드 바로 진입 온도 조건
	{
		gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	}

	if ((gi16_ColdSensor.DEGREE >= Z_TEMP6) || (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP6))
	{
		gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
	}

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);

	if (gi16_ColdSensor.DEGREE >= RAPID_COMPLETE_DEGREE)	//	급속모드 완료조건 체크
	{
		if (F_CoolingRapid_Complete == CLEAR)
		{
			if (mu8_OPENSHORT_Error == 0)
			{
				if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP33)
				{
#ifndef _FOR_DEVELOP_
					gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out33Degree;
#else
					gu16_TimerCoolingRapid = gu16_ColdTimeData;
#endif
				}
				else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP30)
				{
					gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out30Degree;  //3600/1sec = 60min = 1hour
				}
				else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP20)
				{
					gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out20Degree;
				}
				else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP12)
				{
					gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out12Degree;
				}
				else
				{
					gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out6Degree;
				}
			}
			else
			{
				//gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out12Degree;		//	외기온도센서 고장 시
				gu16_TimerCoolingRapid = Z_TimerCoolingRapid_Out20Degree;		//	외기온도센서 고장 시
			}
		}
		else
		{
			F_CoolingRapid_Complete = SET;
		}
	}

//	if (gu8_TimerCoolingStep_100ms_3 == 0)
//	{
//		Ini_CoolingStep_10();
//	}
//	else
	if (gu16_TimerCoolingRapid == 0)				//	추가 냉각 완료 시
	{
		Ini_CoolingStep_21();
	}
	else if (gu8_TimerCoolingStep_100ms_3 == 0)
	{
		Ini_CoolingStep_21();
	}
	else if (gu16_TimerCoolingRapid_6Hour == 0)
	{
		Ini_CoolingStep_41();
	}
	else if (F_ColdOff == SET)
	{
		Ini_CoolingStep_10();
	}
	else if (gu8_WaterSelect == HOT_WATER)			//	온수 선택 시
	{
		Ini_CoolingStep_30();
	}
	else if (gu8_TimerCoolingStep_100ms_2 == 0)		//	1도 과냉 판단
	{
		Ini_CoolingStep_40();
	}
	else if (gu8_TimerCoolingStep_100ms_4 == 0) 	//	냉수 6도 and 외기 6도 판단
	{
		Ini_CoolingStep_40();
	}
//@011	else if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu16_TimerColdEcoMode_24Hour == 0))
	else if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu16_TimerColdEcoMode_24Hour == 0) && (F_CoolingEcoMode_Disable == CLEAR))	//@011
	{
		Ini_CoolingStep_22();
	}
	else
	{

	}
}
static void Ini_CoolingStep_20(void)
{
	gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
	
	//20260101
	gu16_Timer_CoolingRapid_12V = Z_TIMER_COOLINGRAPID_12V;
	
	CoolingStepNoSet(Z_CoolingStepNum_20);
}

/// @brief    유지모드
/// @param    void
/// @return   void
static void CoolingStep_21(void)
{
	U8 mu8_OPENSHORT_Error = 0;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);

	if (gu8_TimerCooling_PID == 0)					//@022
	{                                               //@022
		gu8_TimerCooling_PID = Z_TimerCooling_PID;  //@022
		Cooling_PID_Calculration();                 //@022
	}                                               //@022

	if (gi16_ColdSensor.DEGREE < RAPID_OPERATE_DEGREE)  //7.2 degrees or higher
	{
		gu8_TimerCoolingStep_100ms = Z_TimerCoolingStep_100ms_3s;
	}

//	if (gi16_HeatSinkSensor.DEGREE < Z_Cold_HeatSinkTH_CutDegree)
//	{
//		gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
//	}

	if (gi16_ColdSensor.DEGREE >= COLD_COMPLETE_DEGREE) //1 degrees or less
	{
		gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	}

	if (mu8_OPENSHORT_Error == 0)
	{
		if ((gi16_ColdSensor.DEGREE >= Z_TEMP6) || (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP6))	//	냉각 정지조건 판단
		{
			gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
		}
	}
	else
	{
		gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
	}

//	if (gu8_TimerCoolingStep_100ms_3 == 0)
//	{
//		Ini_CoolingStep_10();
//	}
//	else
	if (gu8_TimerCoolingStep_100ms == 0)		//	급속모드 진입 판단
	{
		Ini_CoolingStep_20();
	}
	else if (gu8_WaterSelect == HOT_WATER)			//	온수 선택 시
	{
		Ini_CoolingStep_30();
	}
	else if (gu8_TimerCoolingStep_100ms_2 == 0)		//	1도 과냉 판단
	{
		Ini_CoolingStep_40();
	}
	else if (gu8_TimerCoolingStep_100ms_4 == 0)		//	냉수 6도 and 외기 6도 판단
	{
		Ini_CoolingStep_40();
	}
//@011	else if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu16_TimerColdEcoMode_24Hour == 0))
	else if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu16_TimerColdEcoMode_24Hour == 0) && (F_CoolingEcoMode_Disable == CLEAR))	//@011
	{
		Ini_CoolingStep_22();
	}
	else if (F_ColdOff == SET)
	{
		Ini_CoolingStep_10();
	}
	else
	{

	}
}
static void Ini_CoolingStep_21(void)
{
	U16 mu16_I_Volt = 0;								//@022

	F_CoolingRapid_Complete = CLEAR;
	gu8_TimerCoolingStep_100ms = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
//	gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;

	//@022	시작
	if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP38) mu16_I_Volt = 180;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP33) mu16_I_Volt = 170;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP31) mu16_I_Volt = 120;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP27) mu16_I_Volt = 100;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP24) mu16_I_Volt = 90;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP18) mu16_I_Volt = 80;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP15) mu16_I_Volt = 50;
	else if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP6) mu16_I_Volt = 40;
	else mu16_I_Volt = 30;

	if (gu8_CoolingStep == Z_CoolingStep_20)		//	급속모드에서 유지모드 넘어올 때 최저 I값 설정
	{
		if (gi16_TEM_Volt_I < mu16_I_Volt)
		{
			gi16_TEM_Volt_I = (I16)mu16_I_Volt;
		}
	}
	else
	{
		gi16_TEM_Volt_I = (I16)mu16_I_Volt;
	}
	//@022	끝

	CoolingStepNoSet(Z_CoolingStepNum_21);
}

/// @brief    에코모드 방치 14시간
/// @param    void
/// @return   void
static void CoolingStep_22(void)
{
	U8 mu8_OPENSHORT_Error = 0;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);

	if ((mu8_OPENSHORT_Error == 0) && (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP23) && (gi16_OutDoorAirSensor.DEGREE < Z_TEMP29))
	{
		gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gi16_ColdSensor.DEGREE >= COLD_COMPLETE_DEGREE)
	{
		gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gu8_TimerCoolingStep_100ms_2 == 0)		//	1도 과냉 판단
	{
		Ini_CoolingStep_40();
	}
	else if (gu8_TimerCoolingStep_100ms_3 == 0)
	{
		Ini_CoolingStep_21();
	}
	else if (F_CoolingEcoMode_Disable == SET)		//@011
	{
		Ini_CoolingStep_21();						//@011
	}
//	else if (gu8_Key_Short.byte != 0)				//	220310 냉동담당자 요청으로 삭제
//	{
//		Ini_CoolingStep_21();
//	}
	else if (gu16_TimerColdEcoMode_14Hour == 0)
	{
		Ini_CoolingStep_23();
	}
	else if (gu8_WaterSelect == HOT_WATER)				//	온수 선택 시
	{
		Ini_CoolingStep_30();
	}
	else if (F_ColdOff == SET)
	{
		Ini_CoolingStep_10();
	}
	else if (gu8_ColdEffStep == Z_ColdEffStep_22)
	{
		Ini_CoolingStep_24();
	}
	else
	{

	}
}
static void Ini_CoolingStep_22(void)
{
	F_CoolingRapid_Complete = CLEAR;
	gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	gu16_TimerColdEcoMode_14Hour = Z_TimerColdEcoMode_14Hour;
	CoolingStepNoSet(Z_CoolingStepNum_22);
}

/// @brief    (에코모드) 방치 14시간~냉수 추출 사이
/// @param    void
/// @return   void
static void CoolingStep_23(void)
{
	U8 mu8_OPENSHORT_Error = 0;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);

	if ((mu8_OPENSHORT_Error == 0) && (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP23) && (gi16_OutDoorAirSensor.DEGREE < Z_TEMP29))
	{
		gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gi16_ColdSensor.DEGREE >= COLD_COMPLETE_DEGREE)
	{
		gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gu8_WaterSelect == HOT_WATER)				//	온수 선택 시
	{
		Ini_CoolingStep_30();
	}
	else if (gu8_TimerCoolingStep_100ms_2 == 0)		//	1도 과냉 판단
	{
		Ini_CoolingStep_40();
	}
	else if (gu8_TimerCoolingStep_100ms_3 == 0)
	{
		Ini_CoolingStep_21();
	}
	else if (F_CoolingEcoMode_Disable == SET)		//@011
	{
		Ini_CoolingStep_21();						//@011
	}
	else if (gu8_ColdEffStep == Z_ColdEffStep_22)
	{
		Ini_CoolingStep_24();
	}
	else if (F_ColdOff == SET)
	{
		Ini_CoolingStep_10();
	}
	else
	{

	}
}
static void Ini_CoolingStep_23(void)
{
	F_CoolingRapid_Complete = CLEAR;
	gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	CoolingStepNoSet(Z_CoolingStepNum_23);
}

/// @brief    (에코모드) 냉수 첫 추출 후 10시간 방치
/// @param    void
/// @return   void
static void CoolingStep_24(void)
{
	U8 mu8_OPENSHORT_Error = 0;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);

	if ((mu8_OPENSHORT_Error == 0) && (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP23) && (gi16_OutDoorAirSensor.DEGREE < Z_TEMP29))
	{
		gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gi16_ColdSensor.DEGREE >= COLD_COMPLETE_DEGREE)
	{
		gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	}

	if (gu8_WaterSelect == HOT_WATER)				//	온수 선택 시
	{
		Ini_CoolingStep_30();
	}
	else if (gu8_TimerCoolingStep_100ms_2 == 0)		//	1도 과냉 판단
	{
		Ini_CoolingStep_40();
	}
	else if (gu8_TimerCoolingStep_100ms_3 == 0)
	{
		Ini_CoolingStep_21();
	}
	else if (F_CoolingEcoMode_Disable == SET)		//@011
	{
		Ini_CoolingStep_21();						//@011
	}
	else if (gu16_TimerColdEcoMode_10Hour == 0)
	{
		Ini_CoolingStep_21();
	}
	else if (F_ColdOff == SET)
	{
		Ini_CoolingStep_10();
	}
	else
	{

	}
}
static void Ini_CoolingStep_24(void)
{
	F_CoolingRapid_Complete = CLEAR;
	gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_3 = Z_TimerCoolingStep_100ms_3s;
	gu16_TimerColdEcoMode_10Hour = Z_TimerColdEcoMode_10Hour;
	CoolingStepNoSet(Z_CoolingStepNum_24);
}

/// @brief    온수 선택 MIN
/// @param    void
/// @return   void
static void CoolingStep_30(void)
{
	if (gu8_WaterSelect != HOT_WATER)
	{
		CoolingStepNoSet(gu8_CoolingStepNum_Before);
	}
	else if (F_ColdOff == SET)
	{
		Ini_CoolingStep_10();
	}
	else
	{

	}
}
static void Ini_CoolingStep_30(void)
{
	CoolingStepNoSet(Z_CoolingStepNum_30);
}

/// @brief    냉각 종료
/// @param    void
/// @return   void
static void CoolingStep_40(void)
{
	U8 mu8_OPENSHORT_Error = 0;

	mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);

	if (gi16_ColdSensor.DEGREE < COLD_REOPERATE_DEGREE)
	{
		gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	}

	if (mu8_OPENSHORT_Error == 0)
	{
		if ((gi16_ColdSensor.DEGREE < Z_TEMP6) && (gi16_OutDoorAirSensor.DEGREE < Z_TEMP6))
		{
			gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
		}
	}
	else
	{
		gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
	}

	if (mu8_OPENSHORT_Error == 0)
	{
		if ((gu8_TimerCoolingStep_100ms_2 == 0) && (gu8_TimerCoolingStep_100ms_4 == 0))	//	냉각 재동작 체크
		{
			Ini_CoolingStep_21();
		}
	}
	else
	{
		if (gu8_TimerCoolingStep_100ms_2 == 0)
		{
			Ini_CoolingStep_21();
		}
	}
}
static void Ini_CoolingStep_40(void)
{
	gu8_TimerCoolingStep_100ms_2 = Z_TimerCoolingStep_100ms_3s;
	gu8_TimerCoolingStep_100ms_4 = Z_TimerCoolingStep_100ms_3s;
	CoolingStepNoSet(Z_CoolingStepNum_40);
}


/// @brief    냉각 10분 정지
/// @param    void
/// @return   void
static void CoolingStep_41(void)
{
	if (gu16_TimerCoolingStep_1s == 0)
	{
		gu16_TimerCoolingRapid_6Hour = Z_TimerCoolingRapid_6Hour;
		Ini_CoolingStep_10();
	}
}
static void Ini_CoolingStep_41(void)
{
	gu16_TimerCoolingStep_1s = Z_TimerCoolingStep_1s_600s;
	CoolingStepNoSet(Z_CoolingStepNum_41);
}

/// @brief
/// @param    void
/// @return   void
static void CoolingStep_E0(void)
{
	if ((F_Err25_HeatSinkTH == CLEAR) && (F_Err44_ColdTH == CLEAR) && (F_Err29_TEM_OverDrive1 == CLEAR) && (F_Err29_TEM_OverDrive2 == CLEAR))
	{
		Ini_CoolingStep_10();
	}
}
static void Ini_CoolingStep_E0(void)
{
	CoolingStepNoSet(Z_CoolingStepNum_E0);
}

/// @brief
/// @param    void
/// @return   void
static void CoolingStepNoSet(U8 data)
{
	static const U8 DT_CoolingNumber[] =
	{
		Z_CoolingStep_00,
		Z_CoolingStep_10,
		Z_CoolingStep_20,
		Z_CoolingStep_21,
		Z_CoolingStep_22,
		Z_CoolingStep_23,
		Z_CoolingStep_24,
		Z_CoolingStep_30,
		Z_CoolingStep_40,
		Z_CoolingStep_41,
		Z_CoolingStep_E0,
	};
	/************************************************/
	gu8_CoolingStepNum_Before = gu8_CoolingStepNum;
	gu8_CoolingStepNum = data; 							/*	일련 번호 set*/

	gu8_CoolingStep_Before = gu8_CoolingStep;
	gu8_CoolingStep = DT_CoolingNumber[gu8_CoolingStepNum];	/*	상태 mode No. set*/
}

/// @brief	  TEM PI 연산	@022
/// @param    void
/// @return   void
static void Cooling_PID_Calculration(void)
{
	gi16_TEM_Volt_P = (U16)TEM_Volt_Calculation_P();			//	P값 계산
	gi16_TEM_Volt_I = (U16)TEM_Volt_Calculation_I();			//	I값 계산

	/*	P+I 값 설정	*/
	gi16_TEM_Volt_P_I = gi16_TEM_Volt_P + gi16_TEM_Volt_I;//	P+I

	if (gi16_TEM_Volt_P_I >= Z_RetainMode_TEM_Volt_Max)		//	결정값이 최대출력값 보다 높을 경우
	{
		gi16_TEM_Volt_P_I = Z_RetainMode_TEM_Volt_Max;		//		최대값 제한
	}
	else if (gi16_TEM_Volt_P_I < Z_RetainMode_TEM_Volt_Min)		//	이상 계산일 경우
	{
		gi16_TEM_Volt_P_I = Z_RetainMode_TEM_Volt_Min;			//		최소값 제한
	}
	else
	{

	}

	if (gi16_TEM_Volt_P_I < Z_RetainMode_TEM_Volt_Min)	//	PI 값이 마이너스인 경우
	{
		gu16_TEM_Volt = Z_RetainMode_TEM_Volt_Min;	//		0으로 고정
	}
	else											//	양수인 경우
	{
		gu16_TEM_Volt = (U16)gi16_TEM_Volt_P_I;		//		PI값으로 갱신
	}
}

/// @brief    냉각 P값 연산
/// @param    void
/// @return   void
static I32 TEM_Volt_Calculation_P(void)
{
	I16	mi16_diff;									//	Tset'－Tout buffer
	I32	mi32_DP;									//	DP buffer
	I32	mi32_max, mi32_min;
	I32	mi32_max_work;								//	max buf

	/*	P 값 설정	*/
	mi16_diff = gi16_ColdSensor.DEGREE - (I16)gu16_Cooling_TargetDegree;		//		Tset - Tout				[1/10:℃]
	if (mi16_diff >= Z_Cooling_P_Control_LimitTemp)
	{												//		차이가 ?℃ 이상 차이 있는 경우
		mi16_diff = Z_Cooling_P_Control_LimitTemp;			//			max의 제한
	}
	else if (mi16_diff <= -Z_Cooling_P_Control_LimitTemp)
	{												//		차이가 ?℃ 이하인 경우
		mi16_diff = -Z_Cooling_P_Control_LimitTemp;		//			min의 제한
	}
	else
	{
		/* ... */
	}

	mi32_DP = ((I32)gu16_Cooling_Dp1 * (I32)mi16_diff) / 10;	//		DP＝dp1×(DTset'－DTout)	[1/10V]

	if (mi32_DP < 0)
	{
		mi32_DP *= 2;
	}

	/************************************************/
	mi32_max_work = (I32)Z_RetainMode_TEM_Volt_Max;		//
	mi32_max = (mi32_max_work * gu16_Cooling_Dp2) / 10;	//	MAX치 = max × Dp2 	[1/10V]
//	mi32_min = 0;									//	MIN치 = 0V	[1/10V]
	mi32_min = -mi32_max;							//	MIN치 = -MAX	[1/10V]

	if (mi32_DP < mi32_min)
	{												//	최소치보다 작은 경우
		mi32_DP = mi32_min;							//		DP의 MIN 제한
	}
	else if (mi32_DP > mi32_max)
	{												//	최대치보다 큰 경우
		mi32_DP = mi32_max;							//		DP의 MAX 제한
	}
	else
	{
		/* ... */
	}

	return (mi32_DP);								//	제한후의 DP의 설정
}

/// @brief    냉각 I값 연산		@022
/// @param    void
/// @return   void
static I32 TEM_Volt_Calculation_I(void)
{
	I16	mi16_diff;									//	Tset' - Tout buffer
	I32	mi32_DIn;									//	DI buffer
	I32	mi32_DIn2;									//	DI buffer
	I32	mi32_max, mi32_min;
	I32	mi32_DIDP;									//	DI+DP buffer

//#ifdef _FOR_DEVELOP_
//	if (PSM_gu16_Cooling_Di1 != 0) gu16_Cooling_Di1 = PSM_gu16_Cooling_Di1;
//#endif

	/*	I 값 설정	*/
	mi16_diff = gi16_ColdSensor.DEGREE - (I16)gu16_Cooling_TargetDegree;		//		Tset - Tout				[1/10:℃]
	if (mi16_diff >= Z_Cooling_I_Control_LimitTemp)
	{												//		차이가 ?℃ 이상 차이 있는 경우
		mi16_diff = Z_Cooling_I_Control_LimitTemp;			//			max의 제한
	}
	else if (mi16_diff <= -Z_Cooling_I_Control_LimitTemp)
	{												//		차이가 ?℃ 이하인 경우
		mi16_diff = -Z_Cooling_I_Control_LimitTemp;		//			min의 제한
	}
	else
	{
		/* ... */
	}

	mi32_DIn2 = (I32)(((I32)gu16_Cooling_Di1 * (I32)mi16_diff) / 10000);	//	I값 누적
	mi32_DIn = gi16_TEM_Volt_I + mi32_DIn2;	//	I값 누적

	if ((mi32_DIn+gi16_TEM_Volt_P) > Z_RetainMode_TEM_Volt_Max)			//	I값 최대치 제한
	{
		mi32_DIn = Z_RetainMode_TEM_Volt_Max-gi16_TEM_Volt_P;
	}

	mi32_max = Z_RetainMode_TEM_Volt_Max;				//	MAX치 = [1/10V]
	mi32_min = Z_RetainMode_TEM_Volt_Min;				//	MIN치 = [1/10V]

//	mi32_DIDP = mi32_DIn + gi16_TEM_Volt_P;			//	DI＋DP
	mi32_DIDP = mi32_DIn;			//	DI＋DP

	if (mi32_DIDP <= mi32_min)						//	최소값보다 작은 경우
	{
		mi32_DIn = mi32_min;		//		DI의 MIN 제한(DI)
	}
	else if (mi32_DIDP >= mi32_max)					//	최대값보다 큰 경우
	{
		mi32_DIn = mi32_max;
	}
	else
	{

	}

	return (mi32_DIn);									//	DI의 설정							[1/1000:Kcal/min]
}
