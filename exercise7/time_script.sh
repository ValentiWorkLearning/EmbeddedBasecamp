#!/bin/bash
#-x

readonly RTC_ADDRESS=0x68
readonly RTC_TEMP_UPPER_BYTE=0x11
readonly RTC_TEMP_LOWER_BYTE=0x12

currentTimeHour=0
currentTimeMin=0
currentTimeSec=0
currentTemperature=0

detectDevicesOnBus () {

	echo "-------------Detecting i2c devices on the i2c bus 1----------------"
	i2cdetect -y 1
	echo "-------------Devices detection has been completed------------------"
}

readCurrentTemperature () {
	tempUpper=`i2cget -y 1 $RTC_ADDRESS $RTC_TEMP_UPPER_BYTE`
	tempLower=`i2cget -y 1 $RTC_ADDRESS $RTC_TEMP_LOWER_BYTE`
	let tempFraction=$((tempLower>>6))
	let tempFractionConverted=0
	case "$tempFraction" in
		0 )
			tempFractionConverted="0"
		;;
		1 )
			tempFractionConverted="25"
		;;
		2 )
			tempFractionConverted="50"
		;;
		3 )
			tempFractionConverted="75"
		;;
	esac
	printf "Temperature is: %d.%s \n" $tempUpper $tempFractionConverted 
}

detectDevicesOnBus
readCurrentTemperature
