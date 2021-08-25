rsync -avz --rsync-path="sudo rsync" --delete pi@192.168.0.197:/lib rootfs
rsync -avz --rsync-path="sudo rsync" --delete pi@192.168.0.197:/usr/include rootfs/usr
rsync -avz --rsync-path="sudo rsync" --delete pi@192.168.0.197:/usr/lib rootfs/usr
rsync -avz --rsync-path="sudo rsync" --delete pi@192.168.0.197:/opt/vc rootfs/opt