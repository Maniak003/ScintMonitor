#include <stdio.h>
#include <inttypes.h>
#include "main.h"
#define AD5245

extern I2C_HandleTypeDef hi2c2;

void AD5245level(uint8_t lev);
#ifdef AD5245
	void AD5245level(uint8_t lev);
	#define AJUST_DELAY 0
	#define AD5245_I2C_ADDR (0x2C << 1)
	#define AD5245_WRITE    0x00
	#define AD5245_RESET    0x40
	#define AD5245_SHUTDOWN 0x20
	#define AD5245_I2C_PORT hi2c2
#endif
