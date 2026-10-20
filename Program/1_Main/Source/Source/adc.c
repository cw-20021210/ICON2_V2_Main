/// @file     adc.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    각 센서류  동작 제어 file
#define ADC_C
#include "include/_include.h"

void ADC_Channel_Increase(void);

#define ADC_Resolution 	(50000/1024)				//	5V * 10000배 / 1024bit
#define Z_HeaterWATT_Revise			(30)			//	히터 WATT 감산 보정

/*	ADC 채널	*/
#define Z_ADC_ANI0_24V_FB	   (0)
#define Z_ADC_ANI1_TEM_FB	   (1)
#define Z_ADC_ANI2_OUTDOORAIRTH	   (2)
#define Z_ADC_ANI3_HOTINTH	   (3)
#define Z_ADC_ANI4_HOTOUTTH	   (4)
#define Z_ADC_ANI5_HEATSINKTH	   (5)
#define Z_ADC_ANI6_COLDTH	   (6)
#define Z_ADC_ANI7_HEATERTH	   (7)
#define Z_ADC_ANI8_FAN_FB	   (8)

//20260101
#define Z_ADC_ANI9_TEMVAD	   (9)

#define Z_ADC_ANI10_LEAKSENSOR	   (10)
#define Z_ADC_ANI19_HEATER_FB	   (19)
#define Z_ADC_MCU_TEMPSENSOR       (0x80)

ADC_Data ADC_Data_Table[ZC_Adchannel] =
{//	ADC 채널				ADC 평균		  	ADC 현재값		ADC 합계			ADC 카운터			ADC 카운터 기준 횟수
	{Z_ADC_ANI0_24V_FB,		&gu16_AdcAverage[0],	&gu16_NowAdc[0],	&gu16_SumAdc[0],	&gu8_CounterAdc[0],		8},
	{Z_ADC_ANI1_TEM_FB,		&gu16_AdcAverage[1],	&gu16_NowAdc[1],	&gu16_SumAdc[1],	&gu8_CounterAdc[1],		8},
	{Z_ADC_ANI2_OUTDOORAIRTH,	&gu16_AdcAverage[2],	&gu16_NowAdc[2],	&gu16_SumAdc[2],	&gu8_CounterAdc[2],		32},
	{Z_ADC_ANI3_HOTINTH,		&gu16_AdcAverage[3],	&gu16_NowAdc[3],	&gu16_SumAdc[3],	&gu8_CounterAdc[3],		8},
	{Z_ADC_ANI4_HOTOUTTH,		&gu16_AdcAverage[4],	&gu16_NowAdc[4],	&gu16_SumAdc[4],	&gu8_CounterAdc[4],		8},
	{Z_ADC_ANI5_HEATSINKTH,	  	&gu16_AdcAverage[5],	&gu16_NowAdc[5],	&gu16_SumAdc[5],	&gu8_CounterAdc[5],		8},
	{Z_ADC_ANI6_COLDTH,		&gu16_AdcAverage[6],	&gu16_NowAdc[6],	&gu16_SumAdc[6],	&gu8_CounterAdc[6],		8},
	{Z_ADC_ANI7_HEATERTH,		&gu16_AdcAverage[7],	&gu16_NowAdc[7],	&gu16_SumAdc[7],	&gu8_CounterAdc[7],		4},
	{Z_ADC_ANI8_FAN_FB,		&gu16_AdcAverage[8],	&gu16_NowAdc[8],	&gu16_SumAdc[8],	&gu8_CounterAdc[8],		8},

	//20260101
	//{Z_ADC_ANI10_LEAKSENSOR,	&gu16_AdcAverage[9],	&gu16_NowAdc[9],	&gu16_SumAdc[9],	&gu8_CounterAdc[9],		32},
	//{Z_ADC_ANI19_HEATER_FB,	&gu16_AdcAverage[10],	&gu16_NowAdc[10],	&gu16_SumAdc[10],	&gu8_CounterAdc[10],		8},
	//{Z_ADC_MCU_TEMPSENSOR,	&gu16_AdcAverage[11],	&gu16_NowAdc[11],	&gu16_SumAdc[11],	&gu8_CounterAdc[11],		8},
	//{Z_ADC_MCU_TEMPSENSOR,	&gu16_AdcAverage[12],	&gu16_NowAdc[12],	&gu16_SumAdc[12],	&gu8_CounterAdc[12],		8},
	
	{Z_ADC_ANI9_TEMVAD,		&gu16_AdcAverage[9],	&gu16_NowAdc[9],	&gu16_SumAdc[9],	&gu8_CounterAdc[9],		8},
	{Z_ADC_ANI10_LEAKSENSOR,	&gu16_AdcAverage[10],	&gu16_NowAdc[10],	&gu16_SumAdc[10],	&gu8_CounterAdc[10],		32},
	{Z_ADC_ANI19_HEATER_FB,	    	&gu16_AdcAverage[11],	&gu16_NowAdc[11],	&gu16_SumAdc[11],	&gu8_CounterAdc[11],		8},
	{Z_ADC_MCU_TEMPSENSOR,		&gu16_AdcAverage[12],	&gu16_NowAdc[12],	&gu16_SumAdc[12],	&gu8_CounterAdc[12],		8},
	{Z_ADC_MCU_TEMPSENSOR,		&gu16_AdcAverage[13],	&gu16_NowAdc[13],	&gu16_SumAdc[13],	&gu8_CounterAdc[13],		8},
};

/// @brief    AD Convert Initialize
/// @param    void
/// @return   void
void ADC_Initialize(void)
{
	gu8_TimerAdcheck = ZT_Adcheck;					//	AD check timer set
	gu16_OrdinaryCheck_Result_HeaterRelay_ADC = 0xFFFFU;	//	상시 진단 히터 AD값 Clear
	gu16_OrdinaryCheck_Result_HeaterTriac_ADC = 0xFFFFU;	//	상시 진단 히터 AD값 Clear
}

/// @brief   ADC 처리(1ms)
/// @param    void
/// @return   void
void Input_AD_Change(void)
{
	U8 mu8_Check = 0;

	/*------------------------------------------------*/
	if (ADIF == 0)									//	adc interrupt 요구 없는 경우
	{
		NOP();										//		nop
		NOP();										//		nop
		NOP();										//		nop
		if (gu8_TimerAdcheck <= 0)					//		AD check timer up인 경우
		{
			SafetyLoop();							//			Reset
		}
		return;
	}
	gu8_TimerAdcheck = ZT_Adcheck;					//	AD check timer set

	ADIF = 0;										// 	AD 변환 인터럽트 요청 플래그 비트 클리어
	ADCS = 0;										// 	AD 변환 동작 정지
	ADCE = 0;										// 	AD 동작 정지

	if (ADC_Data_Table[gu8_CounterAdchannel].channel == Z_ADC_ANI6_COLDTH)
	{
		if (P_COLDSENSOR_ON == HIGH)				//	센서 ON
		{
			if (gu8_TimerColdSensor_Sensing == 0)
			{
				mu8_Check = SET;
			}
		}
		else
		{
			if (F_TestMode_PBA == SET)
			{
				mu8_Check = SET;
			}
			else
			{
				gu8_CounterAdc[Z_ADC_ANI6_COLDTH] = 0;
				gu16_SumAdc[Z_ADC_ANI6_COLDTH] = 0;
				gu16_NowAdc[Z_ADC_ANI6_COLDTH] = 0;
			}
		}
	}
	else
	{
		mu8_Check = SET;
	}

	if (mu8_Check == SET)							//	ADC읽기 허용 일 때
	{
		*ADC_Data_Table[gu8_CounterAdchannel].now = (U16)(ADCR >> 6);	//		현재값 저장
		*ADC_Data_Table[gu8_CounterAdchannel].sum += *ADC_Data_Table[gu8_CounterAdchannel].now;	//		Sum 저장
		(*ADC_Data_Table[gu8_CounterAdchannel].counter)++;	//		카운터 증가

		if (*ADC_Data_Table[gu8_CounterAdchannel].counter >= ADC_Data_Table[gu8_CounterAdchannel].average_counter)	//		읽기 횟수 카운터 완료 일 때
		{
			*ADC_Data_Table[gu8_CounterAdchannel].average = *ADC_Data_Table[gu8_CounterAdchannel].sum / *ADC_Data_Table[gu8_CounterAdchannel].counter;	//		평균값 계산
			*ADC_Data_Table[gu8_CounterAdchannel].sum = 0;	//	Sum 초기화
			*ADC_Data_Table[gu8_CounterAdchannel].counter = 0;	//	 카운터 초기화
		}
	}

	ADC_Channel_Increase();							//	ADC 채널 증가

	if (ADC_Data_Table[gu8_CounterAdchannel].channel == Z_ADC_ANI6_COLDTH)
	{
		if ((F_TestMode_PBA == CLEAR) && (P_COLDSENSOR_ON == LOW))				//	센서 OFF이면
		{
			gu8_CounterAdc[Z_ADC_ANI6_COLDTH] = 0;
			gu16_SumAdc[Z_ADC_ANI6_COLDTH] = 0;
			gu16_NowAdc[Z_ADC_ANI6_COLDTH] = 0;
			ADC_Channel_Increase();
		}
		else
		{
			if (gu8_TimerColdSensor_Sensing != 0)
			{
				ADC_Channel_Increase();
			}
		}
	}
	ADCE = 1;										//	AD Enable
	ADS = ADC_Data_Table[gu8_CounterAdchannel].channel;			//	AD channel change
	NOP();											// 	지연
	NOP();											// 	지연
	ADCS = 1;										// 	AD 변환 동작 시작
	ADIF = 0;										// 	AD 변환 인터럽트 요청 플래그 비트 클리어(만일을 위해)

	if (F_TestMode_LineFront == SET)				//	공장 검사 중
	{
		if (ADC_RESULT_FAN_FB > gu16_FAN_FB_forLineTest)
		{
			gu16_FAN_FB_forLineTest = ADC_RESULT_FAN_FB;
		}

//		if ((gu8_TimerLineTest_Heater != 0) && (gu8_TimerLineTest_Heater > ((Z_TimerLineTest_Heater/2)+50)))
//		{
//			if (ADC_RESULT_HEATER_FB > gu16_Heater_FB_Relay_forLineTest)
//			{
//				gu16_Heater_FB_Relay_forLineTest = ADC_RESULT_HEATER_FB;
//			}
//		}
//
//		if ((gu8_TimerLineTest_Heater != 0) && (gu8_TimerLineTest_Heater <= ((Z_TimerLineTest_Heater/2)-70)))	//	70인 이유는 릴레이의 잔전류가 있기때문에
//		{
//			if (ADC_RESULT_HEATER_FB > gu16_Heater_FB_Triac_forLineTest)
//			{
//				gu16_Heater_FB_Triac_forLineTest = ADC_RESULT_HEATER_FB;
//			}
//		}
	}
}

/// @brief    온도데이터 변환 (Main Loop)
/// @param    void
/// @return   void
void Temperature_Change(void)
{
	I16 temp = 0;
	U8 mu8_OPENSHORT_Error1 = 0;
	U8 mu8_OPENSHORT_Error2 = 0;
#ifdef _FOR_DEVELOP_
	if (gi16_ColdSensor_Test != 0) gi16_ColdSensor.DEGREE = gi16_ColdSensor_Test;
	else
	{
		if (*gi16_ColdSensor.TH_ADC_AVERAGE < 171)
		{
			gi16_ColdSensor.DEGREE = ColdTemp_Table[0];
		}
		else if (*gi16_ColdSensor.TH_ADC_AVERAGE > 811)
		{
			gi16_ColdSensor.DEGREE = ColdTemp_Table[Z_ColdTemp_Table_Max - 1];
		}
		else
		{
			gi16_ColdSensor.DEGREE = ColdTemp_Table[*gi16_ColdSensor.TH_ADC_AVERAGE - 171];
		}
	}

	if (gi16_HotOutSensor_Test != 0) gi16_HotOutSensor.DEGREE = gi16_HotOutSensor_Test;
	else
	{
		if (*gi16_HotOutSensor.TH_ADC_AVERAGE < 201)
		{
			gi16_HotOutSensor.DEGREE = HotOutTemp_Table[0];
		}
		else if (*gi16_HotOutSensor.TH_ADC_AVERAGE > 1004)
		{
			gi16_HotOutSensor.DEGREE = HotOutTemp_Table[Z_HotOutTemp_Table_Max - 1];
		}
		else
		{
			gi16_HotOutSensor.DEGREE = HotOutTemp_Table[*gi16_HotOutSensor.TH_ADC_AVERAGE - 201];
		}
	}

	if (gi16_HotInSensor_Test != 0) gi16_HotInSensor.DEGREE = gi16_HotInSensor_Test;
	else
	{
		if (*gi16_HotInSensor.TH_ADC_AVERAGE < 45)
		{
			gi16_HotInSensor.DEGREE = HotInTemp_Table[0];
		}
		else if (*gi16_HotInSensor.TH_ADC_AVERAGE > 906)
		{
			gi16_HotInSensor.DEGREE = HotInTemp_Table[Z_HotInTemp_Table_Max - 1];
		}
		else
		{
			gi16_HotInSensor.DEGREE = HotInTemp_Table[*gi16_HotInSensor.TH_ADC_AVERAGE - 45];
		}
	}

	if (gi16_HeaterSensor_Test != 0) gi16_HeaterSensor.DEGREE = gi16_HeaterSensor_Test;
	else
	{
		if (*gi16_HeaterSensor.TH_ADC_AVERAGE < 201)
		{
			gi16_HeaterSensor.DEGREE = HeaterTemp_Table[0];
		}
		else if (*gi16_HeaterSensor.TH_ADC_AVERAGE > 1004)
		{
			gi16_HeaterSensor.DEGREE = HeaterTemp_Table[Z_HeaterTemp_Table_Max - 1];
		}
		else
		{
			gi16_HeaterSensor.DEGREE = HeaterTemp_Table[*gi16_HeaterSensor.TH_ADC_AVERAGE - 201];
		}
	}

	if (gi16_HeatSinkSensor_Test != 0) gi16_HeatSinkSensor.DEGREE = gi16_HeatSinkSensor_Test;
	else
	{
//		if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE < 51)
		if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE < 75)
		{
			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[0];
		}
//		else if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE > 947)
		else if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE > 954)
		{
			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[Z_HeatSinkTemp_Table_Max - 1];
		}
		else
		{
//			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[*gi16_HeatSinkSensor.TH_ADC_AVERAGE - 51];
			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[*gi16_HeatSinkSensor.TH_ADC_AVERAGE - 75];
		}
	}

	if (gi16_OutDoorAirSensor_Test != 0) gi16_OutDoorAirSensor.DEGREE = gi16_OutDoorAirSensor_Test;
	else
	{
		if (*gi16_OutDoorAirSensor.TH_ADC_AVERAGE < 113)
		{
			gi16_OutDoorAirSensor.DEGREE = OutDoorAirTemp_Table[0];
		}
		else if (*gi16_OutDoorAirSensor.TH_ADC_AVERAGE > 929)
		{
			gi16_OutDoorAirSensor.DEGREE = OutDoorAirTemp_Table[Z_OutDoorAirTemp_Table_Max - 1];
		}
		else
		{
			gi16_OutDoorAirSensor.DEGREE = OutDoorAirTemp_Table[*gi16_OutDoorAirSensor.TH_ADC_AVERAGE - 113];
		}
	}

	if (gi16_MCUSensor_Test != 0) gi16_MCUSensor.DEGREE = gi16_MCUSensor_Test;
	else
	{
		if (*gi16_MCUSensor.TH_ADC_AVERAGE < 68)
		{
			gi16_MCUSensor.DEGREE = MCUTemp_Table[0];
		}
		else if (*gi16_MCUSensor.TH_ADC_AVERAGE > 474)
		{
			gi16_MCUSensor.DEGREE = MCUTemp_Table[Z_MCUTemp_Table_Max - 1];
		}
		else
		{
			gi16_MCUSensor.DEGREE = MCUTemp_Table[*gi16_MCUSensor.TH_ADC_AVERAGE - 68];
		}
	}
#endif

#ifndef _FOR_DEVELOP_
	if (*gi16_ColdSensor.TH_ADC_AVERAGE < 171)
	{
		gi16_ColdSensor.DEGREE = ColdTemp_Table[0];
	}
	else if (*gi16_ColdSensor.TH_ADC_AVERAGE > 811)
	{
		gi16_ColdSensor.DEGREE = ColdTemp_Table[Z_ColdTemp_Table_Max-1];
	}
	else
	{
		gi16_ColdSensor.DEGREE = ColdTemp_Table[*gi16_ColdSensor.TH_ADC_AVERAGE - 171];
	}

	if (*gi16_HotOutSensor.TH_ADC_AVERAGE < 201)
	{
		gi16_HotOutSensor.DEGREE = HotOutTemp_Table[0];
	}
	else if (*gi16_HotOutSensor.TH_ADC_AVERAGE > 1004)
	{
		gi16_HotOutSensor.DEGREE = HotOutTemp_Table[Z_HotOutTemp_Table_Max - 1];
	}
	else
	{
		gi16_HotOutSensor.DEGREE = HotOutTemp_Table[*gi16_HotOutSensor.TH_ADC_AVERAGE - 201];
	}

	if (*gi16_HotInSensor.TH_ADC_AVERAGE < 45)
	{
		gi16_HotInSensor.DEGREE = HotInTemp_Table[0];
	}
	else if (*gi16_HotInSensor.TH_ADC_AVERAGE > 906)
	{
		gi16_HotInSensor.DEGREE = HotInTemp_Table[Z_HotInTemp_Table_Max - 1];
	}
	else
	{
		gi16_HotInSensor.DEGREE = HotInTemp_Table[*gi16_HotInSensor.TH_ADC_AVERAGE - 45];
	}

	if (*gi16_HeaterSensor.TH_ADC_AVERAGE < 201)
	{
		gi16_HeaterSensor.DEGREE = HeaterTemp_Table[0];
	}
	else if (*gi16_HeaterSensor.TH_ADC_AVERAGE > 1004)
	{
		gi16_HeaterSensor.DEGREE = HeaterTemp_Table[Z_HeaterTemp_Table_Max - 1];
	}
	else
	{
		gi16_HeaterSensor.DEGREE = HeaterTemp_Table[*gi16_HeaterSensor.TH_ADC_AVERAGE - 201];
	}

//		if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE < 51)
		if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE < 75)
		{
			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[0];
		}
//		else if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE > 947)
		else if (*gi16_HeatSinkSensor.TH_ADC_AVERAGE > 954)
		{
			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[Z_HeatSinkTemp_Table_Max - 1];
		}
		else
		{
//			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[*gi16_HeatSinkSensor.TH_ADC_AVERAGE - 51];
			gi16_HeatSinkSensor.DEGREE = HeatSinkTemp_Table[*gi16_HeatSinkSensor.TH_ADC_AVERAGE - 75];
		}

	if (*gi16_OutDoorAirSensor.TH_ADC_AVERAGE < 113)
	{
		gi16_OutDoorAirSensor.DEGREE = OutDoorAirTemp_Table[0];
	}
	else if (*gi16_OutDoorAirSensor.TH_ADC_AVERAGE > 929)
	{
		gi16_OutDoorAirSensor.DEGREE = OutDoorAirTemp_Table[Z_HeaterTemp_Table_Max-1];
	}
	else
	{
		gi16_OutDoorAirSensor.DEGREE = OutDoorAirTemp_Table[*gi16_OutDoorAirSensor.TH_ADC_AVERAGE - 113];
	}

	if (*gi16_MCUSensor.TH_ADC_AVERAGE < 68)
	{
		gi16_MCUSensor.DEGREE = MCUTemp_Table[0];
	}
	else if (*gi16_MCUSensor.TH_ADC_AVERAGE > 474)
	{
		gi16_MCUSensor.DEGREE = MCUTemp_Table[Z_MCUTemp_Table_Max-1];
	}
	else
	{
		gi16_MCUSensor.DEGREE = MCUTemp_Table[*gi16_MCUSensor.TH_ADC_AVERAGE - 68];
	}
#endif
}

/// @brief    히터 최대출력 전력치 저장
/// @param    void
/// @return   void
void HeaterFullCurrent_Decide(void)
{
	U32 mu32_buf1, mu32_buf2;
#ifdef _FOR_HEATER_ENDURANCE_TEST_
	if ((P_HEATER_RELAY == ON) && ((P_HEATER_TRIAC == ON) || (P_HEATER_PWM_TDR > _DABF_TAU_TDR02_VALUE))	// 히터 두개가 다 켜진 경우에만 체크
		&& ((gu8_HotStep == Z_HotStep_02) || (gu8_HotStep == Z_HotStep_33) || (gu8_HotStep == Z_HotStep_35) /*|| ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerHeaterControl_AfterEff == 0))*/))
#else
	if ((P_HEATER_RELAY == ON) && ((P_HEATER_TRIAC == ON) || (P_HEATER_PWM_TDR > _DABF_TAU_TDR02_VALUE))	// 히터 두개가 다 켜진 경우에만 체크
//		&& ((gu8_HotStep == Z_HotStep_02) || (gu8_HotStep == Z_HotStep_33) /*|| ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerHeaterControl_AfterEff == 0))*/))
		&& ((gu8_TimerLineTest_Heater != 0) || (gu8_HotStep == Z_HotStep_02) || (gu8_HotStep == Z_HotStep_33) /*|| ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerHeaterControl_AfterEff == 0))*/))
#endif
	{
		if ((gu8_HotStep == Z_HotStep_33) && (gu8_TimerHeaterCheck != 0))   //	(온수 예열) & (히터 체크 대기 타이머 미경과시)
		{
			return;
		}
		else if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			if ((ADC_RESULT_HEATER_FB >= 80) && (ADC_RESULT_HEATER_FB <= 700))
			{
//				mu32_buf1 = (U32)ADC_RESULT_HEATER_FB * 830;
//				mu32_buf2 = mu32_buf1 - 132600;		//

				mu32_buf2 = HeaterWattCalculation();                               //	실제 히터 최대 전력 측정 (실측치)

				mu32_buf2 /= 100;
				if (mu32_buf2 >= Z_HeaterWATT_Revise)                                   //	히터 최대 전력 실측치가 히터 감산 보정값(30W) 이상인 경우
				{
					gu16_HeaterFull_WATT = (U16)(mu32_buf2 - Z_HeaterWATT_Revise); //	실제 히터 최대 전력 측정값 감산 보정
					if (F_TestMode_LineFront == SET)
					{
						gu16_Heater_FB_forLineTest = gu16_HeaterFull_WATT;
					}
					if (gu16_HeaterFull_WATT < 1500)	//	(히터 커넥터가 1개 빠져서 낮게 입력되는 경우 너무 낮아 유량이 적은 경우가 있어서 Limit)
					{
						gu16_HeaterFull_WATT = 1500;
					}
					gu16_HeaterRELAY_WATT = (U16)((U32)gu16_HeaterFull_WATT * HEATER_RELAY_Percent_KOR / 100);
				}
				else
				{
					gu16_HeaterFull_WATT = HEATER_FULL_WATT_KOR;
					gu16_HeaterRELAY_WATT = HEATER_RELAY_WATT_KOR;
				}
			}
			else
			{
				gu16_HeaterFull_WATT = HEATER_FULL_WATT_KOR;
				gu16_HeaterRELAY_WATT = HEATER_RELAY_WATT_KOR;
			}
			//-------------------------------------------20260201--------------------------
			//cbr_test 2025-09-15
			//	저전압, 정격, 고전압 구별
			//190V 2208Watt 424
			//200V 2573Watt 452
			//210V 2739Watt 484
			//220V 22963Watt 512
			//230V 3196Watt 542
			//240V 3362Watt 560
			//if(gu16_HeaterFull_WATT <= 2400)
			if(ADC_RESULT_HEATER_FB <= 490)
			{
				gu8_Heater_PowerLevel = Z_HEATER_POWER_LEVEL_LOW;//0
			}
			//else if(gu16_HeaterFull_WATT <= 2800)
			else if(ADC_RESULT_HEATER_FB <= 550)
			{
				gu8_Heater_PowerLevel = Z_HEATER_POWER_LEVEL_RATED;//1
			}
			else
			{
				gu8_Heater_PowerLevel = Z_HEATER_POWER_LEVEL_HIGH;//2
			}
			//-------------------------------------------20260201--------------------------
		}
		else										//	일본향
		{
			if ((ADC_RESULT_HEATER_FB >= 80) && (ADC_RESULT_HEATER_FB <= 900))
			{
				mu32_buf1 = (U32)ADC_RESULT_HEATER_FB * 331;
				if (mu32_buf1 >= 75000)
				{
					mu32_buf2 = mu32_buf1 - 75000;
				}
				else
				{
					mu32_buf2 = 0;
				}

				mu32_buf2 /= 100;
				if (mu32_buf2 >= Z_HeaterWATT_Revise)
				{
					gu16_HeaterFull_WATT = (U16)(mu32_buf2 - Z_HeaterWATT_Revise);
					if (F_TestMode_LineFront == SET)
					{
						gu16_Heater_FB_forLineTest = gu16_HeaterFull_WATT;
					}
					if (gu16_HeaterFull_WATT < 700)		//	(히터 커넥터가 1개 빠져서 낮게 입력되는 경우 너무 낮아 유량이 적은 경우가 있어서 Limit)
					{
						gu16_HeaterFull_WATT = 700;
					}
					gu16_HeaterRELAY_WATT = (U16)((U32)gu16_HeaterFull_WATT * HEATER_RELAY_Percent_JP / 100);
				}
				else
				{
					gu16_HeaterFull_WATT = HEATER_FULL_WATT_JP;
					gu16_HeaterRELAY_WATT = HEATER_RELAY_WATT_JP;
				}
			}
			else
			{
				gu16_HeaterFull_WATT = HEATER_FULL_WATT_JP;
				gu16_HeaterRELAY_WATT = HEATER_RELAY_WATT_JP;
			}
		}
	}

#ifdef _FOR_DEVELOP_
	if (gu16_HeaterFull_WATT_Test != 0)
	{
		gu16_HeaterFull_WATT = gu16_HeaterFull_WATT_Test;
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			gu16_HeaterRELAY_WATT = (U16)((U32)gu16_HeaterFull_WATT * HEATER_RELAY_Percent_KOR / 100);
		}
		else
		{
			gu16_HeaterRELAY_WATT = (U16)((U32)gu16_HeaterFull_WATT * HEATER_RELAY_Percent_JP / 100);
		}
	}
#endif
}

/// @brief    수위 확정
/// @param    void
/// @return   void
void WaterLevel_Decide(void)
{

}

/// @brief    ADC 채널 증가
/// @param    void
/// @return   void
void ADC_Channel_Increase(void)
{
	if (++gu8_CounterAdchannel >= ZC_Adchannel)
	{
		gu8_CounterAdchannel = 0;
	}
}
U32 AC_Watt_test;
/// @brief    전류 계산
/// @param    void
/// @return   void
void Get_WattCheck(void)
{
	U32 DC_voltage = 0;							//	[1/100V]
	U32 DC_mA = 0;								//	[1/1mA]
	U32 DC_Watt = 0;							//	[1/100W]
	U32 AC_voltage = 0;							//	[1/100V]
	U32 AC_mA = 0;								//	[1/1mA]
	U32 AC_Watt = 0;							//	[1/100W]
	U32 TEM_voltage = 0;						//	[1/100V]
	U32 TEM_mA = 0;								//	[1/1mA]
	U32 TEM_Watt = 0;							//	[1/100W]
	U32 mu32_buf1, mu32_buf2 = 0;

	/*	DC mA 계산	*/
	if (ADC_RESULT_24V_FB != 0)					//	전류 값이 있는 경우
	{
		DC_voltage = ADC_Resolution * ADC_RESULT_24V_FB / 100;	//	[1/100V]
//		DC_mA = (DC_voltage * 100 / 2048);
//		DC_mA = (DC_voltage * 100 / 2467);
		DC_mA = DC_voltage * 4;						//	[1/100V] * [1/10] = [1/1000A]
		DC_Watt = DC_mA * 24 / 10;					//	24V 곱하기 [1/100W]
	}

	/*	TEM mA 계산	*/
	if (ADC_RESULT_TEM_FB >= 10)					//	전류 값이 있는 경우
	{
		TEM_voltage = ADC_Resolution * ADC_RESULT_TEM_FB / 100;
#ifndef _SUNRINTECH_SMPS_
//		TEM_mA = (TEM_voltage * 100 / 409);
//		TEM_mA = (TEM_voltage * 156 + 4291) / 10;		//	(([1/100V] * [1/100]) + [1/10000] / 10)= [1/1000A]
		TEM_mA = (TEM_voltage * 116 + 4291) / 10;		//	(([1/100V] * [1/100]) + [1/10000] / 10)= [1/1000A]
#else
		TEM_mA = TEM_voltage / 10;
#endif
		TEM_Watt = TEM_mA * 24 / 10;				//	24V 곱하기 [1/100W]
	}

	if (gu8_ModelType_forCHP == MODEL_CHP)
	{
		if (ADC_RESULT_HEATER_FB != 0)
		{
	//		mu32_buf1 = (U32)ADC_RESULT_HEATER_FB * 830;
	//		mu32_buf2 = mu32_buf1 - 132600;

			mu32_buf2 = HeaterWattCalculation();
			AC_Watt_test = mu32_buf2;
//			AC_Watt = mu32_buf2 / 100;	//	[1/1W]
			AC_Watt = mu32_buf2;		//	[1/100W]
//			AC_voltage = 22000;			//	220V
			AC_voltage = 220;			//	220V
//			AC_mA = AC_Watt / AC_voltage / 10;	//
			AC_mA = AC_Watt / AC_voltage * 10;	//
		}
	}
	else
	{
		AC_Watt = 0;
		AC_voltage = 0;
		AC_mA = 0;
	}

	gu32_AC_mA_Result = (U32)AC_mA;
	gu16_DC_mA_Result = (U16)DC_mA;
	gu16_TEM_mA_Result = (U16)TEM_mA;
	
	gu32_Total_mA_Result = (U32)(DC_mA + AC_mA + TEM_mA);		//	[1/1mA]
	gu32_Watt_Result = (U32)(DC_Watt + AC_Watt + TEM_Watt);		//	[1/100W]
}

/// @brief    추출 온도 평균
/// @param    void
/// @return   void
void Effluent_Degree_Average(void)
{
	if (F_100ms_for_Degree_Average == SET)
	{
		F_100ms_for_Degree_Average = CLEAR;
		/*	온수 출수	*/
		if ((gu8_HotStep == Z_HotStep_35) && (gu16_FlowDecide_W != 0))	//	온수 출수 중이고 유량이 있는 경우
		{
			gu32_HotOut_Degree_Total += gi16_HotOutSensor.DEGREE;	//		온수 출수온도 누적
			gu16_HotOut_Degree_Cnt++;					//		카운트 누적

			if ((gu32_HotOut_Degree_Total != 0) && (gu16_HotOut_Degree_Cnt != 0))	//		데이터가 있는 경우
			{
				gu16_HotOut_Degree_Ave = (U16)(gu32_HotOut_Degree_Total / gu16_HotOut_Degree_Cnt);	//			평균 산출
			}
		}
		else											//	상기 외
		{

		}

		/*	냉수 출수	*/
		if ((gu8_OperationStep == Z_OperationStep_50) && (gu8_WaterSelect == COLD_WATER) && (gu16_FlowDecide_W != 0))	//	냉수 출수 중이고 유량이 있는 경우
		{
			gu32_Cold_Degree_Total += gi16_ColdSensor.DEGREE;	//		냉수온도 누적
			gu16_Cold_Degree_Cnt++;						//		카운트 누적
		}
		else											//	상기 외
		{
			if ((gu32_Cold_Degree_Total != 0) && (gu16_Cold_Degree_Cnt != 0))	//		데이터가 있는 경우
			{
				gu16_Cold_Degree_Ave = (U16)(gu32_Cold_Degree_Total / gu16_Cold_Degree_Cnt);	//			평균 산출
			}
			gu32_Cold_Degree_Total = 0;					//		데이터 clr
			gu16_Cold_Degree_Cnt = 0;					//		데이터 clr
		}
	}
}

/// @brief    히터 전력 계산
/// @param    void
/// @return   void
U32 HeaterWattCalculation(void)
{
	U32 mu32_buf1, mu32_buf2;

	mu32_buf1 = (U32)ADC_RESULT_HEATER_FB * 830;
	if (mu32_buf1 >= 125600)
	{
		//mu32_buf2 = mu32_buf1 - 125600;		//132600
		//mu32_buf2 = mu32_buf1 - 130000;		//132600
		
		
		mu32_buf2 = mu32_buf1 - 132600;
		
		
		//mu32_buf2 = mu32_buf1 - 132000;
		//20260310
		//mu32_buf2 = mu32_buf1 - 140000;
		
	}
	else
	{
		mu32_buf2 = 0;
	}

	return mu32_buf2;
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/*
U16 calculate_uv_current( U16 mu16_ad )
{
    U16 mu16_value = 0;

    //mu16_value = (U16)(((U32)mu16_ad * (U32)5000) / (U32)20480);
    mu16_value = (U16)((((U32)mu16_ad * (U32)5000) / (U32)1024) / 4);

    return mu16_value;
}
*/
/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 calculate_tem_current( U16 mu16_ad )
{
	U16 mu16_value = 0;

	mu16_value = (U16)((((U32)mu16_ad * (U32)50000) / (U32)1024) / (U32)8);
	
	return mu16_value;	
}

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/*
U16 calculate_fan_current( U16 mu16_ad )
{
	U16 mu16_value = 0;

	mu16_value = (U16)(((((U32)mu16_ad * 5000) / 1024) / 4));

	return mu16_value;	
}
*/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
/*
U16 calculate_dc_current( U16 mu16_ad )
{
	U16 mu16_value = 0;
	U16 i = 0;

	//	LJH : 2025-12-11 10:00:00 전류값 계산 다시.
	if (ADC_RESULT_24V_FB != 0)					//	전류 값이 있는 경우
	{
		i = ADC_RESULT_24V_FB - 10;		//	ad값이 좀 떠잇어서 보정...
		mu16_value = (U16)(((((U32)i * 5000) / 1024) / 40) * 10);

		return mu16_value;	
	}
}
*/

/***********************************************************************************************************************
* Function Name: System_ini
* Description  :
***********************************************************************************************************************/
U16 calculate_smps_voltage_feedback( U16 mu16_ad )
{
	U16 mu16_value = 0;

	mu16_value = (U16)((((U32)mu16_ad * 5000) / 1024)); 

	return mu16_value;	
}

/***********************************************************************************************************************
* Function Name: check_smps_voltage_range
* Description  : SMPS 전압 MIN/MAX 범위 확인 (±10%)
* @param       : mu16_value - 확인할 값
*                mu16_min   - MIN 값
*                mu16_max   - MAX 값
* @return      : 0 - 정상 범위 내, 1 - 범위 초과 (ERROR)
***********************************************************************************************************************/
U8 check_smps_voltage_range( U16 mu16_min, U16 mu16_max )
{
	U16 mu16_min_limit = 0;
	U16 mu16_max_limit = 0;
	U16 mu16_VolValue = 0;
        /*
	if(gu8_F_SmartCheck_Tem_Check == SET)
	{
		mu16_VolValue = gu16_SmartCheck_Result_SMPS_Voltage_Feedback;
		gu16_SmartCheck_Result_SMPS_Voltage_Feedback = 0xFFFFU;
	}
	else if(gu8_f_ad_feedback_mode == SET)
	*/
	if(gu8_f_ad_feedback_mode == SET)
	{
		mu16_VolValue = gu16_ad_feedback_mode_smps_voltage_Result;
	}
	else
	{
		mu16_VolValue = gu16_SMPS_Voltage_Feedback_Result;
	}

	// MIN - 10% 계산
	#if 0
	mu16_min_limit = (U16)((mu16_min * 90) / 100);
	
	// MAX + 10% 계산
	mu16_max_limit = (U16)((mu16_max * 110) / 100);
	#endif

	// 범위 확인: MIN-10% ~ MAX+10% 내에 있으면 0, 아니면 1
	#if 0
	if ((mu16_VolValue >= mu16_min_limit) && (mu16_VolValue <= mu16_max_limit))
	#endif
	if(mu16_VolValue >= mu16_min)
	{
		return FEEDBACK_OK;  // 정상 범위
	}
	else
	{
		return FEEDBACK_ERROR;  // ERROR
	}
}

/***********************************************************************************************************************
* Function Name: check_tem_current_range
* Description  : TEM 전류 MIN/MAX 범위 확인 (±10%)
* @param       : mu16_value - 확인할 값
*                mu16_min   - MIN 값
*                mu16_max   - MAX 값
* @return      : 0 - 정상 범위 내, 1 - 범위 초과 (ERROR)
***********************************************************************************************************************/
U8 check_tem_current_range( U16 mu16_min )
{
	U16 mu16_min_limit = 0;
	U16 mu16_max_limit = 0;
	U16 mu16_TemValue = 0;
        /*
	if(gu8_F_SmartCheck_Tem_Check == SET)
	{
		mu16_TemValue = gu16_SmartCheck_Result_TEM_mA;
		gu16_SmartCheck_Result_TEM_mA = 0xFFFFU;
	}
	else if(gu8_f_ad_feedback_mode == SET)
	*/
	if(gu8_f_ad_feedback_mode == SET)
	{
		mu16_TemValue = gu16_ad_feedback_mode_tem_mA_Result;
	}
	else
	{
		mu16_TemValue = gu16_TEM_mA_Result;
	}
	
	#if 0
	// MIN - 10% 계산
	mu16_min_limit = (U16)((mu16_min * 90) / 100);
	
	// MAX + 10% 계산
	mu16_max_limit = (U16)((mu16_max * 110) / 100);
	#endif
	
	// 범위 확인: MIN-10% ~ MAX+10% 내에 있으면 0, 아니면 1
	#if 0
	if ((mu16_TemValue >= mu16_min_limit) && (mu16_TemValue <= mu16_max_limit))
	#endif
	if(mu16_TemValue >= mu16_min)
	{
		return FEEDBACK_OK;  // 정상 범위
	}
	else
	{
		return FEEDBACK_ERROR;  // ERROR
	}
}

/***********************************************************************************************************************
* Function Name: check_tem_current_range
* Description  : TEM 전류 MIN/MAX 범위 확인 (±10%)
* @param       : mu16_value - 확인할 값
*                mu16_min   - MIN 값
*                mu16_max   - MAX 값
* @return      : 0 - 정상 범위 내, 1 - 범위 초과 (ERROR)
***********************************************************************************************************************/
void ad_feedback_mode_ad_sensing(void)
{
	if(gu8_f_ad_feedback_mode == SET)
	{
		if(gu8_Timer_Forced_TemOutput != 0)
		{
			gu16_ad_feedback_mode_tem_mA_Result = calculate_tem_current(ADC_RESULT_TEM_FB);
			gu16_ad_feedback_mode_smps_voltage_Result = calculate_smps_voltage_feedback(ADC_RESULT_VOLTAGE_FB);
		}
	}
	else 
	{
		gu16_ad_feedback_mode_tem_mA_Result = 0;
	}
}
