#include "header.h"

void checkfile(void);

int main(){
    printf("building...\n");
    _Bool SystemState = 1;
    while(SystemState){
        _Bool StateBooks = 0;
        _Bool StateStudents = 0;
        checkfile();
        printf("start\n");
        FILE *UserFile=fopen(users_csv, "r");
        
        if (UserFile!=NULL){
            int LineCount=0;
            while(!feof(UserFile)){
                if (fgetc(UserFile)=='\n')
                    LineCount++;
            }
        fclose(UserFile);
            
        while (SystemState){
            printf("Enter your login: ");
            char Login[128];
            strcpy(Login, scan('\n'));
            
            printf("Enter your password: ");
            char Password[128];
            strcpy(Password, scan('\n'));
            
            char Buff[1024];
            UserFile = fopen(users_csv, "r");
            check_fopen(UserFile, users_csv);
            for (int i=0; i<LineCount;i++){
                fgets(Buff, 1024,UserFile);
                char CheckLogin[128];
                char CheckPassword[128];
                strcpy(CheckLogin,strtok(Buff,";"));
                strcpy(CheckPassword,strtok(NULL,";"));
                if (strcmp(Password, CheckPassword)==0 && strcmp(Login, CheckLogin)==0){
                    StateStudents=atoi(strtok(NULL,";"));
                    StateBooks=atoi(strtok(NULL,";"));
                    break;
                }
            }
            fclose(UserFile);
            setup_user(Login);
            
            if (StateStudents==1 && StateBooks==1){
                notify("i", log_login, 0, NULL, 1);
                int Command;
                while(SystemState){
                    printf("\n------------\n");
                    printf("Menu:\n");
                    printf("0 - Exit\n");
                    printf("1 - Books\n");
                    printf("2 - Students\n");
                    printf("\n");
                    printf("Main> ");
                    scanf("%d", &Command);
                    if (Command==0){
                        printf("Shutdown... Bye :)\n");
                        notify("i", log_shutdown, 0, NULL, 1);
                        return 0;
                    }
                    else if(Command==1){
                        notify("i", log_enter_books, 0, NULL, 1);
                        Books_start();
                    }
                    else if(Command==2){
                        notify("i", log_enter_students, 0, NULL, 1);
                        Students_start();
                    }
                }
            }
            else if(StateStudents==1){
                notify("i", log_enter_students, 0, NULL, 1);
                Students_start();
                printf("Shutdown... Bye :)\n");
                notify("i", log_shutdown, 0, NULL, 1);
                return 0;
            }
            else if(StateBooks==1){
                notify("i", log_enter_books, 0, NULL, 1);
                Books_start();
                printf("Shutdown... Bye :)\n");
                notify("i", log_shutdown, 0, NULL, 1);
                return 0;
            }
            else{
                notify("w", log_login_failed, 0, NULL, 1);
                p_err("Invalid login or password.");
            }
        }
    }
    }
    return SystemState;
}

