/*
 * main.c
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */
#include "main.h"
#include "sysclk.h"
#include "systick.h"
#include "gpio.h"
#include "adc.h"
#include "image.h"
#include "lcd.h"
#include "nokia5110.h"

uint8_t SysTick_Flag = 0;
//float EMA_a = 0.6;
//uint32_t EMA_filt_x = 0;
//uint32_t EMA_filt_y = 0;
//volatile uint32_t ADC_xy_Data[2] = { 0, 0 };

//#define PA4   (*((volatile unsigned long *)0x40004040))
//#define PA5   (*((volatile unsigned long *)0x40004080))

#define PLAYERH    ((uint8_t) PlayerShip0[22])
#define ENEMY10H 0x0A


void EnableInterrupts(void)
{
    __asm ("    CPSIE  I\n");
}

int main(void)
{
    SYSCLK_80MHz();
    SysTick_Init(2400); // 1/30 sec (30Hz).

    PortE_Init();

    Nokia5110_Init();
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer();
    Nokia5110_PrintBMP(32,47,PlayerShip0,0);
    Nokia5110_PrintBMP(33,47-PLAYERH,Bunker0,0);
    Nokia5110_PrintBMP(32,ENEMY10H-1,SmallEnemy10PointA,0);
    Nokia5110_PrintBMP(32,ENEMY10H-1,SmallEnemy20PointA,0);
    Nokia5110_PrintBMP(32,ENEMY10H-1,SmallEnemy30PointA,0);
    Nokia5110_PrintBMP(32,ENEMY10H-1,SmallEnemy30PointB,0);
    Nokia5110_PrintBMP(32,ENEMY10H-1,SmallEnemy20PointB,0);
    Nokia5110_DisplayBuffer();



    ADC0_PortE_Init();
    ADC_var_Init();

    EnableInterrupts();

    while (1)
    {
        if (SysTick_Flag)
        {

            //LCD_Dis();

            SysTick_Flag = 0;
        }
    }
    return 0;
}

void SysTick_Handler(void)
{
    /*ADC routine*/
    //Neutral value -> 2047 is an example.
    //should be checked by experiment.
    ADC0_InSeq2(&ADC);
    /*noise filtering*/
    //EMA_filt_x = (EMA_a*ADC.xy_Data[0]) + ((1-EMA_a)*EMA_filt_x);
    //EMA_filt_y = (EMA_a*ADC.xy_Data[0]) + ((1-EMA_a)*EMA_filt_y);

    if (ADC.xy_Data[0] != 2047)
    {

    }
    if (ADC.xy_Data[1] != 2047)
    {

    }

    /*Switch routine*/
    //-> External interrupt로 해야하나....? 한다면 -> interrupt로 해야함.
    //priority는 systick보다 낮게 해야함.
    SysTick_Flag = 1;
}

