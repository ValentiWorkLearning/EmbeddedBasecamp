#!/bin/bash

readonly RTC_ADDRESS=0x68
readonly RTC_TEMP_UPPER_BYTE=0x11
readonly RTC_TEMP_LOWER_BYTE=0x12

currentTimeHour=0
currentTimeMin=0
currentTimeSec=0
currentTemperature=0

function detectDevicesOnBus()
{

	echo "-------------Detecting i2c devices on the i2c bus 1----------------"
	i2cdetect -y 1
	echo "-------------Devices detection has been completed------------------"
}

function readCurrentTemperature()
{
	local tempUpper = i2cget -y 1 $(RTC_ADDRESS) $(RTC_TEMP_UPPER_BYTE)
	local tempLower = i2cget -y 1 $(RTC_ADDRESS) $(RTC_TEMP_LOWER_BYTE)
	echo "Temp upper:" $(tempUpper)
	echo "Temp lower:" $(tempLower)
}

detectDevicesOnBus
readCurrentTemperature
