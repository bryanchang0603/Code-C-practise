#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>

void c_ls_l (struct stat fileInfo, struct dirent *ent);
char * getFileName(char * file);
void showInfo(char* file);
void getFile( char* path);

//primary function for part 2
void getFile( char* path){
	DIR *dir;
	struct dirent *ent;
	struct stat fileInfo;
	char newpath[256];
	if ((dir = opendir (path)) != NULL) {// assert it is a file or directory, and run accordingly
		chdir(path);// change the directory in order for ent->d_name to set path properly

		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {//go through all the files and directories in the file
			if(lstat(ent->d_name, &fileInfo) == -1)// if fail to get file information, show error reason
				perror("Error no file");
			else{
				if(S_ISDIR(fileInfo.st_mode) ){// check if is directory
					if(!(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)){
						// .. and . represent upper level directory and current directory

						// create path for new directory and call recursively
						strcpy(newpath, path);
						strcat(newpath, "/");
						strcat(newpath, ent->d_name);
						getFile(newpath);
						chdir(path);// change path back to current directory after recursive call.
					}	
				}
				c_ls_l(fileInfo, ent);//show file information similating ls -l
			}
  		}
  		closedir (dir); //close directory after the all files in it is being viewed
	} 
	else { //execute as input path being a file
		printf("this is a file\n");
		showInfo(path);
	}
}

// similulate ls-l 
void c_ls_l (struct stat fileInfo, struct dirent *ent){
	struct passwd *pw;
	struct group *gr;
	char * time;

	// exporting the permission
	printf( (S_ISDIR(fileInfo.st_mode)) ? "d" : "-");// check if it is dir

	//check user permission on the file
	printf( (fileInfo.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileInfo.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileInfo.st_mode & S_IXUSR) ? "x" : "-");

	//check group permission on the file
    printf( (fileInfo.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileInfo.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileInfo.st_mode & S_IXGRP) ? "x" : "-");

    //check other users permission on the file
    printf( (fileInfo.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileInfo.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileInfo.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    // exporting number of links
    printf("%ld ", fileInfo.st_nlink);

    //exporting Owner's name
    pw = getpwuid(fileInfo.st_uid);
    printf("%s ", pw->pw_name);

    // exporting Group's name
    gr = getgrgid(fileInfo.st_gid);
    printf("%s ", gr->gr_name);

	// exporting size in byte
    printf("%ld\t", fileInfo.st_size);

    // exporting last modification time
    time = ctime(&fileInfo.st_mtime);
    time[24] = '\0';
    printf("%s ", time);

    // exporting the name of file
    printf ("%s\n", ent->d_name);


}

// reporting file information for part 1
void showInfo(char* file){
	struct stat fileInfo;
	struct passwd *pw;
	struct group *gr;
	if(stat(file, &fileInfo) == -1){
		perror("Error no file");
		exit(1);
	}

	// exporting the name of file
	printf("File Name: %s\n", getFileName(file));
	// exporting the permission
	printf("File Permissions: ");
	printf( (S_ISDIR(fileInfo.st_mode)) ? "d" : "-"); // check if it is dir

	//check user permission on the file
	printf( (fileInfo.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileInfo.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileInfo.st_mode & S_IXUSR) ? "x" : "-");

	//check group permission on the file
    printf( (fileInfo.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileInfo.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileInfo.st_mode & S_IXGRP) ? "x" : "-");

    //check other users permission on the file
    printf( (fileInfo.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileInfo.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileInfo.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    // exporting number of links
    printf("File Link: %ld\n", fileInfo.st_nlink);

    //exporting Owner's name
    pw = getpwuid(fileInfo.st_uid);
    printf("Owner's Name is %s\n", pw->pw_name);

    // exporting Group's name
    gr = getgrgid(fileInfo.st_gid);
    printf("Group's Name is %s\n", gr->gr_name);

    // exporting size in byte and size in block
	printf("size in byte is %ld\n", fileInfo.st_size);// in byte
	printf("size in block is %ld with block size %ld\n", fileInfo.st_blocks, fileInfo.st_blksize); // in block
	// exporting last modification time
	printf("last modification time is %s\n", asctime(gmtime(&fileInfo.st_mtime)));

}

//designed to get the name of the file for part 1
char * getFileName(char * file){
	char * FileName;
	char * token = strtok(file, "/");
	while(token != NULL){
		FileName = token;
		token = strtok(NULL, "/");
	}
	return FileName;
}


void main(int argc, char *argv[]){
	getFile(argv[1]);// send the input directory from comman line to funcuon.
}