#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void recebeMatriz(int matriz[9][9]){
    for (int i = 0;i<9;i++){
        for(int j = 0; j<9; j++) scanf("%d ", &matriz[i][j]);
    } 
}

void printMatriz(int matriz[9][9]){
    for (int i = 0;i<9;i++){
        printf("\n");
        for(int j = 0; j<9; j++){  
            printf("%d ", matriz[i][j]);
        }
    }
    printf("\n");
}

void main(){
    int matriz[9][9];
    recebeMatriz(matriz);
    printMatriz(matriz);

    pthread_t horizontal;
    pthread_create(&horizontal, NULL, h_thread, int[9][9]);
 
}