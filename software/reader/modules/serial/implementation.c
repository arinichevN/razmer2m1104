#include "interface.h"

#include "../Arduino.h"

#ifdef MODE_TEST
#include <stdio.h>
void serial_begin(int baud_rate, int config){
	;
}

result_t serial_writeByte(void *v){
	char *pc = v;
	printf("%c\n", *pc);
	return RESULT_SUCCESS;
}
#else
void serial_begin(int baud_rate, int config){
	Serial.begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
}

result_t serial_writeByte(void *v){
	if(Serial.availableForWrite() < 1) {
		return RESULT_FAILURE;
	}
	Serial.write((char *)v, 1);
	return RESULT_SUCCESS;
}
#endif
