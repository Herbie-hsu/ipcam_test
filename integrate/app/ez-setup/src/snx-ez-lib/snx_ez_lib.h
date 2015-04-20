#ifndef __SNX_LIBEZ_H__
#define __SNX_LIBEZ_H__

#define FILE_BUF_LEN		2048


#define GPIO_PIN_0		0
#define GPIO_PIN_1		1
#define GPIO_PIN_2		2
#define GPIO_PIN_3		3

#if WIFI_ENCRYPT == 0		// WEP
#define DEF_CONF_AP		"/root/etc_default/SNIP39/hostapd_wep.conf"
#elif WIFI_ENCRYPT == 1		// WPA
#define DEF_CONF_AP		"/root/etc_default/SNIP39/hostapd_wpa.conf"
#else				// WPA2
#define DEF_CONF_AP		"/root/etc_default/SNIP39/hostapd_wpa2.conf"
#endif  			// WIFI_ENCRYPT

#define DEF_CONF_STA		"/root/etc_default/SNIP39/wpa_supplicant.conf"

//#define DEF_CONF_DHCPD		"/root/etc_default/udhcpd_wpa2.conf"
#define DEF_CONF_DHCPD		"/root/etc_default/SNIP39/udhcpd.conf"

//#define DEF_CONF_DEF		"/root/etc_default/rst_default/default.conf"
#define DEF_CONF_DEF		"/root/etc_default/SNIP39/default.conf"

//#define CONF_DEF		"/etc/rst_default/default.conf"
#define CONF_DEF		"/etc/SNIP39/default.conf"


#if WIFI_ENCRYPT == 0		// WEP
#define CONF_AP			"/etc/SNIP39/hostapd_wep.conf"
#elif WIFI_ENCRYPT == 1		// WPA
#define CONF_AP			"/etc/SNIP39/hostapd_wpa.conf"
#else				// WPA2
#define CONF_AP			"/etc/SNIP39/hostapd_wpa2.conf"
#endif  // WIFI_ENCRYPT


#define CONF_STA		"/etc/SNIP39/wpa_supplicant.conf"

//#define CONF_DHCPD		"/etc/udhcpd_wpa2.conf"
#define CONF_DHCPD		"/etc/SNIP39/udhcpd.conf"

//#define SNIP39_UID_FILE		"/etc/SNIP39_UID.conf"
#define SNIP39_UID_FILE		"/etc/SNIP39/SNIP39_UID.conf"

#define SNIP39_VERSION_FILE	"/etc/SNIP39/SNIP39_VERSION.conf"
#define SNIP39_UIDBACKUP_FILE	"/etc/SNIP39/uidbackup.conf"

#define GALAXY_PID_FILE		"/var/run/sonix-proj.pid"
#define STREAM_PID_FILE		"/var/run/stream.pid"
#define EZ_PID_FILE		"/var/run/ez.pid"
#define RECORDER_PID_FILE	"/var/run/recorder.pid"

#define CAMERA_CONFIG_FILE "/etc/camera_config"
#define RESET_KEY_FLAG		"/etc/reset_key_flag"


#define WIFI_DEV		"wlan0"
#define ETH_DEV			"eth0"

#define DETECT_USB		"/proc/bus/usb/001/002"
#define DETECT_WIFI		"/proc/sys/net/ipv4/conf/"

#define CMD_COPY		"cp -f"

#define CMD_AP			"hostapd -B"
#define CMD_STA			"wpa_supplicant -B -c /etc/SNIP39/wpa_supplicant.conf -Dwext -i "
#define CMD_DHCPC		"udhcpc -i "
#define CMD_DHCPD		"udhcpd"


#define CMD_EZ			"snx_ez "
#define CMD_TUNNELING		"http-tunneling-serv "
#define CMD_PROJ		"sonix-proj "
#define CMD_TWOWAY		"twowayaudio "

#define	SN98600_WIFI_INFO		"/etc/SNIP39/wifi_info"

#define GFWVER "/usr/bin/gfwver"
#define FW_VERSION_SIZE "64"
#define FW_VERSION_SIZE_NUM  64

// status
#define OFF_LINE_STATUS     0
#define ON_LINE_STATUS      1
#define VIDEO_START_STATUS  2
#define VIDEO_STOP_STATUS   3


/* audio data */
#define START_PCM		"/etc/notify/start.pcm"
#define ONLINE_PCM		"/etc/notify/online.pcm"
#define OFFLINE_PCM		"/etc/notify/offline.pcm"
#define RESET_PCM		"/etc/notify/reset.pcm"
#define QRSCAN_PCM		"/etc/notify/qrcode.pcm"
#define VIEW_PCM		"/etc/notify/view.pcm"
#define RETRY_PCM		"/etc/notify/retry.pcm"
#define WIFI_FAIL_PCM		"/etc/notify/wifi_fail.pcm"

#define QUIT_TEMP_FILE 		"/var/run/quit_temp"

#define SNX_EZ_HTTP_BUF_LEN          2048
#define SNX_EZ_IPCAM_DAEMON_IP       "127.0.0.1"
#define SNX_EZ_IPCAM_DAEMON_PORT     52078


extern int	snx_gpio_open(int pin_number);
extern int	snx_gpio_write(int pin_number, int value);
extern int	snx_gpio_read(int pin_number, int *value);
extern int	snx_gpio_close(int pin_number);

extern void	encrypt_string(char *text, char *encryptText, int len);
extern void	decrypt_string(char *encryptText, char *text, int len);

extern int	snx_get_mode(void);
extern void	snx_set_mode(int ivalue);


extern int	snx_write_nvram(char *id, char *data);
extern int	snx_read_nvram(char *id, char *data, int len);
extern void	snx_read_uid();
extern void	snx_write_uid(char *uid);

extern void	snx_kill_ez();

extern void	snx_set_ssid(void);
extern void	snx_set_ssid(void);
extern void	snx_set_uid(char *uid);

extern char *base64_decode(const char *data, int data_len,int *len);


extern void snx_play_audio(char *src_audio);

extern void aes_cbc_encrypt(unsigned char* in, int inl, unsigned char *out, int* len, unsigned char * key);
extern void aes_cbc_decrypt(unsigned char* in, int inl, unsigned char *out, unsigned char *key);

extern int fileExists(const char* file);

extern int QRcode_check_flow(char *res_buf);

#endif //__SNX_LIBEZ_H__
