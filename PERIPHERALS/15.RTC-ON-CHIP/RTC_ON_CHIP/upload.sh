#!/bin/bash

echo "STARTING UPLOAD PROCESS"
echo "----------------------------------------------------------------"
cd .pio/build/genericSTM32F407VET6
sudo su
dfu-util -a 0 -i 0 -s 0x08000000:leave -D firmware.bin
exit

echo "-----------------------------------------------------------------"
echo "FIRMWARE UPLOADED SUCCESSFULLY"