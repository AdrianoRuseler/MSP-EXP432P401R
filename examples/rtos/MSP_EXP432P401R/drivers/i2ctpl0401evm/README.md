---
# i2ctpl0401evm

---

## Example Summary

Sample application to communicate with the I2C peripherals on a TPL0401EVM
Booster Pack.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicator LED
* `Board_GPIO_LED1` - Indicator LED
* `Board_I2C_TPL0401` - I2C used to communicate with I2C peripherals on
TPL0401EVM.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

This example was designed to use the TPL0401 Booster Pack (TPL0401EVM). The 
booster pack is required to successfully complete this example.

The I2C GPIO driver on the TPL0401 requires external current sourcing to power
the RGB LEDs.

## Example Usage

* Connect the TPL0401EVM Booster Pack before powering the hardware. You
__must__ connect the 5V pin of your LaunchPad to the 5V header pin on the top
left corner of the TPL0401EVM Booaster Pack.

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver
initialization is complete.

* The TLC59108 will read the color patterns in the rgbcmd variable and toggle
the RGB LEDs ON/OFF. `Board_GPIO_LED1` will turn ON if there has been a problem
with I2C communication.

## Application Design Details

This application uses one thread:

`mainThread` performs the following actions:

1. Opens and initializes an I2C driver object.

2. Uses the I2C driver to initialize the PWM oscillator on the TLC59108 and
set the LED outputs to PWM mode.

3. A RGBCMD structure (contains LED address and individual color values for
Red, Green and Blue) is read from the rgbcmd array and sent via I2C
driver to the TLC59108.

> If the end of the array is reached, the index is reset to 0 and the thread
sleeps for 100 milliseconds before starting the next iteration.

TI-RTOS:

* When building in Code Composer Studio, the kernel configuration project will
be imported along with the example. The kernel configuration project is
referenced by the example, so it will be built first. The "release" kernel
configuration is the default project used. It has many debug features disabled.
These feature include assert checking, logging and runtime stack checks. For a
detailed difference between the "release" and "debug" kernel configurations and
how to switch between them, please refer to the SimpleLink MCU SDK User's
Guide. The "release" and "debug" kernel configuration projects can be found
under &lt;SDK_INSTALL_DIR&gt;/kernel/tirtos/builds/&lt;BOARD&gt;/(release|debug)/(ccs|gcc).

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
