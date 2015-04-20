#include <stdio.h>    // for printf()  
#include <stdlib.h>

#include <unistd.h>   // for pause()  
#include <signal.h>   // for signal()  
#include <string.h>   // for memset()  
#include <sys/time.h> // struct itimeral. setitimer()   
#include <sys/stat.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>

//#include "des.h"
#include "snx_ez_lib.h"

#define GPIO3_BLINK  			1
#define CAPTURE_WIDTH			640
#define CAPTURE_HEIGHT			480
#define CAPTURE_FPS			    30
#define VERSION_PATH_MAX_LEN    64
#define VENDOR_NAME_MAX_LEN     64
#define MODEL_NAME_MAX_LEN      64
#define PRODUCT_NAME_MAX_LEN    64

#define RESET_DEVICE			"/tmp/reset_device"
#define USE_IWLIST

static volatile int rst_def = 0;

//static volatile int proxy = 8;
static volatile int debug = 0;
static volatile int restart = 0;
static volatile int ez_stat = 0;	// 0: wating start, 1: ready -1: need restart
static volatile sig_atomic_t quitting = 0;
int m_bind_user_flag = 0;

//static volatile int test = 0;

/* refer to get_frame_isp.c */
int QR_Code_Scan_flow(char *rec_str);

//To create local socket and connect to 52078 port
int snx_ez_create_local_client_socket();

//To decide the file whether is existed or not
int snx_ez_file_exist(char *fname);

void snx_wifi_rdy(void)
{
	char sys_cmd[128];
	struct stat tst;

	// Check wifi device ready
	// stat("/proc/sys/net/ipv4/conf/wlan0", &tst)
	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"%s%s", DETECT_WIFI, WIFI_DEV);
	while(stat(sys_cmd, &tst) == -1) {
		sleep(1);
		printf("Waiting WIFi ready\n");
	}
}


static inline int  snx_start_sonix()
{
    char sys_cmd[128];
    int ret=0;

	// system("led.sh online");;// blue led (pwm2) on

    if(access("/root/etc_default/rescue", F_OK) == 0){
        system("qiwo_dl_fw.sh");
    }

    // CMD http-tunneling-serv
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", CMD_TUNNELING);
    ret = system(sys_cmd);
    // CMD sonix-proj
    //memset(sys_cmd, 0x0, 128);
    //sprintf(sys_cmd,"%s&", CMD_PROJ);
    //ret = system(sys_cmd);

    // gemtek stream
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "stream -c /etc/stream.conf -i wlan0 & sleep 3 && recorder ");
    ret = system(sys_cmd);

    // CMD twowayaudio
    //memset(sys_cmd,	0x0, 128);
    //sprintf(sys_cmd,"%s&", CMD_TWOWAY);
    //ret = system(sys_cmd);

    //CMD litedm_cli
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "/bin/litedm_cli -c /etc/ldm.conf > /dev/null ");
    ret = system(sys_cmd);

    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "ntpdate 0.uk.pool.ntp.org ");
    ret = system(sys_cmd);

    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "crontab /etc/crontab ");
    ret = system(sys_cmd);

    system("killall crond");
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "crond ");
    ret = system(sys_cmd);

    return ret;
}

void snx_restart_sonix(void)
{
    struct stat tst;
    char sys_cmd[128];

    printf("---------------------------------\n");
    printf("---------------------------------\n");
    printf(" %s\n",__func__);
    printf("---------------------------------\n");
    printf("---------------------------------\n");
    system ("killall litedm_cli");
    system ("killall ipcam_cli");
    system("rm -rf /tmp/p2p_status");
    system("killall stream");
    system("killall recorder");
    //system ("killall sonix-proj");
    //system ("killall twowayaudio");
    system ("killall http-tunneling-serv");
    //sleep(3);

	
    // detect USB WIFI modules
    // stat("/proc/bus/usb/001/002", &tst)
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s", DETECT_USB);

    if (stat(sys_cmd, &tst) != -1) {
        snx_start_sonix(WIFI_DEV);
    }
    else {
        printf("NON USB wifi\n");
        snx_start_sonix(ETH_DEV);
    }
}


static inline void snx_monitor(void)
{
        /*TODO
            To monitor the rtsp server of gemtek (name: stream)
        */
        //Marked by Chris Chris, 2014/08/25
	struct stat tst;
	char sys_cmd[128];
	FILE *pFile;
	int pid=0, len=0;
	pFile = fopen(STREAM_PID_FILE, "r");
	if(pFile == NULL) {
		fprintf(stderr, "[%s] open %s failed !!\n", __FUNCTION__, STREAM_PID_FILE);
        goto RESTART_STREAM;
	}

	len = fread(sys_cmd, sizeof(char), 20, pFile);
	pid = atoi(sys_cmd);
	fclose(pFile);

	sprintf(sys_cmd,"/proc/%d", pid);

	if (stat(sys_cmd, &tst) == -1) {
		printf(" restart !!!!!!\n");
//		printf("Can't find /var/run/sonix-proj.pid\n");
		//snx_restart_sonix();
        goto RESTART_STREAM;
	}

	pFile = fopen(RECORDER_PID_FILE, "r");
	if(pFile == NULL) {
		fprintf(stderr, "[%s] open %s failed !!\n", __FUNCTION__, RECORDER_PID_FILE);
        goto RESTART_RECORDER;
	}
	len = fread(sys_cmd, sizeof(char), 20, pFile);
	pid = atoi(sys_cmd);
	fclose(pFile);

	sprintf(sys_cmd,"/proc/%d", pid);
	if (stat(sys_cmd, &tst) == -1) {
		printf(" restart !!!!!!\n");
//		printf("Can't find /var/run/sonix-proj.pid\n");
		//snx_restart_sonix();
        goto RESTART_RECORDER;
	}
    return;

RESTART_STREAM:
	system("killall stream");
    system("killall recorder");
    system("rm -f /var/run/recorder.pid");
    system("rm -f /var/run/stream.pid");
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "stream -c /etc/stream.conf -i wlan0 & sleep 3 && recorder ");
    system(sys_cmd);
	return;


RESTART_RECORDER:
    system("killall recorder");
    system("rm -f /var/run/recorder.pid");
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s&", "recorder ");
    system(sys_cmd);
	return;
}


static inline void snx_set_sta(void)
{
    char sys_cmd[128];
    struct stat tst;
    int ret;

    // CMD "wpa_supplicant -B -c /etc/SNIP39/wpa_supplicant.conf -Dwext -i wlan0 "
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s%s", CMD_STA, WIFI_DEV);
    ret = system(sys_cmd);

    // Check wifi device ready
    snx_wifi_rdy();

    // CMD udhcpc -i wlan0
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"%s%s", CMD_DHCPC, WIFI_DEV);
    ret = system(sys_cmd);
}

static int do_reset(void)
{
	char cmd[64] = {0};
	char sys_cmd[128];
	sprintf(cmd,"touch %s", RESET_KEY_FLAG);
	system(cmd);
	system("rm -f /etc/bindok");
	system("rm -f /etc/onlineok");
	sprintf(cmd,"rm %s", CAMERA_CONFIG_FILE);
	system(cmd);
	printf ("reset to default setting\n");
	ez_stat = -1;
	rst_def=1;

	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"touch %s", QUIT_TEMP_FILE);
	system(sys_cmd);
	system("killall litedm_cli");
	system("killall ipcam_cli");
	system("rm -rf /tmp/p2p_status");
	system("killall stream");
    system("killall recorder");
	snx_kill_ez();
	//snx_rst_def();
	system("cp /root/etc_default/SNIP39/* /etc/SNIP39/");
}

static inline void snx_gpio_detect(void)
{
	int value = 0;
	int reset = 0;

	// GPIO reset to default
	snx_gpio_read (GPIO_PIN_2,&value);
	reset = fileExists(RESET_DEVICE);

	if ((value == 0) || (reset==0)) {
		printf ("reset bottom \n");

		if(reset==0) {
			system("rm /tmp/reset_device");
		}
		do_reset();
#if GPIO3_BLINK
		system("led.sh offline"); // (Green led)pwm2 start blink
#endif
		// play audio file for reset

		if(reset != 0)
			snx_play_audio(RESET_PCM);
		// CP /root/etc_default/ --> /etc/
		// Read NVRAM UID to /etc/SNIP39/SNIP39_UID.conf
		// Set /etc/SNIP39/SNIP39_UID.conf to /etc/SNIP39/hostap_XXX.conf

		printf ("reset\n");
	}	// 	if (value == 0)
}

void sigalrm_fn(int sig)
{
    struct stat tst;
    int ret;
    char sys_cmd[128];
    // printf("<<%s>>\n", __func__);
    // gpio detect reset to default
    snx_gpio_detect();

    if(ez_stat == 1) {
        // monitor
        snx_monitor();
    }
    alarm(2);
}


void sigterm_fn(int sig)
{
    char sys_cmd[128];

    // printf("<<%s>>\n", __func__);
    snx_kill_ez();

    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"rm %s", GALAXY_PID_FILE);
    system(sys_cmd);
	
    ez_stat = -1;
    quitting = 1;

    // touch /var/run/quit_temp
    if(snx_get_mode() == 0) { // scan mode
        memset(sys_cmd, 0x0, 128);
        sprintf(sys_cmd,"touch %s", QUIT_TEMP_FILE);
        system(sys_cmd);
        while(fileExists(QUIT_TEMP_FILE) != 0) {
        }
    }

    // remove snx_ez pid
    memset(sys_cmd, 0x0, 128);
    sprintf(sys_cmd,"rm %s", EZ_PID_FILE);
    system(sys_cmd);
}

static void usage(FILE * fp, int argc, char ** argv) {
    fprintf(fp, "Usage: %s [options]/n\n"
        "Options:\n"
	"-h Print this message\n"
	"-d show debug message\n"
	"-r restart \n"
	"\n"
        "", argv[0]);
}



int get_wifi_info(char *buf,char *ssid,char *passwd,char *ts,char *qid)
{
    char *ssid_beg = strstr(buf,":S:");
    if(ssid_beg == NULL)
        return 0;
    sscanf(ssid_beg + strlen(":S:"),"%[^;]",ssid);
	
    char *passwd_beg = strstr(ssid_beg,";P:");
    if(passwd_beg == NULL)
        return 0;
    sscanf(passwd_beg + strlen(";P:"),"%[^;]",passwd);

    char *ts_beg = strstr(passwd_beg,";TS:");
    if(ts_beg == NULL)
        return 0;
    sscanf(ts_beg + strlen(";TS:"),"%[^;]",ts);

    char *qid_beg = strstr(ts_beg,";Q:");
    if(qid_beg == NULL)
        return 0;
    sscanf(qid_beg + strlen(";Q:"),"%[^;]",qid);
    return 1;
}

void wpaFileHandle(char	*name, const char *pass, int encrypt_type)
{
	FILE *pFile = NULL;
	char sys_cmd[128];

	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"%s", CONF_STA);

	pFile = fopen(sys_cmd, "w");
	if(pFile!=NULL) { 
		fprintf(pFile, "network={\n");
		fprintf(pFile, "ssid=\"%s\"\n", name);
		fprintf(pFile, "scan_ssid=1\n");
		fprintf(pFile, "pairwise=CCMP TKIP\n");
		fprintf(pFile, "group=CCMP TKIP WEP104 WEP40\n");
		if (strcmp(pass, "") == 0)	 // NONE 
			fprintf(pFile, "key_mgmt=NONE\n");
		else { 
			if(encrypt_type == 1)	// WPA/WPA2
				fprintf(pFile, "psk=\"%s\"\n", pass);

			else {// WEP 
				int len = strlen(pass);
				fprintf(pFile, "key_mgmt=NONE\n");
				//check HEX or String
				/*
				In WEP mode, there are 64bit/128bit/152bit key. Hence it mapping the length of available password
				is 5/13/16 in string based (with 24-bit random number). In Hex mode, it mapping the length of
				availabale password is 10/26/32. 
				
				example: Password of string based is apple, then the hex mode is 6170706C65.
				<String based>
				wep_key0 = "<string based password>"
				<Hex based>
				wep_key0 = <hex based password>
				*/ 
				if(len == 5 || len == 13 || len == 16) {
					// string 
					fprintf(pFile, "wep_key0=\"%s\"\n", pass);
				}
				else {
					// hex 
					fprintf(pFile, "wep_key0=%s\n", pass);
				}


			} 
		}
		fprintf(pFile, "}");
		fclose(pFile);
	}
	else { 
		fprintf(stderr, "[%s] open %s failed \n", __FUNCTION__, sys_cmd);
	}
}

static int
save_wifi_info(char* ssid, char* passwd, char* ts, char* qid)
{
        FILE *fp = NULL;
        char sys_cmd[128];
#ifdef USE_IWLIST
        int encrypt_type = -1;
#else
        int encrypt_type = 1;       // default only use WPA/WPA2
#endif

//     if( !ssid || !passwd || !ts || !qid ) {
        if( !ssid || !passwd) {
                fprintf(stderr, "[%s] invaild paramters! \n", __FUNCTION__);
                return -1;
        }
        
        // Check wifi device ready
        //snx_wifi_dev_rdy();
        
        system("ifconfig wlan0 up");
        
        
#ifdef USE_IWLIST
        // using iwlist to scan and check ssid
        encrypt_type = scan_wifi_info(ssid);
        if(encrypt_type == -1 ) {
                return -1;
        }
#endif
        /* stored information from QR/Audio tone */

        memset(sys_cmd, 0x0, 128);
        sprintf(sys_cmd,"%s", SN98600_WIFI_INFO);
        
        fp = fopen(sys_cmd,"wb");
        if(fp == NULL) {
                fprintf(stderr, "[%s] open %s failed!! \n", __FUNCTION__, sys_cmd);
                return -1;
        }
        else {
                fwrite(ssid,strlen(ssid),1,fp);
                fwrite("\r\n",2,1,fp);
                fwrite(passwd,strlen(passwd),1,fp);
                fwrite("\r\n",2,1,fp);
                fwrite(ts,strlen(ts),1,fp);
                fwrite("\r\n",2,1,fp);
                fwrite(qid,strlen(qid),1,fp);
                fclose(fp);
                printf("ssid:%s,passwd:%s\n",ssid,passwd);
                m_bind_user_flag = 1;
        }
    
        wpaFileHandle(ssid, (const char *)passwd, encrypt_type);  

        return 0;
}

int scan_wifi_info(char *scan_ssid)
{
        FILE *fp = NULL;
        char syscmd[128];
        char getvalue[256];
        char* name = NULL;
        int wpa_used = 0;
        int retry_count = 0;
        int end_check = 0;
        char m_ssid[96+1] = {'\0'}; 

        memset(syscmd, 0x00, sizeof(syscmd));
        if(!scan_ssid) {
                fprintf(stderr, "ssid is NULL!! \n");
                return -1;
        }
        
        //system_cmd_transform(scan_ssid, m_ssid, 32+1);

        while(retry_count < 3) {
        
                sprintf(syscmd, "iwlist wlan0 scan | grep -A20 \"%s\" > /tmp/ap_result", scan_ssid);
                system(syscmd);

                fp = fopen("/tmp/ap_result", "r");
                if(!fp) {
                        fprintf(stderr, "open %s failed!! \n", "/tmp/ap_result");
                        return -1;
                }
                
                // set beginning
            fseek(fp, 0, SEEK_SET);

                memset(getvalue, 0x00, sizeof(getvalue));
                fgets(getvalue, sizeof(getvalue), fp);    // Read next record

                /* verify SSID */
                name = strstr(getvalue,"ESSID:\"");
                if(!name) {
                        retry_count++;
                        continue;
                }
                else {
                        name += 7;
                        fprintf(stderr, "name = %s \n", name);
                        if(strncasecmp(scan_ssid, name, strlen(scan_ssid))) {
                                retry_count++;
                                continue;
                        }
                }

                // start to verifying the content
            while (1) {
                        memset(getvalue, 0x00, sizeof(getvalue));
                        
                        fgets(getvalue, sizeof(getvalue), fp);    // Read next record

                        /* check the end of section */
                        name = strstr(getvalue,"Extra:fm=");
                        if(name) {
                                end_check = 1;
                                break;
                        }
                        else {
                                name = strstr(getvalue, "IE: ");
                                if(name) {
                                        fprintf(stderr, "name = %s \n", name);
                                        if(!strncasecmp("IE: WPA Version 1", name, strlen("IE: WPA Version 1"))) {
                                                wpa_used = 1;
                                        }
                                        else if(!strncasecmp("IE: IEEE 802.11i/WPA2 Version 1", name, strlen("IE: IEEE 802.11i/WPA2 Version 1"))) {
                                                wpa_used = 1;
                                        }
                                }
                        }

                        if(feof(fp)) {
                                break;
                        }
            }

                if(end_check)
                        break;
        }

        fclose(fp);

        if(!end_check) {
                fprintf(stderr, "Cannot scan the specified ssid %s or extract end of section \n", scan_ssid);
                return -1;
        }

        if(wpa_used) {
                fprintf(stderr, "Use WPA/WPA2 \n");
                return 1;  // use WPA/WPA2
        }
        else {
                fprintf(stderr, "Use WEP/NONE \n");
                return 0;          // use WEP
        }
}

int snx_ez_create_local_client_socket()
{
    int ret = 0, opt_val;
    char http_buf[SNX_EZ_HTTP_BUF_LEN] = {"\0"};

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) return sock;
    struct sockaddr_in serv_addr;
    long sock_arg;
    struct timeval timeout_tv;
    fd_set sock_set;
    socklen_t opt_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SNX_EZ_IPCAM_DAEMON_IP);
    serv_addr.sin_port = htons(SNX_EZ_IPCAM_DAEMON_PORT);

    //To set non-blocking mode
    sock_arg = fcntl(sock, F_GETFL, NULL);
    sock_arg |= O_NONBLOCK;
    fcntl(sock, F_SETFL, sock_arg);

    //To connect recorder server by non-blocking socket
    ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
       if (errno == EINPROGRESS)
       {
          timeout_tv.tv_sec = 1;
          timeout_tv.tv_usec = 0;
          FD_ZERO(&sock_set);
          FD_SET(sock, &sock_set);
          if (select(sock+1, NULL, &sock_set, NULL, &timeout_tv) > 0) {
             opt_len = sizeof(int);
             getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&opt_val), &opt_len);
             if (opt_val)
             {
                printf("Error in connection() %d - %s\n", opt_val, strerror(opt_val));
             }
             else
             {
                 ret = 0;
                 //printf("Connect success\n");
             }
          }
          else
          {
              printf("Timeout or error() %d - %s\n", opt_val, strerror(opt_val));
          }
       }
       else
       {
           printf("Error connecting %d - %s\n", errno, strerror(errno));
       }
    }

    if(ret == 0)
    {
        //To set blocking mode
        sock_arg = fcntl(sock, F_GETFL, NULL);
        sock_arg &= (~O_NONBLOCK);
        fcntl(sock, F_SETFL, sock_arg);

        //memcpy(http_buf, "12345", strlen("12345"));
        //To send the http data to recording server
        //ret = send(sock, http_buf, strlen(http_buf), 0);
        //printf("Sending data to cvr :%s\n, len:%d\n", http_buf, ret);

        return sock;
    }
    else
    {
        close(sock);
        return -1;
    }
}


static inline int snx_ez_scan(void)
{
	// Parser information from QR information 
	char ssid[32]={'\0'};	
	char passwd[32]={'\0'};	
	char ts[32]={'\0'};	
	char qid[32]= {'\0'};
	char data_buf[512];

	printf("System on Scan Mode\n");

#if GPIO3_BLINK
	system("led.sh offline"); // (Green led)pwm2 start blink
#endif    

	if(QRcode_check_flow(data_buf) == -1)
		return -1;
				
	if((strstr(data_buf,"WIFI"))&&(data_buf[0]=='W')) {
		snx_play_audio(QRSCAN_PCM);
		if(get_wifi_info(data_buf, ssid, passwd, ts, qid) == 0) {
			memset(ssid,0x0,sizeof(ssid));
			memset(passwd,0x0,sizeof(passwd));
			memset(ts,0x0,sizeof(ts));
			memset(qid,0x0,sizeof(qid));
			return -1;
		}
  	}
	else {
		return -1;
	}			

	if(save_wifi_info(ssid, passwd, ts, qid) == -1) {
		fprintf(stderr, "save_wifi_info failed!! \n");
		return -1;
	}
	return 0;
}

int snx_ez_file_exist(char *fname)
{
    if( access( fname, F_OK ) != -1 ) {
        return 0; // file exists
    }
    return -1; // file doesn't exist
}

//int main(void)
int main(int argc, char **argv)
{
	struct stat tst;
	int ret;
	char sys_cmd[128];
	char debug_cmd[32];
	int pid;
	struct sigaction sa;

	memset(sys_cmd, 0x0, 128);

	if(fileExists(EZ_PID_FILE)==0) {
		printf("snx_ez is running\n");
		exit(EXIT_SUCCESS);
		printf("snx_ez is running\n");
	}

	pid = getpid();
	sprintf(sys_cmd,"echo %d > %s", pid, EZ_PID_FILE);
	system(sys_cmd);

	for (;;) {
		int index;
		int c;
		c = getopt(argc, argv, "p:d:r:");
		if (-1 == c)
			break;
		switch (c) {
//			case 0: /* getopt_long() flag */
//				usage(stderr, argc, argv);
//				break;
			case 'h':
				usage(stdout, argc, argv);
				exit(EXIT_SUCCESS);

			case 'd':
				sscanf(optarg, "%d", &debug);
				break;
			case 'r':
				sscanf(optarg, "%d", &restart);
				break;

			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
		}
	}

	//To do fw upgrade if the fw is existed in SD card
	if(snx_ez_file_exist("/media/mmcblk0/FIRMWARE_QIWO_build1.bin") == 0)
	{
        system("fwburnonly /media/mmcblk0/FIRMWARE_QIWO_build1.bin");

        //To sleep 1 second and then reboot
        sleep(1);
        system("reboot");
	}

	//To copy /etc/ipcam_cli/ipcam.conf to /data/ipcam.conf if /data/ipcam.conf is not existed
	if(snx_ez_file_exist("/data/ipcam.conf") == -1)
	{
        system("cp /etc/ipcam_cli/ipcam.conf /data/ipcam.conf");
	}

    //For 5.0.0.8 or 5.0.0.9 version, check vendor_name, model_name, product_name
    FILE *fp_version;
    char version[VERSION_PATH_MAX_LEN];
    fp_version = fopen("/etc/SNIP39/SNIP39_VERSION.conf", "rb");
    if (!fp_version) {
        printf("Cannot open file\n");
        return -1;
    }
    fread(version, VERSION_PATH_MAX_LEN, 1, fp_version);
    if ((strstr(version, "fw_hw04_Qiwo_5.0.0.8_1_US") != NULL) || (strstr(version, "fw_hw04_Qiwo_5.0.0.9_1_US") != NULL) || (strstr(version, "fw_hw04_Qiwo_5.0.0.10_1_US"))) {
        system("getenv vendor_name > /tmp/vendor_name");
        system("getenv model_name > /tmp/model_name");
        system("getenv product_name > /tmp/product_name");
        FILE *fp1, *fp2, *fp3;
        char vendor_name[VENDOR_NAME_MAX_LEN], model_name[MODEL_NAME_MAX_LEN], product_name[PRODUCT_NAME_MAX_LEN];
        fp1 = fopen("/tmp/vendor_name", "rb");
        fp2 = fopen("/tmp/model_name", "rb");
        fp3 = fopen("/tmp/product_name", "rb");
        if (!fp1) {
            printf("Cannot open file\n");
            return -1;
        }
        fread(vendor_name, VENDOR_NAME_MAX_LEN, 1, fp1);
        if (strstr(vendor_name, "vendor_name=QIWO") == NULL) {
            system("saveenv vendor_name=QIWO");
            printf("saveenv vendor_name=QIWO");
        }
        if (!fp2) {
            printf("Cannot open file\n");
            return -1;
        }
        fread(model_name, MODEL_NAME_MAX_LEN, 1, fp2);
        if (strstr(model_name, "model_name=D302") == NULL) {
            system("saveenv model_name=D302");
            printf("saveenv model_name=D302");
        }
        if (!fp3) {
            printf("Cannot open file\n");
            return -1;
        }
        fread(product_name, PRODUCT_NAME_MAX_LEN, 1, fp3);
        if (strstr(product_name, "product_name=Qiwo_Cam") == NULL) {
            system("saveenv product_name=Qiwo_Cam");
            printf("saveenv product_name=Qiwo_Cam");
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
    }
    fclose(fp_version);

	// Bootup init UID from NVRAM
	// Read NVRAM UID to /etc/SNIP39/SNIP39_UID.conf
	//snx_read_uid();
	// Set /etc/SNIP39/SNIP39_UID.conf to /etc/SNIP39/hostap_XXX.conf
	//snx_set_ssid();

	// GPIO init
	snx_gpio_open (GPIO_PIN_2);
	snx_gpio_write (GPIO_PIN_2,1);


	while(!quitting) {
		printf("start sonix ez monitor\n");

		ez_stat = 0;

		// set signal handlers
		signal(SIGALRM, sigalrm_fn);
		alarm(1);

		memset(&sa, 0, sizeof(struct sigaction));
		sa.sa_handler = sigterm_fn;
		if(sigaction(SIGTERM, &sa, NULL)) {
			printf("Failed to set SIGTERM handler. EXITING");
			quitting = 1;
			goto finally;
		}
		if(sigaction(SIGINT, &sa, NULL)) {
			printf("Failed to set SIGINT handler. EXITING");
			quitting = 1;
			goto finally;
		}

		// detect USB WIFI modules
		// stat("/proc/bus/usb/001/002", &tst)
		memset(sys_cmd, 0x0, 128);
		sprintf(sys_cmd,"%s", DETECT_USB);

		if (stat(sys_cmd, &tst) != -1) {
			// CMD ifconfig eth0 down
			memset(sys_cmd, 0x0, 128);
			sprintf(sys_cmd,"%s", "ifconfig eth0 down");
			ret = system(sys_cmd);

			if(snx_get_mode() == 1) { // STA mode
				printf("System on STA Mode\n");
				snx_set_sta();
			}
			else {	// Scan mode

				// Parser information from QR information
				if ( snx_ez_scan() == -1)
					goto finally;

				system("killall hostapd");
				system("killall udhcpd");

		                // CMD "wpa_supplicant -B -c /etc/SNIP39/wpa_supplicant.conf -Dwext -i wlan0 "
        		        memset(sys_cmd, 0x0, 128);
                		sprintf(sys_cmd,"%s%s", CMD_STA, WIFI_DEV);
               			 ret = system(sys_cmd);

  				 // Check wifi device ready
				snx_wifi_rdy();

				snx_set_mode(1);
				// CMD udhcpc -i wlan0
				memset(sys_cmd,	0x0, 128);
				sprintf(sys_cmd,"timeout.sh 15 \"%s%s\"", CMD_DHCPC, WIFI_DEV);
				ret = system(sys_cmd);
				if (ret != 0) {
					do_reset();
					snx_play_audio(WIFI_FAIL_PCM);
					printf("DHCP client can't get IP. Return to scan QR code!!!\n");
					continue;
				}

#if GPIO3_BLINK
				system("killall	gpio3_blink");
#endif
				
				fprintf(stderr,	"end to	scan!! \n");
			}
			if(ez_stat	== -1) {
				fprintf(stderr,	"xxxxxxxx ez_stat = -1 xxxxxxxx\n");
				goto finally;
			}
			
			snx_start_sonix();

		} // if	(stat(sys_cmd, &tst) !=	-1)
		else {
			printf("NON USB	wifi\n");

			if(snx_get_mode() == 1)	{ // STA mode
			}
			else {	// Scan mode
				if ( snx_ez_scan() == -1)
					goto finally;
				snx_set_mode(1);
#if GPIO3_BLINK
				system("killall	gpio3_blink");
#endif
				fprintf(stderr,	"end to	scan!! \n");
			}
			if(ez_stat	== -1) {
				fprintf(stderr,	"xxxxxxxx ez_stat = -1 xxxxxxxx\n");
				goto finally;
			}
			snx_start_sonix();

		} // else // if (stat(sys_cmd, &tst) != -1)
		ez_stat = 1;

finally:

        while(!quitting) {
            FILE *fp=NULL;
            fp = fopen("/tmp/p2p_status", "r");
            char buf_line[1024]={"\0"};
            char cloud_status[32]= {"\0"};
            char *result=NULL;

            if(fp != NULL)
            {
                while (fgets(buf_line, 1024, fp) != NULL)
                {
                    result = strtok(buf_line, "=");
                    if(result != NULL)
                    {
                        result = strtok(NULL, "\n"); //key value
                        if(result != NULL)
                        {
                            memcpy(cloud_status, result, strlen(result));
                        }
                    }
                    memset(buf_line, 0x00, 1024);
                }
                fclose(fp);
            }

            char event_buf[1024] = {"\0"};
            if(m_bind_user_flag == 1 && strncmp(cloud_status, "connect", strlen("connect")) == 0)
            {
                FILE *fp2=NULL;
                fp2 = fopen(SN98600_WIFI_INFO, "r");
                if(fp2 != NULL)
                {
                    memset(buf_line, 0x00, 1024);
                    int count1 = 0;
                    char user_id[64] = {"\0"};
                    while (fgets(buf_line, 1024, fp) != NULL)
                    {
                        if(count1 == 3)
                        {
                            memcpy(user_id, buf_line, strlen(buf_line));
                        }
                        memset(buf_line, 0x00, 1024);
                        count1++;
                    }
                    sprintf(event_buf, "{\"type\":\"ev-binduser\",\"user_id\":\"%s\",\"time\":\"1386760811\",\"tz\":\"+08\"}", user_id);
                    int cli_fd = snx_ez_create_local_client_socket();
                    send(cli_fd, event_buf, strlen(event_buf), 0);
                    m_bind_user_flag = 0;
                    fclose(fp2);
                    close(cli_fd);
                }
            }

            // printf("pause\n");
            pause();
            while(fileExists(QUIT_TEMP_FILE) == 0) {
                if(rst_def) {
                memset(sys_cmd, 0x0, 128);
                sprintf(sys_cmd,"rm %s", QUIT_TEMP_FILE);
                system(sys_cmd);
                break;
                }
            }

            if(rst_def) {
                rst_def =0;
                break;
            }
        } // while(1)
    } // while(1)
}
