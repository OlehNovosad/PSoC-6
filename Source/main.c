/*
 * In the beginning, the program checks the date and time and prints it in the terminal also reads temperature from thermistor
 * and print it in the terminal.These actions repeat every second and every 30 sec. this information is burned to the SD card
 * during the program running. Temperature is set in Celsius by default but you can change it to Fahrenheit by pushing the
 * button (Pin: 0.4).
 */

#include "cy_device_headers.h"
#include "cycfg.h"
#include "stdio.h"
#include "cy_sd_host.h"
#include "cy_sysint.h"
#include "string.h"
#include "stdbool.h"
#include "Thermistor.h"

#define FAHRENHEIT(temperature)	(9.0/5.0 * temperature + 32.0)

#define SWITCH_INTR_PRIORITY	(3u)
#define SD_Host_INTR_NUM        sdhc_1_interrupt_general_IRQn
#define SD_Host_INTR_PRIORITY   (3UL)

cy_stc_scb_uart_context_t uart_context;
cy_stc_rtc_config_t dateTime;
cy_stc_sd_host_context_t sdHostContext;
cy_en_sd_host_card_type_t cardType;
cy_en_sd_host_card_capacity_t cardCapacity;
cy_stc_sd_host_write_read_config_t data;
cy_en_sd_host_status_t ret;

uint32_t rca;

//value for change temperature_sw
bool temperature_sw = true;

/* Switch interrupt configuration structure */
const cy_stc_sysint_t switch_intr_config = {
	.intrSrc = ioss_interrupts_gpio_0_IRQn,		/* Source of interrupt signal */
	.intrPriority = SWITCH_INTR_PRIORITY		/* Interrupt priority */
};

//SDHC interruption
const cy_stc_sysint_t sdHostIntrConfig =
{
	#if (CY_CPU_CORTEX_M0P)
		  .intrSrc = NvicMux4_IRQn,
		  .cm0pSrc = SD_Host_INTR_NUM,
	#else
		 .intrSrc = SD_Host_INTR_NUM, /* SD Host interrupt number (non M0 core)*/
	#endif
		 .intrPriority = SD_Host_INTR_PRIORITY
};

//SDHC config
const cy_stc_sd_host_init_config_t sdHostConfig =
{
    .dmaType = CY_SD_HOST_DMA_ADMA2,
    .enableLedControl = false,
    .emmc = false,
};

//SD card config
cy_stc_sd_host_sd_card_config_t sdCardConfig =
{
    .lowVoltageSignaling = false,
    .busWidth = CY_SD_HOST_BUS_WIDTH_4_BIT,
    .cardType = &cardType,
    .rca = &rca,
    .cardCapacity = &cardCapacity,
};

//Change pin card detect to pin 13.5
__WEAK bool Cy_SD_Host_IsCardConnected(SDHC_Type const *base)
{
	// pseudocode
	//return (bool)Cy_GPIO_Read(13_5);
	return true;
}

//Function for SW interruption
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

//Function for SDHC interruption
void SD_Host_User_Isr(void)
{
    uint32_t normalStatus;
    uint32_t errorStatus;

    normalStatus = Cy_SD_Host_GetNormalInterruptStatus(SDHC_HW);
    /* Check the Error event */
    if (0u < normalStatus)
    {
        /* Clear the normalStatus event */
        Cy_SD_Host_ClearNormalInterruptStatus(SDHC_HW, normalStatus);
    }

    errorStatus = Cy_SD_Host_GetErrorInterruptStatus(SDHC_HW);
    /* Check the Error event */
    if (0u < errorStatus)
    {
        /* Clear the Error event */
        Cy_SD_Host_ClearErrorInterruptStatus(SDHC_HW, errorStatus);
    }

    /* Add the use code here. */
}

void PrintDateAndTime();

char * ThermistorInfo(char * terminfo);

bool SDHCRW(char * dateandtime);

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
    	PrintDateAndTime();
    }
}

void PrintDateAndTime(){
	Cy_GPIO_Write(Pin_Led_PORT,Pin_Led_NUM,1);
	char dateandtime[CY_SD_HOST_BLOCK_SIZE];
	char terminfo[CY_SD_HOST_BLOCK_SIZE];

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

		sprintf(dateandtime, "<%2u-%2u-%2u %2u:%2u:%2u> ", (uint16_t)dateTime.date, (uint16_t)dateTime.month, (uint16_t)dateTime.year,\
				(uint16_t)dateTime.hour, (uint16_t)dateTime.min, (uint16_t)dateTime.sec);

		/*Print Thermistor info.*/
		ThermistorInfo(terminfo);

		strncat(dateandtime, terminfo, 20);

		if(secPrev == 0 || secPrev == 30){
			if(SDHCRW(dateandtime)){
				Cy_GPIO_Write(Pin_Led_PORT,Pin_Led_NUM,0);
				Cy_SysLib_Delay(500);
			}
		}
	}
}

char * ThermistorInfo(char * terminfo){
	int16_t countThermistor, countReference;

	countReference = Cy_SAR_GetResult16(SAR, 0);
	countThermistor = Cy_SAR_GetResult16(SAR, 1);

	uint32 resT = Thermistor_GetResistance(countReference, countThermistor);
	float temperature = (float)Thermistor_GetTemperature(resT) / 100.0;

	printf("<T = %.3f%c>\r\n", temperature_sw == true ? (temperature) : FAHRENHEIT(temperature),
			temperature_sw == true ? 'C' : 'F');

	sprintf(terminfo, "<T = %.3f%c>\r\n", temperature_sw == true ? (temperature) : FAHRENHEIT(temperature),
			temperature_sw == true ? 'C' : 'F');

	return terminfo;
}

bool SDHCRW(char * dateandtime){
	Cy_SysInt_Init(&sdHostIntrConfig, &SD_Host_User_Isr);
	NVIC_EnableIRQ(SD_Host_INTR_NUM);

	Cy_SD_Host_Enable(SDHC_HW);
	ret = Cy_SD_Host_Init(SDHC_HW, &sdHostConfig, &sdHostContext);

	if (CY_SD_HOST_SUCCESS == ret)
		ret = Cy_SD_Host_InitCard(SDHC1, &sdCardConfig, &sdHostContext);

	char_t rxBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to read. */

	data.address = 0UL;         /* The address to write/read data on the card or eMMC. */
	data.numberOfBlocks = 1UL;  /* The number of blocks to write/read (Single block write/read). */
	data.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
	data.dataTimeout = 12UL;     /* The timeout value for the transfer. */
	data.enReliableWrite = false; /* For EMMC cards enable reliable write. */
	data.enableDma = true;  /* Enable DMA mode. */

	data.data = (uint32_t*)dateandtime;  /* The pointer to data to write. */
	ret = Cy_SD_Host_Write(SDHC1, &data, &sdHostContext);  /* Write data to the card. */

	if (CY_SD_HOST_SUCCESS == ret)
	{
		while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		{
			/* Wait for the data-transaction complete event. */
		}
		Cy_SD_Host_ClearNormalInterruptStatus(SDHC1, CY_SD_HOST_XFER_COMPLETE);
	} else {
		return false;
	}

	data.data = (uint32_t*)rxBuff;  /* The pointer to data to read. */
	ret = Cy_SD_Host_Read(SDHC1, &data, &sdHostContext);   /* Read data from the card. */

	if (CY_SD_HOST_SUCCESS == ret)
	{
		while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		{
			/* Wait for the data-transaction complete event. */
		}
		/* Clear the data-transaction complete event. */
		Cy_SD_Host_ClearNormalInterruptStatus(SDHC1, CY_SD_HOST_XFER_COMPLETE);
	} else {
		return false;
	}

	printf("Info is written:\n\r");
	puts(rxBuff);
	return true;
}

