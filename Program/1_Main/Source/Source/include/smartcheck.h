/// @file   buzzer.h
/// @date   2019.07.10
/// @author PSM
/// @brief  Buzzer 동작 제어 관련 헤더 file

#ifdef SMARTCHECK_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_SMARTCHECK()	{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_SMARTCHECK()	{	/*	10ms 타이머 감산		*/	\
									TDEC(gu8_TimerSmartCheck_10ms);	\
								}
#define	TDEC_100ms_SMARTCHECK()	{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerSmartCheck_100ms);	\
								}
#define	TDEC_1s_SMARTCHECK()	{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_SMARTCHECK()	{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_SMARTCHECK()	{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_SMARTCHECK()	{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_SMARTCHECK()	{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
enum {
	Z_SmartCheckStepNum_00,
	Z_SmartCheckStepNum_10,
	Z_SmartCheckStepNum_20,
	Z_SmartCheckStepNum_21,
	Z_SmartCheckStepNum_22,
	Z_SmartCheckStepNum_23,
	Z_SmartCheckStepNum_24,
	Z_SmartCheckStepNum_25,
	Z_SmartCheckStepNum_26,
	Z_SmartCheckStepNum_27,
	Z_SmartCheckStepNum_28,
	Z_SmartCheckStepNum_29,
	Z_SmartCheckStepNum_2A,
	Z_SmartCheckStepNum_2B,
	Z_SmartCheckStepNum_2C,
	Z_SmartCheckStepNum_2D,
	Z_SmartCheckStepNum_Max
};

#define Z_SmartCheckStep_00			(0x00)
#define Z_SmartCheckStep_10			(0x10)
#define Z_SmartCheckStep_20			(0x20)
#define Z_SmartCheckStep_21			(0x21)
#define Z_SmartCheckStep_22			(0x22)
#define Z_SmartCheckStep_23			(0x23)
#define Z_SmartCheckStep_24			(0x24)
#define Z_SmartCheckStep_25			(0x25)
#define Z_SmartCheckStep_26			(0x26)
#define Z_SmartCheckStep_27			(0x27)
#define Z_SmartCheckStep_28			(0x28)
#define Z_SmartCheckStep_29			(0x29)
#define Z_SmartCheckStep_2A			(0x2A)
#define Z_SmartCheckStep_2B			(0x2B)
#define Z_SmartCheckStep_2C			(0x2C)
#define Z_SmartCheckStep_2D			(0x2D)


#define Z_TimerSmartCheck_10ms_500ms	(50)			//	<500ms/10ms>

#define Z_TimerSmartCheck_100ms_1s		(10)			//	<1s/100ms>
#define Z_TimerSmartCheck_100ms_1_5s	(15)			//	<1.5s/100ms>
#define Z_TimerSmartCheck_100ms_2s		(20)			//	<2s/100ms>
#define Z_TimerSmartCheck_100ms_3s		(30)			//	<3s/100ms>
#define Z_TimerSmartCheck_100ms_5s		(50)			//	<5s/100ms>

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_SmartCheckStep;
EXTERN U8 gu8_SmartCheckStepNum;

EXTERN U8 gu8_TimerSmartCheck_10ms;
EXTERN U8 gu8_TimerSmartCheck_100ms;

EXTERN U8 gu8_SmartCheck_Percent;

EXTERN WORD_BIT gu16_SmartCheck_LoadCMD;
	#define F_SmartCheck_HeaterTriac			(gu16_SmartCheck_LoadCMD.Bit.b0)
	#define F_SmartCheck_HeaterRelay			(gu16_SmartCheck_LoadCMD.Bit.b1)
	#define F_SmartCheck_Pure_VV				(gu16_SmartCheck_LoadCMD.Bit.b2)
	#define F_SmartCheck_HotIn_VV				(gu16_SmartCheck_LoadCMD.Bit.b3)
	#define F_SmartCheck_HotOut_VV				(gu16_SmartCheck_LoadCMD.Bit.b4)
	#define F_SmartCheck_ColdIn_VV				(gu16_SmartCheck_LoadCMD.Bit.b5)
	#define F_SmartCheck_ColdOut_VV				(gu16_SmartCheck_LoadCMD.Bit.b6)
	#define F_SmartCheck_1stNos_VV				(gu16_SmartCheck_LoadCMD.Bit.b7)
	#define F_SmartCheck_Fan					(gu16_SmartCheck_LoadCMD.Bit.b8)
	#define F_SmartCheck_TEM					(gu16_SmartCheck_LoadCMD.Bit.b9)
	#define F_SmartCheck_FlowMeter				(gu16_SmartCheck_LoadCMD.Bit.b10)
	#define F_SmartCheck_UV						(gu16_SmartCheck_LoadCMD.Bit.b11)
	#define F_SmartCheck_SteppingMotor			(gu16_SmartCheck_LoadCMD.Bit.b12)

EXTERN WORD_BIT gu16_SmartCheck_Result;
	#define F_SmartCheck_Result_HeaterTriac		(gu16_SmartCheck_Result.Bit.b0)
	#define F_SmartCheck_Result_HeaterRelay		(gu16_SmartCheck_Result.Bit.b1)
	#define F_SmartCheck_Result_Pure_VV			(gu16_SmartCheck_Result.Bit.b2)
	#define F_SmartCheck_Result_HotIn_VV		(gu16_SmartCheck_Result.Bit.b3)
	#define F_SmartCheck_Result_HotOut_VV		(gu16_SmartCheck_Result.Bit.b4)
	#define F_SmartCheck_Result_ColdIn_VV		(gu16_SmartCheck_Result.Bit.b5)
	#define F_SmartCheck_Result_ColdOut_VV		(gu16_SmartCheck_Result.Bit.b6)
	#define F_SmartCheck_Result_1stNos_VV		(gu16_SmartCheck_Result.Bit.b7)
	#define F_SmartCheck_Result_Fan				(gu16_SmartCheck_Result.Bit.b8)
	#define F_SmartCheck_Result_TEM				(gu16_SmartCheck_Result.Bit.b9)
	#define F_SmartCheck_Result_FlowMeter		(gu16_SmartCheck_Result.Bit.b10)
	#define F_SmartCheck_Result_UV				(gu16_SmartCheck_Result.Bit.b11)
	#define F_SmartCheck_Result_SteppingMotor	(gu16_SmartCheck_Result.Bit.b12)


EXTERN U16 gu16_SmartCheck_Result_HeaterTriac_ADC;
EXTERN U16 gu16_SmartCheck_Result_HeaterRelay_ADC;
EXTERN U16 gu16_SmartCheck_Result_Pure_VV_mA;
EXTERN U16 gu16_SmartCheck_Result_HotIn_VV_mA;
EXTERN U16 gu16_SmartCheck_Result_HotOut_VV_mA;
EXTERN U16 gu16_SmartCheck_Result_ColdIn_VV_mA;
EXTERN U16 gu16_SmartCheck_Result_ColdOut_VV_mA;
EXTERN U16 gu16_SmartCheck_Result_1stNos_VV_mA;
EXTERN U16 gu16_SmartCheck_Result_Fan_mA;
EXTERN U16 gu16_SmartCheck_Result_TEM_mA;
EXTERN U16 gu16_SmartCheck_Result_FlowMeter;		//	유속[1/1ccpm] (cc/min)
EXTERN U16 gu16_SmartCheck_Result_UV_mA;
EXTERN U16 gu16_SmartCheck_Result_SteppingMotor_mA;

EXTERN U8 F_SmartCheck;								//	스마트 고장진단 시작 flag
EXTERN U8 F_SmartCheck_Stop;						//	스마트 고장진단 정지 flag
EXTERN U8 F_SmartCheck_Complete;					//	스마트 고장진단 완료 flag
EXTERN U8 F_SmartCheck_Complete_2;					//	스마트 고장진단 완료 flag (0040 cmd 수신 시)
EXTERN U8 F_SmartCheck_Start;						//	스마트 고장진단 시작 송신 flag
EXTERN U8 F_SmartCheck_Prohibit;					//	스마트 고장진단 불가 flag

/*==============================================================================
 function Definition
 ==============================================================================*/
void SmartCheck_Initialize(void);
void SmartCheckControl(void);


//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
