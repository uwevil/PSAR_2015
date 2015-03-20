//
//  projet.h
//  
//
//  Created by Cao Sang DOAN on 08/03/15.
//
//

#ifndef ____projet__
#define ____projet__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct books{
    unsigned int id:8;
    char *titre;
    char *mot_cle;
    int page_rank;
    char *filtre;
    struct books *next;
} book;

typedef struct liste{
    book *debut;
    book *fin;
} liste_books;

liste_books tab[16][128];

int init();

int convertisseur(char *binaire);

//int ajouter(char *titre, char *mot_cle, int pagerank, char *filtre);

book *ajouter_book_dans_liste(liste_books tableau[16][128], int position, int index, char *titre, char *mot_cle, int pagerank, char *filtre);

int lire_fichier(char *fichier, int taille);

void erreur(char *erreur);


#endif /* defined(____projet__) */
