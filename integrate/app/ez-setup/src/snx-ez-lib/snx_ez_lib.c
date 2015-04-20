#include <stdio.h>    // for printf()  
#include <stdlib.h>

#include <unistd.h>   // for pause()  
#include <signal.h>   // for signal()  
#include <string.h>   // for memset()  
#include <sys/time.h> // struct itimeral. setitimer()   
#include <sys/stat.h>

#include <getopt.h> 			/* getopt_long() */   
#include <fcntl.h>				/* low-level i/o */   
#include <pthread.h>
#include <openssl/evp.h>

#include <sys/mman.h>


#include "snx_ez_lib.h"
#include "nvram.h"
#include "des.h"
////////////////////////////////////////////


#define WIFI_TEMP_FILE 			"/tmp/wifi_temp"
#define DES_KEY				"a5b3t8s4"
#define QR_SCAN_MAX_RETRY_COUNT		3

#define NVRAM_ID    0x4dae7f6c

#define snx_debug(fmt, args...)		fprintf(stderr, "[<DBG><%s>]::" fmt, __func__, ## args)
#define snx_info(fmt, args...)		fprintf(stderr, "[<INFO>]::" fmt, ## args)
#define snx_error(fmt, args...)		fprintf(stderr, "[<ERR>]::" fmt, ## args)

typedef struct
{
	char Map_str[16];		//> input parameter string
	char NVRAM_CTRL_PARAM[64];	//> mapping NVRAM parameter
}NVRAM_Param;


#define PARAM_NUM	(sizeof(CFG_Param_Tbl)/sizeof(NVRAM_Param))


// ! Mapping NVRAM string table 
NVRAM_Param CFG_Param_Tbl[]=
{
	{"uid",		"UserRelated_UID"},
	{"ssid",	"UserRelated_SSID"},
	{"key",		"UserRelated_Password"},
	{"ip",		"NetRelated_IP"},
	{"mac",		"NetRelated_MAC"}
};

////////////////////////////////////////////

#define GPIO_SUCCESS 0
#define GPIO_FAIL    1
#define GPIO_UDELAY  500000

#define BUFFER_SIZE		63
char buf[BUFFER_SIZE];

#define PCM_PLAY "/usr/bin/pcm_play"

                                //0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
const char cipherTbl_LN_Enc[] = "8M9AV0aZ6zUoE5YNnBmTqr7sDpOu2tvCxbSyW4wcPFeLdGh1gIfHXQiJjR3kKl";
const char cipherTbl_LN_Des[] = "5lSwbD8M023HVOCfjpntyh1FQervYJA4aqE76XdigomksuxzIGBPKLNTRUcWZ9";


                                // !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
const char cipherTbl_SB_Enc[] = {'!', '$', '%', '*', ',', '-', '<', '#', '>', '@', 
                                 '\\', '\'', ' ', '&', ':', '^', '?', '/', '{', '"', 
                                 ')', '=', '(', '[', '|', '~', '+', ']', '.', '}', 
                                 '`', ';', '_'};
const char cipherTbl_SB_Des[] = {',', ' ', '=', '\'', '!', '"', '-', '+', '@', '>',
                                 '#', '^', '$', '%', '`', ';', '.', '}', '&', '?',
                                 '(', ':', ')', '[', '*', '_', '/', '~', '|', '<',
                                 '\\', '{', ']'};

#define ACCOUNT_BUF_LEN	        sizeof(account_info)
#define USRNM_AND_PWD_BUF_LEN   (sizeof(account_info) - sizeof(short))



int snx_gpio_open(int pin_number)
{
	int fd, len;
	memset (buf,0,BUFFER_SIZE);
	sprintf(buf, "echo %d > /sys/class/gpio/export", pin_number);
	system (buf);
	return GPIO_SUCCESS;
}


int snx_gpio_write(int pin_number, int value)
{
	int fd, len;
	memset (buf,0,BUFFER_SIZE);
	sprintf(buf, "echo out > /sys/class/gpio/gpio%d/direction", pin_number);
	system (buf);
	memset (buf,0,BUFFER_SIZE);
	sprintf(buf, "echo %d > /sys/class/gpio/gpio%d/value",value, pin_number);
	system (buf);


	return GPIO_SUCCESS;
}
int snx_gpio_read(int pin_number, int *value)
{
	int fd, len;
	memset (buf,0,BUFFER_SIZE);

	memset (buf,0,BUFFER_SIZE);
	sprintf(buf, "echo in > /sys/class/gpio/gpio%d/direction", pin_number);
	system (buf);

	sprintf(buf, "/sys/class/gpio/gpio%d/value", pin_number);
	fd = open(buf, O_RDWR);
	if(fd < 0)
		return GPIO_FAIL;

	read(fd, buf, BUFFER_SIZE);
	sscanf(buf, "%d", value);

	close(fd);

	return GPIO_SUCCESS;
}



int snx_gpio_close(int pin_number)
{
	int fd, len;
	memset (buf,0,BUFFER_SIZE);
	sprintf(buf, "echo %d > /sys/class/gpio/unexport", pin_number);
	system (buf);
	return GPIO_SUCCESS;
}

/*

static pthread_mutex_t mutex_mode = PTHREAD_MUTEX_INITIALIZER;
static int snx_gpio_write_tst(int pin_number, int value)
{
  pthread_mutex_lock(&mutex_mode);
	if (snx_gpio_open(pin_number) != GPIO_SUCCESS)
  {
    printf ("snx_gpio_open fail\n");
    return  GPIO_FAIL;
  }
//	fprintf(stderr, "write: %d\n",value);
	if (snx_gpio_write(pin_number, value) != GPIO_SUCCESS)
  {
    printf ("snx_gpio_write fail\n");
    return  GPIO_FAIL; 
  }

	if (snx_gpio_close(pin_number) != GPIO_SUCCESS)
  {
    printf ("snx_gpio_close fail\n");
    return  GPIO_FAIL; 
  }
  pthread_mutex_unlock(&mutex_mode);
	return GPIO_SUCCESS;
}
*/


void encrypt_string(char *text,	char *encryptText, int len)
{
    
	memset(encryptText, '\0', len);
    
	while (len) {
	if (*text >= 48	&& *text <= 57)	{
	    *encryptText = cipherTbl_LN_Enc[*text - 48];
	}
	else if	(*text >= 65 &&	*text <= 90) {
	    *encryptText = cipherTbl_LN_Enc[*text - 55];
	}
	else if	(*text >= 97 &&	*text <= 122) {
	    *encryptText = cipherTbl_LN_Enc[*text - 61];
	}
	else if	(*text >= 32 &&	*text <= 47) {
	    *encryptText = cipherTbl_SB_Enc[*text - 32];
	}
	else if	(*text >= 58 &&	*text <= 64) {
	    *encryptText = cipherTbl_SB_Enc[*text - 42];
	}
	else if	(*text >= 91 &&	*text <= 96) {
	    *encryptText = cipherTbl_SB_Enc[*text - 68];
	}
	else if	(*text >= 123 && *text <= 126) {
	    *encryptText = cipherTbl_SB_Enc[*text - 94];
	}
	
	//printf("be: %c, ae: %c\n", *text, *encryptText);

	text++;
	encryptText++;
	len--;
    }
}

void decrypt_string(char *encryptText, char *text, int len)
{
    memset(text, '\0', len);
	while (len) {
        	if (*encryptText >= 48 && *encryptText <= 57) {
			*text = cipherTbl_LN_Des[*encryptText - 48];
        	}
        	else if (*encryptText >= 65 && *encryptText <= 90) {
			*text = cipherTbl_LN_Des[*encryptText - 55];
		}
        	else if (*encryptText >= 97 && *encryptText <= 122) {
            		*text = cipherTbl_LN_Des[*encryptText - 61];
        	}
        	else if (*encryptText >= 32 && *encryptText<= 47) {
            		*text = cipherTbl_SB_Des[*encryptText - 32];
        	}
        	else if (*encryptText >= 58 && *encryptText <= 64) {
			*text = cipherTbl_SB_Des[*encryptText - 42];
        	}
        else if (*encryptText >= 91 && *encryptText <= 96) {
            *text = cipherTbl_SB_Des[*encryptText - 68];
        }
        else if (*encryptText >= 123 && *encryptText <= 126) {
            *text = cipherTbl_SB_Des[*encryptText - 94];
        }        
        //printf("be: %c, ae: %c\n", *encryptText, *text);

        text++;
        encryptText++;
        len--;
    }
}
////////////////////////////////////////////

// ! find out the mapping Nvram string 
static char* check_param(char *input_str)
{
	int i, exist_flag = 0 ;
	NVRAM_Param *pNvram_param = NULL;

	for (i=0; i<PARAM_NUM; i++) {
		pNvram_param = &CFG_Param_Tbl[i];

		if (!strcasecmp(input_str, pNvram_param->Map_str)){
			return pNvram_param->NVRAM_CTRL_PARAM;
		}
	}

	return NULL;
}

int snx_write_nvram(char *id, char *data)
{
	int ret;
	char *cfg_param = NULL;
	char *cfg_value = NULL;
	INFO *info_cur = NULL;
	size_t cfg_size = 0;

	ret = nvram_init(NVRAM_ID);
	if (ret < 0) {
		snx_error("nvram_init error %d!!\n", ret);
		return -1;
	}
//	snx_debug("Set Parameter %s\n", data);

	cfg_param = check_param(id);
	if(cfg_param==NULL) {
		nvram_close();
		snx_error("No matched Param %s!!\n", data);
		return -1;
	}

	info_cur = get_info(cfg_param);
	cfg_size = GET_INFO_SIZE(info_cur->misc);

	// ! alloce space to store 
	cfg_value = (char *)malloc(cfg_size+1);
	if(!cfg_value) {
		snx_error("malloc");
		nvram_close();
		return -1;
	}

	memset(cfg_value, 0x00, cfg_size+1);
	if(strlen(data)>cfg_size) {
		snx_error("over parameter range %d>%d!!!!\n", cfg_size, strlen(data));
		free(cfg_value);
		nvram_close();
		return -1;
	}
	else
		strncpy(cfg_value, data, strlen(data));

	ret = nvram_set(info_cur, cfg_value);
	nvram_commit_all();//sync to flash
	if(ret!=0) {
		snx_info("set %s failed\n",id);
		return -1;
	}
//	else 
//		snx_info("set %s successful\n",id);
		
	free(cfg_value);
	//nvram_commit_all();//sync to flash
	
	return 0;
}

////////////////////////////////////////////
//void snx_write_nvram(char *uid)
void snx_write_uid(char *uid)
{
	char sys_cmd[1024];
	char ssid[16];
	char ssid_key_gen[16];
	char ssid_key[16];
	
	// Set nvram UID
	snx_write_nvram("uid", uid);

	// Gen SSID
	strtok(uid, ":");
	memset(ssid, 0x0, 16);
	sprintf(ssid,"%s",uid);

	// Gen SSID_KEY
	memset(ssid_key_gen, 0x0, 16);
	sprintf(ssid_key_gen,"%s",uid+3);
	memset(ssid_key, 0x0, 16);
	encrypt_string(ssid_key_gen, ssid_key, 8);

	// Set nvram SSID
	snx_write_nvram("ssid", ssid);
	
	// Set nvram SSID_KEY
	snx_write_nvram("key", ssid_key);



	printf("uid		= %s\n",uid);
	printf("ssid		= %s\n",ssid);
	printf("ssid_key	= %s\n",ssid_key);
}

// Read NVRAM to File 
int snx_read_nvram(char *id, char *data, int len)
{
	int ret;
	char *cfg_param = NULL;
	INFO *info_cur = NULL;
	size_t cfg_size = 0;

//	snx_debug("Query %s \n", id);

	ret = nvram_init(NVRAM_ID);
	if (ret < 0) {
		snx_error("nvram_init error %d!!\n", ret);
		return -1;
	}

	cfg_param = check_param(id);

	if(cfg_param==NULL) {
		nvram_close();
		snx_error("No matched Param %s!!\n", id);
		return -1;
	}
	
	info_cur = get_info(cfg_param);
	cfg_size = GET_INFO_SIZE(info_cur->misc);

//	snx_debug("cfg_size=%d data=%d\n", cfg_size+1,len);
	if(cfg_size+1 > len) {
		snx_error("over parameter range %d>%d!!!!\n", cfg_size, len);
		nvram_close();
		return -1;
	}
	memset(data, 0x00, len);
	nvram_get(info_cur, data);
	nvram_close();
		
//	snx_info("%s :\n%s\n",id, data);
	return 0;
}
void snx_read_uid()
{
	char sys_cmd[1024];
	char uid[1024];

	// Read uid from NVRAM
	snx_read_nvram("uid", uid, sizeof(uid));
	// Set uid to /etc/SNIP39/SNIP39_UID.conf
	snx_set_uid(uid);
	
	snx_debug("uid:%s\n",strtok(uid, ":"));

}



void snx_set_uid(char *uid)
{
	FILE *pFile;
	int length;

	pFile = fopen(SNIP39_UID_FILE, "w");
	if(pFile!=NULL) {
		length = fwrite(uid, 1, strlen(uid), pFile);	
		fclose(pFile);
	}
	else
		fprintf(stderr, "[%s] open %s error\n", __FUNCTION__ , SNIP39_UID_FILE);
}

void snx_set_ssid(void)
{
	FILE *pFile;
	int length=FILE_BUF_LEN;
	char uid_buf[20];
	char pbuffer[FILE_BUF_LEN];
	char *uid;
	char ssid[32];
	char ssid_key[16];

	char *point, *point1;
	int ret;

	memset(ssid, '\0', 32);
	memset(ssid_key, '\0', 16);
	memset(uid_buf, '\0', 20);
	memset(pbuffer, '\0', FILE_BUF_LEN);


	// Get SNIP39 UID
	pFile = fopen(SNIP39_UID_FILE, "r");
	if(pFile==NULL) {
		fprintf(stderr, "[%s] open %s failed!! \n", __FUNCTION__ , SNIP39_UID_FILE);
		return;
	}
		
	length = fread(uid_buf, sizeof(char), 20, pFile);	
	fclose(pFile);

	uid = strtok(uid_buf, ":");
	sprintf(ssid,"%s",uid+3);
	encrypt_string(ssid, ssid_key, 8);

	memset(ssid, 0x20, 32);
	sprintf(ssid,"%s",uid);

	// Read /etc/SNIP39/hostap_XXX.conf
	pFile = fopen(CONF_AP, "rb+");
	if(pFile==NULL) {
		fprintf(stderr, "[%s] open %s failed!! \n", __FUNCTION__ , CONF_AP);
		return;
	}
	length = fread(pbuffer, sizeof(char), FILE_BUF_LEN, pFile);

	// Set /etc/SNIP39/hostap_XXX.conf SSID
	point = strstr(pbuffer, "ssid=");
	fseek(pFile, (point-pbuffer+strlen("ssid=")), SEEK_SET);
	ret = fwrite(ssid, 1, strlen(ssid), pFile);

#if WIFI_ENCRYPT == 0	// WEP
	// Set /etc/SNIP39/hostapd_wep.conf WEP_KEY
	point = strstr(pbuffer, "wep_key0=");
	fseek(pFile,(point-pbuffer+strlen("wep_key0=:")),SEEK_SET);
	fwrite(ssid_key, strlen(ssid_key), 1, pFile);

#elif WIFI_ENCRYPT == 1	// WPA
	// Set /etc/SNIP39/hostapd_wpa.conf WPA_KEY
	point = strstr(pbuffer, "wpa_passphrase=");
	fseek(pFile,(point-pbuffer+strlen("wpa_passphrase=")),SEEK_SET);
	fwrite(ssid_key, strlen(ssid_key), 1, pFile);

#else			// WPA2
	// Set /etc/SNIP39/hostapd_wep2.conf WPA2_KEY
	point = strstr(pbuffer, "wpa_passphrase=");
	fseek(pFile,(point-pbuffer+strlen("wpa_passphrase=")),SEEK_SET);
	fwrite(ssid_key, strlen(ssid_key), 1, pFile);

#endif  		// WIFI_ENCRYPT
	fclose(pFile);
	
}

// # AP(0),STA(1)
int snx_get_mode(void)
{
	FILE *pFile;
	int length;
	char* pbuffer;

	pFile = fopen(CONF_DEF, "rb");
	if(pFile==NULL) {
		fprintf(stderr, "[%s] open %s failed!! \n", __FUNCTION__ , CONF_DEF);
		return -1;
	}
	else {		
		fseek(pFile, 0, SEEK_END);   
		length = ftell(pFile);         
		rewind(pFile);          

		pbuffer = (char*)malloc(sizeof(char) * length + 1);
		int readlen = fread(pbuffer, sizeof(char), length, pFile);	
		pbuffer[readlen] = '\0';
		fclose(pFile);
	}
//	printf("%s\n",pbuffer);

	// select ap/sta mode ap(0), sta(1)
	if(strstr(pbuffer, "OperationMode=1")) { 
		return 1;	// STA mode
	}
	else {	
		return 0;	// AP mode
	}
}



// # AP(0),STA(1)
void snx_set_mode(int ivalue)
{
	FILE *pFile;
	int length;
	char* pbuffer;

	pFile = fopen(CONF_DEF, "rb+");
	if(pFile==NULL) {
		fprintf(stderr, "[%s] open %s failed!! \n", __FUNCTION__ , CONF_DEF);
		return;
	}
	else {		
		fseek(pFile, 0, SEEK_END);   
		length = ftell(pFile);         
		rewind(pFile);          

		pbuffer = (char*)malloc(sizeof(char) * length + 1);
		int readlen = fread(pbuffer, sizeof(char), length, pFile);	
		pbuffer[readlen] = '\0';

		fclose(pFile);
	}

	//if(argc == 2)	
	{
//		pbuffer[14] = '1';//*argv[1];
		pbuffer[14] = 48+ivalue;//'1';//*argv[1];
	}

	pFile = fopen(CONF_DEF, "wb+");
	if(pFile==NULL) {
		fprintf(stderr, "[%s] open %s failed!! \n", __FUNCTION__ , CONF_DEF);
		free(pbuffer);
		return;
	}

	fputs(pbuffer, pFile);
	fclose(pFile);
	
	free(pbuffer);
}

void snx_kill_ez()
{
	struct stat tst;
	int pid, ez_pid;
	char    data[4];
	FILE    *fp = NULL;
	char sys_cmd[128];
	int ret;

	pid = getpid();
	if((fp = fopen(EZ_PID_FILE, "r")) == NULL) { 
		fprintf(stderr, "[%s] open %s failed.\n", __FUNCTION__, EZ_PID_FILE );
		return;
	}
	fread(data, 4, 1, fp);
	ez_pid = atoi(data);	
	fclose(fp);
	if(pid != ez_pid) {	// from snx_rst

		memset(sys_cmd, 0x0, 128);
		sprintf(sys_cmd,"%s%s", "killall ", CMD_EZ);
		ret = system(sys_cmd);
	}
	else {
		// CMD snx_scan
		system("killall snx_scan");

		// CMD http-tunneling-serv
		memset(sys_cmd, 0x0, 128);
		sprintf(sys_cmd,"%s%s", "killall ", CMD_TUNNELING);
		ret = system(sys_cmd);

		// CMD sonix-proj
		memset(sys_cmd, 0x0, 128);
		sprintf(sys_cmd,"%s%s", "killall ", CMD_PROJ);
		ret = system(sys_cmd);
		// CMD twowayaudio
		memset(sys_cmd, 0x0, 128);
		sprintf(sys_cmd,"%s%s", "killall ", CMD_TWOWAY);
		ret = system(sys_cmd);
		
		system ("killall wpa_supplicant");
		system ("killall udhcpc");
//
//		system ("killall udhcpd");
//		system ("killall hostapd");
	}
}

void snx_rst_def()
{
	
	char sys_cmd[128];

	// cp -rf /root/etc_default/rst_default/* /etc/rst_default
	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"%s %s %s", CMD_COPY, DEF_CONF_DEF, CONF_DEF);
	system(sys_cmd);

	// cp -f /root/etc_default/SNIP39/hostapd.conf /etc/SNIP39/hostapd.conf
	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"%s %s %s", CMD_COPY, DEF_CONF_AP, CONF_AP);
	system(sys_cmd);

	// cp -f /root/etc_default/udhcpd.conf /etc/udhcpd.conf
	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"%s %s %s", CMD_COPY, DEF_CONF_DHCPD, CONF_DHCPD);
	system(sys_cmd);

	// cp -f /root/etc_default/wpa_supplicant.conf /etc/wpa_supplicant.conf
	memset(sys_cmd, 0x0, 128);
	sprintf(sys_cmd,"%s %s %s", CMD_COPY, DEF_CONF_STA, CONF_STA);
	system(sys_cmd);
	
	// Read NVRAM UID to /etc/SNIP39/SNIP39_UID.conf
	snx_read_uid();
	// Set /etc/SNIP39/SNIP39_UID.conf to /etc/SNIP39/hostap_XXX.conf
	snx_set_ssid();	
	system("sync");
}

const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";

static char find_pos(char ch)   
{ 
    char *ptr = (char*)strrchr(base, ch);//the last position (the only) in base[] 
    return (ptr - base); 
} 

char *base64_decode(const char *data, int data_len,int *len)
{
	int ret_len = (data_len / 4) * 3;
	int equal_count = 0;
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	int temp = 0;
	char need[3];
	int prepare = 0;
	int i = 0;
	if (*(data + data_len - 1) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 2) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 3) == '=')
	{//seems impossible
		equal_count += 1;
	}
	switch (equal_count)
	{
		case 0:
			ret_len += 4;//3 + 1 [1 for NULL]
			break;
		case 1:
			ret_len += 4;//Ceil((6*3)/8)+1
			break;
		case 2:
			ret_len += 3;//Ceil((6*2)/8)+1
			break;
		case 3:
			ret_len += 2;//Ceil((6*1)/8)+1
			break;
	}
	ret = (char *)malloc(ret_len);
	*len = ret_len;
	if (ret == NULL)
	{
		printf("No enough memory.\n");
		return NULL;
//		exit(0);
	}
	memset(ret, 0, ret_len);
	f = ret;
	while (tmp < (data_len - equal_count))
	{
		temp = 0;
		prepare = 0;
		memset(need, 0, 4);
		while (temp < 4)
		{
			if (tmp >= (data_len - equal_count))
			{
				break;
			}
			prepare = (prepare << 6) | (find_pos(data[tmp]));
			temp++;
			tmp++;
		}
		prepare = prepare << ((4-temp) * 6);
		for (i=0; i<3 ;i++ )
		{
			if (i == temp)
			{
				break;
			}
			*f = (char)((prepare>>((2-i)*8)) & 0xFF);
			f++;
		}
	}
	*f = '\0';
	return ret;
}

void snx_play_audio(char *src_audio)
{
	char cmd[64] = {0};
    system ("/bin/gpio_ms1 -n 7 -m 1 -v 1");
    usleep(35000); // dealy 35ms
    //system("/usr/bin/pcm_play  /etc/notify/xxx.pcm");
    sprintf(cmd, "%s %s",PCM_PLAY, src_audio);
	system(cmd);
    system ("/bin/gpio_ms1 -n 7 -m 1 -v 0");
}

void aes_cbc_encrypt(unsigned char* in, int inl, unsigned char *out, int* len, unsigned char * key){
	unsigned char iv[16];
	EVP_CIPHER_CTX ctx;
	int i;
	strncpy((char*)iv, key, 16);

	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);  

	*len = 0;
	int outl = 0;
	printf("1 inl==%d\n", inl);
	if((inl%16)==0) {
		for (i=0; i<16; ++i)
			in[inl+i] = 16;
		inl += 16;
	}

	EVP_EncryptUpdate(&ctx, out+*len, &outl, in+*len, inl);
   	*len+=outl;
   	int test = inl>>4;
   	if(inl != test<<4){
   		EVP_EncryptFinal_ex(&ctx,out+*len,&outl);  
	   	*len+=outl;
	}
	printf("2 len==%d\n", *len);

	EVP_CIPHER_CTX_cleanup(&ctx);

}


void aes_cbc_decrypt(unsigned char* in, int inl, unsigned char *out, unsigned char *key){
	unsigned char iv[10000];
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);

	strncpy((char*)iv, key, 16);

//	EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, key, iv); 
	EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv); 
	int len = 0;
	int outl = 0;

	EVP_DecryptUpdate(&ctx, out+len, &outl, in+len, inl);
   	len += outl;
   	 
   	EVP_DecryptFinal_ex(&ctx, out+len, &outl);  
   	len+=outl;
	out[len]=0;
	EVP_CIPHER_CTX_cleanup(&ctx);
}

int fileExists(const char* file) {
    struct stat buf;
	int ret = stat(file, &buf);
	//printf("fileExists = %d \n", ret);
    return ret;
}

/*
 *	return: 1: st58xxx chip, 0: not st58xxx chip
 */
#define	CHIP_VER_OFFSET		0x98000010
inline int is_58_series_chip(void)
{
	off_t page_offset;
	long page_size;
	unsigned int chip_ver;
	int retval = 0;
	int fd = -1;
	void *dev_mem = NULL;

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
		goto exit;
	page_size = sysconf(_SC_PAGE_SIZE);
	page_offset = CHIP_VER_OFFSET & (~(page_size - 1));
	dev_mem = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, page_offset);
	if(dev_mem < 0)
		goto exit;

	chip_ver = *((unsigned int*)(dev_mem + (CHIP_VER_OFFSET & (page_size - 1))));
//	printf("version = 0x%08x\n", chip_ver);
	chip_ver = (chip_ver & 0x000FF000) >> 12;
	if(chip_ver == 0x58)
		retval = 1;
	goto exit;

exit:
	if(dev_mem != NULL)
		munmap(dev_mem, page_size);
	if(fd > 0)
		close(fd);
	return retval;
}

int QRcode_check_flow(char *res_buf) 
{
	FILE *fp = NULL;
	char sys_cmd[128];
	char qr_enc_str[512];
	char *des_b64_decode_str = NULL, *result = NULL;
	int len_b64_str = 0, len_result = 0;
	int retry_count = 0;
	int ret = -1;

	if(!is_58_series_chip()) {
		fprintf(stderr, "!!!!!!\n");
		return -1;
	}
	
	if(res_buf==NULL) {
		fprintf(stderr, "Invaild data address!!\n");
		return -1;
	}
	
	while( retry_count < QR_SCAN_MAX_RETRY_COUNT ) {
		system("/bin/snx_scan &");
		while(fileExists(WIFI_TEMP_FILE) != 0) {
//			if(rst_def == 1|| quitting == 1) {

			if(fileExists(QUIT_TEMP_FILE)==0) {
				system("killall snx_scan");
				printf("Receive terminate QR scan siganl\n");

				memset(sys_cmd, 0x0, 128);
				sprintf(sys_cmd,"rm %s", QUIT_TEMP_FILE);
				system(sys_cmd);
				return -1;
			}
			printf("Waiting QR ....\n");
			sleep(1);
		}

		fp = fopen(WIFI_TEMP_FILE, "r");
		if(fp == NULL) {
			fprintf(stderr, "[%s] open %s failed!!\n", __FUNCTION__, WIFI_TEMP_FILE);
			continue;
		}
		else {
			fread(qr_enc_str, 512, 1, fp);
			fclose(fp);
		}

		/*! clear temp wif infor via qr scan */
		system("rm /tmp/wifi_temp");

		/*! do base 64 decode */
		des_b64_decode_str = base64_decode(qr_enc_str, strlen(qr_enc_str), &len_b64_str);
		if(des_b64_decode_str==NULL) {
			fprintf(stderr, "des_b64_decode_str non!!\n");
			retry_count++;
			continue;
		}

		/*! do des cbc decode */
		DES_CBC_Decode((unsigned char *)DES_KEY, strlen(DES_KEY), 
				(unsigned char*)DES_KEY, strlen(DES_KEY),
				(unsigned char *)des_b64_decode_str, len_b64_str,
				(unsigned char **)&result, (DWORD *)&len_result);

		free(des_b64_decode_str);
		
		if(result == NULL) {
			fprintf(stderr, "result non!!\n");
			retry_count++;
			continue;
		}
		else {
			fprintf(stderr, "Decode QR code Sucessfully !!\n");
			ret = 0;
			/*! reset to zero */
			memset(res_buf, 0x00, sizeof(res_buf));
			memcpy(res_buf, result, strlen(result));
			/*! clear des cbc */
			free(result);
			break;
		}
	}
	
	fprintf(stderr, "Show QR code decode string !!\n");
	fprintf(stderr, "%s \n", res_buf);
	return ret;
	
}

