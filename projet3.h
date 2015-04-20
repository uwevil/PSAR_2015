//
//  projet3.h
//
//
//  Created by Cao Sang DOAN on 24/03/15.
//
//


#ifndef ____projet__
#define ____projet__

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <CommonCrypto/CommonDigest.h>

#define DIR "./"
#define VA_FILE "test/va_file"

#define MIN_FILTER 16
#define MAX_FILTER 512     //bit
#define MAX_CHUNKS_SIZE 8  //bit
#define MAX_DIMENSION 6    //2^6 = 64
#define MAX_STORE 512      //seuil

extern int min_dimension;

unsigned long hex2int(char *a, unsigned int len);
char *create_filter(char *keyword, int size_filter);
int generator_filter(char *name);
int create_file_vector(char *vector, char *data);
char *find_file_name_vector(char *vector);
char *create_vector(char *filtre, int n);
unsigned long bit2int(char *a, unsigned int len);
char *int2bit(int a, int len);
int put(char *filtre);


#endif /* defined(____projet__) */
