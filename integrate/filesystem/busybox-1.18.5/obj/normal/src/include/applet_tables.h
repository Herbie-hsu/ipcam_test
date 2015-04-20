/* This is a generated file, don't edit */

#define NUM_APPLETS 155

const char applet_names[] ALIGN1 = ""
"[" "\0"
"[[" "\0"
"addgroup" "\0"
"adduser" "\0"
"arping" "\0"
"ash" "\0"
"basename" "\0"
"cat" "\0"
"chgrp" "\0"
"chmod" "\0"
"chown" "\0"
"chroot" "\0"
"clear" "\0"
"cp" "\0"
"crond" "\0"
"crontab" "\0"
"cut" "\0"
"date" "\0"
"dc" "\0"
"dd" "\0"
"delgroup" "\0"
"deluser" "\0"
"depmod" "\0"
"df" "\0"
"dhcprelay" "\0"
"diff" "\0"
"dirname" "\0"
"dmesg" "\0"
"dnsd" "\0"
"dnsdomainname" "\0"
"du" "\0"
"dumpleases" "\0"
"echo" "\0"
"egrep" "\0"
"env" "\0"
"expr" "\0"
"false" "\0"
"fdformat" "\0"
"fdisk" "\0"
"fgrep" "\0"
"find" "\0"
"flash_eraseall" "\0"
"free" "\0"
"fsync" "\0"
"ftpget" "\0"
"ftpput" "\0"
"getopt" "\0"
"getty" "\0"
"grep" "\0"
"halt" "\0"
"hd" "\0"
"head" "\0"
"hexdump" "\0"
"hostid" "\0"
"hostname" "\0"
"hwclock" "\0"
"id" "\0"
"ifconfig" "\0"
"ifdown" "\0"
"ifup" "\0"
"inetd" "\0"
"init" "\0"
"insmod" "\0"
"install" "\0"
"ipcs" "\0"
"kill" "\0"
"killall" "\0"
"klogd" "\0"
"linuxrc" "\0"
"ln" "\0"
"logger" "\0"
"login" "\0"
"logname" "\0"
"losetup" "\0"
"ls" "\0"
"lsmod" "\0"
"md5sum" "\0"
"mdev" "\0"
"mkdir" "\0"
"mkdosfs" "\0"
"mke2fs" "\0"
"mkfifo" "\0"
"mkfs.ext2" "\0"
"mkfs.reiser" "\0"
"mkfs.vfat" "\0"
"mknod" "\0"
"mktemp" "\0"
"modprobe" "\0"
"more" "\0"
"mount" "\0"
"mt" "\0"
"mv" "\0"
"netstat" "\0"
"nice" "\0"
"nslookup" "\0"
"ntpd" "\0"
"passwd" "\0"
"pidof" "\0"
"ping" "\0"
"ping6" "\0"
"poweroff" "\0"
"printenv" "\0"
"printf" "\0"
"ps" "\0"
"pwd" "\0"
"reboot" "\0"
"rm" "\0"
"rmdir" "\0"
"rmmod" "\0"
"route" "\0"
"run-parts" "\0"
"sed" "\0"
"sh" "\0"
"sha1sum" "\0"
"sleep" "\0"
"sort" "\0"
"start-stop-daemon" "\0"
"strings" "\0"
"stty" "\0"
"su" "\0"
"sulogin" "\0"
"swapoff" "\0"
"swapon" "\0"
"sync" "\0"
"syslogd" "\0"
"tail" "\0"
"tar" "\0"
"tee" "\0"
"telnet" "\0"
"telnetd" "\0"
"test" "\0"
"time" "\0"
"top" "\0"
"touch" "\0"
"tr" "\0"
"true" "\0"
"tty" "\0"
"udhcpc" "\0"
"udhcpd" "\0"
"umount" "\0"
"uname" "\0"
"uniq" "\0"
"uptime" "\0"
"usleep" "\0"
"uudecode" "\0"
"uuencode" "\0"
"vi" "\0"
"vlock" "\0"
"wc" "\0"
"wget" "\0"
"which" "\0"
"who" "\0"
"whoami" "\0"
"xargs" "\0"
"yes" "\0"
;

#ifndef SKIP_applet_main
int (*const applet_main[])(int argc, char **argv) = {
test_main,
test_main,
addgroup_main,
adduser_main,
arping_main,
ash_main,
basename_main,
cat_main,
chgrp_main,
chmod_main,
chown_main,
chroot_main,
clear_main,
cp_main,
crond_main,
crontab_main,
cut_main,
date_main,
dc_main,
dd_main,
deluser_main,
deluser_main,
modprobe_main,
df_main,
dhcprelay_main,
diff_main,
dirname_main,
dmesg_main,
dnsd_main,
hostname_main,
du_main,
dumpleases_main,
echo_main,
grep_main,
env_main,
expr_main,
false_main,
fdformat_main,
fdisk_main,
grep_main,
find_main,
flash_eraseall_main,
free_main,
fsync_main,
ftpgetput_main,
ftpgetput_main,
getopt_main,
getty_main,
grep_main,
halt_main,
hexdump_main,
head_main,
hexdump_main,
hostid_main,
hostname_main,
hwclock_main,
id_main,
ifconfig_main,
ifupdown_main,
ifupdown_main,
inetd_main,
init_main,
modprobe_main,
install_main,
ipcs_main,
kill_main,
kill_main,
klogd_main,
init_main,
ln_main,
logger_main,
login_main,
logname_main,
losetup_main,
ls_main,
modprobe_main,
md5_sha1_sum_main,
mdev_main,
mkdir_main,
mkfs_vfat_main,
mkfs_ext2_main,
mkfifo_main,
mkfs_ext2_main,
mkfs_reiser_main,
mkfs_vfat_main,
mknod_main,
mktemp_main,
modprobe_main,
more_main,
mount_main,
mt_main,
mv_main,
netstat_main,
nice_main,
nslookup_main,
ntpd_main,
passwd_main,
pidof_main,
ping_main,
ping6_main,
halt_main,
printenv_main,
printf_main,
ps_main,
pwd_main,
halt_main,
rm_main,
rmdir_main,
modprobe_main,
route_main,
run_parts_main,
sed_main,
ash_main,
md5_sha1_sum_main,
sleep_main,
sort_main,
start_stop_daemon_main,
strings_main,
stty_main,
su_main,
sulogin_main,
swap_on_off_main,
swap_on_off_main,
sync_main,
syslogd_main,
tail_main,
tar_main,
tee_main,
telnet_main,
telnetd_main,
test_main,
time_main,
top_main,
touch_main,
tr_main,
true_main,
tty_main,
udhcpc_main,
udhcpd_main,
umount_main,
uname_main,
uniq_main,
uptime_main,
usleep_main,
uudecode_main,
uuencode_main,
vi_main,
vlock_main,
wc_main,
wget_main,
which_main,
who_main,
whoami_main,
xargs_main,
yes_main,
};
#endif

const uint16_t applet_nameofs[] ALIGN2 = {
0x0000,
0x0002,
0x0005,
0x000e,
0x0016,
0x001d,
0x0021,
0x002a,
0x002e,
0x0034,
0x003a,
0x0040,
0x0047,
0x004d,
0x0050,
0x8056,
0x005e,
0x0062,
0x0067,
0x006a,
0x006d,
0x0076,
0x007e,
0x0085,
0x0088,
0x0092,
0x0097,
0x009f,
0x80a5,
0x00aa,
0x00b8,
0x00bb,
0x00c6,
0x00cb,
0x00d1,
0x00d5,
0x00da,
0x00e0,
0x00e9,
0x00ef,
0x00f5,
0x00fa,
0x0109,
0x010e,
0x0114,
0x011b,
0x0122,
0x0129,
0x012f,
0x0134,
0x0139,
0x013c,
0x0141,
0x0149,
0x0150,
0x0159,
0x0161,
0x0164,
0x016d,
0x0174,
0x0179,
0x017f,
0x0184,
0x018b,
0x8193,
0x0198,
0x019d,
0x01a5,
0x01ab,
0x01b3,
0x01b6,
0x81bd,
0x01c3,
0x01cb,
0x01d3,
0x01d6,
0x01dc,
0x01e3,
0x01e8,
0x01ee,
0x01f6,
0x01fd,
0x0204,
0x020e,
0x021a,
0x0224,
0x022a,
0x0231,
0x023a,
0x023f,
0x0245,
0x0248,
0x024b,
0x0253,
0x0258,
0x0261,
0x8266,
0x026d,
0x4273,
0x4278,
0x027e,
0x0287,
0x0290,
0x0297,
0x029a,
0x029e,
0x02a5,
0x02a8,
0x02ae,
0x02b4,
0x02ba,
0x02c4,
0x02c8,
0x02cb,
0x02d3,
0x02d9,
0x02de,
0x02f0,
0x02f8,
0x82fd,
0x0300,
0x0308,
0x0310,
0x0317,
0x031c,
0x0324,
0x0329,
0x032d,
0x0331,
0x0338,
0x0340,
0x0345,
0x034a,
0x034e,
0x0354,
0x0357,
0x035c,
0x0360,
0x0367,
0x036e,
0x0375,
0x037b,
0x0380,
0x0387,
0x038e,
0x0397,
0x03a0,
0x83a3,
0x03a9,
0x03ac,
0x03b1,
0x03b7,
0x03bb,
0x03c2,
0x03c8,
};

const uint8_t applet_install_loc[] ALIGN1 = {
0x33,
0x11,
0x13,
0x13,
0x11,
0x41,
0x13,
0x34,
0x13,
0x13,
0x11,
0x12,
0x34,
0x13,
0x14,
0x33,
0x11,
0x33,
0x31,
0x12,
0x43,
0x13,
0x33,
0x21,
0x21,
0x33,
0x33,
0x21,
0x23,
0x22,
0x24,
0x32,
0x13,
0x23,
0x10,
0x13,
0x23,
0x21,
0x23,
0x21,
0x32,
0x22,
0x12,
0x21,
0x11,
0x11,
0x11,
0x43,
0x13,
0x11,
0x12,
0x13,
0x21,
0x11,
0x22,
0x11,
0x31,
0x31,
0x32,
0x11,
0x22,
0x12,
0x32,
0x31,
0x43,
0x33,
0x13,
0x13,
0x23,
0x14,
0x31,
0x13,
0x33,
0x31,
0x33,
0x33,
0x33,
0x03,
};

#define MAX_APPLET_NAME_LEN 17
