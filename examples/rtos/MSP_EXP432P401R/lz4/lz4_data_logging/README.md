## Example Summary

This example is intended to demonstrate how to use the lz4 compression
library to sample and compress sensor data from the Sensors BoosterPack.
The NVS drivers are used to write the compressed data into the MSP432's
flash memory starting at address 0x3B000.

## Peripherals Exercised

* `Board_GPIO_LED0`    - LED0 toggles.
* `TMP007_SLAVE_ADDR`  - I2C slave address used to communicate with TMP007 sensor.
* `OPT3001_SLAVE_ADDR` - I2C slave address used to communicate with OPT3001 sensor.
* The Sensors BoosterPack (http://www.ti.com/tool/boostxl-sensors) was used. It has the TMP007 and OPT3001 sensors, along with several other nifty sensors.

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
 Homepage"), etc.) to the appropriate COM port.
 *The COM port can be determined via Device Manager in Windows or via
 `ls /dev/tty*` in Linux. The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

* Run the example. The example will begin reading data from the TMP007 and OPT3001 sensors
once per second and printing them to the console. Console output should resemble:
```
    1
    9/12/1958 - 12:00:00
    ObjTemp: 18.2187 C    64.7937 F    291.3687 K
    DieTemp: 26.2500 C    79.2500 F    299.3999 K
    Lux:     334.8799 lx
```

* After 32 samples have been read, the compress thread will wake up, compress the 32 samples,
and write them to flash memory on the MSP432. The sensor thread will then begin reading data again.

* This loop will execute forever, overwriting older sensor data in flash memory if it exceeds
the 16k bytes that is allocated for NVS.

## Application Design Details

This application uses two threads:


`sensor_thread` - performs the following actions:

1. Opens and initializes I2C and Display driver objects.

2. Initializes the RTC module with a start date and time for data logging.

3. Uses the I2C driver to get data from the TMP007 and OPT3001 sensors every second.

4. Prints sensor and logging data to console via UART.

5. Posts a semaphore after every 32 samples to initiate compression and storage of data.


`compress_thread` - performs the following actions:

1. Opens and initializes an NVS driver object.

2. Initializes lz4 compression parameters.

3. Uses the lz4 compression library to compress sensor data in 32-sample chunks.

4. Writes the compressed data to non-volatile storage (flash) starting at address 0x3B000.


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
