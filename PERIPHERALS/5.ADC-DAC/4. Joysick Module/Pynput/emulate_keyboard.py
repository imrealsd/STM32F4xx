# use this program to recive serial data from serial port [pyserial module]
# and emulate keyboard press [pynput module]

from pynput.keyboard import Key, Controller
keyboard = Controller()
key = " "

import serial
serialPort = serial.Serial('/dev/ttyUSB0')  # open serial port
print(serialPort.name)         # check which port was really used
serialPort.write(b'hello')     # write a string
while(1):

    # Wait until there is data waiting in the serial buffer
    if(serialPort.in_waiting > 0):

        # Read data out of the buffer until a carraige return / new line is found
        serialString = serialPort.read(1)

        # Print the contents of the serial data
        print(serialString.decode('Ascii'))
        key = serialString.decode('Ascii')
        keyboard.press(key)
        keyboard.release(key)

        # Tell the device connected over the serial port that we recevied the data!
        # The b at the beginning is used to indicate bytes!
        #serialPort.write(b"Thank you for sending data \r\n")

ser.close()             # close port