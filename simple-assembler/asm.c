#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.c" /* Independent */
#include "error.c" /* Independent */
#include "warnings.c" /* Independent */
#include "functions/Utilities.c" /* Independent */
#include "operand.c"     /* Independent */
#include "mnemonic.c"   /* Independent */
#include "functions/Comment.c" /* Independent */
#include "label.c" /* Dependent on Utilities */
#include "parsedCode.c" /* Dependent on mnemonic and label*/
#include "file.c"
/* PROTOTYPES */
void parseSentence(char* line, int *pCounter);
void parse1(FILE** file);
void parse2();

int main(int argc, char* argv[])
{
    /* Initialize the list of mnemonics in an array */
    initMnemonicList();
    /* Initialize the parsed code array of structures with default values */
    initParsedCode();
    /* Errors out if no argument provided */
    if (argc == 1) printf("Please enter file path to assemble");
    else {    
        /* Reading the file */
        FILE *fin = fopen(argv[1], "r");
        /* Errors out if file does not open */
    if (fin == 0) printf("Not able to open %s\n",argv[1]);
    else {
        /* Running parse1, parses the code in a well defined data structure, ie arrays of structures */
        parse1(&fin);
        /* Running parse2 which validates each lines for errors and warnings*/
        parse2();
        /* Creates the error log, the executable and the listing file according to certain conditions */
        createFile(getFileName(argv[1]));
    }
    }
    return 0;
}
 
/* =============================================================================================== */
void parse2() {
    int i = 0;
    /* Assign valid addresses to each line, index of array does not correspond to correct address as 
    comments do not have any address*/
    assign_valid_address();
    /* Checking for validity of a label and push the errors or warnings
    1.  If label used as an operand it must be defined somewhere
    2. Multiple definitions of labels not allowed 
    3. Labels should be only alphanumeric
    4. The labels can't start with a number*/
    check_label_valid();
    for (;i<pc;i++) {
        /* Checking for errors in the operators and operands. Errors like out of bounds, extra variables etc */
        check_and_set_line(i);
    }
    /* Set lines with only comments having invalid address */
        assignInstr();
    /* Exactly as it sounds */
        issue_warnings_for_unused_labels();
        /* This assembler does not have a data segment thus only after HALT should data be specified */
        check_halt_exists_before_data_or_set();
}
void parse1(FILE** file) {
    FILE* fin = *file;
    /* Static character array for taking a line in */
        char str[MAX_LINE_SIZE];
        /* Getting the first character of the line */
        char c = getc(fin);
        /* Input every line until there is end of file */
        while(c != EOF) {
            /* Clear the string of any previously held trash */
            clearString(str, MAX_LINE_SIZE);
            /* Store the first character taken from the file at the first index */
            str[0] = c;
            /* Break if it is endline because there is nothing in the current line then */
            if (c == '\n') {
                c = getc(fin);
                continue; 
            }
            /* Get the remaining line in the string */
            fgets(str+1, MAX_LINE_SIZE-2, fin);
            /* fscanf(fin, "%[^\n]%*c", str+1); */
            /* Store the current line in the structure as an object with various different properites */
            parseSentence(str, &pc);
            /* Getting the first character of the next line */
            c = getc(fin);
            /* Incrementing program counter(denoting line number useful for errors) */
            pc++;
        };
}
void parseSentence(char* line, int *pCounter) {
    int size = getSize(line);
    int delimeter = -1;
    int opcodeIndex = -1;
    int startOperand;
    /* Find and remove comment */
    int index = hasComment(line);
    /* Check if last chracter is next line, if it is remove it */
    if (line[size-1] == '\n') line[size-1] = '\0';
    if (index != -1) {
        /* Allocate space and store the comment */
        parsedCode[*pCounter].comment = (char*)(malloc((size-index+1)*sizeof(char)));
        strcpy(parsedCode[*pCounter].comment, line + index+1);
        line = removeComment(line, index);
        size = getSize(line);
    }
    /* Remove all whitespace */
    line = removeWhiteSpace(line);
    /* Validating line after removing commment and whitespace */
    validate_line(line);
    size = getSize(line);
    if (!size) return;
    delimeter = hasLabel(line);
        if (delimeter != -1) {
            storeLabel(*pCounter, line, delimeter);
            pushLabel(line, delimeter, *pCounter);
        }
    /* If there is a label and after removing label there is nothing then return */
    if (delimeter != -1 && delimeter == size-1) return; 
    /* Getting the operation and ensuring it is valid */
    opcodeIndex = getOperation(line, delimeter, size);
    if (opcodeIndex != -1) {
        /* Storing the operand to the database */
        storeOperation(opcodeIndex, *pCounter);
        startOperand = delimeter+1+parsedCode[*pCounter].op.size;
        /* Storing the operand after checking for some errors */
        storeOperand(*pCounter, startOperand, line);
    } else {
        push_errors("Bogus mnemonic", *pCounter);
    }
}
