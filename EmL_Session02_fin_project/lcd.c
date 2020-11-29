/*
 * lcd.c
 *
 *  Created on: Nov 25, 2020
 *      Author: taehoonkim
 */
#include "lcd.h"

void LCD_WriteCommand(uint8_t command)
{
    if((SSI0_SR_R&0x08) == 0x08){}    //1) Read SSI0_SR_R and check bit 4,
                                       //2) If bit 4 is high,
                                       //loop back to step 1 (wait for BUSY bit to be low)
    else if((SSI0_SR_R&0x08)!=0x08){
       PA6 = 0x00;                     //3) Clear D/C=PA6 to zero
       SSI0_DR_R = command;            //4) Write the command to SSI0_DR_R
       if((SSI0_SR_R&0x08) == 0x08){} //5) Read SSI0_SR_R and check bit 4,
                                       //6) If bit 4 is high,
                                       //loop back to step 5 (wait for BUSY bit to be low)
     }
}

void LCD_WriteData(uint16_t data)
{
   if((SSI0_SR_R&0x01) == 0x00){}
   else if((SSI0_SR_R&0x01) != 0x00) {
       PA6 = 0x80;
       SSI0_DR_R = data;
   }
}
