---
# portable

---

## Example Summary

This example is a multi-threaded application that uses multiple drivers.
It demonstrates how SimpleLink SDK examples can be portable across devices,
kernels, and compilers. This is accomplished by using TI Drivers and
POSIX APIs. This example is described in more detail in the _SDK User's Guide_
at &lt;SDK_INSTALL_DIR&gt;/docs/simplelink_mcu_sdk/Users_Guide.html.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicates that temperature has exceeded the alert
threshold
* `Board_GPIO_BUTTON1` - Used to restart the UART console
* `Board_UART0` - Used for a simple console
* `Board_I2C_TMP` - Used to read temperature
* For boards with an on-board TMP sensor (e.g. CC3220 Launchpads), that
sensor is used.
* For boards without an on-board TMP sensor, the Sensors Boosterpack
(http://www.ti.com/tool/boostxl-sensors) is needed. It has the TMP007 sensor
along with several other nifty sensors.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.


## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

The connection should have the following settings
```
    Baud-rate:  115200
    Data bits:       8
    Stop bits:       1
    Parity:       None
    Flow Control: None
```

* Run the example.

* The target displays a simple console on the serial session.
```
    Valid Commands
    --------------
    h: help
    q: quit and shutdown UART
    c: clear the screen
    t: display current temperature
```

* If you are using TMP006, the DIE temperature is displayed. You'll need to
change the temperature of the actual TMP006 sensor to see a change. If you
have a TMP007, the OBJECT temperature is displayed. To change this value,
simply have the sensor pointed at something with a different temperature (e.g.
the palm of your hand). Please note to keep the example short and sweet, the
TMP sensor was not calibrated.

* If you quit the console, the device will go into a lower power mode.
To start the console back up, hit `Board_GPIO_BUTTON1`.

## Application Design Details

* This example shows how to initialize the UART driver in blocking read
and write mode.

* A thread, `consoleThread`, is a simple console. When the UART is closed
the device can go into a lower power mode.

* A thread, `temperatureThread`, reads the temperature via I2C. The thread reads
the sensor every second. The timing is controlled by a POSIX timer and
semaphore. This approach was used to eliminate time drift (which would have
occurred with simply using sleep(1)).

* The temperatureMutex mutex is used to protect the reading/writing of the
temperature variables. The reading/writing of these two variables must be
done atomically. For example without the mutex, the temperature thread
(which is higher priority) could interrupt the reading of the variables
by the console thread. This could result with the console thread printing a
Celsius value that did not match up with the Fahrenheit value.

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

Code Composer Studio:

* When using in CCS for the CC3220 devices, it's recommended you do
a  "Free Run" instead of "Run". This is because when the CC3220 device goes
into low power mode, emulation is lost.

SimpleLink Sensor and Actuator Plugin:

* This example uses a TMP007 (or TMP006) sensor. It interfaces via the I2C
Driver. For a richer API set for the TMP sensors, please refer the SimpleLink
Sensor and Actuator Plugin. This plugin also offers a Button module to handle
debounce also.
