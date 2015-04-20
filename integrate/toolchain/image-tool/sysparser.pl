#!/usr/bin/perl -w
use strict;
use Spreadsheet::ParseExcel;
use Digest::MD5;
use Digest::CRC;
use Encode;
use Encode::Detect::Detector;


# get time
my $NOW_TIME;	
my @time_arr;
my $half_left;
my $half_right;
my $m_d;
my $h_m;
my $reg_time;


# data type
my $CHAR = "char";
my $UINT8 = "UINT8";
my $UINT16 = "UINT16";
my $UINT32 = "UINT32";
my $INT32 = "INT32";
my $CHAR_STR = "char";
my $U8_STR = "uint8_t";
my $U16_STR = "uint16_t";	
my $U32_STR = "uint32_t";
my $I32_STR = "int32_t";

#confiuration related
my $configuration_file = "Configuration.bin";               
my $configuration_header_file = "nvram.h";      
my $all_tot_size;
my $configuration_md5;
my $configuration_name = "configuration";
my $all_con_idex = 0;
my @all_config_struct_sizesum;  
my $zero_num="";
my $inter_ff="";

#u-env
my $envs_tack = "\0";
my $mem_excel = 0;
my @envs_args;
my @envs_cmds;

#Package
my $UBOOT_NAME = "uboot";
my $KERNEL_NAME = "kernel";
my $ROOTFS_R_NAME = "rootfs-r";
my $ROOTFS_RW_NAME = "rootfs-rw";
my $ENGINE_NAME = "engine";
my $ULOGO_NAME = "ulogo";
my $RESCUE_NAME = "rescue";

my $UBOOT_PZNAME = "UBOOT.bin";              
my $KERNEL_PZNAME = "KERNEL.bin";            
my $RESCUE_PZNAME = "RESCUE.bin";            
my $ROOTFS_R_PZNAME = "ROOTFS-R.bin";        
my $ROOTFS_RW_PZNAME = "ROOTFS-RW.bin";      
my $ENGINE_PZNAME = "ENGINE.bin";  
my $ULOGO_PZNAME = "ulogo.bin.d";            

my $spi_file_d = "spi_flash_layout.d";       
my $spi_type_d = "SF";
my $nand_file_d = "nand_flash_layout.d";     
my $nand_type_d = "NAND";

my $NAND_BLK_SIZE = 0xFF4;
my $SPI_BLK_SIZE = 0xFF4;

#parameter from Makefile
my $flash_type;
my $uboot;
my $u_logo;
my $flash_info;
my $pll_setting;
my $hw_setting;
my $flash_layout;
my $kernel;
my $rescue;
my $rootfs_r;
my $rootfs_rw;
my $engine;
my $dotconfig;
my $image;
my $platform;
my $done_uboot_file;
my $done_kernel_file;
my $done_rootfsr_file;
my $done_nvram_file;
my $done_rescue_file;
my $rescue_enable;
my $isp_path;
my $sdk_version = "8888";
my $ddr_freq;
my $ddr_project = "";
my $config_path;
my $hw_version="";

#uexce related
my $uexce_command ="./src/code/image_tool";
my $header_file = "./src/header/header.bin"; 
my $uboot_file = "./uexce.bin.d";
my $u_boot_env_file = "u-boot-env.bin.d";
my $flash_layout_header = "flash_layout.h";
my $flash_layout_file = "flash_layout.bin.d";

#FIRMWARE.bin
my $layoutfile_fir = "flash_layout_firmware.bin";
my $layoutfile_fir_size = 1024;
my $firmware_file = "FIRMWARE.bin";
my $firmware_factory_file = "FIRMWARE_F.bin";
my $FIRMWARE_NAME = "firmware";
my $FIRMWARE_FACTORY_NAME = "firmware_f";

#U-ENV
my $U_BOOT_ENV_NAME = "u-boot-env";
my $env_file = "u-boot-";


#flash_layout related
my $flash_layout_file_size = 512;
my @flash_nand = qw/flash-info-1-10 hw_setting-1 u-boot-1 reserve-1 u-env-1 flash-layout-1 
	                   hw_setting-2 u-boot-2 reserve-2 u-env-2 flash-layout-2 factory-1 kernel 
	                   rootfs-r rootfs-rw user-1 reserved-for-user u-logo rescue user-2 factory-2 
	                   user-3 factory-3 hw_setting-3 u-boot-3 reserve-3 u-env-3 flash-layout-3 bbt add/;
my @flash_spi =qw/hw-setting u-boot u-env flash-layout factory kernel rootfs-r rescue 
					rootfs-rw user 
	                u-logo add/;


#calculate crc16
my @crc16_tab=(
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
);

###deal with time###
$NOW_TIME = &gettime;
@time_arr = split(/\//, $NOW_TIME);

$half_left=substr($time_arr[1],0,2);
$half_right=substr($time_arr[1],2,2);
$m_d=$half_left."-".$half_right;

$half_left=substr($time_arr[2],0,2);
$half_right=substr($time_arr[2],2,2);
$h_m=$half_left.":".$half_right;

$reg_time=$time_arr[0]."-".$m_d." ".$h_m;
$NOW_TIME=$reg_time;

###abstract @ARGV###
foreach (@ARGV){
	
	if (/^dotconfig=.*/){
		s/.*=//g;
		$dotconfig = $_;
	}
	elsif (/^image=.*/){
		s/.*=//g;
		$image = $_;
	}
	elsif(/^flash_type=.*/){
		s/.*=//g;
		$flash_type = $_;
	}
	elsif(/^uboot=.*/){
		s/.*=//g;
		$uboot = $_;
	}
	elsif(/^u_logo=.*/){
		s/.*=//g;
		$u_logo = $_;
	}
	elsif(/^config_path=.*/){
		s/.*=//g;
		s/\/$//;
		$config_path = $_;
	}
	elsif(/^rescue=.*/){
		s/.*=//g;
		$rescue= $_;
	}	
	elsif(/^kernel=.*/){
		s/.*=//g;
		$kernel= $_;
	}	
	elsif(/^rootfs_r=.*/){
		s/.*=//g;
		$rootfs_r= $_;
	}
	elsif(/^rootfs_rw=.*/){
		s/.*=//g;
		$rootfs_rw= $_;
	}	
	elsif(/^engine=.*/){
		s/.*=//g;
		$engine= $_;
	}
	elsif(/^platform=.*/)	{
		s/.*=//g;
		$platform= $_;
	}
	elsif(/^hw_version=.*/)	{
		s/.*=//g;
		$hw_version= $_;
	}
	elsif (/^done_uboot_file=.*/){
	  s/.*=//g;
		$done_uboot_file= $_;
	}
	elsif(/^done_kernel_file=.*/){
		s/.*=//g;
		$done_kernel_file= $_;
	}
	elsif(/^done_rootfsr_file=.*/){
		s/.*=//g;
		$done_rootfsr_file= $_;
	}
	elsif(/^done_rescue_file=.*/){
		s/.*=//g;
		$done_rescue_file= $_;
	}
	elsif(/^done_nvram_file=.*/){
		s/.*=//g;
		$done_nvram_file= $_;
	}
	elsif(/^isp_path=.*/){
		s/.*=//g;
		s/\/$//;
		$isp_path= $_;
	}
	elsif(/^sdk_version=.*/){
		s/.*=//g;
		$sdk_version= $_;
	}
	elsif(/^ddr_freq=.*/){
		s/.*=//g;
		$ddr_freq= $_;
	}
	elsif(/^rescue_system=.*/){
		s/.*=//g;
		$rescue_enable= $_;
	}
	elsif(/^ddr_project=.*/){
		s/.*=//g;
		$ddr_project= $_;
	}
	else{
		print $_ , "\n";
		print "the image parameter is wrong!\n";
	}	
}
#####select flash_layout#######
if ($flash_type =~ /NAND/i){
 $flash_layout = "$config_path/flash-layout/nand_flashlayout_1Gb.conf";
}
elsif ($flash_type =~ /SF/i){
 $flash_layout = "$config_path/flash-layout/serial_flashlayout_16mb.conf";
}

unless (-e $flash_layout){
 print "\nError:	flash layout  file--->$flash_layout does not exist!\n\n";
 exit;
}

###VERSION###
my $UBOOT_VER = $sdk_version.$envs_tack ;
my $KERNEL_VER = $sdk_version.$envs_tack ;
my $ROOTFS_R_VER = $sdk_version.$envs_tack ;
my $ROOTFS_RW_VER = $sdk_version.$envs_tack ;
my $ENGINE_VER = $sdk_version.$envs_tack ;
my $ULOGO_VER = $sdk_version.$envs_tack ;
my $RESCUE_VER = $sdk_version.$envs_tack ;
my $FIRMWARE_VER = $sdk_version.$envs_tack ;
my $U_BOOT_ENV_VER = $sdk_version.$envs_tack ;
my $configuration_ver = $sdk_version.$envs_tack ;

################## abstract flash_layout data ##############
open(oFILE,$flash_layout) || die("Cannot Open File: $!");
my @flash_layout_raw = <oFILE>;
close(oFILE);
my @flash_layout_value;
foreach (@flash_layout_raw){
 unless((/^Flash-Type.*/)||(/^Content.*/)){
  if(/(\S+)\s+(\S+)\s+(\S+)/){
   push @flash_layout_value,$2;
   push @flash_layout_value,$3;
  }
 }
}

################## main body  ##############
if ($image =~ /uboot/) {
	&fun_uboot($flash_type,$uboot,$u_logo,$rescue,$flash_info,$pll_setting,$hw_setting,$flash_layout,$dotconfig)
	}
elsif ($image =~ /rescue/) {
	&packize($rescue,$RESCUE_PZNAME,$RESCUE_NAME,$RESCUE_VER,$NOW_TIME,'S')
	}
elsif ($image =~ /kernel/) {
	&packize($kernel,$KERNEL_PZNAME,$KERNEL_NAME,$KERNEL_VER,$NOW_TIME,'S')
	}
elsif ($image =~ /^rootfs-r$/) {
	&packize($rootfs_r,$ROOTFS_R_PZNAME,$ROOTFS_R_NAME,$ROOTFS_R_VER,$NOW_TIME,'N')
	}
elsif ($image =~ /^rootfs-rw$/) {
	&packize($rootfs_rw,$ROOTFS_RW_PZNAME,$ROOTFS_RW_NAME,$ROOTFS_RW_VER,$NOW_TIME,'N')
	}
elsif ($image =~ /engine/) {
	# spi_flash_layout.d - SPI
	&fun_flash_layout_bin(\$spi_file_d, \$spi_type_d, \@flash_layout_value);
	# nand_flash_layout.d - NAND
	&fun_flash_layout_bin(\$nand_file_d, \$nand_type_d, \@flash_layout_value);
	# ./Image/u-boot.bin - ENGINE.bin
	&packize($engine,$ENGINE_PZNAME,$ENGINE_NAME,$ENGINE_VER,$NOW_TIME,'F',$spi_file_d,$nand_file_d);
	#system("mv ./$ENGINE_PZNAME ../Engine");	
	}
elsif ($image =~ /^firmware_factory$/) {
	generate_firmware($done_uboot_file,$done_kernel_file,$done_rootfsr_file,$done_nvram_file,$done_rescue_file ,$flash_layout,$flash_type,$firmware_factory_file,$FIRMWARE_FACTORY_NAME);
}
elsif ($image =~ /^firmware$/) {
	generate_firmware($done_uboot_file,$done_kernel_file,$done_rootfsr_file,$done_nvram_file,$done_rescue_file,$flash_layout,$flash_type,$firmware_file,$FIRMWARE_NAME);
}
else {print "the image parameter is wrong!\n"}

################## sub function ##############
#about firmware
sub generate_firmware
{
	my $uboot_fir = $_[0];
	my $kernel_fir = $_[1];
	my $rootfsr_fir = $_[2];
	my $nvram_fir = $_[3];
	my $rescue_fir = $_[4];
	my $flash_fir = $_[5];
	my $type_fir = $_[6];
	my $target_file = $_[7];#$firmware_file
	my $target_name = $_[8];#$FIRMWARE_NAME
	
	################## abstract flash_layout data ##############
  open(oFILE,$flash_fir) || die("Cannot Open File: $!");
  my @flash_layout_ori = <oFILE>;
  close(oFILE);
  my @flash_layout_content;
  foreach (@flash_layout_ori){
	unless((/^Flash-Type.*/)||(/^Content.*/)){
		if(/(\S+)\s+(\S+)\s+(\S+)/){
			push @flash_layout_content,$2;
			push @flash_layout_content,$3;
		}
	}
 }
 ############################################################
 &firmware_flayout_bin(
	\$layoutfile_fir,
	\$type_fir,
	\@flash_layout_content
                      );
 
#UBOOT.bin
 my @data_uboot = ();	
 if($target_name =~ /^firmware_f$/){
 	open(oFILE, $uboot_fir) or die "Can't open '$uboot_fir': $!";
 	binmode(oFILE);
 	# read file into an array
 	@data_uboot = <oFILE>;	
 	close oFILE;
 }

#KERNEL.bin
 open(oFILE, $kernel_fir) or die "Can't open '$kernel_fir': $!";
 binmode(oFILE);
 # read file into an array
 my @data_kernel = <oFILE>;	
 close oFILE;

#ROOTFS-R.bin 
 open(oFILE, $rootfsr_fir) or die "Can't open '$rootfsr_fir': $!";
 binmode(oFILE);
 # read file into an array
 my @data_rootfsr = <oFILE>;	
 close oFILE;
 
 #nvram.bin
 open(oFILE, $nvram_fir) or die "Can't open '$nvram_fir': $!";
 binmode(oFILE);
 # read file into an array
 my @data_nvram_fir = <oFILE>;	
 close oFILE;

 my @data_rescue_fir = ();	
 	if($rescue_enable =~ /^y$/){
 		if($target_name =~ /^firmware_f$/){
 			open(oFILE, $rescue_fir) or die "##### #### Can't open '$rescue_fir': $!";
			binmode(oFILE);
 			@data_rescue_fir = <oFILE>;	
			close oFILE;
		}
	}
#flash_layout_firmware.bin 
 open(oFILE, $layoutfile_fir) or die "Can't open '$layoutfile_fir': $!";
 binmode(oFILE);
 # read file into an array
 my @data_layoutfile_fir = <oFILE>;	
 close oFILE;

#abstract their size
 my $size_uboot_fir = 0;
 if($target_name =~ /^firmware_f$/){
 	 $size_uboot_fir = -s $uboot_fir;
 }
 my $size_kernel_fir = -s $kernel_fir;
 my $size_rootfsr_fir = -s $rootfsr_fir;
 my $size_nvram_fir = -s $nvram_fir;

 my $size_rescue_fir = 0;
 if($rescue_enable =~ /^y$/){
	if($target_name =~ /^firmware_f$/){
 	 	$size_rescue_fir = -s $rescue_fir;
	}
 }
 my $size_layoutfile_fir = -s $layoutfile_fir;
 
 my $total_size_fir = $size_layoutfile_fir + 20 + $size_uboot_fir + $size_nvram_fir + $size_kernel_fir + $size_rootfsr_fir + $size_rescue_fir;

 #generate FIRMWARE.bin#
#######################
#######################
 open oFILE, ">" . $target_file or die "Can't open '$target_file': $!";
#size
 #print oFILE pack('L',$total_size_fir);
#flash_layout_firmware.bin
 print oFILE @data_layoutfile_fir;	
#UBOOT.bin
 print oFILE pack('L', $size_uboot_fir);
 if($target_name =~ /^firmware_f$/){
 	 print oFILE @data_uboot;
 }
 #nvram
 print oFILE pack('L', $size_nvram_fir);
 print oFILE @data_nvram_fir;
#KERNE.bin
 print oFILE pack('L', $size_kernel_fir);
 print oFILE @data_kernel;
#ROOTFS-R.bin
 print oFILE pack('L', $size_rootfsr_fir);
 print oFILE @data_rootfsr;
 
 print oFILE pack('L', $size_rescue_fir);
 if($rescue_enable =~ /^y$/){
	if($target_name =~ /^firmware_f$/){
	 	 print oFILE @data_rescue_fir;
	}
 }
 close oFILE;
 
#calculate how many bytes needed to add#
########################################
########################################
	#make $file to 16times large
	my $what_large = $total_size_fir;
	until ($what_large < 16){
		$what_large -= 16;
  }

  unless ($what_large == 0){
	  my $how_add = 16 - $what_large;
	  $total_size_fir = $total_size_fir + $how_add;
    open(oFILE, ">>$target_file");	# Open for appending
		while ($how_add > 0){
			$how_add --;
			my $add_str = sprintf("%s", "FF");
			my $str_done = pack("a2", $add_str); 
			print oFILE pack('H*',$str_done);
		}
		close oFILE;  
  }
  
###        calculate crc16           ###
########################################
########################################
	my $crc16;
	my @message;
	my $message_bytes = $total_size_fir;
	my $count=$message_bytes;
	my $index_message=0;
	
	open(GIF, $target_file) or die "can't open '$target_file': $!";
	binmode(GIF);
	while ($count > 0){
		$count--;
		read(GIF, $message[$index_message], 1);
		$message[$index_message] = unpack("C",$message[$index_message]);
		$index_message++;
	}
	close GIF;
	&crcSlow(
		\$crc16,
		\@crc16_tab, 
		\@message,       
		\$message_bytes
 	);
################################
#add size#
#######################
####################### 
 open(oFILE, $target_file) or die "Can't open '$target_file': $!";
 binmode(oFILE);
 # read file into an array
 my @data_firmware = <oFILE>;	
 close oFILE; 
 
 open oFILE, ">" . $target_file or die "Can't open '$target_file': $!";
 #size
 print oFILE pack('L',$total_size_fir);
 #raw data
 print oFILE @data_firmware;	 
 close oFILE; 

#add name + version + time + crc16#
###################################
###################################
#name
	open(oFILE, ">>$target_file");	# Open for appending
	my $str = pack( "a16", $target_name); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
#version
	$str = pack("a64", $FIRMWARE_VER); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
#time	
	$str = pack("a16", $NOW_TIME); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
#crc16
#	$crc16 = sprintf("%s", $crc16);
#	$str = pack("a8", $crc16); 
#	print oFILE pack('H*',$str);
  print oFILE pack('L',$crc16);
	
	my $add_str = sprintf("%s", "FF");
	my $time_add = 12;
	while ($time_add){
		$time_add --;
		my $str_done = pack("a2", $add_str); 
		print oFILE pack('H*',$str_done);
	}
	
	close oFILE;	
	
}

#about firmware
sub firmware_flayout_bin
{
	my (
			$fir_layoutfile, 
			$fir_type, 
			$fir_layout_value	#@                      
	) = @_;	 

	my $flash_index = 0;
		
	open oFILE, ">" . $$fir_layoutfile;
	if ($$fir_type =~ /NAND/i){
	my $i = 0;
	foreach (@flash_nand){
		my $nand_string_fir = $_;
		my $str	= $$fir_layout_value[$flash_index];
		$flash_index ++;
		my $end = $$fir_layout_value[$flash_index];
		$flash_index ++;
				
		$str =~ s/\s+//g;
		$end =~ s/\s+//g;
		
		$nand_string_fir = $nand_string_fir.$envs_tack;
		foreach(unpack("(a1)*", $nand_string_fir)) {
			print oFILE;
		}
					
		my	@p32u_str = split(/x/, $str);
		my $ff32u_str = sprintf "%08d", hex($p32u_str[1]);
		print oFILE pack('L', $ff32u_str);
					
		my	@p32u_end = split(/x/, $end);
		my $ff32u_end = sprintf "%08d", hex($p32u_end[1]);
		print oFILE pack('L', $ff32u_end);
		}
	}
	elsif ($$fir_type =~ /SF/i){
		my $i = 0;
		$flash_index = 0;
		foreach (@flash_spi){
		my $spi_string_fir = $_;
		my $str	= $$fir_layout_value[$flash_index];
		$flash_index ++;
		my $end = $$fir_layout_value[$flash_index];
		$flash_index ++;
					
		$str =~ s/\s+//g;
		$end =~ s/\s+//g;
		
		$spi_string_fir = $spi_string_fir.$envs_tack;
		foreach(unpack("(a1)*", $spi_string_fir)) {
			print oFILE;
		}
					
		my	@p32u_str = split(/x/, $str);
		my $ff32u_str = sprintf "%08d", hex($p32u_str[1]);
		print oFILE pack('L', $ff32u_str);
					
		my	@p32u_end = split(/x/, $end);
		my $ff32u_end = sprintf "%08d", hex($p32u_end[1]);
		print oFILE pack('L', $ff32u_end);
		}
	}
	else{
		print "Error !! Flash Type Mistake....\n"
	}
		
	close oFILE;

	#make $file to 16times large
	my $what_large = -s $$fir_layoutfile;
	until ($what_large < 16){
		$what_large -= 16;
	} 
	unless ($what_large == 0){
	my $how_add = 16 - $what_large;
	open(oFILE, ">>$$fir_layoutfile");	# Open for appending
	while ($how_add > 0){
			$how_add --;
			my $add_str = sprintf("%s", "FF");
			my $str_done = pack("a2", $add_str); 
			print oFILE pack('H*',$str_done);
		}
		close oFILE;  
	}
	###########	
	
	
	### Attach CRC16 [16-bytes] ###
	my $crc16;
	my @message;
	my $message_bytes = -s $$fir_layoutfile;
	my $count=$message_bytes;
	my $index_message=0;
	
	open(GIF, $$fir_layoutfile) or die "can't open '$$fir_layoutfile': $!";
	binmode(GIF);
	while ($count > 0){
		$count--;
		read(GIF, $message[$index_message], 1);
		$message[$index_message] = unpack("C",$message[$index_message]);
		$index_message++;
	}
	close GIF;
	
	&crcSlow(
		\$crc16,
		\@crc16_tab, 
		\@message,       
		\$message_bytes
 	);
	################################
		
	my $the_long = -s $$fir_layoutfile;
	
	
	open(oFILE, ">>$$fir_layoutfile");	# Open for appending
	################################
#	$crc16 = sprintf("%s", $crc16);
#	my $str_crc = pack("a8", $crc16); 
#	print oFILE pack('H*',$str_crc);
	print oFILE pack('L',$crc16);
	
	my $add_str = sprintf("%s", "FF");
	my $time_add = 12;
	while ($time_add){
		$time_add --;
		my $str_done = pack("a2", $add_str); 
		print oFILE pack('H*',$str_done);
	}
	
	#zeroing to size
	for (my $i=0;
		$i < ($layoutfile_fir_size - $the_long -16);
		$i++){
			print oFILE pack('H2', 'FF');
		}	
	close oFILE;
}


sub fun_uboot
{		
	my $flash_type_para = $_[0];
	my $uboot_para = $_[1];
	my $u_logo_para = $_[2];
#	my $rescue_para = $_[3];
#	my $flash_info_para = $_[4];
#	my $pll_setting_para = $_[5]; # 'S'
#	my $hw_setting_para = $_[6];
	my $flash_layout_para = $_[7];
#	my $excel_sys_para = $_[8];
#	my $hw_setting_file = "hw_setting.conf";
	my $pz_ufile = "./uboot.d";
	my $config = $_[8];

############################# Config processing  ################################
	my $hw_setting_af_pll = "$config_path/template/hw_setting_af_pll.txt";
	my $hw_setting_af_ddr = "$config_path/template/hw_setting_af_ddr_asic.txt";
	my $flashinfo_file = "$config_path/template/flash_info_nl.txt";
	my $ddr_config_file = "";
	my $hw_file = "sn986xx";
	$ddr_config_file = "$config_path/hw-setting/".$hw_file."/";
	my $hw_template_file = "$config_path/template/hw_setting_af_pll_asic.txt";
	my $flash_layout_header_file = "$config_path/flash-layout/";

	my $flashtype = "";
	my $rescue_mode = "none";

	my $ramboot_file;
#	my $rescue_file;
	my $burn_file;

	my $project = "";
	my $ptye = "full";
	
	my $ddr_project_para = "";
	my $bits = "";
	my $odt = "_2.5_BL4_no_ODT";
	
	my $mem_kconfig = "";
	
	if ($ddr_project =~ /([a-zA-Z0-9]+)_([0-9a-zA-Z]+)/){
		$ddr_project_para = $1;
		my $mem_bit = $2;
		if ($mem_bit =~ /([^x]+)x([^x]+)/){
			$mem_kconfig = $1;
			$bits = $2;
		}
	}
#	print "\n***$ddr_project_para/$bits/$mem_kconfig***\n";

	#################platform#################
	if(($platform =~ /sn98600/) || ($platform =~ /sn98601/)){
  		my $hw_which_file = "sn9860x";
  		unless ($hw_version =~ /none/){
  			$hw_which_file = $hw_which_file."_".$hw_version;
  		}
			$ddr_config_file = $ddr_config_file.$hw_which_file."/";
			$project = $ddr_project_para;
			$ptye = "half";
	}
#	elsif (($platform =~ /sn93560/) || ($platform =~ /sn93561/)){
#  		my $hw_which_file = "sn9356x";
#  		unless ($hw_version =~ /none/){
#  			$hw_which_file = $hw_which_file."_".$hw_version;
#  		}
#			$ddr_config_file = $ddr_config_file.$hw_which_file."/";
#			$project = $ddr_project_para;
#			$ptye = "half";
#	}
	elsif (($platform =~ /sn98610/)){
  		my $hw_which_file = "sn98610";
  		unless ($hw_version =~ /none/){
  			$hw_which_file = $hw_which_file."_".$hw_version;
  		}
			$ddr_config_file = $ddr_config_file.$hw_which_file."/";
			$project = $ddr_project_para;
			$ptye = "half";
	}
	
	###################ddr######################################
	if ($ddr_freq =~ /25/){
			$hw_template_file = "$config_path/template/hw_setting_af_pll_fpga.txt";
			$hw_setting_af_ddr = "$config_path/template/hw_setting_af_ddr_fpga.txt";
			$project = $ddr_project_para;
			$ddr_config_file = "$config_path/hw-setting/sn986xx/st58600_fpga/";
			$ddr_config_file = $ddr_config_file ."ddr2"."_25mhz_". $bits ."_".$project."_2.5_BL4";
	}
	elsif ($ddr_freq =~ /150/){
			$ddr_config_file = $ddr_config_file ."ddr2_".$ptye."_150mhz_".$bits."_".$project.$odt;
	}
	elsif ($ddr_freq =~ /198/) {
			$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_198mhz_".$bits."_".$project.$odt;
	}
	elsif ($ddr_freq =~ /201/) {
			$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_201mhz_".$bits."_".$project.$odt;
	}
	elsif ($ddr_freq =~ /312/) {
			$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_312mhz_".$bits."_".$project.$odt;
	}
	elsif ($ddr_freq =~ /324/) {
			$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_324mhz_".$bits."_".$project.$odt;
	}
	elsif ($ddr_freq =~ /336/) {
			$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_336mhz_".$bits."_".$project.$odt;
	}
	else{
		if ($platform =~ /sn98610/){
			if ($bits =~ /32bit/){
				$odt = "_2.5_BL4_ODT_150ohm";
			}
		}
		if ($ddr_freq =~ /348/){
				$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_348mhz_".$bits."_".$project.$odt;
		}
		elsif ($ddr_freq =~ /360/) {
				$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_360mhz_".$bits."_".$project.$odt;
		}
		elsif ($ddr_freq =~ /372/){
				$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_372mhz_".$bits."_".$project.$odt;	
		}
		elsif ($ddr_freq =~ /384/){
				$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_384mhz_".$bits."_".$project.$odt;	
		}
		elsif ($ddr_freq =~ /396/){
				$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_396mhz_".$bits."_".$project.$odt;	
		}
		elsif ($ddr_freq =~ /402/){
				$ddr_config_file = $ddr_config_file . "ddr2_".$ptye."_402mhz_".$bits."_".$project.$odt;	
		}
	}

	###generate ddr config
	unless (-e $ddr_config_file){
		print "\nError:	ddr origin file--->$ddr_config_file does not exist!\n\n";
		exit;
	}
	system ("make -C $config_path/hw-setting/ sdk-f WHICH_VALUE=$ddr_config_file");
	$ddr_config_file =~ s/\/sn986xx\//\/sdk\/sn986xx\//;
	unless (-e $ddr_config_file){
		print "\nError:	ddr target file--->$ddr_config_file does not exist!\n\n";
		exit;
	}
  
	### DEBUG ###
	print "flinfo = $flashinfo_file\n";
	print "hw_t = $hw_template_file\n";
	print "ddr = $ddr_config_file\n";
	unless (-e $flashinfo_file){
		print "\nError:	flashinfo file--->$flashinfo_file does not exist!\n\n";
		exit;
	}
	unless (-e $hw_template_file){
		print "\nError:	hw template file--->$hw_template_file does not exist!\n\n";
		exit;
	}
	unless (-e $ddr_config_file){
		print "\nError:	ddr config file--->$ddr_config_file does not exist!\n\n";
		exit;
	}
	#unless (-e $uexce_command){
	#	print "\nError:	uexce command file--->$uexce_command does not exist!\n\n";
	#	exit;
	#}
	#unless (-e "./src/header/header.bin"){
	#	print "\nError:	header file--->'./src/header/header.bin' does not exist!\n\n";
	#	exit;
	#}
	unless (-e $uboot_para){
		print "\nError:	uboot file--->$uboot_para does not exist!\n\n";
		exit;
	}	
	unless (-e $hw_setting_af_ddr){
		print "\nError:	af_ddr file--->$hw_setting_af_ddr does not exist!\n\n";
		exit;
	}	
	############### ADD ##########################
	$uexce_command = $uexce_command." -e ./src/header/header.bin";
	if($flash_type_para =~ /NAND/i ){
		$uexce_command = $uexce_command." -f $flashinfo_file";
	}

	open(oDDR, $ddr_config_file) or die "Can't open '$ddr_config_file': $!";
	#binmode(oDDR);
	# read file into an array
	my @data_ddr = <oDDR>;	
	close oDDR;
	open(oHW, $hw_template_file) or die "Can't open '$hw_template_file': $!";
	#binmode(oHW);
	# read file into an array
	my @data_hw = <oHW>;	
	close oHW;
	open(oAFDDR, $hw_setting_af_ddr) or die "Can't open '$hw_setting_af_ddr': $!";
	# read file into an array
	my @data_afddr = <oAFDDR>;	
	close oAFDDR;
	
	my $last_line = "";
	my $fill = "empty";
	my $find_030 = "empty";
	open oHW_SETTING, ">" . "./hw_setting.txt.d";
	foreach (@data_hw){
		if(($_ =~ /^\#/)||($_ =~ /^b/)){
			print oHW_SETTING;
		}
	}
	foreach (@data_ddr){
		my $th_li = $_;
		if (($th_li =~ /^w 0x9030/)&&($find_030 =~ /^empty$/)){
			$find_030 = "full";
		}
		if($find_030 =~ /^full$/){
		 unless($th_li =~ /^w 0x9030/){
		 foreach(@data_afddr){
		 	  unless($_ =~ /^\#/){
					print oHW_SETTING;
					$last_line = $_;
				}
			}
			unless($last_line =~ /^\s*$/){
				print oHW_SETTING "\n";
			}
		 }
		}
		print oHW_SETTING $th_li;
		if (($_ =~ /^n/)&&($fill =~ /^empty$/)){
			$fill = "full";
			foreach (@data_hw){
				unless(($_ =~ /^\#/)||($_ =~ /^b/)){
					print oHW_SETTING;
					$last_line = $_;
				}
			}
			unless($last_line =~ /^\s*$/){
				print oHW_SETTING "\n";
			}
		}
	}
	close oHW_SETTING;

	#system("cat $hw_template_file $ddr_config_file > ./hw_setting.txt.d");
	$uexce_command = $uexce_command." -h ./hw_setting.txt.d";


	&padd32($uboot_para,$pz_ufile);
	$uexce_command = $uexce_command." -u ".$pz_ufile;

	$env_file = $config_path."/u-boot-env/sn986xx/".$env_file;
	my $env_type = lc($flash_type_para);
	$env_file = $env_file."$platform-".$env_type.".txt";
	unless (-e $env_file){
		print "\nError:	env file--->$env_file does not exist!\n\n";
		exit;
	}
	&read_env;
	$uexce_command = $uexce_command." -n ".$u_boot_env_file;
	#mem_excel check
	my $bit_value = "";
	if ($bits =~ /([0-9]+)[a-zA-Z]+/){
	 	 $bit_value = $1;
	}
	my $mem_size = $mem_kconfig * $bit_value / 8;
	#print "mem_excel  :$mem_excel\n";
	#print "mem_kconfig:$mem_kconfig\n";
	#print "bit_value  :$bit_value\n";
	#print "mem_size   :$mem_size\n";
	if ($mem_size != $mem_excel){
	 	 print "WARNING: mem size between 'excel' and 'kconfig' are different!\n";
	 	 if ($mem_excel > $mem_size){
	 	 	 print "ERROR: mem size in 'excel' is larger than in 'kconfig'!\n";
	 	 	 exit;
	 	 }
	}


	### flash_layout.h ###
	&fun_flash_layout_header(
		\$flash_layout_header,
		\$flash_type_para,
		\@flash_layout_value
                      );
                       
	### flash_layout.bin ###
	&fun_flash_layout_bin(
 		\$flash_layout_file, 
    \$flash_type_para, 
    \@flash_layout_value
                   );
	system("mv ./$flash_layout_header ./src/header/");
	system("make -C ./src/header/ clean;make -C ./src/header/");
	$uexce_command = $uexce_command." -l ".$flash_layout_file;

	system("make -C ./src/code/ clean;make -C ./src/code/");

	###test ulogo###
	#  my $u_logo_fname = "./u-logo.bmp";
	#  my $ULOGO_PZNAME = "ulogo.bin.d";
	#  my $ULOGO_NAME = "ulogo";
	#  &packize_crc32($u_logo_fname,$ULOGO_PZNAME,$ULOGO_NAME,$ULOGO_VER,$NOW_TIME,'N');
	#	$uexce_command = $uexce_command." -g ".$ULOGO_PZNAME;
	################
  
	$uexce_command = $uexce_command." -v 0x12345678";
	$uexce_command = $uexce_command." -o ./";

	system($uexce_command);
	print $uexce_command;
	print "\n";	
	
	# ./uexce.bin.d - UBOOT.bin -
	&packize($uboot_file,$UBOOT_PZNAME,$UBOOT_NAME,$UBOOT_VER,$NOW_TIME,'N');
}

sub read_env
{
	# read input file 
	open(oFILE, $env_file) or die "Can't open '$env_file': $!";
	binmode(oFILE);
	# read file into an array
	my @env_data = <oFILE>;	
	close oFILE;
	
	my $record_start = "no";
	foreach(@env_data){
		if($_ =~ /^[\s]*$/){
			next;
		}
		if($record_start =~ /^yes$/i){
			my $this_line = $_;
			my @this_lines = split /=/,$this_line;
			my $env_arg = "";
			my $env_cmd = "";
			###get args content
			$env_arg = 	$this_lines[0];
			$env_arg = $env_arg."=";
			###get cmds content
			$this_line =~ s/^$this_lines[0]=//;
			$env_cmd = $this_line;
			$env_cmd =~ s/\s*$//;
			###push into it
			push(@envs_args,$env_arg);
			push(@envs_cmds,$env_cmd);
		}
		if($_ =~ /^---/){
			$record_start = "yes";
		}
	}
	
	#foreach (@envs_args){print; print ",";} print "\n";
	#foreach (@envs_cmds){print; print ",";} print "\n";

	#find out $mem_excel
	foreach (@envs_cmds){
		if (/mem=([0-9]+)/){
			 $mem_excel = $1;
			 last;
		}
	}
	
	### u-boot-env.bin ###
		my $uenv_i = 0;
		my $uenv_str;
		open oFILE, ">" . $u_boot_env_file;
			foreach (@envs_args){
				if ($envs_cmds[$uenv_i] !~ /NULL/){
					$uenv_str = $envs_args[$uenv_i].$envs_cmds[$uenv_i].$envs_tack;
					foreach(unpack("(a1)*", $uenv_str)) {
					print oFILE;
					}
				}
				$uenv_i = $uenv_i + 1;
			}
		close oFILE;
		&padd_env($u_boot_env_file, $flash_type);	
}

sub padd32                             
{
	my $file = $_[0];
	my $pz_file = $_[1];
	
	my $filesize = 0;
	my @data_w;
	my $buffer;
	my $filebuff;
	my @vtime;
	my $i = 0;
	
	$filesize = -s $file;          
	open(oFILE, $file) or die "Can't open '$file': $!";     
	#binmode(oFILE);
	# read file into an array
	#@data_w = <oFILE>;	
	read(oFILE, $filebuff, $filesize - 32, 0);              
	close oFILE;
	
	open(oFILE, ">$pz_file") or die "Can't open '$pz_file': $!";
	#binmode(oFILE);
	#print oFILE @data_w;
	print oFILE $filebuff;                                       
	close oFILE;
	
	#print "============================\n";	
	open(oFILE, "<$file");
	seek(oFILE,$filesize - 32,0);                          
	read(oFILE, $buffer, 32, 0);                             
	close(oFILE);

	foreach (split(//, $buffer)) {
		$vtime[$i] = ord($_);                              
		$i++;
	}
	my $size = ($filesize + 4);
	my $pad = 0;
	$pad  = (($size + 0x1F) & (~0x1F)) - ($size);    


	if(($vtime[16]==50)&&        
	($vtime[20]==47)&&
	($vtime[25]==47)) {
	#uboot format
		open(oFILE, ">>$pz_file");        
		for (my $i=0;           
			$i < $pad;
			$i++){
			print oFILE pack('H2', 'FF');      
			}	
			
			foreach(@vtime){
				my $hexval = sprintf("%x", $_);	
				print oFILE pack('H2',$hexval);   
			}
		close oFILE;
	}
	else{                                                
	#others
		open(oFILE, ">>$pz_file");
		foreach(@vtime){
				my $hexval = sprintf("%x", $_);	
				print oFILE pack('H2',$hexval);
			}
		
		for (my $i=0;
			$i < $pad;
			$i++){
			print oFILE pack('H2', 'FF');
			}
		close oFILE;
	}
}

sub padd_env
{
	my $file = $_[0];
	my $type = $_[1];
	my $filesize;
	my $pad = 0;
	my $crc32 = 0;


	$filesize = -s $file;
	
	if ($type =~ /NAND/i){
		$pad = $NAND_BLK_SIZE - $filesize - 116;
	}
	elsif ($type =~ /SF/i){
		$pad = $SPI_BLK_SIZE - $filesize - 116 ;
	}
	else{
		print "flash type ERROR!!\n"
	}

	open(oFILE, ">>$file");	# Open for appending
	for (my $i=0;
			$i < $pad;
			$i++){
			print oFILE pack('H2', 'FF');
#			foreach(unpack("(a1)*", $envs_tack)) {
#					print oFILE;
#			}
	}
	close oFILE;

	#######
	#make $file to 16times large
	my $what_large = -s $file;
	until ($what_large < 16){
		$what_large -= 16;
  } 
  unless ($what_large == 0){
  	print "\n\n\n--------------------\n\n\n";
	  my $how_add = 16 - $what_large;
    open(oFILE, ">>$file");	# Open for appending
		while ($how_add > 0){
			$how_add --;
			my $add_str = sprintf("%s", "FF");
			my $str_done = pack("a2", $add_str); 
			print oFILE pack('H*',$str_done);
		}
		close oFILE;  
  }
  ###########

	### Attach CRC16 [16-bytes] ###
	my $crc16;
	my @message;
	my $message_bytes = -s $file;
	my $count=$message_bytes;
	my $index_message=0;
	
	open(GIF, $file) or die "can't open '$file': $!";
	binmode(GIF);
	while ($count > 0){
		$count--;
		read(GIF, $message[$index_message], 1);
		$message[$index_message] = unpack("C",$message[$index_message]);
		$index_message++;
	}
	close GIF;
	
	&crcSlow(
		\$crc16,
		\@crc16_tab, 
		\@message,       
		\$message_bytes
 	);


	my $filebuff;
	
	$filesize = -s $file;
	open(oFILE, $file) or die "Can't open '$file': $!";
	read(oFILE, $filebuff, $filesize , 0);
	close oFILE;
	
	open(oFILE, ">$file");	# Open for appending
	print oFILE pack('L',$filesize);
	print oFILE $filebuff;
	close oFILE;
	
#name
	open(oFILE, ">>$file");	# Open for appending
	my $str = pack( "a16", $U_BOOT_ENV_NAME); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
#version
	$str = pack("a64", $U_BOOT_ENV_VER); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
#time	
	$str = pack("a16", $NOW_TIME); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
#crc16
#  print "---$crc16-------\n";
#	$crc16 = sprintf("%s", $crc16);
#	$str = pack("a8", $crc16); 
#	print oFILE pack('H*',$str);
  print oFILE pack('L',$crc16);
	
	my $add_str = sprintf("%s", "FF");
	my $time_add = 12;
	while ($time_add){
		$time_add --;
		my $str_done = pack("a2", $add_str); 
		print oFILE pack('H*',$str_done);
	}	

	close (oFILE);

	
	open(oFILE, ">./env.d");	# Open for appending
	print oFILE $filebuff;
	close oFILE;
}

sub packize
{
	my $file = $_[0];
	my $pz_file = $_[1];
	my $name = $_[2];
	my $version = $_[3];
	my $time = $_[4];
	my $append_size = $_[5]; # 'S'
	my $spi_file_d = $_[6];
	my $nand_file_d = $_[7];
	my @data_w;
	my @data_spi;
	my @data_nand;
	my $filesize;
	my $str;
	
	#######
	#make $file to 16times large
	my $what_large = -s $file;
	until ($what_large < 16){
		$what_large -= 16;
	} 
	unless ($what_large == 0){
		my $how_add = 16 - $what_large;
		open(oFILE, ">>$file");	# Open for appending
		while ($how_add > 0){
			$how_add --;
			my $add_str = sprintf("%s", "FF");
			my $str_done = pack("a2", $add_str); 
			print oFILE pack('H*',$str_done);
		}
		close oFILE;  
	}
	###########

	### Attach CRC16 [16-bytes] ###
	my $crc16;
	my @message;
	my $message_bytes = -s $file;
	my $count=$message_bytes;
	my $index_message=0;
	
	open(GIF, $file) or die "can't open '$file': $!";
	binmode(GIF);
	while ($count > 0){
		$count--;
		read(GIF, $message[$index_message], 1);
		$message[$index_message] = unpack("C",$message[$index_message]);
		$index_message++;
	}
	close GIF;
	
	&crcSlow(
		\$crc16,
		\@crc16_tab, 
		\@message,       
		\$message_bytes
 	);
	################################
	
	open(oFILE, $file) or die "Can't open '$file': $!";
	binmode(oFILE);
	# read file into an array
	@data_w = <oFILE>;	
	close oFILE;
	
	#packize : file size
	if(($append_size eq "S")|| 
		($append_size eq "I")||
		($append_size eq "F"))
	{
		#open(oFILE, $pz_file) or die "Can't open '$pz_file': $!";
		open(my $out, '>:raw', $pz_file) or die "Unable to open: $!";
		$filesize = -s $file;
		print $out pack('L',$filesize);
		close $out;
		
		open(oFILE, ">>$pz_file");	# Open for appending
		print oFILE @data_w;
		close oFILE;
	}
	else{
		open(oFILE, ">$pz_file") or die "Can't open '$pz_file': $!";
		binmode(oFILE);
		print oFILE @data_w;
		close oFILE;
	}
	
	if($append_size eq "F"){
		open(oFILE, $spi_file_d) or die "Can't open '$file': $!";
		binmode(oFILE);
		@data_spi = <oFILE>;	
		close oFILE;
	
		open(oFILE, $nand_file_d) or die "Can't open '$file': $!";
		binmode(oFILE);
		@data_nand = <oFILE>;	
		close oFILE;
		
		open(oFILE, ">>$pz_file");	# Open for appending
		print oFILE @data_spi;
		close oFILE;
		
		open(oFILE, ">>$pz_file");	# Open for appending
		print oFILE @data_nand;
		close oFILE;
	}
	
	#packize : name/version/tm/md5
	open(oFILE, ">>$pz_file");	# Open for appending
	$str = pack( "a16", $name); 
	
	if($append_size ne "I") {
		foreach(unpack("(a1)*", $str)) {
			print oFILE;
		}
	}
	
	$str = pack("a64", $version); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
	
	$str = pack("a16", $time); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}

	################################
#	$crc16 = sprintf("%s", $crc16);
#	$str = pack("a8", $crc16); 
#	print oFILE pack('H*',$str);
	print oFILE pack('L',$crc16);
	
	my $add_str = sprintf("%s", "FF");
	my $time_add = 12;
	while ($time_add){
		$time_add --;
		my $str_done = pack("a2", $add_str); 
		print oFILE pack('H*',$str_done);
	}
	
	close oFILE;
}

sub packize_crc32
{
	my $file = $_[0];
	my $pz_file = $_[1];
	my $name = $_[2];
	my $version = $_[3];
	my $time = $_[4];
	my $append_size = $_[5]; # 'S'
	my $spi_file_d = $_[6];
	my $nand_file_d = $_[7];
	my @data_w;
	my @data_spi;
	my @data_nand;
	my $filesize;
	my $str;
	
	open(oFILE, $file) or die "Can't open '$file': $!";
	binmode(oFILE);
	# read file into an array
	@data_w = <oFILE>;	
	close oFILE;
	
	#packize : file size      
	if($append_size eq "S")                           #  N
	{
		#open(oFILE, $pz_file) or die "Can't open '$pz_file': $!";
		open(my $out, '>:raw', $pz_file) or die "Unable to open: $!";
		$filesize = -s $file;
		print $out pack('L',$filesize) ;
		close $out;
		
		open(oFILE, ">>$pz_file");	# Open for appending
		print oFILE @data_w;
		close oFILE;
	}
	else{
		open(oFILE, ">$pz_file") or die "Can't open '$pz_file': $!";
		binmode(oFILE);
		print oFILE @data_w;                   # copy  ./Image/u-logo.bmp[0]  to  ulogo.bin.d[1]
		close oFILE;
	}
	
	#padding 32 bytes
	$filesize = -s $pz_file;
	my $pad  = (($filesize + 0x1F + 32) & (~0x1F)) - $filesize;
	open(oFILE, ">>$pz_file");
	#zeroing to size
		for (my $i=0;
			$i < $pad;
			$i++){
			print oFILE pack('H2', 'FF');
			}	
	close oFILE;
	
	#######
	#make $file to 16times large
	my $what_large = -s $pz_file;
	until ($what_large < 16){
		$what_large -= 16;
	} 
	unless ($what_large == 0){
		my $how_add = 16 - $what_large;
		open(oFILE, ">>$pz_file");	# Open for appending
		while ($how_add > 0){
			$how_add --;
			my $add_str = sprintf("%s", "FF");
			my $str_done = pack("a2", $add_str); 
			print oFILE pack('H*',$str_done);
		}
		close oFILE;  
	}
	###########
	### Attach CRC16 [16-bytes] ###
	my $crc16;
	my @message;
	my $message_bytes = -s $pz_file;
	my $count=$message_bytes;
	my $index_message=0;
	
	open(GIF, $pz_file) or die "can't open '$pz_file': $!";
	binmode(GIF);
	while ($count > 0){
		$count--;
		read(GIF, $message[$index_message], 1);
		$message[$index_message] = unpack("C",$message[$index_message]);
		$index_message++;
	}
	close GIF;
	
	&crcSlow(
		\$crc16,
		\@crc16_tab, 
		\@message,       
		\$message_bytes
 	);
	################################
	
	
	$filesize = -s $pz_file;
	print "*******logo size:  $filesize************\n";
	open(oFILE, $pz_file) or die "Can't open '$pz_file': $!";
	binmode(oFILE);
	# read file into an array
	@data_w = <oFILE>;	
	close oFILE;
	
	open(oFILE, ">$pz_file") or die "Can't open '$pz_file': $!";
	binmode(oFILE);
	print oFILE pack('L',$filesize);
	print oFILE @data_w; 
	close oFILE;
	

	#packize : name/version/tm/crc16
	open(oFILE, ">>$pz_file");	# Open for appending
	$str = pack( "a16", $name); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
	
	$str = pack("a64", $version); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
	
	$str = pack("a16", $time); 
	foreach(unpack("(a1)*", $str)) {
		print oFILE;
	}
	
#	print "*******logo crc16:  $crc16************\n";
#	$crc16 = sprintf("%s", $crc16);
#	my $str_crc = pack("a8", $crc16); 
#	print oFILE pack('H*',$str_crc);
	print oFILE pack('L',$crc16);
	
	my $add_str = sprintf("%s", "FF");
	my $time_add = 12;
	while ($time_add){
		$time_add --;
		my $str_done = pack("a2", $add_str); 
		print oFILE pack('H*',$str_done);
	}
	
	close oFILE;
}

sub fun_flash_layout_header
{
	my (
			$filelayout_file, 
			$filelayout_type, 
			$flash_layout_value_para	#@                      
	) = @_;	 
	open oHEADER, ">" . $$filelayout_file or die "Can't open '$$filelayout_file': $!";
	print oHEADER "/*\n";
	print oHEADER "*	NAND\n";
	print oHEADER "*/\n";
	my $flash_index = 0;
	my $flash_value ="0x00000000";
	foreach (@flash_nand){
  	$_ =~s/-/_/g;
  	if($$filelayout_type =~ /NAND/i){
  		$flash_value = $$flash_layout_value_para[$flash_index];
  	}
		print oHEADER "#define    ";
		print oHEADER "NAND_".uc($_)."_STR";
		print oHEADER "\t".$flash_value;
		print oHEADER "\n";
		$flash_index ++;
			
		if($$filelayout_type =~ /NAND/i){
  		$flash_value = $$flash_layout_value_para[$flash_index];
  	}	
		print oHEADER "#define    ";
		print oHEADER "NAND_".uc($_)."_END";
		print oHEADER "\t".$flash_value;
		print oHEADER "\n";
		$flash_index ++;
	}
	print oHEADER "\n\n";
	print oHEADER "/*\n";
	print oHEADER "*	SPI\n";
	print oHEADER "*/\n";
	$flash_index = 0;
	$flash_value ="0x00000000";
	foreach (@flash_spi){
  	$_ =~s/-/_/g;
  	if($$filelayout_type =~ /SF/i){
  		$flash_value = $$flash_layout_value_para[$flash_index];
  	}
		print oHEADER "#define    ";
		print oHEADER "SPI_".uc($_)."_STR";
		print oHEADER "\t".$flash_value;
		print oHEADER "\n";
		$flash_index ++;
			
		if($$filelayout_type =~ /SF/i){
  		$flash_value = $$flash_layout_value_para[$flash_index];
  	}	
		print oHEADER "#define    ";
		print oHEADER "SPI_".uc($_)."_END";
		print oHEADER "\t".$flash_value;
		print oHEADER "\n";
		$flash_index ++;
	}
  close(oHEADER);

}

sub fun_flash_layout_bin
{
	my (
			$filelayout_file, 
			$filelayout_type, 
			$flash_layout_value_para	#@                      
	) = @_;	 

	my $flash_index = 0;
		
	open oFILE, ">" . $$filelayout_file;
	if ($$filelayout_type =~ /NAND/i){
	my $i = 0;
	foreach (@flash_nand){
		my $str	= $$flash_layout_value_para[$flash_index];
		$flash_index ++;
		my $end = $$flash_layout_value_para[$flash_index];
		$flash_index ++;
				
		$str =~ s/\s+//g;
		$end =~ s/\s+//g;
					
		my	@p32u_str = split(/x/, $str);
		my $ff32u_str = sprintf "%08d", hex($p32u_str[1]);
		print oFILE pack('L', $ff32u_str);
					
		my	@p32u_end = split(/x/, $end);
		my $ff32u_end = sprintf "%08d", hex($p32u_end[1]);
		print oFILE pack('L', $ff32u_end);
		}
	}
	elsif ($$filelayout_type =~ /SF/i){
		my $i = 0;
		$flash_index = 0;
		foreach (@flash_spi){
		my $str	= $$flash_layout_value_para[$flash_index];
		$flash_index ++;
		my $end = $$flash_layout_value_para[$flash_index];
		$flash_index ++;
					
		$str =~ s/\s+//g;
		$end =~ s/\s+//g;
					
		my	@p32u_str = split(/x/, $str);
		my $ff32u_str = sprintf "%08d", hex($p32u_str[1]);
		print oFILE pack('L', $ff32u_str);
					
		my	@p32u_end = split(/x/, $end);
		my $ff32u_end = sprintf "%08d", hex($p32u_end[1]);
		print oFILE pack('L', $ff32u_end);
		}
	}
	else{
		print "Error !! Flash Type Mistake....\n"
	}
		
	close oFILE;
	
	#######
	#make $file to 16times large
	my $what_large = -s $$filelayout_file;
	until ($what_large < 16){
		$what_large -= 16;
	} 
	unless ($what_large == 0){
	  my $how_add = 16 - $what_large;
		open(oFILE, ">>$$filelayout_file");	# Open for appending
		while ($how_add > 0){
			$how_add --;
			my $add_str = sprintf("%s", "FF");
			my $str_done = pack("a2", $add_str); 
			print oFILE pack('H*',$str_done);
		}
		close oFILE;  
	}
	###########	
	
	
	### Attach CRC16 [16-bytes] ###
	my $crc16;
	my @message;
	my $message_bytes = -s $$filelayout_file;
	my $count=$message_bytes;
	my $index_message=0;
	
	open(GIF, $$filelayout_file) or die "can't open '$$filelayout_file': $!";
	binmode(GIF);
	while ($count > 0){
		$count--;
		read(GIF, $message[$index_message], 1);
		$message[$index_message] = unpack("C",$message[$index_message]);
		$index_message++;
	}
	close GIF;
	
	&crcSlow(
		\$crc16,
		\@crc16_tab, 
		\@message,       
		\$message_bytes
 	);
	################################
		
	my $flash_layout_size = -s $$filelayout_file;
	
	
	open(oFILE, ">>$$filelayout_file");	# Open for appending
	#print oFILE pack('H*',$flash_layout_md5);
	################################
#	$crc16 = sprintf("%s", $crc16);
#	my $str_crc = pack("a8", $crc16); 
#	print oFILE pack('H*',$str_crc);
	print oFILE pack('L',$crc16);
	
	my $add_str = sprintf("%s", "FF");
	my $time_add = 12;
	while ($time_add){
		$time_add --;
		my $str_done = pack("a2", $add_str); 
		print oFILE pack('H*',$str_done);
	}
	
	#zeroing to size
	for (my $i=0;
		$i < ($flash_layout_file_size - $flash_layout_size -16);
		$i++){
			print oFILE pack('H2', 'FF');
		}	
	close oFILE;
}


sub gettime
{
	my $now_string = localtime;  # e.g., "Thu Oct 13 04:54:34 1994"
	my @now_time = split(/ +/, $now_string);
	$now_time[3] =~ s/://g; #hrs:min:sec
	
	my $month = $now_time[1];
	my $mth;
	
	if ($month	=~	/Jan/){		$mth = "01";}
	elsif ($month	=~	/Feb/){ $mth = "02";}
	elsif ($month	=~	/Mar/){ $mth = "03";}
	elsif ($month	=~	/Apr/){ $mth = "04";}
	elsif ($month	=~	/May/){ $mth = "05";}
	elsif ($month	=~	/Jun/){ $mth = "06";}
	elsif ($month	=~	/Jul/){ $mth = "07";}
	elsif ($month	=~	/Aug/){ $mth = "08";}
	elsif ($month	=~	/Sep/){ $mth = "09";}
	elsif ($month	=~	/Oct/){ $mth = "10";}
	elsif ($month	=~	/Nov/){ $mth = "11";}
	elsif ($month	=~	/Dec/){	$mth = "12";} 

	my $what_size = length($now_time[2]);
	if($what_size == 1){
		$now_time[2] = "0".$now_time[2];
	}
	my $time_string = $now_time[4]."/".$mth.$now_time[2]."/".$now_time[3];
	
	$time_string;
}

#calculate crc16
sub crcSlow
{
	my (
	     $crc_16_value, 
	     $crc_16_tab,  #@
	     $crc_message, #@
	     $crc_bytes
	   ) = @_;
	   
	my $crc_count=$$crc_bytes;
	my $crc_temp;
	my $crc_sum=0xffff;
	my $message_index="0";
	
	while ($crc_count > "0"){
		$crc_count--;
		$crc_temp = $$crc_message[$message_index] ^ $crc_sum;
		my $temp_x = sprintf ("%04x",$crc_temp);
		$temp_x =~ s/^[0-9a-zA-Z][0-9a-zA-Z]//i;
		my $temp_d = hex($temp_x);
		$crc_sum >>= 8;
		$crc_sum ^= $$crc_16_tab[$temp_d];
		$message_index++;	
	}
	$$crc_16_value = $crc_sum;
}
