# FinalQuestino

Basically a version of Dragon Warrior (Dragon Quest) ported to Arduino.

## Building With Arduino IDE

The FinalQuestino project can be built and uploaded using the [standard Arduino IDE](https://www.arduino.cc/en/software).

Almost all of the game data is generated from the "DataGenerator" project, which is a Visual Studio 2022 solution built on the .NET 6.0 platform. This project generates the `data_loader.c` file and automatically copies it to the FinalQuestino directory. If you don't plan on changing any of the game data, you can disregard this project entirely.

## Building With Visual Studio

Since it's extremely difficult (impossible?) to debug in any kind of useful way on an actual Arduino board, I've set up a Visual Studio 2022 project to make better tools available for development in Windows. This project is located in the `FinalQuestinoWinDev` folder.

## A Note About Coding Style

Some of the decisions in this repo about how exactly to render the game, store/load data, lay out structs, and other things may seem odd or overly complicated. Bear in mind that the Arduino board that I'm targeting has a ridiculously low 8 Kb of dynamic memory, and only 256 Kb of "program space" (basically hard drive space), and it's very difficult not to blast through both of those barriers on a regular bases. As a result, I've been following these guidelines:

- Only add a member to a struct if it's absolutely necessary.
- If you need to add a member to a struct, don't use larger types than necessary (don't use an int32 when an int16 will do, etc).
- When dealing with strings, use the `PSTR()` and `SPRINTF_P()` macros, it will use program storage instead of dynamic memory.
- Everything in `data_loader.c` should be packed as tightly as possible to save program storage.
- Use `#define`s where possible (instead of `enum`s, for example).
- Avoid anything at all C++-related, this will cause the C++ runtime library to load and use more memory.
- Basically anything possible to use less memory and/or program storage.

## Hardware

### Arduino Board

Although it might be possible to port it to some other board, this repo was built to work with the [Arduino Mega 2560 Rev3](https://store.arduino.cc/products/arduino-mega-2560-rev3). One major reason for this is that the board offers a relatively large 256k of program storage space, making it possible to load all the various maps and sprites without using an external SD card.

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
