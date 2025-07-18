#include "networkint.h"



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


wiz_NetInfo net_info = {
	.mac  = { MAC_ADDRESS },
	.dhcp = NETINFO_DHCP
};

__attribute__((aligned(4)))
uint8_t dhcp_buffer[1024];
__attribute__((aligned(4)))
uint8_t receive_buff[RECEIVE_BUFF_SIZE];

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

/*
 * Процедура для блокирующего чтения из сокета
 */
int linsten_tcp_socket(void) {
	uint16_t t_out_connect;
	int8_t ss_state = 0x00;
	ss_state = socket(HTTP_SOCKET, Sn_MR_TCP, LISTEN_PORT, 0);
	if (ss_state != HTTP_SOCKET) {
		#ifdef ZABBIX_DEBUG
		UART_Printf("socket - Error: %d\r\n", ss_state);
		#endif
		return (-1);
	}

	//uint8_t socket_io_mode = SOCK_IO_NONBLOCK;
	uint8_t socket_io_mode = SOCK_IO_BLOCK;

	ss_state = ctlsocket(HTTP_SOCKET, CS_SET_IOMODE, &socket_io_mode);
	if (ss_state != SOCK_OK) {
		#ifdef ZABBIX_DEBUG
		UART_Printf("ctlsocket - Error: %d\r\n", ss_state);
		#endif
		//close(HTTP_SOCKET);
		return (-1);
	}

	ss_state = listen(HTTP_SOCKET);
	if(ss_state != SOCK_OK) {
		#ifdef ZABBIX_DEBUG
		UART_Printf("listen - Error: %d\r\n", ss_state);
		#endif
		//close(HTTP_SOCKET);
		return (-1);
	}
	uint8_t sr = 0x00, sr_tmp = 0x00;
	do {
		 sr = getSn_SR(HTTP_SOCKET);
		//#ifdef ZABBIX_DEBUG
		 //if (sr != sr_tmp) {
		//	 UART_Printf("SR: %x.\r\n", sr);
		//	 sr_tmp = sr;
		 //}
		//#endif
	} while (sr != 0x17 && sr != 0x00 /*&& sr != 0x14*/);

	switch (sr) {
	case 0x00:
		#ifdef ZABBIX_DEBUG
		UART_Printf("Some error occurred on server socket. Please restart.\r\n");
		#endif
		return(-1);
		//break;
	case 0x17:
		#ifdef ZABBIX_DEBUG
		UART_Printf("Client connected.\r\n");
		#endif
		t_out_connect = TOUTCONNECT;
		while (t_out_connect-- > 1) {
			int len = recv(HTTP_SOCKET, receive_buff, RECEIVE_BUFF_SIZE);
			if(len == SOCKERR_SOCKSTATUS) {
				#ifdef ZABBIX_DEBUG
				UART_Printf("Client disconnect.\r\n");
				#endif
				break;
			}
			receive_buff[len]='\0';
			#ifdef ZABBIX_DEBUG
			UART_Printf("Read from client: %s\r\n", receive_buff);
			#endif
			send(HTTP_SOCKET, (uint8_t *)"123\n\r", 5);
			//HAL_Delay(1000);
			disconnect(HTTP_SOCKET);
			break;
		}
		//close(HTTP_SOCKET);
		break;
	}
	return(0);
}

void init_w5500() {
	#ifdef ZABBIX_DEBUG
    UART_Printf("\r\ninit() called!\r\nW5500 callbacks...\r\n");
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
    UART_Printf("setSHAR) with mac:%02x:%02x:%02x:%02x:%02x:%02x, DHCP:%d\r\n", net_info.mac[0], net_info.mac[1], net_info.mac[2], net_info.mac[3], net_info.mac[4], net_info.mac[5], net_info.dhcp);
	#endif

    // set MAC address before using DHCP
    setSHAR(net_info.mac);
    UART_Printf("DHCP_init()...\r\n");
    DHCP_init(DHCP_SOCKET, dhcp_buffer);
	#ifdef ZABBIX_DEBUG
    UART_Printf("DHCP callbacks...\r\n");
	#endif
    reg_dhcp_cbfunc(Callback_IPAssigned, Callback_IPAssigned, Callback_IPConflict);
	#ifdef ZABBIX_DEBUG
    UART_Printf("DHCP_run()...\r\n");
	#endif
    // actually should be called in a loop, e.g. by timer
    uint32_t ctr = 100;
    while((!ip_assigned) && (ctr > 0)) {
		#ifdef ZABBIX_DEBUG
        UART_Printf("DHCP_try: %3lu\r", ctr);
		#endif
        DHCP_run();
        ctr--;
        HAL_Delay(1000);
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
    //getZABBIXfromDHCP(net_info.zabbix);
    //getHostNamefromDHCP(net_info.hostname);
    //getTimeSrvfromDHCP(net_info.tmsrv);
    //if (net_info.hostname[0] == '\0') {
    //	sprintf(ZabbixHostName, "%s", ZABBIXAGHOST);
    //} else {
    //	sprintf(ZabbixHostName, "%s", net_info.hostname);
    //}

    //uint8_t dns[4];
    getDNSfromDHCP(net_info.dns);
	#ifdef ZABBIX_DEBUG
    UART_Printf("IP:  %d.%d.%d.%d\r\nGW:  %d.%d.%d.%d\r\nNet: %d.%d.%d.%d\r\nDNS: %d.%d.%d.%d\r\n",
        net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3],
        net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3],
        net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3],
		net_info.dns[0], net_info.dns[1], net_info.dns[2], net_info.dns[3]
    );
    UART_Printf("Calling wizchip_setnetinfo()...\r\n");
	#endif
    //wizchip_setnetinfo(&net_info);
    ctlnetwork(CN_SET_NETINFO, (void*) &net_info);

    /*
    wiz_PhyConf phyconf;

	phyconf.by=PHY_CONFBY_SW;
	phyconf.duplex=PHY_DUPLEX_FULL;
	phyconf.speed=PHY_SPEED_100;					//100MBps Ethernet link speed
	phyconf.mode=PHY_MODE_AUTONEGO;					//best to go with auto-negotiation
	ctlwizchip(CW_SET_PHYCONF, (void*) &phyconf);
	*/
}


#ifdef ZABBIX_ENABLE

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
#endif
