#include "lcd1602.h"


void LCD_string(uint8_t* str){
	for(uint8_t i=0;str[i]!='\0';i++){
		lcdChar(str[i]);
	}
}

void setCursor(uint8_t col, uint8_t row)
{
	lcdSend(1,0x80 | (col | row*0x40));
}

void parametrsToggle(uint8_t display,uint8_t cursor,uint8_t blink){
	uint8_t comm = 0x08;
	if(display)comm|=0x04;
	if(cursor)comm|=0x02;
	if(blink)comm|=0x01;
	lcdCommand(comm);
}

void shiftToggle(uint8_t rightShift,uint8_t scrollInc){
	uint8_t comm = 0x04;
	if(rightShift)comm|=0x02;
	if(scrollInc)comm|=0x01;
	lcdCommand(comm);
}

void scrollRight(void){
	lcdCommand(0x10|0x08|0x00);
}

void scrollLeft(void){
	lcdCommand(0x10|0x08|0x04);
}

void clear(void)
{
	lcdCommand(0x01);
	HAL_Delay(2);
}

void home(void)
{
	lcdCommand(0x02);
	HAL_Delay(2);
}

void lcdSend(uint8_t isCommand, uint8_t data) {
    if(isCommand==1) RS(RESET) else RS(SET);
    HAL_Delay(5);

    if(((data >> 7) & 1)==1) DH7(SET) else DH7(RESET);
    if(((data >> 6) & 1)==1) DH6(SET) else DH6(RESET);
    if(((data >> 5) & 1)==1) DH5(SET) else DH5(RESET);
    if(((data >> 4) & 1)==1) DH4(SET) else DH4(RESET);

    EN(SET);
    HAL_Delay(5);
    EN(RESET);

    if(((data >> 3) & 1)==1) DH7(SET) else DH7(RESET);
    if(((data >> 2) & 1)==1) DH6(SET) else DH6(RESET);
    if(((data >> 1) & 1)==1) DH5(SET) else DH5(RESET);
    if(((data >> 0) & 1)==1) DH4(SET) else DH4(RESET);

    EN(SET);
    HAL_Delay(5);
    EN(RESET);
}

void lcdCommand(uint8_t cmd) {
    lcdSend(1, cmd);
}

void lcdChar(const char chr) {
    lcdSend(0, (uint8_t)chr);
}

void lcdWriteCustomChar(uint8_t* data){
	lcdCommand(0x40);
	HAL_Delay(5);
	for(uint8_t i=0;i<7;i++){
		lcdChar(data[i]);
		HAL_Delay(5);
	}
	lcdCommand(0x80);
	HAL_Delay(5);
	lcdChar(0);
}

void lcdInit(){
	HAL_Delay(50);
	lcdCommand(0x30);
	home();
	parametrsToggle(1,1,0);
	clear();
	setCursor(0,0);
}
