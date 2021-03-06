#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

bool verifyTrue(bool arg[9]){
    for (int i = 0 ; i < 9 ; i++){
        if (arg[i] == false){
            return false;
        }
    }
    return true;
} 

void *h_thread(int arg[9][9]) {
  int ret = 1;
  bool verify[9];
  for(int z = 0; z < 9; z++)   verify[z] = false;
  

  for (int i = 0 ; i < 9 ; i++ ){
    for (int j = 0 ; j < 9 ; j++ ) verify[ arg[i][j] - 1 ] = true;
    if (!verifyTrue(verify)) ret = 0;
    for(int z = 0; z < 9; z++)   verify[z] = false; 
    }
  

  pthread_exit(ret);
}


void *v_thread(int arg[9][9]) {
  int ret = 1;
  bool verify[9];
  for(int z = 0; z < 9; z++)   verify[z] = false;  

  for (int j = 0 ; j < 9 ; j++ ){
    for (int i = 0 ; i < 9 ; i++ )  verify[ arg[i][j] - 1 ] = true; 
    if (!verifyTrue(verify))    ret = 0;        
    for(int z = 0; z < 9; z++)   verify[z] = false;  
  }
  

  pthread_exit(ret);
}

void *c_thread(int arg[9][9]) {
  int ret = 1;
  bool verify[9];
  for(int z = 0; z < 9; z++)   verify[z] = false;  

    for(int l = 0; l < 3; l++){ // varia os blocos de 3 linhas
        for(int k = 0; k < 3; k++){ // varia os blocos de 3 colunas
            
            for(int i = 0; i < 3; i++){    
                for(int j = 0; j < 3; j++)  verify[arg[i+3*l][j+3*k]-1] = true; 
            }
            if (!verifyTrue(verify))    ret = 0;
            for(int z = 0; z < 9; z++)   verify[z] = false; 
        }
    }

  pthread_exit(ret);
}

void main(){
    int matriz[9][9];
    recebeMatriz(matriz);

    int *ret[3];
    pthread_t horizontal;
    pthread_t vertical;
    pthread_t circular;
    pthread_create(&horizontal, NULL, h_thread, (int*) matriz);
    pthread_create(&vertical, NULL, v_thread, (int*) matriz);
    pthread_create(&circular, NULL, c_thread, (int*) matriz);
    pthread_join(horizontal, (void**)&(ret[0]));
    pthread_join(vertical, (void**)&(ret[1]));
    pthread_join(circular, (void**)&(ret[2]));
    if(!ret[0] || !ret[1] || !ret[2])  printf("o sudoku esta errado");
    else    printf("o sudoku esta correto");
}