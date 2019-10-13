/***************************************************************************//**
* \file main.c
* \version 1.0
*
* \brief
* Minimal new application template for 150MHz PSoC 6 devices. Debug is enabled 
* and platform clocks are set for high performance (144MHz CLK_FAST for CM4 
* core and 72MHz CLK_SLOW for CM0+) but with a peripheral-friendly CLK_PERI 
* frequency (72MHz).
*
********************************************************************************
* \copyright
* Copyright 2017-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cy_device_headers.h"
#include "cycfg.h"
#include "cy_sysint.h"
#include "stdio_user.h"
#include <stdio.h>
#include "Thermistor.h"

cy_stc_scb_uart_context_t uart_context;
cy_stc_rtc_config_t dateTime;

//value for change temperature_sw
bool temperature_sw = true;

#define SWITCH_INTR_PRIORITY	(3u)

/* Switch interrupt configuration structure */
const cy_stc_sysint_t switch_intr_config = {
	.intrSrc = ioss_interrupts_gpio_0_IRQn,		/* Source of interrupt signal */
	.intrPriority = SWITCH_INTR_PRIORITY		/* Interrupt priority */
};

void Isr_switch(void)
{
	if(temperature_sw == true){
		temperature_sw = false;
	}
	else {
		temperature_sw = true;
	}

	/* Clears the triggered pin interrupt */
	Cy_GPIO_ClearInterrupt(PIN_SW_PORT, PIN_SW_NUM);
	NVIC_ClearPendingIRQ(switch_intr_config.intrSrc);
}

void PrintDateAndTime();

void ThermistorInfo();

int main(void)
{
    /* Set up the device based on configurator selections */
    init_cycfg_all();

    __enable_irq();

    //Set UART
    Cy_SCB_UART_Init(Bridge_UART_HW, &Bridge_UART_config, &uart_context);
	Cy_SCB_UART_Enable(Bridge_UART_HW);

	//Set RTC
	Cy_RTC_Init(&RTC_config);

	//Set System Analog Reference Block
	cy_en_sysanalog_status_t status_aref;
	status_aref = Cy_SysAnalog_Init(&Cy_SysAnalog_Fast_Local);
	Cy_SysAnalog_Enable();

	//Set SAR ADC
	Cy_SAR_Init(SAR_HW, &SAR_config);
	Cy_SAR_Enable(SAR_HW);
	Cy_SAR_StartConvert(SAR_HW, CY_SAR_START_CONVERT_CONTINUOUS);

	//Set button interrupt
	Cy_SysInt_Init(&switch_intr_config, Isr_switch);
	NVIC_ClearPendingIRQ(switch_intr_config.intrSrc);
	NVIC_EnableIRQ(switch_intr_config.intrSrc);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
	printf("\x1b[2J\x1b[;H");

    for(;;)
    {
    	Cy_GPIO_Write(Pin_Led_PORT,Pin_Led_NUM,1);
    	PrintDateAndTime();
    }
}

void PrintDateAndTime(){
	static bool access;

	/* Variable used to store previous second value */
	static uint32_t secPrev = CY_RTC_MAX_SEC_OR_MIN + 1;

	/* Get current date and time */
	Cy_RTC_GetDateAndTime(&dateTime);

	if(dateTime.sec != secPrev)
	{
		secPrev = dateTime.sec;
		printf("<%2u-%2u-%2u %2u:%2u:%2u> ",	\
				(uint16_t)dateTime.date, (uint16_t)dateTime.month, (uint16_t)dateTime.year,\
					(uint16_t)dateTime.hour, (uint16_t)dateTime.min, (uint16_t)dateTime.sec);

		/*Print Thermistor info.*/
		ThermistorInfo();

		if(secPrev == 0 || secPrev == 30)
			access = true;
	}
	if(access){
		Cy_GPIO_Write(Pin_Led_PORT,Pin_Led_NUM,0);
		printf("Here will be SD writing. Soon:D\r\n");
		access = false;
		CyDelay(500);
	}
}

void ThermistorInfo(){
	int16_t countThermistor, countReference;

	countReference = Cy_SAR_GetResult16(SAR, 0);
	countThermistor = Cy_SAR_GetResult16(SAR, 1);

	uint32 resT = Thermistor_GetResistance(countReference, countThermistor);
	float temperature = (float)Thermistor_GetTemperature(resT) / 100.0;

	printf("<T = %.3f%c>\r\n", temperature_sw == true ? (temperature) : (9.0/5.0 * temperature + 32.0),
			temperature_sw == true ? 'C' : 'F');
}
