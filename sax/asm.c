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

void parseSentence(char* line, int *pCounter);
void parse1(FILE** file);
void parse2();
int main(int argc, char* argv[])
{
    initMnemonicList();
    initParsedCode();
    if (argc == 1) printf("Please enter file path to assemble");
    else {    
        FILE *fin = fopen(argv[1], "r");
    if (fin == 0) printf("Not able to open %s\n",argv[1]);
    else {
        parse1(&fin);
        parse2();
        show_errors();
        createFile(getFileName(argv[1]));
    }
    }
    return 0;
}
 
/* =============================================================================================== */
void parse2() {
    int i = 0;
    assign_valid_address();
    check_label_valid();
    for (;i<pc;i++) {
        check_and_set_line(i);
    }
    /* Set lines with only comments having invalid address */
        assignInstr();
}
void parse1(FILE** file) {
    FILE* fin = *file;
        char str[MAX_LINE_SIZE];
        char c = getc(fin);
        while(c != EOF) {
            clearString(str, MAX_LINE_SIZE);
            str[0] = c;
            if (c == '\n') {
                c = getc(fin);
                continue; 
            }
            fgets(str+1, MAX_LINE_SIZE-2, fin);
            /* fscanf(fin, "%[^\n]%*c", str+1); */
            parseSentence(str, &pc);
            c = getc(fin);
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
    opcodeIndex = getOperation(line, delimeter, size);
    if (opcodeIndex != -1) {
        storeOperation(opcodeIndex, *pCounter);
        startOperand = delimeter+1+parsedCode[*pCounter].op.size;
        storeOperand(*pCounter, startOperand, line);
    } else {
        push_errors("Bogus mnemonic", *pCounter);
    }
}
