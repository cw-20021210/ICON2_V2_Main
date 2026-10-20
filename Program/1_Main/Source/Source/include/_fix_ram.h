#ifdef FIX_RAM_C
	#define	EXTERN
	#define	GLOBAL_VARIABLE_DEFINE
#else
	#define	EXTERN extern
#endif

/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_ErrorHistory			(5)					//	에러이력 갯수

//***************************************
//*              adc.c		            *
//***************************************


//***************************************
//*              cold.c		            *
//***************************************


//***************************************
//*              command.c		        *
//***************************************


//***************************************
//*              display.c		        *
//***************************************



//***************************************
//*              eeprom.c		        *
//***************************************
EXTERN U8 F_EEPROM_CommunicationError;     			// 	EEPROM 통신 에러시 표시 (PBA 검사 시)

//***************************************
//*              pure.c                 *
//***************************************
EXTERN U8 F_FirstHotEffluent;          				// 	부팅 후 온수 추출 첫잔 Flag (CLEAR:미완료 / SET:완료)

EXTERN U8 gu8_WaterSelect;               			// 	냉수 : 0, 정수 : 1, 온수 : 2
EXTERN U8 gu8_DefaultWaterSelect;					//	물 선택 Default

//EXTERN U8 gu8_CupSize;								//	Cup Size
//EXTERN U8 gu8_DefaultCupSize;            			// 	컵 사이즈 default값
EXTERN U16 gu16_CupSize;								//	Cup Size
EXTERN U8 gu8_DefaultCupSize;            			// 	컵 사이즈 default값

EXTERN volatile U16 gu16_Effluent_Hz;             	// 	[1/1Hz] 온수 정량 추출동작 Hz(추출동작시 Flow Meter를 통해 Hz 체크)
EXTERN U16 gu16_FirstHotEff_Hz;						//	온수 첫잔 Hz
EXTERN U16 gu16_TimerEffluent;           			// 	연속 추출 시 추출 시간 Timer	<1800s/100ms>
EXTERN U8 F_FirstHotEff_HeaterSensing;				//	온수 첫잔 중 히터센싱 완료 flag



EXTERN U8 F_100ms_for_FlowMeter_Average;			//	유량센서 업데이트 100ms
EXTERN U8 F_Pure_First;								//	부팅 후 첫회 추출 이후 인 경우 (유량변화 20%이상 체크를 위함)
EXTERN U16 gu16_FlowDecide_W_Ave;					//	현재추출 평균 값
EXTERN U32 gu32_FlowDecide_W_Ave_Sum;				//	평균 유량 합산 값
EXTERN U16 gu16_FlowDecide_W_Ave_Cnt;				//	평균 유량 카운터
EXTERN U16 gu16_FlowDecide_W_Ave_Pure_Standard;		//	평균유량 기준 값

//***************************************
//*              error.c		        *
//***************************************
EXTERN U8 gu8_ErrorCode;							//	에러 코드

EXTERN U8 gu8_ErrorHistory[Z_ErrorHistory];			//	에러이력 저장

//***************************************
//*              flowmeter.c	        *
//***************************************
EXTERN U16 gu16_FlowDecide_Hz_forLineAuto;			//	자동화 검사 피드백을 위한 데이터 저장 [1/10Hz]
EXTERN U16 gu16_FlowDecide_Hz;						//	유량 Hz변환		[1/10Hz]
EXTERN U16 gu16_FlowDecide_W;						//	유량 LPM 변환	[1/100LPM]
EXTERN U16 gu16_FlowMeter_Standard;
EXTERN U16 gu16_FlowMeter_Standard_forHotWater;
//EXTERN U16 gu16_FlowMeter_Standard_forHotWater_forJapan;

EXTERN U16 gu16_FlowDecide_W_HotEffMax;

//***************************************
//*              hot.c		            *
//***************************************
EXTERN U8 gu8_HotStep;								//	온수 스텝
	#define gu8_HotStep_hi	(gu8_HotStep & 0xf0)
	#define gu8_HotStep_lo	(gu8_HotStep & 0x0f)

EXTERN U16 gu16_HeaterFull_WATT;					//	히터 최대출력 시 WATT저장
EXTERN U16 gu16_HeaterRELAY_WATT;					//	히터 최대출력 대비 RELAY WATT저장
EXTERN U16 gu16_StepCount_BeforeEff;				//	이전 추출 시에 스텝 값
EXTERN U16 gu16_FlowDecide_W_BeforeEff;				//	이전 추출 시에 유량 값
EXTERN U16 gu16_Drain_Flow;							//	스텝 이동 후 드레인 유량 값
//EXTERN U8 F_StartFlow_ReCalibration;				//	유량 리캘리브레이션 필요 flag (첫 회 추출시보다 80%이하로 유량 감소 했을 때)
EXTERN U16 gu16_HeaterPWM_TDR;
EXTERN U16 gu16_HeaterPWM_TDR_Test;

EXTERN I16 gi16_EffStartTargetTemp;					//	예열 중 추출시작  목표온도
//EXTERN I16 gi16_EffStartTargetTemp2;				//	두번째잔 추출시작  목표온도

EXTERN U16 gu16_HotEffStart_Standard;
EXTERN U16 gu16_HotEffStart_DeltaT;
EXTERN U16 gu16_HotEffStart_First;
EXTERN U8 gu8_HotPreHeatingStep;

EXTERN U16 gu16_Heater_Di1;							//	히터 제어 I제어 기준값 (누적 감가산)
EXTERN U16 gu16_Heater_Dp1;							//	히터 제어 P제어 기준값 (기울기)
EXTERN U16 gu16_Heater_Dp2;							//	히터 제어 P제어 기준값 (Max)
EXTERN I16 gi16_Heater_Di_ini;						//	히터 제어 I제어 기준값 (I누적 시작 데이터)

EXTERN I32 gi32_Heater_P;							//	히터 제어 P값
EXTERN I32 gi32_Heater_I;							//	히터 제어 I값
EXTERN I32 gi32_Heater_P_I;							//	히터 제어 P+I값

EXTERN U16 gu16_HeaterWATT;							//	히터제어 전력 값 [1W]
EXTERN U16 gu16_HeaterWATT_forCal;					//	전력 값 [1W]
//EXTERN U8 F_OverHeating;							//	10분간 7분 연속 온수사용 확정 플래그

EXTERN U8 F_Flow_Rapidly_Decrease;					//	온수 추출 중 유량 급격 감소 체크 flag
EXTERN U16 gu16_Flow_Buf[10];						//	온수 추출 중 현재유량 저장

EXTERN U8 gu8_My_HotTemp;							//	app에서 설정한 my 온도
//***************************************
//*              input.c		        *
//***************************************


//***************************************
//*              keycontrol.c		    *
//***************************************
EXTERN U8 gu8_KeyControlStep;						//	Key 제어 스텝

//EXTERN ITTI3 Struct_KeyData;						//	3회 일치 후 키 데이터
//	#define KEY_AMOUNT_LEVEL	(Struct_KeyData.Level.Bit.b0)
//	#define KEY_COLDWATER_LEVEL	(Struct_KeyData.Level.Bit.b1)
//	#define KEY_PUREWATER_LEVEL	(Struct_KeyData.Level.Bit.b2)
//	#define KEY_HOTWATER_LEVEL	(Struct_KeyData.Level.Bit.b3)
//	#define KEY_EFFLUENT_LEVEL	(Struct_KeyData.Level.Bit.b4)

//EXTERN volatile BYTE_BIT gu8_TouchData;				//	터치 데이터
//	#define TOUCH_AMOUNT		(gu8_TouchData.Bit.b0)
//	#define TOUCH_COLDWATER		(gu8_TouchData.Bit.b1)
//	#define TOUCH_PUREWATER		(gu8_TouchData.Bit.b2)
//	#define TOUCH_HOTWATER		(gu8_TouchData.Bit.b3)

//EXTERN WORD_BIT gu16_ShortKeyArray;					//	단키 확정
//	#define KEY_AMOUNT_SHORT		(gu16_ShortKeyArray.BIT.b0)
//	#define KEY_COLDWATER_SHORT		(gu16_ShortKeyArray.BIT.b1)
//	#define KEY_PUREWATER_SHORT		(gu16_ShortKeyArray.BIT.b2)
//	#define KEY_HOTWATER_SHORT		(gu16_ShortKeyArray.BIT.b3)
//	#define KEY_EFFLUENT_SHORT		(gu16_ShortKeyArray.BIT.b4)
//	#define KEY_ALLLOCK_SHORT		(gu16_ShortKeyArray.BIT.b5)
//	#define KEY_TH_CONFIRM_SHORT	(gu16_ShortKeyArray.BIT.b6)
//	#define KEY_FCT_TESTMODE_SHORT	(gu16_ShortKeyArray.BIT.b7)
//	#define KEY_EEPROMCLEAR_SHORT	(gu16_ShortKeyArray.BIT.b8)
//	#define KEY_FLUSHING_CANCLE_SHORT	(gu16_ShortKeyArray.BIT.b9)
//	#define KEY_FCT_TESTMODE_KOREA_SHORT	(gu16_ShortKeyArray.BIT.b10)
//	#define KEY_KIT_MODE_SHORT	(gu16_ShortKeyArray.BIT.b11)
//	#define KEY_ERROR_HISTORY_SHORT	(gu16_ShortKeyArray.BIT.b12)
//	#define KEY_COLD_TIME_SHORT	(gu16_ShortKeyArray.BIT.b13)
//	#define KEY_SILENT_SHORT	(gu16_ShortKeyArray.BIT.b14)

EXTERN LONG_BIT gu32_LongKeyArray;					//	롱키 확정
	#define KEY_HOTWATER_LONG			(gu32_LongKeyArray.BIT.b0)
	#define KEY_COLDWATER_LONG			(gu32_LongKeyArray.BIT.b1)
	#define KEY_PUREWATER_LONG			(gu32_LongKeyArray.BIT.b2)
	#define KEY_AMOUNT_LONG				(gu32_LongKeyArray.BIT.b3)
	#define KEY_EFFLUENT_LONG			(gu32_LongKeyArray.BIT.b4)
	#define KEY_LINETEST_MANAGE_LONG		(gu32_LongKeyArray.BIT.b5)
	#define KEY_COLDTANK_PBATEST_LONG		(gu32_LongKeyArray.BIT.b6)
	#define KEY_ERROR_HIS_LONG			(gu32_LongKeyArray.BIT.b7)
	#define KEY_ALL_LOCK_LONG			(gu32_LongKeyArray.BIT.b8)
	#define	KEY_WIFI_AP_PAIRING_LONG		(gu32_LongKeyArray.BIT.b9)
	#define	KEY_WIFI_ONOFF_LONG			(gu32_LongKeyArray.BIT.b10)
	#define	KEY_FLU_CAN_TH_CON_LONG			(gu32_LongKeyArray.BIT.b11)
	#define KEY_SLEEPMODE_LONG			(gu32_LongKeyArray.BIT.b12)
	#define KEY_STE_KITMODE_LONG			(gu32_LongKeyArray.BIT.b13)
	#define KEY_SOUND_TYPE_LONG			(gu32_LongKeyArray.BIT.b14)
	#define KEY_VALUE_TEM_SMPS_TEST_LONG			(gu32_LongKeyArray.BIT.b15)

EXTERN LONG_BIT gu32_VeryLongKeyArray;				//	베리롱키 확정
	#define KEY_HOTWATER_VERYLONG			(gu32_VeryLongKeyArray.BIT.b0)
	#define KEY_EFFLUENT_VERYLONG			(gu32_VeryLongKeyArray.BIT.b4)
	#define KEY_LINETEST_MANAGE_VERYLONG		(gu32_VeryLongKeyArray.BIT.b5)
	#define KEY_COLDTANK_PBATEST_VERYLONG		(gu32_VeryLongKeyArray.BIT.b6)


EXTERN LONG_BIT gu32_FCTLongKeyArray;				//	테스트모드 롱키 확정
//	#define KEY_HOTWATER_LONG			(gu32_FCTLongKeyArray.BIT.b0)
//	#define KEY_COLDWATER_LONG			(gu32_FCTLongKeyArray.BIT.b1)
//	#define KEY_PUREWATER_LONG			(gu32_FCTLongKeyArray.BIT.b2)
//	#define KEY_AMOUNT_LONG				(gu32_FCTLongKeyArray.BIT.b3)
//	#define KEY_EFFLUENT_LONG			(gu32_FCTLongKeyArray.BIT.b4)
	#define KEY_LINETEST_MANAGE_FCTLONG	(gu32_FCTLongKeyArray.BIT.b5)	//@023
//	#define KEY_COLDTANK_PBATEST_LONG	(gu32_FCTLongKeyArray.BIT.b6)
//	#define KEY_ERROR_HIS_LONG			(gu32_FCTLongKeyArray.BIT.b7)
//	#define KEY_ALL_LOCK_LONG			(gu32_FCTLongKeyArray.BIT.b8)
	#define	KEY_WIFI_AP_PAIRING_FCTLONG	(gu32_FCTLongKeyArray.BIT.b9)	//@023
//	#define	KEY_WIFI_ONOFF_LONG			(gu32_FCTLongKeyArray.BIT.b10)
	#define	KEY_FLU_CAN_TH_CON_FCTLONG	(gu32_FCTLongKeyArray.BIT.b11)	//@023
//	#define KEY_SLEEPMODE_LONG			(gu32_FCTLongKeyArray.BIT.b12)
//	#define KEY_STE_KITMODE_LONG		(gu32_FCTLongKeyArray.BIT.b13)
//	#define KEY_SOUND_TYPE_LONG			(gu32_FCTLongKeyArray.BIT.b14)


//***************************************
//*               macro.c		        *
//***************************************


//***************************************
//*               main.c                *
//***************************************


//***************************************
//*               operation.c           *
//***************************************
EXTERN U8 gu8_OperationStep;						//	운전상태
	#define gu8_OperationStep_hi	(gu8_OperationStep & 0xf0)
	#define gu8_OperationStep_lo	(gu8_OperationStep & 0x0f)

EXTERN U8 F_HotLock;                   				// 	온수 잠금 Flag

EXTERN U8 F_ColdOff;
EXTERN U8 F_ColdOff_Display;						//	냉각 OFF 표시 flag

EXTERN U8 F_SleepMode;
EXTERN U8 F_SleepMode_Display;

EXTERN U8 F_FOTA_Setup;
EXTERN U8 F_SelfService;							//	[0:방문관리] [1:자가관리]
EXTERN U8 F_First_Set;								//	최초 설치
EXTERN U8 F_FirstFlushing_AirPurge;					//	최초 설치 질소 퍼지 유무
EXTERN U8 F_Cooling_Up;								//	유지모드 냉각 성능 Up
EXTERN U8 F_Cooling_PID;							//@022	냉각 PI제어
//EXTERN U8 F_Filter_Alram_OFF;
EXTERN U8 gu8_DefaultHotTempSetup;            		// 	온수 디폴트 설정 온도
EXTERN U8 gu8_HotTempSetup;            				// 	온수 설정 온도
EXTERN U8 F_HotWater_100Degree;						//	100도 사용여부
EXTERN U8 F_HotTemp_wifi_Setting;
EXTERN U8 F_HotWater_DegreeDisplay;					//	온수 추출 시 온도 표시 (1℃ 단위)
EXTERN U8 F_HotWater_DegreeDisplay_TH;				//	온수 추출 시 온도 표시 (0:출수센서 / 1:히터센서)
EXTERN U8 F_FAN_NoiseClear;							//	팬 유지모드 21V → 18V 변경
EXTERN U8 F_TEMP_A_SMPS_Voltage_Display;			//	SMPS 전압 표시
EXTERN U8 F_Eff_Continue_Heater_Off;


EXTERN U8 F_FirstFlushing;							//	초기 플러싱 flag (CLEAR:플러싱 필요 / SET:플러싱 완료)
EXTERN U8 F_Heater_Fill;							//	히터  물 채우기 (SET:완료 /  CLEAR:미완료)
EXTERN U8 F_ColdTank_Fill;							//	냉수탱크 물 채우기 (SET:완료 /  CLEAR:미완료)

EXTERN U8 gu8_TimerWaterSelect;          			// 	냉수, 온수 설정 시간 타이머		<7s/100ms>
EXTERN U8 gu8_TimerCupSelect;						//	컵 셀렉트 타이머		<7s/100ms>

EXTERN U8 F_AllLock;								//	전체잠금

//JHKO_260417
EXTERN U8 gu8_WifiPairingHistory; // 0: 미페어링(최초), 1: 페어링 성공 이력 있음

//***************************************
//*               output.c              *
//***************************************

//***************************************
//*               pba.c          	    *
//***************************************
EXTERN U8 gu8_PBAStep;								//	PBA 검사 스텝


EXTERN BYTE_BIT F_TestMode;
	#define F_TestMode_LineAuto 	(F_TestMode.Bit.b0)
	#define F_TestMode_LineFront 	(F_TestMode.Bit.b1)
	#define F_TestMode_PBA 			(F_TestMode.Bit.b2)		//	수동JIG 테스트
	#define F_TestMode_PBA_2 		(F_TestMode.Bit.b3)		//	수동JIG 테스트
	#define F_TestMode_LineTest2	(F_TestMode.Bit.b4)		//	공장 단축 검사

EXTERN U16 gu16_Effluent_Hz_Amount_forWIFI;
//EXTERN U16 gu16_Effluent_Hz_Amount;

//***************************************
//*               safety.c              *
//***************************************

//***************************************
//*               serial.c              *
//***************************************

//***************************************
//*               stepmotor.c           *
//***************************************
EXTERN volatile I16 gi16_StepCount;					//	현재 스텝 값
EXTERN U16 gu16_OpenCloseStep_Value;				//	이동 할 스텝 값

//***************************************
//*               timer.c               *
//***************************************


//***************************************
//*               테스트 용	            *
//***************************************
EXTERN U8 gu8_FrontTest_HotButton_Counter;
EXTERN U8 gu8_FrontTest_PureButton_Counter;
EXTERN U8 gu8_FrontTest_ColdButton_Counter;
EXTERN U8 gu8_FrontTest_AmountButton_Counter;
EXTERN U8 F_FrontTest_EffluentButton;

/*	결로 테스트 및 히터내구 테스트 용	*/
EXTERN U32 gu32_Timer_Condensation;
EXTERN U8 F_Heater_Endurance_Test_Start;			//	시작 플래그
EXTERN U8 F_Heater_Endurance_Test;
EXTERN U8 gu8_TimerHeaterEndurance_Test;
EXTERN U8 gu8_TimerHeaterEndurance_Test_30min;
EXTERN U8 gu8_Timer_TEM_Volt_Display;
/*	...	*/

EXTERN U16 gu8_TimerTestColdOff;
EXTERN U16 gu8_TimerTestColdOn;

EXTERN U16 gu16_HotEffluentTime;//20260310
#ifdef _FOR_DEVELOP_
EXTERN I16 gi16_Heater_Di_ini_Test;					//
EXTERN U16 gu16_FlowDecide_W_Test;					//
EXTERN U16 gu16_FlowDecide_Hz_Test;					//
EXTERN U16 gu16_HotEffluentTime;
EXTERN U8 gu8_Counter_HeaterOFF;

EXTERN I16 PSM1;
EXTERN I16 PSM2;
EXTERN U8 PSM3;
EXTERN U16 PSM4;
EXTERN U8 PSM5;
EXTERN U16 PSM6;

EXTERN U16 PSM_TimerCheck_1s;

EXTERN I16 gi16_ColdSensor_Test;
EXTERN I16 gi16_HotInSensor_Test;
EXTERN I16 gi16_HotOutSensor_Test;
EXTERN I16 gi16_HeaterSensor_Test;
EXTERN I16 gi16_HeatSinkSensor_Test;
EXTERN I16 gi16_OutDoorAirSensor_Test;
EXTERN I16 gi16_MCUSensor_Test;

EXTERN U8 gu8_PSM_TEM_Volt;

EXTERN U8 INPUT_REED_SW_COVER_Test;
EXTERN U8 INPUT_REED_SW_FILTER_Test;
EXTERN U8 INPUT_REED_SW_WATERFLOW_Test;

EXTERN U16 PSM_Dp1;
EXTERN U16 PSM_Dp2;
EXTERN U16 PSM_gu16_Heater_Di1;

EXTERN U8 gu8_ModelType_forNational_Test;
EXTERN U8 gu8_ModelType_forCHP_Test;

EXTERN U16 gu16_TEM_Voltage_Test;
EXTERN U16 P_TEM_PWM_TDR_Test;

EXTERN U16 gu16_HeaterFull_WATT_Test;

EXTERN U8 PSM_Voice;
#endif


/*===========================================================================*/
#undef	EXTERN
#undef	GLOBAL_VARIABLE_DEFINE
/*===========================================================================*/
