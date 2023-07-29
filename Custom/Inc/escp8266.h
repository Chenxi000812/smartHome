//
// Created by ³¿êØ on 2023/7/26.
//

#ifndef TEST01_ESCP8266_H
#define TEST01_ESCP8266_H

#endif //TEST01_ESCP8266_H
#include "gpio.h"

extern uint8_t CMD_INDEX;
extern uint8_t RETRY_COUNT;
extern char * esp8266_CMD[];
extern char * BAFA_CMD[];
extern uint8_t IN_BAFA;
void ESP8266_ON_MSG(char* msg);
void ESP8266_RESET();
void ESP8266_SEND_MSG(char* msg);
void BAFA_SEND_MSG(char* msg);
void BAFA_ON_MSG(char* msg);
void BAFA_TTL();
void BAFA_SUB();
