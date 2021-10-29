/* 
Vivian Duong
CS 360 HW3
Ben McCamish 
9/21/21

This function takes in a path and returns the number of readable
files in the given path. If no path is given, then the number of 
readbale files will be returned for the cwd.

*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>


// checks if path is a directory 
int isDirectory(char *pathName) {
	struct stat area, *s = &area;
	return (lstat(pathName, s) == 0) && S_ISDIR (s->st_mode);
}

// checks if the file is readable
int canRead(char *pathName) {
	struct stat area, *s = &area;
	lstat(pathName, s); 
	if (access(pathName, R_OK) == 0) {
	 	return 1; // return 1 if readable
	} else {
		return -1; // return -1 if not readable
	}
}

// counts number of readable files in in path
int readable(char *inputPath) {
	char path[256]; 		// stores path name
	struct dirent *dp; 		// directory pointer
	int count = 0; 			// counts number of readable files
	int closeCheck;			// stores return value of closedir
	struct stat area, *s = &area; 

	// if path is null, use cwd
	if (inputPath == NULL){
		inputPath = ".";
	}

	DIR *directory = opendir(inputPath);

	// checks if path is a regular file
	// and if it is readable
	if (lstat(inputPath, s) == 0){
		if(S_ISREG(s->st_mode)) {
			if(canRead(inputPath) == 1) {
				count++;
				return count;
			} else {
				return count;
			}
		}
	} else {
		// if not directory throw error
		if (!isDirectory(inputPath)) {
			return errno * -1;
		}
	}

	// if path is directory, recurse
	if (isDirectory (inputPath)) {
		if (!directory) {
			return errno * -1;
		}
		if ((access(inputPath, R_OK)) == 0) {
			while ((dp = readdir(directory)) != NULL) {
				if (strcmp(dp->d_name, ".") != 0 && 
					strcmp(dp->d_name, "..") != 0) {
					strcpy(path, inputPath);
					strcat(path, "/");
					strcat(path, dp->d_name);
					if (access(path, R_OK) == 0) {
						count += readable(path);
					}
				}
			}
		} 
	}

	closeCheck = closedir(directory);
	if (closeCheck < 0){
		return errno *-1;
	}
	return count;
}

