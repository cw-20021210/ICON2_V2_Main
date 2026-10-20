/// @file   safety.h
/// @date   2019.07.10
/// @author PSM
/// @brief  Safety 동작 제어 관련 헤더 file

#ifdef SAFETY_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_SAFETY()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_SAFETY()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_SAFETY()		{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_SAFETY()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_SAFETY()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_SAFETY()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_SAFETY()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_SAFETY()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/



/*==============================================================================
 Variable Definition
 ==============================================================================*/


/*==============================================================================
 function Definition
 ==============================================================================*/
void Safety_Initialize(void);
void Safety(void);
void WatchdogClear(void);
void SafetyLoop(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

