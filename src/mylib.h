#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>
#define MAX_LINE_WIDTH 1024
#define MAX_FILENAME_LEN 64
#define COURSES_MAX 8
#define STUDENTS_MAX 32

int convertToCSV(char *fileName, char *baseName);
void stripNewLineChar(char *line);
int getBaseName(char *baseName, char *fileName);
void removeDoubleSpaces(char *name);
void removeGrandfatherName(char *name);
void capitalizeFirstLetter(char *str);

typedef struct{
	long int id;
	char name[128];
} Student;

typedef struct{
	char code[16];
	char name[128];
	char shortName[5];
	int section;
	int seq;
	Student students[STUDENTS_MAX];
	int studentCount;
} Course;

void getStudentsInfo(Course *course, FILE *file);
void readCSV(Course *course, FILE *file, char *semesterCode);
void createTables(Course *courses, char *semesterCode, int filesCount);

#endif
