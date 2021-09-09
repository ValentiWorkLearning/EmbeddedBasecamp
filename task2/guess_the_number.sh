
upperRangeLimit=100
lowerRangeMin=1
upperRange=0
gameStatus=0

# Generate the random number value from min to max range values
function generateRandomValue()
{
    local minValue=$1
    local maxValue=$2
    local remainder=$(($maxValue + 1 - $minValue))
    local randomValue=$(($RANDOM % $remainder + $minValue ))

    echo $randomValue
}

# Processing of the first guessed number
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

# Process the available upper range value
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

# Obtain the comparison result
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

# Check the number of guessings from the either cmd or the user input
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

# Obtain the message to user for a function return
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

# Main game loop playing
# Obtain the random number and eval all of the game functions flow
function playGameLoop()
{
    randomNumber=$(generateRandomValue 1 $upperRange);

    result=$(checkEnteredValues);
    processResultMessage $result

    if [ $result -eq 0 ]
        then
            gameStatus=0;
        return;
    fi

    attemptsCounter=1;

    while [[ $attemptsCounter -le $numberOfGuessings ]]; do
       echo "enter your new guess:"
       read toCompareCmdNumber;
       result=$(checkEnteredValues);
       processResultMessage $result
        if [ $result -eq 0 ]
        then
            gameStatus=0;
            return;
        fi

       ((attemptsCounter=attemptsCounter+1))
    done
    gameStatus=-1;
}
processEnteredUsersValue $1
checkAvailableUpperRange $2
checkNumberOfGuessings $3

playGameLoop
while [[ $gameStatus -eq 0 ]]; do
    echo "Play new game?";
    read playAgain

    if [[ $playAgain = "yes" ]]
    then
        playGameLoop
    else
        exit 0;
    fi

    if [[ $gameStatus -eq -1 ]]
    then
        exit -1;
    fi
done

