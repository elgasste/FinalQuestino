# FinalQuestino

Basically a version of Dragon Warrior (Dragon Quest) ported to Arduino.

## Building

The FinalQuestino project can be built and uploaded using the [standard Arduino IDE](https://www.arduino.cc/en/software).

Almost all of the game data is generated from the "DataGenerator" project, which is a Visual Studio 2022 solution built on the .NET 6.0 platform. This project generates the `data_loader.c` file and automatically copies it to the FinalQuestino directory.

## Hardware

### Arduino Board

Although it might be possible to port it to some other board, this repo was built to work with the [Arduino Mega 2560 Rev3](https://store.arduino.cc/products/arduino-mega-2560-rev3). One major reason for this is that the board offers a relatively large 256k of program storage space, making it possible to load all the various maps without using an external SD card.

### LCD Screen

This repo targets the [ELEGOO UNO R3 2.8 Inch TFT Touch Screen](https://www.amazon.com/dp/B01EUVJYME), which has 320x240 resolution. In the interest of speed, it completely bypasses the libraries provided by Elegoo, and hard-codes as many drawing operations as possible. For that reason, if you plan to use a different screen, many significant changes will need to be made to both `screen.h` and `screen.c`.

### Controller

This repo was designed to work with a simple analog stick and two push-buttons (see below for wiring information), but the code running D-pad and button operations is quite simple, so it should be very easy to port to any other controllers you'd like to use.

## Wiring

My hardware is currently set up like so:

![PXL_20240318_013518974](https://github.com/elgasste/FinalQuestino/assets/2153171/02bb3b19-7daa-4d4c-acbd-8e34d5743897)
![PXL_20240318_013535405](https://github.com/elgasste/FinalQuestino/assets/2153171/3fdabdec-dd5e-48a9-92eb-1a1643d3590d)
![PXL_20240318_013615640](https://github.com/elgasste/FinalQuestino/assets/2153171/8f6a7f29-8d87-4fb0-8340-37285a80cfc3)
![PXL_20240318_013643310](https://github.com/elgasste/FinalQuestino/assets/2153171/a52caaed-0c87-47e4-a9d0-558a7654adc0)
![PXL_20240318_013704221](https://github.com/elgasste/FinalQuestino/assets/2153171/ce26b83a-014e-45c2-8cb7-5ac1a32c768e)
