/// @file   buzzer.h
/// @date   2019.07.10
/// @author PSM
/// @brief  Buzzer 동작 제어 관련 헤더 file

#ifdef BUZZER_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_BUZZER()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_BUZZER()		{	/*	10ms 타이머 감산		*/	\
									TDEC(gu16_TimerBuzzer);		\
								}
#define	TDEC_100ms_BUZZER()		{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_BUZZER()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_BUZZER()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_BUZZER()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_BUZZER()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_BUZZER()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_MAINCLOCK_HZ		(32000000)				//	메인클럭 설정
#define	Z_TPS_FOR_BUZZER	(1)						//	레지스터 TPS 데이터

//// 4옥타브
#define Do4   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/261)-1)/2)
#define Re4   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/293)-1)/2)
#define Mi4   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/329)-1)/2)
#define Pa4   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/349)-1)/2)
#define Pas4  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/369)-1)/2)
#define Sol4  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/391)-1)/2)
#define La4   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/440)-1)/2)
#define Si4   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/493)-1)/2)
// 5옥타브
#define Do5   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/523)-1)/2)    // 도 : 523.2511Hz = 1.912ms
#define Dos5  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/554)-1)/2)    // 도#: 554.3653Hz = 1.804ms
#define Re5   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/587)-1)/2)    // 레 : 587.3295Hz = 1.704ms
#define Mi5   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/659)-1)/2)    // 미 : 659.2551Hz = 1.517ms
#define Pa5   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/698)-1)/2)    // 파 : 698.4565Hz = 1.433ms
#define Pas5  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/739)-1)/2)    // 파#: 739.9888Hz = 1.351ms
#define Sol5  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/783)-1)/2)    // 솔 : 783.9909Hz = 1.276ms
#define Sols5 ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/830)-1)/2)    // 솔#: 830.6094Hz = 1.204ms
#define La5   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/880)-1)/2)    // 라 : 880.0000Hz = 1.136ms
#define Si5   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/987)-1)/2)    // 시 : 987.7666Hz = 1.012ms
// 6옥타브
#define Do6   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1046)-1)/2)    // 도 : 1046.502Hz = 0.955ms
#define Dos6  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1108)-1)/2)    // 도#: 1108.731Hz = 0.902ms
#define Re6   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1174)-1)/2)    // 레 : 1174.659Hz = 0.851ms
#define Mi6   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1318)-1)/2)    // 미 : 1318.510Hz = 0.758ms
#define Pa6   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1396)-1)/2)    // 파 : 1396.913Hz = 0.716ms
#define Pas6  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1479)-1)/2)    // 파#: 1479.978Hz = 0.676ms
#define Sol6  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1567)-1)/2)    // 솔 : 1567.982Hz = 0.638ms
#define Sols6 ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1661)-1)/2)    // 솔#: 1661.219Hz = 0.602ms
#define La6   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1760)-1)/2)    // 라 : 1760.000Hz = 0.568ms
#define Si6   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/1975)-1)/2)    // 시 : 1975.533Hz = 0.506ms
// 7옥타브
#define Do7   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/2093)-1)/2)    // 도 : 2093.005Hz = 0.478ms
#define Dos7  ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/2217)-1)/2)    // 도#: 2217.461Hz = 0.451ms
#define Re7   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/2349)-1)/2)    // 레 : 2349.318Hz = 0.426ms
#define Mi7   ((((Z_MAINCLOCK_HZ/Z_TPS_FOR_BUZZER)/2637)-1)/2)    // 미 : 2637.02Hz = 0.379ms
// 7옥타브

#define BuzzerOFF			(0xFFU)

#define BUZZER_TIMER_TT		(TT0)					//	부저 레지스터 정의
#define BUZZER_TIMER_TS		(TS0)                   //	부저 레지스터 정의
#define BUZZER_TIMER_TDR	(TDR04)                 //	부저 레지스터 정의
#define BUZZER_TIMER_TOE	(TOE0)                  //	부저 레지스터 정의
#define BUZZER_TIMER_TO		(TO0)                   //	부저 레지스터 정의

#define BUZZER_TIMER_SET 	(0x0010U)				//	타이머 채널에 따른 SET 데이터 정의
#define BUZZER_TIMER_CLR 	(0xFFEFU)				//	타이머 채널에 따른 CLEAR 데이터 정의

enum {
	BUZZER_OFF,
	BUZZER_POWER_ON,								//	부팅음
	BUZZER_EFFLUENT,								//	추출 시작음
	BUZZER_EFFLUENT_END,							//	추출 종료음
	BUZZER_COOL_SETUP,								//	냉각 ON음
	BUZZER_COOL_CANCEL,								//	냉각 OFF음
	BUZZER_SELECT,									//	선택음
	BUZZER_ERROR,									//	에러음
	BUZZER_SETUP,									//	설정음
	BUZZER_CANCEL,									//	해제음
	BUZZER_EFFLUENT_CONTINUE,						//	연속추출음
	BUZZER_AP_CONNECT,
	BUZZER_SERVER_CONNECT,
//	BUZZER_AP_PAIRING,								//	선택음과 같음
	BUZZER_Beep_1,
	BUZZER_Beep_2,
	BUZZER_Beep_3,
	BUZZER_Beep_4,
	BUZZER_Beep_5,
	BUZZER_ALRAM,
	BUZZER_NULL										//	미정의
};


/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U16 gu16_TimerBuzzer;						//	부저제어 타이머	<?ms/10ms>
//EXTERN U8 F_SilentBuzzer;              				// 	음소거모드 flag

/*==============================================================================
 function Definition
 ==============================================================================*/
void Buzzer_Initialize(void);
void BuzzerControl(void);
//void BuzzerSet(U8 bz);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
