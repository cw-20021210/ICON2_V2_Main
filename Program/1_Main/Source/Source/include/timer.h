/// @file   timer.h
/// @date   2019.07.10
/// @author PSM
/// @brief  Timer 동작 제어 관련 헤더 file

#ifdef TIMER_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_TIMER()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_TIMER()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_TIMER()		{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_TIMER()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_TIMER()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_TIMER()		{	/*	1min 타이머 감산		*/	\
									TDEC(gu16_TimerPartTest);	\
								}
#define	TDEC_10min_TIMER()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_TIMER()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/



/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN volatile U8 gu8_Counter10ms;
EXTERN volatile U8 gu8_Counter100ms;
EXTERN volatile U8 gu8_FlagTime100ms;
EXTERN volatile U8 gu8_FlagTime1000ms;

EXTERN volatile U8 gu8_Counter_1msTimerCheck;
EXTERN volatile U8 gu8_Counter_MainLoopCheck;

EXTERN volatile U8 F_10ms_for_InputDigital;
EXTERN U8 F_100ms_for_FlowControl;
EXTERN U8 F_100ms_for_HeaterControl;
//EXTERN U8 F_200ms_for_HeaterControl;
//EXTERN U8 F_3s_for_FlowControl;
//EXTERN U8 F_1s_for_TEM_OnTimeCheck;
EXTERN U8 F_100ms_for_Effluent_Degree_Ave;
EXTERN U8 F_100ms_for_HeaterDegreeSave;
EXTERN U8 F_100ms_for_Degree_Average;

EXTERN U16 gu16_PartTest_Period;
EXTERN U16 gu16_TimerPartTest;

/*==============================================================================
 function Definition
 ==============================================================================*/
void Timer_Initialize(void);
void Timer_1msBase_Timer(void);
void Timer_Process(void);
void Time1ms(void);
void Time10ms(void);
void Time100ms(void);
void Time1s(void);
void Time10s(void);
void Time1min(void);
void Time10min(void);
void Time1h(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

