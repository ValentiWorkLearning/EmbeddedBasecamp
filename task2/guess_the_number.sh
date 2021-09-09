
upperRangeLimit=100
lowerRangeMin=1
upperRange=0

function generateRandomValue()
{
    local minValue=$1
    local maxValue=$2
    local remainder=$(($maxValue + 1 - $minValue))
    local randomValue=$(($RANDOM % $remainder + $minValue ))

    echo $randomValue
}

function processEnteredUsersValue()
{
    if [ -z "$1" ]
    then
        echo "Enter the number please"
        read toCompareCmdNumber
    else
        toCompareCmdNumber=$1
    fi
}

function checkAvailableUpperRange()
{
    if [ -z "$1" ]
     then
        echo "Enter the upper range number please"
        read toCompareCmdNumber
    else
        toCompareCmdNumber=$1
    fi

    if [ $toCompareCmdNumber -le $upperRangeLimit ] && [ $toCompareCmdNumber -ge $lowerRangeMin ]
    then
        upperRange=$toCompareCmdNumber
    else
        upperRange=$upperRangeLimit
    fi
}

function checkEnteredValues()
{
    if [[ $randomNumber -gt $toCompareCmdNumber ]]
    then
        echo "Random number is greater than cmd param";
    else
        echo "Cmd number is greater than generated random"
    fi
}

processEnteredUsersValue $1
checkAvailableUpperRange $2


randomNumber=$(generateRandomValue 1 $upperRange);


checkEnteredValues