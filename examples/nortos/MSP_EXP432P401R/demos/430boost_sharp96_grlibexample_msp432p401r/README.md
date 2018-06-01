## Example Summary

This demo software shows how to use the MSP Graphics Library in a project with the Sharp 96x96 display. This demo introduces the functions to configure grlib such as initialization, color inversion, and using foreground and background colors properly.

The Sharp 96x96 display BoosterPack plug-in module does not support touch or color, it is a simple monochrome LCD. It is great for ultra-low power display applications and has a unique mirrored pixel display.

## BoosterPack Requirements

This demo requires the [Sharp ® Memory LCD BoosterPack (430BOOST-SHARP96)](http://www.ti.com/tool/430BOOST-SHARP96) paired with the MSP-EXP432P401R LaunchPad

## Example Usage

Upon running the demo, the display cycles through various screens without user interaction to show the following simple graphics primitive:

* Pixels
* Lines
* Circles
* Rectangles
* Text
* Images

## Application Design Details

* No-RTOS
  * Clock System is configured with MCLK = 12 MHz.
  * EUSCI_B0 is used for SPI communication with the LCD driver.
