/* Maximum labelswe can have */
#define lArrSize 100
/* Maximum size of a single line in assembly code */
#define MAX_LINE_SIZE 500
/* Max number of lines in the code */
#define MAX_CODE_SIZE 1000
/* Number of identified mnemonics in the language */
#define MNEMONIC_STR_SIZE 21
/* Max size of label in language */
#define MAX_LABEL_SIZE 20
/* Size of warning array */
#define MAX_WARNING_SIZE 200
/* Size of error array */
#define MAX_ERROR_SIZE 100
/* Max file size */
#define MAX_FILENAME_SIZE 50



/* Index in the list of labels */
int label_array_index = 0;
/* Index in the parsed code array */
int pc = 0;
/* Index of the error array */
int error_list_index=0;
/* Index of ignore list */ 
int ignore_list_iterator = 0;
/* Warning list index */
int warning_index = 0;
