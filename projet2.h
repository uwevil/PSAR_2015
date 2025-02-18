//
//  projet2.h
//
//
//  Created by Cao Sang DOAN on 18/03/15.
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
#include <CommonCrypto/CommonDigest.h>

#define CHUNKS 2

struct vec{
    int index;
    struct vec *next;
};

struct list_va{
    int size;
    int id[CHUNKS];
    struct vec *va;
    struct list_va *next;
};

int create_database(char *file_name);

// lire un fichier entrée de type titre;mot_cles;rank;*
// creer un fichier sortie de type index titre filtredeBloom
int create_filter(char *file_name); // 20, 28, 32, 48, 64 byte

// avec index et fichier de filtres retourner le filtre et le titre du doc
char *read_filter(char *file_name, unsigned int index, char **titre_return); // return titre

// convertir hexadecimal en decimal
unsigned int hex2int(char *a, unsigned int len);
char int2hex(int a);

char *union2filter(char *f1, char *f2, int len);
int insert_va(struct list_va *listeva, struct vec *va);

int add_va(struct list_va *listeva, int tab[], int len_tab, int index);

int find_index(char *file_name, struct list_va *listeva, int tab[], int len_tab);
int find_request(char *file_name, struct list_va *l, struct list_va *list_req);
struct list_va *create_request(char *mot_cle);
int *create_va(char *mot);

int create_va_file(char *file_name, struct list_va *listeva);

#endif /* defined(____projet__) */
