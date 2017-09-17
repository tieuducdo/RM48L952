/** @file sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "gio.h"
#include "het.h"
#include "sci.h"
#include "spi.h"
#include "adc.h"
#include "stdlib.h"
#include "pinmux.h"
#include "PCBsignals.h"
unsigned char command[8];
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

	  adcData_t adc_data; //ADC Data Structure
	  adcData_t *adc_data_ptr = &adc_data; //ADC Data Pointer
	  unsigned int NumberOfChars, value, val; //Declare variables

	   /** bring SPI2 out of reset */
	    spiREG2->GCR0 = 0U;
	    spiREG2->GCR0 = 1U;
	    hetREG1->GCR = 0U;
	    hetREG1->GCR = 1U;

//	  spiInit();	// use spiREG2 7F600 - 7F7FF
	  muxInit();
	  sciInit(); //Initializes the SCI (UART) module
	  adcInit(); //Initializes the ADC module
	  gioSetDirection(gioPORTB, 0x0);
	  gioSetDirection(hetPORT1, (uint32)((uint32)1U << PATIENT_AIR_B4) | ((uint32)1U << PATIENT_WATER_N1) | ((uint32)1U << DIODE_FAN_V6) \
			  | ((uint32)1U << Aiming_Beam_W5)| ((uint32)1U << Illumination_K18));

	  while(1)  // Loop to acquire and send ADC sample data via the SCI (UART)
	  {
			val =  gioGetPort( hetPORT1);
			gioToggleBit(hetPORT1,Illumination_K18);
			gioToggleBit(hetPORT1,Aiming_Beam_W5);
		//	val =  gioGetPort( gioPORTB);
	    adcStartConversion(adcREG1, 1U);  //Start ADC conversion
	    while(!adcIsConversionComplete(adcREG1, 1U)); //Wait for ADC conversion
	    adcGetData(adcREG1, 1U, adc_data_ptr); //Store conversion into ADC pointer
	    value = (unsigned int)adc_data_ptr->value;
	    NumberOfChars = ltoa(value,(char *)command);
	    sciSend(sciREG, 2, (unsigned char *)"0x"); //Sends '0x' hex designation chars
	    sciSend(sciREG, NumberOfChars, command); //Sends the ambient light sensor data
	    sciSend(sciREG, 2, (unsigned char *)"\r\n"); //Sends new line character
	    sciSend(scilinREG, 2, (unsigned char *)"0x"); //Sends '0x' hex designation chars
	    sciSend(scilinREG, NumberOfChars, command); //Sends the ambient light sensor data
	    sciSend(scilinREG, 2, (unsigned char *)"\r\n"); //Sends new line character
	  }

/* USER CODE END */

    //return 0;
}


/* USER CODE BEGIN (4) */
//void gioNotification(int bit)
//{
//	return;
//}
void esmGroup1Notification(int bit)
{
	return;
}
void esmGroup2Notification(int bit)
{
	return;
}
/* USER CODE END */
