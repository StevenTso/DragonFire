/*
 * ssp.h
 *
 *  Created on: Jan 5, 2013
 *      Author: steven
 */

#ifndef SSP_H_
#define SSP_H_

#include "type.h"

/*-----------------------------------------------------------------------*/
/*SSP0 Ports/Pins
 * SSEL0 -> PIO0_2
 * MISO0 -> PIO0_8
 * MOSI0 -> PIO0_9
 * SCK0 -> PIO1_29
-------------------------------------------------------------------------*/
#define SSP0_SSELPORT		(0x0)
#define SSP0_MISOPORT		(0x0)
#define SSP0_MOSIPORT		(0x0)
#define SSP0_SCKPPORT		(0x1)

#define SSP0_SSELPIN		(0x02)
#define SSP0_MISOPIN		(0x08)
#define SSP0_MOSIPIN		(0x09)
#define SSP0_SCKPIN			(0x1D)


/* SSP Status register */
#define SSPSR_TFE       (0x1<<0)
#define SSPSR_TNF       (0x1<<1)
#define SSPSR_RNE       (0x1<<2)
#define SSPSR_RFF       (0x1<<3)
#define SSPSR_BSY       (0x1<<4)

void SSP0_Init(void);
void SSP0_Send(uint8_t *buf, uint32_t Length );
void SSP0_Receive(uint8_t *buf, uint32_t Length );

#endif /* SSP_H_ */
