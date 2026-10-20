/// @file   keycontrol.h
/// @date   2019.07.10
/// @author PSM
/// @brief  키 입력 처리 관련 헤더 file

#ifdef KEYCONTROL_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_KEYCONTROL()	{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_KEYCONTROL()	{	/*	10ms 타이머 감산		*/	\
									TDEC(gu16_TimerKeyDecide);	\
									TDEC(gu8_TimerKeyProhibit);	\
								}
#define	TDEC_100ms_KEYCONTROL()	{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerKeyOnBreak);	\
								}
#define	TDEC_1s_KEYCONTROL()	{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_KEYCONTROL()	{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_KEYCONTROL()	{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_KEYCONTROL()	{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_KEYCONTROL()	{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_TimerKeyProhibit	(10)					//	<100ms/10ms>


/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U16 gu16_TimerKeyDecide;						//	키 확정 타이머
EXTERN U8 gu8_TimerKeyProhibit;						//	키 입력 금지 타이머
//EXTERN U8 gu8_EffluentKey_OnBreak;
EXTERN U8 gu8_TimerKeyOnBreak;						//	키 온고장 확정 타이머
EXTERN U8 F_VeryLongKey_Rejection;					//	Long key가 Active하고 Very Long key가 한번 더 동작되는 것을 방지하는 플래그
EXTERN U8 F_ShortKeyProhibit;						//	단키 중복

EXTERN U8 F_KeyOnce;								//	전원 부팅 후 최초 키 입력 플래그 (눌려있는 상태로 진입했을 경우를 판단)
EXTERN U8 gu8_KeyOnBreak;							//	전원 부팅 후 최초 키 입력 데이터 (눌려있는 상태로 진입했을 경우를 판단)

/*==============================================================================
 function Definition
 ==============================================================================*/
void KeyControl_Initialize(void);
void KeyControl(void);
void KeyDecide(ITTI1* p);
void Delay_Nop(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

