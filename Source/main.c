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
#include "stdio.h"
#include "Thermistor.h"

cy_stc_scb_uart_context_t uart_context;

void PrintDateAndTime();

void ThermistorInfo();

int main(void)
{
    /* Set up the device based on configurator selections */
    init_cycfg_all();

    Cy_SCB_UART_Init(Bridge_UART_HW, &Bridge_UART_config, &uart_context);
    Cy_SCB_UART_Enable(Bridge_UART_HW);

    Cy_RTC_Init(&RTC_config);

    Cy_SAR_Init(SAR, &SAR_config);
    Cy_SAR_Enable(SAR);
    Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_CONTINUOUS);

    __enable_irq();

    for(;;)
    {
    	PrintDateAndTime();
    	ThermistorInfo();
    	Cy_SysLib_Delay(1000);
    }
}

void PrintDateAndTime(){
	cy_stc_rtc_config_t dateTime;

	Cy_RTC_GetDateAndTime(&dateTime);

	printf("<%u-%u-%u %u:%u:%u> ",(uint16_t)dateTime.date, (uint16_t)dateTime.month, (uint16_t)dateTime.year,
			(uint16_t)dateTime.hour,(uint16_t)dateTime.min,(uint16_t)dateTime.sec);
}

void ThermistorInfo(){
	int16_t countThermistor, countReference;

	countReference = Cy_SAR_GetResult16(SAR, 0);
	countThermistor = Cy_SAR_GetResult16(SAR, 1);

	uint32 resT = Thermistor_GetResistance(countReference, countThermistor);
	float temperature = (float)Thermistor_GetTemperature(resT) / 100.0;

	printf("<T = %.3fC T = %.3fF>\r\n", temperature, 9.0/5.0 * temperature + 32.0);
}