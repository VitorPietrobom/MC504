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

bool verifyTrue(bool[9] arg){
    for (int i = 0 ; i < 9 ; i++){
        if (arg[i] == false){
            return false;
        }
    }
    return true;
} 

void *h_thread(int[9][9] arg) {
  bool ret = true;
  bool verify[9] = [false, false, false, false, false, false, false, false, false];
  

  for (int i = 0 ; i < 9 ; i++ ){
      for (int j = 0 ; j < 9 ; j++ ){
          verify[ arg[i][j] - 1 ] = true;
      }
      if (!verifyTrue(verify)){
          ret = false;
          break;
      }
      verify = [false, false, false, false, false, false, false, false, false]; 
  }
  

  pthread_exit(ret);
}


void *v_thread(int[9][9] arg) {
  bool ret = true;
  bool verify[9] = [false, false, false, false, false, false, false, false, false];
  

  for (int j = 0 ; j < 9 ; j++ ){
      for (int i = 0 ; i < 9 ; i++ ){
          verify[ arg[i][j] - 1 ] = true;
      }
      if (!verifyTrue(verify)){
          ret = false;
          break;
      }
      verify = [false, false, false, false, false, false, false, false, false]; 
  }
  

  pthread_exit(ret);
}

void *c_thread(int[9][9] arg) {
  bool ret = true;
  bool verify[9] = [false, false, false, false, false, false , false, false, false];
  int lim = 3

  for (int z = 0 ;)
  for (int i = 0 ; i < 3 ; i++ ){
      for (int j = lim-3 ; j < lim ; j++ ){
          verify[ arg[i][j] - 1 ] = true;
      }
      if (!verifyTrue(verify)){
          ret = false;
          break;
      }
      verify = [false, false, false, false, false, false, false, false, false]; 
  }

  for (int i = 3 ; i < 6 ; i++ ){
      for (int j = 3 ; j < 6 ; j++ ){
          verify[ arg[i][j] - 1 ] = true;
      }
      if (!verifyTrue(verify)){
          ret = false;
          break;
      }
      verify = [false, false, false, false, false, false, false, false, false]; 
  }

  for (int i = 6 ; i < 9 ; i++ ){
      for (int j = 6 ; j < 9 ; j++ ){
          verify[ arg[i][j] - 1 ] = true;
      }
      if (!verifyTrue(verify)){
          ret = false;
          break;
      }
      verify = [false, false, false, false, false, false, false, false, false]; 
  }
  

  pthread_exit(ret);
}

void main(){
    int matriz[9][9];
    recebeMatriz(matriz);
    //printMatriz(matriz);

    pthread_t horizontal;
    pthread_create(&horizontal, NULL, h_thread, int[9][9]);
 
}