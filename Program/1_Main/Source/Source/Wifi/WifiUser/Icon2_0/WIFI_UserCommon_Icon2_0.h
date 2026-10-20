
#ifndef _WIFI_COMMON_MAX_H_
#define _WIFI_COMMON_MAX_H_

#ifdef ICON_MAX

/***** User headers ****************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "WIFI_DefUser.h"

/***** Type Define *****************************************************************/
typedef unsigned char       U8;
typedef signed char         I8;
typedef unsigned int        U16;
typedef signed int          I16;
typedef const signed char   CI8;
typedef const signed int    CI16;
typedef unsigned long int   U32;
typedef float               F32;
typedef double              D64;

/***** library headers ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

/********** icon 2.0 **********/
#define MODEL_NO_3
#define WIFI_VERSION    "V1.0.0.1"	//  V2 MP Version Test 용
//#define WIFI_VERSION    "V1.0.1.1"	//V2 hot test

#define MODEL_NAME_CHP  "CHP-7211N"
#define MODEL_NAME_CP   "CP-7211N"


// USER WIFI Port & Register
#define R_TXD           TXD1
#define R_RXD           RXD1
#define R_STIF          STIF1
#define P_WIFI_ON       P14.5
#define P_WIFI_RSTN     P0.0

// USER Define (Number of User Data)
#define USER_DATA_MAX_NUM   35  // The number of data
#define USER_INFO_MAX_NUM   15  // The number of info
//#define USER_USE_ACCUMULATE     // A10A0 used / unused
//#define USER_USE_WEATHER        // A2010 Weather used / unused
#define USER_USE_ACK            // WiFi Ack used / unused

// USER Define (RomSize/ProductNum)
#define FOTA            1           // 0:Unused / 1:Used Fota
#define FOTA_SIZE       "513"       // FotaSize = Rom Size * 4 + 1
//#define FOTA_TYPE_CHP   "113417"    // Product Code Number
//#define FOTA_TYPE_CP    "113418"    // Product Code Number
#define FOTA_TYPE_CHP   "114280"    // Product Code Number
#define FOTA_TYPE_CP    "114286"    // Product Code Number

// USER Define (EEPROM Address)
#define EEPROM_ADDR_WIFI_POWER      0x12   // WIFI On/Off Status
#define EEPROM_ADDR_FIRST_PARING    0x13   // WIFI First Paring Status
#define EEPROM_ADDR_FOTA_MODULE     0x14   // WIFI Module Fota Status
#define EEPROM_ADDR_FOTA_MCU        0x15   // WIFI MCU Fota Status

// USER Define (Buzzer)
//#define	WIFI_BUZZER_SELECT          6U  // 페어링 (선택음 "띵")
//#define	WIFI_BUZZER_AP_CONNECT      11U // AP연결 (진입음 "띠리링" / 와이파이 연결을 시작합니다)
//#define	WIFI_BUZZER_SETUP           8U  // WIFI ON (설정음 "띵동↗" / 와이파이 연결을 시작합니다)
//#define	WIFI_BUZZER_SERVER_CONNECT  12U // 서버연결 (연결음 "띠리링↗" / 와이파이 연결이 완료되었습니다)
//#define	WIFI_BUZZER_FAIL            9U  // 페어링30분 경과 (해제음 "동띵" / 와이파이 연결이 다시 시도해주세요)
//#define	WIFI_BUZZER_CANCEL          9U  // WIFI OFF (해제음 "동띵" / 와이파이 연결이 해제되었습니다)
//#define WIFI_BUZZER_AP_START        8U  // AP시작음 (설정음 "띵동 ↗")
//#define	WIFI_BUZZER_ERROR           7U  // 불가음 (에러음 "띵깡띵깡")
//#define	WIFI_BUZZER_SELECT          0x68 // 버튼선택음(띵)
//#define	WIFI_BUZZER_AP_CONNECT      0x40 // 와이파이 연결을 시작합니다.
//#define	WIFI_BUZZER_SETUP           0x40 // 와이파이 연결을 시작합니다.
//#define	WIFI_BUZZER_SERVER_CONNECT  0x44 // 와이파이 연결이 완료되었습니다.
//#define	WIFI_BUZZER_AP_FAIL         0x42 // 와이파이 연결이 다시 시도해주세요.
//#define	WIFI_BUZZER_CANCEL          0x45 // 와이파이 연결이 해제되었습니다.
//#define WIFI_BUZZER_AP_START        0x6A // 설정( 띵동 ↗)
//#define	WIFI_BUZZER_ERROR           0x69 // 불가음(띵깡띵깡)
//#define WIFI_BUZZER_BLE_OFF	        0x6B
//#define WIFI_BUZZER_SERVER_FIRST    0x41

#define WIFI_BUZZER_SELECT          0x68 // 버튼선택음(띵)
#define	WIFI_BUZZER_AP_CONNECT      0x40 // 와이파이 연결을 시작합니다.
#define	WIFI_BUZZER_SETUP           0x40 // 와이파이 연결을 시작합니다.
#define	WIFI_BUZZER_SERVER_CONNECT  0x44 // 와이파이 연결이 완료되었습니다.
#define	WIFI_BUZZER_AP_FAIL         0x42 // 와이파이 연결이 실패되었습니다. 인터넷 공유기를 확인해주세요. 문제가 반복되면 콜센터에 연락주세요.
#define	WIFI_BUZZER_CANCEL          0x7F // 와이파이 연결이 해제되었습니다. 다시 연결하시려면 냉수와 용량 버튼을 동시에 3초 눌러주세요.
#define WIFI_BUZZER_AP_START        0x6A // 설정( 띵동 ↗)
#define WIFI_BUZZER_ERROR           0x69 // 불가음(띵깡띵깡)
#define WIFI_BUZZER_BLE_OFF	        0x6B //	해제음
#define WIFI_BUZZER_SERVER_FIRST    0x41 // 와이파이 연결이 완료되었습니다. 이제부터 아이오케어 앱을 통해 스마트한 관리를 받으실 수 있습니다.

#endif

#endif

