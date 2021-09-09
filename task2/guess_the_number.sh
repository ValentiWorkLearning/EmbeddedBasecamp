
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

#"Random number is greater than cmd param"
#"Cmd number is greater than generated random"
#0

function checkEnteredValues()
{
    if [[ $randomNumber -gt $toCompareCmdNumber ]]
    then
        echo -1;
    elif [[ $randomNumber -lt $toCompareCmdNumber ]]
    then
        echo 1
    elif [[ $randomNumber -eq $toCompareCmdNumber ]]
    then
        echo 0
    fi
}

function checkNumberOfGuessings()
{
     if [ -z "$1" ]
     then
        echo "Enter the number of guessing"
        read numberOfGuessings
    else
        numberOfGuessings=$1
    fi
}

function processResultMessage()
{
    case $1 in
    -1 )
        echo "Random number is greater than cmd param"
    ;;
    1 )
        echo "Cmd number is greater than generated random"
    ;;
    0 )
        echo "Guessed right!"
    ;;
    esac
}

function playGameLoop()
{
    randomNumber=$(generateRandomValue 1 $upperRange);

    result=$(checkEnteredValues);
    processResultMessage $result
    attemptsCounter=1;

    while [[ $attemptsCounter -le $numberOfGuessings ]]; do
       echo "enter your new guess:"
       read toCompareCmdNumber;
       result=$(checkEnteredValues);
       processResultMessage $result
        if [ $result -eq 0 ]
        then
            return
        fi

       ((attemptsCounter=attemptsCounter+1))
    done
}
processEnteredUsersValue $1
checkAvailableUpperRange $2
checkNumberOfGuessings $3
playGameLoop


