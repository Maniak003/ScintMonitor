/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "AD5245.h"
#include "w5500.h"
#include "socket.h"
#include "dhcp.h"
#include <stdarg.h>
//#include "webpage.h"
//#include "httpServer.h"
//#include <wizchip_conf.h>
#include "networkint.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define SPECTER_SIZE 1024
#define COMPARATOR_LEVEL 120
#define HV_LEVEL 100
#define AMP_DEFAULT 255
#define DELAYTIME1 1000

/* For W5500*/
/*
#define ZABBIX_ENABLE
#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define TCP_SOCKET		2
#define _DHCP_DEBUG_
#define ZABBIX_DEBUG
#define DATA_BUF_SIZE   16384
#define ZABBIX_ENABLE
#define ZABBIXAGHOST	"SCINTMONITOR"  // Default hostname.
#define ZABBIXPORT		10051
#define ZABBIXMAXLEN	128
#define MAC_ADDRESS		0x00, 0x11, 0x22, 0x33, 0x45, 0xF1
#define MAX_HTTPSOCK	4
#define UARTN			&huart1
#define SPIN			&hspi2
*/
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern uint32_t pulseLevel[1];
extern uint32_t pulseCounter;
extern uint32_t specterBuffer[SPECTER_SIZE];
extern bool reqToSpecter;
extern uint8_t ampLevel;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SOUND_Pin GPIO_PIN_13
#define SOUND_GPIO_Port GPIOC
#define AnalogIN_Pin GPIO_PIN_3
#define AnalogIN_GPIO_Port GPIOA
#define AdjHV_Pin GPIO_PIN_4
#define AdjHV_GPIO_Port GPIOA
#define AdjCOMP_Pin GPIO_PIN_6
#define AdjCOMP_GPIO_Port GPIOA
#define W5500_CS_Pin GPIO_PIN_11
#define W5500_CS_GPIO_Port GPIOB
#define W5500_SCK_Pin GPIO_PIN_13
#define W5500_SCK_GPIO_Port GPIOB
#define W5500_MISO_Pin GPIO_PIN_14
#define W5500_MISO_GPIO_Port GPIOB
#define W5500_MOSI_Pin GPIO_PIN_15
#define W5500_MOSI_GPIO_Port GPIOB
#define W5500_RST_Pin GPIO_PIN_10
#define W5500_RST_GPIO_Port GPIOA
#define Pulse_Pin GPIO_PIN_11
#define Pulse_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOA
#define HVEnable_Pin GPIO_PIN_3
#define HVEnable_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define W5500_INIT_TEXT "W5500 init.."
#define INIT_OK "Ok\n\r"

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
