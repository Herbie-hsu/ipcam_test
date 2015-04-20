#include <stdio.h>
#include <stdlib.h>
#include <zbar.h>
#include <sys/stat.h>
#include <signal.h> 
#define video_dev "/dev/video0"

#define WIFI_TEMP_FILE "/tmp/wifi_temp"

int init_isp(const char *dev, int width, int height, int rate);
int get_frame(char *data);
int close_isp();

zbar_image_scanner_t *scanner = NULL;

static int gQuit_scan_flag = 1;
static int gRec_success_flag = 0;

/* signal handler to disable qr scan process */
static void sig_handler(void)
{
	fprintf(stderr, "Receive signal to terminate \n");
	gQuit_scan_flag = 0;
	gRec_success_flag = 0;
}

int main (int argc, char **argv)
{
	int finish = 1;
	int size;
	char *data = NULL;
	FILE *fp = NULL;
	int width = 640, height = 480, rate = 30;
	char buf[640*480];

	/* add signal handler */
	signal(SIGINT, (__sighandler_t)sig_handler); 
	signal(SIGQUIT, (__sighandler_t)sig_handler);
	signal(SIGTERM, (__sighandler_t)sig_handler);
	
	/* create a reader */
	scanner = zbar_image_scanner_create();
	/* configure the reader */
	zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

	/* obtain image data */
	if (argc >= 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		if (argc > 3)
			rate=atoi(argv[3]);
	}
	printf("width=%d, height=%d, rate=%d \n", width, height, rate);
	size = width * height;

	init_isp(video_dev, width, height, rate);

	/* wrap image data */
	zbar_image_t *image = zbar_image_create();
	zbar_image_set_format(image, *(int*)"Y800");
	zbar_image_set_size(image, width, height);
	while (gQuit_scan_flag) {
		int n;
		const zbar_symbol_t *symbol;

		get_frame(buf);
		get_frame(buf);
		get_frame(buf);
		zbar_image_set_data(image, buf,size, zbar_image_free_data);

		/* scan the image for barcodes */
		n = zbar_scan_image(scanner, image);//cost mush time if the picture's size is very big
		/* extract results */
		symbol = zbar_image_first_symbol(image);
		for (; symbol; symbol = zbar_symbol_next(symbol)) {
			/* do something useful with results */
			zbar_symbol_type_t type = zbar_symbol_get_type(symbol);
			data = zbar_symbol_get_data(symbol);
			printf("decoded %s symbol \"%s\"\n", zbar_get_symbol_name(type), data);
			gQuit_scan_flag = 0;
			gRec_success_flag = 1;
			break;
		}
	}

	if (gRec_success_flag == 1) {
		fprintf(stderr, "extract code done!! \n");
		fp = fopen(WIFI_TEMP_FILE, "wb");
		if (fp == NULL) {
			fprintf(stderr, "[%s] open %s failed.\n", __FUNCTION__, WIFI_TEMP_FILE);
		} else {
			fwrite(data, strlen(data), 1, fp);
			fclose(fp);
		}
	}

	/* clean up */
	zbar_image_destroy(image);
	
	zbar_image_scanner_destroy(scanner);
	close_isp();

	return(0);
}
