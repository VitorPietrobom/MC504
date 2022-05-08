#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

const int ID_AGENTE_PAPEL_TABACO = 1;
const int ID_AGENTE_TABACO_FOSFORO = 2;
const int ID_AGENTE_FOSFORO_PAPEL = 3;

const int ID_FUMANTE_PAPEL = 1;
const int ID_FUMANTE_TABACO = 2;
const int ID_FUMANTE_FOSFORO = 3;

bool finishProgram = false;

bool temTabaco, temFosforo, temPapel = false;
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

void *thread_semaforo_agente() {
    // chama os semaforos de agentes randomicamente
    int id_agente;
    for (int i = 0; i < 3; i++) {
        sem_wait(&semaforo_agentes);
        id_agente = rand() % 3;
        if (id_agente == ID_AGENTE_PAPEL_TABACO) {
            sem_post(&semaforo_papel);
        } else if (id_agente == ID_AGENTE_TABACO_FOSFORO) {
            sem_post(&semaforo_tabaco);
        } else if (id_agente == ID_AGENTE_FOSFORO_PAPEL) {
            sem_post(&semaforo_fosforo);
        }
    }
    finishProgram = true;
    return 0;
}


void *thread_agente_papel_tabaco() {
    while(!finishProgram) {
        sem_wait(&semaforo_agente_papel_tabaco);
        sem_post(&semaforo_papel);
        sem_post(&semaforo_tabaco);
    }
    return 0;
}

void *thread_agente_tabaco_fosforo() {
    while(!finishProgram) {
        sem_wait(&semaforo_agente_tabaco_fosforo);
        sem_post(&semaforo_tabaco);
        sem_post(&semaforo_fosforo);
    }
    return 0;
}

void *thread_agente_fosforo_papel() {
    while(!finishProgram) {
        sem_wait(&semaforo_agente_fosforo_papel);
        sem_post(&semaforo_papel);
        sem_post(&semaforo_fosforo);
    }
    return 0;
}

void *thread_pusher_papel() {
    while(!finishProgram) {
        sem_wait(&semaforo_papel);
        // wait mutex dos pushers
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
    }
    return 0;
}

void *thread_pusher_tabaco() {
    while(!finishProgram) {
        sem_wait(&semaforo_tabaco);
        // wait mutex dos pushers
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
    }
    return 0;
}

void *thread_pusher_fosforo() {
    while(!finishProgram) {
        sem_wait(&semaforo_fosforo);
        // wait mutex dos pushers
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
    }
    return 0;
}

void fumar(char* nome_fumante) {
    printf("Fumante do %s fumou!", nome_fumante);
}

void fazer_cigarro(int id_fumante) {
    if (id_fumante == ID_FUMANTE_PAPEL) {
        temTabaco = false;
        temFosforo = false;
    } else if (id_fumante == ID_FUMANTE_TABACO) {
        temPapel = false;
        temFosforo = false;
    } else if (id_fumante == ID_FUMANTE_FOSFORO) {
        temPapel = false;
        temTabaco = false;
    }
}

void *thread_fumante_papel() {
    while(!finishProgram) {
        sem_wait(&semaforo_fumante_papel);
        fazer_cigarro(ID_FUMANTE_PAPEL);
        sem_post(&semaforo_agentes);
        fumar("Papel");
    }
    return 0;
}

void *thread_fumante_tabaco() {
    while(!finishProgram) {
        sem_wait(&semaforo_fumante_tabaco);
        fazer_cigarro(ID_FUMANTE_TABACO);
        sem_post(&semaforo_agentes);
        fumar("Tabaco");
    }
    return 0;
}

void *thread_fumante_fosforo() {
    while(!finishProgram) {
        sem_wait(&semaforo_fumante_fosforo);
        fazer_cigarro(ID_FUMANTE_FOSFORO);
        sem_post(&semaforo_agentes);
        fumar("Fosforo");
    }
    return 0;
}

void main() {
    pthread_t agentes;
    pthread_t agente_papel_tabaco;
    pthread_t agente_tabaco_fosforo;
    pthread_t agente_fosforo_papel;

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
    // cria as threads
    pthread_create(&agentes, NULL, thread_agente_papel_tabaco, NULL);
    pthread_create(&agente_papel_tabaco, NULL, thread_agente_papel_tabaco, NULL);
    pthread_create(&agente_tabaco_fosforo, NULL, thread_agente_tabaco_fosforo, NULL);
    pthread_create(&agente_fosforo_papel, NULL, thread_agente_fosforo_papel, NULL);

    pthread_create(&fumante_papel, NULL, thread_agente_papel_tabaco, NULL);
    pthread_create(&fumante_tabaco, NULL, thread_agente_papel_tabaco, NULL);
    pthread_create(&fumante_fosforo, NULL, thread_agente_papel_tabaco, NULL);
    // para as threads

    pthread_join(agentes, 0);
    pthread_join(agente_papel_tabaco, 0);
    pthread_join(agente_tabaco_fosforo, 0);
    pthread_join(agente_fosforo_papel, 0);

    pthread_join(fumante_papel, 0);
    pthread_join(fumante_tabaco, 0);
    pthread_join(fumante_fosforo, 0);
}