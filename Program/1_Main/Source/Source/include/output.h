/// @file   output.h
/// @date   2019.07.10
/// @author PSM
/// @brief  출력 처리 file

#ifdef OUTPUT_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_OUTPUT()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_OUTPUT()		{	/*	10ms 타이머 감산		*/	\
									TDEC(gu8_TimerColdSensor_Sensing);	\
									TDEC(gu8_Timer_TEM_24V_OffDelay);	\
									TDEC(gu8_Timer_TEM_12V_OffDelay);	\
								}
#define	TDEC_100ms_OUTPUT()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerBoot_After1s);	\
									TDEC(gu8_TimerStanbyOn_After1s);	\
									TDEC(gu8_TimerFanDelay);	\
									TDEC(gu8_Timer_Forced_TemOutput);	\
								}
#define	TDEC_1s_OUTPUT()		{	/*	1s 타이머 감산			*/	\
									TDEC(gu16_TimerColdSensor_ON);	\
									TDEC(gu16_TimerColdSensor_OFF);	\
									TDEC(gu8_Timer_TEM_Voltage_Retain);	\
								}
#define	TDEC_10s_OUTPUT()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_OUTPUT()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_OUTPUT()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_OUTPUT()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
typedef struct {
	I16 Outdoor_Degree;					//	외기온도 기준
	U16 TEM_Volt;						//	TEM 전압
} TEM_Data;

typedef struct {
	U16 TEM_Volt;						//	전압
	U16 TEM_TDR;						//	TDR
} TEM_Voltage_Data;

#define Z_TimerColdSensor_ON_Init	(600)			//	<600s/1s>
#define Z_TimerColdSensor_ON		(10)			//	<10s/1s>
#define Z_TimerColdSensor_OFF		(30)			//	<30s/1s>

#define Z_TimerColdSensor_Sensing	(50)			//	<500ms/10ms>

#define Z_TimerBoot_After1s			(10)			//	<1s/100ms>
#define Z_TimerStanbyOn_After1s			(10)			//	<1s/100ms>

#define Z_TimerFanDelay				(10)			//	<1s/100ms>

#define Z_TEM_Voltage_Data_Max	    (8)
#define Z_CoolingUp_Volatage	    (1)					//	TEM 전압 1V 상향

#define Z_Cooling_Voltage_24V		(24)
#define Z_Cooling_Voltage_12V		(12)
#define Z_Cooling_Voltage_11V		(11)
#define Z_Cooling_Voltage_10V		(10)
#define Z_Cooling_Voltage_9V		(9)
#define Z_Cooling_Voltage_8V		(8)
#define Z_Cooling_Voltage_7V		(7)
#define Z_Cooling_Voltage_6V		(6)
#define Z_Cooling_Voltage_5V		(5)

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_PureValve_CMD;
//EXTERN U8 gu8_NosValve_CMD;
EXTERN U8 gu8_HotInValve_CMD;
EXTERN U8 gu8_HotOutValve_CMD;
EXTERN U8 gu8_HeaterRelay_CMD;
EXTERN U8 gu8_HeaterTriac_CMD;
EXTERN U8 gu8_Cold_Out_Valve_CMD;
EXTERN U8 gu8_Cold_In_Valve_CMD;
EXTERN U8 gu8_TEM_PWM_CMD;
EXTERN U8 gu8_TEM_EN_CMD;
EXTERN U8 gu8_FAN_CMD;
//EXTERN U8 gu8_FAN_21V_CMD;
EXTERN U8 gu8_UV_CMD;

EXTERN U8 gu8_TimerFanDelay;						//	<1s/100ms>

EXTERN U8 F_HeaterPWM_SingleControl;				//	히터 PWM만 제어하는 flag (Relay OFF)

EXTERN U8 gu8_TimerColdSensor_Sensing;				//	센서 OFF→ON시 회로 안정화 시간 (Cap으로 인한 전압 늦게 상승) <500ms/10ms>

EXTERN U8 F_ColdSensor_OFF;
EXTERN U16 gu16_TimerColdSensor_ON;
EXTERN U16 gu16_TimerColdSensor_OFF;
EXTERN U8 gu8_TimerColdSensor_Sensing;

EXTERN U16 gu16_TEM_Voltage;
EXTERN U16 gu16_TEM_Voltage_Backup;
EXTERN U8 gu8_Timer_TEM_Voltage_Retain;				//	TEM 출력 변경 시, 최소 1분은 해당 출력 유지하도록.
EXTERN U16 gu16_TEM_TDR_Data_Backup;



EXTERN U16 gu16_TEM_Voltage;
EXTERN U8 gu8_TEM_Voltage_BoJung;					//@022	TEM전압 0.1V단위 조절을 위한 보정 데이터

EXTERN U8 gu8_TimerBoot_After1s;
EXTERN U8 gu8_TimerStanbyOn_After1s;

EXTERN U8 gu8_Timer_TEM_12V_OffDelay;
EXTERN U8 gu8_Timer_TEM_24V_OffDelay;

EXTERN U8 gu8_f_ad_feedback_mode;		//	TEM 출력 강제 표시 플래그
EXTERN U8 gu8_Timer_Forced_TemOutput;				//	TEM 출력 강제 표시 타이머	

EXTERN U32 TEM_TDR_Data_TEST;
EXTERN U8 gu16_TEM_Voltage_Test;

/*==============================================================================
 function Definition
 ==============================================================================*/
void OutputControl(void);
void Output_Initialize(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

