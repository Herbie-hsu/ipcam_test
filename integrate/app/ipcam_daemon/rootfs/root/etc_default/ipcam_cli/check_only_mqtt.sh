#!/bin/sh

MQTT_TIME=0
MQTT_FAIL=0
OUTPUT=/tmp/mqtt_conn_ret.txt

netstat -apn | grep "$1:$2" | grep "ESTABLISHED"
if [ $? -ne 0 ]; then
    echo $(date) "MQTT disconnected"
    MQTT_FAIL=$(($MQTT_FAIL+1))
fi

if [ $MQTT_FAIL -ge 1 ]; then
    echo "0" > $OUTPUT
else
    echo "1" > $OUTPUT
fi
