/* This is a generated file, don't edit */

#define NUM_APPLETS 105

const char applet_names[] ALIGN1 = ""
"[" "\0"
"[[" "\0"
"ash" "\0"
"basename" "\0"
"cat" "\0"
"chgrp" "\0"
"chmod" "\0"
"chown" "\0"
"chroot" "\0"
"cp" "\0"
"cut" "\0"
"date" "\0"
"dd" "\0"
"depmod" "\0"
"diff" "\0"
"dmesg" "\0"
"dnsdomainname" "\0"
"echo" "\0"
"egrep" "\0"
"env" "\0"
"expr" "\0"
"false" "\0"
"fdformat" "\0"
"fdisk" "\0"
"fgrep" "\0"
"flash_eraseall" "\0"
"fsync" "\0"
"getopt" "\0"
"getty" "\0"
"grep" "\0"
"halt" "\0"
"head" "\0"
"hostid" "\0"
"hostname" "\0"
"ifconfig" "\0"
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
"mdev" "\0"
"mkdir" "\0"
"mkdosfs" "\0"
"mke2fs" "\0"
"mkfs.ext2" "\0"
"mkfs.reiser" "\0"
"mkfs.vfat" "\0"
"mknod" "\0"
"modprobe" "\0"
"mount" "\0"
"mt" "\0"
"mv" "\0"
"ntpd" "\0"
"passwd" "\0"
"pidof" "\0"
"ping" "\0"
"poweroff" "\0"
"printenv" "\0"
"printf" "\0"
"ps" "\0"
"pwd" "\0"
"reboot" "\0"
"rm" "\0"
"rmmod" "\0"
"route" "\0"
"sed" "\0"
"sh" "\0"
"sleep" "\0"
"sort" "\0"
"start-stop-daemon" "\0"
"strings" "\0"
"stty" "\0"
"sulogin" "\0"
"swapoff" "\0"
"swapon" "\0"
"sync" "\0"
"syslogd" "\0"
"tar" "\0"
"test" "\0"
"time" "\0"
"touch" "\0"
"tr" "\0"
"true" "\0"
"tty" "\0"
"udhcpc" "\0"
"umount" "\0"
"uniq" "\0"
"usleep" "\0"
"uudecode" "\0"
"uuencode" "\0"
"vlock" "\0"
"wc" "\0"
"wget" "\0"
"yes" "\0"
;

#ifndef SKIP_applet_main
int (*const applet_main[])(int argc, char **argv) = {
test_main,
test_main,
ash_main,
basename_main,
cat_main,
chgrp_main,
chmod_main,
chown_main,
chroot_main,
cp_main,
cut_main,
date_main,
dd_main,
modprobe_main,
diff_main,
dmesg_main,
hostname_main,
echo_main,
grep_main,
env_main,
expr_main,
false_main,
fdformat_main,
fdisk_main,
grep_main,
flash_eraseall_main,
fsync_main,
getopt_main,
getty_main,
grep_main,
halt_main,
head_main,
hostid_main,
hostname_main,
ifconfig_main,
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
mdev_main,
mkdir_main,
mkfs_vfat_main,
mkfs_ext2_main,
mkfs_ext2_main,
mkfs_reiser_main,
mkfs_vfat_main,
mknod_main,
modprobe_main,
mount_main,
mt_main,
mv_main,
ntpd_main,
passwd_main,
pidof_main,
ping_main,
halt_main,
printenv_main,
printf_main,
ps_main,
pwd_main,
halt_main,
rm_main,
modprobe_main,
route_main,
sed_main,
ash_main,
sleep_main,
sort_main,
start_stop_daemon_main,
strings_main,
stty_main,
sulogin_main,
swap_on_off_main,
swap_on_off_main,
sync_main,
syslogd_main,
tar_main,
test_main,
time_main,
touch_main,
tr_main,
true_main,
tty_main,
udhcpc_main,
umount_main,
uniq_main,
usleep_main,
uudecode_main,
uuencode_main,
vlock_main,
wc_main,
wget_main,
yes_main,
};
#endif

const uint16_t applet_nameofs[] ALIGN2 = {
0x0000,
0x0002,
0x0005,
0x0009,
0x0012,
0x0016,
0x001c,
0x0022,
0x0028,
0x002f,
0x0032,
0x0036,
0x003b,
0x003e,
0x0045,
0x004a,
0x0050,
0x005e,
0x0063,
0x0069,
0x006d,
0x0072,
0x0078,
0x0081,
0x0087,
0x008d,
0x009c,
0x00a2,
0x00a9,
0x00af,
0x00b4,
0x00b9,
0x00be,
0x00c5,
0x00ce,
0x00d7,
0x00dd,
0x00e2,
0x00e9,
0x80f1,
0x00f6,
0x00fb,
0x0103,
0x0109,
0x0111,
0x0114,
0x811b,
0x0121,
0x0129,
0x0131,
0x0134,
0x013a,
0x013f,
0x0145,
0x014d,
0x0154,
0x015e,
0x016a,
0x0174,
0x017a,
0x0183,
0x0189,
0x018c,
0x018f,
0x8194,
0x019b,
0x41a1,
0x01a6,
0x01af,
0x01b8,
0x01bf,
0x01c2,
0x01c6,
0x01cd,
0x01d0,
0x01d6,
0x01dc,
0x01e0,
0x01e3,
0x01e9,
0x01ee,
0x0200,
0x0208,
0x020d,
0x0215,
0x021d,
0x0224,
0x0229,
0x0231,
0x0235,
0x023a,
0x023f,
0x0245,
0x0248,
0x024d,
0x0251,
0x0258,
0x025f,
0x0264,
0x026b,
0x0274,
0x827d,
0x0283,
0x0286,
0x028b,
};

const uint8_t applet_install_loc[] ALIGN1 = {
0x33,
0x31,
0x11,
0x11,
0x14,
0x13,
0x21,
0x13,
0x11,
0x31,
0x13,
0x23,
0x41,
0x11,
0x12,
0x32,
0x13,
0x42,
0x22,
0x33,
0x31,
0x02,
0x31,
0x31,
0x12,
0x22,
0x21,
0x22,
0x22,
0x21,
0x11,
0x41,
0x13,
0x21,
0x31,
0x11,
0x12,
0x22,
0x11,
0x31,
0x32,
0x21,
0x22,
0x21,
0x31,
0x13,
0x13,
0x23,
0x31,
0x31,
0x33,
0x33,
0x03,
};

#define MAX_APPLET_NAME_LEN 17
