showHelp()
{
	echo "Place a directory name to delete it's content"
	echo "Enter -h or --help for show the help message"
}

removeFiles()
{
	fileList=$(find $1 -name '~*.*' -o -name '*.tmp' -o -name '-*' -o -name '_*.*')	

	for fileItem in $fileList; do
		echo $fileItem
		if [ "$2" != "test-mode" ]; then
			rm $fileItem
		fi
	done
}

if [ $# -eq 1 ]; then
	case $1 in 

	--help | -h)
		showHelp
	;;
	*)
		removeFiles $1 ""
		exit 0
	;;
	esac

else
	case $2 in
		--test | -t)
			removeFiles $1 'test-mode'
		;;
	*)
		removeFiles $1  ""
		;;
	esac
fi




