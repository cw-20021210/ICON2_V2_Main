/// @file   macro.h
/// @date   2019.07.10
/// @author PSM
/// @brief  공용 Macro 처리 file

#ifdef MACRO_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_MACRO()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_MACRO()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_MACRO()		{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_MACRO()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_MACRO()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_MACRO()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_MACRO()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_MACRO()			{	/*	1h 타이머 감산			*/	\
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
void Delay(U16 mu8Time);
void Delay_ms(U16 mu8Time);
U8 HEX2DEC(U8 mu8HexData);
U8 DEC2HEX(U8 mu8DecData);
U16 DEC2HEX_16BIT(U16 mu16DecData);

U8 DisAsciiNibble(U8 *pBuffer, U8 *pNibbleDat);/*	NIBBLE DATA 역ASCII 변환 */
U8 Ascii(U8 data, U8 hilo); 						/*	ASCII 변환 			*/
U8 DisAsciiByte(U8 *pBuffer, U8 *pByteDat);	/*	1byte 역ASCII 변환	*/
U8 DisAsciiWord(U8 *pBuffer, U16 *pWordDat);	/*	2byte 역ASCII 변환	*/

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

