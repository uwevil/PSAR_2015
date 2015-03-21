#include "projet2.h"

int create_database(char *file_name){
    FILE *file_in, *file_out;
    
    static char file_name2[128];
    strcpy(file_name2, file_name);
    strcat(file_name2, "_database");
    
    file_in = fopen(file_name, "r");
    file_out = fopen(file_name2, "w");
    
    if (file_out == NULL){
        printf("create_database: fopen %s == NULL\n", file_name);
        exit(1);
    }
    if (file_in == NULL){
        printf("create_database: fopen %s == NULL\n", file_name2);
        exit(1);
    }
    
    printf("create_database/ %s => OK\n", file_name2);
    
    char tmp[512];
    char *titre;
    char *mot_cle;
    int page_rank, index;
    char *filtre;
    
    if (fgets(tmp, 512, file_in) == NULL) {
        printf("create_database: fichier de taile vide\n");
        exit(1);
    }
    
    fprintf(file_out, "%s;%s;%s\n", "index", "titre", "mot_cle");
    while (fgets(tmp, 512, file_in) != NULL){
        titre = strtok(tmp, ";");
        mot_cle = strtok(NULL, ";");
        page_rank = atoi(strtok(NULL, ";"));
        filtre = strtok(NULL, ";");
        fprintf(file_out, "%d;%s;%s\n", index, titre, mot_cle);
        fflush(file_out);
        index++;
    }
    fclose(file_in);
    fclose(file_out);
    printf("create_database: %s => DONE\n", file_name2);
    return 0;
}

int create_filter(char *file_name){
    FILE *file_in, *file_out;
    static char file_name1[128], file_name2[128];

    strcpy(file_name1, file_name);
    strcat(file_name1, "_database");
    file_in = fopen(file_name, "r");
    
    if (file_in == NULL){
        printf("create_filter: fopen %s == NULL\n", file_name);
        exit(1);
    }
    
    strcpy(file_name2, file_name);
    strcat(file_name2, "_sha");
    file_out = fopen(file_name2, "w+");
    
    int i, j, index;

    if (file_out == NULL){
        printf("create_filter: fopen %s == NULL\n", file_name2);
        exit(1);
    }

    printf("create_filter:  %s => OK\n", file_name);
    printf("create_filter:  %s => OK\n", file_name2);

    char tmp[512];
    char *titre;
    char *mot_cle;
    int page_rank;
    char *filtre;
    
    if (fgets(tmp, 512, file_in) == NULL) {
        printf("create_filter: fichier de taile vide\n");
        exit(1);
    }
    
    unsigned char md[32];
    char hash[2*sizeof(md) + 1];
    char *morceau;
    
    CC_SHA256_CTX c;

    fputs("index titre filtreSHA1\n", file_out);
    index = 0;
    
    while (fgets(tmp, 512, file_in) != NULL){
        titre = strtok(tmp, ";");
        mot_cle = strtok(NULL, ";");
        page_rank = atoi(strtok(NULL, ";"));
        filtre = strtok(NULL, ";");

        if ((titre == NULL) || (mot_cle == NULL) || (filtre == NULL)){
            printf("create_filter: titre or mot_cle == NULL\n");
            exit(1);
        }
        morceau = strtok(mot_cle, ",");
        CC_SHA256_Init(&c);
        
        while (morceau){
            CC_SHA256_Update(&c, (const void *)morceau, strlen(morceau));
            morceau = strtok(NULL, ",");
        }
        CC_SHA256_Final(md, &c);
        for (i = 0; i < sizeof(md); i++) {
            snprintf(hash+(2*i), 3, "%02x\n", (int)md[i]);
            //   printf("%d ", (int)md[i]);
        }
        fprintf(file_out, "%d %s %s\n", index, titre, hash);
        fflush(file_out);
        index++;
    }
    fclose(file_out);
    fclose(file_in);
    printf("create_filter: %s => DONE\n", file_name2);
    return 0;
}

char *read_filter(char *file_name, unsigned int index, char **titre_return){
    FILE *file_in;
    char tmp[512];
    static char file_name2[128];
    
    strcpy(file_name2, file_name);
    strcat(file_name2, "_sha");
    file_in = fopen(file_name2, "r");
    
    if (file_in == NULL){
        printf("read_filter: fopen %s == NULL\n", file_name2);
        exit(1);
    }
    
    if (fgets(tmp, 512, file_in) == NULL) {
        printf("read_filter: fichier de taile vide\n");
        exit(1);
    }
    
    int index_out, trouve;
    char *titre, *filtre;
    trouve = 0;
    while (trouve == 0){
        if (fgets(tmp, 512, file_in) != NULL){
            index_out = atoi(strtok(tmp, " "));
            titre = strtok(NULL, " ");
            filtre = strtok(NULL, " ");
            if (index_out == index) {
                trouve = 1;
                *titre_return = titre;
            }
        }else{
            return NULL;
        }
    }
    fclose(file_in);
    return filtre;
}

unsigned int hex2int(char *a, unsigned int len){
    int i, tmp;
    
    unsigned int val = 0;
    for (i = 0; i < len; i++) {
        switch (a[i]) {
            case 'a':
                val = val + (10<<((len - 1 - i)*4));
                break;
            case 'b':
                val = val + (11<<((len - 1 - i)*4));
                break;
            case 'c':
                val = val + (12<<((len - 1 - i)*4));
                break;
            case 'd':
                val = val + (13<<((len - 1 - i)*4));
                break;
            case 'e':
                val = val + (14<<((len - 1 - i)*4));
                break;
            case 'f':
                val = val + (15<<((len - 1 - i)*4));
                break;
            default:
                if (isdigit(a[i]) == 0) {
                    printf("hex2int: erreur not a number %c %d!\n", a[i], i);
                    exit(1);
                }
                tmp = a[i] - '0';
                val = val + (tmp<<((len - 1 - i)*4));
                break;
        }
    }
    
    return val;
}

char int2hex(int a){
    if ((a < 0) || (a > 15)){
        printf("int2hex: erreu > 15 or < 0\n");
        exit(1);
    }
    switch (a) {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'a';
        case 11:
            return 'b';
        case 12:
            return 'c';
        case 13:
            return 'd';
        case 14:
            return 'e';
        default:
            return 'f';
    }
}

char *union2filter(char *f1, char *f2, int len){
    char *tmp1, *tmp2, *tmp;
    unsigned int i, i1, i2;
    
    tmp = (char *)malloc(sizeof(char));
    tmp1 = f1;
    tmp2 = f2;
    for (i = 0; i < len; i++) {
        i1 = hex2int(tmp1+i, 1);
        i2 = hex2int(tmp2+i, 1);
        tmp[i] = int2hex(i1 | i2);
    }
    return tmp;
}

int insert_va(struct list_va *listeva, struct vec *va){
    struct list_va *l = listeva;
    struct vec *v = va;
    struct vec *tmp_va, *tmp_va1;
    
    if (l->va == NULL) {
        l->size++;
        l->va = (struct vec *)malloc(sizeof(struct vec));
        l->va->index = v->index;
        l->va->next = NULL;
        return 0;
    }
    
    tmp_va = l->va;
    while (1) {
        if (tmp_va->index == v->index) {
            return 0;
        }
        if (tmp_va->index > v->index) {
            l->size++;
            tmp_va1 = (struct vec *)malloc(sizeof(struct vec));
            tmp_va1->next = tmp_va;
            tmp_va1->index = v->index;
            l->va = tmp_va1;
            return 0;
        }
        if (tmp_va->next == NULL) {
            l->size++;
            tmp_va->next = (struct vec *)malloc(sizeof(struct vec));
            tmp_va = tmp_va->next;
            tmp_va->index = v->index;
            tmp_va->next = NULL;
            return 0;
        }
        if (tmp_va->next->index > v->index) {
            l->size++;
            tmp_va1 = (struct vec *)malloc(sizeof(struct vec));
            tmp_va1->next = tmp_va->next;
            tmp_va->next = tmp_va1;
            tmp_va1->index = v->index;
            return 0;
        }
        tmp_va = tmp_va->next;
    }
    return 0;
}

int add_va(struct list_va *listeva, int tab[], int len_tab, int index){
    int i;
    struct list_va *tmp;
    struct vec *va;
    
    tmp = listeva;
    if (tmp->size == 0) {
        tmp->size++;
     //   printf("%d: ", index);
        for (i = 0; i < len_tab; i++) {
            tmp->id[i] = tab[i];
    //        printf("%d ", tmp->id[i]);
        }
     //   printf("\n");
        tmp->va = (struct vec *)malloc(sizeof(struct vec));
        tmp->va->index = index;
        tmp->va->next = NULL;
        tmp->next = NULL;
        return 0;
    }

    while (1) {
        for (i = 0; i < len_tab; i++) {
            if (tab[i] != tmp->id[i]) {
                break;
            }
        }
        if (i == len_tab) {
            break;
        }
        if (tmp->next != NULL) {
            tmp = tmp->next ;
        }else{
            break;
        }
    }

    if (i != len_tab) {  // ne pas trouve
        tmp->next = (struct list_va *)malloc(sizeof(struct list_va));
        if (tmp->next == NULL) {
            printf("add_va: erreur malloc tmp->next");
            exit(1);
        }
        tmp = tmp->next;
        tmp->size = 1;
      //  printf("%d: ", index);
        for (i = 0; i < len_tab; i++) {
            tmp->id[i] = tab[i];
      //      printf("%d ", tmp->id[i]);
        }
     //   printf("\n");
        tmp->va = (struct vec *)malloc(sizeof(struct vec));
        if (tmp->va == NULL) {
            printf("add_va: erreur malloc tmp->va");
            exit(1);
        }
        va = tmp->va;
        va->index = index;
        va->next = NULL;

    }else{  //trouve
     //   printf("va: %d %d\n", tmp->id[0], tmp->id[1]);
        va = (struct vec *)malloc(sizeof(struct vec));
        if (index == -1) {
            return 0;
        }
        va->index = index;
        va->next = NULL;
        insert_va(tmp, va);
    }
    
    return 0;
}

int find_index(char *file_name, struct list_va *listeva, int tab[], int len_tab){
    struct list_va *l = listeva;
    struct vec *va;
    int i, trouve;

    if (l == NULL) {
        printf("find_index: liste est vide\n");
        return 1;
    }
    
    if (l->va == NULL) {
        printf("find_index: liste->va est vide\n");
        return 1;
    }
    
    while (1){
        for (i = 0; i < len_tab; i++) {
            if (l->id[i] != tab[i]) {
                break;
            }
        }
        if (i == len_tab) {
            trouve = 1;
            break;
        }

        if (l->next == NULL) {
            printf("pas trouve!\n");
            return 1;
        }
        l = l->next;
    }
    va = l->va;
    printf("liste d'index trouve:\n");
    char *filtre, *titre_return;
    
    while (1) {
        filtre = read_filter(file_name, va->index, &titre_return);
        printf("%d-%s-%s", va->index, titre_return, filtre);
        if (va->next == NULL) {
//            printf("\n");
            return 0;
        }
        va = va->next;
    }
    return 1;
}

int *create_va(char *mot){
    static int id[CHUNKS];
    
    if ((mot == NULL)||(strlen(mot) == 0)) {
        printf("create_va: mot vide\n");
        exit(1);
    }
    
    int len = strlen(mot);
    char tmp[len];
    int i;
    CC_SHA256_CTX c;
    unsigned char md[32];
    char hash[2*sizeof(md) + 1];
    
    for (i = 0; i < len; i++) {
        tmp[i] = tolower(mot[i]);
    }
    tmp[len] = '\0';

    CC_SHA256_Init(&c);
    CC_SHA256_Update(&c, (const void *)tmp, strlen(tmp));
    CC_SHA256_Final(md, &c);

    for (i = 0; i < sizeof(md); i++) {
        snprintf(hash+(2*i), 3, "%02x\n", (int)md[i]);
        //   printf("%d ", (int)md[i]);
    }
    for (i = 0; i < CHUNKS; i++) {
        id[i] = hex2int(hash+(i*(64/CHUNKS)), 1);
    }

    return id;
}

int create_va_file(char *file_name, struct list_va *listeva){
    FILE *file_in;
    static char file_name2[128];
    
    strcpy(file_name2, file_name);
    strcat(file_name2,"_database");
    file_in = fopen(file_name2, "r");
   // printf("file: %s\n", file_name2);

    if (file_in == NULL){
        printf("create_va_file in: fopen %s == NULL\n", file_name2);
        exit(1);
    }

    char tmp[512];
    
    if (fgets(tmp, 512, file_in) == NULL) {
        printf("create_filter: fichier de taile vide\n");
        exit(1);
    }

    int index, i, x, *id;
    char *titre, *morceau, *tmp2, *mot_cle;
    
    while (fgets(tmp, 512, file_in) != NULL){
        index = atoi(strtok(tmp, ";"));
        titre = strtok(NULL, ";");
        mot_cle = strtok(NULL, ";");
        mot_cle[strlen(mot_cle) - 1] = '\0';
        morceau = strsep(&mot_cle, ",");
        while (morceau){
            id = create_va(morceau);
            morceau = strsep(&mot_cle, ",");
            add_va(listeva, id, CHUNKS, index);
        }
    }
    fclose(file_in);
    printf("create_va_file: %s => DONE\n", file_name2);
    return 0;
}

struct list_va *create_request(char *mot_cle){
    char *mot, *tmp, *tmp_mot;
    mot = (char *)malloc(sizeof(char));

    if (mot_cle == NULL) {
        printf("create_request: mot cle == NULL\n");
        exit(1);
    }
    
    tmp = strdup(mot_cle);
    mot = strsep(&tmp, ",");
    
    struct list_va *l = (struct list_va *)malloc(sizeof(struct list_va));
    if (l == NULL) {
        printf("create_request: l created == NULL\n");
        exit(1);
    }
    l->size = 0;
    
    int i, *id;
    id = (int *)malloc(sizeof(int));
    if (mot == NULL) {
        printf("create_request: mot vide\n");
        exit(1);
    }else{
        while (1) {
            id = create_va(mot);
            add_va(l, id, CHUNKS, -1);
            mot = strsep(&tmp, ",");
            if (mot == NULL) {
                break;
            }
        }
        printf("create_request: request created => DONE\n");
        return l;
    }
    return NULL;
}

int find_request(char *file_name, struct list_va *l, struct list_va *list_req){
    struct list_va *list;
    struct list_va *req;
    struct vec *va;
    int i, j;

    if (l == NULL) {
        printf("find_request: list_va == NULL\n");
        return 1;
    }
    if (list_req == NULL) {
        printf("find_request: list_req == NULL\n");
        return 1;
    }

    list = l;
    char *filtre, *titre_return;
    
    for (req = list_req; req != NULL; req = req->next) {
        while (1) {
            for (i = 0; i < CHUNKS; i++) {
                if (list->id[i] != req->id[i]) {
                    break;
                }
            }
            if (i == CHUNKS) {
                va = list->va;
                printf("titre trouve: \n");
                while (1){
                    filtre = read_filter(file_name, va->index, &titre_return);
                    printf("- %s \n", titre_return);
                    va = va->next;
                    if (va == NULL) {
                        break;
                    }
                }
            }
            if (list->next == NULL) {
                break;
            }
            list = list->next;
        }
    }
    return 0;
}

int main(int argc, char **argv){
    char *file_name = argv[1];
    struct list_va *list = (struct list_va *)malloc(sizeof(struct list_va));
    struct list_va *l;
    struct vec *va;
    int i;
    
    list->size = 0;
    
    create_va_file(file_name, list);
    char *a = (char *)malloc(sizeof(char));
    strcpy( a, "faillure,fedora,feed,facebook,fiber,filename,folder,filter,final,firefox,fireware,flash,float,flow,function,fonctionning,format,fichier,fat32,fifo");
    l = create_request(a);
    find_request(file_name, list, l);

    return 0;
}











