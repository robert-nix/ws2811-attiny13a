# WS2811 driver for the ATTiny13a

This is a repo for my toying with an aliexpress-sourced, questionable-looking WS2811 LED strip.

My current setup:

- 12V 3+ Amp power supply for the LED power
- A 27 Ohm resistor between PB3 and the Din pin on the LED strip connector
- 5V for the ATTiny is from the USB-ASP programmer's Vcc.
- PB4 is not used in the source but I've got it hooked up to a status LED.
- Rest of the I/Os are used for the USB-ASP's MOSI/MISO/SClk/RST.

Careful attention must be paid to the protocol timing. Use `make disassemble` and count clock cycles if something breaks.
See "21. Instruction Set Summary" in [the datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny13A-Data-Sheet-DS40002307A.pdf).
