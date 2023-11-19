# Elli Energy Meter Modbus Sniffer

An esp8266 with an external EIA-422 converter sniffs the modbus communication between the Elli Wallbox Controller and the Energy Meter. With this esphome component you can integrate the powermeter for example to homeassistant.

Meter reading is only requested from Wallbox Controller if there is an active charging session.

Take a look at the picture and the example*.yaml code for more informations and hints.