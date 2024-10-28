// this file contains helper functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mylib.h"

int convertToCSV(char *fileName, char *baseName) {
	char command[128];	
	sprintf(command, "ssconvert %s ./%s.csv", fileName, baseName);
	if (system(command) != 0) {
		printf("Error converting %s\n", fileName);
		return 1;
	} else {
		printf("%s converted successfully.\n", fileName);
		return 0;
	}
}

void stripNewLineChar(char *line) {
	char *ptr = strchr(line, '\n');
	if (ptr != NULL) {
   		*ptr = '\0';
	}
}

// get file name without the extension
int getBaseName(char *baseName, char *fileName) {
	char* extensionStart = strchr(fileName, '.');
		if (extensionStart == NULL) {
			printf("File name does not have an extension: %s\n", fileName);
			return 1;
		} else {
        	int length = extensionStart - fileName;
			strncpy(baseName, fileName, length);
			baseName[length] = '\0';
			return 0;
		}
}

void readCSV(Course *course, FILE *file, char *semesterCode) {
    char line[MAX_LINE_WIDTH];

	// skip first two lines
    for (int i = 0; i < 2; i++) {
        fgets(line, MAX_LINE_WIDTH, file);
    }

	// get course name
    fgets(line, MAX_LINE_WIDTH, file);
    char *courseName = strtok(line, ",");
    courseName = strtok(NULL, ",");
	courseName = strtok(courseName, "\"");

	strcpy(course->name, courseName);

	// get section number
    fgets(line, MAX_LINE_WIDTH, file);
    char *sectionString = strtok(line, ",");
    sectionString = strtok(NULL, ",");
	sectionString = strtok(sectionString, "(");
	sectionString = strtok(NULL, ")");

    course->section = atoi(sectionString);

	// get seq number
    fgets(line, MAX_LINE_WIDTH, file);
    char *seq= strtok(line, ",");
	seq = strtok(NULL, ",");
	
	course->seq = atoi(seq);

	// get course code 
	if (strcmp(course->name, "Electronic Circuits Troubleshooting") == 0){
		strcpy(course->code, "ienx2215");
		strcpy(course->shortName, "TRBL");
	} else if (strcmp(course->name, "Electronic Circuits") == 0){
		strcpy(course->code, "ienx1211");
		strcpy(course->shortName, "ELCR");
	} else if (strcmp(course->name, "Instrumentation Skills Practice 2") == 0){
		strcpy(course->code, "inct2433");
		strcpy(course->shortName, "ISP2");
	} else if (strncmp(course->name, "PLC", strlen("PLC")) == 0){
		strcpy(course->code, "inct2431");
		strcpy(course->shortName, "PLC");
	} else {
		printf("Error: cannot figure out course, maybe you have a non-standard file?\n");
		return;
	}

	printf("course: %s: %s, section: %d\n", course->code, course->name, course->section);
	printf("seq = %d\n", course->seq);
	printf("----------------------------------------------------\n");

	getStudentsInfo(course, file);
}

void getStudentsInfo(Course *course, FILE *file) {
	//skip course information
	rewind(file);
	char line[MAX_LINE_WIDTH];
    for (int i = 0; i < 10; i++) {
        fgets(line, MAX_LINE_WIDTH, file);
    }

	course->studentCount = 0;
	int i = 0;
	while(fgets(line, MAX_LINE_WIDTH, file)) {
		char *cursor = strtok(line, ",");
		cursor = strtok(NULL, ",");
		course->students[i].id = atoi(cursor);
		
		cursor = strtok(NULL, ",");
		cursor = strtok(cursor, "\"");
		char tempString[128];
		strcpy(tempString, cursor);
		removeDoubleSpaces(tempString);
		removeGrandfatherName(tempString);
		capitalizeFirstLetter(tempString);
		strcpy(course->students[i].name, tempString);
		course->studentCount++;

		i++;
	}
}

// this function exists because some student names have double spaces between names
void removeDoubleSpaces(char *name) {
    int len = strlen(name);
    for (int i = 0; i < len - 1; i++) {
        if (name[i] == ' ' && name[i+1] == ' ') {
            // Shift all characters after the extra space to the left
            for (int j = i+1; j < len; j++) {
                name[j] = name[j+1];
            }
            // Null-terminate the string
            name[len-1] = '\0';
            // Repeat the process to handle multiple extra spaces
            i--;
            len--;
        }
    }
}

// shorten full name by removing 3rd name (usually grandfather's name)
void removeGrandfatherName(char *str) {
    char *space1 = strchr(str, ' ');
    char *space2 = strchr(space1 + 1, ' ');
    char *space3 = strchr(space2 + 1, ' ');

    if (space1 == NULL || space2 == NULL || space3 == NULL) {
        printf("Invalid string format. Expected 4 words separated by spaces.\n");
        return;
    }

    // move characters from the fourth word to the position of the third word
    int len = strlen(space3);
    memmove(space2 + 1, space3 + 1, len);

    // remove the trailing space
    space2[len] = '\0';
}

// print header info of sc file
void printBasicInfo(FILE *file, Course *courses, int i, int inGradesSheet) {
	fprintf(file, "\
offscr_sc_cols 0\n\
offscr_sc_rows 0\n\
nb_frozen_rows 0\n\
nb_frozen_cols 0\n\
nb_frozen_screenrows 0\n\
nb_frozen_screencols 0\n\
format A 6 2 0\n\
format B 12 2 0\n\
format C 30 2 0\n\
");

	if (!inGradesSheet)
		fprintf(file, "format D 7 0 0\n");
	else
		fprintf(file, "\
format D 7 2 0\n\
format E 7 2 0\n\
format F 7 2 0\n\
format G 7 2 0\n\
format H 7 2 0\n\
format I 7 2 0\n\
");
	fprintf(file, "label A0 = \"Course\"\n");
	fprintf(file, "label B0 = \"%s\"\n", courses[i].shortName);
	fprintf(file, "leftstring D0 = \"Seq.\"\n");
	fprintf(file, "label E0 = \"%d\"\n", courses[i].seq);
	fprintf(file, "\
label A2 = \"Seq.\"\n\
label B2 = \"Student ID\"\n\
label C2 = \"Student Name\"\n\
");
}

// print footer info of sc file
void printEndInfo(FILE *file, int inGradesSheet) {
	if (!inGradesSheet)
		fprintf(file, "\
cellcolor C0 \"fg=CYAN bg=DEFAULT_COLOR\"\n\
cellcolor A2:D2 \"fg=YELLOW bg=DEFAULT_COLOR underline=1\"\n\
cellcolor E2 \"fg=YELLOW bg=DEFAULT_COLOR\"\n\
goto A0\n\
");
	else
		fprintf(file, "\
cellcolor A2:I2 \"fg=YELLOW bg=DEFAULT_COLOR underline=1\n\
goto A0\n\
");
}

// main sc file creation
void createTables(Course *courses, char *semesterCode, int filesCount) {
	char fileName[MAX_FILENAME_LEN];

	sprintf(fileName, "master%s.sc", semesterCode);

	FILE *file = fopen(fileName, "w");
	int inGradesSheet = 0;
	for (int i=0; i < filesCount; i++)
		fprintf(file, "newsheet \"%s_%d\"\n", courses[i].shortName, courses[i].section);
	
	for (int i=0; i < filesCount; i++)
		fprintf(file, "newsheet \"%s%d_grd\"\n", courses[i].shortName, courses[i].section);

	for (int i=0; i < filesCount; i++) {
		fprintf(file, "movetosheet \"%s_%d\"\n",  courses[i].shortName, courses[i].section);
		printBasicInfo(file, courses, i, inGradesSheet);
		for (int j = 0; j < courses[i].studentCount; j++) {
			fprintf(file, "label A%d = \"%d\"\n", j+3,j+1);
			fprintf(file, "label B%d = \"%ld\"\n", j+3, courses[i].students[j].id);
			fprintf(file, "leftstring C%d = \"%s\"\n", j+3, courses[i].students[j].name);
		}
		printEndInfo(file, inGradesSheet);
	}

	for (int i=0; i < filesCount; i++) {
		inGradesSheet = 1;
		fprintf(file, "movetosheet \"%s%d_grd\"\n",  courses[i].shortName, courses[i].section);
		printBasicInfo(file, courses, i, inGradesSheet);
		fprintf(file, "\
label D2 = \"Q1\"\n\
label E2 = \"Mid\"\n\
label F2 = \"Q2\"\n\
label G2 = \"Fin\"\n\
label H2 = \"Bns\"\n\
label I2 = \"Tot\"\n\
");
		for (int j = 0; j < courses[i].studentCount; j++) {
			fprintf(file, "label A%d = \"%d\"\n", j+3,j+1);
			fprintf(file, "label B%d = \"%ld\"\n", j+3, courses[i].students[j].id);
			fprintf(file, "leftstring C%d = \"%s\"\n", j+3, courses[i].students[j].name);
		}
		printEndInfo(file, inGradesSheet);
	}

}

void capitalizeFirstLetter(char *str) {
    int len = strlen(str);

    // Check if the string is not empty
    if (len > 0) {
        // Capitalize the first letter of the string
        str[0] = toupper(str[0]);

        // Capitalize the first letter of each subsequent word
        for (int i = 1; i < len; i++) {
            // Check if the current character is a space
            if (str[i - 1] == ' ') {
                str[i] = toupper(str[i]);
            } else {
                str[i] = tolower(str[i]);
            }
        }
    }
}

