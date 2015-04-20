#!/bin/sh

led.sh wifi_setup
export `getenv model_name`
if [ "${model_name}" = "" ]; then
    led.sh fw_fail
    echo "Checksum Fail!"
    exit;
fi

API_URL="https://qiwo-dm.securepilot.com/dm/v1/fota/stable?model_name=${model_name}"

curl -k -X GET ${API_URL} > /tmp/dl.log
if [ ! "$?" = "0" ]; then
    led.sh fw_fail
    echo "API Fail!!!"
    exit;
fi
DL_URL=`grep "url" /tmp/dl.log | cut -d "\"" -f 4`
CKS=`grep "checksum" /tmp/dl.log | cut -d "\"" -f 4`
echo DL_URL=$DL_URL
echo CKS=$CKS
led.sh fw_download
curl -k -o /tmp/FIRMWARE.bin ${DL_URL}
if [ ! "$?" = "0" ]; then
    led.sh fw_fail
    echo "Download Fail!"
    exit;
fi
M5S=`md5sum /tmp/FIRMWARE.bin | cut -d " " -f 1`
echo M5S=$M5S

if [ ! -f /tmp/FIRMWARE.bin ]; then
    led.sh fw_fail
    echo "FIRMWARE is not exist!"
    exit;
elif [ "$CKS" = "${M5S}" ]; then
    led.sh fw_upgrade
    echo "Checksum OK!"
    echo "Writting $FILE to flash..."
    fwburnonly /tmp/FIRMWARE.bin
    reboot
else
    led.sh fw_fail
    echo "Checksum Fail!"
    exit;
fi

led.sh fw_fail
