/*
 * Project name: TAU edx chap. 8
 * Dev. by: Sean K.
 * Data: 2020.09.28
 */

/*
 * PF4 -> Input (Polling)
 * PF1, 2, 3 -> Output (Depending on switch status)
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"

/*variables & definitions*/
//PF1의 경우에는 0x02, PF2는 0x04, PF3은 0x08을 씀으로써 toggle이 가능하다.
#define PF1   (*((volatile unsigned long *)0x40025008)) //GPIO_PCTL_PF1_M을 보면 0xF0으로 마스킹되어 있는데 따라서 4개의 비트를 한개의 비트가 차지하고 있으므로 *4한다.
#define PF2   (*((volatile unsigned long *)0x40025010))
#define PF3   (*((volatile unsigned long *)0x40025020))
#define PF4   (*((volatile unsigned long *)0x40025040))
#define PF0   (*((volatile unsigned long *)0x40025004))

/*func.*/
void Sys_Init();

/*  */
void Sys_Init()
{
    /*SysCLK*/

    /*GPIO*/
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x20;           // 1) activate clock for Port F
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F -> GPIOCR을 unlock.
    GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
                                      // only PF0 needs to be unlocked, other bits can't be locked
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out //0b0000,1110
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
    GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0

    PF1 = 0x00;
    PF2 = 0x00;
    PF3 = 0x00;
}

int main()
{
    Sys_Init();

    while (1)
    {
        /*
        In01 = (GPIO_PORTF_DATA_R & 0x10);
        In01 >>= 4;
        In02 = (GPIO_PORTF_DATA_R & 0x01);
        */

        if (PF4 == 0x00)
        {
            //PF1 on
            PF1 = 0x02;
            PF2 = 0x00;
            PF3 = 0x00;
        }
        else if (PF0 == 0x00)
        {
            //PF2 on
            PF1 = 0x00;
            PF2 = 0x04;
            PF3 = 0x00;
        }
        else
        {
            //PF3 on
            PF1 = 0x00;
            PF2 = 0x00;
            PF3 = 0x08;
        }
    }
}


/*
 void LED_On(void);

 // Make PF1 High
 void LED_Off(void)
 {
 GPIO_PORTF_DATA_R |= 0x04;
 }
 // Make PF1 low
 void LED_On(void)
 {
 GPIO_PORTF_DATA_R &= ~0x04;
 }
 unsigned long Switch_Input(void)
 {

 return PA5; // return 0x20(pressed) or 0(not pressed)

 }

 unsigned long Switch_Input2(void)
 {

 return (GPIO_PORTA_DATA_R & 0x20); // 0x20(pressed) or 0(not pressed)

 }
 */

