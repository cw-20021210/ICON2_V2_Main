
#ifndef _WIFI_SetFunctionData_H_
#define _WIFI_SetFunctionData_H_

typedef enum
{
    WIFI_FUNC_0001_POWER_SEL = 0x0001,
    WIFI_FUNC_0002_COLD_SEL,    // 냉수설정
    WIFI_FUNC_0003_HOT_LOCK_SEL,// 온수잠금설정
    WIFI_FUNC_0004_SAVE_SEL,    // 절전설정
    WIFI_FUNC_0005_LOCK_SEL,    // 잠금설정
    WIFI_FUNC_0006_MUTE_SEL,    // 무음설정
    WIFI_FUNC_0007_STERILIZE_SEL,   // 살균설정
    WIFI_FUNC_0008_ICE_SEL,     // 제빙설정
    WIFI_FUNC_0009_HOT_TEMP,    // 온수온도표시
    WIFI_FUNC_000A_COLD_TEMP,   // 냉수온도표시
    WIFI_FUNC_000B_WATER_SEL,   // 물 선택
    WIFI_FUNC_000C_USER_SEL,    // 사용자
    WIFI_FUNC_000D_WATER_OUTQUANTITY,// 추출물량
    WIFI_FUNC_000E_TIME_YEAR,       // 현재시간 연
    WIFI_FUNC_000F_TIME_MONTHDAY,   // 현재시간 월일
    WIFI_FUNC_0010_TIME_MINSEC,     // 현재시간 시분
    WIFI_FUNC_0011_STERILIZE_CYCLE, // 살균주기
    WIFI_FUNC_0012_STERILIZE_MODE,  // 살균주기모드
    WIFI_FUNC_0013_STERILIZE_TIME,  // 살균시간설정
    WIFI_FUNC_0014_STERILIZE_STATUS, // 살균동작상태
    WIFI_FUNC_0015_TDS_IN,  // 입수 TDS
    WIFI_FUNC_0016_TDS_OUT, // 출수 TDS
    WIFI_FUNC_0017_WATER_PRESSURE,  // 폐 수압
    WIFI_FUNC_0018_TDS_INTEMP,  // TDS 입수온도
    WIFI_FUNC_0019_TDS_OUTTEMP, // TDS 출수온도
    WIFI_FUNC_001A_HOT_INTEMP,  // 순간온수 원수온도
    WIFI_FUNC_001B_HOT_MEANTEMP,// 순간온수 평균온도
    WIFI_FUNC_001C_HOT_SELTEMP, // 순간온수 설정온도
    WIFI_FUNC_001D_WATER_INQUANTITY,// 입수유량
    WIFI_FUNC_001E_AUTODRAIN_SEL,   // 자동배수 설정
    WIFI_FUNC_001F_DRAIN_STATUS,     // 배수동작
    WIFI_FUNC_0020_WATER_REQUANTITY,// 재 정수 유량
    WIFI_FUNC_0021_HEART_TIME1, // 하트서비스시간
    WIFI_FUNC_0022_HEART_TIME2, //
    WIFI_FUNC_0023_HEART_TIME3, //
    WIFI_FUNC_0024_HEART_TIME4, //
    WIFI_FUNC_0025_FILTER_NOTICE1,  // 필터교환알림
    WIFI_FUNC_0026_FILTER_NOTICE2,
    WIFI_FUNC_0027_FILTER_NOTICE3,
    WIFI_FUNC_0028_FILTER_NOTICE4,
    WIFI_FUNC_0029_SERVICE_TIME,    // 서비스 경과일
    WIFI_FUNC_002A_REMOVE_STATE,    // 제거율 상태
    WIFI_FUNC_002B_SAVE_DETECT,     // 절전진입X
    WIFI_FUNC_002C_SILVER_CARE,     // 실버케어
    WIFI_FUNC_002D_DRAIN_QUANTITY,  // 배수물량
    WIFI_FUNC_002E_STERILIZE_QUANTITY,  // 살균물량
    WIFI_FUNC_002F_SPARKLING_SEL,   // 탄산수 선택
    WIFI_FUNC_0030_ICE_MAKETIME,    // 얼음추출시간
    WIFI_FUNC_0031_VOICE_SEL,   // 음성안내설정
    WIFI_FUNC_0032_ICELOCK_SEL, // 얼음잠금 설정
    WIFI_FUNC_0033_VOICE_VOL_SEL,    // 음성볼륨 설정
    WIFI_FUNC_0034_LIGHT_SEL,   // 조명설정
    WIFI_FUNC_0035_COVER1_OPEN, // 커버1 열림
    WIFI_FUNC_0036_COVER2_OPEN, // 커버2 열림
    WIFI_FUNC_0037_SLEEP_SEL,  // 슬립모드 선택
    WIFI_FUNC_0038_MASH_CLEAN,  // 매쉬망 청소알람
    WIFI_FUNC_0039_HOT_SEL,     // 온수설정
    WIFI_FUNC_003A_FREEZE_BURST,
    WIFI_FUNC_003B_HOT_TEMP_SEL,    // 나의 온수온도 설정
    WIFI_FUNC_003C_COLD_STRENGTH_SEL,   // 냉수 강약조절
    WIFI_FUNC_003D_HEIGHT_SEL,          // 고도모드
    WIFI_FUNC_003E_WATER_COURSE_STATUS, // 유로모듈 장착상태
    WIFI_FUNC_003F_WATER_COURSE_RESET,  // 유로모듈 리셋
    WIFI_FUNC_0040_SMART_CHECK_RQST,    // 스마트 고장진단 설정
    WIFI_FUNC_0041_SMART_CHECK_STATUS,  // 스마트 고장진단 상태
    WIFI_FUNC_0042_SMART_CHECK_PROGRESS,// 스마트 고장진단 진행률
    WIFI_FUNC_0043_FILTER_STATUS1,      // 필터 장착상태
    WIFI_FUNC_0044_FILTER_STATUS2,
    WIFI_FUNC_0045_FILTER_STATUS3,
    WIFI_FUNC_0046_FILTER_STATUS4,
    WIFI_FUNC_0047_WATER_QUANTITY_SEL,  // 물량 선택
    WIFI_FUNC_0048_HOT_TEMP_SEL,
    WIFI_FUNC_0049_OUT_BUTTON_LIGHT,
    WIFI_FUNC_004A_WELCOME_LIGHT,
    WIFI_FUNC_004B_HIDDEN_LCD_SEL,
    WIFI_FUNC_004C_FAUCET_MOVE_SEL,
    WIFI_FUNC_004D_FAUCET_MOVE_STATUS,
    WIFI_FUNC_004E_CUP_SENSING_SEL,
    WIFI_FUNC_004F_SOUND_VOL_SEL,
    WIFI_FUNC_0050_UV_STERILIZE_STATUS,
    WIFI_FUNC_0051_QUANTITY_USE_SEL,
    WIFI_FUNC_0052_VOICE_MODE_SEL,
    WIFI_FUNC_0053_FILTER_RESET1,
    WIFI_FUNC_0054_FILTER_RESET2,
    WIFI_FUNC_0055_FILTER_RESET3,
    WIFI_FUNC_0056_FILTER_RESET4,
    WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,
    WIFI_FUNC_0058_ICE_SIZE_SEL,
    WIFI_FUNC_0059_ICE_ONOFF_SEL,
    WIFI_FUNC_005A_UV_ICE_TANK_STATUS,
    WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,
    WIFI_FUNC_005C_SMART_CHECK_RESULT,
    //-------------------------------------------20260201--------------------------
    WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,		//	얼음 파우셋 살균 동작 상태
    WIFI_FUNC_005E_SLEEP_MODE_SEL,				//	취침모드 설정
    WIFI_FUNC_005F_SLEEP_START_HOUR,			//	취침모드 시작 시간
    WIFI_FUNC_0060_SLEEP_START_MIN,				//	취침모드 시작 분
    WIFI_FUNC_0061_SLEEP_END_HOUR,				//	취침모드 종료 시간
    WIFI_FUNC_0062_SLEEP_END_MIN,				//	취침모드 종료 분
    WIFI_FUNC_0063_DEFAULT_QUANTITY_ML_SEL,		//	기본 용량 설정
    WIFI_FUNC_0064_STERILIZE_EXPECTED_TIME,		//	EW 살균 소요 예정 시간
    WIFI_FUNC_0065_DEFAULT_TEMP_SEL,			//	기본 온수 온도 설정 상태 (○)
    WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,		//	기본 추출 물량 설정 상태 (○)
    WIFI_FUNC_0067_WATER_QUANTITY_USE_SEL0,		//	4oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_0068_WATER_QUANTITY_USE_SEL1,		//	6oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_0069_WATER_QUANTITY_USE_SEL2, 	//	8oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_006A_WATER_QUANTITY_USE_SEL3, 	//	10oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_006B_WATER_QUANTITY_USE_SEL4, 	//	12oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_006C_WATER_QUANTITY_USE_SEL5, 	//	14oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_006D_WATER_QUANTITY_USE_SEL6, 	//	16oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_006E_WATER_QUANTITY_USE_SEL7, 	//	20oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_006F_WATER_QUANTITY_USE_SEL8, 	//	32oz 물량 사용 설정 상태 (○)
    WIFI_FUNC_0070_WATER_QUANTITY_USE_SEL9,		//	연속 물량 사용 설정 상태 (○)
    WIFI_FUNC_0071_WATER_QUANTITY_USE_SEL10,	//	reserve2 물량 사용 설정 상태
    WIFI_FUNC_0072_WATER_QUANTITY_SORT_SEL,		//	물량 정렬 설정 상태 (○)
    WIFI_FUNC_0073_WATER_TEMP_SORT_SEL,			//	온수 온도 정렬 설정 상태 (○)
    WIFI_FUNC_0074_UNUSE_POWER_SAVING_SEL,		//	미사용 절전 설정
    WIFI_FUNC_0075_TEMP_UNIT_SEL,				//	온도 단위 표시 설정 상태 (○)
    WIFI_FUNC_0076_WATER_QUANTITY_UNIT_SEL,		//	추출 물량 단위 표시 설정 상태 (○)
    WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME,        // 	온수 목표 온도 도달 시간
    WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,       // 	냉수 목표 온도 도달 시간
    WIFI_FUNC_0079_MIXED_WATER_AVE_TEMP,		//	혼합 추출 평균 온도
    WIFI_FUNC_007A_MIXED_WATER_MIN_TEMP,		//	혼합 추출 최소 온도
    WIFI_FUNC_007B_MIXED_WATER_MAX_TEMP,		//	혼합 추출 최대 온도
    WIFI_FUNC_007C_COLDTANK_LOW_WATERLEVEL,		//	냉수 탱크 저수위 상태
    WIFI_FUNC_007D_COLDTANK_HIGH_WATERLEVEL,	//	냉수 탱크 만수위 상태
    WIFI_FUNC_007E_HIGH_TEMP_STL_SEL,			//	고온수 살균 설정
    // WIFI_FUNC_007F_HIGH_TEMP_STL_STATUS,		//	고온수 살균 상태
    WIFI_FUNC_007F_DEFAULT_TEMP_SET,            //  기본온수온도설정(온도값)

    WIFI_FUNC_0080_HIGH_TEMP_STL_TIME,			//	고온수 살균 설정 시간
    WIFI_FUNC_0081_HIGH_TEMP_STL_TIME_M,		//	고온수 살균 설정 시간 (분)
    WIFI_FUNC_0082_UNUSE_POW_SAVING_STATUS,		//	미사용 절전 동작 상태
    WIFI_FUNC_0083_POWER_SAVING_STATUS,			//	절전 동작 상태
    WIFI_FUNC_0084_HOT_RESTARTING_TEMP,			//	온수 재기동 온도
    WIFI_FUNC_0085_HOT_STOP_TEMP,				//	온수 정지 온도
    WIFI_FUNC_0086_COLD_RESTARTING_TEMP,		//	냉수 재기동 온도
    WIFI_FUNC_0087_COLD_STOP_TEMP,				//	냉수 정지 온도
    WIFI_FUNC_0088_COLD_ADDITIONAL_TIME,    	//	냉수 냉각 추가 시간
    WIFI_FUNC_0089_BOOST_PUMP_STATUS,			//	부스트 펌프 동작 상태
    WIFI_FUNC_008A_COLDTANK_UV_STL_STATUS,		//	냉수 탱크 살균 동작 상태 (UV)
    WIFI_FUNC_008B_HEATER_STATUS,				//	히터 동작 상태 (저탕식)
    WIFI_FUNC_008C_COMP_STATUS,					//	COMP 동작 상태
    WIFI_FUNC_008D_FILTER_WATER_USAGE,			//	필터 물 사용량
    WIFI_FUNC_008E_FILTER_REMAINING_DAY,		//	필터 잔여 사용일 (시간)
    WIFI_FUNC_008F_ICE_FULL_STATUS,				//	만빙 감지 상태

    WIFI_FUNC_0090_ICE_TRAY_POSITION_STATUS,	//	얼음 트레이 위치 상태
    WIFI_FUNC_0091_REFRIGERANT_SWC_STATUS,		//	냉매 전환 밸브 상태
    WIFI_FUNC_0092_DEICING_STAUTS,				//	탈빙 밸브 상태
    WIFI_FUNC_0093_ICEMAKING_STAUTS,			//	제빙 동작 상태
    WIFI_FUNC_0094_DUMMY_DEICING_STAUTS,		//	더미 탈빙 동작 상태
    WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,  	// 	제빙 완료 시간
    WIFI_FUNC_0096_DEICING_COMPLET_TIME,    	// 	탈빙 완료 시간
    WIFI_FUNC_0097_TARY_WATER_COMPLET_TIME, 	// 	트레이 입수 완료 시간
    WIFI_FUNC_0098_TARY_WATER_QUANTITY,     	//	트레이 입수 유량
    WIFI_FUNC_0099_COMP_TARGET_RPS,				//	압축기 목표 RPS
    WIFI_FUNC_009A_COMP_CURRENT_RPS,			//	압축기 현재 RPS
    WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,		//	드레인 탱크 저수위 센서 상태
    WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,		//	드레인 탱크 만수위 센서 상태
    WIFI_FUNC_009D_DRAINTANK_DRAIN_STAUTS,		//	드레인 배수 동작 상태
    WIFI_FUNC_009E_COLD_EVA_TEMP,				//	쿨링 코일 증발기 온도 센서
    WIFI_FUNC_009F_ICE_EVA_TEMP,				//	얼음 증발기 온도 센서

    WIFI_FUNC_00A0_COLD_TANK_TEMP,				//	냉수 탱크 온도
    WIFI_FUNC_00A1_HOT_TANK_TEMP,				//	온수 탱크 온도
    WIFI_FUNC_00A2_CHILD_LOCK,					//	전체 버튼 자동 잠금 (○)
    WIFI_FUNC_00A3_FLOWPATH_EMPTY,				//	유로 비움 설정
    WIFI_FUNC_00A4_HOTTANK_LOW_W_LEVEL,			//	온수 탱크 저수위 상태
    WIFI_FUNC_00A5_HOTTANK_HIGH_W_LEVEL,		//	온수 탱크 만수위 상태
    WIFI_FUNC_00A6_PURETANK_LOW_W_LEVEL,		//	정수 탱크 저수위 상태
    WIFI_FUNC_00A7_PURETANK_HIGH_W_LEVEL,		//	정수 탱크 만수위 상태
    WIFI_FUNC_00A8_COLDTANK_MID_W_LEVEL,		//	냉수 탱크 중수위 상태
    WIFI_FUNC_00A9_UNUSED_TIME,					//	제품 미사용 시간
    WIFI_FUNC_00AA_SMART_SAVING_SET_TIME,		//	스마트 절전 미사용 설정시간
    WIFI_FUNC_00AB_SAMRT_SAVING_STATE,			//	스마트 절전 상태
    WIFI_FUNC_00AC_FILTER1_WATER_USAGE,			//	필터1 누적 물 사용량 (○)
    WIFI_FUNC_00AD_FILTER1_UASGE_DAY,			//	필터1 잔여 사용 시간 (○)
    WIFI_FUNC_00AE_FILTER2_WATER_USAGE,			//	필터2 누적 물 사용량
    WIFI_FUNC_00AF_FILTER2_UASGE_DAY,			//	필터2 잔여 사용 시간
    WIFI_FUNC_00B0_FILTER3_WATER_USAGE,			//	필터3 누적 물 사용량
    WIFI_FUNC_00B1_FILTER3_UASGE_DAY,			//	필터3 잔여 사용 시간
    WIFI_FUNC_00B2_FILTER_NOTICE1,				//	필터 교체 알림 (○)
    WIFI_FUNC_00B3_FILTER_NOTICE2,				//	필터 교체 알림 (○)
    WIFI_FUNC_00B4_FILTER_NOTICE3,				//	필터 교체 알림 (○)
    WIFI_FUNC_00B5_FILTER_FLUSHING,             //  필터 플러싱 상태
    WIFI_FUNC_00B6_VOICE_LANGUAGE,              //  음성언어설정
    WIFI_FUNC_00B7_FILTER_SEQ,                  //  필터교체순서
    WIFI_FUNC_00B8_FILTER1_DDAY,                //  필터1 교체 도래일
    WIFI_FUNC_00B9_FILTER2_DDAY,                //  필터2 교체 도래일
    WIFI_FUNC_00BA_FILTER3_DDAY,                //  필터3 교체 도래일
    WIFI_FUNC_00BB_FLOWPATH_STL_STATUS,         //  유로 고온수 살균상태
    WIFI_FUNC_00BC_FAUCE_STL_STATUS,            //  파우셋 고온수 살균상태
    WIFI_FUNC_00BD_ICETANK_STL_STATUS,          //  얼음탱크 고온수 살균상태
    WIFI_FUNC_00BE_QUANTITY_HALFCUP,            //  반컵 용량
    WIFI_FUNC_00BF_QUANTITY_ONECUP,             //  한컵 용량

    WIFI_FUNC_00C0_QUANTITY_TWOCUPS,            //  두컵 용량
    WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,            //  온수온도 사용 설정 0 (45)
    WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,            //  온수온도 사용 설정 1 (50)
    WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,            //  온수온도 사용 설정 2 (55)
    WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,            //  온수온도 사용 설정 3 (60)
    WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,            //  온수온도 사용 설정 4 (65)
    WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,            //  온수온도 사용 설정 5 (70)
    WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,            //  온수온도 사용 설정 6 (75)
    WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,            //  온수온도 사용 설정 7 (80)
    WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,            //  온수온도 사용 설정 8 (85)
    WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,            //  온수온도 사용 설정 9 (100)
    WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,        //  물 추출상태
    WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,          //  얼음 추출상태
    WIFI_FUNC_00CD_SETUP_MODE_STATUS,           //  제품 설정 상태

    WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,           //  온수온도 사용 설정 10
    WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,           //  온수온도 사용 설정 11
    
    WIFI_FUNC_00D0_UV_ICE_TANK_STATUS2,			//	얼음 탱크 살균 동작 상태2
    WIFI_FUNC_00D1_ALTITUDE_AUTO,               //  고도모드 자동 설정
    WIFI_FUNC_00D2_ALARM_1_SET,                  //  알람시계 1 설정
    WIFI_FUNC_00D3_ALARM_2_SET,                  //  알람시계 2 설정
    WIFI_FUNC_00D4_ALARM_3_SET,                  //  알람시계 3 설정
    WIFI_FUNC_00D5_ALARM_1_START_HOUR,           //  알람시계 1 시작시간 시 설정
    WIFI_FUNC_00D6_ALARM_2_START_HOUR,           //  알람시계 2 시작시간 시 설정
    WIFI_FUNC_00D7_ALARM_3_START_HOUR,           //  알람시계 3 시작시간 시 설정
    WIFI_FUNC_00D8_ALARM_1_START_MIN,            //  알람시계 1 시작시간 분 설정
    WIFI_FUNC_00D9_ALARM_2_START_MIN,            //  알람시계 2 시작시간 분 설정
    WIFI_FUNC_00DA_ALARM_3_START_MIN,            //  알람시계 3 시작시간 분 설정
    WIFI_FUNC_00DB_ALARM_1_SOUND,                //  알람시계 1 알람음 설정
    WIFI_FUNC_00DC_ALARM_2_SOUND,                //  알람시계 2 알람음 설정
    WIFI_FUNC_00DD_ALARM_3_SOUND,                //  알람시계 3 알람음 설정
    WIFI_FUNC_00DE_HOME_SCREEN_MODE,             //  기본홈 모드 설정 (메인 / 레시피 / 마이워터)
    WIFI_FUNC_00DF_STANBY_SCREEN,                //  대기화면 (날시, 시간, 사용X)

    WIFI_FUNC_00E0_SCREEN_BRIGHTNESS,            //  밝기 설정
    WIFI_FUNC_00E1_MYWATER_TEMP_1,               //  마이워터1 온도
    WIFI_FUNC_00E2_MYWATER_QUANTITY_1,           //  마이워터1 용량
    WIFI_FUNC_00E3_MYWATER_RECIPE_1,             //  마이워터1 레시피
    WIFI_FUNC_00E4_MYWATER_TEMP_2,               //  마이워터2 온도
    WIFI_FUNC_00E5_MYWATER_QUANTITY_2,           //  마이워터2 용량
    WIFI_FUNC_00E6_MYWATER_RECIPE_2,             //  마이워터2 레시피
    WIFI_FUNC_00E7_MYWATER_TEMP_3,               //  마이워터3 온도
    WIFI_FUNC_00E8_MYWATER_QUANTITY_3,           //  마이워터3 용량
    WIFI_FUNC_00E9_MYWATER_RECIPE_3,             //  마이워터3 레시피
    WIFI_FUNC_00EA_MYWATER_TEMP_4,               //  마이워터4 온도
    WIFI_FUNC_00EB_MYWATER_QUANTITY_4,           //  마이워터4 용량
    WIFI_FUNC_00EC_MYWATER_RECIPE_4,             //  마이워터4 레시피
    WIFI_FUNC_00ED_MYWATER_TEMP_5,               //  마이워터5 온도
    WIFI_FUNC_00EE_MYWATER_QUANTITY_5,           //  마이워터5 용량
    WIFI_FUNC_00EF_MYWATER_RECIPE_5,             //  마이워터5 레시피

    WIFI_FUNC_00F0_MYWATER_TEMP_6,               //  마이워터6 온도
    WIFI_FUNC_00F1_MYWATER_QUANTITY_6,           //  마이워터6 용량
    WIFI_FUNC_00F2_MYWATER_RECIPE_6,             //  마이워터6 레시피
    WIFI_FUNC_00F3_MYWATER_TEMP_7,               //  마이워터7 온도
    WIFI_FUNC_00F4_MYWATER_QUANTITY_7,           //  마이워터7 용량
    WIFI_FUNC_00F5_MYWATER_RECIPE_7,             //  마이워터7 레시피
    WIFI_FUNC_00F6_MYWATER_TEMP_8,               //  마이워터8 온도
    WIFI_FUNC_00F7_MYWATER_QUANTITY_8,           //  마이워터8 용량
    WIFI_FUNC_00F8_MYWATER_RECIPE_8,             //  마이워터8 레시피
    WIFI_FUNC_00F9_MYWATER_TEMP_9,               //  마이워터9 온도
    WIFI_FUNC_00FA_MYWATER_QUANTITY_9,           //  마이워터9 용량
    WIFI_FUNC_00FB_MYWATER_RECIPE_9,             //  마이워터9 레시피
    WIFI_FUNC_00FC_MYWATER_TEMP_10,               //  마이워터10 온도
    WIFI_FUNC_00FD_MYWATER_QUANTITY_10,           //  마이워터10 용량
    WIFI_FUNC_00FE_MYWATER_RECIPE_10,             //  마이워터10 레시피
    
    WIFI_FUNC_NUM
}E_API_A1010_T;

typedef enum
{
    WIFI_FUNC_00FF_CLEANING_STATUS = WIFI_FUNC_NUM,              //  구연산 세정상태 
    
    WIFI_FUNC_0100_CLEANING_SW_STATUS,              //  구연산 리드 스위치 장착 상태
    WIFI_FUNC_0101_STERILIZE_REMAINING_DAY,         //  살균시작 잔여일
    WIFI_FUNC_0102_SCREEN_SAVER,                    //  스크린세이버 설정 ON/OFF
    WIFI_FUNC_0103_SCREEN_SAVER_START,              //  스크린세이버 시작 시간 (시)
    WIFI_FUNC_0104_SCREEN_SAVER_END,                //  스크린세이버 종료 시간 (시)
    WIFI_FUNC_0105_MAIN_THEME,                      //  메인화면 테마 0 클래식 / 1 네이처
    WIFI_FUNC_0106_ADJUSTMENT_THEME,                //  미세조절 테마 0 원형 / 1 버튼형
    WIFI_FUNC_0107_MYWATER_ENABLE_1,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_0108_MYWATER_ENABLE_2,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_0109_MYWATER_ENABLE_3,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_010A_MYWATER_ENABLE_4,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_010B_MYWATER_ENABLE_5,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_010C_MYWATER_ENABLE_6,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_010D_MYWATER_ENABLE_7,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_010E_MYWATER_ENABLE_8,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_010F_MYWATER_ENABLE_9,                //  MYWATER 사용여부 0(미사용) / 1(사용)
    WIFI_FUNC_0110_MYWATER_ENABLE_10,               //  MYWATER 사용여부 0(미사용) / 1(사용)

    WIFI_FUNC_0111_WATER_COURSE_UASGE_DAY,          //  유료모듈 잔여사용일 (12개월)

    WIFI_FUNC_011E_HOT_TEMP_MIN_COUNT = 0x011E,            //  온수온도 최소 선택 갯수(1개)
    WIFI_FUNC_0120_HOT_TEMP_MAX_COUNT = 0x0120,            //  온수온도 최대 선택 갯수(5개)

    WIFI_FUNC_0126_FAUCET_EXPOSURE_LEVEL = 0x0126,

    WIFI_FUNC_0129_AMOUNT_SLOT1_ENABLE = 0x0129,
    WIFI_FUNC_012A_AMOUNT_SLOT2_ENABLE = 0x012A,
    WIFI_FUNC_012B_AMOUNT_SLOT3_ENABLE = 0x012B,
    WIFI_FUNC_012C_AMOUNT_SLOT4_ENABLE = 0x012C,

    WIFI_FUNC_0132_START_WATER_QUANTITY_SLOT = 0x0132,
    WIFI_FUNC_0133_START_WATER_QUANTITY_SEL_1,
    WIFI_FUNC_0134_START_WATER_QUANTITY_SEL_2,
    WIFI_FUNC_0135_START_WATER_QUANTITY_SEL_3,
    WIFI_FUNC_0136_START_WATER_QUANTITY_SEL_4,
    WIFI_FUNC_0137_START_WATER_QUANTITY_SEL_5,
    WIFI_FUNC_0138_START_WATER_QUANTITY_SEL_6,
    WIFI_FUNC_0139_START_WATER_QUANTITY_SEL_7,
    WIFI_FUNC_013A_START_WATER_QUANTITY_SEL_8,
    WIFI_FUNC_013B_START_WATER_QUANTITY_SEL_9,
    WIFI_FUNC_013C_START_WATER_QUANTITY_SEL_10,

    WIFI_FUNC_0140_FAUCET_UP_REED_SW = 0x0140,                  //  파우셋 상승 리드스위치       
    WIFI_FUNC_0141_FAUCET_DOWN_REED_SW = 0x0141,                //  파우셋 하강 리드스위치
    WIFI_FUNC_0142_FAUCET_CUP_DETECTION_MICRO_SW = 0x0142,      //  파우셋 컵감지 마이크로 스위치

    WIFI_FUNC_NUM_2

}E_API_A1010_T_2;



void* GetWifiFuncEventList ( void );
U16 GetWifiFuncListSize ( void );

U8 IsExceptId ( U16 mId );


#endif


