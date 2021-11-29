/* 
 * This file is part of the EMTSE Project (https://gitlab.com/emtse/emtse).
 * Copyright (c) 2018-2021 Renzo Grover Fabián Espinoza (renzo.fe@ieee.org).
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * This is the foreign model of EMTP-ATP in charge of synchronization
 * that allows real-time simulation
 *
 */


#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

#include<stdio.h>
#include<complex.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/io.h>

#include<inttypes.h>
#include<math.h>

#define NSEC_PER_SEC 1000000000ULL

#define timestamp xin_ar[0]
#define stoptime = xdata_ar[0]
#define step = xdata_ar[1]

long int time_diff(struct timespec *stop, struct timespec *start)
{
  return (stop->tv_nsec-start->tv_nsec)+(stop->tv_sec-start->tv_sec)*NSEC_PER_SEC;
}

double total_time;

long int diff;

static struct timespec tt1, tt2, tstart;


void emtse_sync_m_(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{
	if(timestamp < step){
		usleep(2000);
		clock_gettime(CLOCK_MONOTONIC, &tt1);
	}

	if(timestamp > step && timestamp <= stoptime) {

		clock_gettime(CLOCK_MONOTONIC, &tt2);
		diff=(timestamp-tt2.tv_sec+tt1.tv_sec)*NSEC_PER_SEC-tt2.tv_nsec+tt1.tv_nsec;

	}

	do{ //Waiting
		clock_gettime(CLOCK_MONOTONIC, &tt2);
		diff=(timestamp-tt2.tv_sec+tt1.tv_sec)*NSEC_PER_SEC-tt2.tv_nsec+tt1.tv_nsec;
	}
	while(diff > 0);

	
	if(stoptime-timestamp < step) { // Last simulation step
		total_time=(tt2.tv_sec-tt1.tv_sec)*NSEC_PER_SEC+tt2.tv_nsec-tt1.tv_nsec;
		printf("----------------------------------------------\n");
		printf("EMTSE SIMULATION >> TOTAL REAL TIME (mS): %.4f\n", total_time*0.000001);
		printf("EMTSE SIMULATION >> TOTAL SIMU TIME (mS): %.4f\n", timestamp*1000);
		printf("----------------------------------------------\n");
	}
	
	return;
}

void emtse_sync_i_(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{

	printf("-----------------------------------------------------------\n");
	printf("EMTSE SIMULATION >> STARTING RT SYNCHRONIZATION COMPONENT \n");
	printf("-----------------------------------------------------------\n");

	usleep(2000);
	return;
}

