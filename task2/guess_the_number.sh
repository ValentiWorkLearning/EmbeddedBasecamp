
function generateRandomValue()
{
    local minValue=$1
    local maxValue=$2
    local remainder=$(($maxValue + 1 - $minValue))
    local randomValue=$(($RANDOM % $remainder + $minValue ))

    echo $randomValue
}

randomNumber=$(generateRandomValue 1 6);
if [ -z "$1" ]
then
    echo "Enter the number please"
    read toCompareCmdNumber
else
    toCompareCmdNumber=$1
fi

if [[ $randomNumber -gt $toCompareCmdNumber ]]
then
    echo "Random number is greater than cmd param";
else
    echo "Cmd number is greater than generated random"
fi