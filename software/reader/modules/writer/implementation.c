#include "interface.h"

#ifdef MODE_TEST
#include <stdio.h>
#endif
#include "../Arduino.h"
#include "../connection.h"
#include "../coop_t/interface.h"
#include "../Ton/interface.h"

#define BUSY_TIMEOUT_US				1700

static Ton busy_timern;
static Ton *busy_timer = &busy_timern;

static uint8_t wsign = 0;

static coop_t writeData_DUMMY(Buffer *buffer);
static coop_t writeData_WAIT_DATA(Buffer *buffer);
static coop_t writeData_GET_NEXT_SIGN(Buffer *buffer);
static coop_t writeData_WRITE_START_SIGN(Buffer *buffer);
static coop_t writeData_WRITE_SIGNS(Buffer *buffer);
static coop_t writeData_WRITE_STOP_SIGN(Buffer *buffer);

static coop_t (*writeData)(Buffer *) = writeData_DUMMY;
	
static coop_t writeData_DUMMY(Buffer *buffer){
	return YIELD;
}

static coop_t writeData_WAIT_DATA(Buffer *buffer){
	
	if(buffer_lockForReading(buffer) != RESULT_SUCCESS){
		return YIELD;
	}
	
	writeData = writeData_WRITE_START_SIGN;
	return PROCEED;
}

static coop_t writeData_GET_NEXT_SIGN(Buffer *buffer){
	if(buffer_readNext(buffer, &wsign) != RESULT_SUCCESS){
		writeData = writeData_WRITE_STOP_SIGN;
		return PROCEED;
	}
	writeData = writeData_WRITE_SIGNS;
	return PROCEED;
}

static coop_t writeData_WRITE_START_SIGN(Buffer *buffer){
	wsign = CONNECTION_PACKAGE_DELIMITER_START;
#ifdef MODE_TEST
	printf("%c", wsign);
#else
	if(serial_writeByte(&wsign) != RESULT_SUCCESS){
		return PROCEED;
	}
#endif
	writeData = writeData_GET_NEXT_SIGN;
	return PROCEED;
}

static coop_t writeData_WRITE_SIGNS(Buffer *buffer){
#ifdef MODE_TEST
	printf("%hhd.", wsign);
	//printf("%.2hhd.", wsign);
#else
	if(serial_writeByte(&wsign) != RESULT_SUCCESS){
		return PROCEED;
	}
#endif
	writeData = writeData_GET_NEXT_SIGN;
	return PROCEED;
}

static coop_t writeData_WRITE_STOP_SIGN(Buffer *buffer){
	wsign = CONNECTION_PACKAGE_DELIMITER_STOP;
#ifdef MODE_TEST
	printf("%c\n", wsign);
#else
	if(serial_writeByte(&wsign) != RESULT_SUCCESS){
		return PROCEED;
	}
#endif
	buffer_unlockForReading(buffer);
	writeData = writeData_WAIT_DATA;
	return YIELD;
}

void writer_begin(){
	ton_setInterval(busy_timer, BUSY_TIMEOUT_US);
	tonu_reset(busy_timer);
	writeData = writeData_WAIT_DATA;
}

void writer_run(Buffer *buffer){
	tonu_reset(busy_timer);
	while (1) {
		if (tonu(busy_timer)) {return;}
	    if(writeData(buffer) == YIELD){
			return;
		}
	}
}
