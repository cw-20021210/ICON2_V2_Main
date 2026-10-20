#ifndef _TYPE_DEFINE_H_
#define _TYPE_DEFINE_H_

//***************************************************
//*                    DATA TYPES                   *
//***************************************************
typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long int U32;
typedef char I8;
typedef int I16;
typedef long int I32;
typedef const char CI8;
typedef const int CI16;
typedef const long int CI32;
typedef float F32;
typedef double D64;

union adress_para
{
	U8 *pnt;
	U16 adr;
};

//typedef enum
//{																// BOOL 형식
//	TRUE = 1, FALSE = 0
//} bool_t;

typedef struct
{
	U8 b0 :1; /* bit0 */
	U8 b1 :1; /* bit1 */
	U8 b2 :1; /* bit2 */
	U8 b3 :1; /* bit3 */
	U8 b4 :1; /* bit4 */
	U8 b5 :1; /* bit5 */
	U8 b6 :1; /* bit6 */
	U8 b7 :1; /* bit7 */
} BIT; /* BIT형 bit access */

typedef struct
{
	U8 b0 :1; /* bit0 */
	U8 b1 :1; /* bit1 */
	U8 b2 :1; /* bit2 */
	U8 b3 :1; /* bit3 */
	U8 b4 :1; /* bit4 */
	U8 b5 :1; /* bit5 */
	U8 b6 :1; /* bit6 */
	U8 b7 :1; /* bit7 */
	U8 b8 :1; /* bit8 */
	U8 b9 :1; /* bit9 */
	U8 b10 :1; /* bit10 */
	U8 b11 :1; /* bit11 */
	U8 b12 :1; /* bit12 */
	U8 b13 :1; /* bit13 */
	U8 b14 :1; /* bit14 */
	U8 b15 :1; /* bit15 */
} BIT_16; /* BIT형 bit access */

typedef struct
{
	U8 b0 :1; /* bit0 */
	U8 b1 :1; /* bit1 */
	U8 b2 :1; /* bit2 */
	U8 b3 :1; /* bit3 */
	U8 b4 :1; /* bit4 */
	U8 b5 :1; /* bit5 */
	U8 b6 :1; /* bit6 */
	U8 b7 :1; /* bit7 */
	U8 b8 :1; /* bit8 */
	U8 b9 :1; /* bit9 */
	U8 b10 :1; /* bit10 */
	U8 b11 :1; /* bit11 */
	U8 b12 :1; /* bit12 */
	U8 b13 :1; /* bit13 */
	U8 b14 :1; /* bit14 */
	U8 b15 :1; /* bit15 */
	U8 b16 :1; /* bit16 */
	U8 b17 :1; /* bit17 */
	U8 b18 :1; /* bit18 */
	U8 b19 :1; /* bit19 */
	U8 b20 :1; /* bit20 */
	U8 b21 :1; /* bit21 */
	U8 b22 :1; /* bit22 */
	U8 b23 :1; /* bit23 */
	U8 b24 :1; /* bit24 */
	U8 b25 :1; /* bit25 */
	U8 b26 :1; /* bit26 */
	U8 b27 :1; /* bit27 */
	U8 b28 :1; /* bit28 */
	U8 b29 :1; /* bit29 */
	U8 b30 :1; /* bit30 */
	U8 b31 :1; /* bit31 */
} BIT_32;

typedef struct
{
	U8 lo :4; /* bit0~3 */
	U8 hi :4; /* bit4~7 */
} NIBBLE; /* NIBBLE형	nibble access */

typedef union
{
	U8 byte; /* byte 	*/
	BIT Bit; /* bit 		*/
	NIBBLE nibble; /* nibble 	*/
} BYTE_BIT; /* BYTE_BIT형 byte & bit & nibble access */

/*==============================================================================
 WORD형
 ==============================================================================*/
typedef union
{
	U16 word;			//	word
	struct
	{
		U8 b0;		//	하위 byte
		U8 b1;		//	상위 byte
	} byte;
	struct
	{
		U16 n1 :4;		//	bit12~15
		U16 n2 :4;		//	bit 8~11
		U16 n3 :4;		//	bit 4~7
		U16 n4 :4;		//	bit 0~3
	} nibble;
	struct
	{
		U16 b0 :1;		//	bit15
		U16 b1 :1;		//	bit14
		U16 b2 :1;		//	bit13
		U16 b3 :1;		//	bit12
		U16 b4 :1;		//	bit11
		U16 b5 :1;		//	bit10
		U16 b6 :1;		//	bit 9
		U16 b7 :1;		//	bit 8
		U16 b8 :1;		//	bit 7
		U16 b9 :1;		//	bit 6
		U16 b10 :1;     //	bit 5
		U16 b11 :1;     //	bit 4
		U16 b12 :1;     //	bit 3
		U16 b13 :1;     //	bit 2
		U16 b14 :1;     //	bit 1
		U16 b15 :1;     //	bit 0
	} Bit;
} WORD;					//	WORD	word & byte & nibble & bit access

/*==============================================================================
 LONG형
 ==============================================================================*/
typedef union
{
	U32 ulong;			//	word
	struct
	{
		U16 b0;		//	하위 byte
		U16 b1;		//	상위 byte
	} word;
	struct
	{
		U16 b0 :1;		//	bit15
		U16 b1 :1;		//	bit14
		U16 b2 :1;		//	bit13
		U16 b3 :1;		//	bit12
		U16 b4 :1;		//	bit11
		U16 b5 :1;		//	bit10
		U16 b6 :1;		//	bit 9
		U16 b7 :1;		//	bit 8
		U16 b8 :1;		//	bit 7
		U16 b9 :1;		//	bit 6
		U16 b10 :1;     //	bit 5
		U16 b11 :1;     //	bit 4
		U16 b12 :1;     //	bit 3
		U16 b13 :1;     //	bit 2
		U16 b14 :1;     //	bit 1
		U16 b15 :1;     //	bit 0
		U16 b16 :1;     //	bit15
		U16 b17 :1;     //	bit14
		U16 b18 :1;     //	bit13
		U16 b19 :1;     //	bit12
		U16 b20 :1;     //	bit11
		U16 b21 :1;     //	bit10
		U16 b22 :1;     //	bit 9
		U16 b23 :1;     //	bit 8
		U16 b24 :1;     //	bit 7
		U16 b25 :1;     //	bit 6
		U16 b26 :1;     //	bit 5
		U16 b27 :1;     //	bit 4
		U16 b28 :1;     //	bit 3
		U16 b29 :1;     //	bit 2
		U16 b30 :1;     //	bit 1
		U16 b31 :1;     //	bit 0
	} Bit;
} LONG;					//

typedef union
{
	U32 LONG;
	BIT_32 BIT;
} LONG_BIT;

struct void_func
{
	U8 num;
	void (*exeadr)(void);
};

typedef struct 										/*	3회 연속 일치형 (8bit)					*/
{
	U8 First; 										/*		이번 입력치					*/
	U8 Second; 										/*		전회 입력치					*/
	U8 Third; 										/*		전전번 입력치				*/
	BYTE_BIT Level; 								/*		이번 레벨 확정치				*/
	BYTE_BIT OnEdge; 								/*		이번 엣지 확정치				*/
	BYTE_BIT OffEdge; 								/*		이번 OFF엣지 확정치			*/
} ITTI3;

typedef struct 										/*	회 연속 일치형 (8bit)					*/
{
	U8 First; 										/*		이번 입력치					*/
	BYTE_BIT Level; 								/*		이번 레벨 확정치				*/
	BYTE_BIT OnEdge; 								/*		이번 엣지 확정치				*/
	BYTE_BIT OffEdge; 								/*		이번 OFF엣지 확정치			*/
} ITTI1;

typedef struct 										/*	3회 연속 일치형 (16bit)					*/
{
	U16 First; 										/*		이번 입력치					*/
	U16 Second; 									/*		전회 입력치					*/
	U16 Third; 										/*		전전번 입력치				*/
	WORD Level; 									/*		이번 레벨 확정치				*/
	WORD OnEdge; 									/*		이번 엣지 확정치				*/
	WORD OffEdge; 									/*		이번 OFF엣지 확정치			*/
} ITTI3_WORD;

typedef union
{
	U16 Word;
	BIT_16 Bit;
} WORD_BIT;

typedef struct
{													//	표준 SERIAL COMMAND 검색형
	U8 name;										//		COMMAND명
	U8 cmdsize;										//		COMMAND길이 (CMD1~CMDn까지의BYTE수)
	U8 (*exeadr)(void);								//		실행 함수
} CMD_PARA;

typedef struct
{
	U8 stepno;										//		스텝 No
	U8 SEGMENT :5;
	U8 EFFLUENT :5;
	U8 HOT_LED :5;
	U8 PURE_LED :5;
	U8 COLD_LED :5;
	U8 AMOUNT_LED :5;
	U8 HOT_LOCK :5;
	U8 CUP_LED :5;
	U8 COLD_TEMP_LED :5;
	U8 WIFI_LED :5;
	U8 CHANGE_ALRAM_LED :5;
	U8 FILTER_CLEAN_LED :5;
	U8 ML_LED :5;
	U8 DEGREE_LED :5;
	U8 STE_LED :5;
	U8 UV_LED :5;
	void (*exe)(void);
} Operation_StepTable;								//	운전스텝

typedef struct
{
	U8 stepno;										//		스텝 No
	U8 COLD_IN_V :2;
	U8 COLD_OUT_V :2;
	void (*exe)(void);
} ColdEff_StepTable;								//	냉수 추출 스텝

typedef struct
{
	U8 stepno;										//		스텝 No
	U8 TEM_EN  :2;
	U8 TEM_PWM :3;
	U8 FAN 	   :3;
	void (*exe)(void);
} Cooling_StepTable;									//	냉각스텝

typedef struct
{
	U8 stepno;										//		스텝 No
	U8 HOTIN_V :1;
	U8 HOTOUT_V :1;
	U8 HEATER_RELAYCMD :2;
	U8 HEATER_PWMCMD :4;
	void (*exe)(void);
} Hot_StepTable;									//	온수스텝

typedef struct
{
	U8 stepno;										//		스텝 No
	U8 PURE_V :3;
	void (*exe)(void);
} Pure_StepTable;

typedef struct
{
	U8 stepno;										//		스텝 No
	void (*exe)(void);
} PBA_StepTable;									//	PBA검사 스텝

typedef struct
{
	U8 stepno;										//		스텝 No
	void (*exe)(void);
} SmartCheck_StepTable;

typedef struct
{
	U16* TH_ADC_AVERAGE;						//	ADC 평균 값
	I16 DEGREE;										//	온도값 [1/10℃]
	U16 SHORT_DATA;									//	SHORT AD값
	U16 OPEN_DATA;									//	OPEN AD값
} TH_DataTable;										//	TH데이터 테이블

typedef struct
{
	U16 START_TIME;
	U16 ON_TIME;
} HEATER_ON_TIME;									//	히터 과사용 테이블


typedef struct
{
	unsigned char b0 :1;
	unsigned char b1 :1;
	unsigned char b2 :1;
	unsigned char b3 :1;
	unsigned char b4 :1;
	unsigned char b5 :1;
	unsigned char b6 :1;
	unsigned char b7 :1;
} BYTE_FIELD;

typedef struct
{
	unsigned char high_nibble :4;
	unsigned char low_nibble :4;
} BYTE_FIELD1;

typedef struct
{
	unsigned char b0 :1;
	unsigned char b1 :1;
	unsigned char b2 :1;
	unsigned char b3 :1;
	unsigned char b4 :1;
	unsigned char b5 :1;
	unsigned char b6 :1;
	unsigned char b7 :1;
	unsigned char b8 :1;
	unsigned char b9 :1;
	unsigned char b10 :1;
	unsigned char b11 :1;
	unsigned char b12 :1;
	unsigned char b13 :1;
	unsigned char b14 :1;
	unsigned char b15 :1;
} WORD_FIELD;

typedef struct
{
	unsigned char data_l :8;
	unsigned char data_h :8;
	unsigned char data_id :8;
	unsigned char parity :1;
	unsigned char msg_type :3;
	unsigned char space :4;
} OT_FIELD;

/*=======  BYTE IO ========*/
typedef union
{
	unsigned char byte;
	BYTE_FIELD Bit;
} TYPE_BYTE;

typedef union
{
	unsigned char byte;
	BYTE_FIELD1 Bit;
} TYPE_BYTE1;

/*======= WORD IO ========*/
typedef union
{
	unsigned int word;
	unsigned char byte[2];
	WORD_FIELD Bit;
} TYPE_WORD;

/*====== DOUBLE IO =======*/
typedef union
{
	unsigned long wword;
	unsigned int word[2];
	unsigned char byte[4];
	OT_FIELD Bit;
} TYPE_DWORD;

typedef struct
{
	U8 LED_Title;
	U8 Operation_CMD;
	U8 Display_CMD_Before;
	U8 Display_CMD;
	U8 Fade_PWM;
	U8 Fade_PWM_Target;
	U8 Fade_Timer;
	U8 Fade_Timer_Target;
} Display_Control;

typedef struct
{
	U8 stepno;										//		스텝 No
	void (*exe)(void);
} EEPROM_StepTable;

typedef struct
{
	U8 amount_volume;
	U8 enable;
}CupSize_Data;

#endif
