
function generateRandomValue()
{
    local minValue=$1
    local maxValue=$2
    local remainder=$(($maxValue + 1 - $minValue))
    local randomValue=$(($RANDOM % $remainder + $minValue ))

    echo $randomValue
}

result=$(generateRandomValue 1 6);
echo $result 