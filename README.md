
Serial Filter Design Description:

The incoming serial bits are stored in a 8 bit buffer through the use of UART Recieve peripheral. THe buffer is then used to configure the MASKx and VALUEx registers of filters. The acceptance filter then performs the message id check and is placed in the FIFO accordingly.

The following approach is coded in C suitable for ARM STM32 microcontrollers.

Approach:

1. For data reception the registers of UART Rx are configured with appropriate values and enabled.

2. The reciever block of the UART is enabled and also the RXNE bit which will indicate if the data byte is ready to be read from the data register.

3. The RXNE bit is cleared when the data byte is copied to the buffer. This process takes place through interrupts.

4. The buffer is passed to a callback function which populates the register value of the filter sensor from the data recieved.

5. Message ID check is performed and the values which match are passed to the FIFO. 



