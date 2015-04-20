#!/bin/sh

/bin/gpio_ms1 -n 7 -m 1 -v 1
/usr/bin/pcm_play /etc/notify/sdcard_mount_successfully.pcm
sleep 4
/bin/gpio_ms1 -n 7 -m 1 -v 0

