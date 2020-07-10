#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

int main (void){
	int inputFile;
	int* resource = malloc(20* sizeof(int));
	FILE *resFile;
	int count = 0;

	struct stat temp;
	int *start;

	int continueBool = 1;
	int modifyInt = -1;
	int modifyRes = 0;
	int modIntBool = 0;
	int modIntLoc = 0;

    resFile = fopen("res.txt", "r");
    for (char c = getc(resFile); c != EOF; c = getc(resFile)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
    resource = realloc(resource, count * 2 * sizeof(int));
    printf("%d\n", count);
    rewind(resFile);
    for(int i = 0; i < count; i++){
    	fscanf(resFile, "%d", &resource[i*2]);
    	fscanf(resFile, "%d", &resource[i*2+1]);
    	printf("%d\t%d\n", resource[i*2], resource[i*2 + 1]);
    }
    fclose(resFile);

	inputFile = open("res.txt", O_RDONLY);
	fstat(inputFile, &temp);
	start = mmap(NULL, temp.st_size, PROT_WRITE, MAP_PRIVATE, inputFile, 0);
	printf("%d\n", temp.st_size);
	close(inputFile);
    printf("%s", start); 


    while(continueBool != 0){
    	printf("current resource\n");
    	for(int i = 0; i < count; i++){
			printf("%d\t%d\n", resource[i*2], resource[i*2 + 1]);
    	}
    	printf("please enter the resource modifying\n>>>");
    	scanf("%d", &modifyInt);
    	printf("your input is %d\n", modifyInt);
    	for(int i = 0; i < count; i++){
    		if(modifyInt == resource[i*2]) {
    			modIntBool = 1;
    			modIntLoc = i;
    		}
    	}
    	if(modIntBool == 0){
    		printf("input invalid\n");
    	}
    	else{
    		modIntBool = 0;
    		printf("please enter the number of resource %d needed\n>>>", modifyInt);
    		scanf("%d", &modifyRes);
    		if(resource[modIntLoc*2 + 1] - modifyRes >= 0){
    			resource[modIntLoc*2 + 1] = resource[modIntLoc*2 + 1] - modifyRes;
    			modifyRes = 0;
    		}
    		else{
    			printf("not enough resource\n");
    		}
    	}
    	printf("do you want to continue? Yes: 1 or other integer, No: 0\n>>>");
    	scanf("%d", &continueBool);
    }
    printf("final resoruce\n");
    for(int i = 0; i < count; i++){
		printf("%d\t%d\n", resource[i*2], resource[i*2 + 1]);
    }


    munmap(start, temp.st_size); /* 解除映射 */
}