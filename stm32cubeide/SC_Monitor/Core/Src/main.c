/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

DAC_HandleTypeDef hdac1;
DAC_HandleTypeDef hdac2;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
uint16_t HVLevel = HV_LEVEL, COMPLevel = COMPARATOR_LEVEL;
char uartBuffer[400] = {0,};
uint32_t specterBuffer[SPECTER_SIZE] = {0,};
uint32_t pulseCounter = 0, measurementTime = 0;
uint8_t ctrlChar[1] = {0,}, ampLevel = AMP_DEFAULT;
uint32_t pulseLevel[1];
bool reqToSpecter = false;
char text1306[11];
char ZabbixHostName[255];

#ifdef ZABBIX_ENABLE
wiz_NetInfo net_info = {
	.mac  = { MAC_ADDRESS },
	.dhcp = NETINFO_DHCP
};
//uint8_t ntp_server[4] = {192, 168, 1, 1};
uint8_t gDATABUF[DATA_BUF_SIZE];
//datetime timeNTP;
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC1_Init(void);
static void MX_DAC2_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

#ifdef ZABBIX_ENABLE

#ifdef ZABBIX_DEBUG
void UART_Printf(const char* fmt, ...) {
	char buff[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buff, sizeof(buff), fmt, args);
	HAL_UART_Transmit(&huart1, (uint8_t*) buff, strlen(buff), HAL_MAX_DELAY);
	va_end(args);
}
#endif

void W5500_Select(void) {
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
	HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
	HAL_SPI_Receive(&hspi2, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
	HAL_SPI_Transmit(&hspi2, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void) {
	uint8_t byte;
	W5500_ReadBuff(&byte, sizeof(byte));
	return byte;
}

void W5500_WriteByte(uint8_t byte) {
	W5500_WriteBuff(&byte, sizeof(byte));
}

volatile bool ip_assigned = false;

void Callback_IPAssigned(void) {
#ifdef ZABBIX_DEBUG
	UART_Printf("Callback: IP assigned! Leased time: %d sec\r\n", getDHCPLeasetime());
#endif
    ip_assigned = true;
}

void Callback_IPConflict(void) {
#ifdef ZABBIX_DEBUG
    UART_Printf("Callback: IP conflict!\r\n");
#endif
}

// 1K should be enough, see https://forum.wiznet.io/t/topic/1612/2
uint8_t dhcp_buffer[1024];
// 1K seems to be enough for this buffer as well
//uint8_t dns_buffer[1024];

/*
 * addr -- IP address zabbix server, dhcp option 224.
 * Settings for isc-dhcp-server:
 * 		option zabbix-server-ip code 224 = ip-address ;
 * 		option zabix-host-name code 225 = string ;
 * 		host anemometr {
 *      	hardware ethernet 00:11:22:33:44:xx;
 *      	fixed-address 192.168.1.24;
 *       	option zabbix-server-ip 192.168.1.6;
 *       	option zabix-host-name "Ed";
 * 		}
 *
 * key -- Zabbix key: {ALTIM_DIRECT, ALTIM_SPEED}
 *
 * value -- Float value of key.
*/
uint8_t sendToZabbix(uint8_t * addr, char * host, char * key, float value) {
#ifdef ZABBIX_DEBUG
    UART_Printf("Creating socket...\r\n");
#endif
    uint8_t tcp_socket = TCP_SOCKET;
    uint8_t code = socket(tcp_socket, Sn_MR_TCP, 10888, 0);
    if(code != tcp_socket) {
	#ifdef ZABBIX_DEBUG
        UART_Printf("socket() failed, code = %d\r\n", code);
	#endif
        return(-1);
    }
#ifdef ZABBIX_DEBUG
    UART_Printf("Socket created, connecting...\r\n");
#endif
    code = connect(tcp_socket, addr, ZABBIXPORT);
    if(code != SOCK_OK) {
	#ifdef ZABBIX_DEBUG
        UART_Printf("connect() failed, code = %d\r\n", code);
	#endif
        close(tcp_socket);
        return(-2);
    }
#ifdef ZABBIX_DEBUG
    UART_Printf("Connected, sending ZABBIX request...\r\n");
#endif
    {
    	char req[ZABBIXMAXLEN];
    	char str[ZABBIXMAXLEN - 13];
    	sprintf(str, "{\"request\":\"sender data\",\"data\":[{\"host\":\"%.20s\",\"key\":\"%s\",\"value\":\"%f\"}]}", host, key, value);
    	req[0] = 'Z';
    	req[1] = 'B';
		req[2] = 'X';
		req[3] = 'D';
		req[4] = 0x01;
		req[5] = strlen(str);
		req[6] = 0;
		req[7] = 0;
		req[8] = 0;
		req[9] = 0;
		req[10] = 0;
		req[11] = 0;
		req[12] = 0;
		strcpy(req + 13, str);
        //char req[] = "ZBXD\1\0\0\0\0\0\0\0\0{\"request\":\"sender data\",\"data\":[{\"host\":\"Ed\",\"key\":\"ALTIM_DIRECT\",\"value\":\"10\"}]}";
        uint8_t len = req[5] + 13;
        uint8_t* buff = (uint8_t*)&req;
        while(len > 0) {
		#ifdef ZABBIX_DEBUG
            UART_Printf("Sending %d bytes, data length %d bytes...\r\n", len, req[5]);
		#endif
            int32_t nbytes = send(tcp_socket, buff, len);
            if(nbytes <= 0) {
			#ifdef ZABBIX_DEBUG
                UART_Printf("send() failed, %d returned\r\n", nbytes);
			#endif
                close(tcp_socket);
                return(-3);
            }
			#ifdef ZABBIX_DEBUG
            UART_Printf("%d b sent!\r\n", nbytes);
			#endif
            len -= nbytes;
        }
    }
    /* Read data from Zabbix */
	#ifdef ZABBIX_DEBUG
		UART_Printf("Read.\r\n");
		{
			char buff[32];
			for(;;) {
				int32_t nbytes = recv(tcp_socket, (uint8_t*)&buff, sizeof(buff)-1);
				if(nbytes == SOCKERR_SOCKSTATUS) {
					UART_Printf("\r\nDisconnect.\r\n");
					break;
				}

				if(nbytes <= 0) {
					UART_Printf("\r\nrecv() failed, %d\r\n", nbytes);
					break;
				}

				buff[nbytes] = '\0';
				UART_Printf("%s", buff);
			}
		}

		UART_Printf("Closing socket.\r\n");
	#endif
    close(tcp_socket);
    return(0);
}



void init_w5500() {
	#ifdef ZABBIX_DEBUG
    UART_Printf("\r\ninit() called!\r\n");
    UART_Printf("W5500 callbacks...\r\n");
	#endif
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
	#ifdef ZABBIX_DEBUG
    UART_Printf("wizchip_init()...\r\n");
	#endif
    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	#ifdef ZABBIX_DEBUG
    UART_Printf("DHCP_init()...\r\n");
	#endif

    // set MAC address before using DHCP
    setSHAR(net_info.mac);
    DHCP_init(DHCP_SOCKET, dhcp_buffer);
	#ifdef ZABBIX_DEBUG
    UART_Printf("DHCP callbacks...\r\n");
	#endif
    reg_dhcp_cbfunc(Callback_IPAssigned, Callback_IPAssigned, Callback_IPConflict);
	#ifdef ZABBIX_DEBUG
    UART_Printf("DHCP_run()...\r\n");
	#endif
    // actually should be called in a loop, e.g. by timer
    uint32_t ctr = 10000;
    while((!ip_assigned) && (ctr > 0)) {
        DHCP_run();
        ctr--;
        HAL_Delay(300);
    }
    if(!ip_assigned) {
		#ifdef ZABBIX_DEBUG
        UART_Printf("\r\nIP was not assigned :(\r\n");
		#endif
        return;
    }

    getIPfromDHCP(net_info.ip);
    getGWfromDHCP(net_info.gw);
    getSNfromDHCP(net_info.sn);
    getZABBIXfromDHCP(net_info.zabbix);
    getHostNamefromDHCP(net_info.hostname);
    //getTimeSrvfromDHCP(net_info.tmsrv);
    if (net_info.hostname[0] == '\0') {
    	sprintf(ZabbixHostName, "%s", ZABBIXAGHOST);
    } else {
    	sprintf(ZabbixHostName, "%s", net_info.hostname);
    }

    //uint8_t dns[4];
    //getDNSfromDHCP(dns);
	#ifdef ZABBIX_DEBUG
    UART_Printf("IP:  %d.%d.%d.%d\r\nGW:  %d.%d.%d.%d\r\nNet: %d.%d.%d.%d\r\nZabbix: %d.%d.%d.%d\r\nHostName:%s\r\n",
        net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3],
        net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3],
        net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3],
        net_info.zabbix[0], net_info.zabbix[1], net_info.zabbix[2], net_info.zabbix[3],
		ZabbixHostName
    );
    UART_Printf("Calling wizchip_setnetinfo()...\r\n");
	#endif
    wizchip_setnetinfo(&net_info);

}
#endif


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t sendWorld(uint32_t dt) {
	uint8_t l1_word, l2_word, l3_word, l4_word;
	l1_word = dt & 0xFF;
	l2_word = (dt >> 8) & 0xFF;
	l3_word = (dt >> 16) & 0xFF;
	l4_word = (dt >> 24) & 0xFF;

	while ( HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) &l1_word , 1, 100));
	while ( HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) &l2_word , 1, 100));
	while ( HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) &l3_word , 1, 100));
	while ( HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) &l4_word , 1, 100));
	return dt;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC1_Init();
  MX_DAC2_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* Start init */
  bzero((char *) uartBuffer, sizeof(uartBuffer));
  sprintf(uartBuffer, "\n\rStart discriminator.\n\r");
  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

  HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);	// Reset W5500
  #ifdef ZABBIX_ENABLE
  HAL_UART_Transmit(&huart1, (uint8_t *) W5500_INIT_TEXT, sizeof(W5500_INIT_TEXT), 1000);
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
  HAL_Delay(3000);
  init_w5500();
  HAL_UART_Transmit(&huart1, (uint8_t *) INIT_OK, sizeof(INIT_OK), 1000);
  #endif


  bzero((char *) uartBuffer, sizeof(uartBuffer));
  sprintf(uartBuffer, "Resolution: %d channels.\n\r", SPECTER_SIZE);
  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);

  /* Подготовка массива для спектра */
  for (int ii = 0; ii < SPECTER_SIZE; ii++) {
	  specterBuffer[ii] = 0;
  }

  /* Подготовка счетчиков */
  pulseCounter = 0;

  /* Запускаем ADC */
  uint8_t hal_code;
  if (  HAL_OK != (hal_code = HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED))) {
	  bzero((char *) uartBuffer, sizeof(uartBuffer));
	  sprintf(uartBuffer, "Error ADC calibrate: %u.\n\r", hal_code);
	  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);
  }

  /* HV Power on */
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, HVLevel);
  HAL_Delay(500);
  HAL_GPIO_WritePin(HVEnable_GPIO_Port, HVEnable_Pin, GPIO_PIN_SET);
  bzero((char *) uartBuffer, sizeof(uartBuffer));
  sprintf(uartBuffer, "HV power on, HVLevel: %u.\n\r", HVLevel);
  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);
  bzero((char *) uartBuffer, sizeof(uartBuffer));
  sprintf(uartBuffer, "Amplifier level: %u.\n\r", ampLevel);
  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);
  bzero((char *) uartBuffer, sizeof(uartBuffer));
  sprintf(uartBuffer, "Comparator level: %u.\n\r", COMPLevel);
  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);
  bzero((char *) uartBuffer, sizeof(uartBuffer));
  sprintf(uartBuffer, "'+' - HVpower down\n\r'-' - HVPower up\n\r'p' - Comparator level up\n\r'm' - Comparator level down\n\r'u' - Amplifyre level up\n\r'd' - Amplifyre level down\n\r");
  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);

  /* Установка уровня компаратора */
  HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);
  HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, COMPLevel);

  /* Настройка уровня усиления */
  AD5245level(ampLevel);

  /*  Настройка uart для приема */
  HAL_UART_Receive_DMA(&huart1, ctrlChar, 1);

  /* Запускаем набор спектра */
  HAL_ADC_Start_DMA(&hadc1, pulseLevel, 1);
  measurementTime = HAL_GetTick();

  ssd1306_Fill(Black);
  //ssd1306_SetCursor(0, 0);
  //sprintf(text1306, "Bat:");
  //ssd1306_WriteString(text1306, Font_6x8, White);
  //ssd1306_SetCursor(0, 8);
  //sprintf(text1306, "HV: %d", HVLevel);
  //ssd1306_WriteString(text1306, Font_6x8, White);
  //ssd1306_SetCursor(0, 16);
  //sprintf(text1306, "C: %d", COMPLevel);
  //ssd1306_WriteString(text1306, Font_6x8, White);
  //ssd1306_SetCursor(0, 24);
  //sprintf(text1306, "A: %d", 1);
  //ssd1306_WriteString(text1306, Font_6x8, White);
  ssd1306_UpdateScreen();



  while (1)
  {
	  uint32_t tm = ( HAL_GetTick() - measurementTime) / 1000;

	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

	  ssd1306_SetCursor(0, 0);
	  sprintf(text1306, "C:%lu", pulseCounter);
	  ssd1306_WriteString(text1306, Font_6x8, White);
	  ssd1306_SetCursor(0, 8);
	  sprintf(text1306, "T:%lu", tm);
	  ssd1306_WriteString(text1306, Font_6x8, White);
	  ssd1306_SetCursor(0, 16);
	  sprintf(text1306, "L:%d/%d", COMPLevel, HVLevel);
	  ssd1306_WriteString(text1306, Font_6x8, White);
	  ssd1306_SetCursor(0, 24);
	  sprintf(text1306, "A:%d", ampLevel);
	  ssd1306_WriteString(text1306, Font_6x8, White);
	  ssd1306_UpdateScreen();
	  //HAL_GPIO_TogglePin(SOUND_GPIO_Port, SOUND_Pin);
	  HAL_Delay(100);
	  //uint32_t sm = 0;
	  //for (int i = 0; i < SPECTER_SIZE; i++) {
		//  sm = sm + specterBuffer[i];
	  //}
	  bzero((char *) uartBuffer, sizeof(uartBuffer));
	  sprintf(uartBuffer, "Time: %lu, Count: %lu, Rate: %0.4f cps, HVPower: %u, Comparator: %u, Amp: %u          \r", tm, pulseCounter, (float) pulseCounter / tm, HVLevel, COMPLevel, ampLevel);
	  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);
	  if (reqToSpecter) {
		  reqToSpecter = false;
		  /* Выгрузка спектра в UART */
		  if (strchr((char *) ctrlChar, 'l')) {
			  HAL_UART_Transmit(&huart1, (uint8_t *) "\n\r$\n\r", 5, 100);
			  for (int i = 0; i < SPECTER_SIZE; i++) {
				  bzero((char *) uartBuffer, sizeof(uartBuffer));
				  sprintf(uartBuffer, "%d; %lu\n\r", i, specterBuffer[i]);
				  HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100);
			  }
			  HAL_UART_Transmit(&huart1, (uint8_t *) "#\n\r", 3, 100);
		  } else
			  /* Уровень высокого напряжения */
			  if (strchr((char *) ctrlChar, '+')) {
				  	  HVLevel++;
				  	  if (HVLevel <= 0x0FFF) {
				  		  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, HVLevel);
				  	  } else {
				  		  HVLevel = 0xFFF;
				  	  }
		  } else
			  /* Уменьшение уровня усиления */
			  if (strchr((char *) ctrlChar, '-')) {
			  	  if (HVLevel != 0) {
				  	  HVLevel--;
			  	  } else {
			  		HVLevel = 0;
			  	  }
		  		  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, HVLevel);
		  } else
			  /* Очистка буфера спектрометра и счетчиков */
			  if (strchr((char *) ctrlChar, 'c')) {
					  for (int i = 0; i < SPECTER_SIZE; i++) {
						  specterBuffer[i] = 0;
					  }
					  pulseCounter = 0;
					  measurementTime = HAL_GetTick();
		  } else
			  /* Уменьшение уровня компаратора */
			  if (strchr((char *) ctrlChar, 'm')) {
				  if (COMPLevel != 0) {
					  COMPLevel--;
					  HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, COMPLevel);
				  } else {
					  COMPLevel = 0;
				  }
		  } else
			  /* Увеличение уровня компаратора */
			  if (strchr((char *) ctrlChar, 'p')) {
				  if (COMPLevel <= 0xFFF) {
					  COMPLevel++;
					  HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, COMPLevel);
				  } else {
					  COMPLevel = 0xFFF;
				  }
			  } else
				  /* Увеличение Усиления */
				  if (strchr((char *) ctrlChar, 'u')) {
					  if (ampLevel <= 0xFF) {
						  ampLevel++;
						  AD5245level(ampLevel);
					  } else {
						  ampLevel = 0xFF;
					  }
				  } else
					  /* Уменьшение усиления */
					  if (strchr((char *) ctrlChar, 'd')) {
						  if (ampLevel <= 0xFF) {
							  ampLevel++;
							  AD5245level(ampLevel);
						  } else {
							  ampLevel = 0xFF;
						  }
		  } else
			/* Выгрузка бинарных данных */
			  if (strchr((char *) ctrlChar, 'b')) {
				  /* Подготовка тестовых данных */
				  //for(int i = 3900; i < SPECTER_SIZE; i++) {
				  //	specterBuffer[i] = 0;
				  //}
				  //specterBuffer[4095] = 0;
				  while ( HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) "<B>", 3, 100));
				  uint32_t cs_summ = 0, tmp_data;

				  /* Подготовка и передача времени измерения */
				  tmp_data = tm;
				  cs_summ = sendWorld(tmp_data);

				  /* Подготовка и передача количества импульсов */
				  tmp_data = pulseCounter;
				  cs_summ = cs_summ + sendWorld(tmp_data);

				  /* Передача данных спектра */
				  int idd = 0;
				  for (int i = 0; i < SPECTER_SIZE; i++) {
					  tmp_data = specterBuffer[i];
					  cs_summ = cs_summ + sendWorld(tmp_data);
					  idd++;
				  }
				  sendWorld(cs_summ);
				  //bzero((char *) uartBuffer, sizeof(uartBuffer));
				  //sprintf(uartBuffer,  "\n\rc_summ: %lu, idx: %d\n\r", cs_summ, idd);
				  //while ( HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) uartBuffer, strlen(uartBuffer), 100));
			  }
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 21;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_EXT_IT11;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DAC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC2_Init(void)
{

  /* USER CODE BEGIN DAC2_Init 0 */

  /* USER CODE END DAC2_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC2_Init 1 */

  /* USER CODE END DAC2_Init 1 */

  /** DAC Initialization
  */
  hdac2.Instance = DAC2;
  if (HAL_DAC_Init(&hdac2) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac2, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC2_Init 2 */

  /* USER CODE END DAC2_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00503D58;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SOUND_GPIO_Port, SOUND_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, W5500_CS_Pin|HVEnable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, W5500_RST_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SOUND_Pin */
  GPIO_InitStruct.Pin = SOUND_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SOUND_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : W5500_CS_Pin HVEnable_Pin */
  GPIO_InitStruct.Pin = W5500_CS_Pin|HVEnable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : W5500_RST_Pin LED_Pin */
  GPIO_InitStruct.Pin = W5500_RST_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Pulse_Pin */
  GPIO_InitStruct.Pin = Pulse_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Pulse_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
