currentMode=long
currentSortModeArg=""
currentSortMode="file2dir"
initialDir=""

function setSortMode()
{
    case $1 in 
	file2dir)
		currentSortModeArg=""
	;;
	dir2file)
		currentSortModeArg="--reverse"
	;;
	*)
		currentSortModeArg=""
	;;
	esac
}

function listCurrentDirectory()
{

	case $1 in
		short)
		ls $currentSortModeArg
	;;
		long)
		ls -a $currentSortModeArg
	;;
		dironly)
		ls -d */
	;;
		ls_like)
		ls -l $currentSortModeArg
	;;
	esac
}

function fCopyFile()
{
 	read -p "Enter the source filename:" n1
      	read -p "Enter the destination filename:" n2
	cp -r $n1 $n2
}

function fChangeDir()
{
	read -p "Enter the directory name:" n1
	cd $n1
}

function fMoveDir()
{
     read -p "Enter the source filename:" n1
      read -p "Enter the destination:" n2
	mv $n1 $n2
}

function fDeleteFile()
{
      read -p "Enter the filename:" n1
	rm -r -f $n1
}

function fChangeShowMode()
{
      read -p "Enter the showmode:" n1
      echo "selected mode: $n1"
	currentMode=$n1
}

function fChangeSortMode()
{
	read -p "Choose sortmode:" n1
	setSortMode $n1
}

function fSetInitialDir()
{
	initialDir=$1
	if [ -z $initialDir ]
	then
		initialDir=pwd
	fi
	cd $initialDir
}

function fQuit()
{
	exit 0
}

while getopts "m:s:i:h:" opt; do
  case ${opt} in
    m )
	currentMode=$OPTARG
      ;;
    s )
	setSortMode $OPTARG
      ;;
    i )
	fSetInitialDir $OPTARG
       ;;
    \? ) echo "Usage: commands [-m{current display mode }] [-s {sort order}]"
      ;;
  esac
done

function printUserMenu()
{
	echo "1) copy"
	echo "2) change"
	echo "3) move"
	echo "4) delete"
	echo "5) showmode"
	echo "6) sortmode"
	echo "7) quit"
}

function handleUserChoise()
{
 	read -p ":" option
        case  $option  in
                1)
                      fCopyFile
                ;;
                2)
                      fChangeDir
                ;;
		3)
			fMoveDir
		;;
		4)
			fDeleteDir
		;;
		5)
			fChangeShowMode
		;;
		6)
			fChangeSortMode
		;;
		7)
			fQuit
		;;
                *)
                        echo "Invalid user option"
                ;;
        esac
}

function processUserApp()
{

while :
do
	listCurrentDirectory $currentMode
	printUserMenu
	handleUserChoise
done

}


processUserApp
