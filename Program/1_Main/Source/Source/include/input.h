/// @file   input.h
/// @date   2019.07.10
/// @author PSM
/// @brief  입력 처리 file

#ifdef INPUT_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_INPUT()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_INPUT()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_INPUT()		{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_INPUT()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_INPUT()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_INPUT()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_INPUT()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_INPUT()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/


/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN ITTI3 gu8_DigitalInput;
	#define INPUT_REED_SW_COVER			(gu8_DigitalInput.Level.Bit.b0)
	#define INPUT_REED_SW_FILTER		(gu8_DigitalInput.Level.Bit.b1)
	#define INPUT_REED_SW_FAUCET		(gu8_DigitalInput.Level.Bit.b2)
	#define INPUT_MODEL_NATIONAL		(gu8_DigitalInput.Level.Bit.b3)

EXTERN U8 INPUT_REED_SW_COVER_Before;
EXTERN U8 INPUT_REED_SW_FILTER_Before;
EXTERN U8 INPUT_REED_SW_FAUCET_Before;

/*==============================================================================
 function Definition
 ==============================================================================*/
void Input_Initialize(void);
void InputControl(void);
void Model_Decide(void);

void OneMatchModule(ITTI1* p, U8 data);
void ThreeMatchModule(ITTI3* p, U8 data);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

