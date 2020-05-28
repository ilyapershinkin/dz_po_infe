#include "header.h"

char *scan(char EndSymb){
    int i=0,n=1;
    char *arr,c;
    arr = calloc(n, sizeof(char));
    while((c=getchar()) != EndSymb && c!='\n' && c!=EOF){
        arr[i]=c;
        n++;
        arr = (char*) realloc(arr, n);
        i++;
    }
    if(c==EOF){
        p_info("\nDetected EOF. System shutdown quickly...\n");
        notify("i", log_EOF_detected, 50, NULL, 1);
    }
    arr = (char*) realloc(arr, n+1);
    arr[i]='\0';
    return arr;
}

void p_warn(char text[]){
    printf("%s: %s\n",WARNING,text);
}

void p_err(char text[]){
    printf("%s: %s\n",ERROR,text);
}

void p_info(char text[]){
    printf("%s: %s\n",INFO,text);
}


void check_fopen(FILE *test_file, char filename[]){
    if(test_file == NULL){
        char text[512] = {0};
        char text2[512] = {0};
        strcat(text, log_cant_open_file);
        strcat(text, ": ");
        strcat(text, filename);
        
        strcat(text2, "Can't open file: ");
        strcat(text2, filename);

        notify("e", text, 52, NULL, 1);
        p_err(text2);
    }
}

/** Notification function with log and exit
    @param mode "e" for ERROR, "w" for WARNING, "i" for INFO
    @param text text to
    @param exitcode exit with code (2 digit num), 0 for no exit
    @param params function's parameters to write log
    @param silent to not printf it
    
    Exit Codes:
    @li 1st digit:
        @li - 1: from main
        @li - 2: from books
        @li - 3: from student
        @li - 4: from log library
        @li - 5: from function library
        @li - 9: from other
    @li 2rd digit:
        @li - 0: critical ???
        @li - 1: unexpecting logic
        @li - 2: file access
        @li - 3: file syntax
        @li - 3: memory
        @li - 4: variable
        @li - 9: unknown
**/
void notify(char mode[], char text[], int exitcode, char *params, _Bool silent){
    if(text == NULL){
        return;
    }
    char new_text[1024] = {0};
    char cmode = mode[0];
    if(cmode == 'e'){
        strcat(new_text, "[ERROR]: ");
        if(!silent)
            printf("%s: %s\n",ERROR, text);
    } else if(cmode == 'w'){
        strcat(new_text, "[WARNING]: ");
        if(!silent)
            printf("%s: %s\n",WARNING, text);
    } else if(cmode == 'i'){
        strcat(new_text, "[INFO]: ");
        if(!silent)
            printf("%s: %s\n",INFO, text);
    }else {return;}
    strcat(new_text, text);
    add_log(new_text, params);
    
    if(exitcode >= 10 && exitcode <= 99){
        exit(exitcode);
    }
}

void checkfile(void){
    _Bool err=0;
    FILE *test_books_csv = fopen(books_csv, "r");
    FILE *test_users_csv = fopen(users_csv, "r");
    FILE *test_student_books_csv = fopen(student_books_csv, "r");
    FILE *test_students_csv = fopen(students_csv, "r");
    FILE *test_log_file = fopen(log_file, "r");
    if(test_books_csv==NULL){
        printf("%s: Can't open book library '%s'\n",ERROR,books_csv); err=1;
        notify("e", "BooksCsvMissing", 0, NULL, 1);
    }
    if(test_users_csv==NULL){
        printf("%s: Can't open users library '%s'\n",ERROR,users_csv); err=1;
        notify("e", "UsersCsvMissing", 0, NULL, 1);
    }
    if(test_student_books_csv==NULL){
        printf("%s: Can't open student's book library '%s'\n",ERROR,student_books_csv); err=1;
        notify("e", "StudentBooksCsvMissing", 0, NULL, 1);
    }
    if(test_students_csv==NULL){
        printf("%s: Can't open students library '%s'\n",ERROR,students_csv); err=1;
        notify("e", "StudentsCsvMissing", 0, NULL, 1);
    }
    if(log_write==1 && test_log_file==NULL){
        printf("%s: Can't open log file '%s', creating new...\n",WARNING,log_file);
        notify("w", "LogFileCreatingNew", 0, NULL, 1);
    }
    fclose(test_books_csv);
    fclose(test_users_csv);
    fclose(test_student_books_csv);
    fclose(test_students_csv);
    fclose(test_log_file);
    if(err){
        printf("Press enter to exit...");
        getchar();
        notify("e", NULL , 52, NULL, 1);
    }
}

void swap(struct Book *a, struct Book *b) {
    struct Book tmp = *a;
    *a = *b;
    *b = tmp;
}

void quick_sort_struct_book_by_isbn(struct Book *Books, int left, int right){
    int i, mid;
    if (left >= right)
        return;
    swap(&Books[left], &Books[(left + right)/2]);
    mid = left;
    
    for (i = left+1; i <= right; i++)
        if (Books[i].ISBN < Books[left].ISBN)
            swap(&Books[++mid], &Books[i]);
    
    swap(&Books[left], &Books[mid]);
    quick_sort_struct_book_by_isbn(Books, left, mid-1);
    quick_sort_struct_book_by_isbn(Books, mid+1, right);
}
