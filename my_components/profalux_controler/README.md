# ESPHome component to manage 'Profalux' shutters

## Context
We have electric shutters, of the brand 'Profalux', and they radio-controled, each
with an individual remote control.

The signal uses the usual 868 MHz frequency, and the commands are quite simple, however
for security reasons each command starts with a rolling code value. It is not 
possible to reverse engineer this code system, thus the only way to integrate
these shutters to our Home Assistant network is to use the original remote controls
and to have them send the move commands.

The remote controls are composed of a PCB, a conductive pad for the buttons, and
a small plastic box. The PCBs fortunately are designed with vias to solder an
extenal relay and activate the control by an external system. There are 3 inputs
(one per command: up, stop, down) and one output which is the tension to be applied
on the inputs.

I checked that one just has to apply a 3V tension on any input to activate it,
with very little current. Thus it is not necessary to install relays, it is
possible to connect these inputs directly to some micro-controller outputs.

I chosed a Raspbery Pi Pico as the controler, because it is very small, has 
a version with Wifi (Pico W), uses very little current and can provide
a 3.3V powersupply with a reasonable current usage.

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
