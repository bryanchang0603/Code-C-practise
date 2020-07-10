#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <semaphore.h>  
#include <time.h>
#include <signal.h>
#include <stdlib.h>

int main (void){

	int inputFile;
	FILE *resFile;

	struct stat temp;
	char *start;
	unsigned char vec[2];

	int continueBool = 1;
	char modifyInt = -1;
	int modifyRes = 0;
	int modIntBool = 0;
	int modIntLoc = 0;

	pid_t pid;
	sem_t *sem;

	inputFile = open("res.txt", O_RDWR);
	fstat(inputFile, &temp);

    start = mmap(NULL, temp.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, inputFile, 0);
    close(inputFile);
	printf("%d\n", temp.st_size);
    printf("%s", start); 

    sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1); 

	if (( pid = fork () ) == -1) {
		sem_unlink("pSem");
		sem_close(sem);
		perror (" fork ") ;
		exit (0) ;
	}

	if(pid == 0){
		while(1){
			sem_wait(sem);
			printf("current state of resource\n%s", start); 
    		printf("page size:%d\n", getpagesize());
    		printf("current status of page:%d\n", mincore(start, temp.st_size, vec));
    		sem_post(sem);
    		sleep(10);
		}
	}
	else{
	    while(continueBool != 0){
        	printf("please enter the resource modifying\n>>>");
        	scanf("%s", &modifyInt);
        	printf("your input is %c\n", modifyInt);
        	for(int i = 0; i < temp.st_size; i+=4){
            	if(modifyInt == start[i]) {
                	modIntBool = 1;
                	modIntLoc = i;
            	}
        	}
        	if(modIntBool == 0){
            	printf("input invalid\n");
        	}
        	else{
            	modIntBool = 0;
            	printf("please enter the number of resource %d needed\n>>>", modifyInt-48);
            	scanf("%d", &modifyRes);

            	sem_wait(sem);
                start[modIntLoc+2] = (int)(start[modIntLoc+2]) + modifyRes;
                msync(start, temp.st_size, MS_ASYNC);
    			sem_post(sem);
            	}
        	printf("do you want to continue? Yes: 1 or other integer, No: 0\n>>>");
        	scanf("%d", &continueBool);
        }

		kill(pid, SIGKILL);
		sem_unlink("pSem");
		sem_close(sem);
	}
}