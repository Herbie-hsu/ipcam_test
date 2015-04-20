cmd_libbb/ptr_to_globals.o := arm-linux-gcc -Wp,-MD,libbb/.ptr_to_globals.o.d   -std=gnu99 -Iinclude -Ilibbb -Iinclude2 -I/home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/include -I/home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.18.5)" -DBB_BT=AUTOCONF_TIMESTAMP -I/home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb -Ilibbb -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ptr_to_globals)"  -D"KBUILD_MODNAME=KBUILD_STR(ptr_to_globals)" -c -o libbb/ptr_to_globals.o /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb/ptr_to_globals.c

deps_libbb/ptr_to_globals.o := \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/filesystem/busybox-1.18.5/src/libbb/ptr_to_globals.c \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/errno.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/features.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_config.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/cdefs.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/errno.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/errno.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/errno.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/errno.h \
  /home/media/data/hudson_build/32bitProduct_SN98600_1.20_P2P_QIWO/ST58600_SRC/toolchain/crosstool-4.5.2/bin/../arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/errno-base.h \

libbb/ptr_to_globals.o: $(deps_libbb/ptr_to_globals.o)

$(deps_libbb/ptr_to_globals.o):
