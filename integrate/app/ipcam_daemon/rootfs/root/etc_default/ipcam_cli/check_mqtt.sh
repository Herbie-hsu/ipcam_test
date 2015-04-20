#!/bin/sh

HOSTS=`route | grep default | sed 's/ //g' | sed 's/default//g' | sed 's/0.0.0.0UG000wlan0//g'`
TIMEOUT=1
ping_time=1
mqtt_time=1
PINGCOUNT=1
MQTTCOUNT=1
SLEEP_SEC=1
PING_FAIL=1
MQTT_FAIL=1
error_count=0

while [ $mqtt_time -le 3 ]; do
    netstat -apn | grep "$1:$2" | grep "ESTABLISHED"
    if [ $? -ne 0 ]; then
        echo "MQTT disconnected"
        MQTT_FAIL=$(($MQTT_FAIL+1))
    fi
    sleep $SLEEP_SEC
    mqtt_time=$(($mqtt_time+1))
done

if [ $MQTT_FAIL -ge 3 ]; then
    echo "MQTT disconnect then check DHCP"
    while [ $ping_time -le 3 ]; do
        ping $HOSTS -c $PINGCOUNT -W $TIMEOUT #ping hosts, limit: 1sec=1000ms
        if [ $? -eq 0 ]; then #connection success
            echo "ping gateway success!"
        else
            echo "ping gateway fail"
            PING_FAIL=$(($PING_FAIL+1))
        fi
        sleep $SLEEP_SEC
        ping_time=$(($ping_time+1))
    done
    if [ $PING_FAIL -ge 3 ]; then
        echo "Ping GateWay fail reset DHCP"
        killall udhcpc
        udhcpc -i wlan0 &
    fi
    echo "Reset ipcam_cli"
    killall ipcam_cli
fi
