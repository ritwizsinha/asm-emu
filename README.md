# SIMPLE ASSEMBLER AND EMULATOR
EMULATOR CURRENTLY BUILDING
1. All files are separated semantically according to the nature of the functions and the data they access. 
    - Functions/
                - Comment.c : Functions related to comment detection and comment removal
                - Utilities.c :  General helper functions required during various operations
    - asm.c : Main file which has all the includes and the main function. The main file includes functions for initializing and parse1 and 
    parse2. It also deals with opening the file and reading it. 
    - constants.c : Initialization and # define of constants needed throughout program
    - error.c : Structure for storing errors and related functions for pushing and printing errors
    - label.c : Structure for storing the labels and related functions for extracting and detecting the labels
    - mnemonic.c : Structure for storing the mnemonics and related functions for initializing and detecting and formatting mnemonics
    - operand.c : Structure for operand 
    - parsedCode.c : Structure for the parsed code and related functions
    - warning.c : Structure for warnings and related functions, pretty similar to errors. 

2. Working: 
    - The file is read and parsed using first parse and stored in the structure with separation.
    - The parse1 parses the sentence and and stores the line number, label, operator and operand and also stores some errors which can be stored at this stage
    - The parse 2 detects all errors and warnings using different logic.
    - If there are errors only the error file is created
    - If there are no errors then the listing file, machine code file and the warnings file is created. 

