/**********************************************************************
 *
 * Function:   MCU 2511 proction IC 
 * How it works: Host issues a i2c write commands with data (0xa8 0x0) to
 *               MCU (slave addr: 0xA8) periodly. 
 *               MCU will issue reset signal to host if no i2c communication
 *               in 6 seconds. 
 *
 **********************************************************************/
 
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
 #include <getopt.h>

#define I2C_BUS 			"/dev/i2c-0"
#define MCU_SLAVE_ADDR		0xA8
#define MCU_OP_CODE1		0xA8
#define MCU_OP_CODE2		0x0

int snx_i2c_open(char *dev)
{
	int fd;
	fd = open(dev, O_RDWR);
	if (fd < 0) {
		printf("open %s failed\n", dev);
		exit(1);
	}
	return fd;
}

int snx_i2c_close(int fd)
{
        return close(fd);
}

int snx_i2c_write(int fd, int chip_addr, int addr, int data)
{
	struct i2c_msg msgs[1];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;
	__u8 buf[2];

	buf[0] = addr;
	buf[1] = data;
	msgs[0].addr = chip_addr;
	msgs[0].flags = 0;				//Write Operation
	msgs[0].len = 2;				
	msgs[0].buf = buf;

	ioctl_data.nmsgs = 1;
	ioctl_data.msgs = msgs;
	ret = ioctl(fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		printf("%s: ioctl return: %d\n", __func__, ret);
	}

	return ret;
}

int snx_i2c_burst_read(int fd, int chip_addr, int start_addr, int end_addr, void *data)
{
	struct i2c_msg msgs[2];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;

	msgs[0].addr = chip_addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = data;
	((__u8 *)data)[0] = start_addr;
	
	msgs[1].addr = chip_addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = end_addr-start_addr+1;
	msgs[1].buf = data;
	
	ioctl_data.nmsgs = 2;
	ioctl_data.msgs = msgs;
	ret = ioctl(fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		printf("%s: ioctl return: %d\n", __func__, ret);
	}

	return ret;
}

int snx_i2c_read(int fd, int chip_addr, int addr)
{
	__u8 value = -1;

	snx_i2c_burst_read(fd, chip_addr, addr, addr, &value);

	return value;
}

int snx_mcu_querry(int fd, int chip_addr, void *data)
{
	struct i2c_msg msgs[2];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;

	msgs[0].addr = chip_addr;
	msgs[0].flags = 0;
	msgs[0].len = 2;
	msgs[0].buf = (__u8 *)data;
	
	ioctl_data.nmsgs = 1;
	ioctl_data.msgs = msgs;
	ret = ioctl(fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		printf("%s: ioctl return: %d\n", __func__, ret);
	}
	return ret;
}
static void usage(FILE * fp, int argc, char ** argv) {   
    fprintf(fp, "Usage: %s [options]/n\n"   
        "Options:\n"
        "-C cmd 0:write 1:read 2:mcu querry\n"  
        "-a address\n"
        "-r register\n"
        "-v value \n"
        "", argv[0]);   
}  
static const char short_options[] = "C:a:r:v:o";   
static const struct option long_options[] =
{
	     { "help", no_argument, NULL, 'h' },
	     { "command", required_argument, NULL, 'C' },
       { "address", required_argument, NULL, 'a' }, 
       { "reg", required_argument, NULL, 'r' },
       { "value", required_argument, NULL, 'v' }, 
       { "o", required_argument, NULL, 'o' },    
       { 0, 0, 0, 0 }
}; 
/*
* main()
*/
int main(int argc,char *argv[])
{                          

	int reg = -1; 
  int value = -1; 
  int addr = -1;
  int status = -1;
	int fd;
	char opcode[2] = {MCU_OP_CODE1, MCU_OP_CODE2};
	addr = MCU_SLAVE_ADDR;

	fd = snx_i2c_open(I2C_BUS);
  for (;;)
	{
    int index;   
    int c;   

    c = getopt_long(argc, argv, short_options, long_options, &index);

    if (-1 == c)   
			break;   
		switch (c)
		{   
			case 0: /* getopt_long() flag */   
				break;
			case 'h':   
				usage(stdout, argc, argv);   
				exit(0);   
			case 'C':   
        sscanf(optarg, "%d", &status);
				break;   
			case 'a':
				sscanf(optarg, "%x", &addr);
				break;
			case 'r':
				sscanf(optarg, "%x", &reg);
				break;
			case 'v':
				sscanf(optarg, "%x", &value);
				break;
			default:   
				usage(stderr, argc, argv);   
				exit(0);   
		}    
  }
  
	if(fd > 0) {
	
		if (status == 0) {
      if (addr == -1 || reg == -1 || value == -1)
      {
        usage(stdout, argc, argv);   
				goto EXIT;  
      }
      snx_i2c_write(fd, addr, reg, value);
      printf("addr: %x set 0x%x = 0x%x\n", addr, reg, value);
			goto EXIT;
    }		
		else if (status == 1) {
      if (addr == -1 || reg == -1)
      {
        usage(stdout, argc, argv);   
				goto EXIT;  
      }
      printf("addr: %x get 0x%x = 0x%x\n", addr, reg, snx_i2c_read(fd, addr, reg));
			goto EXIT;
    }
		
    else if (status == 2){
      printf("[SNX MCU CTL] Slave addr: 0x%x, Op code: 0x%x 0x%x\n", MCU_SLAVE_ADDR, MCU_OP_CODE1, MCU_OP_CODE2);
		  while (1) {
		
			 if(snx_mcu_querry(fd, MCU_SLAVE_ADDR, opcode) < 0)
				  break;
			
			 usleep(5000000);
		  }
    }
    else
      usage(stdout, argc, argv);     
        
EXIT:
	snx_i2c_close(fd);
  }
	return 0;
}


