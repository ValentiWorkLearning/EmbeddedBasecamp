sudo insmod task_timer_module.ko
cat /proc/time_processing_module/absolute_time
sleep 3
cat /proc/time_processing_module/last_accessed
echo 1 > /sys/class/timer-mod-sys/display_mode
sleep 5
cat /proc/time_processing_module/last_accessed
echo 0 > /sys/class/timer-mod-sys/display_mode
sleep 2
cat /proc/time_processing_module/last_accessed
sudo rmmod task_timer_module
sleep 5
dmesg | tail -10