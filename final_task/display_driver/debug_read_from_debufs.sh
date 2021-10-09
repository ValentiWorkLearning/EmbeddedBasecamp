echo "DMA Transfers:"
sudo cat /sys/kernel/debug/spi-bcm2835-20204000.spi/count_transfer_dma
echo "IRQ Transfers:"
sudo cat /sys/kernel/debug/spi-bcm2835-20204000.spi/count_transfer_irq
echo "POLLING"
sudo cat /sys/kernel/debug/spi-bcm2835-20204000.spi/count_transfer_polling
echo "IRQ AFTER POLLING"
sudo cat /sys/kernel/debug/spi-bcm2835-20204000.spi/count_transfer_irq_after_polling