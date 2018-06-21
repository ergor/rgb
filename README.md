# rgb
Atmega AVR RGB Controller via serial port.

A really simple project, conceived when I was looking to mod my PC case with RGB LED-strips. 
I discovered my motherboard had a header for RS232 serial comms.
The idea is to hook a small Atmega board inside the PC, leech off of a 12V supply rail,
and receive commands from the PC to change colors via the serial port.

### Target
- ATMega16 @ 16MHz for testing
- in the future also ATMega328 @ 10 MHz (internal oscillator) for actual mounting inside the PC

### Environment
- Eclipse CDT with AVR plugin.
- [AVR toolchain version 3.5.4](http://distribute.atmel.no/tools/opensource/Atmel-AVR-GNU-Toolchain/3.5.4/)

