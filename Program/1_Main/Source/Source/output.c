/// @file     output.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    출력 처리 file
#define OUTPUT_C
#include "include/_include.h"

static void Output_PureValve(void);
static void Output_NosValve(void);
static void Output_HotInValve(void);
static void Output_HotOutValve(void);
static void Output_ColdInValve(void);
static void Output_ColdOutValve(void);
static void Output_Heater(void);
static void Output_TEM(void);
static void Output_FAN(void);
static void Output_ColdSensor_ONOFF(void);
static void Output_LeakSensor(void);
static void Output_VoiceReset(void);
static void Output_FCT_TxRx(void);
static void Output_UV(void);
static void Output_Power(void);

static void OutputNotUsePort(void);

#ifdef _DPP_FLOWMETER_
#define Z_HeaterOFF_Hz		(100)					//	100Hz(약10cc) [1Hz]
#else
//#define Z_HeaterOFF_Hz		(20)					//	20Hz(약8cc) [1Hz]
#define Z_HeaterOFF_Hz		(35)					//	35(약14cc) [1Hz]
#endif





//#define TEM_PWM_FULL_TDR	((_DABF_TAU_TDR02_VALUE+1) - (_DABF_TAU_TDR02_VALUE+1))	//	반전 출력
//#define TEM_PWM_OFF_TDR		((_DABF_TAU_TDR02_VALUE+1))		//	반전 출력

#define TEM_PWM_FULL_TDR	((_DABF_TAU_TDR10_VALUE+1) - (_DABF_TAU_TDR10_VALUE+1))	//	반전 출력
#define TEM_PWM_OFF_TDR		((_DABF_TAU_TDR10_VALUE+1)) //DABF = 55,999



//#define TEM_PWM_RMS_CAL		((_DABF_TAU_TDR02_VALUE+1) / 240 * 8 / 10)
//#define TEM_PWM_RMS_CAL		(106)		//	((_DABF_TAU_TDR02_VALUE+1) / 240 * 8 / 10)


TEM_Voltage_Data TEM_Voltage_DataTable[Z_TEM_Voltage_Data_Max] =    //  파워넷 SMPS
{
		{	12,  	0	},
		{	11, 	10000	},
		{	10,	20000	},
		{	9, 	30000	},
		{	8, 	40000	},
		{	7, 	50000	},
		{	6, 	53000	},
		{	5, 	55000	},//PWM RMS : 5.5, OUTPUT RMS :  4.8
		
		//{	9, 	28000	}, //PWM RMS : 1.82, OUTPUT RMS :  9.1
		//{	9, 	27000	}, //PWM RMS : 1.82, OUTPUT RMS :  9.1
		//{	9, 	26000	}, //PWM RMS : 1.82, OUTPUT RMS :  9.2
		//{	9, 	25000	}, //PWM RMS : 1.82, OUTPUT RMS :  9.4
		//{	9, 	20000	}, //PWM RMS : 1.6, OUTPUT RMS :  10.00
	        //{	9, 	17000	}, //PWM RMS : 1.3, OUTPUT RMS :  10.37
	        // {	9, 	14000	},
		
		// {	5, 	52000	},//PWM RMS : 4.4, OUTPUT RMS :  6.0
		// {	5, 	45000	},//PWM RMS : 3.4, OUTPUT RMS :  6.9
		//{	5, 	32000	},//PWM RMS : 2.6, OUTPUT RMS :  8.5
};
/*20260101
#ifndef _SUNRINTECH_SMPS_
TEM_Voltage_Data TEM_Voltage_DataTable[Z_TEM_Voltage_Data_Max] =	//	파워넷 SMPS
{
	{	24, 0		},
	{	23, 5500	},
	{	22, 11500	},
	{	21, 16300	},
	{	20, 20300	},
	{	19, 23400	},
	{	18, 26000	},
	{	17, 28500	},
	{	16, 30500	},
	{	15, 32400	},
	{	14, 34000	},
	{	13, 35600	},
	{	12, 37000	},
	{	11, 38500	},
	{	10,	39800	},
	{	9, 41100	},
	{	8, 42400	},
	{	7, 43700	},
	{	6, 45000	},
	{	5, 46600	},
	{	4, 48100	},
	{	3, 50000	},
	{	2, 50000	},
	{	1, 50000	},
};
#else
TEM_Voltage_Data TEM_Voltage_DataTable[Z_TEM_Voltage_Data_Max] =	//	선린테크
{
	{	24, 0		},
	{	23, 26100	},
	{	22, 27600	},
	{	21, 29100	},
	{	20, 30600	},
	{	19, 32100	},
	{	18, 33600	},
	{	17, 35000	},
	{	16, 36400	},
	{	15, 37500	},
	{	14, 39000	},
	{	13, 40400	},
	{	12, 41600	},
	{	11, 42900	},
	{	10, 44100	},
	{	9, 45300	},
	{	8, 46500	},
	{	7, 47600	},
	{	6, 48700	},
	{	5, 49800	},
	{	4, 50800	},
	{	3, 51800	},
	{	2, 52800	},
	{	1, 53800	},
};
#endif
*/
#define Z_TEM_Rapid_Data_Max	(3)

#ifndef _SUNRINTECH_SMPS_
TEM_Data TEM_Rapid_DataTable[Z_TEM_Rapid_Data_Max] =
{	//	외기온		전압
	{Z_TEMP30, 		21 		},
	{Z_TEMP12, 		21		},
	{Z_TEMP12, 		12		},
};
#else
TEM_Data TEM_Rapid_DataTable[Z_TEM_Rapid_Data_Max] =
{	//	외기온		전압
	{Z_TEMP30, 		15 		},
	{Z_TEMP12, 		15		},
	{Z_TEMP12, 		12		},
};
#endif

#define Z_TEM_Retain_Data_Max	(10)

//@014 시작
TEM_Data TEM_Retain_DataTable[Z_TEM_Retain_Data_Max] =
{	//	외기온		전압
	{Z_TEMP38, 		18		},
	{Z_TEMP33, 		17		},
	{Z_TEMP31, 		12		},
	{Z_TEMP27, 		10 		},
	{Z_TEMP24, 		9		},
	{Z_TEMP21, 		8		},
	{Z_TEMP18, 		5 		},
	{Z_TEMP15, 		4		},
	{Z_TEMP6, 		4		},
	{Z_TEMP6, 		3		},
};
//@014 끝

/// @brief    출력처리 초기화
/// @param    void
/// @return   void
void Output_Initialize(void)
{
	F_ColdSensor_OFF = CLEAR;
	gu16_TimerColdSensor_ON = Z_TimerColdSensor_ON_Init;
	gu8_TimerBoot_After1s = Z_TimerBoot_After1s;
	gu8_TimerStanbyOn_After1s = Z_TimerStanbyOn_After1s;
}

/// @brief    출력 처리
/// @param    void
/// @return   void
void OutputControl(void)
{
	/************************************************/
	/*---------------------------------*/
	/*	전 부하의 출력 처리				   */
	/*---------------------------------*/
	Output_PureValve();								//	정수 피드밸브
	Output_NosValve();								//	노스밸브
	Output_ColdInValve();							//	냉수입수 피드밸브
	Output_ColdOutValve();							//	냉수출수 피드밸브
	if ((gu8_ModelType_forCHP == MODEL_CHP) || (F_TestMode_PBA == SET))	//	온수모델인 경우 or PBA테스트모드인 경우
	{
		Output_HotInValve();						//		온수입수 피드밸브
		Output_HotOutValve();						//		온수출수 피드/노스밸브
		Output_Heater();							//		히터 RELAY/TRIAC
	}
	Output_TEM();									//	열전모듈
	Output_FAN();									//	FAN
	Output_ColdSensor_ONOFF();
	Output_VoiceReset();
	Output_LeakSensor();
	Output_FCT_TxRx();
	Output_UV();
	Output_Power();

	/*---------------------------------*/
	/*	미사용 PORT 출력 처리			   */
	/*---------------------------------*/
	OutputNotUsePort(); 							//	미사용 포트 출력
}

/// @brief    미사용 포트 처리
/// @param    void
/// @return   void
static void OutputNotUsePort(void)
{
	/************************************************/

}

/// @brief    정수밸브
/// @param    void
/// @return   void
static void Output_PureValve(void)
{
	#ifdef _COLD_TEST_
	if (gu8_Timer_RAPID_OUT != 0)
	{
		gu8_PureValve_CMD = VALVE_ON;
	}
	#endif
#ifdef _FRONT_JIG_PROGRAM_
//	if ((gu8_FRONT_JIG_Complete & 0b00011111) == 0b00011111)
	if ((gu8_FrontTest_HotButton_Counter >= 6)
		&& (gu8_FrontTest_PureButton_Counter >= 8)
		&& (gu8_FrontTest_ColdButton_Counter >= 6)
		&& (gu8_FrontTest_AmountButton_Counter >= 2)
		&& (F_FrontTest_EffluentButton == SET))
	{
		P_FEED_PURE_V = ON;
	}
	else
	{
		P_FEED_PURE_V = OFF;
	}
	return;
#endif
	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_PureLoad == 2)
		{
			P_FEED_PURE_V = ON;
		}
		else
		{
			P_FEED_PURE_V = OFF;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if (F_LineTest2_Purge == SET)
		{
			P_FEED_PURE_V = ON;
		}
		else
		{
			P_FEED_PURE_V = OFF;
		}
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_Pure_VV == SET)
		{
			P_FEED_PURE_V = ON;
		}
		else
		{
			P_FEED_PURE_V = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		if ((F_LineFrontTest_Effluent_Oper == SET) && (F_LineFrontTest_Pure_Select == SET))
		{
			P_FEED_PURE_V = ON;
		}
		else
		{
			P_FEED_PURE_V = OFF;
		}
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_Pure_VV == SET)
			{
				P_FEED_PURE_V = ON;
			}
			else
			{
				P_FEED_PURE_V = OFF;
			}
		}
		else
		{
			if ((gu8_OperationStep == Z_OperationStep_F0) && ((gu16_TimerKITMode > (Z_TimerKITMode-30)) || (gu16_TimerKITMode < 30)))
			{
				gu8_PureValve_CMD = VALVE_ON;
			}

			if (gu8_PureValve_CMD == VALVE_ON)
			{
				P_FEED_PURE_V = ON;
			}
			else
			{
				P_FEED_PURE_V = OFF;
			}
		}
	}
}

/// @brief    NOS밸브
/// @param    void
/// @return   void
static void Output_NosValve(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_PureLoad == 1)
		{
			P_NOS_V = N_OFF;
		}
		else
		{
			P_NOS_V = N_ON;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		P_NOS_V = N_ON;
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_1stNos_VV == SET)
		{
			P_NOS_V = N_OFF;
		}
		else
		{
			P_NOS_V = N_ON;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		if (F_LineFrontTest_1stNos_VV == SET)	//@023
		{                                       //@023
			P_NOS_V = N_OFF;                    //@023
		}                                       //@023
		else                                    //@023
		{                                       //@023
			P_NOS_V = N_ON;                     //@023
		}                                       //@023
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if ((F_SmartCheck_1stNos_VV == SET) || (INPUT_REED_SW_FILTER == CLEAR))	//
			{
				P_NOS_V = N_OFF;
			}
			else
			{
				P_NOS_V = N_ON;
			}
		}
		else
		{
			if ((gu8_ErrorCode == Z_Ecode01)
				|| (gu8_ErrorCode == Z_Ecode02)
				|| ((gu8_OperationStep != Z_OperationStep_F0) && ((INPUT_REED_SW_FILTER == CLEAR) || (INPUT_REED_SW_COVER == CLEAR) || (INPUT_REED_SW_FAUCET == CLEAR)))
//				|| (INPUT_REED_SW_FILTER == CLEAR) || (INPUT_REED_SW_COVER == CLEAR) || (INPUT_REED_SW_FAUCET == CLEAR)
				|| (gu8_OperationStep == Z_OperationStep_B0))
			{
				P_NOS_V = N_OFF;
			}
			else
			{
				P_NOS_V = N_ON;
			}
		}
	}
}

/// @brief    온수입수 밸브
/// @param    void
/// @return   void
static void Output_HotInValve(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_HotLoad == 1)
		{
			P_FEED_HOTIN_V = ON;
		}
		else
		{
			P_FEED_HOTIN_V = OFF;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if (F_LineTest2_Purge == SET)
		{
			P_FEED_HOTIN_V = ON;
		}
		else
		{
			P_FEED_HOTIN_V = OFF;
		}
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_HotIn_VV == SET)
		{
			P_FEED_HOTIN_V = ON;
		}
		else
		{
			P_FEED_HOTIN_V = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		if ((F_LineFrontTest_Effluent_Oper == SET) && (F_LineFrontTest_Hot_Select == SET))
		{
			P_FEED_HOTIN_V = ON;
		}
		else
		{
			P_FEED_HOTIN_V = OFF;
		}
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_HotIn_VV == SET)
			{
				P_FEED_HOTIN_V = ON;
			}
			else
			{
				P_FEED_HOTIN_V = OFF;
			}
		}
		else
		{
			if (gu8_HotInValve_CMD == VALVE_ON)
			{
				P_FEED_HOTIN_V = ON;
			}
			else
			{
				P_FEED_HOTIN_V = OFF;
			}
		}
	}
}

/// @brief    온수출수 FEED/NOS 밸브
/// @param    void
/// @return   void
static void Output_HotOutValve(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_HotLoad == 2)
		{
			P_FEED_HOTOUT_V = ON;
		}
		else
		{
			P_FEED_HOTOUT_V = OFF;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if ((F_LineTest2_Valve == SET) || (F_LineTest2_Purge == SET))
		{
			P_FEED_HOTOUT_V = ON;
		}
		else
		{
			P_FEED_HOTOUT_V = OFF;
		}
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_HotOut_VV == SET)
		{
			P_FEED_HOTOUT_V = ON;
		}
		else
		{
			P_FEED_HOTOUT_V = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		if ((F_LineFrontTest_Effluent_Oper == SET) && (F_LineFrontTest_Hot_Select == SET))
		{
			P_FEED_HOTOUT_V = ON;
		}
		else if (F_LineFrontTest_Hot_Complete == SET)
		{
			P_FEED_HOTOUT_V = ON;
		}
		else
		{
			P_FEED_HOTOUT_V = OFF;
		}
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_HotOut_VV == SET)
			{
				P_FEED_HOTOUT_V = ON;
			}
			else
			{
				P_FEED_HOTOUT_V = OFF;
			}
		}
		else
		{
			if (gu8_HotOutValve_CMD == VALVE_ON)		//		출수 지시인 경우
			{
				P_FEED_HOTOUT_V = ON;
			}
			else
			{
				P_FEED_HOTOUT_V = OFF;
			}
		}
	}
}

/// @brief    냉수출수 밸브
/// @param    void
/// @return   void
static void Output_ColdOutValve(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_ColdLoad == 2)
		{
			P_FEED_COLD_OUT_V = ON;
		}
		else
		{
			P_FEED_COLD_OUT_V = OFF;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if ((F_LineTest2_Valve == SET) || (F_LineTest2_Purge == SET))
		{
			P_FEED_COLD_OUT_V = ON;
		}
		else
		{
			P_FEED_COLD_OUT_V = OFF;
		}
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_ColdOut_VV == SET)
		{
			P_FEED_COLD_OUT_V = ON;
		}
		else
		{
			P_FEED_COLD_OUT_V = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		if ((F_LineFrontTest_Effluent_Oper == SET) && (F_LineFrontTest_Cold_Select == SET))
		{
			P_FEED_COLD_OUT_V = ON;
		}
		else if (F_LineFrontTest_Cold_Complete == SET)
		{
			P_FEED_COLD_OUT_V = ON;
		}
		else
		{
			P_FEED_COLD_OUT_V = OFF;
		}
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_ColdOut_VV == SET)
			{
				P_FEED_COLD_OUT_V = ON;
			}
			else
			{
				P_FEED_COLD_OUT_V = OFF;
			}
		}
		else
		{
			if ((gu8_OperationStep == Z_OperationStep_F0) && ((gu16_TimerKITMode <= (Z_TimerKITMode-30)) && (gu16_TimerKITMode >= 30)))
			{
				gu8_Cold_Out_Valve_CMD = VALVE_ON;
			}

			if (gu8_Cold_Out_Valve_CMD == VALVE_B0)
			{
				if (gu8_TimerPressureClear_ColdVV_Delay == 0)
				{
					if (gu8_TimerPressureClear_ColdVV != 0)
					{
						P_FEED_COLD_OUT_V = ON;
					}
					else
					{
						P_FEED_COLD_OUT_V = OFF;
					}
				}
				else
				{
					P_FEED_COLD_OUT_V = OFF;
				}
			}
			else if (gu8_Cold_Out_Valve_CMD == VALVE_ON)
			{
				P_FEED_COLD_OUT_V = ON;
			}
			else
			{
				P_FEED_COLD_OUT_V = OFF;
			}
		}
	}
}

/// @brief    냉수입수 밸브
/// @param    void
/// @return   void
static void Output_ColdInValve(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_ColdLoad == 1)
		{
			P_FEED_COLD_IN_V = ON;
		}
		else
		{
			P_FEED_COLD_IN_V = OFF;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if (F_LineTest2_Purge == SET)
		{
			P_FEED_COLD_IN_V = ON;
		}
		else
		{
			P_FEED_COLD_IN_V = OFF;
		}
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_ColdIn_VV == SET)
		{
			P_FEED_COLD_IN_V = ON;
		}
		else
		{
			P_FEED_COLD_IN_V = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		if ((F_LineFrontTest_Effluent_Oper == SET) && (F_LineFrontTest_Cold_Select == SET))
		{
			P_FEED_COLD_IN_V = ON;
		}
		else
		{
			P_FEED_COLD_IN_V = OFF;
		}
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_ColdIn_VV == SET)
			{
				P_FEED_COLD_IN_V = ON;
			}
			else
			{
				P_FEED_COLD_IN_V = OFF;
			}
		}
		else
		{
			if ((gu8_OperationStep == Z_OperationStep_F0) && ((gu16_TimerKITMode <= (Z_TimerKITMode-30)) && (gu16_TimerKITMode >= 30)))
			{
				gu8_Cold_In_Valve_CMD = VALVE_ON;
			}

			if (gu8_Cold_In_Valve_CMD == VALVE_B0)
			{
				if (gu8_TimerPressureClear_ColdVV_Delay == 0)
				{
					if (gu8_TimerPressureClear_ColdVV != 0)
					{
						P_FEED_COLD_IN_V = ON;
					}
					else
					{
						P_FEED_COLD_IN_V = OFF;
					}
				}
				else
				{
					P_FEED_COLD_IN_V = OFF;
				}
			}
			else if (gu8_Cold_In_Valve_CMD == VALVE_ON)
			{
				P_FEED_COLD_IN_V = ON;
			}
			else
			{
				P_FEED_COLD_IN_V = OFF;
			}
		}
	}
}

/// @brief    히터
/// @param    void
/// @return   void

U8 gu8Test_Var;
U16 mu16_HeaterTEst_TDR;
bit Z__PWM_TDR_2_3, Z__PWM_TDR_50,Z__PWM_TDR_60,Z__PWM_TDR_72,Z__PWM_TDR_100,Z__PWM_TDR_66, Z__PWM_TDR_00;

static void Output_Heater(void)
{
//	U16 mu16_HeaterPWM_TDR = 0;
	U16 mu16_Heater_work1 = 0;
	U16 mu16_Heater_work2 = 0;
	U16 mu16_Buf1 = 0;
	U8 mu8_OPENSHORT_Error1 = 0;
	U8 mu8_OPENSHORT_Error2 = 0;
	U16 HeaterOff_Time = 0;
	I16 HeaterOff_Buf = 0;
	I16 mi16_Buf1, mi16_Buf2 = 0;
	/*
	Z__PWM_TDR_50=0;
	Z__PWM_TDR_60=0;
	Z__PWM_TDR_72=0;
	Z__PWM_TDR_100=0;
	Z__PWM_TDR_66=0;
	Z__PWM_TDR_00=0;
	*/

#ifdef _HEATER_OFF_
	P_HEATER_RELAY = OFF;
	P_HEATER_TRIAC = OFF;
	P_HEATER_PWM_TDR = 0x0000U;
	return;
#endif

#ifdef _FOR_WQA_
	P_HEATER_RELAY = OFF;
	P_HEATER_TRIAC = OFF;
	P_HEATER_PWM_TDR = 0x0000U;
	return;
#endif

	mu8_OPENSHORT_Error1 = TH_Open_Short_Check(gi16_HotOutSensor.TH_ADC_AVERAGE);	//	온수출수TH 단선단락 체크
	mu8_OPENSHORT_Error2 = TH_Open_Short_Check(gi16_HeaterSensor.TH_ADC_AVERAGE);	//	히터TH 단선단락 체크
#ifdef _FOR_HEATER_ENDURANCE_TEST_
//	if ((F_Heater_Endurance_Test == SET) && (gu8_TimerHeaterEndurance_Test >= 30) && (gu8_HotStep == Z_HotStep_35))
//	{
//		P_HEATER_RELAY = OFF;
//		P_HEATER_TRIAC = OFF;
//		P_HEATER_PWM_TDR = 0x0000U;
//		return;
//	}
	if ((F_Heater_Endurance_Test == SET) && (gu16_TimerEffluent < 600) && (gu8_HotStep == Z_HotStep_35))
	{
		P_HEATER_RELAY = OFF;
		P_HEATER_TRIAC = OFF;
		P_HEATER_PWM_TDR = 0x0000U;
		return;
	}
#endif
	if ((gu8_SmartCheckStep & 0xF0) == 0x20)
	{
		if (F_SmartCheck_HeaterRelay == SET)
		{
			P_HEATER_RELAY = ON;
		}
		else
		{
			P_HEATER_RELAY = OFF;
		}

		if (F_SmartCheck_HeaterTriac == SET)
		{
			P_HEATER_TRIAC = ON;
			P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
		}
		else
		{
			P_HEATER_TRIAC = OFF;
			P_HEATER_PWM_TDR = 0x0000U;
		}
	}
	else if ((mu8_OPENSHORT_Error1 != 0) || (mu8_OPENSHORT_Error2 != 0))	//	단선단락 시
	{
		P_HEATER_RELAY = OFF;
		P_HEATER_TRIAC = OFF;
		P_HEATER_PWM_TDR = 0x0000U;
	}
	else if ((gi16_HeaterSensor.DEGREE >= Z_TEMP103)
		&& ((gu8_HotStep == Z_HotStep_33) || (gu8_HotStep == Z_HotStep_34)
		|| ((gu8_HotStep == Z_HotStep_35) && (gu16_FlowDecide_W < 10) && (gu8_TimerFlowCheck_AfterEff != 0))))	//	온수 추출 중 플로우체크 타이머 경과 되지 않았을 때
	{
		P_HEATER_RELAY = OFF;
		P_HEATER_TRIAC = OFF;
		P_HEATER_PWM_TDR = 0x0000U;
	}
	else
	{
		if ((gu8_HotStep != Z_HotStep_32) &&(gu8_HotStep != Z_HotStep_33) && (gu8_HotStep != Z_HotStep_34)
			&& ((gu8_HotStep & 0xF0) == 0x30) && (gu16_FlowDecide_W < 10) && (gu8_TimerFlowCheck_AfterEff == 0))	//	온수 추출 시퀀스 중 예열스텝을 제외하고 유량이 0.1LPM 미만이면 히터 정지
		{
			P_HEATER_RELAY = OFF;
			P_HEATER_TRIAC = OFF;
			P_HEATER_PWM_TDR = 0x0000U;
		}
		else
		{
			HeaterOff_Buf = (((I16)Z_TEMP25 - gi16_HotInSensor.DEGREE)/5);
			if (HeaterOff_Buf <= Z_HeaterOFF_Hz)
			{
				gu16_HeaterOff_Hz = (U16)Z_HeaterOFF_Hz - HeaterOff_Buf;
			}
			else
			{
				gu16_HeaterOff_Hz = 0;
			}
//			gu16_HeaterOff_Hz = Z_HeaterOFF_Hz - (((I16)Z_TEMP25 - gi16_HotInSensor.DEGREE)/5);
			HeaterOff_Time = 20;					//	2초 <1/10s>

#ifdef _HeaterOff_OFF_
			gu16_HeaterOff_Hz = 0;
			HeaterOff_Time = 0;
#else
//			/*	잠열 제거 히터 OFF 조건	*/
#ifdef _HOT_TEMP_4STEP_
                         //-------------------------------------------20260201--------------------------
 			if ((gu8_ModelType_forNational == MODEL_KOREA)
            			&& (gu8_HotStep == Z_HotStep_35) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95)
                    		|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90) ||(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)))
			//if ((gu8_ModelType_forNational == MODEL_KOREA)
			//&& (gu8_HotStep == Z_HotStep_35) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)))
			//--------------------------------------------------------------------------------
//			if ((gu8_HotStep == Z_HotStep_35) && (gu8_HotTempSetup == HOT_TEMP_SETUP_85))
#else
			if ((gu8_ModelType_forNational == MODEL_KOREA)
			&& (gu8_HotStep == Z_HotStep_35) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))
#endif
			{
				//if (gu8_CupSize == CUPSIZE_CONTINUE)	//	연속 추출 시 마지막 3초 잠열제거 시작
				if (gu16_CupSize == CUPSIZE_CONTINUE)	//	연속 추출 시 마지막 3초 잠열제거 시작
				{
					if (gu16_TimerEffluent <= HeaterOff_Time)
					{
						if (gu16_TimerEffluent < (HeaterOff_Time/3))	//220405
						{
							F_LatentHeat_Clear = SET;
						}
						gu8_HeaterRelay_CMD = HEATER_OFF;
						gu8_HeaterTriac_CMD = HEATER_OFF;
					}
				}
				else
				{	/*
					//-------------------------------------------20260201--------------------------
					if (gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH)
					{
						F_LatentHeat_Clear = CLEAR;
						if (gu16_Effluent_Hz <= gu16_HeaterOff_Hz)				//2025-12-30
						{
							gu8_HeaterRelay_CMD = HEATER_OFF;
							gu8_HeaterTriac_CMD = HEATER_OFF;
						}
					}
					else
					{
						*/
					//-------------------------------------------20260201--------------------------
						if (gu16_Effluent_Hz <= gu16_HeaterOff_Hz)	//	잠열제거 시작 유량 체크
						{
							if (gu16_Effluent_Hz < (gu16_HeaterOff_Hz/3))	//220405
							{
								F_LatentHeat_Clear = SET;
							}
							gu8_HeaterRelay_CMD = HEATER_OFF;
							gu8_HeaterTriac_CMD = HEATER_OFF;
						}
					//}
				}
			}
#endif
			/*	릴레이	*/
			if (F_TestMode_PBA == SET)
			{
				if ((gu8_PBATest_HotLoad == 3) && (gu8_TimerPBATest_HeaterRelay != 0))
				{
					P_HEATER_RELAY = ON;
				}
				else
				{
					P_HEATER_RELAY = OFF;
				}
			}
			else if (F_TestMode_LineTest2 == SET)
			{
				if (gu8_TimerLineTest2_Heater != 0)
				{
					P_HEATER_RELAY = ON;
				}
				else
				{
					P_HEATER_RELAY = OFF;
				}
			}
			else if (F_TestMode_LineFront == SET)
			{
//				if ((gu8_TimerLineTest_Heater != 0) && (gu8_TimerLineTest_Heater > ((Z_TimerLineTest_Heater/2)+50)))
				if (gu8_TimerLineTest_Heater != 0)
				{
					P_HEATER_RELAY = ON;
				}
				else
				{
					P_HEATER_RELAY = OFF;
				}
			}
			else
			{
				switch (gu8_HeaterRelay_CMD)
				{
					case HEATER_OFF:
					default:
						P_HEATER_RELAY = OFF;
						Z__PWM_TDR_50=0;
	Z__PWM_TDR_60=0;
	Z__PWM_TDR_72=0;
	Z__PWM_TDR_100=0;
	Z__PWM_TDR_66=0;
	Z__PWM_TDR_00=0;
	Z__PWM_TDR_2_3=0;
						break;
					case HEATER_FIRST:
						if (gu8_TimerFirstHot_HeaterSensing != 0)
						{
							P_HEATER_RELAY = ON;
						}
						else
						{
							P_HEATER_RELAY = OFF;
						}
						break;
					case HEATER_ON:
						if (gu16_HeaterRELAY_WATT > 200)		//		히터 RELAY WATT가 200W 이상인 경우
						{
							mu16_Buf1 = gu16_HeaterRELAY_WATT - 200;	//
							if (gu16_HeaterWATT > gu16_HeaterRELAY_WATT)
							{
								F_HeaterPWM_SingleControl = CLEAR;
							}
						}
#ifndef _HEATER_CURRENT_CHECK_
						if ((F_HeaterPWM_SingleControl == SET)
						|| ((gu8_HotStep == Z_HotStep_35) && (gu16_HeaterWATT <= mu16_Buf1)) 	//		온수 추출 중 필요 WATT가 X보다 낮은 경우
						|| (F_MilkEff_MoreHot == SET))	//		분유 추출 시작 시 내열이 높은 경우
						{
							F_HeaterPWM_SingleControl = SET;	//			PWM Single Control 제어 설정
							P_HEATER_RELAY = OFF;				//			히터 RELAY OFF
						}
						else
						{       
							if ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerHeaterSafety_AfterEff == 0)
							&& (gu16_FlowDecide_W < (gu16_FlowTarget_W*2/3)))	//	유량이 적은 경우
							{
								mi16_Buf1 = gu16_FlowTarget_W - gu16_FlowDecide_W;
								if ((gi16_HotOutSensor.DEGREE > (Z_TEMP100-mi16_Buf1)) || (gi16_HeaterSensor.DEGREE > (Z_TEMP100-mi16_Buf1)))
								{
									P_HEATER_RELAY = OFF;				//			히터 RELAY OFF
								}
								else if ((gi16_HotOutSensor.DEGREE <= (Z_TEMP100-mi16_Buf1-Z_TEMP2)) && (gi16_HeaterSensor.DEGREE <= (Z_TEMP100-mi16_Buf1-Z_TEMP2)))
								{
									P_HEATER_RELAY = ON;
								}
								else
								{
									//	히스테리시스
								}
							}
							else
							{
//								if (gi16_HotOutSensor.DEGREE > Z_TEMP103_3)
								if ((gi16_HotOutSensor.DEGREE > Z_TEMP103_5) || (gi16_HeaterSensor.DEGREE > Z_TEMP107))
								{
									P_HEATER_RELAY = OFF;				//			히터 RELAY OFF
								}
//								else if (gi16_HotOutSensor.DEGREE <= Z_TEMP100)     //	일반 제어
								else if ((gi16_HotOutSensor.DEGREE <= Z_TEMP100) && (gi16_HeaterSensor.DEGREE <= Z_TEMP105))     //	일반 제어
								{
									P_HEATER_RELAY = ON;				//			히터 RELAY
								}
								else
								{
									//	히스테리시스
								}
							}
							
						
						}
#else
						P_HEATER_RELAY = ON;
#endif
						break;
				}
			}
#ifdef _FOR_DEVELOP_
			if ((gu8_HotStep == Z_HotStep_35) && (P_HEATER_RELAY == OFF))
			{
				gu8_Counter_HeaterOFF++;
			}
#endif

			/*	TRIAC	*/
			if (F_TestMode_PBA == SET)
			{
				if ((gu8_PBATest_HotLoad == 4) && (gu8_TimerPBATest_HeaterTriac != 0))
				{
					P_HEATER_TRIAC = ON;
					P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
				}
				else
				{
					P_HEATER_TRIAC = OFF;
					P_HEATER_PWM_TDR = 0x0000;			//			TDR CLEAR
				}
			}
			else if (F_TestMode_LineTest2 == SET)
			{
				if (gu8_TimerLineTest2_Heater != 0)
				{
					P_HEATER_TRIAC = ON;
					P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
				}
				else
				{
					P_HEATER_TRIAC = OFF;
					P_HEATER_PWM_TDR = 0x0000;			//			TDR CLEAR
				}
			}
			else if (F_TestMode_LineFront == SET)
			{
//				if ((gu8_TimerLineTest_Heater != 0) && (gu8_TimerLineTest_Heater <= ((Z_TimerLineTest_Heater/2)-50)))
				if (gu8_TimerLineTest_Heater != 0)
				{
					P_HEATER_TRIAC = ON;
					P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
				}
				else
				{
					P_HEATER_TRIAC = OFF;
					P_HEATER_PWM_TDR = 0x0000;		//			TDR CLEAR
				}
			}
			else
			{
				switch (gu8_HeaterTriac_CMD)		//	히터 PWM 지시
				{
					case HEATER_OFF:				//		OFF 지시
					default:
						P_HEATER_PWM_TDR = 0x0000;	//			TDR CLEAR
						P_HEATER_TRIAC = OFF;		//			PORT OFF (Default)
						break;
					case HEATER_FIRST:
						if (gu8_TimerFirstHot_HeaterSensing != 0)
						{
							P_HEATER_TRIAC = ON;
						}
						else
						{
							P_HEATER_TRIAC = OFF;
							P_HEATER_PWM_TDR = 0x0000;		//			TDR CLEAR
						}
						break;
					case HEATER_ON:							//		100% ON 지시
						P_HEATER_TRIAC = ON;
						P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);

						#ifdef _EFF_Continue_Heater_OFF
                        if(F_Eff_Continue_Heater_Off == SET){
                            P_HEATER_RELAY = OFF;                           //  히터 Relay Off
                            P_HEATER_TRIAC = OFF;                           //  히터 트리악 Off
                            P_HEATER_PWM_TDR = 0x0000U;                     //  히터 트리악 PWM Off
                            return;
                        }

						#endif

						break;
					case HEATER_PRE:
						#ifdef _EFF_Continue_Heater_OFF
                        if(F_Eff_Continue_Heater_Off == SET){
                            P_HEATER_RELAY = OFF;                           //  히터 Relay Off
                            P_HEATER_TRIAC = OFF;                           //  히터 트리악 Off
                            P_HEATER_PWM_TDR = 0x0000U;                     //  히터 트리악 PWM Off
                            return;
                        }

						#endif
						if (gu8_ModelType_forNational == MODEL_KOREA)
						{
							if (F_2ndHot_Effluent_HeaterDown == SET)	//	두번째잔 히터 예열 약하게
							{
								P_HEATER_TRIAC = OFF;
								if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
								{
									if (F_LatentHeat_Clear == SET)		//	잠열 제거 실시한 경우
									{
										P_HEATER_PWM_TDR = (U16)((U32)((_DABF_TAU_TDR02_VALUE+1)/4) * HEATER_FULL_WATT_KOR / gu16_HeaterFull_WATT);
									}
									else					//	잠열 제거 미실시한 경우
									{
										P_HEATER_PWM_TDR = (U16)((U32)((_DABF_TAU_TDR02_VALUE+1)/10) * HEATER_FULL_WATT_KOR / gu16_HeaterFull_WATT);
										//P_HEATER_PWM_TDR = (U16)((U32)((_DABF_TAU_TDR02_VALUE+1)/8) * HEATER_FULL_WATT_KOR / gu16_HeaterFull_WATT);
									}
								}
								else						//	온수 온도 설정 저온인 경우
								{
									P_HEATER_PWM_TDR = (U16)((U32)((_DABF_TAU_TDR02_VALUE+1)/4) * HEATER_FULL_WATT_KOR / gu16_HeaterFull_WATT);
								}
							}
							else
							{
								P_HEATER_TRIAC = ON;
								P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE + 1);
							}
						}
						else
						{
							P_HEATER_TRIAC = ON;
							P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
						}
						
						
						
						
						break;
					case HEATER_PWM:				//		PWM 지시
#ifndef _HEATER_CURRENT_CHECK_
#ifdef _EFF_Continue_Heater_OFF
			                        if(F_Eff_Continue_Heater_Off == SET)
						{
			                            P_HEATER_RELAY = OFF;                           //  히터 Relay Off
			                            P_HEATER_TRIAC = OFF;                           //  히터 트리악 Off
			                            P_HEATER_PWM_TDR = 0x0000U;                     //  히터 트리악 PWM Off
			                            return;
			                        }

#endif
						P_HEATER_TRIAC = OFF;		//			PORT OFF (Default)

						if (gu16_HeaterFull_WATT >= gu16_HeaterRELAY_WATT)	//		히터 최대전력치가 RELAY보다 큰 경우 (안전처리)
						{
							mu16_Heater_work1 = gu16_HeaterFull_WATT - gu16_HeaterRELAY_WATT;	//			RELAY WATT수만큼 빼줌 (PWM 최대 WATT 취득)
						}

						if ((F_HeaterPWM_SingleControl == SET))	//	Single Control 제어거나 분유 and 예열 또는 추출 중인 경우
						{
							mu16_Heater_work2 = gu16_HeaterWATT;	//		필요 WATT 취득
							if ((gu16_HeaterFull_WATT >= gu16_HeaterRELAY_WATT) && (mu16_Heater_work2 >= (gu16_HeaterFull_WATT - gu16_HeaterRELAY_WATT)))	//히터 최대전력치-RELAY 전력치를 최대치로 설정 (RELAY를 OFF하니)
							{
								mu16_Heater_work2 = gu16_HeaterFull_WATT - gu16_HeaterRELAY_WATT;	//	PWM 필요 전력치 취득
							}
//							else
//							{
//								mu16_Heater_work2 = gu16_HeaterFull_WATT;
//							}
						}
						else								//		상기 외
						{
							if (gu16_HeaterWATT >= gu16_HeaterRELAY_WATT)	//			히터필요 WATT가 RELAY WATT보다 큰 경우 (안전처리)
							{
								mu16_Heater_work2 = gu16_HeaterWATT - gu16_HeaterRELAY_WATT;	//	PWM 필요 전력치
							}
						}

						mi16_Buf1 = (I16)Z_TEMP20 - gi16_HotInSensor.DEGREE;            //	입수 온도 보정 계산
						mi16_Buf2 = (I16)gu16_FlowDecide_W - (I16)70;
						mi16_Buf2 *= 2;                                                 //	유량 보정 계산

						//gu16_HotOut_ClearMilkHot = Z_TEMP60 + mi16_Buf1 + mi16_Buf2;	//	기준온도 + 입수온도 보정 + 유량 보정
						if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50)
						{
			                        	if(gi16_HotInSensor.DEGREE >= Z_TEMP25){
			                        		gu16_HotOut_ClearMilkHot = Z_TEMP62 + mi16_Buf1 + mi16_Buf2;
			                        	}
			                        	else if(gi16_HotInSensor.DEGREE >= Z_TEMP15 ){
			//@cjh241217                	gu16_HotOut_ClearMilkHot = Z_TEMP58 + mi16_Buf1 + mi16_Buf2;
			                        		gu16_HotOut_ClearMilkHot = Z_TEMP55 + mi16_Buf1 + mi16_Buf2;	//@cjh241217
			                        	}
			                        	else{
			                        		gu16_HotOut_ClearMilkHot = Z_TEMP54 + mi16_Buf1 + mi16_Buf2;
			                        	}
			                        }
									//260416_LEE
						else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55)
						{
			                        	if(gi16_HotInSensor.DEGREE >= Z_TEMP25){
			                        		gu16_HotOut_ClearMilkHot = Z_TEMP67 + mi16_Buf1 + mi16_Buf2;
			                        	}
			                        	else if(gi16_HotInSensor.DEGREE >= Z_TEMP15 ){
			//@cjh241217                	gu16_HotOut_ClearMilkHot = Z_TEMP58 + mi16_Buf1 + mi16_Buf2;
			                        		gu16_HotOut_ClearMilkHot = Z_TEMP63 + mi16_Buf1 + mi16_Buf2;	//@cjh241217
			                        	}
			                        	else{
			                        		gu16_HotOut_ClearMilkHot = Z_TEMP59 + mi16_Buf1 + mi16_Buf2;
			                        	}
			                        }
			                        else{
			                        	gu16_HotOut_ClearMilkHot = Z_TEMP60 + mi16_Buf1 + mi16_Buf2;    //  기준온도 + 입수온도 보정 + 유량 보정
			                        }
									
									//260416_LEE
						if ((F_MilkEff_MoreHot == SET) && (gu8_TimerHeaterControl_AfterEff == 0))
						{       /*
							if (gu16_HotOut_Degree_Ave > gu16_HotOut_ClearMilkHot)
							{
								mu16_Heater_work2 = 0;
							}
							else
							{
								F_MilkEff_MoreHot = CLEAR;
							}
							*/
							if((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50) && (gi16_HotInSensor.DEGREE <= Z_TEMP25))
							{	//@cjh241217
			                        		if(gi16_HotOutSensor.DEGREE <= Z_TEMP55){												//@cjh241217
			                        			F_MilkEff_MoreHot = CLEAR;															//@cjh241217
			                        		}																						//@cjh241217
			                        		else{																					//@cjh241217
			                        			mu16_Heater_work2 = 0;																//@cjh241217
			                        		}																						//@cjh241217
			                        	}																							//@cjh241217
			                        	else{																						//@cjh241217
			                        		if (gu16_HotOut_Degree_Ave > gu16_HotOut_ClearMilkHot)
			                        		{
			                        			mu16_Heater_work2 = 0;
			                        		}
			                        		else
			                        		{
			                        			F_MilkEff_MoreHot = CLEAR;
			                        		}
			                        	}	
							
						}
						else
						{
//							F_MilkEff_MoreHot = CLEAR;
						}

						if ((gu8_TimerHeaterSafety_AfterEff == 0)                       //히터 안전 타이머 경과
							&& (gu16_FlowDecide_W < (gu16_FlowTarget_W*10/11)))	//	유량이 목표유량만큼 안나오는 경우
						{
							mi16_Buf1 = gu16_FlowTarget_W - gu16_FlowDecide_W;					//	유량 차이 계산
							
							//20260310
							//if ((gi16_HotOutSensor.DEGREE > (Z_TEMP95-mi16_Buf1)) || (gi16_HeaterSensor.DEGREE > (Z_TEMP95-mi16_Buf1)))
							if ((gi16_HotOutSensor.DEGREE > (Z_TEMP95_5 - mi16_Buf1)) || (gi16_HeaterSensor.DEGREE > (Z_TEMP95_5 - mi16_Buf1)))
							{
								mu16_Heater_work2 = mu16_Heater_work2 / 3;	//	히터 트리악 PWM 전력치 감소
								Z__PWM_TDR_2_3=1;
							}
							//20260310
							//else if ((gi16_HotOutSensor.DEGREE <= (Z_TEMP95-mi16_Buf1-Z_TEMP2)) && (gi16_HeaterSensor.DEGREE <= (Z_TEMP95-mi16_Buf1-Z_TEMP2)))
							else if ((gi16_HotOutSensor.DEGREE <= (Z_TEMP95_5 - mi16_Buf1 - Z_TEMP2)) && (gi16_HeaterSensor.DEGREE <= (Z_TEMP95_5 - mi16_Buf1 - Z_TEMP2)))
							{
								//	감소 없음. 정상 출력
								Z__PWM_TDR_2_3=0;
							}
							else
							{
								mu16_Heater_work2 = mu16_Heater_work2 / 3;	//	(히스테리 시스 없음)
								Z__PWM_TDR_2_3=1;
							}
						}

						if ((F_2ndHot_Effluent == CLEAR) && (gu8_TimerHeaterControl_AfterEff != 0))	//@001
						{
							P_HEATER_PWM_TDR = 0;	//@001
							Z__PWM_TDR_00=1;
						}
						else
						
						{
							if (((gu8_ModelType_forNational == MODEL_KOREA)
                					&& ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
                					&& (gu8_TimerHeaterSafety_AfterEff != 0)))	//추출 0.7초 pwm최대치 절반
							{
								P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1) / 2;	//@001
								Z__PWM_TDR_50=1;
							}
							else
							{
								//	필요 TDR 연산 : 최대TDR / 히터필요전력치 * 히터PWM최대전력치	
								
								
								gu16_HeaterPWM_TDR = (U16)(((((((U32)_DABF_TAU_TDR02_VALUE+1) * 10) / mu16_Heater_work1) + 1) * mu16_Heater_work2) / 10);
								if ((F_Flow_Rapidly_Decrease == SET) 		//	급 유량감소
								|| ((gu8_ModelType_forNational == MODEL_KOREA) && ((gi16_HotOutSensor.DEGREE >= Z_TEMP101)	|| (gi16_HeaterSensor.DEGREE >= Z_TEMP102))))     //	온수출수 또는 히터TH가 기준온도 이상인 경우 (과열 안전 차단)
								{
									P_HEATER_PWM_TDR = (U16)((U32)gu16_HeaterPWM_TDR * 4 / 6);	////	히터 트리악 PWM 출력 (필요치 대비 66% 출력)
									Z__PWM_TDR_66=1;
								}
								else if (((gu8_ModelType_forNational == MODEL_KOREA)
				                                && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
				                                && (gu8_TimerFlowControl_AfterEff != 0))
				                                || ((gu8_ModelType_forNational == MODEL_KOREA) && ((gi16_HotOutSensor.DEGREE >= Z_TEMP98_5) || (gi16_HeaterSensor.DEGREE >= Z_TEMP100))))     //    온수출수 또는 히터TH가 기준온도 이상인 경우 (과열 안전 차단)
								{
									if (gi16_HotOutSensor.DEGREE >= Z_TEMP101)
									{
										P_HEATER_PWM_TDR = (U16)((U32)gu16_HeaterPWM_TDR * 5 / 10);	//	히터 트리악 PWM 출력 (필요치 대비 50% 출력)
										Z__PWM_TDR_50=1;
									}
									else if (gi16_HotOutSensor.DEGREE >= Z_TEMP100)
									{
										P_HEATER_PWM_TDR = (U16)((U32)gu16_HeaterPWM_TDR * 6 / 10);	//	히터 트리악 PWM 출력 (필요치 대비 60% 출력)
										Z__PWM_TDR_60=1;
									}
									else
									{
										//P_HEATER_PWM_TDR = (U16)((U32)gu16_HeaterPWM_TDR * 8 / 11);	//히터 트리악 PWM 출력 (필요치 대비 72% 출력)
										P_HEATER_PWM_TDR = (U16)((U32)gu16_HeaterPWM_TDR * 9 / 11);	//히터 트리악 PWM 출력 (필요치 대비 72% 출력)//20260403
										Z__PWM_TDR_72=1;
										Z__PWM_TDR_00=0;
									}
								}
								else if (gu8_TimerHeaterLimit != 0)
								{
									P_HEATER_PWM_TDR = (U16)((U32)gu16_HeaterPWM_TDR * 4 / 7);	//	히터 트리악 PWM 출력 (필요치 대비 72% 출력)
									       Z__PWM_TDR_72=1;
									       Z__PWM_TDR_100=0;
								}
								else						//	일반제어
		//						else if ((gi16_HotOutSensor.DEGREE <= Z_TEMP99) && (gi16_HeaterSensor.DEGREE <= Z_TEMP99))     //	일반 제어
								{       
#ifdef _FOR_DEVELOP_
									if (gu16_HeaterPWM_TDR_Test != 0) gu16_HeaterPWM_TDR = gu16_HeaterPWM_TDR_Test;
#endif
									if (gu16_HeaterPWM_TDR > (_DABF_TAU_TDR02_VALUE+1))              //	조건에 따른 실제 히터 트리악 PWM 제어치 > 히터 트리악 PWM 최대 출력치
									{
										P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);            //	히터 트리악 PWM 100% 출력
										Z__PWM_TDR_100=1;
									}
									else if (gu16_HeaterPWM_TDR == 0)				//	조건에 따른 실제 히터 트리악 PWM 제어치 0
									{
										P_HEATER_PWM_TDR = 0x0000;
										Z__PWM_TDR_00=1;  Z__PWM_TDR_72=0;
									}
									else            
									{		////20260403
									/*
											if (((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90)))
											{  	
												
												if(((gi16_HotOutSensor.DEGREE <= Z_TEMP95) || (gi16_HeaterSensor.DEGREE <= Z_TEMP95)) && gu16_HeaterPWM_TDR <=53000)
												{ 
												  P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+2000;
												}
												else if((gi16_HotOutSensor.DEGREE <= Z_TEMP98) || (gi16_HeaterSensor.DEGREE <= Z_TEMP98))
											        { 
												  P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+5;
												}
												else 
												{P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;}
											 }
											*/
											////20260403
											if((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__75) && (gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__60))
											//else if((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__75) && (gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__60) &&(gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__65) )
											{
						                                    		P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;
						                                    	}
						                                    	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__75)
											{
						                                    		if(gi16_HotInSensor.DEGREE <= Z_TEMP15){
						                                    			P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
						                                    		}
						                                    		else if(gi16_HotInSensor.DEGREE >= Z_TEMP25){
						                                    			if(F_Heater_Decrease == SET){
						                                    				P_HEATER_PWM_TDR = (U16)((U32)(gu16_HeaterPWM_TDR+1) * 4 / 10);
						                                    			}
						                                    			else{
						                                    				P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;
						                                    			}
						                                    		}
						                                    		else{
						                                    			P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;
						                                    		}
						                                    	}
						                                    	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)
											{
						                                			if(F_Heater_Decrease == SET){
						//@cjh241217                              		P_HEATER_PWM_TDR = (U16)((U32)(gu16_HeaterPWM_TDR+1) * 4 / 10);
						                                				P_HEATER_PWM_TDR = (U16)((U32)(gu16_HeaterPWM_TDR+1) * 7 / 20);		//@cjh241217
						                                			}
						                                			else{
						                                				P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;
						                                			}
						                                    	}
											/*
											else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65)
											{
						                                			if(F_Heater_Decrease == SET){
						//@cjh241217                              		P_HEATER_PWM_TDR = (U16)((U32)(gu16_HeaterPWM_TDR+1) * 4 / 10);
						                                				P_HEATER_PWM_TDR = (U16)((U32)(gu16_HeaterPWM_TDR+1) * 9 / 20);		//@cjh241217
						                                			}
						                                			else{
						                                				P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;
						                                			}
						                                    	}
											*/
											
						                                       	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50){
						                                       		P_HEATER_PWM_TDR = gu16_HeaterPWM_TDR+1;
						                                       	}
						                                    	else
											{

						                                    	}
									}
								}
#else
								P_HEATER_TRIAC = ON;
								P_HEATER_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1);
#endif
							}
						}
						break;
				}
			}
		}
	}
}

/// @brief    TEM 제어
/// @param    void
/// @return   void
static void Output_TEM(void)
{
	U8 i = 0;
	U16 TEM_TDR_Data = 0;
	U16 TEM_TDR_Data_Bojung = 0;					//@022

	if (F_TestMode_PBA == SET)
	{
		if (gu8_PBATest_ColdLoad == 3)
		{
			//P_TEM_PWM_TDR = TEM_PWM_FULL_TDR; 20260101
			 P_TEM_24V_ONOFF = OFF;
		         P_TEM_12V_ONOFF = ON;
		         P_TEM_PWM_TDR = TEM_PWM_FULL_TDR;
		}
		else if (gu8_PBATest_ColdLoad == 4)
		{
			//P_TEM_PWM_TDR = TEM_PWM_FULL_TDR; 20260101
			P_TEM_24V_ONOFF = ON;
            		P_TEM_12V_ONOFF = OFF;
            		P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
		}
		else
		{
			//P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;	//	SMPS 반전으로 Duty 반전출력 해야 함20260101
			P_TEM_24V_ONOFF = OFF;
            		P_TEM_12V_ONOFF = OFF;
            		P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if (gu8_TimerLineTest2_Cooling != 0)
		{
			//P_TEM_PWM_TDR = TEM_PWM_FULL_TDR;20260101
			P_TEM_24V_ONOFF = ON;
            		P_TEM_12V_ONOFF = OFF;
            		P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
		}
		else									//@023
		{                                       //@023
			//P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;	//@023	SMPS 반전으로 Duty 반전출력 해야 함20260101
			P_TEM_24V_ONOFF = OFF;
            		P_TEM_12V_ONOFF = OFF;
            		P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
		}                                       //@023
		
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_TEM == SET)
		{
			//P_TEM_PWM_TDR = TEM_PWM_FULL_TDR;20260101
			P_TEM_24V_ONOFF = ON;
            		P_TEM_12V_ONOFF = OFF;
            		P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
		}
		else
		{
			//P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;	//20260101
			P_TEM_24V_ONOFF = OFF;
            		P_TEM_12V_ONOFF = OFF;
            		P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
		//P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;	//20260101
		P_TEM_24V_ONOFF = OFF;
        	P_TEM_12V_ONOFF = OFF;
        	P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
	}
	
	////20240409
	else if(gu8_f_ad_feedback_mode == SET)
	{
		if(gu8_Timer_Forced_TemOutput != 0)
		{
			//P_TEM_PWM_TDR = TEM_PWM_FULL_TDR;
			P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
			P_TEM_24V_ONOFF = ON;
			P_TEM_12V_ONOFF = OFF;
		}
		else 
		{
			P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
			P_TEM_24V_ONOFF = OFF;
			P_TEM_12V_ONOFF = OFF;
		}
	}
		////20240409
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_TEM == SET)
			{
				//P_TEM_PWM_TDR = TEM_PWM_FULL_TDR;
				P_TEM_24V_ONOFF = ON;
		                P_TEM_12V_ONOFF = OFF;
		                P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
				
			}
			else
			{
				//P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
				P_TEM_24V_ONOFF = OFF;
		                P_TEM_12V_ONOFF = OFF;
		                P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
			}
		}
		else
		{
			switch (gu8_TEM_PWM_CMD)
			{
				case TEM_PWM_OFF:
				default:
					P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;	//
					gu16_TEM_Voltage = 0;
					break;
				case TEM_PWM_RAPID:
				        /*20260101
					for (i = 0 ; i < Z_TEM_Rapid_Data_Max ; i++)
					{
						if (gi16_OutDoorAirSensor.DEGREE >= TEM_Rapid_DataTable[i].Outdoor_Degree)
						{
							gu16_TEM_Voltage = TEM_Rapid_DataTable[i].TEM_Volt;
							break;
						}
						if (i == (Z_TEM_Rapid_Data_Max-1))
						{
							gu16_TEM_Voltage = TEM_Rapid_DataTable[(Z_TEM_Rapid_Data_Max-1)].TEM_Volt;
						}
					}
					*/

#ifdef _FOR_DEVELOP_
					if (gu16_TEM_Voltage_Test != 0) gu16_TEM_Voltage = gu16_TEM_Voltage_Test;
#endif
					/*20260101
					gu16_TEM_Volt = 210;			//@022

					if (F_Err43_OutDoorAirTH == SET)
					{
						gu16_TEM_Voltage = 21;
					}
					*/
					//  외기온도센서 아닌경우,
					if(F_Err43_OutDoorAirTH == CLEAR)
					{
						if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP12)
						{
							gu16_TEM_Voltage = Z_Cooling_Voltage_24V;
						}
						else
						{
							gu16_TEM_Voltage = Z_Cooling_Voltage_12V;
						}
					}
					else
					{
						//  외기온도센서인경우, 24v 고정.
						gu16_TEM_Voltage = Z_Cooling_Voltage_24V;
					}		
					
					//	급속모드 진입 후, 5분간은 12v 출력
		                        if(gu16_Timer_CoolingRapid_12V != 0)
		                        {
		                    		gu16_TEM_Voltage = Z_Cooling_Voltage_12V;
						//gu16_TEM_Voltage = Z_Cooling_Voltage_5V;
						
		                        }
					 //  Voltage에 따른, TDR 값 세팅.
					for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
					{
						if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
						{
							TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
							break;
						}
					}
#ifdef _FOR_DEVELOP_
					if (P_TEM_PWM_TDR_Test != 0) P_TEM_PWM_TDR = P_TEM_PWM_TDR_Test;
					else
#endif
					//P_TEM_PWM_TDR = TEM_TDR_Data;
					break;
				case TEM_PWM_RETAIN:	//	유지모드
				
					    if (gi16_ColdSensor.DEGREE >= Z_TEMP5)
			                    {
			                        gu16_TEM_Voltage = Z_Cooling_Voltage_12V;
			                    }
			                    else
			                    {
			                        gu16_TEM_Voltage = Z_Cooling_Voltage_5V;
			                    }

			                    //  Voltage에 따른, TDR 값 세팅.
			                    for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
			                    {
			                        if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
			                        {
			                            TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
			                            break;
			                        }
			                    }
				
					/*
				        if (F_Cooling_PID == CLEAR)		//@022
					{
						for (i = 0 ; i < Z_TEM_Retain_Data_Max ; i++)
						{
							if (gi16_OutDoorAirSensor.DEGREE >= TEM_Retain_DataTable[i].Outdoor_Degree)
							{
								gu16_TEM_Voltage = TEM_Retain_DataTable[i].TEM_Volt;
								break;
							}
							if (i == (Z_TEM_Retain_Data_Max-1))
							{
								gu16_TEM_Voltage = TEM_Retain_DataTable[(Z_TEM_Retain_Data_Max-1)].TEM_Volt;
							}
						}

						if (F_Err43_OutDoorAirTH == SET)	//	외기온도 센서 에러일 때
						{
//@014						gu16_TEM_Voltage = 7;
							gu16_TEM_Voltage = 9;			//@014
						}

						if (gi16_ColdSensor.DEGREE < Z_TEMP3)			//@017
						{                                               //@017
							F_ColdRetainMode_VoltMin = SET;             //@017
						}                                               //@017
						else if (gi16_ColdSensor.DEGREE >= Z_TEMP4)     //@017
						{                                               //@017
							F_ColdRetainMode_VoltMin = CLEAR;           //@017
						}                                               //@017
						else                                            //@017
						{                                               //@017
																		//@017
						}                                               //@017

						if (F_ColdRetainMode_VoltMin == SET)            //@017
						{                                               //@017
							gu16_TEM_Voltage = 4;                       //@017
						}                                               //@017

						if (F_Cooling_Up == SET)			//	특수모드 냉각성능 Up
						{
//@010						gu16_TEM_Voltage += 2;
							gu16_TEM_Voltage += Z_CoolingUp_Volatage;			//@010
						}

#ifdef _FOR_DEVELOP_
						if (gu16_TEM_Voltage_Test != 0) gu16_TEM_Voltage = gu16_TEM_Voltage_Test;
#endif

						for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
						{
							if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
							{
								TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
								break;
							}
						}
#ifdef _FOR_DEVELOP_
						if (P_TEM_PWM_TDR_Test != 0) P_TEM_PWM_TDR = P_TEM_PWM_TDR_Test;
						else
#endif
						//P_TEM_PWM_TDR = TEM_TDR_Data;
					}
					//@022	시작
					else							//	냉각 PI 제어
					{
//미적용★★						if (gu16_TimerForced_RapidCooling != 0)	//	냉수 추출 후 유지모드에서 강제 급속 전압 동작 타이머 있는 경우
//						{
//							gu16_TEM_Voltage = 21;
//							gu8_TEM_Voltage_BoJung = 0;
//						}
//						else
//						{
							gu16_TEM_Voltage = gu16_TEM_Volt / 10;
							gu8_TEM_Voltage_BoJung = (U8)(gu16_TEM_Volt % 10);
//						}

#ifdef _FOR_DEVELOP_
						if (gu16_TEM_Voltage_Test != 0) gu16_TEM_Voltage = gu16_TEM_Voltage_Test;
#endif
						if (F_ColdRetainMode_VoltMin == SET)
						{
							gu16_TEM_Voltage = 4;
						}

						for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
						{
							if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
							{
								TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
								if (i > 0)
								{
									TEM_TDR_Data_Bojung = TEM_Voltage_DataTable[i-1].TEM_TDR;
								}
								break;
							}
						}

						TEM_TDR_Data = TEM_TDR_Data - (((TEM_TDR_Data - TEM_TDR_Data_Bojung) / 10) * gu8_TEM_Voltage_BoJung);
#ifdef _FOR_DEVELOP_
						if (P_TEM_PWM_TDR_Test != 0) P_TEM_PWM_TDR = P_TEM_PWM_TDR_Test;
						else
#endif
						P_TEM_PWM_TDR = TEM_TDR_Data;
						//@022	끝
					}
					*/
					break;
				case TEM_PWM_ECOMODE:
					if (gi16_ColdSensor.DEGREE >= Z_TEMP11)
			                    {
			                        gu16_TEM_Voltage = Z_Cooling_Voltage_12V;
			                    }
			                    else
			                    {
			                        gu16_TEM_Voltage = Z_Cooling_Voltage_5V;
			                    }

			                    //  Voltage에 따른, TDR 값 세팅.
			                    for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
			                    {
			                        if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
			                        {
			                            TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
			                            break;
			                        }
			                    }
				        /*
					if (gi16_ColdSensor.DEGREE >= Z_TEMP15)
					{
						gu16_TEM_Voltage = 17;
					}
					else if (gi16_ColdSensor.DEGREE >= Z_TEMP11)
					{
						gu16_TEM_Voltage = 15;
					}
					else if (gi16_ColdSensor.DEGREE >= Z_TEMP7)
					{
						gu16_TEM_Voltage = 9;
					}
					else
					{
						gu16_TEM_Voltage = 6;
					}

					if (F_Cooling_Up == SET)			//@010	특수모드 냉각성능 Up
					{									//@010
						gu16_TEM_Voltage += Z_CoolingUp_Volatage;			//@010
					}									//@010

					for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
					{
						if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
						{
							TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
							break;
						}
					}
#ifdef _FOR_DEVELOP_
					if (P_TEM_PWM_TDR_Test != 0) P_TEM_PWM_TDR = P_TEM_PWM_TDR_Test;
					else
#endif
					P_TEM_PWM_TDR = TEM_TDR_Data;
					*/
					break;
				case TEM_PWM_ECO_9V:
					
					if (gi16_ColdSensor.DEGREE >= Z_TEMP9)
			                    {
			                        gu16_TEM_Voltage = Z_Cooling_Voltage_12V;
			                    }
			                    else
			                    {
			                        gu16_TEM_Voltage = Z_Cooling_Voltage_5V;
			                    }

			                    if (F_Cooling_Up == SET)            //  특수모드 냉각성능 Up
			                    {
			                        gu16_TEM_Voltage += Z_CoolingUp_Volatage;
			                    }

			                    for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
			                    {
			                        if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
			                        {
			                            TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
			                            break;
			                        }
			                    }
				
					
				        /*
					gu16_TEM_Voltage = 9;

					if (F_Cooling_Up == SET)			//@010	특수모드 냉각성능 Up
					{                                   //@010
						gu16_TEM_Voltage += Z_CoolingUp_Volatage;		//@010
					}                                   //@010

					for (i = 0 ; i < Z_TEM_Voltage_Data_Max ; i++)
					{
						if (gu16_TEM_Voltage == TEM_Voltage_DataTable[i].TEM_Volt)
						{
							TEM_TDR_Data = TEM_Voltage_DataTable[i].TEM_TDR;
							break;
						}
					}
#ifdef _FOR_DEVELOP_
					if (P_TEM_PWM_TDR_Test != 0) P_TEM_PWM_TDR = P_TEM_PWM_TDR_Test;
					else
#endif
					P_TEM_PWM_TDR = TEM_TDR_Data;
					*/
					break;
				case TEM_PWM_MIN:
	//				P_TEM_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1) - (U32)(_DABF_TAU_TDR02_VALUE+1) * 34 / 240 * 8 / 10;
//					P_TEM_PWM_TDR = (_DABF_TAU_TDR02_VALUE+1) - (TEM_PWM_RMS_CAL * 34);	//	3.4V
					
					P_TEM_PWM_TDR = TEM_Voltage_DataTable[(Z_TEM_Voltage_Data_Max-1)].TEM_TDR;
					
					gu16_TEM_Voltage = TEM_Voltage_DataTable[(Z_TEM_Voltage_Data_Max-1)].TEM_Volt;
                    			TEM_TDR_Data = TEM_Voltage_DataTable[(Z_TEM_Voltage_Data_Max-1)].TEM_TDR;
					
					break;
			}
		
		
		 //        gu16_TEM_Voltage =  gu16_TEM_Voltage_Test;

            /*  1) 24V → 5 ~ 12V PWM 출력 전환
            *  2) 5 ~ 12V ~ 24V 출력 전환
            *    → 상기 2가지 조건에서 일 경우, 최소 1ms 출력 딜레이 필요(전장회로개발팀 확인)
            */
            //  출력 값 최소 1분 유지.
            if((gu8_CoolingStep >= Z_CoolingStep_20) && (gu8_CoolingStep <= Z_CoolingStep_24))
            {
                if(gu16_TEM_Voltage != gu16_TEM_Voltage_Backup)
                {
                    if(gu8_Timer_TEM_Voltage_Retain == 0)
                    {
                        gu8_Timer_TEM_Voltage_Retain = 60;
                    }
                    else
                    {
                        gu16_TEM_Voltage = gu16_TEM_Voltage_Backup;
                        TEM_TDR_Data = gu16_TEM_TDR_Data_Backup;
                    }
                }

                gu16_TEM_Voltage_Backup = gu16_TEM_Voltage;                     //  출력 전압 값 backup
                gu16_TEM_TDR_Data_Backup = TEM_TDR_Data;
            }
            else
            {
                gu8_Timer_TEM_Voltage_Retain = 0;

                gu16_TEM_Voltage_Backup = 0;
                gu16_TEM_TDR_Data_Backup = 0;
            }
            //  Voltage에 따른, 열전모듈 출력
            /*  1) 24V 출력 :
            *       - TEM ON/OFF 포트 : ON
            *       - TEM 12V ON/OFF 포트 : OFF
            *  2) 5 ~ 12V PWM :
            *
            *
            */
            if(gu16_TEM_Voltage == Z_Cooling_Voltage_24V)
            {
                P_TEM_24V_ONOFF = ON;

                if(gu8_Timer_TEM_12V_OffDelay == 0)
                {
                    P_TEM_12V_ONOFF = OFF;
                }
                P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
                gu8_Timer_TEM_24V_OffDelay = 100;
            }
            else if((gu16_TEM_Voltage >= Z_Cooling_Voltage_5V) && (gu16_TEM_Voltage <= Z_Cooling_Voltage_12V))
            {
                P_TEM_12V_ONOFF = ON;
                P_TEM_PWM_TDR = TEM_TDR_Data;
                if(gu8_Timer_TEM_24V_OffDelay == 0)
                {
                    P_TEM_24V_ONOFF = OFF;
                }
                else
                {
                    P_TEM_24V_ONOFF = ON;
                }

                gu8_Timer_TEM_12V_OffDelay = 100;
            }
            else
            {
                P_TEM_24V_ONOFF = OFF;
                P_TEM_12V_ONOFF = OFF;
                P_TEM_PWM_TDR = TEM_PWM_OFF_TDR;
                gu16_TEM_Voltage = 0;
            }
	}
		
		
		
		
		
    }
}

/// @brief    누수센서 Pulse 출력 (1ms 주기)
/// @param    void
/// @return   void
static void Output_LeakSensor(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (F_PBATest_LeakSensorOFF == SET)
		{
			P_LEAKSENSOR_OFF = HIGH;
		}
		else
		{
			P_LEAKSENSOR_OFF = LOW;
		}
	}
	else
	{
		P_LEAKSENSOR_OFF = LOW;
	}
//	if (gu8_LeakSensor_CMD == ON)
//	{
//		P_LEAKSENSOR_ONOFF = ON;
//	}
//	else
//	{
//		P_LEAKSENSOR_ONOFF = OFF;
//	}
//	P_LEAKSENSOR_ONOFF ^= 1;						//	1ms 마다 반전 출력 (※ 스케일링?)
}

/// @brief    음성 리셋 포트
/// @param    void
/// @return   void
static void Output_VoiceReset(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (F_PBATest_VoiceReset == SET)
		{
			P_VOICE_RESET = LOW;
		}
		else
		{
			P_VOICE_RESET = HIGH;
		}
	}
}

/// @brief    FAN 제어
/// @param    void
/// @return   void
static void Output_FAN(void)
{
	U8 mu8_temp = 0;
	static U8 flag_PL_Prevent = CLEAR;	//@025	SMPS FET 쇼트로 인한 TEM과열 방지를 위해 Fan 가동 플래그 220929

	if (F_TestMode_PBA == SET)
	{
		switch (gu8_PBATest_FanTest)
		{
			case 0:
			default:
				P_FAN_18V = OFF;
				P_FAN_21V = OFF;
#ifndef _FAN_CIRCUIT_CHANGE_
				P_FAN_ONOFF = OFF;
#endif
				break;
			case 1:
				P_FAN_18V = OFF;
				P_FAN_21V = ON;
#ifndef _FAN_CIRCUIT_CHANGE_
				P_FAN_ONOFF = ON;
#endif
				break;
			case 2:
				P_FAN_18V = ON;
				P_FAN_21V = ON;
#ifndef _FAN_CIRCUIT_CHANGE_
				P_FAN_ONOFF = ON;
#endif
				break;
#ifndef _FAN_CIRCUIT_CHANGE_
			case 3:
				P_FAN_18V = ON;
				P_FAN_21V = ON;
				P_FAN_ONOFF = OFF;
				break;
#endif
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if (F_LineTest2_CoolingResult_OK == SET)	//@023	냉각 검사가 완료되면 팬 가동
		{                                           //@023
			P_FAN_18V = OFF;                        //@023
			P_FAN_21V = ON;                         //@023
		}                                           //@023
		else                                        //@023
		{                                           //@023
			P_FAN_18V = OFF;
			P_FAN_21V = OFF;
		}											//@023
#ifndef _FAN_CIRCUIT_CHANGE_
		P_FAN_ONOFF = OFF;
#endif
	}
	else if (F_TestMode_LineAuto == SET)
	{
		if (F_LineTest_Fan == SET)
		{
#ifndef _FAN_CIRCUIT_CHANGE_
			P_FAN_ONOFF = ON;
#endif
			P_FAN_21V = ON;
		}
		else
		{
#ifndef _FAN_CIRCUIT_CHANGE_
			P_FAN_ONOFF = OFF;
#endif
			P_FAN_21V = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
#ifndef _FAN_CIRCUIT_CHANGE_
		P_FAN_ONOFF = ON;
#endif
		P_FAN_21V = ON;
//		P_FAN_18V = OFF;
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
			if (F_SmartCheck_Fan == SET)
			{
#ifndef _FAN_CIRCUIT_CHANGE_
				P_FAN_ONOFF = ON;
#endif
				P_FAN_18V = OFF;
				P_FAN_21V = ON;
			}
			else
			{
#ifndef _FAN_CIRCUIT_CHANGE_
				P_FAN_ONOFF = OFF;
#endif
				P_FAN_18V = OFF;
				P_FAN_21V = OFF;
			}
		}
		else
		{
			switch (gu8_FAN_CMD)
			{
				case FAN_OFF:
				default:
#ifndef _FAN_CIRCUIT_CHANGE_
					P_FAN_ONOFF = OFF;
#endif
					    if (gi16_HeatSinkSensor.DEGREE > Z_TEMP80)
			                    {
			                        flag_PL_Prevent = SET;
			                    }
			                    else if (gi16_HeatSinkSensor.DEGREE <= Z_TEMP60)
			                    {
			                        flag_PL_Prevent = CLEAR;
			                    }
			                    else
			                    {

			                    }

			                    if (flag_PL_Prevent == SET)
			                    {
			                        if (gu8_TimerFanDelay == 0)
			                        {
			                            P_FAN_18V = OFF;
			                            P_FAN_21V = ON;
			                        }
			                        else
			                        {
			                            P_FAN_18V = OFF;
			                            P_FAN_21V = OFF;
			                        }
			                    }
			                    else
			                    {
			                        P_FAN_18V = OFF;
			                        P_FAN_21V = OFF;
			                        gu8_TimerFanDelay = Z_TimerFanDelay;
			                    }
					break;
				case FAN_18V_ON:

					 if (gu8_TimerFanDelay == 0)
			                 {
			                        if(gu8_CoolingStep == Z_CoolingStep_21)
			                        {
							if(gi16_ColdSensor.DEGREE >= Z_TEMP5)
							{
			                                P_FAN_18V = ON;
			                                P_FAN_21V = ON;
							}
							else
							{
								if(gi16_OutDoorAirSensor.DEGREE >= Z_TEMP20)
								{
									P_FAN_18V = ON;
									P_FAN_21V = ON;
								}
								else
								{
									P_FAN_18V = OFF;
									P_FAN_21V = OFF;
								}
							}
			                        }
			                        else
			                        {
			                            P_FAN_18V = ON;
			                            P_FAN_21V = ON;                            
			                        }
			                }		
					break;
				case FAN_21V_ON:
#ifndef _FAN_CIRCUIT_CHANGE_
					P_FAN_ONOFF = ON;
#endif
					if (gu8_TimerFanDelay == 0)
					{
						if (F_FAN_NoiseClear == SET)	//	특수모드 팬 전압 하강
						{
//@009							
							P_FAN_18V = ON;			//@009		특수모드 설정 경우 급속모드도 18V로 변경
							P_FAN_21V = ON;			//@009
						}
						else
						{
							P_FAN_18V = OFF;
							P_FAN_21V = ON;
						}
					}
					break;
			}

		}
	}
}

/// @brief
/// @param    void
/// @return   void
static void Output_ColdSensor_ONOFF(void)
{
	if ((F_TestMode.byte != 0x00) || (F_Err44_ColdTH == SET))
	{
		F_ColdSensor_OFF = CLEAR;					//		상시 ON
		gu16_TimerColdSensor_ON = Z_TimerColdSensor_ON;
	}

	if ((F_ColdSensor_OFF == CLEAR) && (gu16_TimerColdSensor_ON == 0))	//	ON상태 및 센서ON 타이머 경과 된 경우
	{
		gu16_TimerColdSensor_OFF = Z_TimerColdSensor_OFF;	//		OFF 타이머 set
		F_ColdSensor_OFF = SET;							//		센서 읽기 OFF
	}

	if ((F_ColdSensor_OFF == SET) && (gu16_TimerColdSensor_OFF == 0) && (F_ColdOff == CLEAR))	//	OFF상태 및 센서OFF 타이머 경과 된 경우 및 냉각ON상태만
	{
		gu16_TimerColdSensor_ON = Z_TimerColdSensor_ON;		//		ON 타이머 set
		if (F_TestMode.byte == 0x00)
		{
			gu8_TimerColdSensor_Sensing = Z_TimerColdSensor_Sensing;
		}
		F_ColdSensor_OFF = CLEAR;							//		센서 읽기 ON
	}

	if (F_TestMode_PBA == SET)						//	PBA제조업체 검사모드 인 경우
	{
		if (F_PBATest_ColdSensorOFF == SET)
		{
			P_COLDSENSOR_ON = LOW;
		}
		else
		{
			P_COLDSENSOR_ON = HIGH;
		}
	}
	else											//	일반 모드 or 공장자동화
	{
		if (F_ColdSensor_OFF == CLEAR)				//		센서 읽기 ON 상태인 경우
		{
			P_COLDSENSOR_ON = HIGH;
		}
		else
		{
			P_COLDSENSOR_ON = LOW;
		}
	}
}

/// @brief
/// @param    void
/// @return   void
static void Output_FCT_TxRx(void)
{
	if (F_TestMode_PBA == SET)
	{
		if (P_FCT_Rx == LOW)
		{
			P_FCT_Tx = HIGH;
		}
		else
		{
			P_FCT_Tx = LOW;
		}
	}
}

static void Output_UV(void)
{
#ifdef	_FOR_UV_ENDURANCE_TEST_
	gu8_UV_CMD = SET;
#else
#ifdef _FRONT_JIG_PROGRAM_
	if (F_FrontTest_EffluentButton == SET)
	{
		gu8_UV_CMD = SET;
	}
	return;
#else
	if ((gu8_SmartCheckStep & 0xF0) == 0x20)
	{
		if (F_SmartCheck_UV == SET)
		{
			gu8_UV_CMD = SET;
		}
		else
		{
			gu8_UV_CMD = CLEAR;
		}
	}
	else
	{
		if (gu16_TimerUV_Oper != 0)
		{
			gu8_UV_CMD = SET;
		}
		else
		{
			gu8_UV_CMD = CLEAR;
		}
	}
#endif
#endif
}

/// @brief
/// @param    void
/// @return   void

static void Output_Power(void)
{
	/*20260101
	if (F_TestMode_PBA == SET)
	{
		if (F_PBATest_StandBy_OFF == SET)
		{
#ifndef _SUNRINTECH_SMPS_
			P_STANDBY = OFF;
#else
			P_STANDBY = N_ON;
#endif
			P_TEM_ONOFF = OFF;
		}
		else
		{
#ifndef _SUNRINTECH_SMPS_
			P_STANDBY = ON;
#else
			P_STANDBY = N_ON;
#endif
			if (gu8_PBATest_ColdLoad == 4)			//
			{
				P_TEM_ONOFF = OFF;					//
			}
			else
			{
				P_TEM_ONOFF = ON;
			}
		}
	}
	else if (F_TestMode_LineTest2 == SET)
	{
		if (gu8_TimerLineTest2_Cooling != 0)
		{
#ifndef _SUNRINTECH_SMPS_
			P_STANDBY = ON;
#else
			P_STANDBY = N_ON;
#endif
			P_TEM_ONOFF = ON;
		}
	}
	else if (F_TestMode_LineAuto == SET)
	{
#ifndef _SUNRINTECH_SMPS_
		P_STANDBY = ON;
#else
		P_STANDBY = N_ON;
#endif
		if (F_LineTest_TEM == SET)
		{
			P_TEM_ONOFF = ON;
		}
		else
		{
			P_TEM_ONOFF = OFF;
		}
	}
	else if (F_TestMode_LineFront == SET)
	{
#ifndef _SUNRINTECH_SMPS_
		P_STANDBY = ON;
#else
		P_STANDBY = N_ON;
#endif
		P_TEM_ONOFF = ON;
	}
	else
	{
		if ((gu8_SmartCheckStep & 0xF0) == 0x20)
		{
#ifndef _SUNRINTECH_SMPS_
			P_STANDBY = ON;
			P_TEM_ONOFF = ON;
#else
			P_STANDBY = N_ON;
#endif
		}
		else
		{
			if (((F_ColdOff == CLEAR) && (gu8_TimerBoot_After1s == 0)) || (gu8_OperationStep != Z_OperationStep_20) || (gu8_UV_CMD == SET))
			{
#ifndef _SUNRINTECH_SMPS_
				P_STANDBY = ON;
#else
				P_STANDBY = N_ON;
#endif
			}
			else
			{
				P_STANDBY = OFF;
				gu8_TimerStanbyOn_After1s = Z_TimerStanbyOn_After1s;
			}

			if ((P_STANDBY == ON) && (gu8_TimerStanbyOn_After1s == 0))
			{
				switch (gu8_TEM_PWM_CMD)
				{
					case TEM_PWM_OFF:
					default:
						P_TEM_ONOFF = OFF;
						break;
					case TEM_PWM_RAPID:
					case TEM_PWM_RETAIN:
					case TEM_PWM_MIN:
					case TEM_PWM_ECOMODE:
					case TEM_PWM_ECO_9V:
						P_TEM_ONOFF = ON;
						break;
				}
			}
			else
			{
				P_TEM_ONOFF = OFF;
			}
		}
	}
	*/
}
