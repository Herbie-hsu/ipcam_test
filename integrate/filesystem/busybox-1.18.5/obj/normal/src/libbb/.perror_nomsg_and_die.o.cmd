cmd_libbb/perror_nomsg_and_die.o := arm-linux-gcc -Wp,-MD,libbb/.perror_nomsg_and_die.o.d   -std=gnu99 -Iinclude -Ilibbb -Iinclude2 -I/home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/include -I/home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.18.5)" -DBB_BT=AUTOCONF_TIMESTAMP -I/home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb -Ilibbb -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(perror_nomsg_and_die)"  -D"KBUILD_MODNAME=KBUILD_STR(perror_nomsg_and_die)" -c -o libbb/perror_nomsg_and_die.o /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb/perror_nomsg_and_die.c

deps_libbb/perror_nomsg_and_die.o := \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb/perror_nomsg_and_die.c \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../lib/gcc/arm-unknown-linux-uclibcgnueabi/4.5.2/include-fixed/limits.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../lib/gcc/arm-unknown-linux-uclibcgnueabi/4.5.2/include-fixed/syslimits.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/limits.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/features.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_config.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/cdefs.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/posix1_lim.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/local_lim.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/limits.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_local_lim.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/posix2_lim.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/xopen_lim.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/stdio_lim.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/byteswap.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/byteswap.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/byteswap-common.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/endian.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/endian.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../lib/gcc/arm-unknown-linux-uclibcgnueabi/4.5.2/include/stdbool.h \

libbb/perror_nomsg_and_die.o: $(deps_libbb/perror_nomsg_and_die.o)

$(deps_libbb/perror_nomsg_and_die.o):
