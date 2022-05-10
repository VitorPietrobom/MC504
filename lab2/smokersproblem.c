#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

const int ID_AGENTE_PAPEL_TABACO = 0;
const int ID_AGENTE_TABACO_FOSFORO = 1;
const int ID_AGENTE_FOSFORO_PAPEL = 2;

const int ID_FUMANTE_PAPEL = 0;
const int ID_FUMANTE_TABACO = 1;
const int ID_FUMANTE_FOSFORO = 2;

volatile bool finishProgram = false;

volatile bool TabacoisFumando, FosforoisFumando, PapelisFumando = false;

volatile bool temTabaco, temFosforo, temPapel = false;

sem_t semaforo_agentes;
sem_t semaforo_agente_papel_tabaco;
sem_t semaforo_agente_tabaco_fosforo;
sem_t semaforo_agente_fosforo_papel;

sem_t semaforo_papel;
sem_t semaforo_tabaco;
sem_t semaforo_fosforo;

sem_t semaforo_fumante_papel;
sem_t semaforo_fumante_tabaco;
sem_t semaforo_fumante_fosforo;


pthread_mutex_t lock;
pthread_mutex_t print_lock;
pthread_mutex_t isFumando_lock;

void *thread_semaforo_agente() {
    // chama os semaforos de agentes randomicamente
    int id_agente = 0;
    srand ( time(NULL) );
    for (int i = 0; i < 10; i++) {
        sem_wait(&semaforo_agentes);
        id_agente = rand() % 3;
        if (id_agente == ID_AGENTE_PAPEL_TABACO) {
            sem_post(&semaforo_agente_papel_tabaco);
        } else if (id_agente == ID_AGENTE_TABACO_FOSFORO) {
            sem_post(&semaforo_agente_tabaco_fosforo);
        } else if (id_agente == ID_AGENTE_FOSFORO_PAPEL) {
            sem_post(&semaforo_agente_fosforo_papel);
        }
    }
    finishProgram = true;
    return NULL;
}


void *thread_agente_papel_tabaco() {
    while(!finishProgram) {
        sem_wait(&semaforo_agente_papel_tabaco);
        if(finishProgram) break;
        pthread_mutex_lock(&print_lock);
        printf("_______________________MESA___________________ |\n");
        printf("|                                              |\n");
        printf("|                      []   +                  |\n");
        printf("|______________________________________________|\n\n");
        pthread_mutex_unlock(&print_lock);
        sem_post(&semaforo_papel);
        sem_post(&semaforo_tabaco);
    }
    return NULL;
}

void *thread_agente_tabaco_fosforo() {
    while(!finishProgram) {
        sem_wait(&semaforo_agente_tabaco_fosforo);
        if(finishProgram) break;
        pthread_mutex_lock(&print_lock);
        printf("_______________________MESA___________________ |\n");
        printf("|                                              |\n");
        printf("|                      +   !                   |\n");
        printf("|______________________________________________|\n\n");
        pthread_mutex_unlock(&print_lock);
        sem_post(&semaforo_tabaco);
        sem_post(&semaforo_fosforo);
    }
    return NULL;
}

void *thread_agente_fosforo_papel() {
    while(!finishProgram) {
        sem_wait(&semaforo_agente_fosforo_papel);
        if(finishProgram) break;
        pthread_mutex_lock(&print_lock);
        printf("_______________________MESA___________________ |\n");
        printf("|                                              |\n");
        printf("|                      []   !                  |\n");
        printf("|______________________________________________|\n\n");
        pthread_mutex_unlock(&print_lock);
        sem_post(&semaforo_papel);
        sem_post(&semaforo_fosforo);
    }
    return NULL;
}

void *thread_pusher_papel() {
    while(!finishProgram) {
        sem_wait(&semaforo_papel);
        // wait mutex dos pushers
        pthread_mutex_lock(&lock);
        if (temTabaco) {
            temTabaco = false;
            sem_post(&semaforo_fumante_papel);
        } else if (temFosforo)
        {
            temFosforo = false;
            sem_post(&semaforo_fumante_tabaco);
        } else {
            temPapel = true;
        }
        // destravar mutex
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_pusher_tabaco() {
    while(!finishProgram) {
        sem_wait(&semaforo_tabaco);
        // wait mutex dos pushers
        pthread_mutex_lock(&lock);
        if (temPapel) {
            temPapel = false;
            sem_post(&semaforo_fumante_fosforo);
        } else if (temFosforo)
        {
            temFosforo = false;
            sem_post(&semaforo_fumante_papel);
        } else {
            temTabaco = true;
        }
        // destravar mutex
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_pusher_fosforo() {
    while(!finishProgram) {
        sem_wait(&semaforo_fosforo);
        // wait mutex dos pushers
        pthread_mutex_lock(&lock);
        if (temTabaco) {
            temTabaco = false;
            sem_post(&semaforo_fumante_papel);
        } else if (temPapel)
        {
            temPapel = false;
            sem_post(&semaforo_fumante_tabaco);
        } else {
            temFosforo = true;
        }
        // destravar mutex
        pthread_mutex_unlock(&lock);

    }
    return NULL;
}

void fumar(char* nome_fumante) {

    pthread_mutex_lock(&isFumando_lock);

    if (nome_fumante == "Papel"){PapelisFumando = true;}
    else if (nome_fumante == "Fosforo"){FosforoisFumando = true;}
    else if (nome_fumante == "Tabaco"){TabacoisFumando = true;}

    if (TabacoisFumando && PapelisFumando && FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |              |              |                           }\n");
        printf("|                |              |              |           [] + !          }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (TabacoisFumando && PapelisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |              |         !!!  |                           }\n");
        printf("|                |              |   \\ 0 /      |           [] +            }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (PapelisFumando && FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |         +++  |              |                           }\n");
        printf("|                |   \\ 0 /      |              |           [] !            }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (TabacoisFumando && FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |              |              |                           }\n");
        printf("|  \\ 0 /         |              |              |           + !             }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (TabacoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |              |         !!!  |                           }\n");
        printf("|  \\ 0 /         |              |   \\ 0 /      |            +              }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |         +++  |              |                           }\n");
        printf("|  \\ 0 /         |   \\ 0 /      |              |            !              }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (PapelisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Começou fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |         +++  |         !!!  |                           }\n");
        printf("|                |   \\ 0 /      |   \\ 0 /      |           []              }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    }

    pthread_mutex_unlock(&isFumando_lock);

    sleep(1);

    pthread_mutex_lock(&isFumando_lock);

    if (nome_fumante == "Papel"){PapelisFumando = false;}
    else if (nome_fumante == "Fosforo"){FosforoisFumando = false;}
    else if (nome_fumante == "Tabaco"){TabacoisFumando = false;}

    if (!TabacoisFumando && !PapelisFumando && !FosforoisFumando){
        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |        +++   |         !!!  |                           }\n");
        printf("|  \\ 0 /         |  \\ 0 /       |   \\ 0 /      |                           }\n");
        printf("|________________|______________|______________|___________________________}\n\n");

        pthread_mutex_unlock(&print_lock);

    } else if (TabacoisFumando && PapelisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |              |         !!!  |                           }\n");
        printf("|                |              |   \\ 0 /      |           [] +            }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (PapelisFumando && FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |         +++  |              |                           }\n");
        printf("|                |   \\ 0 /      |              |           [] !            }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (TabacoisFumando && FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |              |              |                           }\n");
        printf("|  \\ 0 /         |              |              |           + !             }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (TabacoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |              |         !!!  |                           }\n");
        printf("|  \\ 0 /         |              |   \\ 0 /      |            +              }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (FosforoisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|         [][][] |         +++  |              |                           }\n");
        printf("|  \\ 0 /         |   \\ 0 /      |              |            !              }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    } else if (PapelisFumando){

        pthread_mutex_lock(&print_lock);
        printf("Terminou de fumar: %s\n", nome_fumante);
        printf("|________________SALA DE ESPERA________________|______ÁREA DE FUMANTE______}\n");
        printf("|                |         +++  |         !!!  |                           }\n");
        printf("|                |   \\ 0 /      |   \\ 0 /      |           []              }\n");
        printf("|________________|______________|______________|___________________________}\n\n");
        pthread_mutex_unlock(&print_lock);

    }

    pthread_mutex_unlock(&isFumando_lock);


}

void fazer_cigarro(int id_fumante) {
    if (id_fumante == ID_FUMANTE_PAPEL) {
        temTabaco = false;
        temFosforo = false;
        pthread_mutex_lock(&print_lock);
        printf("_______________________MESA___________________ |\n");
        printf("|                                              |\n");
        printf("|                                              |\n");
        printf("|______________________________________________|\n\n");
        pthread_mutex_unlock(&print_lock);
    } else if (id_fumante == ID_FUMANTE_TABACO) {
        temPapel = false;
        temFosforo = false;
        pthread_mutex_lock(&print_lock);
        printf("_______________________MESA___________________ |\n");
        printf("|                                              |\n");
        printf("|                                              |\n");
        printf("|______________________________________________|\n\n");
        pthread_mutex_unlock(&print_lock);
    } else if (id_fumante == ID_FUMANTE_FOSFORO) {
        temPapel = false;
        temTabaco = false;
        pthread_mutex_lock(&print_lock);
        printf("_______________________MESA___________________ |\n");
        printf("|                                              |\n");
        printf("|                                              |\n");
        printf("|______________________________________________|\n\n");
        pthread_mutex_unlock(&print_lock);
    }
}

void *thread_fumante_papel() {
    while(!finishProgram) {
        sem_wait(&semaforo_fumante_papel);
        if(finishProgram) break;
        fazer_cigarro(ID_FUMANTE_PAPEL);
        sem_post(&semaforo_agentes);
        fumar("Papel");
    }
    return NULL;
}

void *thread_fumante_tabaco() {
    while(!finishProgram) {
        sem_wait(&semaforo_fumante_tabaco);
        if(finishProgram) break;
        fazer_cigarro(ID_FUMANTE_TABACO);
        sem_post(&semaforo_agentes);
        fumar("Tabaco");
    }
    return NULL;
}

void *thread_fumante_fosforo() {
    while(!finishProgram) {
        sem_wait(&semaforo_fumante_fosforo);
        if(finishProgram) break;
        fazer_cigarro(ID_FUMANTE_FOSFORO);
        sem_post(&semaforo_agentes);
        fumar("Fosforo");

    }
    return NULL;
}

int main() {
    pthread_t agentes;
    pthread_t agente_papel_tabaco;
    pthread_t agente_tabaco_fosforo;
    pthread_t agente_fosforo_papel;
    
    pthread_t pusher_papel;
    pthread_t pusher_tabaco;
    pthread_t pusher_fosforo;

    pthread_t fumante_papel;
    pthread_t fumante_tabaco;
    pthread_t fumante_fosforo;
    // inicializa os semaforos (apenas o semaforo de agentes ativo)
    sem_init(&semaforo_agentes, 0, 1);
    sem_init(&semaforo_agente_papel_tabaco, 0, 0);
    sem_init(&semaforo_agente_tabaco_fosforo, 0, 0);
    sem_init(&semaforo_agente_fosforo_papel, 0, 0);

    sem_init(&semaforo_papel, 0, 0);
    sem_init(&semaforo_tabaco, 0, 0);
    sem_init(&semaforo_fosforo, 0, 0);

    sem_init(&semaforo_fumante_papel, 0, 0);
    sem_init(&semaforo_fumante_tabaco, 0, 0);
    sem_init(&semaforo_fumante_fosforo, 0, 0);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    if (pthread_mutex_init(&print_lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    if (pthread_mutex_init(&isFumando_lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    
    // cria as threads
    pthread_create(&agentes, NULL, thread_semaforo_agente, NULL);
    pthread_create(&agente_papel_tabaco, NULL, thread_agente_papel_tabaco, NULL);
    pthread_create(&agente_tabaco_fosforo, NULL, thread_agente_tabaco_fosforo, NULL);
    pthread_create(&agente_fosforo_papel, NULL, thread_agente_fosforo_papel, NULL);

    pthread_create(&pusher_papel, NULL, thread_pusher_papel, NULL);
    pthread_create(&pusher_tabaco, NULL, thread_pusher_tabaco, NULL);
    pthread_create(&pusher_fosforo, NULL, thread_pusher_fosforo, NULL);


    pthread_create(&fumante_papel, NULL, thread_fumante_papel, NULL);
    pthread_create(&fumante_tabaco, NULL, thread_fumante_tabaco, NULL);
    pthread_create(&fumante_fosforo, NULL, thread_fumante_fosforo, NULL);
    // // para as threads

    pthread_join(agentes, NULL);

    sem_post(&semaforo_agentes);
    sem_post(&semaforo_agente_papel_tabaco);
    sem_post(&semaforo_agente_tabaco_fosforo);
    sem_post(&semaforo_agente_fosforo_papel);

    sem_post(&semaforo_papel);
    sem_post(&semaforo_tabaco);
    sem_post(&semaforo_fosforo);

    sem_post(&semaforo_fumante_papel);
    sem_post(&semaforo_fumante_tabaco);
    sem_post(&semaforo_fumante_fosforo);

    pthread_join(agente_papel_tabaco, NULL);
    pthread_join(agente_tabaco_fosforo, NULL);
    pthread_join(agente_fosforo_papel, NULL);

    pthread_join(pusher_papel, NULL);
    pthread_join(pusher_tabaco, NULL);
    pthread_join(pusher_fosforo, NULL);


    pthread_join(fumante_papel, NULL);
    pthread_join(fumante_tabaco, NULL);
    pthread_join(fumante_fosforo, NULL);


    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&isFumando_lock);
    pthread_mutex_destroy(&print_lock);

     return 0;
}