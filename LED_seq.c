/*
 * LED_seq.c
 *
 *  Created on: 	13 June 2013
 *      Author: 	Ross Elliot
 *     Version:		1.1
 */

/********************************************************************************************
* VERSION HISTORY
********************************************************************************************
* v1.1 - 27 January 2014
* 	GPIO_DEVICE_ID definition updated to reflect new naming conventions in Vivado 2013.3
*		onwards.
*
*	v1.0 - 13 June 2013
*		First version created.
*******************************************************************************************/

/********************************************************************************************
 * This file contains an example of using the GPIO driver to provide communication between
 * the Zynq Processing System (PS) and the AXI GPIO block implemented in the Zynq Programmable
 * Logic (PL). The AXI GPIO is connected to the LEDs on the ZedBoard.
 *
 * The provided code demonstrates how to use the GPIO driver to write to the memory mapped AXI
 * GPIO block, which in turn controls the LEDs.
 ********************************************************************************************/

/* Include Files */
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"

/* Definitions */
#define GPIO_DEVICE_ID  XPAR_AXI_GPIO_0_DEVICE_ID	/* GPIO device that LEDs are connected to */
#define LED_DELAY 10000000							/* Software delay length */
#define LED_CHANNEL 1								/* GPIO port for LEDs */
#define NUM_LEDS 8                                  /* Number of LEDs connected */
#define printf xil_printf							/* smaller, optimized printf */

XGpio Gpio;											/* GPIO Device driver instance */

int LEDOutputExample(void)
{
    volatile int Delay;
    int Status;
    u32 led = 0x00000003; /* Initial LED value - XX000011 */
    int direction = 0;    /* 0 for forward, 1 for backward */

    /* GPIO driver initialization */
    Status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /* Set the direction for the LEDs to output. */
    XGpio_SetDataDirection(&Gpio, LED_CHANNEL, 0x00);

    /* Loop forever blinking the LED. */
    while (1) {
        /* Write output to the LEDs. */
        XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, led);

        /* Move the LED sequence forward or backward. */
        if (direction == 0) {
            /* Move forward: shift the LED value to the left. */
            if (led == (1 << (NUM_LEDS - 1)))
                direction = 1;
            else
                led <<= 1;
        } else {
            /* Move backward: shift the LED value to the right. */
            if (led == 0x00000003)
                direction = 0;
            else
                led >>= 1;
        }

        /* Wait for a small amount of time so that the LED blinking is visible. */
        for (Delay = 0; Delay < LED_DELAY; Delay++);
    }

    return XST_SUCCESS; /* Should be unreachable */
}

/* Main function. */
int main(void) {
    int Status;

    /* Execute the LED output. */
    Status = LEDOutputExample();
    if (Status != XST_SUCCESS) {
        xil_printf("GPIO output to the LEDs failed!\r\n");
    }

    return 0;
}
