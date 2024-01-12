# ESPHome component to control up to 5 'Profalux' shutters

## Context
We have electric shutters, of the brand 'Profalux', and they are radio-controled, each
with an individual remote control.

The signal uses the usual 868 MHz frequency, and the commands are quite simple, however
for security reasons each command starts with a rolling code value. It is not 
possible to reverse engineer this code system, thus the only way to integrate
these shutters to our Home Assistant network is to use the original remote controls
and to have them send the move commands.

The remote controls are composed of a PCB, a conductive pad for the buttons, and
a small plastic box. The PCBs fortunately are designed with vias to solder an
external relay and activate the control by an external system. There are 3 inputs
(one per command: up, stop, down) and one output which is the tension to be applied
to the inputs.

I checked that one has simply to apply a 3V tension on any input to activate it,
with very little current. Thus it is not necessary to install relays, it is
possible to connect these inputs directly to some micro-controller outputs, using,
by security, a high enough resistance to limit the current.

I chosed a Raspbery Pi Pico as the controler, because it is very small, has 
a version with Wifi (Pico W), needs very little power (no cooling) and can provide
a 3.3V power supply for a reasonable current usage. The Pi has a bit specific
controler which is the RP2040, offers more than 20 I/O pins, and it is supported by ESPHome.

I designed a PCB to hold the Raspberry Pi and to plug several remotes. It is designed
for 5 remotes but one could add more, each one needs 3 outputs from the Raspberry Pi.
In order to easily plug each remote, I added on them a small angled header set. The remotes
are powered by a 3V battery (one for each) but the current used by one remote is ~50 mA
so I decided to power them from the Raspberry Pi instead. It means that I needed
a 5 pins connector on each remote control, one for 3.3V, one for the GND, and 3 for
the controls. So I repurposed the 4th via by cutting the track on the PCB, and I drilled
a new hole at 1/10" from the 4 others in order to install my 5 pins headers.

The 1st pin (the one which is drilled) is connected, with a soldered wire, to the ground of
the remote PCB. The 2nd one to the VCC to the battery connector. The last 3 are unchanged,
connected to the control inputs of the IC of the remote.

This has to be done for each remote control. It's not a hard task.

So at the end of the day, I get 5 repurposed remotes, a PCB as a "motherboard" where
I soldered the connectors for the Raspberry Pi Pico (2 x 20 pins) and for each remote (5 pins),
and where I also soldered a 10 kΩ resistor for each data line (3 x 5 for 5 remotes).

## TODO
Upload the PCB schema and the Gerber files, and pictures of the remotes PCB, and the 3D model
of the housing.

## Disclaimer
This is my first Github open-source project, my first ESPHome component, my first PCB design. Please be undestanding.
## Hardware needed
- one 'Profalux' radio remote control per shutter
- a Raspberry Pi Pico W
- a custom PCB (costs less that $10 for 5 units and shipping)
- some connectors
- 3 x 10 kΩ resistors per shutter.
- a 3D printed housing.
## Configuration
To configure the component, you must declare a "controler" who knows about the Raspberry Pi Pico
pins, can manage delays to swich on / off the output pins, etc., and one "cover" for each
shutter.

```yaml
# example configuration:

rp2040:
  board: rpipicow
  framework:
    # Required until https://github.com/platformio/platform-raspberrypi/pull/36 is merged
    platform_version: https://github.com/maxgerhardt/platform-raspberrypi.git

profalux_controler:
  id: pfc1
  controler-name: My Shutters Controler
  
cover:
  - platform: profalux_controler
    controler_id: pfc1
    blind_number: 1
    name: Blind 1 (bedroom)
  - platform: profalux_controler
    controler_id: pfc1
    blind_number: 2
    name: Blind 2 (living room)
```
