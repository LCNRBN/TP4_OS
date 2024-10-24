#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/shm.h>
#include "segdef.h"

// linux : library .a : static will be added in the .o by the linker and .so : dynamic can be shared with other softs

// gcc -lseg -L and tell where the lib is (in the current file)

#define SEM0 0
int semid;
int shmid;

// void semcall(int op){
    
//     struct sembuf sop;
//     sop.sem_flg = SEM_UNDO;
//     sop.sem_num = SEM0;
//     sop.sem_op = op;

//     if(semop(semid, &sop, 1) == -1){
//         perror("semop");
//         exit(1);
//     }
// }

int init(int* shmid, int* semid, segment** seg){
    key_t key = cle;

    if((*semid = semget(key, 0, 0)) == -1){
        perror("semget");
        exit(1);
    }

    if((*shmid = shmget(key, segsize, IPC_CREAT | 0666)) == -1){
        perror("shmget");
        exit(1);
    }

    if((*seg = shmat(*shmid, NULL, 0)) == (void*)-1){
        perror("shmat");
        exit(1);
    }

    init_rand();
    return(0);
}


int main(){
    int shmid;
    int semid;
    segment* seg;

    init(&shmid, &semid, &seg);

    int i = 0;
    
    while(i < 10000){
        acq_sem(semid, seg_dispo);

        seg->pid = getpid();
        seg->req = i;
        long sum = 0;
        for(int i = 0; i < maxval; i++){
            seg->tab[i] = getrand();
            sum += seg->tab[i];
        }
        long average = sum/maxval;

        acq_sem(semid, seg_init);
        wait_sem(semid, res_ok);
        lib_sem(semid, seg_init);
        acq_sem(semid, res_ok);
        lib_sem(semid,res_ok);
        lib_sem(semid, seg_dispo);
        printf("Step %d : \n", i);
        printf("average client : %ld \n", average);
        printf("average server : %ld \n", seg->result);
        i++;
    }

    if(shmdt(seg) == -1){
        perror("shmdt");
        exit(1);
    }

    return 0;
}
