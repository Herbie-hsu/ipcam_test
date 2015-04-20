#ifndef _SNX_CONF_H
#define _SNX_CONF_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _elem{
char *key;
char **values;
int value_num;
char *comment;
} elem;
typedef struct _llist{
elem data;
struct _llist *next;
} Llist;


/*
*Create configuration file , if filename exists or create file error ,return -1,other return 0. 
*/
int snx_createconf(const char *filename);

/*
*Open file and read data to list,if error return -1,other return 0. 
*/
int snx_openconf(const char *filename,Llist** list);

/*
*Set value by key in list ,if error ,return -1,other return 0. 
*/
int snx_setvalue(Llist *list,const char *key,const char *value);

/*
*Get value by key from list ,if error ,return -1,other return 0. 
*/
int snx_getvalue(Llist *list,const char *key,char *value);

/*
*Set value by key and value index in list ,if error ,return -1,other return 0. 
*/
int snx_setvalue_index(Llist *list,const char *key, int index,const char *value);

/*
*Get value by key and value indexfrom list ,if error ,return -1,other return 0. 
*/
int snx_getvalue_index(Llist *list,const char *key, int index, char *value);

/*
*Delete key and its value from list ,if error ,return -1,other return 0. 
*/
int snx_delconf(Llist *list,const char *key);

/*
*Add key and its value to list ,if error ,return -1,other return 0. 
*/
int snx_addconf(Llist *list,const char *key,const char *value);

/*
*Write list data to file and free list, if filename is NULL ,so  olny free list.if error ,return -1,other return 0. 
*/
int snx_closeconf(const char *filename,Llist *list);



/*
*This group functions is the same as above functions,but need not call snx_openconf and snx_closeconf by yourself.
*/
int snx_setvalue2(const char *filename,const char *key,const char *value);
int snx_getvalue2(const char *filename,const char *key,char *value);
int snx_setvalue_index2(const char *filename,const char *key, int index,const char *value);
int snx_getvalue_index2(const char *filename,const char *key, int index, char *value);
int snx_delconf2(const char *filename,const char *key);
int snx_addconf2(const char *filename,const char *key,const char *value);

#ifdef __cplusplus
}
#endif

#endif

