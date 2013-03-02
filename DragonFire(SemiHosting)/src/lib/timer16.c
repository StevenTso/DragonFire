/****************************************************************************
 *   $Id:: timer16.c 4785 2010-09-03 22:39:27Z nxp21346                     $
 *   Project: NXP LPC11xx 16-bit timer example
 *
 *   Description:
 *     This file contains 16-bit timer code example which include timer 
 *     initialization, timer interrupt handler, and related APIs for 
 *     timer setup.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
//#include "driver_config.h"
#include "timer16.h"
#include "gpio.h"
#include "i2c.h"
#include "MPU_6050.h"
#include "ff.h"
#include "LPC11Uxx.h"
#include "driver_config.h"
#include <stdio.h>
volatile uint32_t timer16_0_counter = 0;
volatile uint32_t timer16_1_counter = 0;
volatile uint32_t timer16_0_capture = 0;
volatile uint32_t timer16_1_capture = 0;
volatile uint32_t timer16_0_period = 0;
volatile uint32_t timer16_1_period = 0;

/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if (timer_num == 0)
  {
    /*
    * setup timer #0 for delay
    */
    LPC_CT16B0->TCR = 0x02;		/* reset timer */
    LPC_CT16B0->PR  = MHZ_PRESCALE; /* set prescaler to get 1 M counts/sec */
    LPC_CT16B0->MR0 = delayInMs * TIME_INTERVALmS;
    LPC_CT16B0->IR  = 0xff;		/* reset all interrrupts */
    LPC_CT16B0->MCR = 0x04;		/* stop timer on match */
    LPC_CT16B0->TCR = 0x01;		/* start timer */
    /* wait until delay time has elapsed */
    while (LPC_CT16B0->TCR & 0x01);
  }
  else if (timer_num == 1)
  {
    /*
    * setup timer #1 for delay
    */
    LPC_CT16B1->TCR = 0x02;		/* reset timer */
    LPC_CT16B1->PR  = MHZ_PRESCALE; /* set prescaler to get 1 M counts/sec */
    LPC_CT16B1->MR0 = delayInMs * TIME_INTERVALmS;
    LPC_CT16B1->IR  = 0xff;		/* reset all interrrupts */
    LPC_CT16B1->MCR = 0x04;		/* stop timer on match */
    LPC_CT16B1->TCR = 0x01;		/* start timer */
    /* wait until delay time has elapsed */
    while (LPC_CT16B1->TCR & 0x01);
  }
  return;
}

/******************************************************************************
** Function name:		TIMER16_0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER16_0_IRQHandler(void)
{  
  if ( LPC_CT16B0->IR & 0x1 )
  {
  LPC_CT16B0->IR = 1;			/* clear interrupt flag */
  timer16_0_counter++;
  }
  if ( LPC_CT16B0->IR & (0x1<<4) )
  {
	LPC_CT16B0->IR = 0x1<<4;		/* clear interrupt flag */
	timer16_0_capture++;
  }
  return;
}

/******************************************************************************
** Function name:		TIMER16_1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER16_1_IRQHandler(void)
{  


	return;
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void enable_timer16(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_CT16B0->TCR = 1;
  }
  else
  {
    LPC_CT16B1->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void disable_timer16(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_CT16B0->TCR = 0;
  }
  else
  {
    LPC_CT16B1->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void reset_timer16(uint8_t timer_num)
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
    regVal = LPC_CT16B0->TCR;
    regVal |= 0x02;
    LPC_CT16B0->TCR = regVal;
  }
  else
  {
    regVal = LPC_CT16B1->TCR;
    regVal |= 0x02;
    LPC_CT16B1->TCR = regVal;
  }
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		None
** 
******************************************************************************/
void init_timer16_1()
{
      /* Some of the I/O pins need to be clearfully planned if
    you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<8);
    //LPC_IOCON->PIO0_20 |= (0x1<<0); //CAP0
    //LPC_IOCON->PIO0_21 |= (0x1<<0); //MAT0
    //LPC_IOCON->PIO0_22 |= (0x1<<0); //MAT1

    timer16_1_counter = 0;
	timer16_1_capture = 0;
    LPC_CT16B1->PR  = MHZ_PRESCALE; /* set prescaler to get 1 M counts/sec */
	//LPC_CT16B1->PR = 0xFFFF;
	//LPC_CT16B1->PC = 0x30;
    LPC_CT16B1->MCR = (0x1<<0); //enable Match0 interrupt
    LPC_CT16B1->MR0 = TIME_INTERVALmS*10;
    //LPC_CT16B0->MR0 = TIME_INTERVALmS * 10; /* Set up 10 mS interval */
//USE A MATCH TO DO INTERRUPT
	/* Capture 0 on rising edge, interrupt enable. */
	LPC_CT16B1->CCR = (0x1<<0)|(0x1<<2);
    //LPC_CT16B1->MCR = 3;				/* Interrupt and Reset on MR0 and MR1 */

    /* Enable the TIMER1 Interrupt */
    NVIC_EnableIRQ(TIMER_16_1_IRQn);
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
