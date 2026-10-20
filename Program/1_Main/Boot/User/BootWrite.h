/*'""FILE COMMENT""****************************************************
@ FILE Name : BootWrite.h
@ FILE ID    : 
@ Programmer : Junbeen Son
@ Description : 
/*""FILE COMMENT END ""************************************************/

#ifndef _BOOTWRITE_H_
#define _BOOTWRITE_H_

#define PR_MSG_PACKET_SIZE          0x100   /*  */

#define START_WRITEBLOCK 			0x04
#define START_WRITEADDR  			0x00001000 
#define END_WRITEBLOCK   			0x80					//0x80직전 블록까지 Writing 
#define END_WRITEADDRESS 			0x00020000				//0x00020000주소 직전까지 Writing	

#define HEADER_BLOCK_SIZE 			0x01
#define START_SKIP_NEWBOOT_BLOCK 	16		
#define END_SKIP_NEWBOOT_BLOCK 		32

//=== ICT_OTA_SEND_DATA command ===//
#define PARAM_INDEX_START 		19
#define PARAM_DATASIZE_START	24			// index 4자리 변경으로 인한 변경	23->24	
#define PARAM_DATA_START		28			// index 4자리 변경으로 인한 변경	27->28
#define PARAM_CRC_START			285			// index 4자리 변경으로 인한 변경	284->285

//=== ICT_OTA_SEND_END command ===//
#define PARAM_RESULTCODE		18

#define BOOT_RxCheckTime 		3000		// 3000*10ms = 30sec

#define AUCON_DEACTIVATE   		0x30	//'0'
#define AUCON_ACTIVATE   		0x31	//'1'


typedef enum
{
	AT_ICT_AUCONMODE=0,
	AT_ICT_OTA_SEND_DATA,  
	AT_ICT_OTA_SEND_END,
	
	AT_ICT_OTA_MAX
};

typedef enum
{
	ICT_OTA_SEND_DATA=0, 
	ICT_OTA_SEND_END,
	
	ICT_OTA_MAX
};

typedef struct
{
	U8 mu8num;
	__far U8* pmu8Command; 
	U16 mu16length;			// ETX를 제외한 길이
}BOOT_COMMAND_T;  

U8 BOOT_writeProgram(void);
U8 BOOT_startFSL(void);
U8 BOOT_checkFslError(U8 mu8SelfResult);
U8 BOOT_UartRxMsg(U8* pmu8DataBuffer, U8* pmu8Command);
U8 BOOT_UartRxData(void);
U8 BOOT_UartRxCheck(void);
void BOOT_UartTxMsg(U8 mu8num);
void BOOT_UartTxData(U8* pmu8SendData);
U16 CRC16_CCITT_FALSE(U8* puchMsg, U16 usDataLen);
U16 CRC16_CCITT_FALSE_BIN(void);
U8 ASCII_TO_HEX_ONE(U8 Upper, U8 Lower);

#endif

