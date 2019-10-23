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
		  .intrSrc = NvicMux4_IRQn,				/* Source of interrupt signal */
		  .cm0pSrc = SD_Host_INTR_NUM,			/* Interrupt priority */
	#else
		 .intrSrc = SD_Host_INTR_NUM, 			/* SD Host interrupt number (non M0 core)*/
	#endif
		 .intrPriority = SD_Host_INTR_PRIORITY	/* SD Host interrupt priority (non M0 core)*/
};

//SDHC configuration
const cy_stc_sd_host_init_config_t sdHostConfig =
{
    .dmaType = CY_SD_HOST_DMA_ADMA2,
    .enableLedControl = false,
    .emmc = false,
};

//SD card configuration
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

/*******************************************************************************
* Function Name: Isr_switch
********************************************************************************
*
* Summary:
*  This function is used for SW interruption handling.
*
*******************************************************************************/

void Isr_switch()
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

/*******************************************************************************
* Function Name: SD_Host_User_Isr
********************************************************************************
*
* Summary:
*  This function is used for SDHC interruption handling.
*
*******************************************************************************/

void SD_Host_User_Isr()
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

void PrintDateAndTime(int frequency);

char * ThermistorInfo(char * terminfo);

bool SDHC_Read_Write(char * dateandtime);

int main(void)
{
    /* Set up the device based on configurator selections */
    init_cycfg_all();

    __enable_irq();

    // Variable which use for changing frequency of recording.
    char ch;
    // Variable recording frequency
    int frequency = 30;

    //Set up UART
    Cy_SCB_UART_Init(Bridge_UART_HW, &Bridge_UART_config, &uart_context);
	Cy_SCB_UART_Enable(Bridge_UART_HW);

	//Set up RTC
	Cy_RTC_Init(&RTC_config);

	//Set yp System Analog Reference Block
	cy_en_sysanalog_status_t status_aref;
	status_aref = Cy_SysAnalog_Init(&Cy_SysAnalog_Fast_Local);
	Cy_SysAnalog_Enable();

	//Set up SAR ADC
	Cy_SAR_Init(SAR_HW, &SAR_config);
	Cy_SAR_Enable(SAR_HW);
	Cy_SAR_StartConvert(SAR_HW, CY_SAR_START_CONVERT_CONTINUOUS);

	//Set up button interrupt
	Cy_SysInt_Init(&switch_intr_config, Isr_switch);
	NVIC_ClearPendingIRQ(switch_intr_config.intrSrc);
	NVIC_EnableIRQ(switch_intr_config.intrSrc);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
	printf("\x1b[2J\x1b[;H");

    for(;;)
    {
    	//Getting value via UART.
    	ch = Cy_SCB_UART_Get(Bridge_UART_HW);

    	if(ch == 'c'){
    		printf("With which frequency you want to record the info:\r\n");
    		fscanf(stdin, "%d", &frequency);
    	} else {
    		//Checking of admissibility of "frequency" value.
    		if(frequency < 1 || frequency > 59){
				printf("Please choose between <1-59>\r\n");
				fscanf(stdin, "%d", &frequency);
			} else{
				PrintDateAndTime(frequency);
			}
		}
    }
}

/*******************************************************************************
* Function Name: PrintDateAndTime
********************************************************************************
*
* Summary:
*  This function is getting value of date and time and print out it.
*
* Parameters:
*  frequency: Frequency of recording information to SD card.
*
*******************************************************************************/

void PrintDateAndTime(int frequency){
	//Turn off the LED.
	Cy_GPIO_Write(Pin_Led_PORT,Pin_Led_NUM,1);

	//Arrays for the information.
	char dateandtime[CY_SD_HOST_BLOCK_SIZE];
	char terminfo[CY_SD_HOST_BLOCK_SIZE];

	/* Variable used to store previous second value */
	static uint32_t secPrev = CY_RTC_MAX_SEC_OR_MIN + 1;

	/* Get current date and time */
	Cy_RTC_GetDateAndTime(&dateTime);

	if(dateTime.sec != secPrev)
	{
		// Remembering the previous second.
		secPrev = dateTime.sec;
		// Print out the date and time.
		printf("<%2u-%2u-%2u %2u:%2u:%2u> ",	\
				(uint16_t)dateTime.date, (uint16_t)dateTime.month, (uint16_t)dateTime.year,\
					(uint16_t)dateTime.hour, (uint16_t)dateTime.min, (uint16_t)dateTime.sec);
		// Recording the date and time into char array.
		sprintf(dateandtime, "<%2u-%2u-%2u %2u:%2u:%2u> ", (uint16_t)dateTime.date, (uint16_t)dateTime.month, (uint16_t)dateTime.year,\
				(uint16_t)dateTime.hour, (uint16_t)dateTime.min, (uint16_t)dateTime.sec);

		/*Print out Thermistor info.*/
		ThermistorInfo(terminfo);

		//Recording the information from thermistor into char array.
		strncat(dateandtime, terminfo, 20);

		// If modulo division is equal to zero, then information is burning to sd card.
		if(secPrev % frequency == 0){
			if(SDHC_Read_Write(dateandtime)){
				Cy_GPIO_Write(Pin_Led_PORT,Pin_Led_NUM,0);
				Cy_SysLib_Delay(500);
			}
		}
	}
}

/*******************************************************************************
* Function Name: ThermistorInfo
********************************************************************************
*
* Summary:
*  This function is getting values of voltages from ACD and count temperature
*  via thermistor and print out it.
*
* Parameters:
*  terminfo: char array for recording information.
*
* Return:
*  Char array with temperature information.
*
*******************************************************************************/

char * ThermistorInfo(char * terminfo){
	//Variables for voltages.
	float v1,v2;

	int16_t countThermistor, countReference;

	//Getting values from channels.
	countReference = Cy_SAR_GetResult16(SAR, 0);
	countThermistor = Cy_SAR_GetResult16(SAR, 1);

	//Getting values of voltages.
	v1 = Cy_SAR_CountsTo_Volts(SAR_HW, 0, countReference);
	v2 = Cy_SAR_CountsTo_Volts(SAR_HW, 1, countThermistor);

	//Getting the resistance across the thermistor.
	uint32 resT = Thermistor_GetResistance(countReference, countThermistor);

	//Counting temperature in Celsius.
	float temperature = (float)Thermistor_GetTemperature(resT) / 100.0;

	//Print out the voltages and temperature.
	printf("<v1 = %.3f v2 = %.3f T = %.3f%c>\r\n", v1, v2, temperature_sw == true ? (temperature) : FAHRENHEIT(temperature),
			temperature_sw == true ? 'C' : 'F');

	// Recording the temperature into char array.
	sprintf(terminfo, "<T = %.3f%c>\r\n", temperature_sw == true ? (temperature) : FAHRENHEIT(temperature),
			temperature_sw == true ? 'C' : 'F');

	return terminfo;
}

/*******************************************************************************
* Function Name: SDHC_Read_Write
********************************************************************************
*
* Summary:
*  This function is used for initialization of SD Host. Also this function is
*  written information into SD card and read information from there.
*
* Parameters:
*  dateandtime: char array which contains the date, time and temperature information.
*
* Return:
*  true if reading was success and false if recording or reading was unsuccessful.
*
*******************************************************************************/

bool SDHC_Read_Write(char * dateandtime){
	//Initialization of SDHC interruption
	Cy_SysInt_Init(&sdHostIntrConfig, &SD_Host_User_Isr);
	NVIC_EnableIRQ(SD_Host_INTR_NUM);

	//Enable SDHC
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
		fprintf(stderr, "Cannot write information.\r\n");
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
		fprintf(stderr, "Cannot read information.\r\n");
		return false;
	}

	printf("Info is written:\n\r");
	puts(rxBuff);
	return true;
}

