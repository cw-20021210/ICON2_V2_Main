/// @file   buzzer.h
/// @date   2019.07.10
/// @author PSM
/// @brief  Buzzer 동작 제어 관련 헤더 file

#ifdef PBA_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_PBA()			{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_PBA()			{	/*	10ms 타이머 감산		*/	\
									TDEC(gu8_TimerPBATest_TEM_ON);	\
									TDEC(gu8_TimerPBATest_PureVV_ON);	\
									TDEC(gu8_TimerPBATest_FanDisplay);	\
									TDEC(gu8_TimerPBATest_HotLoadDisplay);	\
									TDEC(gu8_TimerPBATest_PureLoadDisplay);	\
									TDEC(gu8_TimerPBATest_ColdLoadDisplay);	\
									TDEC(gu8_TimerPBATest_HeaterRelay);	\
									TDEC(gu8_TimerPBATest_HeaterTriac);	\
								}
#define	TDEC_100ms_PBA()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerPBATest_FanFB);	\
									TDEC(gu8_TimerPBATest_HeaterFB);	\
									TDEC(gu8_TimerPBATest_TemFB);	\
								}
#define	TDEC_1s_PBA()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_PBA()			{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_PBA()			{	/*	1min 타이머 감산		*/	\
									/*TDEC(gu8_TimerFCTReset);*/	\
								}
#define	TDEC_10min_PBA()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_PBA()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
enum {
	Z_PBAStepNum_00,
	Z_PBAStepNum_10,
	Z_PBAStepNum_20,
	Z_PBAStepNum_Max
};

#define Z_PBAStep_00			(0x00)
#define Z_PBAStep_10			(0x10)
#define Z_PBAStep_20			(0x20)

#define Z_TimerFCTReset			(30)				//	<30min/1min>

#define Z_TimerPBATest_FanDisplay	(80)
#define Z_TimerPBATest_HotLoadDisplay	(80)
#define Z_TimerPBATest_PureLoadDisplay	(80)
#define Z_TimerPBATest_ColdLoadDisplay	(80)

#define Z_TimerPBATest_HeaterRelay	(100)			//	<1000ms/10ms>
#define Z_TimerPBATest_HeaterTriac	(100)			//	<1000ms/10ms>

#define Z_TimerPBATest_TEM_ON		(50)
#define Z_TimerPBATest_PureVV_ON	(50)

#define	Z_TimerPBATest_FanFB		(10)
#define	Z_TimerPBATest_HeaterFB		(10)
#define	Z_TimerPBATest_TemFB		(10)

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_PBAStepNum;

//	테스트 모드1
EXTERN U8 F_PBATest_ColdSensorOFF;
EXTERN U8 F_PBATest_LeakSensorOFF;
EXTERN U8 F_PBATest_VoiceReset;
EXTERN U8 gu8_PBATest_FanTest;

EXTERN U8 gu8_TimerPBATest_FanDisplay;				//	<500ms/10ms>
EXTERN U8 gu8_TimerPBATest_HotLoadDisplay;			//	<500ms/10ms>
EXTERN U8 gu8_TimerPBATest_ColdLoadDisplay;			//	<500ms/10ms>
EXTERN U8 gu8_TimerPBATest_PureLoadDisplay;			//	<500ms/10ms>

//	테스트 모드2
EXTERN U8 F_PBATest_SteppingMotor;
EXTERN U8 F_PBATest_StandBy_OFF;

EXTERN U8 gu8_PBATest_HotLoad;
EXTERN U8 gu8_PBATest_ColdLoad;
EXTERN U8 gu8_PBATest_PureLoad;
EXTERN U8 F_PBATest_StandByOFF;

//EXTERN U8 F_PBATest_HeaterRelay;
//EXTERN U8 F_PBATest_HeaterTriac;

EXTERN U8 gu8_TimerPBATest_HeaterRelay;				//	<1s/10ms>
EXTERN U8 gu8_TimerPBATest_HeaterTriac;				//	<1s/10ms>

EXTERN U8 gu8_TimerPBATest_TEM_ON;					//	<500ms/10ms>
EXTERN U8 gu8_TimerPBATest_PureVV_ON;				//	<500ms/10ms>

EXTERN U8 F_HeaterFeedBack_Error;
EXTERN U8 F_TEMFeedBack_Error;
EXTERN U8 F_24VFeedBack_Error;
EXTERN U8 F_PBATest_FanFeedBack_Error;

EXTERN U8 F_PBATest_TEM;							//	실행 여부
EXTERN U8 F_PBATest_Heater;							//	실행 여부
EXTERN U8 F_PBATest_24V;							//	실행 여부
EXTERN U8 F_PBATest_Fan;							//	실행 여부

EXTERN U8 gu8_TimerPBATest_FanFB;
EXTERN U8 gu8_TimerPBATest_HeaterFB;
EXTERN U8 gu8_TimerPBATest_TemFB;

/*==============================================================================
 function Definition
 ==============================================================================*/
void PBA_Initialize(void);
void PBAControl(void);



//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
