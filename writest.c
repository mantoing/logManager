//
// Description : Write stream socket 
// File Name   : writest.c
// Date        : 2013. 10. 23. 09:39:17 KST
// By          : YJYOON
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#ifndef MAX_BUFF
#define MAX_BUFF 62580 /* 255 * 256 + 255 */
#endif
/*******************************************************************************
 * 
 * Prototype : int WriteSize(fd, buff, size)
 * Arguments : int fd, size; char *buff;
 * Return    : 
 ******************************************************************************/
inline int WriteSize(int fd,char *buff,int sz)
{
int rtn;
char *p;
    p = buff;
    do{
        rtn= write(fd, p, sz);
        if(rtn < sz){ sz -=rtn;p += rtn;}
        else if(rtn == sz) break;
        else return 0;
    }while(1);
    return 1;
}
/******************************************************************************/
/* Write data                                                                 */
/* Prototype : int WriteStream(fd, buff, sz)                                  */
/* Arguments : int   fd; Socket FD(I)                                         */
/*             char *buff;Data buffer(I)                                      */
/*             int   sz;  Data size                                           */
/* Return    : int ; SUCESS : write size, FAIL : errno                        */
/******************************************************************************/
int WriteStream(int fd,char *buff,int sz)
{
int  rtn;
char temp[MAX_BUFF];
	memcpy(temp, (char *)&sz, sizeof(int));	
	memcpy(temp + sizeof(int), buff, sz);
	sz += sizeof(int);
	rtn = write(fd, temp, sz);
	if(rtn <= 0)return  -1;
	return sz - sizeof(int);
}
/******************************************************************************/
/* Write data                                                                 */
/* Prototype : int WriteStream2(fd, buff, sz)                                 */
/* Arguments : int   fd; Socket FD(I)                                         */
/*             char *buff;Data buffer(I)                                      */
/*             int   sz;  Data size                                           */
/* Return    : int ; SUCESS : write size, FAIL : errno                        */
/******************************************************************************/
int WriteStream2(int fd,char *buff,int sz)
{
int           rtn;
unsigned char fs, ls, temp[MAX_BUFF];
	fs = (unsigned char)(sz / 256);
	ls = (unsigned char)(sz - fs * 256);
	temp[0] = fs;temp[1] = ls;
	memcpy(temp + 2, buff, sz);
	rtn = write(fd, temp, sz + 2);
	if(rtn <= 0)return -1;
	return sz;
}
