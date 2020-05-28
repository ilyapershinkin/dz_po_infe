#include "header.h"

_Bool write_log = log_write;
char User[128]= log_before_start;

void write_log_act(_Bool do_write_log){
    write_log = do_write_log;
}

void setup_user(char user[]){
    for(int i=0; i<128; i++){
        User[i] = 0;
    }
    strcat(User, user);
}

void add_log(char activity[], char params[]){
    if(!write_log){
        return;
    }
    if(activity == NULL){
        return;
    }
    
    _Bool newfile = 0;
    FILE *LogFile = fopen(log_file, "a");
    if (LogFile == NULL){
        printf("%s: Can't open file '%s'. Creating new...\n",WARNING, log_file);
        newfile = 1;
        
        LogFile = fopen(log_file, "a+");
        if (LogFile == NULL){
            printf("%s: Can't create file '%s'. Writing logs be stop.\n",ERROR, log_file);
            fclose(LogFile);
            write_log = 0;
            return;
        }
    }
    
    char timer[20] = {0};
        
    long int s_time = time(NULL);
    struct tm *m_time = localtime(&s_time);
    strftime(timer, 20, "%d.%m.%Y %H:%M:%S", m_time);
    if(newfile){
        fprintf(LogFile, "%c%s%c;%c%s%c;%c%s%c\n",34,timer,34,34,User,34,34,"!!! This message means a new log file !!!",34);
    }
    if(params != NULL && log_params==1){
        fprintf(LogFile, "%c%s%c;%c%s%c;%c%s(%s)%c\n",34,timer,34,34,User,34,34,activity,params,34);
    } else {
        fprintf(LogFile, "%c%s%c;%c%s%c;%c%s%c\n",34,timer,34,34,User,34,34,activity,34);

    }
    
    fclose(LogFile);
}
