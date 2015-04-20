#ifndef __HW_SETTING_H__
#define __HW_SETTING_H__

#include <stdio.h>

#define WRITE_FEILD_NR 2
#define DELAY_FEILD_NR 1
#define BYPASS_FEILD_NR 1

struct write_struct {
	unsigned int tag;
	unsigned int data[WRITE_FEILD_NR];
};

struct delay_struct {
	unsigned int tag;
	unsigned int data[DELAY_FEILD_NR];
};

struct bypass_struct {
	unsigned int data[DELAY_FEILD_NR];
};


#define LINE_MAX_LEN 128

#define WRITE_TAG_CHAR 'W'
#define DELAY_TAG_CHAR 'N'
#define COMME_TAG_CHAR '#'
#define SEM_TAG_CHAR ','
#define BYPASS_TAG_CHAR 'B'

#define WRITE_TAG 0x00000077
#define DELAY_TAG 0x0000006E
#define BYPASS_TAG 0x00000042

#define PADING 0XFFFFFFFF

int hw_setting_image(FILE *in_fp, FILE * out_fp);

#endif /* __HW_SETTING_H__ */
