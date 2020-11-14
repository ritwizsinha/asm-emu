#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_LINE_SIZE 100
int pc = 0;
#include "error.c" /* Independent */
#include "functions/Utilities.c" /* Independent */
#include "operand.c"     /* Independent */
#include "mnemonic.c"   /* Independent */
#include "functions/Comment.c" /* Independent */
#include "label.c" /* Dependent on Utilities */
#include "parsedCode.c" /* Dependent on mnemonic and label*/


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
        /* int i = 0; */
        parse1(&fin);
      /*   while(i<pc) {    
            printf("Label: %s\nAddr: %d\nOperation: %s\nOpcode: %d\nOperand: %s\nComment: %s\n\n",
            parsedCode[i].label, parsedCode[i].addr, parsedCode[i].op.str, parsedCode[i].op.opcode, parsedCode[i].opr.op,
            parsedCode[i].comment);
            i++;
        } */
        
        /* printf("%d\n", error_list_index); */
        parse2();
        show_errors();
    }
    }
    return 0;
}

/* =============================================================================================== */

void parse2() {
    int i = 0;
    for (;i<pc;i++) {
        check_and_set_line(i);
    }
}
void parse1(FILE** file) {
    FILE* fin = *file;
      char str[MAX_LINE_SIZE];
        char c = getc(fin);
        while(c != EOF) {
            str[0] = c;
            if (c == '\n') {
                c = getc(fin);
                continue; 
            }
            fscanf(fin, "%[^\n]%*c", str+1);
            parseSentence(str, &pc);
            c = getc(fin);
            pc++;
        };
}
void parseSentence(char* line, int *pCounter) {
    int size = getSize(line);
    int delimeter = -1;
    int opcodeIndex = -1;
    /* Find and remove comment */
    int index = hasComment(line);
    if (index != -1) {
        /* Allocate space and store the comment */
        int i=index+1;
        parsedCode[*pCounter].comment = (char*)(malloc((size-index)*sizeof(char)));
        for(;i<size;i++) parsedCode[*pCounter].comment[i-index-1] = line[i];
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
    if (delimeter != -1 && delimeter == size-1) return; 
    opcodeIndex = getOperation(line, delimeter, size);
    parsedCode[*pCounter].addr = *pCounter;
    parsedCode[*pCounter].opr.op = (char*)(malloc(10*sizeof(char)));
    parsedCode[*pCounter].opr.op[0] = '\0';
    if (delimeter != -1) {
        char *tmp = getLabel(line, delimeter);
        if (!bogusLabel(tmp, getSize(tmp))) {
            parsedCode[*pCounter].label = tmp;
            storeLabel(tmp, *pCounter);
            labelArrayIndex++;
        } else {
            push_errors("Bogus label found", pc);
        }
    }
    if (opcodeIndex != -1) {
        int i = delimeter == -1 ? 0 : delimeter;
        parsedCode[*pCounter].op = mnemonics[opcodeIndex];
        /* Getting operand starting point and storing the operand */
        delimeter += (parsedCode[*pCounter].op.size+1);
        for(;i<size;i++) parsedCode[*pCounter].opr.op[i-delimeter] = line[i];
    } else {
        push_errors("Undefined mnemonic", *pCounter);
    }
}
