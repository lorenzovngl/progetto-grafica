#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "headers/Leaderboard.h"

Leaderboard::Leaderboard(char *username){
    setUsename(username);
}

char *Leaderboard::getUsename() {
    return mUsername;
}

void Leaderboard::setUsename(char *username) {
    strcpy(mUsername, username);
}

void Leaderboard::setNumBuoys(int num){
    mNumBuoys = num;
}

Leaderboard::LBItem** Leaderboard::read(){
    char name[50];
    int num, score;
    FILE *file = fopen("leaderboard.txt","r");
    int i = 0;
    while (fscanf(file, "%s\t%d\t%d\n", name, &num, &score) != EOF && i < LEADERBOARD_LENGHT){
        int millis = score;
        int sec = millis/1000;
        int min = sec/60;
        items[i] = new LBItem(name, num, score);
        i++;
    };
    while (i < LEADERBOARD_LENGHT){
        items[i] = nullptr;
        i++;
    }
    fclose(file);
    return items;
}

void Leaderboard::insert(int timeScore) {
    read();
    int i = 0, j;
    // Inserisco il punteggio nella posizione giusta per mantenere l'ordinamento decrescente
    while (i < LEADERBOARD_LENGHT && items[i] != nullptr && items[i]->time/items[i]->numBuoys <= timeScore/mNumBuoys){
        i++;
    }
    for (j = LEADERBOARD_LENGHT - 1; j > i; j--){
        items[j] = items[j-1];
    }
    items[j] = new LBItem(mUsername, mNumBuoys, timeScore);
    FILE *file = fopen("leaderboard.txt","w");
    for (i = 0; i < LEADERBOARD_LENGHT &&  items[i] != nullptr; i++){
        fprintf(file, "%s\t%d\t%d\n", items[i]->name, items[i]->numBuoys, items[i]->time);
    }
    fclose(file);
}