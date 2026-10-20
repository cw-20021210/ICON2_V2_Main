/// @file   I2C.h
/// @date   2019.07.10
/// @author PSM
/// @brief  EEPROM I2C 처리 관련 헤더 file

#ifdef EEPROM_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_EEPROM()		{	/*	1ms 타이머 감산			*/	\
									TDEC(gu8_TimerEEPROM_Write_Delay);	\
								}
#define	TDEC_10ms_EEPROM()		{	/*	10ms 타이머 감산		*/	\
									TDEC(gu8_TimerEEPROM_Limit);	\
								}
#define	TDEC_100ms_EEPROM()		{	/*	100ms 타이머 감산		*/	\
									for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++) {TDEC(EEPROM_Data_Table[i].write_timer);}	\
								}
#define	TDEC_1s_EEPROM()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_EEPROM()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_EEPROM()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_EEPROM()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_EEPROM()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
typedef struct {
	U16 address;					//	EEPROM 어드레스
	U8 init_data;					//	초기값 (Default data)
	U8 eeprom_data;					//	EEPROM 저장 데이터
	U8 write_flag;					//	쓰기 플래그
	U8 read_flag;					//	읽기 플래그
	U8 *data;						//	실 데이터
	U8 write_timer;					//	쓰기까지의 딜레이 타이머 (번복 쓰기 방지)
	U8 data_valid_check_lower;		//	데이터 유효값 최소
	U8 data_valid_check_upper;		//	데이터 유효값 최대
} EEPROM_Data;

//EEPROM ADDRESS
#define EEPROM_ADDR_FIRST_A                         (0x0000)	//	구분자A
#define EEPROM_ADDR_COLDONOFF                       	(0x0001)	//	냉수 ON/OFF
#define EEPROM_ADDR_SLEEPMODE                       	(0x0002)	//
#define EEPROM_ADDR_HOTLOCK                         	(0x0003)	//	온수 잠금
#define EERPOM_ADDR_CUPSIZE                 		(0x0004)	//	컵 사이즈
#define EEPROM_ADDR_HOTWATER_100DEGREE	            	(0x0005)	//
#define EEPROM_ADDR_ALLLOCK                         	(0x0006)	//	정전 전 전체 잠금 상태
#define EEPROM_ADDR_CUPSIZE_MYSET		    	(0x0007)	//
#define EEPROM_ADDR_FLUSHING_STATUS                 	(0x0008)	//	플러싱 모드 동작 여부
#define EEPROM_ADDR_HEATER_FILL			    	(0x0009)	//	히터 채우기 동작 여부
#define EEPROM_ADDR_COLDTANK_FILL		    	(0x000A)	//	냉수탱크 채우기 동작 여부
#define EEPROM_ADDR_CUPSIZE_MY_HIGH			(0x000B)	//
#define EEPROM_ADDR_CUPSIZE_MY_LOW			(0x000C)	//
#define EEPROM_ADDR_ERROR_HISTORY_1            		(0x000D)	//	에러이력1
#define EEPROM_ADDR_ERROR_HISTORY_2            		(0x000E)	//	에러이력2
#define EEPROM_ADDR_ERROR_HISTORY_3            		(0x000F)	//	에러이력3
#define EEPROM_ADDR_ERROR_HISTORY_4            		(0x0010)	//	에러이력4
#define EEPROM_ADDR_ERROR_HISTORY_5            		(0x0011)	//	에러이력5
#define EEPROM_ADDR_WIFI__POWER      			(0x0012)   	// 	WIFI On/Off Status
#define EEPROM_ADDR_FIRST__PAIRING  			(0x0013)   	// 	WIFI First Paring Status
#define EEPROM_ADDR_FOTA__MODULE     			(0x0014)   	// 	WIFI Module Fota Status
#define EEPROM_ADDR_FOTA__MCU      	  		(0x0015)   	// 	WIFI MCU Fota Status
#define EEPROM_ADDR_USEDAY_forCNT_Filter_HIGH     	(0x0016)   	// 	필터 사용일 수
#define EEPROM_ADDR_USEDAY_forCNT_Filter_LOW  		(0x0017)   	//	필터 사용일 수
#define EEPROM_ADDR_USEDAY_forCNT_Faucet_HIGH    	(0x0018)   	//
#define EEPROM_ADDR_USEDAY_forCNT_Faucet_LOW  		(0x0019)   	//
#define EEPROM_ADDR_EFFLUENT_LED_OFF 			(0x001A)	//
#define EEPROM_ADDR_DEFAULT_HOTTEMP        		(0x001B)	//
#define EEPROM_ADDR_FOTA			        (0x001C)	//	포타 진행
#define EEPROM_ADDR_SELFSERVICE		        	(0x001D)	//	자가관리
#define EEPROM_ADDR_FIRST_SET				(0x001E)	//
#define EEPROM_ADDR_COOLING_UP				(0x001F)	//	냉각성능 Up
#define EEPROM_ADDR_SOUND_TYPE				(0x0020)	//
#define EEPROM_ADDR_VOLUME				(0x0021)	//
#define EEPROM_ADDR_FAN_NOISECLEAR			(0x0022)	//
#define EEPROM_ADDR_HOT_DEGREEDISPLAY			(0x0023)	//
#define EEPROM_ADDR_SPARE1				(0x0024)	//
#define EEPROM_ADDR_SPARE2				(0x0025)	//
#define EEPROM_ADDR_COOLING_TARGET			(0x0026)	//
#define EEPROM_ADDR_COOLING_PID				(0x0027)	//
#define EEPROM_ADDR_ECOMODE_DISABLE			(0x0028)	//

#define EEPROM_ADDR_CUPSIZE_ARRAY_ONE_AMOUNT		(0x0029)
#define EEPROM_ADDR_CUPSIZE_ARRAY_TWO_AMOUNT		(0x002A)
#define EEPROM_ADDR_CUPSIZE_ARRAY_THREE_AMOUNT		(0x002B)
#define EEPROM_ADDR_CUPSIZE_ARRAY_FOUR_AMOUNT		(0x002C)
#define EEPROM_ADDR_CUPSIZE_ARRAY_FIVE_AMOUNT		(0x002D)
#define EEPROM_ADDR_CUPSIZE_ARRAY_ONE_ENABLE		(0x002E)
#define EEPROM_ADDR_CUPSIZE_ARRAY_TWO_ENABLE		(0x003F)
#define EEPROM_ADDR_CUPSIZE_ARRAY_THREE_ENABLE		(0x0030)
#define EEPROM_ADDR_CUPSIZE_ARRAY_FOUR_ENABLE		(0x0031)
#define EEPROM_ADDR_CUPSIZE_ARRAY_FIVE_ENABLE		(0x0032)
#define EEPROM_ADDR_HOT_TEMP_DATA_HIGH			(0x0033)
#define EEPROM_ADDR_HOT_TEMP_DATA_LOW			(0x0034)
#define EEPROM_ADDR_HOT_TEMP_SORTING			(0x0035)


#define EEPROM_ADDR_CHECKSUM_HIGH            		(0x0036)	//	체크섬 상위
#define EEPROM_ADDR_CHECKSUM_LOW            		(0x0037)	//	체크섬 하위
#define EEPROM_ADDR_FIRST_B                             (0x0038)	//	구분자B

#define EEPROM_ADDR_WIFI_PAIRING_HISTORY    (0x7E)  // 예시 주소 (기존 주소 확인 필요)  JHKO_260417
/* ※ 16bit dat HIGH, LOW는 Read시 실 데이터 변수에 넣는 작업 해줘야함. void EEPROM_Control_30(void) 함수 참고 */

enum {
	Z_EEPROM_ADDR_FIRST_A,
	Z_EEPROM_ADDR_COLDONOFF,
	Z_EEPROM_ADDR_SLEEPMODE,
	Z_EEPROM_ADDR_HOTLOCK,
	Z_EEPROM_ADDR_CUPSIZE,
	Z_EEPROM_ADDR_HOTWATER_100DEGREE,
	Z_EEPROM_ADDR_ALLLOCK,
	Z_EEPROM_ADDR_CUPSIZE_MYSET,
	Z_EEPROM_ADDR_FLUSHING_STATUS,
	Z_EEPROM_ADDR_HEATER_FILL,
	Z_EEPROM_ADDR_COLDTANK_FILL,
	Z_EEPROM_ADDR_CUPSIZE_MY_HIGH,
	Z_EEPROM_ADDR_CUPSIZE_MY_LOW,
	Z_EEPROM_ADDR_ERROR_HISTORY_1,
	Z_EEPROM_ADDR_ERROR_HISTORY_2,
	Z_EEPROM_ADDR_ERROR_HISTORY_3,
	Z_EEPROM_ADDR_ERROR_HISTORY_4,
	Z_EEPROM_ADDR_ERROR_HISTORY_5,
	Z_EEPROM_ADDR_WIFI__POWER,
	Z_EEPROM_ADDR_FIRST__PAIRING,
	Z_EEPROM_ADDR_FOTA__MODULE,
	Z_EEPROM_ADDR_FOTA__MCU,
	Z_EEPROM_ADDR_USEDAY_forCNT_Filter_HIGH,
	Z_EEPROM_ADDR_USEDAY_forCNT_Filter_LOW,
	Z_EEPROM_ADDR_USEDAY_forCNT_Faucet_HIGH,
	Z_EEPROM_ADDR_USEDAY_forCNT_Faucet_LOW,
	Z_EEPROM_ADDR_EFFLUENT_LED_OFF,
	Z_EEPROM_ADDR_DEFAULT_HOTTEMP,
	Z_EEPROM_ADDR_FOTA,
	Z_EEPROM_ADDR_SELFSERVICE,
	Z_EEPROM_ADDR_FIRST_SET,
	Z_EEPROM_ADDR_COOLING_UP,
	Z_EEPROM_ADDR_SOUND_TYPE,
	Z_EEPROM_ADDR_VOLUME,
	Z_EEPROM_ADDR_FAN_NOISECLEAR,
	Z_EEPROM_ADDR_HOT_DEGREEDISPLAY,
	Z_EEPROM_ADDR_SPARE1,
	Z_EEPROM_ADDR_SPARE2,
//@022	Z_EEPROM_ADDR_SPARE3,
	Z_ADDR_COOLING_TARGET,							//@022
//@022 Z_EEPROM_ADDR_SPARE4,
	Z_ADDR_COOLING_PID,								//@022
//@011	Z_EEPROM_ADDR_SPARE5,
	Z_EEPROM_ADDR_ECOMODE_DISABLE,					//@011
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_ONE_AMOUNT,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_TWO_AMOUNT,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_THREE_AMOUNT,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_FOUR_AMOUNT,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_FIVE_AMOUNT,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_ONE_ENABLE,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_TWO_ENABLE,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_THREE_ENABLE,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_FOUR_ENABLE,
	Z_EEPROM_ADDR_CUPSIZE_ARRAY_FIVE_ENABLE,
	Z_EEPROM_ADDR_HOT_TEMP_DATA_HIGH,
	Z_EEPROM_ADDR_HOT_TEMP_DATA_LOW,
	Z_EEPROM_ADDR_HOT_TEMP_SORTING,
	Z_EEPROM_ADDR_CHECKSUM_HIGH,
	Z_EEPROM_ADDR_CHECKSUM_LOW,
	Z_EEPROM_ADDR_FIRST_B,
	Z_EEPROM_ADDR_Data_Max,
};

#define	MY_CUPSIZE_CC								(550)

#define EEPROM_FIRST_A                              (0xAA)
//#define EEPROM_FIRST_A                              (0xBB)
#define EEPROM_DEFAULT_COLDONOFF                    (CLEAR)    	// 	Cold ON
#define EEPROM_DEFAULT_SLEEPMODE                    (CLEAR)    	// 	슬립모드  OFF
//#define EEPROM_DEFAULT_HOTONOFF                   (CLEAR)    	// 	Hot ON
#define EERPOM_DEFAULT_HOTLOCK                      (CLEAR)    	// 	Hot Unlock
//#define EERPOM_DEFAULT_CUPSIZE			    (CUPSIZE_HALFCUP)
#define EERPOM_DEFAULT_CUPSIZE			    (0)
#define EERPOM_DEFAULT_HOTWATER_100DEGREE	    (SET)
#define EEPROM_DEFAULT_MANAGEMENT                   (0)	    	//	관리모드
#define EEPROM_DEFAULT_ALLLOCK                      (CLEAR)    	// 	전체 잠금 미설정 상태
#define EEPROM_DEFAULT_FLUSHING_STATUS              (CLEAR)    	// 	플러싱 모드가 동작하지 않은 상태
#define EEPROM_DEFAULT_HEATER_FILL					(CLEAR)
#define EEPROM_DEFAULT_COLDTANK_FILL				(CLEAR)
#define EEPROM_DEFAULT_CUPSIZE_MY_HIGH				((U8)(MY_CUPSIZE_CC>>8&0xff))
#define EEPROM_DEFAULT_CUPSIZE_MY_LOW				((U8)(MY_CUPSIZE_CC&0xff))
#if 0
#define EEPROM_DEFAULT_HOTTEMP						(HOT_TEMP_SETUP_KOR100_JP90)
#endif
#define EEPROM_DEFAULT_HOTTEMP						(3)

#define EEPROM_DEFAULT_SOUND_TYPE              		(3)
#define EEPROM_DEFAULT_VOLUME	             		(1)			//	2단계
//#define EEPROM_DEFAULT_FILTERMODULE_AMOUNT_USED     (0)    		//	필터 사용량 0
//#define	EEPROM_DEFAULT_COOLING_TARGET				(Z_TEMP3)	//@022
#define	EEPROM_DEFAULT_COOLING_TARGET				(Z_TEMP4)	//@032
#define EEPROM_DEFAULT_COOLING_PI					(1)			//@032
#define EEPROM_FIRST_B                              (0x55)
//#define EEPROM_FIRST_B                              (0x56)

#define EEPROM_DEFAULT_CUPSIZE_ARRAY_ONE					(12)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_TWO					(25)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_THREE					(50)
//#define EEPROM_DEFAULT_CUPSIZE_ARRAY_ONE					(50)
//#define EEPROM_DEFAULT_CUPSIZE_ARRAY_TWO					(25)
//#define EEPROM_DEFAULT_CUPSIZE_ARRAY_THREE					(12)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_FOUR					(100)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_FIVE					(0xAA)	

#define EEPROM_DEFAULT_CUPSIZE_ARRAY_ONE_ENABLE					(1)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_TWO_ENABLE					(1)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_THREE_ENABLE				(1)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_FOUR_ENABLE				(1)
#define EEPROM_DEFAULT_CUPSIZE_ARRAY_FIVE_ENABLE				(1)

#define EEPROM_DEFAULT_HOT_TEMP_DATA_HIGH					(0x09)
#define EEPROM_DEFAULT_HOT_TEMP_DATA_LOW					(0x21)	


#define EEPROM_DEFAULT_HOT_TEMP_SORTING			(Z_HOT_TEMP_DESCENDING)

#define Z_EEPROM_Control_00		(0x00)
#define Z_EEPROM_Control_10		(0x10)
#define Z_EEPROM_Control_20		(0x20)
#define Z_EEPROM_Control_30		(0x30)
#define Z_EEPROM_Control_40		(0x40)

enum {
	Z_EEPROM_Control_Num_00,
	Z_EEPROM_Control_Num_10,
	Z_EEPROM_Control_Num_20,
	Z_EEPROM_Control_Num_30,
	Z_EEPROM_Control_Num_40,
	Z_EEPROM_Control_Num_Max
};

#define Z_EEPROM_Write_00		(0x00)
#define Z_EEPROM_Write_10		(0x10)
#define Z_EEPROM_Write_20		(0x20)
#define Z_EEPROM_Write_21		(0x21)
#define Z_EEPROM_Write_22		(0x22)
#define Z_EEPROM_Write_23		(0x23)
#define Z_EEPROM_Write_24		(0x24)
#define Z_EEPROM_Write_30		(0x30)

enum {
	Z_EEPROM_Write_Num_00,
	Z_EEPROM_Write_Num_10,
	Z_EEPROM_Write_Num_20,
	Z_EEPROM_Write_Num_21,
	Z_EEPROM_Write_Num_22,
	Z_EEPROM_Write_Num_23,
	Z_EEPROM_Write_Num_24,
	Z_EEPROM_Write_Num_30,
	Z_EEPROM_Write_Num_Max
};

#define Z_EEPROM_Read_00		(0x00)
#define Z_EEPROM_Read_10		(0x10)
#define Z_EEPROM_Read_20		(0x20)
#define Z_EEPROM_Read_21		(0x21)
#define Z_EEPROM_Read_22		(0x22)

enum {
	Z_EEPROM_Read_Num_00,
	Z_EEPROM_Read_Num_10,
	Z_EEPROM_Read_Num_20,
	Z_EEPROM_Read_Num_21,
	Z_EEPROM_Read_Num_22,
	Z_EEPROM_Read_Num_Max
};

#define Z_EEPROM_Write_Timer_Delay		(10)		//	EEPROM 데이터가 ?초 이상 다르면 write 시작 <?s/100ms>
#define Z_TimerEEPROM_Limit				(50)		//	<500ms/10ms>


/*==============================================================================
 Variable Definition
 ==============================================================================*/


/*==============================================================================
 function Definition
 =============================================================================*/
void EEPROM_Initialize(void);
void EEPROM_Control(void);
void EEPROM_Initialize(void);

EXTERN U8 gu8_TxBuf[4];								//	Tx 버퍼
EXTERN U8 gu8_RxBuf;								//	읽기 버퍼
EXTERN U8 gu8_EEPROM_OperNum;						//	현재 동작중인 Table Number

EXTERN U8 gu8_EEPROM_Control;						//	EEPROM 처리 스텝
EXTERN U8 gu8_EEPROM_ControlNum;

EXTERN U8 gu8_EEPROM_Write;							//	EEPROM Write 스텝
EXTERN U8 gu8_EEPROM_WriteNum;

EXTERN U8 gu8_EEPROM_Read;							//	EEPROM Read 스텝
EXTERN U8 gu8_EEPROM_ReadNum;

EXTERN U8 gu8_EEPROM_Data_First_A;					//	구분자A 데이터 저장 버퍼
EXTERN U8 gu8_EEPROM_Data_First_B;					//	구분자B 데이터 저장 버퍼
EXTERN U8 gu8_EEPROM_Data_Checksum_High;			//	체크섬 상위 저장 버퍼
EXTERN U8 gu8_EEPROM_Data_Checksum_Low;				//	체크섬 하위 저장 버퍼

EXTERN U8 F_EEPROM_Forced_Initialize;				//	EEPROM 초기화 Flag

EXTERN U8 gu8_TimerEEPROM_Write_Delay;				//	EEPROM Write 간격 Delay	<?ms/1ms>

EXTERN U8 F_Checksum_Write;

EXTERN U8 F_FirstEEPROM_Read;

EXTERN U8 gu8_TimerEEPROM_Limit;					//	<500ms/10ms>

EXTERN EEPROM_Data EEPROM_Data_Table[Z_EEPROM_ADDR_Data_Max];

EXTERN U8 gu8_WifiPower;
EXTERN U8 gu8_FirstPairing;
EXTERN U8 gu8_FOTA_Module;
EXTERN U8 gu8_FOTA_MCU;

EXTERN U16 gu16_DayCount_forFilter;
EXTERN U8 gu8_UseDay_forCNT_Filter_High;
EXTERN U8 gu8_UseDay_forCNT_Filter_Low;
EXTERN U8 F_FilterChange_Display;					//	필터 사용 도래 표시
EXTERN U8 F_Filter_Reset;							//	서버 송신 데이터

EXTERN U16 gu16_DayCount_forFaucet;
EXTERN U8 gu8_UseDay_forCNT_Faucet_High;
EXTERN U8 gu8_UseDay_forCNT_Faucet_Low;
EXTERN U8 F_FaucetChange_Display;
EXTERN U8 F_Faucet_Reset;							//	서버 송신 데이터

EXTERN U8 F_EEPROM_IniRead;

EXTERN U8 gu8_CupSize_My_High;
EXTERN U8 gu8_CupSize_My_Low;

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
