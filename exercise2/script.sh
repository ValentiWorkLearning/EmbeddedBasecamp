filesList=$(find $1 -mtime +30)

fileRenamed=0

for fileItem in $filesList; do
	mv $fileItem "~$fileItem"
	$fileRenamed = 1
done

exit $fileRenamed
