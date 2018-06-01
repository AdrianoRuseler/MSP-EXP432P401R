---
# watchdog

---

## Example Summary

This application demonstrates how to use the Watchdog driver to call a
function on expiration of the watchdog timer.

## Peripherals Exercised

* `Board_GPIO_LED0`      - Indicator LED
* `Board_GPIO_BUTTON0`   - Used to control the application
* `Board_WATCHDOG0` - Timer to trigger the watchdog interrupt service routine

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* The application turns ON `Board_GPIO_LED0` to indicate driver
initialization is complete.

* `Board_GPIO_LED0` is toggled repeatedly until `Board_GPIO_BUTTON0` is
pressed. A flag is toggled once `Board_GPIO_BUTTON0` is pushed. Once this
occurs, the watchdog timer will be continuously cleared and `Board_GPIO_LED0`
will stay lit.

## Application Design Details

* The application's `mainThread()` opens a Watchdog driver object. The
`watchdog` thread toggles a flag depending on whether `Board_GPIO_BUTTON0` has
been pushed. If the flag is set, the thread will continously call
`Watchdog_clear()` and set `Board_GPIO_LED0`. If the flag is not set, then the
timer will expire and trigger an interrupt to toggle `Board_GPIO_LED0`.

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
