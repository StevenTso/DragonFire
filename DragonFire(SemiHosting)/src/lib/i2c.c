/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   parts of this code are (C) 2010, MyVoice CAD/CAM Services
 *   All rights reserved.
 *
 *   History
 *   2009.12.07  ver 1.00    Preliminary version, first Release
 *   2010.07.19  ver 1.10    Rob Jansen - MyVoice CAD/CAM Services:
 *                           Major cleaning an a rewrite of some functions
 *                           - adding ACK/NACK handling to the state machine
 *                           - adding a return result to the I2CEngine()
 *
*****************************************************************************/
#include "LPC11Uxx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "i2c.h"
#include "driver_config.h"

volatile uint32_t I2CMasterState = I2CSTATE_IDLE;
volatile uint32_t I2CSlaveState = I2CSTATE_IDLE;

volatile uint8_t I2CMasterBuffer[BUFSIZE];
volatile uint8_t I2CSlaveBuffer[BUFSIZE];
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;


/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C_IRQHandler(void) 
{
	uint8_t StatValue;

	/* this handler deals with master read and master write only */
	StatValue = LPC_I2C->STAT;
	switch ( StatValue )
	{
	case 0x08:
		/*
		 * A START condition has been transmitted.
		 * We now send the slave address and initialize
		 * the write buffer
		 * (we always start with a write after START+SLA)
		 */
		WrIndex = 0;
		LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
		LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
		I2CMasterState = I2CSTATE_PENDING;
		break;
	
	case 0x10:
		/*
		 * A repeated START condition has been transmitted.
		 * Now a second, read, transaction follows so we
		 * initialize the read buffer.
		 */
		RdIndex = 0;
		/* Send SLA with R bit set, */
		LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
		LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
		break;
	
	case 0x18:
		/*
		 * SLA+W has been transmitted; ACK has been received.
		 * We now start writing bytes.
		 */
		LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

	case 0x20:
		/*
		 * SLA+W has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		LPC_I2C->CONSET = I2CONSET_STO;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		I2CMasterState = I2CSTATE_SLA_NACK;
		break;

	case 0x28:
		/*
		 * Data in I2DAT has been transmitted; ACK has been received.
		 * Continue sending more bytes as long as there are bytes to send
		 * and after this check if a read transaction should follow.
		 */
		if ( WrIndex < I2CWriteLength )
		{
			/* Keep writing as long as bytes avail */
			LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
		}
		else
		{
			if ( I2CReadLength != 0 )
			{
				/* Send a Repeated START to initialize a read transaction */
				/* (handled in state 0x10)                                */
				LPC_I2C->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
			}
			else
			{
				I2CMasterState = I2CSTATE_ACK;
				LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
			}
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

	case 0x30:
		/*
		 * Data byte in I2DAT has been transmitted; NOT ACK has been received
		 * Send a STOP condition to terminate the transaction and inform the
		 * I2CEngine that the transaction failed.
		 */
		LPC_I2C->CONSET = I2CONSET_STO;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		I2CMasterState = I2CSTATE_NACK;
		break;

	case 0x38:
		/*
		 * Arbitration loss in SLA+R/W or Data bytes.
		 * This is a fatal condition, the transaction did not complete due
		 * to external reasons (e.g. hardware system failure).
		 * Inform the I2CEngine of this and cancel the transaction
		 * (this is automatically done by the I2C hardware)
		 */
		I2CMasterState = I2CSTATE_ARB_LOSS;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

	case 0x40:
		/*
		 * SLA+R has been transmitted; ACK has been received.
		 * Initialize a read.
		 * Since a NOT ACK is sent after reading the last byte,
		 * we need to prepare a NOT ACK in case we only read 1 byte.
		 */
		if ( I2CReadLength == 1 )
		{
			/* last (and only) byte: send a NACK after data is received */
			LPC_I2C->CONCLR = I2CONCLR_AAC;
		}
		else
		{
			/* more bytes to follow: send an ACK after data is received */
			LPC_I2C->CONSET = I2CONSET_AA;
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;

	case 0x48:
		/*
		 * SLA+R has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		LPC_I2C->CONSET = I2CONSET_STO;
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		I2CMasterState = I2CSTATE_SLA_NACK;
		break;

	case 0x50:
		/*
		 * Data byte has been received; ACK has been returned.
		 * Read the byte and check for more bytes to read.
		 * Send a NOT ACK after the last byte is received
		 */
		I2CSlaveBuffer[RdIndex++] = LPC_I2C->DAT;
		if ( RdIndex < (I2CReadLength-1) )
		{
			/* lmore bytes to follow: send an ACK after data is received */
			LPC_I2C->CONSET = I2CONSET_AA;
		}
		else
		{
			/* last byte: send a NACK after data is received */
			LPC_I2C->CONCLR = I2CONCLR_AAC;
		}
		LPC_I2C->CONCLR = I2CONCLR_SIC;
		break;
	
	case 0x58:
		/*
		 * Data byte has been received; NOT ACK has been returned.
		 * This is the last byte to read.
		 * Generate a STOP condition and flag the I2CEngine that the
		 * transaction is finished.
		 */
		I2CSlaveBuffer[RdIndex++] = LPC_I2C->DAT;
		I2CMasterState = I2CSTATE_ACK;
		LPC_I2C->CONSET = I2CONSET_STO;	/* Set Stop flag */
		LPC_I2C->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
		break;

	
	default:
		LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
  }
  return;
}

/*****************************************************************************
** Function name:	I2CStart
**
** Descriptions:	Create I2C start condition, a timeout
**					value is set if the I2C never gets started,
**					and timed out. It's a fatal error.
**
** parameters:		None
** Returned value:	true or false, return false if timed out
** 
*****************************************************************************/
static uint32_t I2CStart( void )
{
	uint32_t timeout = 0;

	/*--- Issue a start condition ---*/
	LPC_I2C->CONSET = I2CONSET_STA;	/* Set Start flag */
    
	while((I2CMasterState != I2CSTATE_PENDING) && (timeout < MAX_TIMEOUT))
	{
		timeout++;
	}

	return (timeout < MAX_TIMEOUT);
}

/*****************************************************************************
** Function name:	I2CStop
**
** Descriptions:	Set the I2C stop condition
**
** parameters:		None
** Returned value:	true or never return
** 
*****************************************************************************/
static uint32_t I2CStop( void )
{
	uint32_t timeout = 0;

	LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
	LPC_I2C->CONCLR = I2CONCLR_SIC;  /* Clear SI flag */

	/*--- Wait for STOP detected ---*/
	while((LPC_I2C->CONSET & I2CONSET_STO) && (timeout < MAX_TIMEOUT))
	{
		timeout++;
	}
	return (timeout >= MAX_TIMEOUT);
}

/*****************************************************************************
** Function name:	I2CInit
**
** Descriptions:	Initialize I2C controller
**
** parameters:		I2c mode is either MASTER or SLAVE
** Returned value:	true or false, return false if the I2C
**					interrupt handler was not installed correctly
** 
*****************************************************************************/
void I2CInit()
{
	LPC_SYSCON->PRESETCTRL |= (0x1<<1);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);
	LPC_IOCON->PIO0_4 &= ~0x3F;	/*  I2C I/O config */
	LPC_IOCON->PIO0_4 |= 0x01;		/* I2C SCL */
	LPC_IOCON->PIO0_5 &= ~0x3F;
	LPC_IOCON->PIO0_5 |= 0x01;		/* I2C SDA */

	/*--- Clear flags ---*/
	LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;

	/*--- Reset registers ---*/

	LPC_I2C->SCLL   = I2SCLL_SCLL;
	LPC_I2C->SCLH   = I2SCLH_SCLH;


	/* Enable the I2C Interrupt */
	NVIC_EnableIRQ(I2C_IRQn);
	NVIC_SetPriority(I2C_IRQn, I2CPRIORITY);

	LPC_I2C->CONSET = I2CONSET_I2EN;

}

/*****************************************************************************
** Function name:	I2CEngine
**
** Descriptions:	The routine to complete a I2C transaction
**					from start to stop. All the intermitten
**					steps are handled in the interrupt handler.
**					Before this routine is called, the read
**					length, write length and I2C master buffer
**					need to be filled.
**
** parameters:		None
** Returned value:	Any of the I2CSTATE_... values. See i2c.h
** 
*****************************************************************************/
uint32_t I2CEngine( void ) 
{
  I2CMasterState = I2CSTATE_IDLE;
  RdIndex = 0;
  WrIndex = 0;
  if ( I2CStart() != TRUE )
  {
	I2CStop();
	return ( FALSE );
  }

  /* wait until the state is a terminal state */
  while (I2CMasterState < 0x100);

  return ( I2CMasterState );
}


uint8_t readRegister(uint8_t DeviceAddress, uint8_t regAddress) {
	  I2CWriteLength = 2;
	  I2CReadLength = 1;
	  I2CMasterBuffer[0] = DeviceAddress;
	  I2CMasterBuffer[1] = regAddress;
	  I2CMasterBuffer[2] = DeviceAddress | RD_BIT;
	  I2CEngine();
	 /* printf("I2CMasterBuffer[0]: %d ", I2CMasterBuffer[0]);
	  printf("I2CMasterBuffer[1]: %d ", I2CMasterBuffer[1]);
	  printf("I2CMasterBuffer[2]: %d ", I2CMasterBuffer[2]);
	  printf("I2CMasterBuffer[3]: %d ", I2CMasterBuffer[3]);
	  printf("I2CMasterBuffer[4]: %d ", I2CMasterBuffer[4]);
	  printf("I2CMasterBuffer[5]: %d \n", I2CMasterBuffer[5]);
	  printf("I2CSlaveBuffer[0]: %d ", I2CSlaveBuffer[0]);
	  printf("I2CSlaveBuffer[1]: %d ", I2CSlaveBuffer[1]);
	  printf("I2CSlaveBuffer[2]: %d ", I2CSlaveBuffer[2]);
	  printf("I2CSlaveBuffer[3]: %d ", I2CSlaveBuffer[3]);
	  printf("I2CSlaveBuffer[4]: %d ", I2CSlaveBuffer[4]);
	  printf("I2CSlaveBuffer[5]: %d \n", I2CSlaveBuffer[5]);*/
	  return I2CSlaveBuffer[0];
}

void writeRegister(uint8_t DeviceAddress, uint8_t regAddress, uint8_t data) {
	  I2CWriteLength = 3;
	  I2CReadLength = 0;
	  I2CMasterBuffer[0] = DeviceAddress;
	  I2CMasterBuffer[1] = regAddress;
	  I2CMasterBuffer[2] = data;
	  I2CEngine();
}
/******************************************************************************
**                            End Of File
******************************************************************************/

