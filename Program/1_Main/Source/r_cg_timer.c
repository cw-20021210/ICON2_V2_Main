/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer.c
* Version      : CodeGenerator for RL78/G13 V2.05.03.01 [12 Nov 2018]
* Device(s)    : R5F100PG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2021-12-20
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TAU1_ch0 pulse measurement */
extern volatile uint32_t g_tau1_ch0_width;
/* For TAU1_ch2 pulse measurement */
extern volatile uint32_t g_tau1_ch2_width;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TAU0_Create
* Description  : This function initializes the TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Create(void)
{
    TAU0EN = 1U;    /* supplies input clock */
    TPS0 = _0000_TAU_CKM0_FCLK_0 | _0020_TAU_CKM1_FCLK_2 | _0000_TAU_CKM2_FCLK_1 | _0000_TAU_CKM3_FCLK_8;
    /* Stop all channels */
    TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
          _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON |
          _0800_TAU_CH3_H8_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK00 = 1U;    /* disable INTTM00 interrupt */
    TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
    /* Mask channel 1 interrupt */
    TMMK01 = 1U;    /* disable INTTM01 interrupt */
    TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
    /* Mask channel 1 higher 8 bits interrupt */
    TMMK01H = 1U;    /* disable INTTM01H interrupt */
    TMIF01H = 0U;    /* clear INTTM01H interrupt flag */
    /* Mask channel 2 interrupt */
    TMMK02 = 1U;    /* disable INTTM02 interrupt */
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
    /* Mask channel 3 interrupt */
    TMMK03 = 1U;    /* disable INTTM03 interrupt */
    TMIF03 = 0U;    /* clear INTTM03 interrupt flag */
    /* Mask channel 3 higher 8 bits interrupt */
    TMMK03H = 1U;    /* disable INTTM03H interrupt */
    TMIF03H = 0U;    /* clear INTTM03H interrupt flag */
    /* Mask channel 4 interrupt */
    TMMK04 = 1U;    /* disable INTTM04 interrupt */
    TMIF04 = 0U;    /* clear INTTM04 interrupt flag */
    /* Mask channel 5 interrupt */
    TMMK05 = 1U;    /* disable INTTM05 interrupt */
    TMIF05 = 0U;    /* clear INTTM05 interrupt flag */
    /* Mask channel 6 interrupt */
    TMMK06 = 1U;    /* disable INTTM06 interrupt */
    TMIF06 = 0U;    /* clear INTTM06 interrupt flag */
    /* Mask channel 7 interrupt */
    TMMK07 = 1U;    /* disable INTTM07 interrupt */
    TMIF07 = 0U;    /* clear INTTM07 interrupt flag */
    /* Set INTTM00 low priority */
    TMPR100 = 1U;
    TMPR000 = 1U;
    /* Channel 0 used as interval timer */
    TMR00 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
    TDR00 = _7CFF_TAU_TDR00_VALUE;
    TO0 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1;
    TOE0 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    /* Channel 2 is used as master channel for PWM output function */
    TMR02 = _8000_TAU_CLOCK_SELECT_CKM1 | _0000_TAU_CLOCK_MODE_CKS | _0800_TAU_COMBINATION_MASTER |
            _0000_TAU_TRIGGER_SOFTWARE | _0001_TAU_MODE_PWM_MASTER;
    TDR02 = _DABF_TAU_TDR02_VALUE;
    TOM0 &= ~_0004_TAU_CH2_OUTPUT_COMBIN;
    TOL0 &= ~_0004_TAU_CH2_OUTPUT_LEVEL_L;
    TO0 &= ~_0004_TAU_CH2_OUTPUT_VALUE_1;
    TOE0 &= ~_0004_TAU_CH2_OUTPUT_ENABLE;
    /* Channel 3 is used as slave channel for PWM output function */
    TMR03 = _8000_TAU_CLOCK_SELECT_CKM1 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0400_TAU_TRIGGER_MASTER_INT | _0009_TAU_MODE_PWM_SLAVE;
    TDR03 = _0000_TAU_TDR03_VALUE;
    TOM0 |= _0008_TAU_CH3_OUTPUT_COMBIN;
    TOL0 &= ~_0008_TAU_CH3_OUTPUT_LEVEL_L;
    TO0 &= ~_0008_TAU_CH3_OUTPUT_VALUE_1;
    TOE0 |= _0008_TAU_CH3_OUTPUT_ENABLE;
    /* Channel 5 is used as slave channel for PWM output function */
    TMR05 = _8000_TAU_CLOCK_SELECT_CKM1 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0400_TAU_TRIGGER_MASTER_INT | _0009_TAU_MODE_PWM_SLAVE;
    TDR05 = _0000_TAU_TDR05_VALUE;
    TOM0 |= _0020_TAU_CH5_OUTPUT_COMBIN;
    TOL0 &= ~_0020_TAU_CH5_OUTPUT_LEVEL_L;
    TO0 &= ~_0020_TAU_CH5_OUTPUT_VALUE_1;
    TOE0 |= _0020_TAU_CH5_OUTPUT_ENABLE;
    /* Channel 4 used as square output function */
    TMR04 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
    TDR04 = _0C7F_TAU_TDR04_VALUE;
    TOM0 &= ~_0010_TAU_CH4_OUTPUT_COMBIN;
    TOL0 &= ~_0010_TAU_CH4_OUTPUT_LEVEL_L;
    TO0 &= ~_0010_TAU_CH4_OUTPUT_VALUE_1;
    TOE0 |= _0010_TAU_CH4_OUTPUT_ENABLE;
    /* Set TO03 pin */
    P3 &= 0xFDU;
    PM3 &= 0xFDU;
    /* Set TO05 pin */
    P4 &= 0xBFU;
    PM4 &= 0xBFU;
    /* Set TO04 pin */
    P4 &= 0xFBU;
    PM4 &= 0xFBU;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Start
* Description  : This function starts TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Start(void)
{
    TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
    TMMK00 = 0U;    /* enable INTTM00 interrupt */
    TS0 |= _0001_TAU_CH0_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Stop
* Description  : This function stops TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Stop(void)
{
    TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK00 = 1U;    /* disable INTTM00 interrupt */
    TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Start
* Description  : This function starts TAU0 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Start(void)
{
    TOE0 |= _0008_TAU_CH3_OUTPUT_ENABLE | _0020_TAU_CH5_OUTPUT_ENABLE;
    TS0 |= _0004_TAU_CH2_START_TRG_ON | _0008_TAU_CH3_START_TRG_ON | _0020_TAU_CH5_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Stop
* Description  : This function stops TAU0 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Stop(void)
{
    TT0 |= _0004_TAU_CH2_STOP_TRG_ON | _0008_TAU_CH3_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON;
    TOE0 &= ~_0008_TAU_CH3_OUTPUT_ENABLE & ~_0020_TAU_CH5_OUTPUT_ENABLE;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel4_Start
* Description  : This function starts TAU0 channel 4 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel4_Start(void)
{
    TOE0 |= _0010_TAU_CH4_OUTPUT_ENABLE;
    TS0 |= _0010_TAU_CH4_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel4_Stop
* Description  : This function stops TAU0 channel 4 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel4_Stop(void)
{
    TT0 |= _0010_TAU_CH4_STOP_TRG_ON;
    TOE0 &= ~_0010_TAU_CH4_OUTPUT_ENABLE;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Create
* Description  : This function initializes the TAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Create(void)
{
    TAU1EN = 1U;    /* supplies input clock */
    TPS1 = _0000_TAU_CKM0_FCLK_0 | _0080_TAU_CKM1_FCLK_8 | _0000_TAU_CKM2_FCLK_1 | _0000_TAU_CKM3_FCLK_8;
    /* Stop all channels */
    TT1 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON | _0800_TAU_CH3_H8_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK10 = 1U;    /* disable INTTM10 interrupt */
    TMIF10 = 0U;    /* clear INTTM10 interrupt flag */
    /* Mask channel 1 interrupt */
    TMMK11 = 1U;    /* disable INTTM11 interrupt */
    TMIF11 = 0U;    /* clear INTTM11 interrupt flag */
    /* Mask channel 1 higher 8 bits interrupt */
    TMMK11H = 1U;    /* disable INTTM11H interrupt */
    TMIF11H = 0U;    /* clear INTTM11H interrupt flag */
    /* Mask channel 2 interrupt */
    TMMK12 = 1U;    /* disable INTTM12 interrupt */
    TMIF12 = 0U;    /* clear INTTM12 interrupt flag */
    /* Mask channel 3 interrupt */
    TMMK13 = 1U;    /* disable INTTM13 interrupt */
    TMIF13 = 0U;    /* clear INTTM13 interrupt flag */
    /* Mask channel 3 higher 8 bits interrupt */
    TMMK13H = 1U;    /* disable INTTM13H interrupt */
    TMIF13H = 0U;    /* clear INTTM13H interrupt flag */
    /* Set INTTM10 low priority */
    TMPR110 = 1U;
    TMPR010 = 1U;
    /* Set INTTM12 high priority */
    TMPR112 = 0U;
    TMPR012 = 0U;
    
    //------------------------------------BEFORE-----------------------------------------------
    /* Channel 0 is used to measure input pulse interval */
    /*
    TMR10 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0100_TAU_TRIGGER_TIMN_VALID | _0000_TAU_TIMN_EDGE_FALLING | _0004_TAU_MODE_CAPTURE |
            _0000_TAU_START_INT_UNUSED;
    TO1 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1;
    TOE1 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    NFEN2 &= (uint8_t)~_01_TAU_CH0_NOISE_ON;
    */
    /* disable using noise filter of TI10 pin input signal */
    /* Channel 2 is used to measure input pulse interval */
    
    TMR12 = _8000_TAU_CLOCK_SELECT_CKM1 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0100_TAU_TRIGGER_TIMN_VALID | _0000_TAU_TIMN_EDGE_FALLING | _0004_TAU_MODE_CAPTURE |
            _0000_TAU_START_INT_UNUSED;
    TOM1 &= ~_0004_TAU_CH2_OUTPUT_COMBIN;
    TOL1 &= ~_0004_TAU_CH2_OUTPUT_LEVEL_L;
    TO1 &= ~_0004_TAU_CH2_OUTPUT_VALUE_1;
    TOE1 &= ~_0004_TAU_CH2_OUTPUT_ENABLE;
    NFEN2 &= (uint8_t)~_04_TAU_CH2_NOISE_ON;    /* disable using noise filter of TI12 pin input signal */
    /* Set TI10 pin */
    //PM6 |= 0x10U;
    /* Set TI12 pin */
    //PM6 |= 0x40U;

    
    //------------------------------------AFTER-----------------------------------------------
    /* Channel 0 is used as master channel for PWM output function */
    TMR10 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_MASTER |
            _0000_TAU_TRIGGER_SOFTWARE | _0001_TAU_MODE_PWM_MASTER;
    TDR10 = _DABF_TAU_TDR10_VALUE;
    //TDR10 = 0x0C7FU;

    TO1 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1;
    TOE1 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    /* Channel 1 is used as slave channel for PWM output function */
    TMR11 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0400_TAU_TRIGGER_MASTER_INT | _0009_TAU_MODE_PWM_SLAVE;
    //TDR11 = _0000_TAU_TDR11_VALUE;
    TDR11 = 0xDABFU;
    TOM1 |= _0002_TAU_CH1_OUTPUT_COMBIN;
    TOL1 &= ~_0002_TAU_CH1_OUTPUT_LEVEL_L;
    TO1 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;
    TOE1 |= _0002_TAU_CH1_OUTPUT_ENABLE;
    /* Set TO11 pin */
    P6 &= 0xDFU;
    PM6 &= 0xDFU;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel0_Start
* Description  : This function starts TAU1 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//void R_TAU0_Channel2_Start(void)
//{
//    TOE0 |= _0008_TAU_CH3_OUTPUT_ENABLE | _0020_TAU_CH5_OUTPUT_ENABLE;
//    TS0 |= _0004_TAU_CH2_START_TRG_ON | _0008_TAU_CH3_START_TRG_ON | _0020_TAU_CH5_START_TRG_ON;
//}


void R_TAU1_Channel0_Start(void)
{   
    /*
    TMIF10 = 0U;    // clear INTTM10 interrupt flag 
    TMMK10 = 0U;    // enable INTTM10 interrupt 
    TS1 |= _0001_TAU_CH0_START_TRG_ON;
    */
    TMIF10 = 0U;    /* clear INTTM10 interrupt flag */
    TMMK10 = 0U;    /* enable INTTM10 interrupt */
    TMIF11 = 0U;    /* clear INTTM11 interrupt flag */
    TMMK11 = 0U;    /* enable INTTM11 interrupt */
    TOE1 |= _0002_TAU_CH1_OUTPUT_ENABLE;
    //TOE1 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    TS1 |= _0001_TAU_CH0_START_TRG_ON | _0002_TAU_CH1_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel0_Stop
* Description  : This function stops TAU1 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel0_Stop(void)
{
    //TT1 |= _0001_TAU_CH0_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    //TMMK10 = 1U;    /* disable INTTM10 interrupt 
    //TMIF10 = 0U;    /* clear INTTM10 interrupt flag 
    TT1 |= _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON;
    TOE1 &= ~_0002_TAU_CH1_OUTPUT_ENABLE;
    /* Mask channel 0 interrupt */
    TMMK10 = 1U;    /* disable INTTM10 interrupt */
    TMIF10 = 0U;    /* clear INTTM10 interrupt flag */
    /* Mask channel 1 interrupt */
    TMMK11 = 1U;    /* disable INTTM11 interrupt */
    TMIF11 = 0U;    /* clear INTTM11 interrupt flag */
    
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel0_Get_PulseWidth
* Description  : This function measures TAU1 channel 0 input pulse width.
* Arguments    : width -
*                    the address where to write the input pulse width
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel0_Get_PulseWidth(uint32_t * const width)
{
    /* For channel 0 pulse measurement */
    *width = g_tau1_ch0_width;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel2_Start
* Description  : This function starts TAU1 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel2_Start(void)
{
    TMIF12 = 0U;    /* clear INTTM12 interrupt flag */
    TMMK12 = 0U;    /* enable INTTM12 interrupt */
    TS1 |= _0004_TAU_CH2_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel2_Stop
* Description  : This function stops TAU1 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel2_Stop(void)
{
    TT1 |= _0004_TAU_CH2_STOP_TRG_ON;
    /* Mask channel 2 interrupt */
    TMMK12 = 1U;    /* disable INTTM12 interrupt */
    TMIF12 = 0U;    /* clear INTTM12 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel2_Get_PulseWidth
* Description  : This function measures TAU1 channel 2 input pulse width.
* Arguments    : width -
*                    the address where to write the input pulse width
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel2_Get_PulseWidth(uint32_t * const width)
{
    /* For channel 2 pulse measurement */
    *width = g_tau1_ch2_width;
}

/* Start user code for adding. Do not edit comment generated here */
void Timer_Start(void)
{
	R_TAU0_Channel0_Start();
	R_TAU0_Channel2_Start();
	R_TAU0_Channel4_Start();
	R_TAU1_Channel0_Start();
	R_TAU1_Channel2_Start();
}
/* End user code. Do not edit comment generated here */
