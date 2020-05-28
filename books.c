#include "header.h"

void Books_exit(struct Book *Books, int LineCount){
    printf("Save state...\n");
    FILE *BookFile=fopen(books_csv, "w");
    check_fopen(BookFile, books_csv);
    
    for(int i=0; i<LineCount; i++){
        fprintf(BookFile,"%010lli;%s;%s;%d;%d\n", Books[i].ISBN, Books[i].Author, Books[i].Name, Books[i].NumAll, Books[i].NumUsed);
    }
    printf("Save successful\n");
    getchar();
    fclose(BookFile);
    return ;
}

struct Book *Books_add(struct Book *Books, int LineCount){
    printf("Enter ISBN:");
    unsigned long long ISBN;
    scanf("%lli", &ISBN);
    for(int i=0; i<LineCount; i++) {
        if(Books[i].ISBN == ISBN){
            notify("i", log_shutdown, 0, NULL, 1);
            p_err("This ISBN is already exist\n");
            return Books;
        }
    }
    
    getchar();
    printf("Enter Author:");
    char *Author;
    
    Author = scan('\n');
    
    printf("Enter book's name:");
    char *Name;
    
    Name = scan('\n');
    
    printf("Enter num of books:");
    int Count = (int)atol(scan('\n'));
//    scanf("%d", &Count);
    
    LineCount++;
    Books=(struct Book*)realloc(Books ,(LineCount)*sizeof(struct Book));
    Books[LineCount-1].ISBN    = ISBN;
    Books[LineCount-1].NumAll  = Count;
    Books[LineCount-1].NumUsed = 0;
    strcpy(Books[LineCount-1].Author, Author);
    strcpy(Books[LineCount-1].Name, Name);
    return Books;
}

void Books_delete(struct Book *Books, int LineCount){
    printf("Enter ISBN:");
    unsigned long long ISBN;
    scanf("%lli", &ISBN);
    printf("Searching book...\n");
    int i=0; int BookLine=-1;
    for (i=0;i<LineCount;i++){
        if(ISBN==Books[i].ISBN){
            printf("Book find...\nPrepere to delete...\n"); BookLine=i; break;
        }
    }
    if (BookLine==-1){
        printf("Book not find.Press enter to leave...\n");
        getchar();getchar();
    } else {
        
        FILE *SBFile=fopen(student_books_csv, "r");
        check_fopen(SBFile, student_books_csv);

        int SBLineCount=0;
        while(!feof(SBFile)){
            if (fgetc(SBFile)=='\n')
                SBLineCount++;
        }
        SBLineCount++;
        fclose(SBFile);
        char Buff[1024];
        SBFile=fopen(student_books_csv, "r");
        int BooksTrigger=0;
        for (i=0; i<SBLineCount;i++){
            fgets(Buff, 1024,SBFile);
            long long ISBN;
            ISBN=atoll(strtok(Buff,";"));
            if (Books[BookLine].ISBN==ISBN){
                BooksTrigger=1;
                break;
            }
        }
        fclose(SBFile);
        
        if (BooksTrigger==0){
            Books[BookLine].ISBN=0;
            printf("Delete successful!\nPress enter to leave...\n");
            getchar();
        }
        else printf("This book on hands!\nPress enter to leave...\n");
        getchar();
        
    }
}

void Books_showall(struct Book *Books, int LineCount){
    quick_sort_struct_book_by_isbn(Books, 0, LineCount);
    for (int i=0; i<LineCount; i++){
        if (Books[i].ISBN!=0) printf("%lli %s %s %d %d\n", Books[i].ISBN,Books[i].Author, Books[i].Name, Books[i].NumAll, Books[i].NumUsed);
    }
}

void Books_find(struct Book *Books, int LineCount){
    printf("Enter ISBN:");
    long long ISBN;
    scanf("%lli", &ISBN);
    printf("Searching...\n");
    int FindLine=-1;
    for (int i=0;i<LineCount;i++){
        if(ISBN==Books[i].ISBN && Books[i].ISBN!=0){
            printf("Book find...\n");
            FindLine=i;
            break;
        }
    
    }
    if (FindLine==-1){
        printf("Book not found, back to menu...\n");
    }
    else{
        printf("ISBN: %lli\nАвтор: %s\nНазвание: %s\nКниг в библиотеке: %d\nКниг на руках: %d\n", Books[FindLine].ISBN,Books[FindLine].Author, Books[FindLine].Name,
               Books[FindLine].NumAll, Books[FindLine].NumUsed);
    
    }
}

void Books_onhands(FILE *BookFile, struct Book *Books, int LineCount){
    printf("Enter ISBN:");
    long long ISBN;
    scanf("%lli", &ISBN);
    printf("Searching book...\n");
    int i=0; int BookLine=-1;
    for (i=0;i<LineCount;i++){
        if(ISBN==Books[i].ISBN && Books[i].ISBN!=0){
            printf("Book find...\n"); BookLine=i; break;
        }
    }
    if (BookLine==-1){
        printf("Book not find.Press enter to leave...\n");
        getchar();getchar();
    } else{
        FILE *SBFile=fopen(student_books_csv, "r");
        if (SBFile==NULL){
            notify("e", "StudentBooksCsvMissing", 0, student_books_csv, 1);
            p_err("File with book's hands out cannot be open");
        } else {
            int SBLineCount=0;
            while(! feof(SBFile)){
                if (fgetc(SBFile)=='\n')
                    SBLineCount++;
            }
            SBLineCount++;
            fclose(SBFile);
            
            char Buff[1024];
            SBFile=fopen(student_books_csv, "r");
            char ArrRecBook[SBLineCount][10];
            char Date[SBLineCount][20];
            int ArrCounter=0;
            for (i=0; i<SBLineCount-1;i++){
                fgets(Buff, 1024,SBFile);
                char CheckRecBook[10];
                long long CheckISBN;
                char CheckDate[32];
                CheckISBN=atoll(strtok(Buff,";"));
                strcpy(CheckRecBook,strtok(NULL,";"));
                strcpy(CheckDate,strtok(NULL,"\n"));
                if (CheckISBN==Books[BookLine].ISBN){
                    strcpy(ArrRecBook[ArrCounter],CheckRecBook);
                    strcpy(Date[ArrCounter],CheckDate);
                    ArrCounter++;
                }
            }
            fclose(SBFile);
            
            if (ArrCounter==0){
                printf("This book not on hands.\n");
            }
            else{
                FILE *StudentFile=fopen(students_csv, "r");
                if (StudentFile==NULL){
                    notify("e", "StudentCsvMissing", 0, students_csv, 1);
                    p_err("File with students cannot be open");
                }
                else{
                    int SLineCount=0;
                    while(! feof(StudentFile)){
                        if (fgetc(StudentFile)=='\n')
                            SLineCount++;
                    }
                    SLineCount++;
                    fclose(BookFile);
                    
                    StudentFile=fopen(students_csv, "r");
                    if (StudentFile==NULL){
                        notify("e", "StudentBooksCsvMissing", 0, student_books_csv, 1);
                        p_err("File with students cannot be open");
                    } else {
                        printf ("This book on hands:\n");
                        for (i=0; i<SLineCount;i++){
                            fgets(Buff, 1024,BookFile);
                            char RecBook[10];
                            strcpy(RecBook,strtok(Buff,";"));
                            int k=0;
                            for (k=0; k<=ArrCounter;k++){
                                if(strcmp(RecBook, ArrRecBook[k])==0){
                                    printf("%s %s %s\n\tReturn Date:%s\n\n",strtok(NULL,";"),strtok(NULL,";"),strtok(NULL,";"), Date[k]);
                                }
                            }
                        }
                        fclose(BookFile);
                    }
                }
            }
        }
    }
    printf("Press enter to leave...\n");
    getchar();getchar();
}

void Books_start(){
    char Buff[1024];
    int Command=-11;
    FILE *BookFile=fopen(books_csv, "r");
    check_fopen(BookFile, books_csv);

    int LineCount = 0;
    while(!feof(BookFile)){
        if (fgetc(BookFile)=='\n'){
            LineCount++;
        }
    }
    if(LineCount == 0){
        notify("e", "Books", 24, "LineCount=0", 0);
        exit(0); //for debuger. there is no sense
    }
    fclose(BookFile);
    
    struct Book *Books;
    Books=(struct Book*)malloc(sizeof(struct Book));
    int i=0;
    BookFile=fopen(books_csv, "r");
    for (i=0; i<LineCount;i++){
        fgets(Buff, 1024,BookFile);
        Books[i].ISBN=atoll(strtok(Buff,";"));
        strcpy(Books[i].Author,strtok(NULL,";"));
        strcpy(Books[i].Name,strtok(NULL,";"));
        Books[i].NumAll=atoi(strtok(NULL,";"));
        Books[i].NumUsed=atoi(strtok(NULL,"\n"));
        Books=(struct Book*)realloc(Books ,(i+2)*sizeof(struct Book));
    }
    fclose(BookFile);
    
    while (Command != 0){
        printf("\n--- Books Library ---\n");
        printf("0 - Save and exit\n");
        printf("1 - Add new book\n");
        printf("2 - Delete book\n");
        printf("3 - Find book\n");
        printf("4 - Show all books\n");
        printf("5 - Find book on hands\n");
        printf("\n");
        
        if(Command == -1){
            p_err("Enter correct command");
            printf("\n");
        }
        
        printf("Books> ");
        scanf("%d", &Command);
        int onemore = -1;// for case 1
        
        switch (Command) {
            case 0:
                Books_exit(Books, LineCount);
                free(Books);
                return;
                break;
            case 1:
                while (onemore != 0){
                    printf("\n--- Add Book ---\n");
                    Books = Books_add(Books, LineCount);
                    LineCount++;
                    while(onemore != 1 && onemore != 0){
                        printf("Enter one more book?\n1 - yes\n0 - no\n");
                        scanf("%d", &onemore);
                        printf("\n");
                    }
                }
                Command = -11;
                break;
            case 2:
                printf("\n--- Delete Book ---\n");
                Books_delete(Books, LineCount);
                LineCount--;
                Command = -11;
                break;
            case 3:
                printf("\n--- Find Book ---\n");
                Books_find(Books, LineCount);
                Command = -11;
                break;
            case 4:
                printf("\n--- Show All Books ---\n");
                Books_showall(Books, LineCount);
                Command = -11;
                break;
            case 5:
                printf("\n--- Find book on hands ---\n");
                Books_onhands(BookFile, Books, LineCount);
                Command = -11;
                break;
            default:
                Command = -1;
                break;break;break;
        }
    }
    free(Books);
}
