#include <stdio.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/sem.h>

int main(void){
	int key;
	key_t semkey;
	semkey = 0;
	semget(semkey,1,IPC_CREAT | IPC_EXCL | 0700);
}