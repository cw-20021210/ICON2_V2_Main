/// @file   stepmotor.h
/// @date   2019.07.10
/// @author PSM
/// @brief  스테핑모터 처리 관련 헤더 file

#ifdef STEPMOTOR_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_STEPMOTOR()	{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_STEPMOTOR()	{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_STEPMOTOR()	{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_STEPMOTOR()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_STEPMOTOR()	{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_STEPMOTOR()	{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_STEPMOTOR()	{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_STEPMOTOR()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define STEP_MOTER_OPEN   (0)						//	OPEN 값
#define STEP_MOTER_CLOSE  (1)						//	CLOSE 값

#define ZERO_BACK         (3000)					//	초기화 값


/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN volatile U8 gu8_Step_Cycle;					//	StepMotor 시퀀스
EXTERN volatile U8 gu8_StepMotorWay;         		// 	StepMotor 방향

EXTERN volatile U8 gu8_StepMotorWay_Backup;         	// 	StepMotor 방향
EXTERN volatile U16 gu16_StepMovingPulse_Backup;	//	스텝 지시값 back up
EXTERN volatile U8 gu8_Stepping_ZeroBack;			//	원위치 flag (0:미동작 1:OPEN 2:CLOSE)

EXTERN volatile U8 gu8_Pps_Control;					//	PPS 세팅
EXTERN volatile U8 gu8Move_PPS;						//
EXTERN volatile U8 gu8Cnt_PPS;
EXTERN volatile U16 gu16_StepMovingPulse;

EXTERN U16 gu16_FlowTarget_W;

/*==============================================================================
 function Definition
 ==============================================================================*/
void FlowControl_StepMove(U8 mu8OpenClose, U16 mu16Pulse);
void FlowControl_StepTarget(U16 mu16Pulse_Target);
void FlowControl(void);
void StepMotorMove(void);
void StepMotor_Initialize(void);
void StepMotor_Control(void);
void StepMotor_Stop(void);
void StepMotor_ZeroBack_Open(void);
void StepMotor_ZeroBack_Close(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
