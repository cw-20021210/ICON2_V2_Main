/// @file   error.h
/// @date   2019.07.10
/// @author PSM
/// @brief  에러 처리 관련 헤더 file

#ifdef ERROR_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_ERROR()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_ERROR()		{	/*	10ms 타이머 감산		*/	\
									TDEC(gu16_TimerErr01);		\
									TDEC(gu16_TimerErr08);		\
									TDEC(gu16_TimerErr09);		\
									TDEC(gu16_TimerErr25);		\
									TDEC(gu16_TimerErr40);		\
									TDEC(gu16_TimerErr43);		\
									TDEC(gu16_TimerErr44);		\
									TDEC(gu16_TimerErr46);		\
									TDEC(gu16_TimerErr47);		\
									TDEC(gu16_TimerErr48);		\
									TDEC(gu16_TimerErr25_Clear);	\
									TDEC(gu16_TimerErr43_Clear);	\
									TDEC(gu16_TimerErr44_Clear);	\
									TDEC(gu16_TimerErr46_Clear);	\
									TDEC(gu16_TimerErr47_Clear);	\
									TDEC(gu16_TimerErr48_Clear);	\
									TDEC(gu8_TimerErr29_Display);	\
								}
#define	TDEC_100ms_ERROR()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu16_TimerErr29_2);	\
									TDEC(gu16_TimerErr25_Display);	\
									TDEC(gu16_TimerErr44_Display);	\
									TDEC(gu8_TimerEcode43_Display);	\
								}
#define	TDEC_1s_ERROR()			{	/*	1s 타이머 감산			*/	\
									TDEC(gu16_TimerErr02);		\
									TDEC(gu8_TimerErr08_Clear);	\
									TDEC(gu8_TimerErr09_Clear);	\
									TDEC(gu8_TimerTEM_PBATest);	\
									TDEC(gu16_TimerErr01_Clear);\
									TDEC(gu16_TimerErr02_Clear);	\
								}
#define	TDEC_10s_ERROR()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_ERROR()		{	/*	1min 타이머 감산		*/	\
									TDEC(gu16_TimerErr29_1);	\
								}
#define	TDEC_10min_ERROR()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_ERROR()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_Ecode01		(0x01)						//	누수 에러
#define Z_Ecode02		(0x02)						//	누수 에러(유량 감지)
#define Z_Ecode08		(0x08)						//	온수 단수 에러
#define Z_Ecode09		(0x09)						//	단수 에러
#define Z_Ecode25		(0x25)						//	TEM 방열판 센서 에러 (단선/단락)
#define Z_Ecode29		(0x29)						//	TEM 온도 이상 에러
#define Z_Ecode40		(0x40)						//	과열 에러
#define Z_Ecode43		(0x43)						//	외기 온도 센서 (단선/단락)
#define Z_Ecode44		(0x44)						//	냉수 온도 센서 (단선/단락)
#define Z_Ecode46		(0x46)						//	온수 입수 온도 센서 (단선/단락)
#define Z_Ecode47		(0x47)						//	온수 출수 온도 센서 (단선/단락)
#define Z_Ecode48		(0x48)						//	히터온도 센서 에러
#define Z_EcodeA0		(0xA0)						//	프론트 통신 에러

#define Z_TimerErr01			(300)				//	<3s/10ms>
#define Z_TimerErr01_Clear		(60)				//	<60s/1s>
#define Z_TimerErr02			(10)				//	<10s/1s>
#define Z_TimerErr02_Clear		(60)				//	<60s/1s>
#define Z_TimerErr08			(100)				//	<1s/10ms>
#define Z_TimerErr09			(500)				//	<5s/10ms>
#define Z_TimerErr09_forFlushing	(2000)			//	<20s/10ms>
#define Z_TimerErr25			(300)				//	<3s/10ms>
#define Z_TimerErr29_1			(120)				//	<120min/1min>
#define Z_TimerErr29_2			(10)				//	<1s/100ms>
#define Z_TimerErr40			(50)				//	<500ms/10ms>
#define Z_TimerErr43			(300)				//	<3s/10ms>
#define Z_TimerErr44			(300)				//	<3s/10ms>
#define Z_TimerErr46			(300)				//	<3s/10ms>
#define Z_TimerErr47			(100)				//	<1s/10ms>
#define Z_TimerErr47_Clear		(300)				//	<3s/10ms>
#define Z_TimerErr48			(100)				//	<1s/10ms>
#define Z_TimerErr48_Clear		(300)				//	<3s/10ms>

#define Z_TimerErr01_TestMode	(30)				//	<300ms/10ms>
#define Z_TimerErr25_TestMode	(30)				//	<300ms/10ms>
#define Z_TimerErr43_TestMode	(30)				//	<300ms/10ms>
#define Z_TimerErr44_TestMode	(30)				//	<300ms/10ms>
#define Z_TimerErr47_TestMode	(30)				//	<300ms/10ms>
#define Z_TimerErr46_TestMode	(30)				//	<300ms/10ms>
#define Z_TimerErr48_TestMode	(30)				//	<300ms/10ms>

#define Z_TimerErr25_Display	(30)				//	<3s/100ms>
#define Z_TimerErr44_Display	(30)				//	<3s/100ms>

#define Z_TimerErr25_Display_TestMode	(5)			//	<0.5s/100ms>
#define Z_TimerErr44_Display_TestMode	(5)			//	<0.5s/100ms>

#define Z_TimerErr08_Clear		(60)				//	<60s/1s>
#define Z_TimerErr09_Clear		(60)				//	<60s/1s>

#define Z_TimerTEM_PBATest		(105)				//	<105s/1s>

#define Z_Timer_Err29_Decide	(50)				//	<500ms/10ms>

#define Z_TimerPumpDelay_forCutWater	(30)		//	단수에러 발생 시 펌프지연시간 30분	<30min/1min>

//#define Z_Cold_HeatSinkTH_CutDegree	(Z_TEMP75)		//	29번 에러 히트싱크TH 에러
#define Z_Cold_HeatSinkTH_CutDegree	(Z_TEMP80)		//@005	29번 에러 히트싱크TH 에러

#define Z_LeakSensor_Error		(475)				//	누수 에러 값	※ SHORT시 950 / 물 넣었을 시 580정도 발생

#define Z_TimerEcode43_Display	(30)

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U16 gu16_TimerErr01;
EXTERN U16 gu16_TimerErr01_Clear;					//	<?s/1s>
EXTERN U16 gu16_TimerErr02_Clear;					//	<?s/1s>
EXTERN U16 gu16_TimerErr02;							//	<10s/1s>
EXTERN U16 gu16_TimerErr08;
EXTERN U16 gu16_TimerErr09;
EXTERN U16 gu16_TimerErr25;
EXTERN U16 gu16_TimerErr29_1;						//	<60min/1min>
EXTERN U16 gu16_TimerErr29_2;						//	<1s/100ms>
EXTERN U16 gu16_TimerErr40;
EXTERN U16 gu16_TimerErr43;
EXTERN U16 gu16_TimerErr44;
EXTERN U16 gu16_TimerErr46;
EXTERN U16 gu16_TimerErr47;
EXTERN U16 gu16_TimerErr48;

EXTERN U16 gu16_TimerErr25_Display;					//	에러확정은 플래그로 하고, 냉수 선택 후에 3초 뒤에 에러코드 표시하기 위함
EXTERN U16 gu16_TimerErr44_Display;					//	에러확정은 플래그로 하고, 냉수 선택 후에 3초 뒤에 에러코드 표시하기 위함
EXTERN U8 gu8_TimerErr29_Display;					//	에러확정은 플래그로 하고, 냉수 선택 후에 0.5초 뒤에 에러코드 표시하기 위함

EXTERN U16 gu16_TimerErr25_Clear;					//	정상데이터 3초 유지 시  TH단선단락 에러 자동해제	<3s/10ms>
EXTERN U16 gu16_TimerErr43_Clear;					//	정상데이터 3초 유지 시  TH단선단락 에러 자동해제	<3s/10ms>
EXTERN U16 gu16_TimerErr44_Clear;					//	정상데이터 3초 유지 시  TH단선단락 에러 자동해제	<3s/10ms>
EXTERN U16 gu16_TimerErr46_Clear;					//	정상데이터 3초 유지 시  TH단선단락 에러 자동해제	<3s/10ms>
EXTERN U16 gu16_TimerErr47_Clear;					//	정상데이터 3초 유지 시  TH단선단락 에러 자동해제	<3s/10ms>
EXTERN U16 gu16_TimerErr48_Clear;					//	정상데이터 3초 유지 시  TH단선단락 에러 자동해제	<3s/10ms>

EXTERN U8 gu8_TimerErr08_Clear;						//	1분 후 단수에러 자동해제	<60s/1s>
EXTERN U8 gu8_TimerErr09_Clear;						//	1분 후 단수에러 자동해제	<60s/1s>

EXTERN U8 gu8_TimerTEM_PBATest;

EXTERN U8 F_OverHeatError;							//	과열에러 발생 후 80도 이하를 감지해야 차회 추출 가능 관련 Flag

EXTERN U8 gu8_TimerEcode43_Display;					//	외기온도 3초 표시

EXTERN WORD_BIT F_Error_Flag;
	#define F_Err01_LeakWater 			(F_Error_Flag.Bit.b0)
	#define F_Err02_LeakWater 			(F_Error_Flag.Bit.b1)
	#define F_Err08_HotCutWater 		(F_Error_Flag.Bit.b2)
	#define F_Err09_CutWater 			(F_Error_Flag.Bit.b3)
	#define F_Err25_HeatSinkTH			(F_Error_Flag.Bit.b4)
	#define F_Err29_TEM_OverDrive1		(F_Error_Flag.Bit.b5)
	#define F_Err29_TEM_OverDrive2		(F_Error_Flag.Bit.b6)
	#define F_Err40_OverHeat 			(F_Error_Flag.Bit.b7)
	#define F_Err43_OutDoorAirTH 		(F_Error_Flag.Bit.b8)
	#define F_Err44_ColdTH 				(F_Error_Flag.Bit.b9)
	#define F_Err47_HotOutTH 			(F_Error_Flag.Bit.b10)
	#define F_Err46_HotInTH				(F_Error_Flag.Bit.b11)
	#define F_Err48_HeaterTH			(F_Error_Flag.Bit.b12)
	#define F_ErrA0_FrontCommunication 	(F_Error_Flag.Bit.b13)


/*==============================================================================
 function Definition
 ==============================================================================*/
void Error_Initialize(void);
void ErrorCheck(void);
void ErrorClear(void);
U8 TH_Open_Short_Check(U16* mu16_ADCAddress);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
