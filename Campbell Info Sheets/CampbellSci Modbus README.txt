Created by A.J. Brown 1/24/18

Campbell Scientific data loggers can be set up as modbus slaves and masters.
If the device is a slave, it will map registers starting at 0 and proceed to the "nth" registers.

The manual shows that the register map starts the first value at register 1, but this is not
correct for the modbus library that was used for the micro controllers (particle photon and electron)
in this project.  When using the Modbus Master library in combination with a CS data logger, make sure
the first register address is "0" and not "1" or it will cause an error in data retreival.

Data can be sent in multiple formats from the slave.  Each register in modbus can only accept up to
16 bit unsigned values, so if you are sending 32 bit numbers from the slave, it will be mapped
to two consecutive registers.