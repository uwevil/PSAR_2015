//
//  projet3.c
//
//
//  Created by Cao Sang DOAN on 24/03/15.
//
//

#include "projet3.h"

int min_dimension = 6;

char *create_vector(char *filtre,int n){
    if ((n > 6) || (n < 2)) {
        printf("create_vector: n > 6 or n < 2\n");
        return NULL;
    }
    if (strlen(filtre) != 512) {
        printf("create_vector: filtre != 512\n");
        return NULL;
    }
    char vector[256], *tmp;
    //vector = (char *)malloc(sizeof(char));
    tmp = strdup(filtre);
    int i, j = 0;

    for (i = 0; i < 512; i = i + 8*pow(2, MAX_DIMENSION - n)){
        strncpy(vector+j, tmp+i, 4);
        j = j+4;
    }
    vector[j] = '\0';

    return strdup(vector);
}

unsigned int bit2int(char *a, unsigned int len){
    int i;
    unsigned int val = 0;
    
    if (strlen(a) < len) {
        printf("bit2int: strlen(a) < len\n");
        exit(1);
    }
    for (i = 0; i < len; i++) {
        switch (a[i]) {
            case '1':
                if (isdigit(a[i]) == 0) {
                    printf("bit2int: erreur not a number %c!\n", a[i]);
                    exit(1);
                }
                val = val + (1<<((len - 1 - i)));
                break;
            default:
                if (isdigit(a[i]) == 0) {
                    printf("bit2int: erreur not a number %c!\n", a[i]);
                    exit(1);
                }
                break;
        }
    }
    return val;
}

char *int2bit(int a, int len){
    int i;
    int tmp = a;
    
    if ((a > (pow(2, len) - 1)) || (a < 0)) {
        printf("int2bit: erreur a et len\n");
        exit(1);
    }
    
    char *val = (char *)malloc(sizeof(char));
    for (i = 0; i < len; i++) {
        if ((1<<(len - i - 1)) <= tmp) {
            val[i] = '1';
            tmp = tmp - (1<<(len - i - 1));
        }else{
            val[i] = '0';
        }
    }
    val[i] = '\0';
    return val;
}

int PUT(char *filtre){
    FILE *f, *v;
    static char file_name[7] = VA_FILE;
    static char file_vector[512];
    
    f = fopen(file_name, "r+");
    if (f == NULL) {
        f = fopen(file_name, "w+");
        if (f == NULL) {
            printf("PUT: error create %s\n", file_name);
            exit(1);
        }
    }
    
    unsigned char md[8];
    char vector[2*sizeof(md) + 1];
    char *vector_tmp = create_vector(filtre, min_dimension);
    int i;
    
    CC_SHA1_CTX c;
    CC_SHA1_Init(&c);
    CC_SHA1_Update(&c, (const void *)vector_tmp, strlen(vector_tmp));
    CC_SHA1_Final(md, &c);
    
    for (i = 0; i < sizeof(md); i++) {
        snprintf(vector+(2*i), 3, "%02x\n", (int)md[i]);
    }
    
    strncat(file_vector, "vector_", 7);
    strncat(file_vector, vector, 16);

    v = fopen(file_vector, "r+");
    if (v == NULL) {
        v = fopen(file_vector, "w+");
        if (v == NULL) {
            printf("PUT: error create %s\n", file_vector);
            exit(1);
        }
    }

    char tmp[600];
    int index = -1;
    char *tmp2;
    
    while (fgets(tmp, 600, v) != NULL) {
        tmp[strlen(tmp) - 1] = '\0';
        index = strtol(strtok(tmp, ";"), NULL, 10);
        tmp2 = strtok(NULL, ";");
        if (strcasecmp(tmp2, filtre) == 0) {
            printf("PUT: existe déjà %s\n", filtre);
            return 1;
        }
    }
    fprintf(v, "%d;%s\n", index + 1, filtre);
    fflush(v);

    index = -1;
    while (fgets(tmp, 600, f) != NULL) {
        tmp[strlen(tmp) - 1] = '\0';
        index = strtol(strtok(tmp, ";"), NULL, 10);
        tmp2 = strtok(NULL, ";");
        
        if (strcasecmp(tmp2, vector_tmp) == 0) {
            printf("PUT: vecteur existe déjà %s\n", vector_tmp);
            return 1;
        }
        
    }
    if (index < MAX_STORE) {
        fprintf(f, "%d;%s\n", index + 1, vector_tmp);
        fflush(f);
    }else{
        min_dimension--;
        fclose(f);
        fclose(v);
        
        
        
    }

    fclose(f);
    fclose(v);
    return 0;
}

int main(int argc, char **argv){
    char filtre[512];

    int i;

    for (i = 0; i < 512; i++) {
        filtre[i] = i < 256 ? '0' : '1';
    }
    filtre[i] = '\0';
    filtre[6] = '1';
    PUT(filtre);

    return 0;
}











