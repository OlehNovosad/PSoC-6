/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
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
********************************************************************************/

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_sysanalog.h"
#include "cy_sar.h"
#include "cy_sysclk.h"
#include "cy_scb_uart.h"
#include "cy_sd_host.h"
#include "cy_rtc.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define SAR_HW SAR
#define SAR_IRQ pass_interrupt_sar_IRQn
#define SAR_CTRL (CY_SAR_VREF_PWR_100 | CY_SAR_VREF_SEL_BGR | CY_SAR_BYPASS_CAP_ENABLE | CY_SAR_NEG_SEL_VREF | CY_SAR_CTRL_NEGVREF_HW | CY_SAR_CTRL_COMP_DLY_12 | CY_SAR_COMP_PWR_100 | CY_SAR_DEEPSLEEP_SARMUX_OFF | CY_SAR_SARSEQ_SWITCH_ENABLE)
#define SAR_SAMPLE (SAR_SAMPLE_CTRL_EOS_DSI_OUT_EN_Msk | CY_SAR_RIGHT_ALIGN | CY_SAR_DIFFERENTIAL_SIGNED | CY_SAR_SINGLE_ENDED_UNSIGNED | CY_SAR_AVG_CNT_256 | CY_SAR_AVG_MODE_SEQUENTIAL_FIXED | CY_SAR_TRIGGER_MODE_FW_ONLY)
#define SAR_CH0_CONFIG (CY_SAR_POS_PORT_ADDR_SARMUX | CY_SAR_CHAN_POS_PIN_ADDR_0 | CY_SAR_NEG_PORT_ADDR_SARMUX | CY_SAR_CHAN_NEG_PIN_ADDR_1 | CY_SAR_CHAN_DIFFERENTIAL_UNPAIRED | CY_SAR_CHAN_AVG_ENABLE | CY_SAR_CHAN_SAMPLE_TIME_0)
#define SAR_CH1_CONFIG (CY_SAR_POS_PORT_ADDR_SARMUX | CY_SAR_CHAN_POS_PIN_ADDR_2 | CY_SAR_NEG_PORT_ADDR_SARMUX | CY_SAR_CHAN_NEG_PIN_ADDR_3 | CY_SAR_CHAN_DIFFERENTIAL_UNPAIRED | CY_SAR_CHAN_AVG_ENABLE | CY_SAR_CHAN_SAMPLE_TIME_1)
#define SAR_MUX_SWITCH (0u | CY_SAR_MUX_FW_P0_VPLUS | CY_SAR_MUX_FW_P2_VPLUS | CY_SAR_MUX_FW_P1_VMINUS | CY_SAR_MUX_FW_P3_VMINUS)
#define SAR_MUX_SWITCH_HW_CTRL (0u | CY_SAR_MUX_SQ_CTRL_P0 | CY_SAR_MUX_SQ_CTRL_P1 | CY_SAR_MUX_SQ_CTRL_P2 | CY_SAR_MUX_SQ_CTRL_P3)
#define SAR_VREF_MV 1200UL
#define Bridge_UART_HW SCB5
#define Bridge_UART_IRQ scb_5_interrupt_IRQn
#define SDHC_HW SDHC1
#define SDHC_IRQ sdhc_1_interrupt_general_IRQn
#define RTC_10_MONTH_OFFSET (28U)
#define RTC_MONTH_OFFSET (24U)
#define RTC_10_DAY_OFFSET (20U)
#define RTC_DAY_OFFSET (16U)
#define RTC_1000_YEAR_OFFSET (12U)
#define RTC_100_YEAR_OFFSET (8U)
#define RTC_10_YEAR_OFFSET (4U)
#define RTC_YEAR_OFFSET (0U)

extern const cy_stc_sysanalog_config_t AREF_config;
extern const cy_stc_sar_config_t SAR_config;
extern const cy_stc_scb_uart_config_t Bridge_UART_config;
extern cy_en_sd_host_card_capacity_t SDHC_cardCapacity;
extern cy_en_sd_host_card_type_t SDHC_cardType;
extern uint32_t SDHC_rca;
extern const cy_stc_sd_host_init_config_t SDHC_config;
extern cy_stc_sd_host_sd_card_config_t SDHC_card_cfg;
extern const cy_stc_rtc_config_t RTC_config;

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
