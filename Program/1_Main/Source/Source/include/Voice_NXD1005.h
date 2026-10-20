#ifdef VOICE_H_
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif


/*==============================================================================
 Constant Definition
 ==============================================================================*/
//#define VOICE_TYPE_VOICOMM 0x01
#define VOICE_TYPE_VOICELAND 0x01
#define VOICE_TYPE_NEXTLAB 0x00

#define VOICE_VOLUME_LEVEL_MAXIUM 4
#define VOICE_VOLUME_LEVEL_MINIUM 0
#define DEFAULT_VOICE_VOLUME_LEVEL 1

#define SOUND_TYPE_0           0   // Type 0 - Sound Type 1, Beep x
#define SOUND_TYPE_1           1   // Type 1 - Sound Type 1, Beep x (무음)
#define SOUND_TYPE_2           2   // Type 2 - Sound Type 1, Beep O (효과음)
#define SOUND_TYPE_3           3   // Type 3 - Sound Type 2, Beep O (안내모드)

#define MK_COMMAND 0x8000

/*NXD Command*/
#define VOICE_COMMAND_ADDRESS_0 0xE000

// 음성안내
#define VOICE_COMMAND_VOLUME_5  0x0024  /* 36 */        // ▶ NUVOTON Volume 7
#define VOICE_COMMAND_VOLUME_4  0x0027  /* 39 */        // ▶ NUVOTON Volume 6
#define VOICE_COMMAND_VOLUME_3  0x002A  /* 42 */        // ▶ NUVOTON Volume 5
#define VOICE_COMMAND_VOLUME_2  0x0030  /* 48 */        // ▶ NUVOTON Volume 4
#define VOICE_COMMAND_VOLUME_1  0x0036  /* 54 */        // ▶ NUVOTON Volume 3

// 효과음
#define BEEP_COMMAND_VOLUME_5  0x0024  /* 36 */        // ▶ NUVOTON Volume 7
#define BEEP_COMMAND_VOLUME_4  0x0027  /* 39 */        // ▶ NUVOTON Volume 6
#define BEEP_COMMAND_VOLUME_3  0x002A  /* 42 */        // ▶ NUVOTON Volume 5
#define BEEP_COMMAND_VOLUME_2  0x0030  /* 48 */        // ▶ NUVOTON Volume 4
#define BEEP_COMMAND_VOLUME_1  0x0036  /* 54 */        // ▶ NUVOTON Volume 3

//// 음성안내
//#define VOICE_COMMAND_VOLUME_5  0x001F  /* 31 */       // ▶ NUVOTON Volume 9
//#define VOICE_COMMAND_VOLUME_4  0x0021  /* 33 */       // ▶ NUVOTON Volume 8
//#define VOICE_COMMAND_VOLUME_3  0x0024  /* 36 */       // ▶ NUVOTON Volume 7
//#define VOICE_COMMAND_VOLUME_2  0x0027  /* 39 */       // ▶ NUVOTON Volume 6
//#define VOICE_COMMAND_VOLUME_1  0x002A  /* 42 */       // ▶ NUVOTON Volume 5
//
//// 효과음
//#define BEEP_COMMAND_VOLUME_5  0x001F  /* 31 */        // ▶ NUVOTON Volume 9
//#define BEEP_COMMAND_VOLUME_4  0x0024  /* 36 */        // ▶ NUVOTON Volume 7
//#define BEEP_COMMAND_VOLUME_3  0x002A  /* 42 */        // ▶ NUVOTON Volume 5
//#define BEEP_COMMAND_VOLUME_2  0x0030  /* 48 */        // ▶ NUVOTON Volume 4
//#define BEEP_COMMAND_VOLUME_1  0x0036  /* 54 */        // ▶ NUVOTON Volume 3

//// 음성안내
//#define VOICE_COMMAND_VOLUME_5  0x0019  /* -9 dB */       // ▶ NUVOTON Volume 8
//#define VOICE_COMMAND_VOLUME_4  0x0023  /* -12 dB */      // ▶ NUVOTON Volume 7
//#define VOICE_COMMAND_VOLUME_3  0x002D  /* -15 dB */      // ▶ NUVOTON Volume 6
//#define VOICE_COMMAND_VOLUME_2  0x0037  /* -18 dB */      // ▶ NUVOTON Volume 5
//#define VOICE_COMMAND_VOLUME_1  0x0041  /* -18 dB */      // ▶ NUVOTON Volume 4
////#define VOICE_COMMAND_VOLUME_1  0x004B  /* -18 dB */      // ▶ NUVOTON Volume 3
////#define VOICE_COMMAND_VOLUME_1  0x0055  /* -18 dB */      // ▶ NUVOTON Volume 4
//
//// 효과음
//#define BEEP_COMMAND_VOLUME_5  0x0023  /* -12 dB */       // ▶ NUVOTON Volume 7
//#define BEEP_COMMAND_VOLUME_4  0x002D  /* -18 dB */       // ▶ NUVOTON Volume 6
//#define BEEP_COMMAND_VOLUME_3  0x0037  /* -24 dB */       // ▶ NUVOTON Volume 5
//#define BEEP_COMMAND_VOLUME_2  0x0041  /* -30 dB */       // ▶ NUVOTON Volume 4
//#define BEEP_COMMAND_VOLUME_1  0x004B  /* -30 dB */       // ▶ NUVOTON Volume 3
////#define BEEP_COMMAND_VOLUME_1  0x0055  /* -30 dB */       // ▶ NUVOTON Volume 2
////#define BEEP_COMMAND_VOLUME_1  0x005F  /* -30 dB */       // ▶ NUVOTON Volume 1

#define VOICE_COMMAND_VOLUME_INITIAL    0xE121     /* -9 dB */
#define VOICE_COMMAND_VOLUME_MINIUM     0xE100
#define VOICE_COMMAND_VOLUME_MAXIUM     0xE157

#define VOICE_COMMAND_CONTROL2 0xE260 /*operating mode: normal play*/
/*Repeat: 1 time play*/
#define VOICE_COMMAND_STOP 0xE210

#define VOICE_COMMAND_CONTROL3 0xE305 /*Disable, 비동기, 18bit, disable*/

#define VOICE_COMMAND_ADDRESS_256 0xE400
#define VOICE_COMMAND_ADDRESS_512 0xE401

/*NXD Address*/
#define VOICE_START 0x00


//==============================================================
//	음성 리스트
//==============================================================
// 음성
//#define VOICE_HOTLOCK_SETTING            	(0x00)      // 온수 버튼이 잠겼습니다.
//#define VOICE_HOTLOCK_CLEAR              	(0x01)      // 온수 버튼을 사용하실 수 있습니다.
//#define VOICE_ALLLOCK_SETTING            	(0x02)      // 모든 버튼이 잠겼습니다.
//#define VOICE_ALLLOCK_CLEAR              	(0x03)      // 모든 버튼을 사용하실 수 있습니다.
//#define VOICE_HOTLOCK_CHECK              	(0x04)      // 온수 버튼이 잠겨있습니다.  온수 버튼을 3초 누르시면 온수를 다시 사용하실 수 있습니다.
//#define VOICE_COLD_OFF                   	(0x05)      // 냉수가 꺼졌습니다.         냉수 버튼을 3초 누르시면 냉수를 다시 사용하실 수 있습니다.
//#define VOICE_ALLLOCK_CHECK              	(0x06)      // 모든 버튼이 잠겨 있습니다. 전체잠금 버튼을 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.
#define VOICE_HOTWATER_READY              	(0x07)      // 온수를 준비 중입니다. 첫 잔은 뜨겁지 않을 수 있습니다.
//#define VOICE_HOTWATER_EFFLUENT         	(0x08)      // 온수가 뜨거우니 주의하세요

//#define VOICE_FLUSHING_START     		    (0x0D)      // 코웨이 워터케어 시스템의 클리닝을 시작합니다.
//#define VOICE_FLUSHING_COMPLETE  		    (0x0E)      // 코웨이 위터게어 시스템의 클리닝이 완료되었습니다. 지금부터 정수기 사용이 가능합니다.
//#define VOICE_TOPCOVER_CHECK  			(0x0F)		// 제품의 상단 커버가 열려있습니다. 커버를 닫아주세요.
//#define VOICE_TOPCOVER_WARNING 			(0x10)		// 제품의 상단 커버가 열렸습니다. 제품에 물공급을 차단합니다.

// Wifi관련
//#define VOICE_WIFI_START                  (0x09)      // 와이파이 연결을 시작합니다.
//#define VOICE_WIFI_COMPLETE               (0x0A)      // 와이파이 연결이 완료되었습니다.
//#define VOICE_WIFI_FAIL         		    (0x0B)      // 와이파이 연결이 다시 시도해주세요.
//#define VOICE_WIFI_OFF           		    (0x0C)      // 와이파이 연결이 해제되었습니다.

// 음성선택
//#define VOICE_VOICE_SETTING_3             (0x15)		// 음성안내 모드입니다.
//#define VOICE_VOICE_SETTING_2             (0x16)		// 효과음 모드입니다.
//#define VOICE_VOICE_SETTING_1             (0x17)		// 무음 모드입니다.

// 에러
//#define VOICE_COLDWATER_ERROR             (0x11)    	// 냉수기능에 이상이 감지되어 냉수가 차갑지 않을 수 있습니다. 고객센터로 문의하세요.
//#define VOICE_HOTWATER_EFFSTOP_ERROR      (0x12)   	// 온수기능에 이상이 감지되어 안전을 위해 온수 추출을 중단합니다. 고객센터로 문의하세요.
//#define VOICE_SHORTAGE_ERROR              (0x13)      // 제품의 물 공급에 이상이 감지되었습니다. 원수공급밸브를 열어주시거나 고객센터로 문의하세요.
//#define VOICE_LEAKAGE_ERROR               (0x14)      // 누수가 감지되었습니다. 원수공급밸브를 닫아주시거나 고객센터로 문의하세요.

// 추가
//#define VOICE_SPECIAL_FUNCTION           	(0x18)		// 특수모드에 진입하였습니다.
#define VOICE_DETECT_ERROR                	(0x19)		// 제품에 이상이 감지되었습니다.
#define VOICE_CALL_SERVICE                	(0x1A)		// 고객센터로 문의하세요.

// 효과
//#define VOICE_CONTIUNE_EFFLUENT         	(0x1B)		// 연속추출
//#define VOICE_EFFLUENT_START            	(0x1C)      // 추출음
//#define VOICE_EFFLUENT_COMPLETE         	(0x1D)      // 추출종료음
//#define VOICE_POWER_ON                    (0x1E)      // 전원 인가시
//#define VOICE_BEEP_SELECT                 (0x1F)      // 버튼선택음(띵)
//#define VOICE_BEEP_WARNING                (0x20)      // 불가음(띵깡띵깡)
//#define VOICE_BEEP_ON                     (0x21)      // 설정( 띵동 ↗)
//#define VOICE_BEEP_OFF                    (0x22)      // 해제( 띵동 ↘)
#define VOICE_BEEP_BAD                    	(0x23)      // 오류( 띡!)
#define VOICE_BEEP_ALARM_1                	(0x24)      // 알람1( 당당당~ 당당당~)
#define VOICE_BEEP_ALARM_2                	(0x25)      // 알람2( 당당당~)
//#define VOICE_CONTIUNE_EFFLUENT_2       	(0x26)		// 연속추출2
//#define VOICE_CAREMODE_1                  (0x27)		// 케어모드1
//#define VOICE_CAREMODE_2                  (0x28)		// 케어모드2

//#define VOICE_HOT90DEGREE_HALFCUP_JP      (0x2F)		// 온수 90도는 반컵만 설정 가능합니다.
//==============================================================
// Noble 정수기 Sound Address
//#define VOICE_LOWER_COVER_WARNING_A   	(0x30)      // 하단 본체에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
//#define VOICE_TOP_COVER_WARNING_A     	(0x31)      // 제품 상단에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
//#define VOICE_FRONT_COVER_WARNING_A   	(0x32)      // 제품 전면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
//#define VOICE_SIDE_COVER_WARNING_A    	(0x33)      // 제품 측면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
#define VOICE_COLD_WATER_ERROR        		(0x34)      // 냉수 기능에 이상이 감지되었습니다. 증상이 반복되면 아이오케어앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
#define VOICE_CLEAN_DUST_FILTER       		(0x35)      // 냉수 기능에 이상이 감지되었습니다. 제품의 먼지 필터를 세척해주세요. 세척 후에도 냉수가 차갑지 않다면 콜센터에 연락해주세요.
#define VOICE_HOT_WATER_ERROR         		(0x36)      // 온수 기능에 이상이 감지되었습니다. 증상이 반복되면 아이오케어앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
#define VOICE_SHORTAGE_ERROR          		(0x37)      // 물 공급에 이상이 감지되었습니다. 원수공급밸브가 열려 있는지 확인해주세요. 증상이 반복되면 아이오케어앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
#define VOICE_LEAKAGE_ERROR           		(0x38)      // 누수가 감지되었습니다. 안전을 위해 원수공급밸브를 닫아주세요. 증상이 반복되면 아이오케어앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
#define VOICE_CUP_SENSOR_ERROR        		(0x39)      // 컵 감지 센서 이상이 감지되었습니다. 아이오케어 앱가이드를 확인해주시거나 증상이 반복되시면 콜센터에 연락해주세요.

//#define VOICE_CHANGE_FILTER           	(0x3A)      // 필터를 교체하셨나요. 교체 하셨다면 추출 버튼을 눌러주세요. 깨끗한 물을 위한 준비 과정이 시작됩니다.
//#define VOICE_FILTER_CONNECT_CHECK    	(0x3B)      // 필터가 올바르게 결합되지 않은 것으로 감지되었습니다. 제품 커버를 열어 다시 한번 확인해주세요.
//#define VOICE_FILTER_FLUSHING     		(0x3C)      // 새 필터 장착 후 깨끗한 물을 제공하기 위해 준비중에 있습니다. 잠시만 기다려주세요.
#define VOICE_FILTER_SYSTEM_READY   		(0x3D)      // 필터 시스템 준비가 완료되었습니다. 이제 코웨이에 깨끗한 물을 드실 수 있습니다.
#define VOICE_SMART_CHECK_ERROR       		(0x3E)      // 스마트 진단 결과 이상이 감지되었습니다. 아이오케어 앱을 통해 상세 내용을 확인해주세요.
#define VOICE_PRE_HEATING             		(0x3F)      // 온수 온도를 맞추고 있습니다. 잠시만 기다려주세요.

#define VOICE_WIFI_START_A           		(0x40)      // 와이파이 연결을 시작합니다. (1)
#define VOICE_WIFI_COMPLETE_IOCARE    		(0x41)      // 와이파이 연결이 완료되었습니다. 이제부터 아이오케어 앱을 통해 스마트한 관리를 받으실 수 있습니다.
#define VOICE_WIFI_FAIL         		  	(0x42)      // 와이파이 연결에 실패하였습니다. 인터넷 공유기를 확인해주시고, 문제가 반복되면 콜센터에 연락해주세요.
//#define VOICE_WIFI_START_B            	(0x43)      // 와이파이 연결을 시작합니다.(2)
#define VOICE_WIFI_COMPLETE           		(0x44)      // 와이파이 연결이 완료되었습니다.
//#define VOICE_WIFI_OFF           		  	(0x45)      // 와이파이 연결이 해제되었습니다. 다시 연결하시려면 제품 설정 모드에서 와이파이를 켜주세요.

#define VOICE_INSTALL           		  	(0x46)      // 안녕하세요. 고객님. 코웨이 정수기의 설치를 도와드리겠습니다. 추출 버튼을 누르시면 설치를 시작하겠습니다.
#define VOICE_FLUSHING_START     		  	(0x47)      // 새 정수기 제품에 깨끗한 물을 제공하기 위해 준비중입니다.
#define VOICE_FLUSHING_COMPLETE  		  	(0x48)      // 깨끗한 물을 제공하기 위한 준비가 완료되었습니다.

#define VOICE_BLE_START  		     		(0x49)      // 블루투스를 통해 아이오케어 앱에 연결을 시작합니다.
#define VOICE_BLE_COMPLETE		    		(0x4A)      // 블루투스를 통해 아이오케어 앱 연결이 완료되었습니다.
#define VOICE_BLE_FAIL		        		(0x4B)      // 블루투스를 통한 아이오케어 앱 연결이 실패하였습니다. 핸드폰 블루투스가 켜져 있는지 확인해주세요.

#define VOICE_HOTWATER_EFFLUENT_A   		(0x4C)      // 온수가 뜨거우니 주의하세요.
//#define VOICE_HOTWATER_EFFLUENT_B   		(0x4D)      // 온수가 뜨거우니 주위하세여~

//#define VOICE_LOWER_COVER_WARNING_B   	(0x4E)      // 하단 본체에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
#define VOICE_TOP_COVER_WARNING_B     		(0x4F)      // 제품 상단에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
//#define VOICE_FRONT_COVER_WARNING_B   	(0x50)      // 제품 전면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
//#define VOICE_SIDE_COVER_WARNING_B    	(0x51)      // 제품 측면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.

#define VOICE_HOT_LOCK_CHECK          		(0x52)      // 온수 버튼이 잠겨있습니다. 온수 버튼을 3초 누르시면 선택하신 온수를 다시 사용하실 수 있습니다.
#define VOICE_COLD_OFF_CHECK          		(0x53)      // 냉수가 꺼져있습니다. 냉수 버튼을 3초 누르시면 선택하신 냉수를 다시 사용하실 수 있습니다.
#define VOICE_ALL_LOCK_CHECK_CP       		(0x54)      // 모든 버튼이 잠겨있습니다. 전체 잠금 버튼을 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.
//#define VOICE_ALL_LOCK_CHECK_CHP      	(0x55)      // 모든 버튼이 잠겨있습니다. 냉수와 용량 버튼을 동시에 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.

#define VOICE_CLEAN_WATER_PREPARE     		(0x56)      // 깨끗한 물을 준비중입니다.
#define VOICE_CLEAN_WATER_COMPLETE    		(0x57)      // 깨끗한 물에 준비가 완료되었습니다.
#define VOICE_EW_START                		(0x58)      // 코웨이 살균 시스템으로 유로를 깨끗하고 안전하게 살균 중입니다.

#define VOICE_HOT_LOCK_SETTING        		(0x59)      // 온수 버튼이 잠겼습니다.
#define VOICE_HOT_LOCK_CLEAR          		(0x5A)      // 온수 버튼을 사용하실 수 있습니다.
#define VOICE_ALL_LOCK_SETTING_A      		(0x5B)      // 모든 버튼이 잠겼습니다.
//#define VOICE_ALL_LOCK_SETTING_B      	(0x5C)      // 모든 버튼이 잠겼습니다.
#define VOICE_ALL_LOCK_CLEAR          		(0x5D)      // 모든 버튼을 사용하실 수 있습니다.

#define VOICE_SETTING_VOICE           		(0x5E)		// 음성안내 모드입니다.
#define VOICE_SETTING_MELODY          		(0x5F)		// 효과음 모드입니다.
#define VOICE_SETTING_SILENT          		(0x60)		// 무음 모드입니다.

#define VOICE_COLD_OFF_SETTING        		(0x61)      // 냉수 기능이 꺼졌습니다.
//#define VOICE_COLD_ON_SETTING_JP       		(0x62)      //	냉수 기능이 켜졌습니다. 설정하신 온도까지 시간이 다소 소요됩니다.
#define VOICE_SETTING_MODE            		(0x63)      // 설정 모드를 선택하셨습니다. 코웨이 정수기에 다양한 기능을 설정하실 수 있습니다.

#define VOICE_CONTIUNE_EFFLUENT     		(0x64)      // 연속추출
#define VOICE_EFFLUENT_START        		(0x65)      // 추출음
#define VOICE_EFFLUENT_COMPLETE     		(0x66)      // 추출종료음
#define VOICE_POWER_ON                		(0x67)      // 전원 인가시(플러그인) 당당당당당~~당. 코웨이~~
#define VOICE_BEEP_SELECT             		(0x68)      // 버튼선택음(띵)
#define VOICE_BEEP_WARNING            		(0x69)      // 불가음(띵깡띵깡)
#define VOICE_BEEP_ON                 		(0x6A)      // 설정( 띵동 ↗)
#define VOICE_BEEP_OFF                		(0x6B)      // 해제( 띵동 ↘)

// 추가
#define VOICE_TOP_COVER_CHECK         		(0x6C)      // 제품의 커버가 열려있습니다. 커버를 닫아주세요.
#define VOICE_SPECIAL_FUNCTION        		(0x6D)      // 특수 모드에 진입하였습니다.
//#define VOICE_WIFI_FAIL_B             	(0x6E)      // 와이파이 연결이 실패되었습니다.
//#define VOICE_BLE_FAIL_B              	(0x6F)      // 블루투스를 통한 아이오케어 앱 연결이 실패하였습니다.
//#define VOICE_IOCARE_START_B          	(0x70)      // 아이오케어 앱에 연결을 시작합니다.
//#define VOICE_IOCARE_COMPLETE_B       	(0x71)      // 아이오케어 앱 연결이 완료되었습니다.
//#define VOICE_IOCARE_FAIL_B           	(0x72)      // 아이오케어 앱 연결이 실패하였습니다.
//#define VOICE_FILTER_CONNECT_WARNING  	(0x73)      // 필터가 올바르게 결합되지 않은 것으로 감지 되었습니다.
#define VOICE_FILTER_CONNECT_CHECK_B  		(0x74)      // 필터가 올바르게 결합되지 않은 것으로 감지 되었습니다. 다시한번 확인해주세요.

/*	icon 2.0 추가	시작		*/
#define VOICE_ALL_LOCK_ICON22				(0x75)		//	모든 버튼이 잠겨있습니다. 온수와 정수 버튼을 동시에 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.
//#define VOICE_STERIZATION_START_ICON22	(0x76)		//	코웨이 살균 시스템으로 파우셋을 깨끗하고 안전하게 살균 합니다.
//#define VOICE_STERIZATION_END_ICON22		(0x77)		//	파우셋을 깨끗하고 안전하게 살균 했습니다. 이제 안심하고 물을 드셔도 됩니다.
//#define VOICE_FILTER_CHANGE_END_ICON22	(0x78)		//	필터를 교체하셨나요? 교체하셨다면 배수호스를 연결하시거나 약 5리터의 큰그릇을 준비해주세요. 추출버튼을 누르시면 깨끗한 물을 위한 준비 과정이 시작됩니다.

#define VOICE_CHANGE_FILTER           		(0x79)      //	필터를 교체하셨나요? 교체하셨다면 배수호스를 연결하시거나 약 5리터의 큰그릇을 준비해주세요. 추출버튼을 누르시면 깨끗한 물을 위한 준비 과정이 시작됩니다. 교체하지 않으셨다면 '냉수' 버튼을 눌러 다시 사용하실 수 있습니다.
#define VOICE_CHANGE_FLOWFAUCET           	(0x7A)		//	유로파우셋을 교체하셨나요? 교체하셨다면 추출버튼을 눌러주세요. 교체하지 않으셨다면 '냉수' 버튼을 눌러주세요.
#define VOICE_CHANGE_FILTER_FLOWFAUCET     	(0x7B)		//	필터와 유로파우셋을 교체하셨나요? 교체하셨다면 배수호스를 연결하시거나 약 5리터의 큰그릇을 준비해주세요. 추출버튼을 누르시면 깨끗한 물을 위한 준비 과정이 시작됩니다. 교체하지 않으셨다면 '냉수' 버튼을 눌러 다시 사용하실 수 있습니다.
#define VOICE_FLOWFAUCET_CONNECT_CHECK		(0x7C)		//	유로파우셋이 올바르게 결합되지 않은 것으로 감지되었습니다. 제품 커버를 열어 다시 한 번 확인해주세요
#define VOICE_FILTER_FLUSHING				(0X7D)		//	교체하신 새 필터로 깨끗한 물을 제공하기 위해 준비 중에 있습니다. 잠시만 기다려 주세요.
#define VOICE_FILTER_FLOWFAUCET_FLUSHING	(0X7E)		//	교체하신 새 필터와 유로파우셋으로 깨끗한 물을 제공하기 위해 준비 중에 있습니다. 잠시만 기다려 주세요.
#define VOICE_WIFI_OFF						(0x7F)		//	와이파이 연결이 해제되었습니다. 다시 연결하시려면 냉수와 용량 버튼을 동시에 3초 눌러주세요.
#define VOICE_STERIZATION_START_ICON22		(0x80)		//	코웨이의 살균수로 유로 살균 서비스를 시작합니다.
#define VOICE_STERIZATION_RINSE_ICON22		(0x81)		//	유로 헹굼을 시작합니다.
#define VOICE_STERIZATION_END_ICON22		(0x82)		//	유로 살균 서비스를 완료했습니다.
#define VOICE_COLD_ON_SETTING         		(0x83)		//	냉수 기능이 켜졌습니다. 적정 온도 도달까지 시간이 다소 소요됩니다.
#define VOICE_MYCUP_SET						(0x84)		//	고객님의 My 용량이 설정되었습니다.
#define VOICE_SMART_CHECK_START      		(0x85)   	//  제품의 상태를 파악하기 위해 스마트 진단을 시작합니다.잠시만 기다려주세요.
#define VOICE_SMART_CHECK_OK	      		(0x86)      //  스마트 진단 결과, 제품이 정상 작동 중입니다. 안심하고 사용하세요.
#define VOICE_HIGH_HOTTEMP_EFFLUENT_1   	(0x87)      //  고온수가 추출 됩니다. 컵을 추출구에 가까이 해주세요.
#define VOICE_HIGH_HOTTEMP_EFFLUENT_2   	(0x88)      //  고온수가 추출 됩니다. 컵을 추출구에 가까이 놓아주세요.
#define VOICE_HOTWATER_FIRST			   	(0x89)      //  온수 첫 잔은 뜨겁지 않을 수 있습니다. 첫 잔을 버린 후 사용해주세요.

#define VOICE_FILTER_OFF			   		(0x8A)      //  (※국내향엔 없음 주의) 필터가 빠졌습니다.
#define VOICE_FILTER_ON			   			(0x8B)      //  (※국내향엔 없음 주의) 필터가 결합되었습니다.
#define VOICE_FLOWFAUCET_OFF		   		(0x8C)      //  (※국내향엔 없음 주의) 유로모듈이 빠졌습니다.
#define VOICE_FLOWFAUCET_ON			   		(0x8D)      //  (※국내향엔 없음 주의) 유로모듈이 결합되었습니다.

/*	icon 2.0 추가	끝		*/

//	★★	음성 필요한 것


//==============================================================

#define VOICE_ACTOR            0					// 성우
#define VOICE_CELEB            1					// BTS


/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U16 COMMAND;

EXTERN U8 gu8_Sound_Type;						// 음성안내 모드 (0,1:무음모드 / 2:효과음모드 / 3:음성안내모드)
//EXTERN U8 gu8SoundTypeAlarm_Time;
EXTERN U8 gu8PlayVoice_Count;
EXTERN U8 gu8_VoiceVolumeLevel;					//	0~4:5단계
EXTERN U8 gu8Voice_Actor;

EXTERN U16 gu16_Voice_Address;
EXTERN U16 gu16VoiceDelay;

//EXTERN U16 VOICE_TEST;

EXTERN U8 F_OneMoreSound;
EXTERN U8 F_Beep_Sound;
EXTERN U8 F_PlayVoice; 								/*Sound Level에 따른 Address 받으면 SET 하여 음성 출력*/
//EXTERN U8 F_Initial_Voice;

//EXTERN U8 F_New_Voice;

/*==============================================================================
 function Definition
 ==============================================================================*/
void Initial_Voice(void);
void SetVoiceLevel(U8 mu8Val);
U8 Get_Sound_Type(void);
void Play_Voice(U16 mu16MemoryAddress);
void SEND_SPI_COMMAND(void);
//void Check_Play_A_Voice(void);
void VoiceStop(void);
//void Voice_Coway_Start(void);
void ProcessVoice(void);

U8 IsLevel_0(U16 addr);
U8 IsLevel_1(U16 addr);
U8 IsLevel_2(U16 addr);
U8 IsLevel_3(U16 addr);
U8 IsCriticalVoice(U16 addr);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
