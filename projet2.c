#include "projet2.h"

int create_filter(char *file_name){
    FILE *file_in, *file_out;
    file_in = fopen(file_name, "r");
    printf("lire fichier %s\n", file_name);
    
    if (file_in == NULL){
        printf("create_filter: fopen %s == NULL\n", file_name);
        exit(1);
    }
    
    char *file_name2;
    file_name2 = strdup(file_name);
    strcat(file_name2, "_sha.txt");
    file_out = fopen(file_name2, "w+");
    printf("creer fichier %s\n", file_name2);
    
    int i, j, index;

    if (file_out == NULL){
        printf("create_filter: fopen %s == NULL\n", file_name2);
        exit(1);
    }

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
        CC_SHA256_Init(&c);
        morceau = strtok(mot_cle, ",");
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
    return 0;
}

char *read_filter(char *file_name, unsigned int index, char **titre_return){
    FILE *file_in;
    char tmp[512];
    char *file_name2;
    
    file_name2 = strdup(file_name);
    strcat(file_name2, "_sha.txt");
    file_in = fopen(file_name2, "r");
    
    if (file_in == NULL){
        printf("create_filter: fopen %s == NULL\n", file_name);
        exit(1);
    }
    
    if (fgets(tmp, 512, file_in) == NULL) {
        printf("create_filter: fichier de taile vide\n");
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
                    printf("hex2int: erreur not a number!\n");
                    exit(1);
                }
                tmp = a[i] - '0';
                val = val + (tmp<<((len - 1 - i)*4));
                break;
        }
    }
    
    return val;
}

int add_va(struct list_va *listeva, int tab[], int len_tab, int index){
    int i, trouve;
    struct list_va *tmp;
    struct vec *va;
    
    tmp = listeva;
    
    if (tmp->size == 0) {
        tmp->size++;
    //    printf("%d: ", index);
        for (i = 0; i < len_tab; i++) {
            tmp->id[i] = tab[i];
    //        printf("%d ", tmp->id[i]);
        }
    //    printf("\n");
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
       // printf("%d: ", index);
        for (i = 0; i < len_tab; i++) {
            tmp->id[i] = tab[i];
      //      printf("%d ", tmp->id[i]);
        }
     //  printf("\n");
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
        tmp->size++;
        va = tmp->va;
        while (va->next) {
            if (va->next->index > index) {
                break;
            }
            va = va->next;
        }
        if (va->next == NULL) {
        va->next = (struct vec *)malloc(sizeof(struct vec));
        if (va->next == NULL) {
            printf("add_va: erreur malloc va->next");
            exit(1);
        }
        va = va->next;
        va->index = index;
        va->next = NULL;
        }else{
            struct vec *va2 = (struct vec *)malloc(sizeof(struct vec));
            va2->next = va->next;
            va->next = va2;
            va->index = index;
        }
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
//        printf("list id: %d %d %d %d size %d\n", l->id[0], l->id[1], tab[0], tab[1], l->size);
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

int create_va_file(char *file_name, struct list_va *listeva){
    FILE *file_in;
    char *file_name2;
    
    file_name2 = strdup(file_name);
    strcat(file_name2,"_sha.txt");
    file_in = fopen(file_name2, "r");
    if (file_in == NULL){
        printf("create_va_file in: fopen %s == NULL\n", file_name2);
        exit(1);
    }
    printf("lire fichier %s\n", file_name2);
    
    char tmp[512];
    
    if (fgets(tmp, 512, file_in) == NULL) {
        printf("create_filter: fichier de taile vide\n");
        exit(1);
    }
    
    int index, i, x, id[4];
    char *titre, *filtre, *resultat, *tmp2;
    
    while (fgets(tmp, 512, file_in) != NULL){
        index = atoi(strtok(tmp, " "));
        titre = strtok(NULL, " ");
        filtre = strtok(NULL, " ");
        filtre[strlen(filtre) - 1] = '\0';
        resultat = filtre;
        // 32 byte divise en m = 4 morceaux de 8 byte, prend k = 1 byte de poids fort chaque morceau comme identifiant de ce morceau, vecteur VA = (k0, k1, k2, k3)
        for (i = 0; i < CHUNKS; i++) {
            id[i] = hex2int(resultat+(i*(64/CHUNKS)), 1);
        }
        add_va(listeva, id, CHUNKS, index);

    }
    return 0;
}

struct list_va *create_request(char *mot_cle, char **filtre_return){
    struct list_va *l = (struct list_va *)malloc(sizeof(struct list_va));
    char *filtre, *mot, *buf, *mot2;

    if (mot_cle == NULL) {
        printf("create_request: mot cle == NULL\n");
        exit(1);
    }
    mot2 = strdup(mot_cle);
    mot = strsep(&mot2, ",");
    
    CC_SHA256_CTX c;
    CC_SHA256_Init(&c);
    unsigned char md[32];
    char hash[2*sizeof(md) + 1];
    
    int i;
    if (mot == NULL) {
        CC_SHA256_Update(&c, (const void *)mot_cle, strlen(mot_cle));
        CC_SHA256_Final(md, &c);
        for (i = 0; i < sizeof(md); i++) {
            snprintf(hash+(2*i), 3, "%02x\n", (int)md[i]);
        }
        *filtre_return = hash;
        l->size = 0;
        char *tmp;
        tmp = *filtre_return;
        for (i = 0; i < CHUNKS; i++) {
            l->id[i] = hex2int(tmp+(i*(64/CHUNKS)), 1);
        }
        l->va = NULL;
        l->next = NULL;
        return l;
    }else{
        char *tmp;
        
        while (mot) {
            CC_SHA256_Update(&c, (const void *)mot, strlen(mot));
            mot = strsep(&mot2, ",");
        }

        CC_SHA256_Final(md, &c);
        for (i = 0; i < sizeof(md); i++) {
            snprintf(hash+(2*i), 3, "%02x\n", (int)md[i]);
        }
        *filtre_return = hash;
        l->size = 0;

        tmp = *filtre_return;
        for (i = 0; i < CHUNKS; i++) {
            l->id[i] = hex2int(tmp+(i*(64/CHUNKS)), 1);
        }
        l->va = NULL;
        l->next = NULL;
        return l;
    }
}

int main(int argc, char **argv){
    char *file_name = argv[1];
    struct list_va *list = (struct list_va *)malloc(sizeof(struct list_va));

    list->size = 0;
    list->va = NULL;
    list->next = NULL;
    //    int size_file = atoi(argv[2]);
  //  char *hash_return, *titre_return;
    
  // create_filter(file_name);
 //   hash_return = read_filter(file_name, 2, &titre_return);
    create_va_file(file_name, list);
  //  int tab[4] = {2,3,15,7};
  //  printf("lol break %d %d\n", list->id[0], tab[0]);
   // find_index(file_name, list, tab , 4);
    
    char *filtre_return;
    struct list_va *list_request;
    list_request = create_request("data,decodage,deconnectable,decoupage,default,definition,dialer,digital,acknowledge,active,admin,adress,adobe,alias,alimentation,Alliance,alpha,alternatif,ampli,antivirus,apache,application,architecture,assembleur,assert", &filtre_return);
    int i;
    for (i = 0; i < CHUNKS; i++) {
        printf("%d ",list_request->id[i]) ;
    }
    printf("\n%s\n", filtre_return);
    find_index(file_name, list, list_request->id , CHUNKS);
    return 0;
}











