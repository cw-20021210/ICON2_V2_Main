
#ifndef _WIFI_COMMON_PRO_H_
#define _WIFI_COMMON_PRO_H_

#ifdef HIDDEN_PRO

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

/********** ICON_MAX **********/
#define MODEL_NO_3
#define WIFI_VERSION    "V0.1.0.1"
#define MODEL_NAME_CHP  "CHP-5610N"
#define MODEL_NAME_CP   "CHP-5610N"
#define MODEL_NAME_CHP_1  "CHP-5610N"
#define MODEL_NAME_CP_1   "CHP-5610N"


// USER WIFI Port & Register
#define R_TXD           TXD1
#define R_RXD           RXD1
#define R_STIF          STIF1
#define P_WIFI_ON       P0.0
#define P_WIFI_RSTN     P14.2

// USER Define (Number of User Data)
#define USER_DATA_MAX_NUM   30  // 사용하는 데이터 총 개수
#define USER_INFO_MAX_NUM   10  // 사용하는 정보 총 개수
//#define USER_USE_ACCUMULATE   // 누적데이터 사용여부
#define USER_USE_INFO           // 정보데이터 사용여부

// USER Define (RomSize/ProductNum)
#define FOTA            1           // 0:Unused / 1:Used Fota
#define FOTA_SIZE       "513"       // FotaSize = Rom Size * 4 + 1
#define FOTA_TYPE_CHP   "113266"    // Product Code Number
#define FOTA_TYPE_CP    "113266"    // Product Code Number
#define FOTA_TYPE_CHP_1 "113266"    // Product Code Number
#define FOTA_TYPE_CP_1  "113266"    // Product Code Number

// USER Define (EEPROM Address)
#define EEPROM_ADDR_WIFI_POWER      0x12   // WIFI On/Off Status
#define EEPROM_ADDR_FIRST_PARING    0x13   // WIFI First Paring Status
#define EEPROM_ADDR_FOTA_MODULE     0x14   // WIFI Module Fota Status
#define EEPROM_ADDR_FOTA_MCU        0x15   // WIFI MCU Fota Status

// USER Define (Buzzer)
#define	WIFI_BUZZER_SELECT          6U  // 페어링 (선택음 "띵")
#define	WIFI_BUZZER_AP_CONNECT      11U // AP연결 (진입음 "띠리링" / 와이파이 연결을 시작합니다)
#define	WIFI_BUZZER_SETUP           8U  // WIFI ON (설정음 "띵동↗" / 와이파이 연결을 시작합니다)
#define	WIFI_BUZZER_SERVER_CONNECT  12U // 서버연결 (연결음 "띠리링↗" / 와이파이 연결이 완료되었습니다)
#define	WIFI_BUZZER_FAIL            9U  // 페어링30분 경과 (해제음 "동띵" / 와이파이 연결이 다시 시도해주세요)
#define	WIFI_BUZZER_CANCEL          9U  // WIFI OFF (해제음 "동띵" / 와이파이 연결이 해제되었습니다)
#define WIFI_BUZZER_AP_START        8U  // AP시작음 (설정음 "띵동 ↗")
#define	WIFI_BUZZER_ERROR           7U  // 불가음 (에러음 "띵깡띵깡")
#endif

#endif

