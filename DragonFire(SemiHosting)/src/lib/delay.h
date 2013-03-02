/*
 * delay.h
 *
 *  Created on: Jan 7, 2013
 *      Author: steven
 *
 *      SUPER-GHETTO DELAY FUNCTION!!!
 *      VERY INACCURATE
 *      BASED OFF THIS CODE AND A LOGIC ANALYZER
 *      CHANGE WHEN YOU HAVE THE CHANCE!!!!
#define OFF		0
#define ON		1
int main(void) {
	int ledStatus;
	ledStatus = OFF;
	GPIOSetDir(0,1, OUTPUT);
	while(1) {
		int i;
		while(i<26) {i++;}
		if(ledStatus==OFF) {
			GPIOSetBitValue(0,1,ON);
			ledStatus = ON;
		}
		else {
			GPIOSetBitValue(0,1,OFF);
			ledStatus = OFF;
		}
		i=0;
	}
	return 0 ;
}
 *
 */

#ifndef DELAY_H_
#define DELAY_H_

//1 us = 26 for i according to my calculations (SUPER INACCURATE!!!)
#define ONE_US		(26)

void dly_us(int n);		/* Delay n microseconds */

#endif /* DELAY_H_ */
