### Install git
```shell
sudo apt-get install git
```

### Fetch the required linux version repo
```shell
git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable -b v5.14 --depth 1
```

### Export  build kernerl directory:
```shell
export BUILD_KERNEL=/home/valenti/build_kernel_x86
```

### Install the necessary tools for the build

```shell
sudo apt install make
sudo apt-get install libncurses-dev gawk flex bison openssl libssl-dev dkms libelf-dev libudev-dev libpci-dev libiberty-dev autoconf

```

### Run make for creating the default configuration
```shell
cd *your cloned linux sources with the linux_stable directory*
```

```shell
make O=${BUILD_KERNEL} i386_defconfig
cd ${BUILD_KERNEL}
make menuconfig
make -j4
```

### Get buildroot sources
```shell
git clone git://git.buildroot.net/buildroot
cd buildroot
```

### Prepare the rootfs sources
```shell
mkdir /home/valenti/rootfs_build
export BUILD_ROOTFS=/home/valenti/rootfs_build
make O=${BUILD_ROOTFS} qemu_x86_defconfig
cd ${BUILD_ROOTFS}
make menuconfig
```

### Correct target options
* Target Options
    * Target Architecture = i386
    * Target Architecture Variant = i686
* Toolchain
    * Custom kernel headers > 5.13
    * Enable WCHAR support
* System configuration
    * System hostname = `valentiKernel`
    * System Banner = `Welcome to ValentiLinux`
    * Enable root login with password
    * Root password = `<password>`
    * Path to the users' tables `${BUILD_ROOTFS}/users`
    * Root filesystem overlay directories = `${BUILD_ROOTFS}/root`
* Kernel
    * Empty option near the `Linux Kernel`

* Target packages:
    *   Show packages from busybox
    *   Development tools
        * binutils
        * binutils binaries
        * findutils
        * grep
        * sed
        * tree
    * Libraries
        * Compression and decompression
            *   zlib
        *   Text and terminal handling
            *   ncurses
            *   readline
        *   Networking applications
            * dropbear
            * wget
        * Shell and utilities
            * bash 
            * file
            * sudo
            * which
        * System tools
            * kmod
            * kmod utilities
            * rsyslog
        * Text editors and viewers
            * joe
            * less
            * mc
            * vim
            * nano
    * Filesystem images
        *   ext2/3/4
        * ext3? variant
        * tar the root filesystem

### Prepare additional folders for the non-root user
* Create user record with the user/pass credentials:

```shell
echo "user 1000 user 1000 =pass /home/user /bin/bash - Linux
User" > ${BUILD_ROOTFS}/users
```

* Add the ability to be promted to a superuser:
```shell
mkdir -p ${BUILD_ROOTFS}/root/etc/sudoers.d
echo "user ALL=(ALL)" > ${BUILD_ROOTFS}/root/etc/sudoers.d/user
```

* Add the shells

```shell
    mkdir -p ${BUILD_ROOTFS}/root/etc
    echo "/bin/sh" > ${BUILD_ROOTFS}/root/etc/shells
    echo "bin/bash" >> ${BUILD_ROOTFS}/root/etc/shells
```

### Launch the build

```shell
    make
```


### Errors
```shell
mkfs.ext2: No such file or directory while trying to determine filesystem size
*** Maybe you need to increase the filesystem size (BR2_TARGET_ROOTFS_EXT2_SIZE)
make[1]: *** [fs/ext2/ext2.mk:63: /home/valenti/rootfs_build/images/rootfs.ext2] Error 1
make: *** [Makefile:23: _all] Error 2

```
### Solution
- Navigate to the `${BUILD_ROOTFS}`
- `make menuconfig`
- `Filesystem Images -> extract size to 250M `

```shell
cd ${BUILD_ROOTFS}
make menuconfig
```


### Build artifacts will be available in:
`${BUILD_ROOTFS}/images/rootfs.ext2`

# QEMU:

### Installation qemu
```shell
apt-get install qemu
sudo apt install qemu-system-x86
```
### Launch QEMU from the cmd

Note: We are running qemu in background, but bound to current
terminal.

```shell
qemu-system-i386 \
-kernel ${BUILD_KERNEL}/arch/i386/boot/bzImage \
-append "root=/dev/sda console=ttyS0" \
-drive format=raw,file=${BUILD_ROOTFS}/images/rootfs.ext2 -nic user, \
hostfwd=tcp::8022-:22 &
```