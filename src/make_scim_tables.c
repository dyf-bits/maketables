#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylib.h"
int filesCount = 0;

int main(int argc, char** argv) {
	char fileName[MAX_FILENAME_LEN];
	int argMode = 0;
	if (argc > 1) argMode = 1;

	// get input files
	char line[MAX_LINE_WIDTH];
	char files[10][64]; //10 files, 64 characters for each file name
	while (1) {
		if (!argMode) {
			printf("Enter name of excel file without the dir (p: proceed, q: quit): ");
			fgets(line, sizeof(line), stdin);
			stripNewLineChar(line);
	
			if (strcmp(line, "p") == 0) break;
			else if (strcmp(line, "q") == 0) return 0;
			
			strcpy(fileName, line);
		}
		else {
			if (filesCount+1 < argc)
				strcpy(fileName, argv[filesCount+1]);
			else break;
		}
		char baseName[MAX_FILENAME_LEN];
		getBaseName(baseName, fileName);
		
		convertToCSV(fileName, baseName);

		snprintf(files[filesCount], 64, "%s.csv", baseName);
		filesCount++;
	}
	printf("%d files converted successfully.\n", filesCount);
	
	Course *courses;
	courses = (Course *)malloc(sizeof(Course)*filesCount);
	if (courses == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}
	char semesterCode[MAX_LINE_WIDTH];
	printf("Enter current semester code: ");
	fgets(semesterCode, sizeof(semesterCode), stdin);
   	stripNewLineChar(semesterCode);

	printf("----------------------------------------------------\n");
	for (int i = 0; i < filesCount; i++) {
		char fileWithPath[MAX_FILENAME_LEN];
		sprintf(fileWithPath, "./%s", files[i]);
		FILE *file = fopen(fileWithPath, "r");
		readCSV(&courses[i],file,semesterCode);
        fclose(file);
	}

	printf("creating tables...\n");
	createTables(courses, semesterCode, filesCount);

	free(courses);

	if (system("rm ./xls/*.csv") != 0) {
		puts("could not delete .csv files in ./");
		return 1;
	}

	puts("[done]");
	return 0;
}

