This project used a Particle, Inc. Electron microcontroller (https://docs.particle.io/electron/) to act as a modbus master for pulling data from a Campbell Scientific CR300 datalogger (https://www.campbellsci.com/cr300) connected to soil moisture sensors and infrared thermometers.

The Electron uses modbus protocol (https://en.wikipedia.org/wiki/Modbus) to ping the data logger for it's current readings, which are then sent to an online data dashboard hosted by Ubidots (https://ubidots.com/).

This enables a user to cost-effectively convert their CR300 datalogger into an internet of things (IoT) device.

Future work for this would be to update it with the Particle Boron, create a new PCB, and adapt the code to work with different/multiple sensors by writing a python code to auto-generate the C++ needed for proper modbus calls.

Please reach out if you have any questions: ansleybrown1337@gmail.com

Cheers,
AJ