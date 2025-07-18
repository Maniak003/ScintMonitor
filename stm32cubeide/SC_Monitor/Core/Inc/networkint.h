#ifndef __NETWORKINT_H
#define __NETWORKINT_H


#include "main.h"
#include "w5500.h"
#include "socket.h"
#include "dhcp.h"
#include "webpages.h"

/* For W5500*/
#define ZABBIX_ENABLE
#define DHCP_SOCKET     0
//#define DNS_SOCKET      1
#define TCP_SOCKET		1
#define HTTP_SOCKET		2
#define _DHCP_DEBUG_
#define ZABBIX_DEBUG
#define DATA_BUF_SIZE   16384
#define ZABBIX_ENABLE
#define ZABBIXAGHOST	"SCINTMONITOR"  // Default hostname.
#define ZABBIXPORT		10051
#define ZABBIXMAXLEN	128
#define MAC_ADDRESS		0x00, 0x11, 0x22, 0x33, 0x45, 0xF1
#define MAX_HTTPSOCK	4
#define LISTEN_PORT		80
#define TOUTCONNECT		100
#define RECEIVE_BUFF_SIZE 128


void init_w5500(void);
int http_socket(uint8_t tcp_s);
void W5500_WriteByte(uint8_t byte);
uint8_t W5500_ReadByte(void);
void W5500_WriteBuff(uint8_t* buff, uint16_t len);
void W5500_ReadBuff(uint8_t* buff, uint16_t len);
void W5500_Unselect(void);
void W5500_Select(void);
void UART_Printf(const char* fmt, ...);
void Callback_IPAssigned(void);
void Callback_IPConflict(void);
uint8_t sendToZabbix(uint8_t * addr, char * host, char * key, float value);
int linsten_tcp_socket(void);

extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi2;
extern uint8_t dhcp_buffer[1024];
extern wiz_NetInfo net_info;
extern volatile bool ip_assigned;
extern uint8_t receive_buff[RECEIVE_BUFF_SIZE];


#endif
