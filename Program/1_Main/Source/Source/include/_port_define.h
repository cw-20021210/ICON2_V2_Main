#ifndef _PORT_DEFINE_H_
#define _PORT_DEFINE_H_

//*************************
//*    PORT DEFINITION    *
//*************************

// EEPROM
#define P_EEPROM_SCL			(P6.0)    			//	I2C CLK - EEPROM, RTC
#define P_EEPROM_SDA			(P6.1)    			//	I2C DAT - EEPROM, RTC
#define P_EEPROM_SCL_PIN_MODE	(PM6.0)   			//	I2C CLK Port IO Mode Select Register
#define P_EEPROM_SDA_PIN_MODE	(PM6.1)   			//	I2C DAT Port IO Mode Select Register

//	Valve
#define P_NOS_V             (P3.0)  				//	NOS 밸브 (HIGH:Close / Low:Open (Normal Open))
#define P_FEED_COLD_IN_V    (P1.6)  				//
#define P_FEED_COLD_OUT_V   (P5.5)  				//
#define P_FEED_HOTOUT_V    	(P8.6)  				//	온수 추출 피드 밸브 (LOW:온수 추출 OFF / HIGH:온수 추출)
#define P_FEED_HOTIN_V      (P5.3)  				//	온수 입수 피드 밸브
#define P_FEED_PURE_V       (P5.7)  				//	정수 피드 밸브

//	Stepping Motor
#define P_STEPMOTOR1		(P14.6)
#define P_STEPMOTOR2		(P11.1)
#define P_STEPMOTOR3		(P11.0)
#define P_STEPMOTOR4		(P10.1)

//	Heater5
#define P_HEATER_RELAY	    (P4.5) 					//	Heater 2
#define P_HEATER_TRIAC		(P4.6) 					//	Heater 1
#define P_HEATER_PWM_TDR	(TDR05)

//	Cooler
//#define P_TEM_PWM 			(P3.1)  				//	TEM PWM (TO07)
//#define P_TEM_PWM_TDR		        (TDR03)
#define P_TEM_PWM 			(P6.5)  				//	TEM PWM (TO07)
#define P_TEM_PWM_TDR		        (TDR11)

#define P_TEM_12V_ONOFF         	(P7.7)
#define P_TEM_24V_ONOFF         	(P5.0)
//#define P_TEM_ONOFF			(P5.0)


#define P_FAN_18V			(P8.2)					//	18V 21V 둘다 ON하면 18V
#define P_FAN_21V			(P8.4)
#ifndef _FAN_CIRCUIT_CHANGE_
#define P_FAN_ONOFF			(P8.0)
#endif
#define P_STANDBY			(P7.4)


//	Sensor
#define P_REED_SW_FILTER	(P4.1)
#define P_REED_SW_COVER		(P6.7)
#define P_REED_SW_WATERFLOW	(P14.0)

//	Output
#define P_LEAKSENSOR_OFF	(P7.6)  				//	Leakage Sensor
#define P_COLDSENSOR_ON		(P6.2)

//	Input
#define P_MODEL_NATIONAL	(P14.1)

//	Voice
#define P_VOICE_RESET		(P0.5)
#define P_VOICE_CSB			(P0.6)
#define P_VOICE_CLK			(P1.0)
#define P_VOICE_DATA		(P1.1)

//	etc.
#define P_FLOWMETER_TDR		(TDR12)
#define P_BUZZER			(P10.2)

#define P_FCT_Rx			(P14.3)
#define P_FCT_Tx			(P14.4)


#endif
