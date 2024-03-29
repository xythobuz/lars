# LARS - Looping Automated Rhythm Station

This is a simple drum machine / loopstation.
It's made for three hand-wound solenoids mounted to a tambourine.
It is controlled by a Raspberry Pi Pico on a custom PCB.

See [this blog post for some more context](https://www.xythobuz.de/lars.html).

## Quick Start

Initialize your local repo and prepare the build:

    git clone https://git.xythobuz.de/thomas/drumkit.git
    cd drumkit
    git submodule update --init
    cd pico-sdk
    git submodule update --init
    cd ..
    mkdir build
    cd build
    cmake ..
    cd ..

Then you can build new binaries like this:

    make -Cbuild -j4

Flash as usual using the mass storage bootloader (hold BOOTSEL while pluggin in the Pico).

## Hardware Connections

    Pin 1    GP0    I2C0 SDA
    Pin 2    GP1    I2C0 SCL

    Pin 29   GP22   Output Channel A
    Pin 31   GP26   Output Channel B
    Pin 32   GP27   Output Channel C

    Pin 34   GP28   Vbat (ADC2)

    Pin 11   GP8    Button 1
    Pin 12   GP9    Button 2
    Pin 16   GP12   Button 3
    Pin 19   GP14   Button 4

    Pin 22   GP17   Encoder A
    Pin 24   GP18   Encoder B
    Pin 21   GP16   Encoder Click

    Pin 14   GP10   LED 1
    Pin 15   GP11   LED 2
    Pin 17   GP13   LED 3
    Pin 20   GP15   LED 4

## License

The firmware of this project is licensed as GPLv3.
A copy of the license can be found in `COPYING`.

It uses the [Pi Pico SDK](https://github.com/raspberrypi/pico-sdk), licensed as BSD 3-clause, and therefore also [TinyUSB](https://github.com/hathach/tinyusb), licensed under the MIT license.

The code in `src/encoder.c` is derived from [mathertel/RotaryEncoder](https://github.com/mathertel/RotaryEncoder) and therefore licensed as BSD 3-clause.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    See <http://www.gnu.org/licenses/>.

The schematic and PCB design of this project is licensed under the [CERN Open Hardware Licence Version 2 - Strongly Reciprocal (CERN-OHL-S-2.0+)](https://ohwr.org/cern_ohl_s_v2.txt) or any later version.
A copy of the license can be found in `LICENSE`.

     ------------------------------------------------------------------------------
    | This source describes Open Hardware and is licensed under the CERN-OHL-S v2  |
    | or any later version.                                                        |
    |                                                                              |
    | You may redistribute and modify this source and make products using it under |
    | the terms of the CERN-OHL-S v2 (https://ohwr.org/cern_ohl_s_v2.txt)          |
    | or any later version.                                                        |
    |                                                                              |
    | This source is distributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,          |
    | INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A         |
    | PARTICULAR PURPOSE. Please see the CERN-OHL-S v2 (or any later version)      |
    | for applicable conditions.                                                   |
    |                                                                              |
    | Source location: https://git.xythobuz.de/thomas/drumkit                      |
    |                                                                              |
    | As per CERN-OHL-S v2 section 4, should You produce hardware based on this    |
    | source, You must where practicable maintain the Source Location visible      |
    | on the external case of the Gizmo or other products you make using this      |
    | source.                                                                      |
     ------------------------------------------------------------------------------
