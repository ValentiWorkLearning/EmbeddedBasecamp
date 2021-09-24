#! /bin/bash
export BUILD_ROOTFS=/home/valenti/Development/Embedded/exercise8/rootfs_build/
export BUILD_KERNEL=/home/valenti/Development/Embedded/exercise8/kernel_build
qemu-system-i386 \
 -kernel ${BUILD_KERNEL}/arch/i386/boot/bzImage \
 -append "root=/dev/sda console=ttyS0" \
 -drive format=raw,file=${BUILD_ROOTFS}/images/rootfs.ext2 \
 -nic user,hostfwd=tcp::8022-:22 &