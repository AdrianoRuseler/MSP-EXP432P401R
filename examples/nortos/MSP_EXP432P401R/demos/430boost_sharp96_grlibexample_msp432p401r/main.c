//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// main.c - GrlibExample
//
//****************************************************************************

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <ti/grlib/grlib.h>

#include "LcdDriver/Sharp96x96.h"
#include "images/images.h"

void clockInit(void);
void boardInit(void);
void timerInit(void);
void Delay(void);

/* TimerA UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        65535-1,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,
        TIMER_A_SKIP_CLEAR
};

Graphics_Context g_sContext;
Graphics_Rectangle g_sRect;
Graphics_Rectangle myRectangle1 = { 5, 15, 65, 45};
Graphics_Rectangle myRectangle2 = { 10, 40, 90, 90};
Graphics_Rectangle myRectangle3 = { 0, 0, 95, 95};
extern void TimerA1_0IsrHandler(void);
int main(void)
{
    volatile int i;
    WDT_A_holdTimer();

    for (i = 0; i <100;i++);

    boardInit();
    clockInit();
    timerInit();

    Interrupt_enableMaster();

    // Set up the LCD
    Sharp96x96_LCDInit();
    Graphics_initContext(&g_sContext, &g_sharp96x96LCD, &g_sharp96x96LCD_funcs);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
    Graphics_flushBuffer(&g_sContext);

    Delay();

    while(1)
    {
        // Intro Screen
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"MSP-EXP432P401R", GRAPHICS_AUTO_STRING_LENGTH, 48, 15, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"MSP432", GRAPHICS_AUTO_STRING_LENGTH, 48, 35, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext, (int8_t *)"Graphics Library", GRAPHICS_AUTO_STRING_LENGTH, 1, 51, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Example", GRAPHICS_AUTO_STRING_LENGTH, 48, 75, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_clearDisplay(&g_sContext);

        // Draw pixels and lines on the display
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Draw Pixels", GRAPHICS_AUTO_STRING_LENGTH, 48, 5, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"& Lines", GRAPHICS_AUTO_STRING_LENGTH, 48, 15, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawPixel(&g_sContext, 30, 30);
        Graphics_drawPixel(&g_sContext, 30, 32);
        Graphics_drawPixel(&g_sContext, 32, 32);
        Graphics_drawPixel(&g_sContext, 32, 30);
        Graphics_drawLine(&g_sContext, 35, 35, 90, 90);
        Graphics_drawLine(&g_sContext, 5, 80, 80, 20);
        Graphics_drawLine(&g_sContext, 0, Graphics_getDisplayHeight(&g_sContext) - 1, Graphics_getDisplayWidth(&g_sContext) - 1, Graphics_getDisplayHeight(&g_sContext) - 1);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_clearDisplay(&g_sContext);

        // Draw circles on the display
        Graphics_drawString(&g_sContext, (int8_t *)"Draw Circles", GRAPHICS_AUTO_STRING_LENGTH, 10, 5, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawCircle(&g_sContext, 30, 70, 20);
        Graphics_fillCircle(&g_sContext, 60, 50, 30);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_clearDisplay(&g_sContext);

        // Draw rectangles on the display
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Draw Rectangles", GRAPHICS_AUTO_STRING_LENGTH, 48, 5, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawRectangle(&g_sContext, &myRectangle1);
        Graphics_fillRectangle(&g_sContext, &myRectangle2);
        // Text below won't be visible on screen due to transparency (foreground colors match)
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Normal Text", GRAPHICS_AUTO_STRING_LENGTH, 50, 50, GRAPHICS_TRANSPARENT_TEXT);
        // Text below draws foreground and background for opacity
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Opaque Text", GRAPHICS_AUTO_STRING_LENGTH, 50, 65, GRAPHICS_OPAQUE_TEXT);
        Graphics_setForegroundColor(&g_sContext, ClrWhite);
        Graphics_setBackgroundColor(&g_sContext, ClrBlack);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Invert Text", GRAPHICS_AUTO_STRING_LENGTH, 50, 80, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_clearDisplay(&g_sContext);

        // Invert the foreground and background colors
        Graphics_setForegroundColor(&g_sContext, ClrBlack);
        Graphics_setBackgroundColor(&g_sContext, ClrWhite);
        Graphics_fillRectangle(&g_sContext, &myRectangle3);
        Graphics_setForegroundColor(&g_sContext, ClrWhite);
        Graphics_setBackgroundColor(&g_sContext, ClrBlack);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Invert Colors", GRAPHICS_AUTO_STRING_LENGTH, 48, 5, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawRectangle(&g_sContext, &myRectangle1);
        Graphics_fillRectangle(&g_sContext, &myRectangle2);
        // Text below won't be visible on screen due to transparency (foreground colors match)
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Normal Text", GRAPHICS_AUTO_STRING_LENGTH, 50, 50, GRAPHICS_TRANSPARENT_TEXT);
        // Text below draws foreground and background for opacity
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Opaque Text", GRAPHICS_AUTO_STRING_LENGTH, 50, 65, GRAPHICS_OPAQUE_TEXT);
        // Text below draws with inverted foreground color to become visible
        Graphics_setForegroundColor(&g_sContext, ClrBlack);
        Graphics_setBackgroundColor(&g_sContext, ClrWhite);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"Invert Text", GRAPHICS_AUTO_STRING_LENGTH, 50, 80, GRAPHICS_TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_setForegroundColor(&g_sContext, ClrBlack);
        Graphics_setBackgroundColor(&g_sContext, ClrWhite);
        Graphics_clearDisplay(&g_sContext);

        // Draw Images on the display
        Graphics_drawImage(&g_sContext, &LPRocket_96x37_1BPP_UNCOMP, 3, 28);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_clearDisplay(&g_sContext);
        Graphics_drawImage(&g_sContext, &TI_Logo_69x64_1BPP_UNCOMP, 15, 15);
        Graphics_flushBuffer(&g_sContext);
        Delay();
        Graphics_clearDisplay(&g_sContext);


    }

}

void clockInit(void)
{
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

}

void boardInit(void)
{
    // All GPIO output and low
    GPIO_setAsOutputPin(GPIO_PORT_P1,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P2,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P3,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P4,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P5,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,PIN_ALL8);

    GPIO_setAsOutputPin(GPIO_PORT_P6,PIN_ALL8);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6,PIN_ALL8);
}

void timerInit(void)
{
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    Timer_A_enableInterrupt(TIMER_A1_BASE);
    Interrupt_enableInterrupt(INT_TA1_0);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}


void Delay(void)
{
    volatile int i;
    for (i=0;i < 1700000 ; i ++);

}


//------------------------------------------------------------------------------
// Timer A0 Interrupt Service Routine
//------------------------------------------------------------------------------
void TA1_0_IRQHandler(void)
{
    Sharp96x96_SendToggleVCOMCommand();

    /* Add Offset to TACCR0 */
    Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,
                          Timer_A_getCaptureCompareCount(TIMER_A1_BASE,
                                  TIMER_A_CAPTURECOMPARE_REGISTER_0)+(100-1));

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
}

