//
// Created by 晨曦 on 2023/7/26.
//
#include <malloc.h>
#include <stdio.h>
#include "escp8266.h"
#include "usart.h"
#include "tim.h"

char * esp8266_CMD[] = {"AT+CWMODE=1\r\n",
                        "AT+CWJAP=\"ChinaNet-MwVV\",\"dm9tqarz\"\r\n",
                        "AT+CIPMUX=0\r\n",
                        "AT+CIPSTART=\"TCP\",\"bemfa.com\",8344\r\n",
                        "AT+CIPMODE=1\r\n",
                        "AT+CIPSEND\r\n"};
uint8_t CMD_INDEX = 0;
uint8_t RETRY_COUNT = 0;
uint8_t TIMEOUT_COUNT=0;
uint8_t IN_BAFA=0;
char* uuid = "3ea77ad32111409cbc79f9f05c8adaa8";
char * BAFA_DEVICES[] = {"light12002"};
char * BAFA_CMD[] = {"ping\r\n","cmd=1&uid=%s&topic=%s\\r\\n","cmd=2&uid=%s&topic=%s&msg=%s\\r\\n"};

/**
 * 当ESP8266收到消息时
 * @param msg 消息
 */
void ESP8266_ON_MSG(char* msg){
    if (IN_BAFA) {BAFA_ON_MSG(msg);
        return;}
    char* res = strstr(msg,"OK");
    if (res!=NULL){
        HAL_TIM_Base_Stop_IT(&htim2);
        if (++CMD_INDEX>=sizeof(esp8266_CMD)/4){
            IN_BAFA = 1;
            HAL_TIM_Base_Start_IT(&htim2);
            BAFA_SUB();
            CMD_INDEX = 0;
            return;
        }
    }else{
        //没有OK继续等
        HAL_TIM_Base_Start_IT(&htim2);
        return;
    }
    ESP8266_SEND_MSG(esp8266_CMD[CMD_INDEX]);
}
void ESP8266_SEND_MSG(char* msg){
    HAL_UART_Transmit_DMA(&huart2,(uint8_t*)msg, strlen(msg));
}
void BAFA_SEND_MSG(char* msg){


}
void BAFA_ON_MSG(char* msg){
    if (strstr(msg,"msg=on")!=NULL){
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    }
    if (strstr(msg,"msg=off")!=NULL){
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);

    }
}


void setUUID(char* uuid){

}
uint8_t devices_len(){
    uint8_t len = 0;
    for (int i = 0; i < sizeof (BAFA_DEVICES)/4; ++i) {
        len+= strlen(BAFA_DEVICES[0]);
    }
    return len;
}

void devicesStr(char * string){
    for (int i = 0; i < sizeof(BAFA_DEVICES)/4; ++i) {
        strcat(string,BAFA_DEVICES[i]);
        if (i != (sizeof(BAFA_DEVICES) /4)-1)strcat(string,",");
    }
}

void BAFA_TTL(){
    ESP8266_SEND_MSG("ping\r\n");

}

void BAFA_SUB(){
    char sstr[devices_len()];
    devicesStr(sstr);

    char buf[500] = { 0 };
    sprintf(buf,"cmd=%s&uid=%s&topic=%s\r\n","1",uuid,sstr);
    ESP8266_SEND_MSG(buf);
}
void BAFA_PUB(){
    
}

/**
 * 重启ESP
 */
void ESP8266_RESET(){
    HAL_UART_Transmit_DMA(&huart2,(uint8_t*)"AT+RST\r\n", 8);
}
