#include <stdio.h>
#include <stdlib.h>
#include "mnemonic.c"
#include "functions/Utilities.c"
#include "functions/Comment.c"
#include "parsedCode.c"
#include "label.c"
#define MAX_LINE_SIZE 100

int pc = 0;

void parseSentence(char* line, int *pCounter);
int main(int argc, char* argv[])
{
    initMnemonicList();
    initParsedCode();
    if (argc == 1) printf("Please enter file path to assemble");
    else {    
        FILE *fin = fopen(argv[1], "r");
    if (fin == 0) printf("Not able to open %s\n",argv[1]);
    else {
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
        int i = 0;

        while(i<pc) {    
            printf("Label: %s\nAddr: %d\nOperation: %s\nOpcode: %d\nOperand: %s\nComment: %s\n\n",
            parsedCode[i].label, parsedCode[i].addr, parsedCode[i].op.str, parsedCode[i].op.opcode, parsedCode[i].operand,
            parsedCode[i].comment);
            i++;
        }

        fclose(fin);
    }
    }
    return 0;
}

/* =============================================================================================== */


void parseSentence(char* line, int *pCounter) {
    int size = getSize(line);
    int index = hasComment(line, size);
    if (index != -1) {
        parsedCode[*pCounter].comment = (char*)(malloc((size-index)*sizeof(char)));
        int i=index+1;
        for(;i<size;i++) parsedCode[*pCounter].comment[i-index-1] = line[i];
        line = removeComment(line, index);
        size = getSize(line);
    }

    line = removeWhiteSpace(line, size);
    size = getSize(line);
    parsedCode[*pCounter].addr = *pCounter;
    parsedCode[*pCounter].operand = (char*)(malloc(10*sizeof(char)));
    parsedCode[*pCounter].operand[0] = '\0';
    int delimeter = hasLabel(line,size);
    if (delimeter != -1) {
        char *tmp = getLabel(line, delimeter);
        parsedCode[*pCounter].label = tmp;
        labels[labelArrayIndex].str = tmp;
        labels[labelArrayIndex].addr = *pCounter;
        labelArrayIndex++;
    }
    int opcodeIndex = getOperation(line, delimeter, size);
    if (opcodeIndex != -1) {
        parsedCode[*pCounter].op = mnemonics[opcodeIndex];
        delimeter += (parsedCode[*pCounter].op.size+1);
        int i = delimeter;
        for(;i<size;i++) parsedCode[*pCounter].operand[i-delimeter] = line[i];
    }
}
