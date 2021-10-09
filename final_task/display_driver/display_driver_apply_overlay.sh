## apply overlay
dtc spidev_disabled.dts -O dtb > spidev_disabled.dtbo
sudo dtoverlay -d . spidev_disabled