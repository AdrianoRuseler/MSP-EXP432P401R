---
# spiloopbackmultislave

---

## Example Summary

Application to demonstrate use of the SPI driver to create a simple external
loop-back with one SPI master that has multiple slaves controlled by GPIO driver
chip select (CS) pins.

## Peripherals Exercised

* `Board_GPIO_LED0` - Indicator LED
* `Board_SPI0` - SPI peripheral assigned as a Master
* `Board_SPI2` - SPI peripheral assigned as Slave1
* `Board_SPI3` - SPI peripheral assigned as Slave2
* `Board_SPI_CS1` - GPIO assigned as Chip Select pin for Slave1
* `Board_SPI_CS2` - GPIO assigned as Chip Select pin for Slave2

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Note: The master uses two GPIO pins for slave chip selects, with each chip select driving the corresponding slave's STE pin.
Please connect the Master pins to the corresponding Slave Pins as shown in the table below.

  |Master Pins|Slave Pins|
  |---|---|
  |`Board_SPI0` `CLK`|`Board_SPI2` `CLK`|
  |`Board_SPI0` `MOSI`|`Board_SPI2` `MOSI`|
  |`Board_SPI0` `MISO`|`Board_SPI2` `MISO`|
  |`Board_SPI_CS1`|`Board_SPI2` `STE`|
  |`Board_SPI0` `CLK`|`Board_SPI3` `CLK`|
  |`Board_SPI0` `MOSI`|`Board_SPI3` `MOSI`|
  |`Board_SPI0` `MISO`|`Board_SPI3` `MISO`|
  |`Board_SPI_CS2`|`Board_SPI3` `STE`|

## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

* Run the example. `Board_GPIO_LED0` turns ON to indicate driver
initialization is complete.

* Once initialized, the Slave SPI will send a message to the Master SPI.
The Master SPI will also send a message to the Slave SPI. After the transfer is
complete, the messages are printed via UART.

Messages should appear as follows:
```
        SPI slave1 initialized
        SPI slave2 initialized
        SPI master initialized
        Slave1: Hello, this is master SPI
        Master: Hello, this is slave1 SPI
        Slave2: Hello, this is master SPI
        Master: Hello, this is slave2 SPI
        Done
```

## Application Design Details

This application uses three threads:

`masterThread` - creates the Master SPI message and initiates the SPI transfer.
Runs at lowest priority, below slave1Thread and slave2Thread because slaves
must be ready before master begins.
`slave1Thread` - creates the Slave1 SPI message and waits for the Master to
start the SPI transaction. This thread runs at the highest priority, above
masterThread and slave2Thread. The Slave1 SPI must be ready before the Master
SPI starts the Slave1 transaction.
`slave2Thread` - creates the Slave2 SPI message and waits for the Master to
start the SPI transaction. This thread runs at mid priority, between the
masterThread and slave1Thread. The Slave2 SPI must be ready before the
Master SPI starts the Slave2 transaction.

The `masterThread` performs the following actions:

1. Opens and initializes a SPI driver object.

2. Creates a SPI transaction structure and sets txBuffer to `Hello, this is
master SPI`.

3. Transfers the message to Slave1. If the transfer is successful, the message
received from Slave1 SPI is printed. Otherwise, an error message is printed.

4. Transfers the message to Slave2. If the transfer is successful, the message
received from Slave2 SPI is printed. Otherwise, an error message is printed.

5. Closes the SPI driver object and terminates execution.

The `slave1Thread` and `slave2Thread` performs the same following actions,
difference being `slave1Thread` will do the first transaction while
`slave2Thread` will do the second transaction. The order is controlled by the
master that enters `slave1Thread` barrier first, then enters `slave2Thread`
barrier second:

1. Opens and initializes a SPI driver object.

2. Creates a SPI transaction structure and sets txBuffer to `Hello, this is
slave1 SPI` or `Hello, this is slave2 SPI` respectively.

3. Waits for the Master SPI before sending the message. If the transfer is
successful, the message received from the Master SPI is printed. Otherwise, an
error message is printed.

4. Closes the SPI driver object.

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
