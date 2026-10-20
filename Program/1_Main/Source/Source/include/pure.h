/// @file   effluent.h
/// @date   2019.07.10
/// @author PSM
/// @brief  추출 동작 제어 관련 헤더 file

#ifdef PURE_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_PURE()			{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_PURE()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_PURE()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_Timer_PureWaterFlow_Average_Start);	\
								}
#define	TDEC_1s_PURE()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_PURE()			{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_PURE()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_PURE()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_PURE()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
enum {
	Z_PureStepNum_00,
	Z_PureStepNum_01,
	Z_PureStepNum_02,
	Z_PureStepNum_10,
	Z_PureStepNum_20,
	Z_PureStepNum_22,
//	Z_PureStepNum_B0,
	Z_PureStepNum_Max
};

#define Z_PureStep_00			(0x00)
#define Z_PureStep_01			(0x01)
#define Z_PureStep_02			(0x02)
#define Z_PureStep_10			(0x10)
#define Z_PureStep_20			(0x20)
#define Z_PureStep_22			(0x22)
//#define Z_PureStep_B0			(0xB0)

/*	컵 사이즈 값	*/
/*
#define CUPSIZE_HALFCUP			(0)					//
#define CUPSIZE_1CUP	        (1)					//
#define CUPSIZE_2CUP         	(2)					//
#define CUPSIZE_CONTINUE        (3)
#define CUPSIZE_MY		        (4)
*/

/*	컵 사이즈 값	*/
#define CUPSIZE_HALFCUP			(120)					//
#define CUPSIZE_1CUP	        (250)					//
#define CUPSIZE_2CUP         	(500)					//
#define CUPSIZE_ONE_BOTTLE       (1000)					//
#define CUPSIZE_CONTINUE        (0x00AA)
#define CUPSIZE_MY		        (5)

#define CUPSIZE_ONE				(0)
#define CUPSIZE_TWO				(1)
#define CUPSIZE_THREE			(2)
#define CUPSIZE_FOUR			(3)
//#define CUPSIZE_FIVE			(4)
#define CUPSIZE_FIVE			(3)
// #define CUPSIZE_MAX				(5)				//	연속 포함 5개

#define CUP_SIZE_CONTINUE		(0xAA)




/*	물 종류 값	*/
#define PURE_WATER            	(0)
#define COLD_WATER              (1)
#define HOT_WATER               (2)

#define Z_Timer_PureWaterFlow_Average_Start		(30)	//	<3s/100ms>

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_PureStep;
EXTERN U8 gu8_PureStepNum;

EXTERN U8 gu8_Timer_PureWaterFlow_Average_Start;	//	정수 추출 후 3초가 지난 뒤(유량계산이 안정화 된 뒤)부터 유량 재계산 확정

/*==============================================================================
 function Definition
 ==============================================================================*/
void Pure_Initialize(void);
void PureControl(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
