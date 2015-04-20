#!/bin/sh

usage()
{
	basename=`basename $0`;
	printf "Usage: $basename [OPTION]\n";
	printf "OPTION:{ fw_fail | offline | QR_code | wifi_setup | fw_download | fw_upgrade | start | online | watching }\n";
	printf "\t-h show help\n";
	printf "Example:\n";
	printf "\t./$basename fw_fail\n";
	exit;
}

turn0ff()
{
	gpio_led -n 3 -m 1 -v 0; gpio_led -n 1 -m 1 -v 0; gpio_led -n 0 -m 1 -v 0;
}

greenOn()
{
	gpio_led -n 0 -m 1 -v 1;
}

greenBlink()
{
	snx_pwm_period 0 500 1000;
}

greenfastBlink()
{
	snx_pwm_period 0 200 400;
}

blueOn()
{
	gpio_led -n 1 -m 1 -v 1;
}

blueBlink()
{
	snx_pwm_period 1 500 1000;
}

bluefastBlink()
{
	snx_pwm_period 1 200 400;
}

redOn()
{
	gpio_led -n 3 -m 1 -v 1;
}

turnOn()
{
	greenOn;
	blueOn;
	redOn;
}

main()
{	
	case $1 in
	"-h")
        usage;
        ;;
	"fw_fail")
        turn0ff;
		exit 0;
        ;;
	"QR_code")
		turn0ff;
		greenBlink;
		exit 0;
        ;;
	"offline")
		turn0ff;
		greenBlink;
		exit 0;
        ;;
	"wifi_setup")
		turn0ff;
		greenfastBlink;
		exit 0;
        ;;
	"fw_download")
		turn0ff;
		bluefastBlink;
		exit 0;
        ;;
	"fw_upgrade")
		turn0ff;
		redOn;
		exit 0;
        ;;
	"start")
		turn0ff;
		greenOn;
		exit 0;
        ;;		
	"online")
		turn0ff;
		blueOn;
		exit 0;
        ;;	
	"watching")
		turn0ff;
		blueBlink;
		exit 0;
        ;;			
	esac
    echo "Need param!!!"
    exit 0;
}

main $1;
