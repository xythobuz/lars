# LARS V2

This is the second iteration of the hardware.
The principle is the same as for the prototype, with the same chinese modules used.
Only the buttons have been changed, and there are now 9 of them, with 8 LEDs.
The battery is now mounted on the board.
It is planned to be used without a case.

You can [download the Gerber files to order PCBs online](./plot/fab_pcb2.zip).

**Warning:**
Revision 1 is missing diodes on the button matrix.
So when holding down three or more buttons simultaneously you get ghosting on other buttons.

## Bill of Materials

These are all the parts required to assemble the V2 PCB.

| ID | Type | Link |
| -- | ---- | ---- |
| U1 | Raspberry Pi Pico | [Reichelt.de](https://www.reichelt.de/raspberry-pi-pico-rp2040-cortex-m0-microusb-rasp-pi-pico-p295706.html?&nbc=1), [Amazon.de](https://www.amazon.de/Raspberry-Pi-Pico/dp/B09KVB8LVR) |
| U2 | TP4056 Charger Module | [Amazon.de](https://www.amazon.de/AZDelivery-%E2%AD%90%E2%AD%90%E2%AD%90%E2%AD%90%E2%AD%90-TP4056-Laderegler-Lithium/dp/B07D2G345P) |
| U3 - U5 | MOSFET Module N-Channel | [Amazon.de](https://www.amazon.de/Schaltersteuerplatine-Motorsteuerplatine-MOSFET-Triggerschalter-Reglersteuerplatine-Dual-Hochleistungs/dp/B0CBK7D1GD) |
| U6 - U8 | Step-Up Converter Module | [Amazon.de](https://www.amazon.de/Converter-Spannungswandler-LAOMAO-Netzteil-Kompatibel/dp/B0B932BR7V) |
| OLED1 | SSD1306 128x64 0.96" Display | [Amazon.de](https://www.amazon.de/AZDelivery-Display-Arduino-Raspberry-gratis/dp/B01L9GC470) |
| BT1 | Keystone 1042 | [Reichelt.de](https://www.reichelt.de/batteriehalter-fuer-1-18650-keystone-1042-p213369.html?&nbc=1) |
| S1 | APEM 5236AB | [Reichelt.de](https://www.reichelt.de/kippschalter-1a-250vac-1x-ein-ein-printanschluss-as-500apc-p4396.html?&nbc=1) |
| SW1 | ALPS EC12E24244 | [Reichelt.de](https://www.reichelt.de/drehimpulsegeber-24-impulse-24-rastungen-vertikal-stec12e08-p73923.html?&nbc=1) |
| - | Cap for encoder knob | [Reichelt.de](https://www.reichelt.de/potentiometerknopf-fuer-achse-6-4-5-mm-schwarz-knopf-10-150e-p73960.html) |
| SW2 - SW10 | DIP 6x6mm push button | [Reichelt.de](https://www.reichelt.de/kurzhubtaster-printmontage-1-schliesser-6-x-6-x-5-mm-dip-dts-62k-v-p360043.html?&nbc=1) |
| D1 - D4 | 1N5819 Schottky Diode | [Reichelt.de](https://www.reichelt.de/schottkydiode-40-v-1-a-do-41-1n-5819-p41850.html?&nbc=1) |
| D5 - D12 | LED 5mm | [Reichelt.de](https://www.reichelt.de/led-5mm-bedrahtet-rot-450-mcd-90--5603r1c-khb-a-p361958.html?&nbc=1) |
| R1 | Resistor 10k Ohm 0.1% Axial | [Reichelt.de](https://www.reichelt.de/widerstand-metallschicht-10-kohm-0207-0-6-w-0-1--mpr-10-0k-p12770.html?&nbc=1) |
| R2 | Resistor 18k Ohm 0.1% Axial | [Reichelt.de](https://www.reichelt.de/widerstand-metallfilm-18-kohm-0-1-0-6-w-tk25-0207-mpr-18-0k-p12837.html?&nbc=1) |
| R3 - R10 | Resistor 1k Ohm | [Reichelt.de](https://www.reichelt.de/widerstand-metallschicht-1-00-kohm-0207-0-6-w-1--metall-1-00k-p11403.html?&nbc=1) |
