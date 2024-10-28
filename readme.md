# Introduction
This program accepts excel files of student lists in a specific format (a sample of the format is available in xls/). It produces tables that can be used in [sc-im](https://github.com/andmarti1424/sc-im). I made this program to automate table creation for my institute, so it is most likely not useful for most people.

The excel files should be placed in the ./xls/ directory.

If the program is run without arguments, it will ask for the excel file names.
If the excel files are passed as arguments, those will be used instead.

The program will create 2 sheets for each file. one sheet is to be used for attendance, and the other for grades.

# Dependencies
The program uses ssconvert to convert the .xls files to .csv files for processing.

ssconvert can be obtained by installing the gnumeric package on Linux. This package is also available on Mac OS through [homebrew](https://brew.sh/).

# Compiling
The ./compile.sh script will compile the program. It will produce the program in the name of "bin/maketables"

I don't know how to write makefiles.

# Limitations
- Because full names can have any number of individual names, the program deletes the grandfather's name to make the full name shorter. This, however, is not perfect. Once you run the program, double-check to see there are no weird names.

