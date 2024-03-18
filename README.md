# Drumkit

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

Flash as usual using the mass storage bootloader of RP2040.

## Hardware Connections

    Pin 1    GP0    I2C SDA
    Pin 2    GP1    I2C SCL

    Pin 4    GP2    Output Channel A
    Pin 5    GP3    Output Channel B
    Pin 6    GP4    Output Channel C

    Pin 7    GP5    Button 1
    Pin 9    GP6    Button 2
    Pin 10   GP7    Button 3
    Pin 11   GP8    Button 4

    Pin 12   GP9    Encoder A
    Pin 14   GP10   Encoder B
    Pin 15   GP11   Encoder Click

    Pin 16   GP12   LED 1
    Pin 17   GP13   LED 2
    Pin 19   GP14   LED 3
    Pin 20   GP15   LED 4
