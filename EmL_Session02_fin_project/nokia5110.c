/*
 * nokia5110.c
 *
 *  Created on: Nov 22, 2020
 *      Author: taehoonkim
 */

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected,
//consists of 4 white LEDs which draw ~80mA total
#include "nokia5110.h"

void Nokia5110_Init(void)
{
    volatile int32_t delay;
    SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0
    SYSCTL_RCGCGPIO_R |= 0x01; // activate port A
    delay = SYSCTL_RCGCGPIO_R;               // allow time to finish activating
    GPIO_PORTA_DIR_R |= 0xC0;             // make PA6,7 out
    GPIO_PORTA_AFSEL_R |= 0x2C;           // enable alt funct on PA2,3,5
    GPIO_PORTA_AFSEL_R &= ~0xC0;          // disable alt funct on PA6,7
    GPIO_PORTA_DEN_R |= 0xEC;             // enable digital I/O on PA2,3,5,6,7
                                          // configure PA2,3,5 as SSI
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFF0F00FF) + 0x00202200;
    // configure PA6,7 as GPIO
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0x00FFFFFF) + 0x00000000;
    GPIO_PORTA_AMSEL_R &= ~0xEC;  // disable analog functionality on PA2,3,5,6,7
    SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
    SSI0_CR1_R &= ~SSI_CR1_MS;            // master mode
                                          // configure for system clock/PLL baud clock source
    SSI0_CC_R = (SSI0_CC_R & ~SSI_CC_CS_M) + SSI_CC_CS_SYSPLL;
    // clock divider for 3.125 MHz SSIClk (50 MHz PIOSC/16)
    SSI0_CPSR_R = (SSI0_CPSR_R & ~SSI_CPSR_CPSDVSR_M) + 16;
    SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (3.125 Mbps data rate)
            SSI_CR0_SPH |         // SPH = 0
            SSI_CR0_SPO);         // SPO = 0
                                  // FRF = Freescale format
    SSI0_CR0_R = (SSI0_CR0_R & ~SSI_CR0_FRF_M) + SSI_CR0_FRF_MOTO;
    // DSS = 8-bit data
    SSI0_CR0_R = (SSI0_CR0_R & ~SSI_CR0_DSS_M) + SSI_CR0_DSS_8;
    SSI0_CR1_R |= SSI_CR1_SSE;            // enable SSI

    RESET = RESET_LOW;                    // reset the LCD to a known state
    for (delay = 0; delay < 10; delay = delay + 1)
        ;                    // delay minimum 100 ns
    RESET = RESET_HIGH;                   // negative logic

    LCD_WriteCommand(0x21); // chip active; horizontal addressing mode (V = 0); use extended instruction set (H = 1)
                            // set LCD Vop (contrast), which may require some tweaking:
    LCD_WriteCommand (0xB1); // try 0xB1 (for 3.3V red SparkFun), 0xB8 (for 3.3V blue SparkFun), 0xBF if your display is too dark, or 0x80 to 0xFF if experimenting
    LCD_WriteCommand(0x04);               // set temp coefficient
    LCD_WriteCommand(0x14);              // LCD bias mode 1:48: try 0x13 or 0x14

    LCD_WriteCommand(0x20); // we must send 0x20 before modifying the display control mode
    LCD_WriteCommand(0x0C); // set display control to normal mode: 0x0D for inverse
}

//********LCD_OutChar*****************
// Print a character to the Nokia 5110 48x84 LCD.  The
// character will be printed at the current cursor position,
// the cursor will automatically be updated, and it will
// wrap to the next row or back to the top if necessary.
// One blank column of pixels will be printed on either side
// of the character for readability.  Since characters are 8
// pixels tall and 5 pixels wide, 12 characters fit per row,
// and there are six rows.
// inputs: data  character to print
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutChar(int8_t data)
{
    int32_t i;
    LCD_WriteData(0x00);                  // blank vertical line padding
    for (i = 0; i < 5; i = i + 1)
    {
        LCD_WriteData(ASCII[data - 0x20][i]);
    }
    LCD_WriteData(0x00);                  // blank vertical line padding
}

//********LCD_OutString*****************
// Print a string of characters to the Nokia 5110 48x84 LCD.
// The string will automatically wrap, so padding spaces may
// be needed to make the output look optimal.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutString(int8_t *ptr)
{
    while (*ptr)
    {
        Nokia5110_OutChar(*ptr);
        ptr = ptr + 1;
    }
}

//********LCD_Goto*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=11)
//         newY  new Y-position of the cursor (0<=newY<=5)
// outputs: none
void Nokia5110_SetCursor(uint8_t newX, uint8_t newY)
{
    if ((newX > 11) || (newY > 5))
    {        // bad input
        return;                             // do nothing
    }
    // multiply newX by 7 because each character is 7 columns wide
    LCD_WriteCommand(0x80 | (newX * 7));     // setting bit 7 updates X-position
    LCD_WriteCommand(0x40 | newY);          // setting bit 6 updates Y-position
}

//********LCD_Clear*****************
// Clear the LCD by writing zeros to the entire screen and
// reset the cursor to (0,0) (top left corner of screen).
// inputs: none
// outputs: none
void Nokia5110_Clear(void)
{
    int32_t i;
    for (i = 0; i < (MAX_X * MAX_Y / 8); i = i + 1)
    {
        LCD_WriteData(0x00);
    }
    Nokia5110_SetCursor(0, 0);
}

//********LCD_DrawFullImage*****************
// Fill the whole screen by drawing a 48x84 bitmap image.
// inputs: ptr  pointer to 504 byte bitmap
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_DrawFullImage(uint8_t *ptr)
{
    int32_t i;
    Nokia5110_SetCursor(0, 0);
    for (i = 0; i < (MAX_X * MAX_Y / 8); i = i + 1)
    {
        LCD_WriteData(ptr[i]);
    }
}

//****************plotting functions********************
#define XSIZE 84
#define YSIZE 40
#define SIZE (XSIZE*YSIZE/8)
uint8_t PlotImage[SIZE];  // 84 wide by 40 tall plot
int32_t Ymax, Ymin, X;  // X goes from 0 to 83
int32_t Yrange;

// *************** LCD_PlotClear ********************
// Clear the graphics buffer, set X coordinate to 0
// It does not output to display until LCD_ShowPlot called
// Inputs: ymin and ymax are range of the plot
// Outputs: none
void LCD_PlotClear(int32_t ymin, int32_t ymax)
{
    unsigned long i;
    for (i = 0; i < SIZE; i++)
    {
        PlotImage[i] = 0; // clear, blank
    }
    if (ymax > ymin)
    {
        Ymax = ymax;
        Ymin = ymin;
        Yrange = ymax - ymin;
    }
    else
    {
        Ymax = ymin;
        Ymin = ymax;
        Yrange = ymax - ymin;
    }
    X = 0;
}
const uint8_t BitMask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
//  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
        };
// *************** LCD_PlotPoint ********************
// Used in the voltage versus time plot, plot one point at y
// It does not output to display until LCD_ShowPlot called
// Inputs: y is the y coordinate of the point plotted
// Outputs: none
void LCD_PlotPoint(int32_t y)
{
    long i, j;
    if (y < Ymin)
        y = Ymin;
    if (y > Ymax)
        y = Ymax;
    // X goes from 0 to 83
    // j goes from 1 to 39
    // y=Ymin maps to j=0
    // y=Ymax maps to j=39
    j = (39 * (y - Ymin)) / Yrange;
    if (j < 0)
        j = 0;
    if (j > 39)
        j = 39;
    i = 4 - j / 8;
    PlotImage[i * 84 + X] |= BitMask[j & 0x07];
}

// *************** LCD_PlotNext ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 83, then back to 0 again
// It does not output to display until LCD_ShowPlot called
// Inputs: none
// Outputs: none
void LCD_PlotNext(void)
{
    if (X == 83)
    {
        X = 0;
    }
    else
    {
        X++;
    }
}

// *************** LCD_ShowPlot ********************
// Dump data to display
// Inputs: none
// Outputs: none
void LCD_ShowPlot(void)
{
    int i;
    Nokia5110_SetCursor(0, 1);
    for (i = 0; i < SIZE; i = i + 1)
    {
        LCD_WriteData(PlotImage[i]);
    }
}

//********LCD_PrintBMP*****************
// Bitmaps defined above were created for the LM3S1968 or
// LM3S8962's 4-bit grayscale OLED display.  They also
// still contain their header data and may contain padding
// to preserve 4-byte alignment.  This function takes a
// bitmap in the previously described format and puts its
// image data in the proper location in the buffer so the
// image will appear on the screen after the next call to
//   LCD_DisplayBuffer();
// The interface and operation of this process is modeled
// after RIT128x96x4_BMP(x, y, image);
// inputs: xpos      horizontal position of bottom left corner of image, columns from the left edge
//                     must be less than 84
//                     0 is on the left; 82 is near the right
//         ypos      vertical position of bottom left corner of image, rows from the top edge
//                     must be less than 48
//                     2 is near the top; 47 is at the bottom
//         ptr       pointer to a 16 color BMP image
//         threshold grayscale colors above this number make corresponding pixel 'on'
//                     0 to 14
//                     0 is fine for ships, explosions, projectiles, and bunkers
// outputs: none
void Nokia5110_PrintBMP(uint32_t xpos, uint32_t ypos, const uint8_t *ptr,
                        uint32_t threshold)
{
    int32_t width = ptr[18], height = ptr[22], i, j;
    uint32_t screenx, screeny;
    uint32_t mask;
    // check for clipping
    if ((height <= 0) || // bitmap is unexpectedly encoded in top-to-bottom pixel order
            ((width % 2) != 0) ||           // must be even number of columns
            ((xpos + width) > SCREENW) || // right side cut off
            (ypos < (height - 1)) ||      // top cut off
            (ypos > SCREENH))
    { // bottom cut off
        return;
    }
    if (threshold > 14)
    {
        threshold = 14;             // only full 'on' turns pixel on
    }
    // bitmaps are encoded backwards, so start at the bottom left corner of the image
    screeny = ypos / 8;
    screenx = xpos + SCREENW * screeny;
    mask = ypos % 8;                // row 0 to 7
    mask = 0x01 << mask;            // now stores a mask 0x01 to 0x80
    j = ptr[10];    // byte 10 contains the offset where image data can be found
    for (i = 1; i <= (width * height / 2); i = i + 1)
    {
        // the left pixel is in the upper 4 bits
        if (((ptr[j] >> 4) & 0xF) > threshold)
        {
            Screen[screenx] |= mask;
        }
        else
        {
            Screen[screenx] &= ~mask;
        }
        screenx = screenx + 1;
        // the right pixel is in the lower 4 bits
        if ((ptr[j] & 0xF) > threshold)
        {
            Screen[screenx] |= mask;
        }
        else
        {
            Screen[screenx] &= ~mask;
        }
        screenx = screenx + 1;
        j = j + 1;
        if ((i % (width / 2)) == 0)
        {     // at the end of a row
            if (mask > 0x01)
            {
                mask = mask >> 1;
            }
            else
            {
                mask = 0x80;
                screeny = screeny - 1;
            }
            screenx = xpos + SCREENW * screeny;
            // bitmaps are 32-bit word aligned
            switch ((width / 2) % 4)
            {      // skip any padding
            case 0:
                j = j + 0;
                break;
            case 1:
                j = j + 3;
                break;
            case 2:
                j = j + 2;
                break;
            case 3:
                j = j + 1;
                break;
            }
        }
    }
}
// There is a buffer in RAM that holds one screen
// This routine clears this buffer
void Nokia5110_ClearBuffer(void)
{
    int32_t i;
    for (i = 0; i < SCREENW * SCREENH / 8; i = i + 1)
    {
        Screen[i] = 0;              // clear buffer
    }
}

//********LCD_DisplayBuffer*****************
// Fill the whole screen by drawing a 48x84 screen image.
// inputs: none
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_DisplayBuffer(void)
{
    Nokia5110_DrawFullImage(Screen);
}
