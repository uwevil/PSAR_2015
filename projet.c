#include "projet.h"

void erreur(char *erreur){
    printf("%s\n", erreur);
}

int convertisseur(char *binaire){
    int len = strlen(binaire);
    int i, res;
    res = 0;
    
    for (i = len - 1; i>= 0; i--) {
        if (binaire[len - i - 1] == '1') res += pow(2, i);
    }
    return res;
}

/*
int ajouter(char *titre, char *mot_cle, int pagerank, char *filtre){
    book *data;
    char *tmp;
    int i = 0;
    int j = 0;
    
    for (i = 0; i < 16; i++) {
        strncpy(tmp, filtre+i*8, 8);
        printf("test %d: %d\n", i, convertisseur(tmp));
    }
}
*/

int init(){
    int i, j;
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 128; j++) {
            tab[i][j].debut = NULL;
            tab[i][j].fin = NULL;
        }
    }
   // tab[15][33].debut = (book *)malloc(sizeof(book));
   // (tab[15][33].debut)->filtre = "dcmmm";
    return 0;
}

book *ajouter_book_dans_liste(liste_books tableau[16][128], int position, int index, char *titre, char *mot_cle, int page_rank, char *filtre){
    
    book *data;
    
    if (tableau[position][index].debut == NULL) {
        tableau[position][index].debut = (book *)malloc(sizeof(book));
        data = tableau[position][index].debut;
        data->id = index;
        data->titre = strdup(titre);
        data->mot_cle = strdup(mot_cle);
        data->page_rank = page_rank;
        data->filtre = strdup(filtre);
        data->next = NULL;
        
        tableau[position][index].fin = data;
        printf("Ajout first dans tab[%d][%d]\n", position, index);
    }else{
        for (data = tableau[position][index].debut;
             data->next != NULL;
             data = data->next) {
        }
        data->next = (book *)malloc(sizeof(book));
        data = data->next;
        
        data->id = index;
        data->titre = strdup(titre);
        data->mot_cle = strdup(mot_cle);
        data->page_rank = page_rank;
        data->filtre = strdup(filtre);
        data->next = NULL;
        tableau[position][index].fin = data;
        printf("Ajout dans tab[%d][%d]\n", position, index);
    }
    return data;
}

int lire_fichier(char *fichier, int taille){
    FILE *file;
    file = fopen(fichier, "r");
    int i, j, index;
    
    printf("lire fichier %s de taille %d entrees\n", fichier, taille);
    
    if (file == NULL){
        erreur("lire_fichier: fopen ligne 9");
        exit(1);
    }
    
    char tmp[512];
    char *titre;
    char *mot_cle;
    int page_rank;
    char *filtre;
    char *morceau, *temp;
    book *res;
    
    if (fgets(tmp, 512, file) == NULL) {
        erreur("lire_fichier: fichier de taile vide\n -> ligne 89");
        exit(1);
    }
    
    while (fgets(tmp, 512, file) != NULL){
        titre = strtok(tmp, ";");
        mot_cle = strtok(NULL, ";");
        page_rank = atoi(strtok(NULL, ";"));
        filtre = strtok(NULL, ";");
        if ((titre == NULL) || (mot_cle == NULL) || (filtre == NULL)){
            erreur("lire_fichier: titre or mot or filtre NULL ligne 23");
            exit(1);
        }
        morceau = filtre;
        for (i = 0; i < 16; i++) {
            j = i*8;
            
            temp = strndup(morceau+j, 8);
            printf("index: %s\n", temp);
            index = convertisseur(temp);
            
            res = ajouter_book_dans_liste(tab, i, index, titre , mot_cle, page_rank, filtre);
            if (res == NULL) {
                erreur("lire_fichier: ajouter_book_dans_liste NULL -> ligne 94");
            }
        }
    }
    return 0;
    
}

int main(int argc, char **argv){
    init();
    lire_fichier(argv[1], atoi(argv[2]));
    printf("NULLLLLLLLLLLLLLLLLLLLLL id %d titre %s, id %d titre %s\n", tab[2][12].debut->id,tab[2][12].debut->titre, tab[2][12].debut->next->id, tab[2][12].debut->next->titre);
    return 0;
}









