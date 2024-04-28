# MIDI

The MIDI mode is only available when a computer is connected via USB to the Raspberry Pi Pico.

You only have two settings in this mode, the receive and transmit MIDI channels, ranging from 1 to 16.

Notes received on the selected Rx channel will be played on the output channels.
Velocity is ignored.

Any pressed note buttons are sent on the selected Tx channel.
