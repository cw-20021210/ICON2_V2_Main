/// @file     timer.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    타이머 인터럽트 처리 file
#define TIMER_C
#include "include/_include.h"


/// @brief    Base Interval Timer(1ms Timer Interrupt)
/// @param    void
/// @return   void
void Timer_Initialize(void)
{
	gu8_Counter_1msTimerCheck = 0xFF; 				/*	1ms timer interrupt check counter clr */
	gu16_PartTest_Period = 1;						//
	gu16_TimerPartTest = gu16_PartTest_Period * 60;
}

/// @brief    Base Interval Timer(1ms Timer Interrupt)
/// @param    void
/// @return   void
void Timer_1msBase_Timer(void)
{
	static U8 mu8_2ms = 0;

	EI();											//	다중인터럽트 허용
	Input_AD_Change();								//	ADC 처리
	if (gu8_ModelType_forCHP == MODEL_CHP)		//	온수모델만 Step Motor 제어
	{
		StepMotorMove();                   			// 		유량 조절 제어
	}

	Time1ms(); 										/*	1ms TIMER 감산			*/

	if (++mu8_2ms > 1)
	{
		mu8_2ms = 0;
		ProcessVoice();
	}

	gu8_Counter10ms++; 								/*	10ms COUNTER +1			*/
	if (gu8_Counter10ms >= 10)						/*	10ms 경과한 경우		*/
	{
		gu8_Counter10ms = 0; 						/*		10ms COUTNER CLR	*/
		Time10ms(); 								/*		10ms TIMER 감산		*/
		gu8_Counter100ms++; 						/*		100ms COUNTER +1	*/
		if (gu8_Counter100ms >= 10)					/*		100ms 경과한 경우	*/
		{
			gu8_Counter100ms = 0; 					/*			100ms COUNTER CLR*/
			gu8_FlagTime100ms = 1; 					/*			100ms 경과 FLAG SET*/
		}
	}
	gu8_Counter_1msTimerCheck = 0xFF; 				/*	1ms timer interrupt check counter clr */

	gu8_Counter_MainLoopCheck--;

	if (gu8_Counter_MainLoopCheck == 0)				//	메인 체크 카운터 reset되지 않는 경우
	{
		SafetyLoop();
	}
}

/// @brief    Timer 처리
/// @param    void
/// @return   void
void Timer_Process(void)
{
	static U8 mu8_Counter1s; 						/*	1s COUNTER 				*/
	static U8 mu8_Counter10s; 						/*	10s COUNTER				*/
	static U8 mu8_Counter1min; 						/*	1min COUNTER			*/
	static U8 mu8_Counter10min; 					/*	10min COUNTER			*/
	static U8 mu8_Counter1h; 						/*	1h COUNTER				*/
	/********************************************/
	if (gu8_FlagTime100ms == 1)						/*	100ms 경과 FLAG SET인 경우	*/
	{
		gu8_FlagTime100ms = 0; 						/*		100ms 경과 FLAG CLR		*/
		Time100ms(); 								/*		100ms TIMER 감산		*/

		mu8_Counter1s++; 							/*		1s COUNTER +1			*/
		if (mu8_Counter1s >= 10)					/*		1s 경과한 경우			*/
		{
			mu8_Counter1s = 0; 						/*			1s COUNTER CLR		*/
			gu8_FlagTime1000ms = 1; 				/*			1s 경과 FLAG SET	*/
			Time1s(); 								/*			1s TIMER 감산		*/

			mu8_Counter10s++; 						/*			10s COUNTER +1		*/
			if (mu8_Counter10s >= 10)
			{ 										/*			10s 경과한 경우		*/
				mu8_Counter10s = 0; 				/*				10s COUNTER CLR	*/
				Time10s(); 							/*				10s TIMER 감산	*/

				mu8_Counter1min++; 					/*				1min COUNTER +1	*/
				if (mu8_Counter1min >= 6)			/*				1min 경과한 경우			*/
				{
					mu8_Counter1min = 0; 			/*					1min COUNTER CLR		*/
					Time1min(); 					/*					1min TIMER 감산			*/

					mu8_Counter10min++; 			/*					10min COUNTER +1		*/
					if (mu8_Counter10min >= 10)		/*					10min 경과한 경우		*/
					{
						mu8_Counter10min = 0; 		/*						10min COUNTER CLR	*/
						Time10min(); 				/*						10min TIMER 감산	*/

						mu8_Counter1h++; 			/*						1h COUNTER +1		*/
						if (mu8_Counter1h >= 6)		/*						1h 경과한 경우		*/
						{
							mu8_Counter1h = 0; 		/*							1h COUNTER CLR	*/
							Time1h(); 				/*							1h TIMER 감산	*/
						}
					}
				}
			}
		}
	}
}

/// @brief    1ms 처리
/// @param    void
/// @return   void
void Time1ms(void)
{
//	static U8 mu8_Timer11ms;
//	/********************************************/
//	if (mu8_Timer11ms == 0)							/*	DIGITAL 입력 TIMER UP인 경우	*/
//	{
//		mu8_Timer11ms = ZT_11ms; 					/*		DIGITAL 입력 TIMER SET		*/
//	}
//	else											/*	상기 이외						*/
//	{
//		mu8_Timer11ms--; 							/*		TIMER 감산					*/
//	}
	/*------------------------------------------*/
	TDEC_1ms_ADC();
	TDEC_1ms_BUZZER();
	TDEC_1ms_COLD();
	TDEC_1ms_DISPLAY();
	TDEC_1ms_PURE();
	TDEC_1ms_EEPROM();
	TDEC_1ms_ERROR();
	TDEC_1ms_HOT();
	TDEC_1ms_INPUT();
	TDEC_1ms_KEYCONTROL();
	TDEC_1ms_MACRO();
	TDEC_1ms_MAIN();
	TDEC_1ms_OPERATION();
	TDEC_1ms_OUTPUT();
	TDEC_1ms_SAFETY();
	TDEC_1ms_STEPMOTOR();
	TDEC_1ms_TIMER();
	TDEC_1ms_FLOWMETER();
	TDEC_1ms_PBA();
	TDEC_1ms_COMMUNICATION();
	TDEC_1ms_LINEAUTOTEST();
	TDEC_1ms_SERIAL();
	TDEC_1ms_COMMAND();
	TDEC_1ms_SMARTCHECK();
}

/// @brief    10ms 처리
/// @param    void
/// @return   void
void Time10ms(void)								//
{												//
	/********************************************/
	TDEC_10ms_ADC();
	TDEC_10ms_BUZZER();
	TDEC_10ms_COLD();
	TDEC_10ms_DISPLAY();
	TDEC_10ms_PURE();
	TDEC_10ms_EEPROM();
	TDEC_10ms_ERROR();
	TDEC_10ms_HOT();
	TDEC_10ms_INPUT();
	TDEC_10ms_KEYCONTROL();
	TDEC_10ms_MACRO();
	TDEC_10ms_MAIN();
	TDEC_10ms_OUTPUT();
	TDEC_10ms_SAFETY();
	TDEC_10ms_STEPMOTOR();
	TDEC_10ms_TIMER();
	TDEC_10ms_FLOWMETER();
	TDEC_10ms_OPERATION();
	TDEC_10ms_PBA();
	TDEC_10ms_COMMUNICATION();
	TDEC_10ms_LINEAUTOTEST();
	TDEC_10ms_COMMAND();
	TDEC_10ms_SERIAL();
	TDEC_10ms_SMARTCHECK();

	F_10ms_for_InputDigital = SET;

#ifdef _Flowmeter_Fake
	if(gu8_HotStep != Z_HotStep_33){
		TDEC(gu16_Effluent_Hz);
	}
	TDEC(gu16_Flushing_Hz_forDisplay);
#endif

}												//

/// @brief    100ms 처리
/// @param    void
/// @return   void
void Time100ms(void)							//
{												//
	/********************************************/
	U8 i = 0;
	static U8 count_100ms = 0;

	TDEC_100ms_ADC();
	TDEC_100ms_BUZZER();
	TDEC_100ms_COLD();
	TDEC_100ms_DISPLAY();
	TDEC_100ms_PURE();
	TDEC_100ms_EEPROM();
	TDEC_100ms_ERROR();
	TDEC_100ms_HOT();
	TDEC_100ms_INPUT();
	TDEC_100ms_KEYCONTROL();
	TDEC_100ms_MACRO();
	TDEC_100ms_MAIN();
	TDEC_100ms_OUTPUT();
	TDEC_100ms_SAFETY();
	TDEC_100ms_STEPMOTOR();
	TDEC_100ms_TIMER();
	TDEC_100ms_FLOWMETER();
	TDEC_100ms_OPERATION();
	TDEC_100ms_PBA();
	TDEC_100ms_COMMUNICATION();
	TDEC_100ms_LINEAUTOTEST();
	TDEC_100ms_COMMAND();
	TDEC_100ms_SERIAL();
	TDEC_100ms_SMARTCHECK();

	F_100ms_for_FlowControl = SET;
	F_100ms_for_HeaterControl = SET;
	F_100ms_for_FlowMeter_Average = SET;
	F_100ms_for_Effluent_Degree_Ave = SET;
	F_100ms_for_Degree_Average = SET;
	F_100ms_for_HeaterDegreeSave = SET;
	if (++count_100ms >= 2)							//	200ms
	{
		count_100ms = 0;
//		F_200ms_for_HeaterControl = SET;
	}
//20260310
//#ifdef _FOR_DEVELOP_
#if 1
	if (gu8_HotStep == Z_HotStep_33)				//	가열 중인 경우
	{
		gu16_HeatingTime++;							//		가열 시간 가산
	}

	if (gu8_HotStep == Z_HotStep_35)				//	온수 추출 중인 경우
	{
		gu16_HotEffluentTime++;						//		추출 시간 가산 (유량 체크 용)
	}
#endif

	WifiControlProcess(WIFI_TIME_100MS);
}

/// @brief    1sec 처리
/// @param    void
/// @return   void
void Time1s(void)
{
//	static U8 mu8_3sCount = 0;
	U8 mu8_WifiDisp = 0;
	U32 Watt_Data1 = 0;
	U32 Watt_Data2 = 0;

	/********************************************/
	TDEC_1s_ADC();
	TDEC_1s_BUZZER();
	TDEC_1s_COLD();
	TDEC_1s_DISPLAY();
	TDEC_1s_PURE();
	TDEC_1s_EEPROM();
	TDEC_1s_ERROR();
	TDEC_1s_HOT();
	TDEC_1s_INPUT();
	TDEC_1s_KEYCONTROL();
	TDEC_1s_MACRO();
	TDEC_1s_MAIN();
	TDEC_1s_OUTPUT();
	TDEC_1s_SAFETY();
	TDEC_1s_STEPMOTOR();
	TDEC_1s_TIMER();
	TDEC_1s_FLOWMETER();
	TDEC_1s_OPERATION();
	TDEC_1s_PBA();
	TDEC_1s_COMMUNICATION();
	TDEC_1s_LINEAUTOTEST();
	TDEC_1s_SERIAL();
	TDEC_1s_COMMAND();
	TDEC_1s_SMARTCHECK();

#ifdef _FOR_DEVELOP_
	PSM_TimerCheck_1s++;
#endif

//	F_1s_for_TEM_OnTimeCheck = SET;

//	if (++mu8_3sCount >= 3)							//	3초 체크
//	{
//		mu8_3sCount = 0;
//		F_3s_for_FlowControl = SET;
//	}

	/*	전력 갱신	*/
	mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_DISP);	//	Wi-Fi 연결상태 확인

	if (mu8_WifiDisp == WIFI_DISP_CONNECT)			//	연결된 경우에만
	{
		/* 1option 실제 계산	*/
		gu32_Watt_per_Min_Result += gu32_Watt_Result / 3600;	//	Watt to Watt per h [W→WH]
		/***********/

		/* 강제 고정	*/
	//	if (gu8_HeaterRelay_CMD == HEATER_ON)			//
	//	{                                               //
	//		Watt_Data1 = 250000;						//		2500W [1/100W]
	//	}                                               //
	//	if (gu8_TEM_PWM_CMD != TEM_PWM_OFF)             //
	//	{                                               //
	//		Watt_Data2 = 10000;							//		100W [1/100W]
	//	}                                               //
	//	gu32_Watt_per_Min_Result = gu32_Watt_per_Min_Result + ((Watt_Data1+Watt_Data2) / 3600);     //
		/***********/
	}

	/*	하트서비스 시간	*/
	if ((gu8_OperationStep == Z_OperationStep_B0) || (gu8_OperationStep == Z_OperationStep_F0))
	{
		if (gu16_HeartServiceTime < 65535)
		{
			gu16_HeartServiceTime++;
		}
	}
	/****************/

	WifiControlProcess(WIFI_TIME_1SEC);

//	if ((F_Cooling_PID == SET) && (gu8_ColdEffStep == Z_ColdEffStep_22) && (gu16_FlowDecide_W != 0))	//@022	미적용★★
//	{                                                                       //@022
//		gu16_CountColdEff_Sec++;                                            //@022
//	}                                                                       //@022
}													//

/// @brief    10sec 처리
/// @param    void
/// @return   void
void Time10s(void)								//
{												//
	/********************************************/
	TDEC_10s_ADC();
	TDEC_10s_BUZZER();
	TDEC_10s_COLD();
	TDEC_10s_DISPLAY();
	TDEC_10s_PURE();
	TDEC_10s_EEPROM();
	TDEC_10s_ERROR();
	TDEC_10s_HOT();
	TDEC_10s_INPUT();
	TDEC_10s_KEYCONTROL();
	TDEC_10s_MACRO();
	TDEC_10s_MAIN();
	TDEC_10s_OUTPUT();
	TDEC_10s_SAFETY();
	TDEC_10s_STEPMOTOR();
	TDEC_10s_TIMER();
	TDEC_10s_FLOWMETER();
	TDEC_10s_OPERATION();
	TDEC_10s_PBA();
	TDEC_10s_COMMUNICATION();
	TDEC_10s_LINEAUTOTEST();
	TDEC_10s_SERIAL();
	TDEC_10s_COMMAND();
	TDEC_10s_SMARTCHECK();
}												//

/// @brief    1min 처리
/// @param    void
/// @return   void
void Time1min(void)								//
{												//
	/********************************************/
	TDEC_1min_ADC();
	TDEC_1min_BUZZER();
	TDEC_1min_COLD();
	TDEC_1min_DISPLAY();
	TDEC_1min_PURE();
	TDEC_1min_EEPROM();
	TDEC_1min_ERROR();
	TDEC_1min_HOT();
	TDEC_1min_INPUT();
	TDEC_1min_KEYCONTROL();
	TDEC_1min_MACRO();
	TDEC_1min_MAIN();
	TDEC_1min_OUTPUT();
	TDEC_1min_SAFETY();
	TDEC_1min_STEPMOTOR();
	TDEC_1min_TIMER();
	TDEC_1min_FLOWMETER();
	TDEC_1min_OPERATION();
	TDEC_1min_PBA();
	TDEC_1min_COMMUNICATION();
	TDEC_1min_LINEAUTOTEST();
	TDEC_1min_SERIAL();
	TDEC_1min_COMMAND();
	TDEC_1min_SMARTCHECK();

	if (gu16_PartTest_Period != 0)					//	송신 허용 중인 경우 (0인 경우 금지임)
	{
		if (gu16_TimerPartTest == 0)				//		상시진단 주기 경과 시
		{
			gu16_TimerPartTest = gu16_PartTest_Period * 60;
			WifiSendData(WIFI_DATA_PART);
		}
	}
	else
	{
		gu16_TimerPartTest = 60;			//		1시간 마다	주기 데이터 요청
	}

	WifiControlProcess(WIFI_TIME_1MIN);
}

/// @brief    10min 처리
/// @param    void
/// @return   void
void Time10min(void)							//
{												//
	/********************************************/
	TDEC_10min_ADC();
	TDEC_10min_BUZZER();
	TDEC_10min_COLD();
	TDEC_10min_DISPLAY();
	TDEC_10min_PURE();
	TDEC_10min_EEPROM();
	TDEC_10min_ERROR();
	TDEC_10min_HOT();
	TDEC_10min_INPUT();
	TDEC_10min_KEYCONTROL();
	TDEC_10min_MACRO();
	TDEC_10min_MAIN();
	TDEC_10min_OUTPUT();
	TDEC_10min_SAFETY();
	TDEC_10min_STEPMOTOR();
	TDEC_10min_TIMER();
	TDEC_10min_FLOWMETER();
	TDEC_10min_OPERATION();
	TDEC_10min_PBA();
	TDEC_10min_COMMUNICATION();
	TDEC_10min_LINEAUTOTEST();
	TDEC_10min_SERIAL();
	TDEC_10min_COMMAND();
	TDEC_10min_SMARTCHECK();
}

/// @brief    1h 처리
/// @param    void
/// @return   void
void Time1h(void)								//
{												//
	static Day_Count = 0;

	U8 mu8_WifiDisp = 0;
	U8 data = 0;
	U16 buf1 = 0;
	U16 buf2 = 0;
	/********************************************/
	TDEC_1h_ADC();
	TDEC_1h_BUZZER();
	TDEC_1h_COLD();
	TDEC_1h_DISPLAY();
	TDEC_1h_PURE();
	TDEC_1h_EEPROM();
	TDEC_1h_ERROR();
	TDEC_1h_HOT();
	TDEC_1h_INPUT();
	TDEC_1h_KEYCONTROL();
	TDEC_1h_MACRO();
	TDEC_1h_MAIN();
	TDEC_1h_OUTPUT();
	TDEC_1h_SAFETY();
	TDEC_1h_STEPMOTOR();
	TDEC_1h_TIMER();
	TDEC_1h_FLOWMETER();
	TDEC_1h_OPERATION();
	TDEC_1h_PBA();
	TDEC_1h_COMMUNICATION();
	TDEC_1h_SERIAL();
	TDEC_1h_COMMAND();
	TDEC_1h_SMARTCHECK();

	mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_DISP);	//	Wi-Fi 연결상태 확인

	if (mu8_WifiDisp == WIFI_DISP_CONNECT)			//	연결된 경우에만
	{
		buf1 = GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY1);
		buf2 = GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY2);

		data = GetWifiStatusValue(WIFI_STATUS_AP_FIRST);

		if ((data == SET) &&						//	최초 연결 완료 제품
			(((buf1 == 0xFFFFU) && (buf2 == 0xFFFFU))	//	방문관리 및 첫 설치는 표시하지 않음
			|| ((buf1 == 9999) && (buf2 == 9999))		//	방문관리 및 첫 설치는 표시하지 않음
			|| ((buf1 == 0x9999) && (buf2 == 0x9999))))	//	방문관리 및 첫 설치는 표시하지 않음
		{
			F_SelfService = CLEAR;
		}
		else
		{
			F_SelfService = SET;
		}
	}

	if (++Day_Count >= 24)
	{
		Day_Count = 0;
		if (gu16_DayCount_forFilter < 65535)
		{
			gu16_DayCount_forFilter++;
		}

		if (gu16_DayCount_forFaucet < 65535)
		{
			gu16_DayCount_forFaucet++;
		}

		gu16_PartTest_Period = GetWifiRequestValue(WIFI_RQST_PART_PERIOD);	//	상시 진단 주기 요청
	}
}
