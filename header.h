#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

//
// Settings
//

// CSV directory
#define books_csv           "csv/books.csv"
#define users_csv           "csv/users.csv"
#define student_books_csv   "csv/student_books.csv"
#define students_csv        "csv/students.csv"

// Other directory
#define log_file            "library.log"
#define backup_derictory    "_backups/"

// log enable
#define log_write           1
#define log_params          1

// log config:  "massage" - enable; NULL - for disable
#define log_before_start    "BeforeStart"
#define log_login           "StartSystem"
#define log_login_failed    "loginFailed"
#define log_shutdown        "ShutdownSystem"
#define log_enter_books     "EnterBooks"
#define log_enter_students  "EnterStudents"
#define log_cant_open_file  "CantOpenFile"
#define log_dup_ISBN        NULL //"IsbnDuplication"
#define log_EOF_detected    NULL //"EOFDetected"

// color + massages
#define COLOR_CLEAR         "\x1b[0m"
#define COLOR_RED           "\x1b[31m"
#define COLOR_CYAN          "\x1b[36m"
#define COLOR_YELLOW        "\x1b[33m"
#define COLOR_BOLD          "\x1b[1m"
#define ERROR               "[\x1b[1m\x1b[31mERROR\x1b[0m]"
#define WARNING             "[\x1b[1m\x1b[33mWARNING\x1b[0m]"
#define INFO                "[\x1b[1m\x1b[36mINFO\x1b[0m]"

//
//  functions.c
//

char *scan          (char EndSymb);
void printlogo      (void);
void p_warn         (char text[]);
void p_err          (char text[]);
void p_info         (char text[]);
void check_fopen    (FILE *test_file, char filename[]);
void notify         (char mode[], char text[], int exitcode, char *params, _Bool silent);
void checkfile      (void);

//
// books.c
//

struct Book{
    unsigned long long ISBN;
    char Author[128];
    char Name[128];
    int  NumAll;
    int  NumUsed;
};

void            Books_start     (void);
void            Books_exit      (struct Book *Books, int LineCount);
struct Book*    Books_add       (struct Book *Books, int LineCount);
void            Books_delete    (struct Book *Books, int LineCount);
void            Books_showall   (struct Book *Books, int LineCount);
void            Books_find      (struct Book *Books, int LineCount);
void            Books_onhands   (FILE *BookFile, struct Book *Books, int LineCount);

//
// students.c
//

struct Student{
    char RecBook[10];
    char Name[32];
    char SName[32];
    char OName[32];
    char Faculty[10];
    char Spec[1024];
};

void            Students_start          (void);
void            Students_exit           (struct Student *Students, int LineCount);
struct Student* Students_add            (struct Student *Students, int LineCount);
void            Students_delete         (struct Student *Students, int LineCount);
void            Students_backup_save    (struct Student *Students, int LineCount);
struct Student* Students_backup_upload  (struct Student *Students, int LineCount, FILE *StudentsFile);
void            Students_search         (struct Student *Students, int LineCount);
void            Students_showall        (struct Student *Students, int LineCount);

//
// log.c
//

void write_log_act  (_Bool do_write_log);
void setup_user     (char user[]);
void add_log        (char activity[], char params[]);

//sort from functions.c
void swap                           (struct Book *a, struct Book *b);
void quick_sort_struct_book_by_isbn (struct Book *Books, int left, int right);
