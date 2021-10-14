#!/bin/bash
#-x

readonly RTC_ADDRESS=0x68

readonly RTC_TEMP_UPPER_BYTE=0x11
readonly RTC_TEMP_LOWER_BYTE=0x12

readonly RTC_SECONDS=0x00
readonly RTC_MINUTES=0x01
readonly RTC_HOURS=0x02
readonly RTC_WEEK_DAY=0x03
readonly RTC_MONTH=0x05
readonly RTC_YEAR=0x06

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

decodeFromBcd () {
	let value=$1
	let decPart=$((value >> 4))
	let remainingPart=$((value&0x0F))
	let conversionResult=$(($decPart*10+$remainingPart))
	return $conversionResult
}

readCurrentTime () {
	let currentSeconds=`i2cget -y 1 $RTC_ADDRESS $RTC_SECONDS`
	decodeFromBcd $currentSeconds
	currentSeconds=$?

	let currentMin=`i2cget -y 1 $RTC_ADDRESS $RTC_MINUTES`
	decodeFromBcd $currentMin
	currentMin=$?

	let currentHours=`i2cget -y 1 $RTC_ADDRESS $RTC_HOURS`
	decodeFromBcd $currentHours
	currentHours=$?

	let currentWeekday=`i2cget -y 1 $RTC_ADDRESS $RTC_WEEK_DAY`
	
	let currentMonth=`i2cget -y 1 $RTC_ADDRESS $RTC_MONTH`
	decodeFromBcd $currentMonth
	currentMonth=$?

	let currentYear=`i2cget -y 1 $RTC_ADDRESS $RTC_YEAR`
	decodeFromBcd $currentYear
	currentYear=$?

	printf "Current time is : %d/%d/%d  %d:%d:%d\n" $currentYear $currentMonth $currentWeekday $currentHours $currentMin $currentSeconds 
}
detectDevicesOnBus
readCurrentTemperature
readCurrentTime
