AVRI2CBootloader
================

I2C Bootloader for Atmega328P

Project consists of a few parts

masterLib/
  Library to communicate with a Atmega328p running this project's bootloader

slave/
  Bootloader executable that is flashed on the Atmega328p

slaveLib/
  Library for the Atmega328p's application to make use of the I2C bus
  and adhere to the communication protocol.

examples/master/
  i2cAVRFlash program that allows a application to be flashed on the Atmega328p
  once the bootloader binary has been flashed.

examples/slave/
  Example of how to create a program that adheres to the communication protocol
  and use the I2C bus for application registers.
