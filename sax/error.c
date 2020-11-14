#define MAX_ERROR_SIZE 100
struct Error {
    char* msg;
    int line;
} errors[MAX_ERROR_SIZE];
int error_list_index=0;

void push_errors(char* errorMsg, int pc) {
    errors[error_list_index].line = pc;
    errors[error_list_index].msg = errorMsg;
    error_list_index++;
}

int ignoreList[MAX_ERROR_SIZE];
int ignore_list_iterator = 0;
void show_errors() {
    int i = 0;
    while(i < error_list_index) {
        printf("Error, Line %d : %s\n", errors[i].line+1, errors[i].msg);
        i++;
    }
}

