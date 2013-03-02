/*
 * delay.c
 *
 *  Created on: Jan 7, 2013
 *      Author: steven
 */

#include "delay.h"

/* Delay n microseconds */
void dly_us(int n) {
	int i, delay;
	i = 0;
	delay = ONE_US*n;
	while(i<delay) {i++;}
}

