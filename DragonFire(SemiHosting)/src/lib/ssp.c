/*
 * ssp.c
 *
 *  Created on: Jan 5, 2013
 *      Author: steven
 */

#include "LPC11Uxx.h"			/* LPC11Uxx Peripheral Registers */
#include "ssp.h"
#include "gpio.h"

void SSP0_Init(void) {
	//uint8_t Dummy;

	/*1*/
	LPC_IOCON->PIO0_2 |= (0x1<<0); //SSEL0
	LPC_IOCON->PIO0_8 |= (0x1<<0); //MISO0
	LPC_IOCON->PIO0_9 |= (0x1<<0); //MOSI0
	LPC_IOCON->PIO1_29 |= (0x1<<0); //SCK0
	/*2*/
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<11); //enables clock to SSP0
	/* Enable AHB clock to the GPIO domain. */
	//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	/*3*/
	LPC_SYSCON->SSP0CLKDIV = (0x1<<0); //divides clock by 1 to get clock rate (FOR NOW)

	/*4*/
	LPC_SYSCON->PRESETCTRL |= (0x1<<0); //SSP0 reset de-asserted

	GPIOSetDir(SSP0_SSELPORT, SSP0_SSELPIN, OUTPUT); //SSEL0
	GPIOSetBitValue(SSP0_SSELPORT, SSP0_SSELPIN, HIGH); //SSEL0 high

	/**************************************************************************************************/
	//LPC_SSP0->CR0 |= (0x07); //8-bit payloads
	LPC_SSP0->CR0 = 0x0707;

	LPC_SSP0->CPSR |= (0x1<<1); //prescaler value of 2 since I have to initialize this (FOR NOW)

	/*for ( i = 0; i < 8; i++ ) {
		Dummy = LPC_SSP0->DR;		 //clear the RxFIFO
	}*/

	LPC_SSP0->CR1 |= (0x1<<1); //SPI enable

}

void SSP0_Send(uint8_t *buf, uint32_t Length ) {
	uint32_t i;
	uint8_t Dummy = Dummy;

	for ( i = 0; i < Length; i++ ) {
		/* Move on only if NOT busy and TX FIFO not full. */
		while ( (LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
		LPC_SSP0->DR = *buf;
		buf++;
		while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
		/* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
		on MISO. Otherwise, when SSP0Receive() is called, previous data byte
		is left in the FIFO. */
		Dummy = LPC_SSP0->DR;
	}
	return;
}

void SSP0_Receive(uint8_t *buf, uint32_t Length ) {
	uint32_t i;

	for ( i = 0; i < Length; i++ ) {
		/* As long as Receive FIFO is not empty, I can always receive. */
		/* If it's a loopback test, clock is shared for both TX and RX,
		no need to write dummy byte to get clock to get the data */
		/* if it's a peer-to-peer communication, SSPDR needs to be written
		before a read can take place. */
		LPC_SSP0->DR = 0xFF;
		/* Wait until the Busy bit is cleared */
		while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
		*buf = LPC_SSP0->DR;
		buf++;
	}
	return;
}
