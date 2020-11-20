/* RITWIZ SINHA
1801CS39
I hereby declare that all this code is written by me and me alone */
/* Operand structure stores the operand meaningfully
@op -> the string for the  stored  operand
@isDigit -> boolean to show whether the operand is digit or not
@digit -> denotes the digit 
@isLabel -> boolean value which shows whether the operand is label or not
@noOp -> finds whether the particular line includes an operand or not*/

struct operand {
    char* op;
    int isDigit;
    int digit;
    int isLabel;
    int noOp;
};