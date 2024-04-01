# LARS - Looping Automated Rhythm Station

![PCB](https://github.com/xythobuz/lars/actions/workflows/kicad.yml/badge.svg)
![Docs](https://github.com/xythobuz/lars/actions/workflows/docs.yml/badge.svg)
![STLs](https://github.com/xythobuz/lars/actions/workflows/scad.yml/badge.svg)

This is a simple drum machine / loopstation.
It's made for three hand-wound solenoids mounted to a tambourine.
It is controlled by a Raspberry Pi Pico on a custom PCB.

Please take a look at [the auto-generated documentation](https://xythobuz.github.io/lars/).

Also see [this blog post for some more context](https://www.xythobuz.de/lars.html).

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

For the first time, flash as usual using the mass storage bootloader (hold BOOTSEL while pluggin in the Pico).
After the firmware has been flashed once you can just use the included `flash.sh` to avoid having to hold the button.
Use `debug.sh` to open a serial console via USB.

## Documentation

You can find the [documentation here on GitHub pages](https://xythobuz.github.io/lars/).

The docs are built using [mdbook](https://github.com/rust-lang/mdBook), licensed as `MPL-2.0`.
Get the [latest release from GitHub](https://github.com/rust-lang/mdBook/releases) for a pre-built binary if you want to test changes to the docs locally.

    ./pcb/generate_plot.sh
    ./pcb2/generate_plot.sh
    ./3dprint/generate_stls.sh

    ./docs/generate_docs.sh serve

This will open your browser to a local development instance of the docs.

## Hardware Connections

These are the used pins on the first prototype.
Follow these if you want to build your own version without the included PCB design.

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

The docs are built using [mdbook](https://github.com/rust-lang/mdBook), licensed as `MPL-2.0`.

The PCB SVG files in the documentation are displayed using [svg-pan-zoom](https://github.com/bumbu/svg-pan-zoom), licensed as `BSD-2-Clause`.

The 3D PCB files in the documentation are displayed using [three.js](hhttps://github.com/mrdoob/three.js), licensed as `MIT`.

Some code in `docs/src/js/3d.js` is derived from [a blog post of Michal Jirků](https://wejn.org/2020/12/cracking-the-threejs-object-fitting-nut/).

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

The PCB designs use a [KiCad-RP-Pico based](https://github.com/ncarandini/KiCad-RP-Pico) symbol and footprint library, licensed as `CC-BY-SA 4.0`.

The `pcb2/extern/EC12E-200.STEP` 3D model for the rotary encoder comes directly from the manufacturer, [ALPS](https://tech.alpsalpine.com/e/products/detail/EC12E24204A8/).

The `pcb2/extern/OLED_128x64.stp` 3D model for the SSD1306 OLED display is made by [
Adrián San José Torices](https://grabcad.com/library/pantalla-oled-0-96-128x64-1).

The SSD1306 footprint for pcb2 is from [KiCAD Rookie](https://kicadrookie.blogspot.com/2022/06/ssd1306-i2c-096in-oled-display-kicad_86.html).

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
