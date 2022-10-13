#script for uploading firmware to stm32f407vet6 through usb cable [dfu bootloader mode]

#! /bin/bash
echo "Start uploading Firmware"
echo "----------------------------------------------------------------"
cd .pio/build/genericSTM32F407VET6
sudo dfu-util -a 0 -i 0 -s 0x08000000:leave -D firmware.bin

echo "-----------------------------------------------------------------"
echo "Firmware uploaded successfullly."