# Introduction
This program accepts excel files of student lists and produces tables that can be used in sc-im. The excel files should be placed in the ./xls/ directory.
A sample excel file with random names is available for testing.

If the program is run without arguments, it will ask for the excel file names.
If the excel files are passed as arguments, those will be used instead.

The program will create 2 sheets for each file. one sheet is to be used for attendance, and the other for grades.

# Dependencies
The program uses ssconvert to convert the .xls files to .csv files for processing.
ssconvert can be obtained by installing the gnumeric package on linux, or homebrew on Mac OS.

# Compiling
The ./compile.sh script will compile the program. It will produce the program in the name of "bin/maketables"
I don't know how to do makefiles.

# Limitations
- Because full names can have any number of individual names, the program deletes the grandfather's name to make the full name shorter. This, however, is not perfect. Once you run the program, double-check to see there are no weird names.

