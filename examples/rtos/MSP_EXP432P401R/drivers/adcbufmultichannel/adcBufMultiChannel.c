/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== adcBufContinuousSampling.c ========
 */
/* DriverLib Includes */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"

#define ADCBUFFERSIZE    (100)

uint16_t sampleBufferOne[ADCBUFFERSIZE];
uint16_t sampleBufferTwo[ADCBUFFERSIZE];
uint16_t sampleBufferThree[ADCBUFFERSIZE];
uint16_t sampleBufferFour[ADCBUFFERSIZE];
uint32_t microVoltBuffer[ADCBUFFERSIZE];
uint32_t buffersCompletedCounter = 0;
char uartTxBuffer[(10 * ADCBUFFERSIZE) + 25];

/* Driver handle shared between the task and the callback function */
UART_Handle uart;

/*
 * This function is called whenever a buffer is full.
 * The content of the buffer is then converted into human-readable format and
 * sent to the PC via UART.
 *
 */
void adcBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion,
    void *completedADCBuffer, uint32_t completedChannel) {
    uint_fast16_t uartTxBufferOffset;
    uint_fast16_t i;
    uint16_t *completedBuffer = (uint16_t *) completedADCBuffer;

    uartTxBufferOffset = sprintf(uartTxBuffer,
        "\r\nBuffer %u finished:\r\n", (unsigned int)buffersCompletedCounter++);

    for (i = 0; i < ADCBUFFERSIZE; i++) {
        uartTxBufferOffset += sprintf(uartTxBuffer + uartTxBufferOffset,
            "%u,", (unsigned int)completedBuffer[i]);
    }
    uartTxBuffer[uartTxBufferOffset] = '\n';
    /* Send out the data via UART */
    UART_write(uart, uartTxBuffer, uartTxBufferOffset + 1);
}

/*
 * Callback function to use the UART in callback mode. It does nothing.
 */

void uartCallback(UART_Handle handle, void *buf, size_t count) {
   return;
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    UART_Params uartParams;
    ADCBuf_Handle adcBuf;
    ADCBuf_Params adcBufParams;
    ADCBuf_Conversion continuousConversion[2];

    /* Call driver init functions */
    ADCBuf_init();
    UART_init();

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.writeCallback = uartCallback;
    uartParams.baudRate = 115200;
    uart = UART_open(Board_UART0, &uartParams);

    /* Set up an ADCBuf peripheral in ADCBuf_RECURRENCE_MODE_CONTINUOUS */
    ADCBuf_Params_init(&adcBufParams);
    adcBufParams.callbackFxn = adcBufCallback;
    adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_CONTINUOUS;
    adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
    adcBufParams.samplingFrequency = 200;
    adcBuf = ADCBuf_open(Board_ADCBUF0, &adcBufParams);

    /* Configure the conversion struct for two channels on same sequencer */
    continuousConversion[0].arg = NULL;
    continuousConversion[0].adcChannel = Board_ADCBUF0CHANNEL0;
    continuousConversion[0].sampleBuffer = sampleBufferOne;
    continuousConversion[0].sampleBufferTwo = sampleBufferTwo;
    continuousConversion[0].samplesRequestedCount = ADCBUFFERSIZE;

    continuousConversion[1].arg = NULL;
    continuousConversion[1].adcChannel = Board_ADCBUF0CHANNEL1;
    continuousConversion[1].sampleBuffer = sampleBufferThree;
    continuousConversion[1].sampleBufferTwo = sampleBufferFour;
    continuousConversion[1].samplesRequestedCount = ADCBUFFERSIZE;

    if (!adcBuf){
        /* AdcBuf did not open correctly. */
        while(1);
    }

    /* Start converting sequencer 0. */
    if (ADCBuf_convert(adcBuf, continuousConversion, 2) !=
        ADCBuf_STATUS_SUCCESS) {
        /* Did not start conversion process correctly. */
        while(1);
    }

    /*
     * Go to sleep in the foreground thread forever. The data will be collected
     * and transfered in the background thread
     */
    while(1) {
        sleep(1000);
    }

}
