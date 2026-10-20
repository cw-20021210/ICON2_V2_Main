/// @file     buzzer.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    부저  동작 제어 file
#define PBA_C
#include "include/_include.h"

static void Ini_PBAStep_00(void);
static void Ini_PBAStep_10(void);
static void Ini_PBAStep_20(void);

static void PBAStep_00(void);
static void PBAStep_10(void);
static void PBAStep_20(void);

static void PBAStepNoSet(U8 data);

PBA_StepTable PBA_Step[Z_PBAStepNum_Max] =
{
	//	스텝 No			 	Step Func.
	{ Z_PBAStep_00, 		PBAStep_00 },     		//	부팅
	{ Z_PBAStep_10, 		PBAStep_10 },    		//	기능검사모드
	{ Z_PBAStep_20, 		PBAStep_20 },     		//	부하검사모드
};

/// @brief    초기화
/// @param    void
/// @return   void
void PBA_Initialize(void)
{
	Ini_PBAStep_00();
}

/// @brief    PBA TestMode 제어
/// @param    void
/// @return   void
void PBAControl(void)
{
	if (gu8_PBAStepNum < Z_PBAStepNum_Max)			//	스텝 허용 범위 이내
	{
		if ((gu8_PBAStep != Z_PBAStep_00) && (F_TestMode_PBA == CLEAR))	//		검사모드가 아닌 경우
		{
			Ini_PBAStep_00();
		}

		(PBA_Step[gu8_PBAStepNum].exe)();			//		해당 스텝 함수 실행
	}
	else											//	스텝범위 외
	{
		SafetyLoop();								//		안전정지 루프
	}
}

/// @brief    대기
/// @param    void
/// @return   void
static void PBAStep_00(void)
{
	if (F_TestMode_PBA == SET)						//	검사모드 진입 인 경우
	{
		Ini_PBAStep_10();
	}
}
static void Ini_PBAStep_00(void)
{
	PBAStepNoSet(Z_PBAStepNum_00);
}

/// @brief    기능검사모드
/// @param    void
/// @return   void
static void PBAStep_10(void)
{
//	if (((gu8_PBATest_FanTest == 1) || (gu8_PBATest_FanTest == 2))
//	if ((gu8_PBATest_FanTest == 2)
	if ((gu8_PBATest_FanTest == 1)
//	&& ((gu8_TimerPBATest_FanDisplay != 0) && (gu8_TimerPBATest_FanDisplay <= (Z_TimerPBATest_FanDisplay/2))))
	&& (gu8_TimerPBATest_FanDisplay <= (Z_TimerPBATest_FanDisplay/2)))
	{
		if (ADC_RESULT_FAN_FB < 10)
		{
			F_PBATest_FanFeedBack_Error = SET;
		}
		else
		{
			F_PBATest_FanFeedBack_Error = CLEAR;
		}
	}
	else if (F_PBATest_Fan == CLEAR)
	{
		if (ADC_RESULT_FAN_FB < 10)
		{
//			F_PBATest_FanFeedBack_Error = CLEAR;
			gu8_TimerPBATest_FanFB = Z_TimerPBATest_FanFB;
		}
		else
		{
			if (gu8_TimerPBATest_FanFB == 0)
			{
				F_PBATest_FanFeedBack_Error = SET;
			}
		}
	}
	else
	{

	}

	if (KEY_AMOUNT_SHORT == SET)
	{
		F_PBATest_ColdSensorOFF ^= 1;
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_HOTWATER_SHORT == SET)
	{
		F_PBATest_LeakSensorOFF ^= 1;
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_PUREWATER_SHORT == SET)
	{
		F_PBATest_VoiceReset ^= 1;
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_COLDWATER_SHORT == SET)
	{
		F_PBATest_Fan = SET;
#ifndef _FAN_CIRCUIT_CHANGE_
		if (++gu8_PBATest_FanTest > 3)
#else
		if (++gu8_PBATest_FanTest > 2)
#endif
		{
			gu8_PBATest_FanTest = 0;
		}
		if (gu8_PBATest_FanTest != 0)
		{
			gu8_TimerPBATest_FanDisplay = Z_TimerPBATest_FanDisplay;
		}
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_COLDTANK_PBATEST_LONG == SET)
	{
		Play_Voice(VOICE_SPECIAL_FUNCTION);
		Ini_PBAStep_20();
	}
}
static void Ini_PBAStep_10(void)
{
	F_TestMode_PBA_2 = CLEAR;					//	부하테스트 모드 해제

	F_PBATest_ColdSensorOFF = 0;
	F_PBATest_LeakSensorOFF = 0;
	F_PBATest_VoiceReset = 0;
	gu8_PBATest_FanTest = 0;

	gu8_TimerPBATest_FanDisplay = 0;

	F_PBATest_SteppingMotor = 0;
	F_PBATest_StandBy_OFF = 0;

	gu8_PBATest_HotLoad = 0;
	gu8_PBATest_ColdLoad = 0;
	gu8_PBATest_PureLoad = 0;
	F_PBATest_StandByOFF = 0;

	gu8_TimerPBATest_HeaterRelay = 0;
	gu8_TimerPBATest_HeaterTriac = 0;

	gu8_TimerPBATest_TEM_ON = 0;
	gu8_TimerPBATest_PureVV_ON = 0;

	F_HeaterFeedBack_Error = 0;
	F_TEMFeedBack_Error = 0;
	F_24VFeedBack_Error = 0;

	gu8_TimerPBATest_FanDisplay = 0;

	gu8_TimerPBATest_FanFB = Z_TimerPBATest_FanFB;

	gu8_TimerFCTSet = Z_TimerFCTSet;			//		테스트모드 전환 표시(tE1, tE2) 타이머 set

	PBAStepNoSet(Z_PBAStepNum_10);
}

/// @brief    부하검사모드
/// @param    void
/// @return   void
static void PBAStep_20(void)
{
	if (((gu8_TimerPBATest_HeaterRelay != 0) && (gu8_TimerPBATest_HeaterRelay <= (Z_TimerPBATest_HeaterRelay/2)))
		|| ((gu8_TimerPBATest_HeaterTriac != 0) && (gu8_TimerPBATest_HeaterTriac <= (Z_TimerPBATest_HeaterTriac/2))))
	{
		if ((ADC_RESULT_HEATER_FB <= 50) || (ADC_RESULT_HEATER_FB >= 950))
		{
			F_HeaterFeedBack_Error = SET;
		}
		else
		{
			F_HeaterFeedBack_Error = CLEAR;
		}
	}
	else if (F_PBATest_Heater == CLEAR)
	{
		if ((ADC_RESULT_HEATER_FB <= 50) || (ADC_RESULT_HEATER_FB >= 950))
		{
//			F_HeaterFeedBack_Error = CLEAR;
			gu8_TimerPBATest_HeaterFB = Z_TimerPBATest_HeaterFB;
		}
		else
		{
			if (gu8_TimerPBATest_HeaterFB == 0)
			{
				F_HeaterFeedBack_Error = SET;
			}
		}
	}
	else
	{

	}

	if (gu8_PBATest_ColdLoad == 3)
	{
		if (gu8_TimerPBATest_TEM_ON == 0)
		{
			if ((ADC_RESULT_TEM_FB <= 20) || (ADC_RESULT_TEM_FB >= 950))
			{
				F_TEMFeedBack_Error = SET;
			}
			else
			{
				F_TEMFeedBack_Error = CLEAR;
			}
		}
	}
	else
	{
		if (F_PBATest_TEM == CLEAR)
		{
			if ((ADC_RESULT_TEM_FB <= 20) || (ADC_RESULT_TEM_FB >= 950))
			{
//				F_TEMFeedBack_Error = CLEAR;
				gu8_TimerPBATest_TemFB = Z_TimerPBATest_TemFB;
			}
			else
			{
				if (gu8_TimerPBATest_TemFB == 0)
				{
					F_TEMFeedBack_Error = SET;
				}
			}
		}
	}

	if ((gu8_PBATest_PureLoad == 2) && (gu8_TimerPBATest_PureVV_ON == 0))
	{
		if ((ADC_RESULT_24V_FB <= 50) || (ADC_RESULT_24V_FB >= 950))
		{
			F_24VFeedBack_Error = SET;
		}
		else
		{
			F_24VFeedBack_Error = CLEAR;
		}
	}
	else if ((gu8_PBATest_HotLoad == 0) && (gu8_PBATest_ColdLoad == 0) && (gu8_PBATest_PureLoad == 0))
	{
		if (F_PBATest_24V == CLEAR)
		{
			if ((ADC_RESULT_24V_FB <= 50) || (ADC_RESULT_24V_FB >= 950))
			{
	//			F_24VFeedBack_Error = CLEAR;
			}
			else
			{
				F_24VFeedBack_Error = SET;
			}
		}
	}
	else
	{

	}

	if (KEY_HOTWATER_SHORT == SET)
	{
		F_PBATest_Heater = SET;
		if (++gu8_PBATest_HotLoad > 4)
		{
			gu8_PBATest_HotLoad = 0;
		}
		if (gu8_PBATest_HotLoad != 0)
		{
			gu8_TimerPBATest_HotLoadDisplay = Z_TimerPBATest_HotLoadDisplay;
		}
		if (gu8_PBATest_HotLoad == 3)
		{
			gu8_TimerPBATest_HeaterRelay = Z_TimerPBATest_HeaterRelay;
		}
		if (gu8_PBATest_HotLoad == 4)
		{
			gu8_TimerPBATest_HeaterTriac = Z_TimerPBATest_HeaterTriac;
		}
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_COLDWATER_SHORT == SET)
	{
		F_PBATest_TEM = SET;
		if (++gu8_PBATest_ColdLoad > 4)
		{
			gu8_PBATest_ColdLoad = 0;
		}
		if (gu8_PBATest_ColdLoad != 0)
		{
			gu8_TimerPBATest_ColdLoadDisplay = Z_TimerPBATest_ColdLoadDisplay;
		}
		if (gu8_PBATest_ColdLoad == 3)
		{
			gu8_TimerPBATest_TEM_ON = Z_TimerPBATest_TEM_ON;
		}
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_PUREWATER_SHORT == SET)
	{
		if (++gu8_PBATest_PureLoad > 2)
		{
			gu8_PBATest_PureLoad = 0;
		}
		if (gu8_PBATest_PureLoad != 0)
		{
			gu8_TimerPBATest_PureLoadDisplay = Z_TimerPBATest_PureLoadDisplay;
		}
		if (gu8_PBATest_PureLoad == 2)
		{
			F_PBATest_24V = SET;
			gu8_TimerPBATest_PureVV_ON = Z_TimerPBATest_PureVV_ON;
		}
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_EFFLUENT_SHORT == SET)
	{
		F_PBATest_SteppingMotor = SET;
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_AMOUNT_SHORT == SET)
	{
		gu8_PBATest_ColdLoad = 0;
		F_PBATest_StandBy_OFF ^= 1;
		Play_Voice(VOICE_BEEP_SELECT);
	}
	else if (KEY_COLDTANK_PBATEST_LONG == SET)
	{
		Play_Voice(VOICE_SPECIAL_FUNCTION);
		Ini_PBAStep_10();
	}
	else
	{

	}
}
static void Ini_PBAStep_20(void)
{
	F_PBATest_ColdSensorOFF = 0;
	F_PBATest_LeakSensorOFF = 0;
	F_PBATest_VoiceReset = 0;
	gu8_PBATest_FanTest = 0;

	gu8_TimerPBATest_FanDisplay = 0;

	F_PBATest_SteppingMotor = 0;
	F_PBATest_StandBy_OFF = 0;

	gu8_PBATest_HotLoad = 0;
	gu8_PBATest_ColdLoad = 0;
	gu8_PBATest_PureLoad = 0;
	F_PBATest_StandByOFF = 0;

	gu8_TimerPBATest_HeaterRelay = 0;
	gu8_TimerPBATest_HeaterTriac = 0;

	gu8_TimerPBATest_TEM_ON = 0;
	gu8_TimerPBATest_PureVV_ON = 0;

	F_HeaterFeedBack_Error = 0;
	F_TEMFeedBack_Error = 0;
	F_24VFeedBack_Error = 0;

	F_TestMode_PBA_2 = SET;						//	부하 테스트모드 설정
	gu8_TimerFCTSet = Z_TimerFCTSet;			//		테스트모드 전환 표시(tE1, tE2) 타이머 set

	gu8_TimerPBATest_HeaterFB = Z_TimerPBATest_HeaterFB;
	gu8_TimerPBATest_TemFB = Z_TimerPBATest_TemFB;

	PBAStepNoSet(Z_PBAStepNum_20);
}

/// @brief    검사모드 스텝 설정
/// @param    void
/// @return   void
static void PBAStepNoSet(U8 data)
{
	static const U8 DT_PBANumber[]
	= {
		Z_PBAStep_00,
		Z_PBAStep_10,
		Z_PBAStep_20,
	};
	/************************************************/
	gu8_PBAStepNum = data; 							/*	일련 번호 set*/

	gu8_PBAStep = DT_PBANumber[gu8_PBAStepNum]; 	/*	상태 mode No. set*/
}
