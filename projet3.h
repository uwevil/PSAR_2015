//
//  projet3.h
//
//
//  Created by Cao Sang DOAN on 24/03/15.
//
//


#ifndef ____projet__
#define ____projet__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <CommonCrypto/CommonDigest.h>

#define VA_FILE "va_file"

#define MAX_FILTRE 512     //bit
#define MAX_CHUNKS_SIZE 8  //bit
#define MAX_DIMENSION 6    //2^6 = 64
#define MAX_STORE 512      //seuil

extern int min_dimension;

char *create_vector(char *filtre, int n);
unsigned int bit2int(char *a, unsigned int len);
char *int2bit(int a, int len);


#endif /* defined(____projet__) */
