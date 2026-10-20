/// @file     eeprom.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    EEPROM 동작 관련 file
#define EEPROM_C
#include "include/_include.h"

void EEPROM_Control_00(void);       	//	부팅 후 AA,55 구분자 확인
void EEPROM_Control_10(void);			//	데이터 ALL 초기화 Write
void EEPROM_Control_20(void);			//	데이터 ALL Read
void EEPROM_Control_30(void);			//	Checksum 확인
void EEPROM_Control_40(void);			//	일반 동작

void Ini_EEPROM_Control_00(void);       //	부팅 후 AA,55 구분자 확인
void Ini_EEPROM_Control_10(void);		//	데이터 ALL 초기화 Write
void Ini_EEPROM_Control_20(void);		//	데이터 ALL Read
void Ini_EEPROM_Control_30(void);		//	Checksum 확인
void Ini_EEPROM_Control_40(void);		//	일반 동작

static void EEPROM_Control_NoSet(U8 data);

void EEPROM_Write_00(void);				//	초기화
void EEPROM_Write_10(void);				//	대기
void EEPROM_Write_20(void);				//	Write 시작
void EEPROM_Write_21(void);				//	Write 종료
void EEPROM_Write_22(void);				//	Write/Address 송신 (Write Confrim-1)
void EEPROM_Write_23(void);				//	Read CMD 송신 (Write Confrim-2)
void EEPROM_Write_24(void);				//	Read 종료 대기
void EEPROM_Write_30(void);				//	Checksum Write

void Ini_EEPROM_Write_00(void);			//	초기화
void Ini_EEPROM_Write_10(void);			//	대기
void Ini_EEPROM_Write_20(void);			//	Write 시작
void Ini_EEPROM_Write_21(void);			//	Write 종료
void Ini_EEPROM_Write_22(void);			//	Write/Address 송신 (Write Confrim-1)
void Ini_EEPROM_Write_23(void);			//	Read CMD 송신 (Write Confrim-2)
void Ini_EEPROM_Write_24(void);			//	Read 종료 대기
void Ini_EEPROM_Write_30(void);			//	Checksum Write

static void EEPROM_Write_NoSet(U8 data);

void EEPROM_Read_00(void);     			//	초기화
void EEPROM_Read_10(void);				//	대기
void EEPROM_Read_20(void);				//	Write/Address 송신
void EEPROM_Read_21(void);				//	Read CMD 송신
void EEPROM_Read_22(void);				//	Read 종료 대기

void Ini_EEPROM_Read_00(void);      	//	초기화
void Ini_EEPROM_Read_10(void);			//	대기
void Ini_EEPROM_Read_20(void);			//	Write/Address 송신
void Ini_EEPROM_Read_21(void);			//	Read CMD 송신
void Ini_EEPROM_Read_22(void);			//	Read 종료 대기

static void EEPROM_Read_NoSet(U8 data);

EEPROM_StepTable EEPROM_Control_Table[Z_EEPROM_Control_Num_Max] =
{
	//	스텝 No				Step Func.
	{ Z_EEPROM_Control_00,	EEPROM_Control_00 },	//	부팅 후 AA,55 구분자 확인
	{ Z_EEPROM_Control_10,	EEPROM_Control_10 },	//	데이터 ALL 초기화 Write
	{ Z_EEPROM_Control_20,	EEPROM_Control_20 },	//	데이터 ALL Read
	{ Z_EEPROM_Control_30,	EEPROM_Control_30 },	//	Checksum 확인
	{ Z_EEPROM_Control_40,	EEPROM_Control_40 },	//	일반 동작
};

EEPROM_StepTable EEPROM_Write_Table[Z_EEPROM_Write_Num_Max] =
{
	//	스텝 No				Step Func.
	{ Z_EEPROM_Write_00,	EEPROM_Write_00 },		//	초기화
	{ Z_EEPROM_Write_10,	EEPROM_Write_10 },		//	대기
	{ Z_EEPROM_Write_20,	EEPROM_Write_20 },		//	Write 시작
	{ Z_EEPROM_Write_21,	EEPROM_Write_21 },		//	Write 종료
	{ Z_EEPROM_Write_22,	EEPROM_Write_22 },     	//	Write/Address 송신
	{ Z_EEPROM_Write_23,	EEPROM_Write_23 },     	//	Read CMD 송신 (Write Confirm)
	{ Z_EEPROM_Write_24,	EEPROM_Write_24 },     	//	Read 종료 대기
	{ Z_EEPROM_Write_30,	EEPROM_Write_30 },		//	Checksum Write
};

EEPROM_StepTable EEPROM_Read_Table[Z_EEPROM_Read_Num_Max] =
{
	//	스텝 No				Step Func.
	{ Z_EEPROM_Read_00,		EEPROM_Read_00 },     	//	초기화
	{ Z_EEPROM_Read_10,		EEPROM_Read_10 },     	//	대기
	{ Z_EEPROM_Read_20,		EEPROM_Read_20 },     	//	Write/Address 송신
	{ Z_EEPROM_Read_21,		EEPROM_Read_21 },     	//	Read CMD 송신
	{ Z_EEPROM_Read_22,		EEPROM_Read_22 },     	//	Read 종료 대기
};

/* ※ 16bit dat HIGH, LOW는 Read시 실 데이터 변수에 넣는 작업 해줘야함. void EEPROM_Control_30(void) 함수 참고 */
EEPROM_Data EEPROM_Data_Table[Z_EEPROM_ADDR_Data_Max] =
{	//EEPROM Adr.					 	//Default Data				//EEPROM Data	//Write Flag			//Read Flag			//Real Data							//Write Delay Time	//Data Valid Check
	{EEPROM_ADDR_FIRST_A, 					EEPROM_FIRST_A, 			0, 		0, 				0, 				&gu8_EEPROM_Data_First_A, 		0, 					0xAA, 	0xAA },
	{EEPROM_ADDR_COLDONOFF, 				EEPROM_DEFAULT_COLDONOFF, 		0, 		0, 				0, 				&F_ColdOff, 				0, 					0, 		1	 },
	{EEPROM_ADDR_SLEEPMODE, 				EEPROM_DEFAULT_SLEEPMODE, 		0, 		0, 				0, 				&F_SleepMode, 				0, 					0, 		1	 },
	{EEPROM_ADDR_HOTLOCK, 					EERPOM_DEFAULT_HOTLOCK, 		0, 		0, 				0, 				&F_HotLock, 				0, 					0, 		1	 },
	{EERPOM_ADDR_CUPSIZE,					EERPOM_DEFAULT_CUPSIZE,		 	0, 		0, 				0, 				&gu8_DefaultCupSize,			0, 					0, 		4 },
	{EEPROM_ADDR_HOTWATER_100DEGREE,			EERPOM_DEFAULT_HOTWATER_100DEGREE,	0, 		0, 				0, 				&F_HotWater_100Degree,			0, 					0, 		1 	 },
	{EEPROM_ADDR_ALLLOCK, 					EEPROM_DEFAULT_ALLLOCK, 		0, 		0, 				0, 				&F_AllLock, 				0, 					0, 		1	 },
	{EEPROM_ADDR_CUPSIZE_MYSET,				0,					0, 		0, 				0, 				&F_CupSize_MySet, 			0, 					0, 		1	 },
	{EEPROM_ADDR_FLUSHING_STATUS,				EEPROM_DEFAULT_FLUSHING_STATUS, 	0, 		0, 				0, 				&F_FirstFlushing, 			0, 					0, 		1 	 },
	{EEPROM_ADDR_HEATER_FILL,				EEPROM_DEFAULT_HEATER_FILL,		0, 		0, 				0, 				&F_Heater_Fill,				0, 					0, 		1 	 },
	{EEPROM_ADDR_COLDTANK_FILL,				EEPROM_DEFAULT_COLDTANK_FILL,		0, 		0, 				0, 				&F_ColdTank_Fill,			0, 					0, 		1 	 },
	{EEPROM_ADDR_CUPSIZE_MY_HIGH,				EEPROM_DEFAULT_CUPSIZE_MY_HIGH,		0, 		0, 				0, 				&gu8_CupSize_My_High,			0, 					0, 		0xFF },
	{EEPROM_ADDR_CUPSIZE_MY_LOW,				EEPROM_DEFAULT_CUPSIZE_MY_LOW,		0, 		0, 				0, 				&gu8_CupSize_My_Low,			0, 					0, 		0xFF },
	{EEPROM_ADDR_ERROR_HISTORY_1, 	 			0, 					0, 		0, 				0, 				&gu8_ErrorHistory[0],		 	0, 					0, 		0xFF },
	{EEPROM_ADDR_ERROR_HISTORY_2, 	 			0, 					0, 		0, 				0, 				&gu8_ErrorHistory[1],			0, 					0, 		0xFF },
	{EEPROM_ADDR_ERROR_HISTORY_3, 	 			0, 					0, 		0, 				0, 				&gu8_ErrorHistory[2], 			0, 					0, 		0xFF },
	{EEPROM_ADDR_ERROR_HISTORY_4, 	 			0,  					0, 		0, 				0, 				&gu8_ErrorHistory[3], 			0, 					0, 		0xFF },
	{EEPROM_ADDR_ERROR_HISTORY_5, 	 			0,  					0, 		0, 				0, 				&gu8_ErrorHistory[4], 			0, 					0, 		0xFF },
	{EEPROM_ADDR_WIFI__POWER, 	 			0,  					0, 		0, 				0, 				&gu8_WifiPower,		 		0, 					0, 		0xFF },
	{EEPROM_ADDR_FIRST__PAIRING, 	 			0,  					0, 		0, 				0, 				&gu8_FirstPairing,			0, 					0, 		0xFF },
	{EEPROM_ADDR_FOTA__MODULE, 	 			0,  					0, 		0, 				0, 				&gu8_FOTA_Module,		 	0, 					0, 		0xFF },
	{EEPROM_ADDR_FOTA__MCU, 	 			0,  					0, 		0, 				0, 				&gu8_FOTA_MCU,		 		0, 					0, 		0xFF },
	{EEPROM_ADDR_USEDAY_forCNT_Filter_HIGH,			0,  					0, 		0, 				0, 				&gu8_UseDay_forCNT_Filter_High,		0, 					0, 		0xFF },
	{EEPROM_ADDR_USEDAY_forCNT_Filter_LOW, 			0,  					0, 		0, 				0, 				&gu8_UseDay_forCNT_Filter_Low,		0, 					0, 		0xFF },
	{EEPROM_ADDR_USEDAY_forCNT_Faucet_HIGH,			0,  					0, 		0, 				0, 				&gu8_UseDay_forCNT_Faucet_High,		0, 					0, 		0xFF },
	{EEPROM_ADDR_USEDAY_forCNT_Faucet_LOW, 			0,  					0, 		0, 				0, 				&gu8_UseDay_forCNT_Faucet_Low,		0, 					0, 		0xFF },
	{EEPROM_ADDR_EFFLUENT_LED_OFF, 	 			0,  					0, 		0, 				0, 				&F_EffluentLED_OFF,			0, 					0, 		1    },
	{EEPROM_ADDR_DEFAULT_HOTTEMP,		 		EEPROM_DEFAULT_HOTTEMP,			0, 		0, 				0, 				&gu8_DefaultHotTempSetup,		0, 					0, 		0xFF },
	{EEPROM_ADDR_FOTA,					0,  					0, 		0, 				0, 				&F_FOTA_Setup,				0, 					0, 		1	 },
	{EEPROM_ADDR_SELFSERVICE,			 	0,  					0, 		0, 				0, 				&F_SelfService,				0, 					0, 		1	 },
	{EEPROM_ADDR_FIRST_SET,			 	 	0,  					0, 		0, 				0, 				&F_First_Set,				0, 					0, 		1	 },
	{EEPROM_ADDR_COOLING_UP,	 	 		0,  					0, 		0, 				0, 				&F_Cooling_Up, 				0, 					0, 		1	 },
	{EEPROM_ADDR_SOUND_TYPE,  				EEPROM_DEFAULT_SOUND_TYPE,		0, 		0, 				0, 				&gu8_Sound_Type, 			0, 					1, 		3	 },
	{EEPROM_ADDR_VOLUME, 	 				EEPROM_DEFAULT_VOLUME,			0, 		0, 				0, 				&gu8_VoiceVolumeLevel,			0, 					0, 		4    },
	{EEPROM_ADDR_FAN_NOISECLEAR, 				0,  					0, 		0, 				0, 				&F_FAN_NoiseClear,			0, 					0, 		1	 },
	{EEPROM_ADDR_HOT_DEGREEDISPLAY, 	 		0,  					0, 		0, 				0, 				&F_HotWater_DegreeDisplay, 		0, 					0, 		1    },
	{EEPROM_ADDR_SPARE1, 	 				0,  					0, 		0, 				0, 				NULL, 					0, 					0, 		0xFF },
	{EEPROM_ADDR_SPARE2, 	 				0,  					0, 		0, 				0, 				NULL, 					0, 					0, 		0xFF },
//@022	{EEPROM_ADDR_SPARE3, 	 				0,  					0, 		0, 				0, 				NULL, 					0, 					0, 		0xFF },
	{EEPROM_ADDR_COOLING_TARGET, 	 			EEPROM_DEFAULT_COOLING_TARGET,		0, 		0, 				0, 				&gu8_Cooling_TargetDegree_forEEPROM,	0, 					0, 		0xFF },	//@022
//@022	{EEPROM_ADDR_SPARE4, 	 				0,  					0, 		0, 				0, 				NULL, 					0, 					0, 		0xFF },
//@032	{EEPROM_ADDR_COOLING_PID, 				0,  					0, 		0, 				0, 				&F_Cooling_PID,				0, 					0, 		1 },	//@022
	{EEPROM_ADDR_COOLING_PID, 				EEPROM_DEFAULT_COOLING_PI,  		0, 		0, 				0, 				&F_Cooling_PID,				0, 					0, 		1 },	//@032
//@011	{EEPROM_ADDR_SPARE5, 	 				0,  					0, 		0, 				0, 				NULL, 					0, 					0, 		0xFF },
	{EEPROM_ADDR_ECOMODE_DISABLE, 	 			0,  					0, 		0, 				0, 				&F_CoolingEcoMode_Disable, 		0, 					0, 		1 	 },	//@011
	
	{EEPROM_ADDR_CUPSIZE_ARRAY_ONE_AMOUNT,			EEPROM_DEFAULT_CUPSIZE_ARRAY_ONE,		0,	0,				0,				&gu8_Cup_size_Array[0].amount_volume,	0,					0,		100},
	{EEPROM_ADDR_CUPSIZE_ARRAY_TWO_AMOUNT,			EEPROM_DEFAULT_CUPSIZE_ARRAY_TWO,		0,	0,				0,				&gu8_Cup_size_Array[1].amount_volume,	0,					0,		100},
	{EEPROM_ADDR_CUPSIZE_ARRAY_THREE_AMOUNT,		EEPROM_DEFAULT_CUPSIZE_ARRAY_THREE,		0,	0,				0,				&gu8_Cup_size_Array[2].amount_volume,	0,					0,		100},
	{EEPROM_ADDR_CUPSIZE_ARRAY_FOUR_AMOUNT,			EEPROM_DEFAULT_CUPSIZE_ARRAY_FOUR,		0,	0,				0,				&gu8_Cup_size_Array[3].amount_volume,	0,					0,		100},
	{EEPROM_ADDR_CUPSIZE_ARRAY_FIVE_AMOUNT,			EEPROM_DEFAULT_CUPSIZE_ARRAY_FIVE,		0,	0,				0,				&gu8_Cup_size_Array[4].amount_volume,	0,					0,		0xAA},
	{EEPROM_ADDR_CUPSIZE_ARRAY_ONE_ENABLE,			EEPROM_DEFAULT_CUPSIZE_ARRAY_ONE_ENABLE,	0,	0,				0,				&gu8_Cup_size_Array[0].enable,		0,					0,		1},
	{EEPROM_ADDR_CUPSIZE_ARRAY_TWO_ENABLE,			EEPROM_DEFAULT_CUPSIZE_ARRAY_TWO_ENABLE,	0,	0,				0,				&gu8_Cup_size_Array[1].enable,		0,					0,		1},
	{EEPROM_ADDR_CUPSIZE_ARRAY_THREE_ENABLE,		EEPROM_DEFAULT_CUPSIZE_ARRAY_THREE_ENABLE,	0,	0,				0,				&gu8_Cup_size_Array[2].enable,		0,					0,		1},
	{EEPROM_ADDR_CUPSIZE_ARRAY_FOUR_ENABLE,			EEPROM_DEFAULT_CUPSIZE_ARRAY_FOUR_ENABLE,	0,	0,				0,				&gu8_Cup_size_Array[3].enable,		0,					0,		1},
	{EEPROM_ADDR_CUPSIZE_ARRAY_FIVE_ENABLE,			EEPROM_DEFAULT_CUPSIZE_ARRAY_FIVE_ENABLE,	0,	0,				0,				&gu8_Cup_size_Array[4].enable,		0,					0,		1},
	{EEPROM_ADDR_HOT_TEMP_DATA_HIGH,			EEPROM_DEFAULT_HOT_TEMP_DATA_HIGH,		0,	0,				0,				&gu8_hot_temp_data_for_wifi.byte[1],	0,					0,		0xFF},
	{EEPROM_ADDR_HOT_TEMP_DATA_LOW,				EEPROM_DEFAULT_HOT_TEMP_DATA_LOW,		0,	0,				0,				&gu8_hot_temp_data_for_wifi.byte[0],	0,					0,		0xFF},
	{EEPROM_ADDR_HOT_TEMP_SORTING,				EEPROM_DEFAULT_HOT_TEMP_SORTING,		0,	0,				0,				&gu8_f_hot_temp_sorting,		0,					0,		1},

	{EEPROM_ADDR_CHECKSUM_HIGH, 	 			0,  					0, 		0, 				0, 				&gu8_EEPROM_Data_Checksum_High, 	0, 					0, 		0xFF },
	{EEPROM_ADDR_CHECKSUM_LOW, 		 		0,  					0, 		0, 				0, 				&gu8_EEPROM_Data_Checksum_Low, 		0, 					0, 		0xFF },
	{EEPROM_ADDR_FIRST_B, 			 		EEPROM_FIRST_B, 			0, 		0, 				0, 				&gu8_EEPROM_Data_First_B, 			0, 					0x55,	0x55 },
};

#define Z_TimerEEPROM_Write_Delay	(50)		//	50ms

/// @brief	  EEPROM 기능 초기화
/// @param    void
/// @return   void
void EEPROM_Initialize(void)
{
	Ini_EEPROM_Control_00();
	Ini_EEPROM_Write_00();
	Ini_EEPROM_Read_00();
}

/// @brief    EEPROM 처리 (Main Loop)
/// @param    void
/// @return   void
void EEPROM_Control(void)
{
	if ((gu8_EEPROM_ControlNum < Z_EEPROM_Control_Num_Max) && (gu8_EEPROM_WriteNum < Z_EEPROM_Write_Num_Max) && (gu8_EEPROM_ReadNum < Z_EEPROM_Read_Num_Max))	//	스텝 허용 범위인 경우
	{
		(EEPROM_Control_Table[gu8_EEPROM_ControlNum].exe)();	//	해당 스텝 함수 실행
		(EEPROM_Write_Table[gu8_EEPROM_WriteNum].exe)();		//	해당 스텝 함수 실행
		(EEPROM_Read_Table[gu8_EEPROM_ReadNum].exe)();			//	해당 스텝 함수 실행
	}
	else
	{
		SafetyLoop();
	}
}

/// @brief    EEPROM 초기화 (구분자A,B 유효성 체크)
/// @param    void
/// @return   void
void EEPROM_Control_00(void)
{
	static U8 reTry = CLEAR;

	if ((EEPROM_Data_Table[Z_EEPROM_ADDR_FIRST_A].read_flag == CLEAR) && (EEPROM_Data_Table[Z_EEPROM_ADDR_FIRST_B].read_flag == CLEAR)) //	구분자 읽기 종료 시
	{
		if ((gu8_EEPROM_Data_First_A == EEPROM_FIRST_A) && (gu8_EEPROM_Data_First_B == EEPROM_FIRST_B))		//	구분자 데이터 OK인 경우
		{
			F_EEPROM_IniRead = SET;
			Ini_EEPROM_Control_20();				//	Data ALL Read
		}
		else
		{
			if (reTry == CLEAR)
			{
				reTry = SET;
				Ini_EEPROM_Control_00();
			}
			else
			{
				Ini_EEPROM_Control_10();				//	Data ALL Clear
			}
		}
	}
}
void Ini_EEPROM_Control_00(void)
{
	EEPROM_Data_Table[Z_EEPROM_ADDR_FIRST_A].read_flag = SET;
	EEPROM_Data_Table[Z_EEPROM_ADDR_FIRST_B].read_flag = SET;
	EEPROM_Control_NoSet(Z_EEPROM_Control_Num_00);
}

/// @brief    EEPROM 데이터 ALL 초기화
/// @param    void
/// @return   void
void EEPROM_Control_10(void)
{
	U8 i = 0;
	U8 flag = 0;

	for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)		//	잔여 Write 데이터 확인
	{
		if (EEPROM_Data_Table[i].write_flag == SET)		//		잔여 Write가 있는 경우
		{
			flag = SET;									//			flag set
			break;
		}


	}

	if (flag == CLEAR)								//	잔여 Write 데이터가 없는 경우
	{
		Ini_EEPROM_Control_30();					//		checksum 확인
	}

}
void Ini_EEPROM_Control_10(void)
{
	U8 i = 0;

	for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)	//	전 영역 write set
	{
		EEPROM_Data_Table[i].write_flag = SET;
		if ((i == Z_EEPROM_ADDR_HOTLOCK) && (gu8_ModelType_forNational == MODEL_JAPAN))
		{
			*EEPROM_Data_Table[i].data = SET;		//		온수 잠금 설정
		}
		else if ((i == Z_EEPROM_ADDR_SELFSERVICE) && (gu8_ModelType_forNational == MODEL_JAPAN))
		{
			*EEPROM_Data_Table[i].data = SET;		//		자가관리 설정
		}
		else if ((i == Z_EEPROM_ADDR_DEFAULT_HOTTEMP) && (gu8_ModelType_forNational == MODEL_JAPAN))
		{
			*EEPROM_Data_Table[i].data = HOT_TEMP_SETUP_KOR70_JP70;		//		70℃ 설정
		}
		else
		{
			*EEPROM_Data_Table[i].data = EEPROM_Data_Table[i].init_data;	//		초기값으로 set
		}
	}

	EEPROM_Control_NoSet(Z_EEPROM_Control_Num_10);
}

/// @brief    EEPROM ALL Read
/// @param    void
/// @return   void
void EEPROM_Control_20(void)
{
	U8 i = 0;
	U8 flag = 0;

	for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)	//	잔여 read 영역 확인
	{
		if (EEPROM_Data_Table[i].read_flag == SET)	//		잔여 read가 있는 경우
		{
			flag = SET;								//			flag set
			break;
		}
	}

	if (flag == CLEAR)								//	잔여	read가 없는 경우
	{
		Ini_EEPROM_Control_30();					//		checksum 확인
	}
}
void Ini_EEPROM_Control_20(void)
{
	U8 i = 0;

	for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)	//	전 영역 read set
	{
		EEPROM_Data_Table[i].read_flag = SET;
	}

	EEPROM_Control_NoSet(Z_EEPROM_Control_Num_20);
}

/// @brief    EEPROM Checksum 확인
/// @param    void
/// @return   void
void EEPROM_Control_30(void)
{
	U8 i = 0;
	U16 checksum_data = 0;
	U16 eeprom_checksum_data = 0;
	static U8 recheck = 0;

	for (i = 0 ; i < Z_EEPROM_ADDR_CHECKSUM_HIGH ; i++)	//	checksum 영역 전 주소까지 sum
	{
		checksum_data += *EEPROM_Data_Table[i].data;
	}

	//	EEPROM 주소 내의 값과 sum값의 비교
	//	eeprom_checksum_data = ((*EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_HIGH].data) << 8) | *EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_LOW].data;
		eeprom_checksum_data = ((U16)(*EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_HIGH].data) << 8) | (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_LOW].data;
	//	eeprom_checksum_data = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_HIGH].data) << 8) | (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_LOW].data;

	if (checksum_data == eeprom_checksum_data)		//	정상 데이터일 경우
	{
		Ini_EEPROM_Control_40();					//		일반 동작

		gu16_CupSize_My = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CUPSIZE_MY_HIGH].data << 8) & 0xFF00U);
		gu16_CupSize_My |= (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CUPSIZE_MY_LOW].data & 0x00FFU;

		gu16_DayCount_forFilter = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Filter_HIGH].data << 8) & 0xFF00U);
		gu16_DayCount_forFilter |= (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Filter_LOW].data & 0x00FFU;

		gu16_DayCount_forFaucet = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Faucet_HIGH].data << 8) & 0xFF00U);
		gu16_DayCount_forFaucet |= (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Faucet_LOW].data & 0x00FFU;

		gu16_Cooling_TargetDegree = (U16)gu8_Cooling_TargetDegree_forEEPROM;	//@022
	}
	else
	{
		if (recheck == SET)		//		1회만 all re wirte 판단
		{
			Ini_EEPROM_Control_40();				//			1회 한 경우는 일반 동작
		}
		else
		{
			recheck = SET;
			if (F_EEPROM_IniRead == SET)
			{
				Ini_EEPROM_Control_20();				//	Data ALL Read
			}
			else
			{
				Ini_EEPROM_Control_10();				//	Data ALL Clear
			}
		}
	}
}
void Ini_EEPROM_Control_30(void)
{
	EEPROM_Control_NoSet(Z_EEPROM_Control_Num_30);
}

/// @brief    EEPROM 일반 동작 (데이터 변경 시  Write)
/// @param    void
/// @return   void
void EEPROM_Control_40(void)
{
	U8 i = 0;
	U16 buf = 0;

	//	My용량 2바이트 Write 체크
	buf = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CUPSIZE_MY_HIGH].data << 8) & 0xFF00U);
	buf |= (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_CUPSIZE_MY_LOW].data & 0x00FFU;
	if (gu16_CupSize_My != buf)
	{
		gu8_CupSize_My_High = (U8)((gu16_CupSize_My >> 8) & 0xFFU);
		gu8_CupSize_My_Low = (U8)(gu16_CupSize_My & 0xFFU);
	}

	//	필터 사용일 2바이트 Write 체크
	buf = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Filter_HIGH].data << 8) & 0xFF00U);
	buf |= (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Filter_LOW].data & 0x00FFU;
	if (gu16_DayCount_forFilter != buf)
	{
		gu8_UseDay_forCNT_Filter_High = (U8)((gu16_DayCount_forFilter >> 8) & 0xFFU);
		gu8_UseDay_forCNT_Filter_Low = (U8)(gu16_DayCount_forFilter & 0xFFU);
	}

	//	유로모듈 사용일 2바이트 Write 체크
	buf = (((U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Faucet_HIGH].data << 8) & 0xFF00U);
	buf |= (U16)*EEPROM_Data_Table[Z_EEPROM_ADDR_USEDAY_forCNT_Faucet_LOW].data & 0x00FFU;
	if (gu16_DayCount_forFaucet != buf)
	{
		gu8_UseDay_forCNT_Faucet_High = (U8)((gu16_DayCount_forFaucet >> 8) & 0xFFU);
		gu8_UseDay_forCNT_Faucet_Low = (U8)(gu16_DayCount_forFaucet & 0xFFU);
	}

	if (gu8_Cooling_TargetDegree_forEEPROM != (U8)gu16_Cooling_TargetDegree)	//@022
	{                                                                           //@022
		gu8_Cooling_TargetDegree_forEEPROM = (U8)gu16_Cooling_TargetDegree;     //@022
	}                                                                           //@022

	for (i = 0 ; i < Z_EEPROM_ADDR_CHECKSUM_HIGH ; i++)	//	checksum 주소 전까지
	{
		if (*EEPROM_Data_Table[i].data == EEPROM_Data_Table[i].eeprom_data)	//	실제 데이터와 현재 저장된 EEPROM값이 같은 경우 (다른경우 타이머를 흘려 Write지시)
		{
			EEPROM_Data_Table[i].write_timer = Z_EEPROM_Write_Timer_Delay;
		}

		if (EEPROM_Data_Table[i].write_timer == 0)	//		write 타이머 경과된 경우
		{
			if (F_TestMode.byte == 0x00)			//		테스트 모드가 아닌경우
			{
				EEPROM_Data_Table[i].write_flag = SET;	//			write flag set
			}
		}
	}

	if (F_EEPROM_Forced_Initialize == SET)			//	EEPROM 강제 초기화 인 경우
	{
		F_EEPROM_Forced_Initialize = CLEAR;			//		flag clear
		Ini_EEPROM_Control_10();					//		EEPROM all clear
	}
}
void Ini_EEPROM_Control_40(void)
{
	F_FirstEEPROM_Read = SET;						//	최초 읽기 완료 flag
	EEPROM_Control_NoSet(Z_EEPROM_Control_Num_40);
}

/// @brief    EEPROM 처리 Step
/// @param    void
/// @return   void
static void EEPROM_Control_NoSet(U8 data)
{
	static const U8 DT_EEPROM_ControlNumber[] =
	{
		Z_EEPROM_Control_00,
		Z_EEPROM_Control_10,
		Z_EEPROM_Control_20,
		Z_EEPROM_Control_30,
		Z_EEPROM_Control_40,
	};
	/************************************************/
	gu8_EEPROM_ControlNum = data; 								/*	일련 번호 set*/

	gu8_EEPROM_Control = DT_EEPROM_ControlNumber[gu8_EEPROM_ControlNum];	/*	상태 mode No. set*/
}

/// @brief    EEPROM Write 초기화
/// @param    void
/// @return   void
void EEPROM_Write_00(void)
{
	Ini_EEPROM_Write_10();
}
void Ini_EEPROM_Write_00(void)
{
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_00);
}

/// @brief    EEPROM Write Flag 확인
/// @param    void
/// @return   void
void EEPROM_Write_10(void)
{
	U8 i = 0;

	if (gu8_EEPROM_Read <= Z_EEPROM_Read_10)
	{
		for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)
		{
			if (EEPROM_Data_Table[i].write_flag == SET)	//	write 지시가 있는 경우
			{
				gu8_EEPROM_OperNum = i;					//		해당 주소 넘버링 저장
				gu8_TxBuf[0] = (U8)(EEPROM_Data_Table[i].address & 0xFF);	//		주소입력
				gu8_TxBuf[1] = *EEPROM_Data_Table[i].data;		//		데이터 입력
				EEPROM_Data_Table[i].eeprom_data = *EEPROM_Data_Table[i].data;	//		EEPROM 데이터 입력
				Ini_EEPROM_Write_20();					//		write 시작
				break;
			}
		}
	}
}
void Ini_EEPROM_Write_10(void)
{
	gu8_EEPROM_OperNum = 0;
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_10);
}

/// @brief    EEPROM Write 시작
/// @param    void
/// @return   void
void EEPROM_Write_20(void)
{
	if (gu8_TimerEEPROM_Write_Delay == 0)			//	50ms
	{
		R_IICA0_Master_Send(_A0_IICA0_MASTERADDRESS, gu8_TxBuf, 2, 0);		//		통신 지시
		Ini_EEPROM_Write_21();						//		종료대기 이행
	}
}
void Ini_EEPROM_Write_20(void)
{
	gu8_TimerEEPROM_Write_Delay = Z_TimerEEPROM_Write_Delay;
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_20);
}

/// @brief    EEPROM Write 종료 확인
/// @param    void
/// @return   void
void EEPROM_Write_21(void)
{
	U8 i = 0;
	U8 flag = CLEAR;

	if (0U == IICBSY0)								//	busy 종료
	{
		Ini_EEPROM_Write_22();						//		write confirm
	}
	else if (gu8_TimerEEPROM_Limit == 0)			//	리미트 타이머
	{
		Ini_EEPROM_Write_10();						//		대기스텝
		F_EEPROM_CommunicationError = SET;			//		에러 flag set
	}
	else
	{

	}
}
void Ini_EEPROM_Write_21(void)
{
	gu8_TimerEEPROM_Limit = Z_TimerEEPROM_Limit;
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_21);
}

/// @brief    Write/Address 송신 (Write Confirm)
/// @param    void
/// @return   void
void EEPROM_Write_22(void)
{
	if (gu8_TimerEEPROM_Write_Delay == 0)			//	대기 타이머 경과 시
	{
		R_IICA0_Master_Send(_A0_IICA0_MASTERADDRESS, gu8_TxBuf, 1, 0);	//	주소 데이터 송신
		Ini_EEPROM_Write_23();
	}
}
void Ini_EEPROM_Write_22(void)
{
	gu8_TimerEEPROM_Write_Delay = Z_TimerEEPROM_Write_Delay;
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_22);
}

/// @brief    Read CMD 송신 (Write Confirm)
/// @param    void
/// @return   void
void EEPROM_Write_23(void)
{
	U8 i = 0;
	U8 flag = 0;

	if (0U == IICBSY0)								//	busy 체크
	{
		R_IICA0_Master_Receive(_A0_IICA0_MASTERADDRESS, &gu8_RxBuf, 1, 0);	//		데이터 읽기
		Ini_EEPROM_Write_24();
	}
	else if (gu8_TimerEEPROM_Limit == 0)			//	리미트 타이머
	{
		Ini_EEPROM_Write_10();						//		대기 스텝
		F_EEPROM_CommunicationError = SET;			//		에러 flag set
	}
	else
	{

	}
}
void Ini_EEPROM_Write_23(void)
{
	gu8_TimerEEPROM_Limit = Z_TimerEEPROM_Limit;
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_23);
}

/// @brief    Read 종료 대기
/// @param    void
/// @return   void
void EEPROM_Write_24(void)
{
	U8 i = 0;
	U8 flag = 0;
	static U8 flag_reWrite = 0;

	if (0U == IICBSY0)								//	busy 체크
	{
		EEPROM_Data_Table[gu8_EEPROM_OperNum].write_flag = CLEAR;	//		write flag clear
		if (gu8_RxBuf == EEPROM_Data_Table[gu8_EEPROM_OperNum].eeprom_data)	//		수신 데이터와 쓰기 데이터가 동일한 경우
		{
			flag_reWrite = CLEAR;
			for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)		//		잔여 write 확인
			{
				if (EEPROM_Data_Table[i].write_flag == SET)
				{
					flag = SET;
					break;
				}
			}

			if (flag == SET)						//		잔여 write가 있는 경우
			{
				Ini_EEPROM_Write_10();				//			대기 스텝
			}
			else									//		잔여 write가 없는 경우
			{
				if (F_Checksum_Write == CLEAR)		//			Checksum write 경험 없는 경우
				{
					Ini_EEPROM_Write_30();			//				Checksum write
				}
				else
				{
					F_Checksum_Write = CLEAR;		//			Checksum write clear
					Ini_EEPROM_Write_10();			//
				}
			}
		}
		else										//		수신 데이터와 쓰기 데이터가 동일 하지 않은 경우
		{
			if (flag_reWrite == CLEAR)				//			re-Write 경험 없는 경우
			{
				EEPROM_Data_Table[gu8_EEPROM_OperNum].write_flag =  SET; 	//	re-Write 시도 1회
				flag_reWrite = SET;					//				re-Write 경험 set
			}
			else
			{
				if (gu8_EEPROM_OperNum == (Z_EEPROM_ADDR_Data_Max-1))
				{
					F_EEPROM_CommunicationError = SET;
				}
				flag_reWrite = CLEAR;
			}
			Ini_EEPROM_Write_10();
		}
	}
	else if (gu8_TimerEEPROM_Limit == 0)			//	리미트 타이머
	{
		Ini_EEPROM_Write_10();						//		대기 스텝
		F_EEPROM_CommunicationError = SET;			//		에러 플래그 set
	}
	else
	{

	}
}
void Ini_EEPROM_Write_24(void)
{
	gu8_TimerEEPROM_Limit = Z_TimerEEPROM_Limit;
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_24);
	R_IICA0_Master_Receive(_A0_IICA0_MASTERADDRESS, &gu8_RxBuf, 1, 0);
}

/// @brief    Checksum Write
/// @param    void
/// @return   void
void EEPROM_Write_30(void)
{
	U8 i = 0;
	U16 checksum_data = 0;

	for (i = 0 ; i < Z_EEPROM_ADDR_CHECKSUM_HIGH ; i++)	//	Checksum 주소 전까지 데이터 sum
	{
		checksum_data += *EEPROM_Data_Table[i].data;
	}

	gu8_EEPROM_Data_Checksum_High = (U8)((checksum_data >> 8) & 0xFF);	//	2바이트 High 1바이트 구분
	gu8_EEPROM_Data_Checksum_Low = (U8)(checksum_data & 0xFF);	//	2바이트 Low 2바이트 구분

	EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_HIGH].write_flag = SET;	//	High write set
	EEPROM_Data_Table[Z_EEPROM_ADDR_CHECKSUM_LOW].write_flag = SET;		//	Low write set

	Ini_EEPROM_Write_10();
}
void Ini_EEPROM_Write_30(void)
{
	F_Checksum_Write = SET;							//	chekcsum Write flag set
	EEPROM_Write_NoSet(Z_EEPROM_Write_Num_30);
}

/// @brief    EEPROM Write 스텝
/// @param    void
/// @return   void
static void EEPROM_Write_NoSet(U8 data)
{
	static const U8 DT_EEPROM_WriteNumber[] =
	{
		Z_EEPROM_Write_00,
		Z_EEPROM_Write_10,
		Z_EEPROM_Write_20,
		Z_EEPROM_Write_21,
		Z_EEPROM_Write_22,
		Z_EEPROM_Write_23,
		Z_EEPROM_Write_24,
		Z_EEPROM_Write_30,
	};
	/************************************************/
	gu8_EEPROM_WriteNum = data; 								/*	일련 번호 set*/

	gu8_EEPROM_Write = DT_EEPROM_WriteNumber[gu8_EEPROM_WriteNum];	/*	상태 mode No. set*/
}

/// @brief    EEPROM 읽기 초기화
/// @param    void
/// @return   void
void EEPROM_Read_00(void)
{
	Ini_EEPROM_Read_10();
}
void Ini_EEPROM_Read_00(void)
{
	EEPROM_Read_NoSet(Z_EEPROM_Read_Num_00);
}

/// @brief    EEPROM 읽기 대기
/// @param    void
/// @return   void
void EEPROM_Read_10(void)
{
	U8 i = 0;

	if (gu8_EEPROM_Write <= Z_EEPROM_Write_10)
	{
		for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max ; i++)
		{
			if (EEPROM_Data_Table[i].read_flag == SET)	//		읽기 요구가 있는 경우
			{
				gu8_EEPROM_OperNum = i;					//		주소 넘버링 저장
				gu8_TxBuf[0] = (U8)(EEPROM_Data_Table[i].address & 0xFF);	//		주소 데이터 set
				Ini_EEPROM_Read_20();
				break;
			}
		}
	}
}
void Ini_EEPROM_Read_10(void)
{
	U8 i = 0;
	gu8_EEPROM_OperNum = 0;

//	for (i = 0 ; i < Z_EEPROM_ADDR_Data_Max; i++)
//	{
//		EEPROM_Data_Table[i].read_flag = CLEAR;
//	}
	EEPROM_Read_NoSet(Z_EEPROM_Read_Num_10);
}

/// @brief    EEPROM 읽기 Addr 송신
/// @param    void
/// @return   void
void EEPROM_Read_20(void)
{
	Ini_EEPROM_Read_21();
	R_IICA0_Master_Send(_A0_IICA0_MASTERADDRESS, gu8_TxBuf, 1, 0);	//		읽기 주소 송신
}
void Ini_EEPROM_Read_20(void)
{
	EEPROM_Read_NoSet(Z_EEPROM_Read_Num_20);
}

/// @brief    EEPROM 읽기 Read CMD 송신
/// @param    void
/// @return   void
void EEPROM_Read_21(void)
{
	if (0U == IICBSY0)								//	busy 체크
	{
		Ini_EEPROM_Read_22();
		R_IICA0_Master_Receive(_A0_IICA0_MASTERADDRESS, &gu8_RxBuf, 1, 0);	//	Read CMD 송신
	}
}
void Ini_EEPROM_Read_21(void)
{
	EEPROM_Read_NoSet(Z_EEPROM_Read_Num_21);
}

/// @brief    EEPROM 읽기 완료 대기
/// @param    void
/// @return   void
void EEPROM_Read_22(void)
{
	if (0U == IICBSY0)								//	busy check
	{
		if ((gu8_RxBuf >= EEPROM_Data_Table[gu8_EEPROM_OperNum].data_valid_check_lower) && (gu8_RxBuf <= EEPROM_Data_Table[gu8_EEPROM_OperNum].data_valid_check_upper))	//	읽은 데이터 유효성 체크
		{
			*EEPROM_Data_Table[gu8_EEPROM_OperNum].data = gu8_RxBuf;	//	데이터 저장
			EEPROM_Data_Table[gu8_EEPROM_OperNum].eeprom_data = *EEPROM_Data_Table[gu8_EEPROM_OperNum].data;	//	데이터 저장

//			if (EEPROM_Data_Table[gu8_EEPROM_OperNum].address == EEPROM_ADDR_FILTERMODULE1_AMOUNT_USED_HIGH)
//			{
//				gu16_Effluent_1L_FilterModule1 |= (((U16)*EEPROM_Data_Table[gu8_EEPROM_OperNum].data << 8) & 0xFF00U);
//			}
//			else if (EEPROM_Data_Table[gu8_EEPROM_OperNum].address == EEPROM_ADDR_FILTERMODULE1_AMOUNT_USED_LOW)
//			{
//				gu16_Effluent_1L_FilterModule1 |= (U16)*EEPROM_Data_Table[gu8_EEPROM_OperNum].data & 0x00FFU;
//			}
//			else if (EEPROM_Data_Table[gu8_EEPROM_OperNum].address == EEPROM_ADDR_FILTERMODULE2_AMOUNT_USED_HIGH)
//			{
//				gu16_Effluent_1L_FilterModule2 |= (((U16)*EEPROM_Data_Table[gu8_EEPROM_OperNum].data << 8) & 0xFF00U);
//			}
//			else if (EEPROM_Data_Table[gu8_EEPROM_OperNum].address == EEPROM_ADDR_FILTERMODULE2_AMOUNT_USED_LOW)
//			{
//				gu16_Effluent_1L_FilterModule2 |= (U16)*EEPROM_Data_Table[gu8_EEPROM_OperNum].data & 0x00FFU;
//			}
//			else
//			{
//
//			}
		}
		else										//		유효성 NG인 경우
		{
			if ((gu8_EEPROM_OperNum != Z_EEPROM_ADDR_FIRST_A) && (gu8_EEPROM_OperNum != Z_EEPROM_ADDR_FIRST_B))		//	구분자 데이터가 아닌 경우
			{
				*EEPROM_Data_Table[gu8_EEPROM_OperNum].data = EEPROM_Data_Table[gu8_EEPROM_OperNum].init_data;	//	초기값 저장
				EEPROM_Data_Table[gu8_EEPROM_OperNum].eeprom_data = *EEPROM_Data_Table[gu8_EEPROM_OperNum].data;	//	초기값 저장
			}
		}
		EEPROM_Data_Table[gu8_EEPROM_OperNum].read_flag = CLEAR;	//		Read flag clear
		Ini_EEPROM_Read_10();
	}
	else if (gu8_TimerEEPROM_Limit == 0)			//	리미트 타이머
	{
		Ini_EEPROM_Read_10();
		F_EEPROM_CommunicationError = SET;
	}
	else
	{

	}
}
void Ini_EEPROM_Read_22(void)
{
	gu8_TimerEEPROM_Limit = Z_TimerEEPROM_Limit;
	EEPROM_Read_NoSet(Z_EEPROM_Read_Num_22);
}
/// @brief    EEPROM Read 스텝
/// @param    void
/// @return   void
static void EEPROM_Read_NoSet(U8 data)
{
	static const U8 DT_EEPROM_ReadNumber[] =
	{
		Z_EEPROM_Read_00,
		Z_EEPROM_Read_10,
		Z_EEPROM_Read_20,
		Z_EEPROM_Read_21,
		Z_EEPROM_Read_22,
	};
	/************************************************/
	gu8_EEPROM_ReadNum = data; 								/*	일련 번호 set*/

	gu8_EEPROM_Read = DT_EEPROM_ReadNumber[gu8_EEPROM_ReadNum];	/*	상태 mode No. set*/
}
