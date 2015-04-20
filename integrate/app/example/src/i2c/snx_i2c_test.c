/* please put ov971x.ko & snx_i2c_test in the same path */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
int main ()
{
  /* must kill stream server process first */
  system("killall sonix_proj");
  /* rmmod isp driver*/
  system("rmmod  snx_isp");
  /* rmmod old ov971x driver*/
  system("rmmod  ov9715");
  /*insmod the simple i2c_tov9715_simple driver  build from example/i2c driver*/
  system("insmod ../../../lib/example/i2c/ov971x.ko");
  /*modprobe the sonix isp driver to run i2c sample */
  system("modprobe snx_isp");
  return 0;
}
