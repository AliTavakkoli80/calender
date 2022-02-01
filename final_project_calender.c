/*****************************Calender.c*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <io.h>

typedef struct day_tasks{
    struct tm date;
    char topic[20];
    char comment[200];
    char task_is_done;
    char flag_del;
}TASK;
TASK task;

typedef struct day_events{
    struct tm date;
    char topic[20];
    char comment[200];
    char flag_del;
}EVENT;
EVENT event;

struct tm * time_console(void);
void mi_to_sh(int *,int *,int *);
void sh_to_mi(int *,int *,int *);
void day_name_mon_name_sh(struct tm *,char [],char []);
void day_name_mon_name_mi(struct tm *,char [],char []);
void pre_next_day(struct tm* ,char);
void gotoxy(int , int );
void line(char ,int ,int ,int ,int);    /*This function is only for horizontal or vertical lines*/ 
void calender_menu (void);
void day_menu (struct tm*);
void task_menu (struct tm*);
int edite_task_menu_write (struct tm*);
void event_menu (struct tm*);
int edite_event_menu_write (struct tm*);
int comput_number_rec_task(void);
int comput_number_rec_event(void);
int read_file_task(struct tm*);
int read_file_event(struct tm*);
int update_file_event(int ,struct tm *);
int update_file_task(int ,struct tm *);
int update_file_task_done(int rec_number,struct tm *);
int delete_file_task(struct tm *);
int delete_file_event(struct tm *);
int update_file_date_task(int ,struct tm *);
int update_file_date_event(int ,struct tm *);
int read_file_task_topic(char [],struct tm *);
int read_file_event_topic(char [],struct tm *);


/*********************************main*********************************/
int main(int argc, char** argv) {
    char dayname[15],moname[15];
    system("cls");
    calender_menu();
    return 0;
}
/**********************************************************************/
void calender_menu (void){
    struct tm timer;
    struct tm * timer_ptr = &timer;
    int year_day;
    int i=0;
    char input;
    char tmp[20];
    system("cls");
    printf("********** < MY CALENDER > **********\n");
    printf("* Press D to enter your date \n");
    printf("* Press E to search the topic of an event \n");
    printf("* Press S to search the topic of a task \n");
    printf("* Press T to go to today\n");
    printf("* Press C to show the clock\n");
    printf("* Press Q to exit");
    timer.tm_hour=time_console()->tm_hour;
    timer.tm_isdst=time_console()->tm_isdst;
    timer.tm_min=time_console()->tm_min;
    timer.tm_sec=time_console()->tm_sec;
    input = getch();
    switch (input){
        case 't':
            timer.tm_year=time_console()->tm_year;
            timer.tm_mon=time_console()->tm_mon;
            timer.tm_mday=time_console()->tm_mday;
            timer.tm_wday=(time_console()->tm_wday+1)%7;
            mi_to_sh(&timer.tm_year,&timer.tm_mon,&timer.tm_mday);
            day_menu (timer_ptr);
            break;
        case 'd':
            system("cls");
            printf("Enter year : ");
            scanf("%d",&timer.tm_year);
            printf("Enter month : ");
            scanf("%d",&timer.tm_mon);
            printf("Enter day of month : ");
            scanf("%d",&timer.tm_mday);
            if (timer.tm_mon >=1 && timer.tm_mon <=6){
                year_day=(timer.tm_mon-1)*31+timer.tm_mday;
            }else if (timer.tm_mon >=7 && timer.tm_mon <=11){
                year_day=6*31+(timer.tm_mon-7)*30+timer.tm_mday;
            }
            timer.tm_wday=(abs(year_day-3)%7);
            day_menu (timer_ptr);
            break;
        case 'c':
            while (i<5){
                system("cls");
                printf("\n\n\n     -*< CLOCK >*- ");
                line('h',2,2,22,2);
                line('h',2,4,22,4);
                line('h',2,6,22,6);
                line('v',2,3,2,7);
                line('v',8,5,8,6);
                line('v',14,5,14,6);
                line('v',20,3,20,7);
                gotoxy(5,5);
                printf("%0d",time_console()->tm_hour);
                gotoxy(11,5);
                printf("%0d",time_console()->tm_min);
                gotoxy(17,5);
                printf("%0d",time_console()->tm_sec);
                gotoxy(0,0);
                i++;
                Sleep(1000);
            }
            calender_menu ();
            break;
        case 's':
            system("cls");
            printf("enter the task topic : ");
            gets(tmp);
            if( read_file_task_topic(tmp,timer_ptr) == -1){
                system("cls");
                printf("you topic not found ... make sure your typing is correct");
                getch();
                calender_menu ();
            }else
                day_menu (timer_ptr);
            break;
        case 'e':
            system("cls");
            printf("enter the event topic : ");
            gets(tmp);
            if(read_file_event_topic(tmp,timer_ptr) == -1){
                system("cls");
                printf("you topic not found ... make sure your typing is correct");
                getch();
                calender_menu ();
            }else
                day_menu (timer_ptr);
            break;
        case 'q':
            exit(0);
            break;
        default:
            calender_menu ();
            break;
    }  
}
/*****************************************/
void day_menu (struct tm *timer){
    char menu_output1,menu_output2,day_name_sh[15],month_name_sh[15],day_name_mi[15],month_name_mi[15];
    int i,num_read_task,num_read_event;
    system("cls");
    num_read_task=read_file_task(timer);
    num_read_event=read_file_event(timer);
    gotoxy(2,6);
    printf("* Tasks : \n");
    if (num_read_task != -1){
        printf(" Topic : < %s >\n",task.topic);
        printf(" Comment : < %s >\n",task.comment);
        printf(" Is the task done ? : < %c >",task.task_is_done);
    }
    gotoxy(2,30);
    printf("* Events : \n");    
    if(num_read_event != -1){
        printf(" Topic : < %s >\n",event.topic);
        printf(" Comment : < %s >",event.comment);
    }
    line('v',0,1,0,50);
    line('v',100,1,100,50);
    line('h',0,0,102,0);
    line('h',0,5,102,5);
    line('h',0,29,102,29);
    line('h',0,49,102,49);
    printf("\n* Press N to go to the next day\n");
    printf("* Press P to go to the previous day\n");
    printf("* Press E to edit events\n");
    printf("* Press T to edit tasks\n");
    printf("* Press M to show the menu\n");
    printf("* Press I to change 'Is the task done ?'");
    day_name_mon_name_sh(timer,day_name_sh,month_name_sh);
    gotoxy(45,1);
    printf("%d/%d/%d",timer->tm_year,timer->tm_mon,timer->tm_mday);
    gotoxy(88,1);
    printf("%s",day_name_sh);
    gotoxy(2,1);
    printf("%s",month_name_sh);
    gotoxy(45,3);
    sh_to_mi(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
    day_name_mon_name_mi(timer,day_name_mi,month_name_mi);
    printf("%d/%d/%d",timer->tm_year,timer->tm_mon,timer->tm_mday);
    gotoxy(88,3);
    printf("%s",day_name_mi);
    gotoxy(2,3);
    printf("%s",month_name_mi);
    mi_to_sh(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
    menu_output1 = getch();
    switch (menu_output1){
        case 'n':    
            pre_next_day(timer,'n');
            day_name_mon_name_sh(timer,day_name_sh,month_name_sh);
            sh_to_mi(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
            day_name_mon_name_mi(timer,day_name_mi,month_name_mi);
            mi_to_sh(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
            day_menu(timer);
            break;
        case 'p':
            pre_next_day(timer,'p');
            day_name_mon_name_sh(timer,day_name_sh,month_name_sh);
            sh_to_mi(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
            day_name_mon_name_mi(timer,day_name_mi,month_name_mi);
            mi_to_sh(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
            system("cls");
            day_menu(timer);
            break;
        case 'e':
            event_menu(timer);
            break;
        case 'i':
            update_file_task_done(read_file_task(timer),timer);
            day_menu(timer);
            break;
            day_menu(timer);
        case 't':
            task_menu(timer);
            break;
        case 'm':
            calender_menu();
            break;
        default:
            day_menu(timer);
            break;
    }
}
/*****************************************/
void task_menu (struct tm *timer){
    char menu_output,day_name_sh[15],month_name_sh[15],day_name_mi[15],month_name_mi[15];
    int num_read_task=read_file_task(timer);
    int num_read_event=read_file_event(timer);
    system("cls");
    gotoxy(2,7);
    printf("* Topic of the task : \n");
    gotoxy(2,15);
    printf("* More declaration : ");
    gotoxy(2,35);
    printf("* Is the task done (Enter 'Y' for yes and 'N' for no) : ");
    line('v',0,1,0,40);
    line('v',70,1,70,40);
    line('h',0,0,72,0);
    line('h',0,5,72,5);
    line('h',0,14,72,14);
    line('h',0,5,72,5);
    line('h',0,33,72,33);
    line('h',0,39,72,39);
    printf("\n* Press E to create tasks\n");
    printf("* Press T to transport to other date\n");
    printf("* Press D to delete tasks\n");
    printf("* Press U for updating current task\n");
    printf("* Press B to move back to related date\n");
    printf("* Press M to show the menu");
    day_name_mon_name_sh(timer,day_name_sh,month_name_sh);
    gotoxy(30,1);
    printf("%d/%d/%d",timer->tm_year,timer->tm_mon,timer->tm_mday);
    gotoxy(60,1);
    printf("%s",day_name_sh);
    gotoxy(2,1);
    printf("%s",month_name_sh);
    gotoxy(30,3);
    sh_to_mi(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
    day_name_mon_name_mi(timer,day_name_mi,month_name_mi);
    printf("%d/%d/%d",timer->tm_year,timer->tm_mon,timer->tm_mday);
    gotoxy(60,3);
    printf("%s",day_name_mi);
    gotoxy(2,3);
    printf("%s",month_name_mi);
    mi_to_sh(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
    menu_output = getch();
    switch (menu_output){
        case 'e':
            edite_task_menu_write (timer);
            day_menu(timer);
            break;
        case 'd':
            delete_file_task(timer);
            day_menu(timer);
            break;
        case 'u':
            update_file_task(read_file_task(timer),timer);
            day_menu(timer);
            break;
        case 'b':
            day_menu (timer);
            break;
        case 'm':
            calender_menu();
            break;
        case 't':
            update_file_date_task(read_file_task(timer),timer);
            delete_file_task(timer);
            day_menu(timer);
            break;
        default:
            task_menu(timer);
            break;
    }
}
/*****************************************/
int edite_task_menu_write (struct tm*timer){
    FILE *ptr;
    task.flag_del=0;
    task.date.tm_year = timer->tm_year;
    task.date.tm_mon = timer->tm_mon;
    task.date.tm_mday = timer->tm_mday;
    gotoxy(25,7);
    gets(task.topic);
    gotoxy(25,15);
    gets(task.comment);
    gotoxy(60,35);
    task.task_is_done=getchar();
    ptr = fopen("calender_task.dat","ab");
    if ( !ptr )  {
        return 0;
    }    
    fwrite( &task , sizeof(TASK), 1 , ptr );
    fclose(ptr);
    return 1;
}
/****************************************/
int read_file_task(struct tm*timer){
    FILE *wp;
    int i,flag=-1;
    if ((wp=fopen("calender_task.dat","rb")) != NULL){        
        for(i=0;i< comput_number_rec_task();i++) {
            fread(&task,sizeof(TASK),1,wp);
            if (task.date.tm_year == timer->tm_year && task.date.tm_mon == timer->tm_mon && task.date.tm_mday == timer->tm_mday && task.flag_del==0){
                flag=i;
                break;
            }
        }
        fclose(wp);
    }
    return flag;
}
/*****************************************/
int read_file_task_topic(char tmp[20],struct tm*timer){
    FILE *wp;
    int i,flag=-1;
    if ((wp=fopen("calender_task.dat","rb")) != NULL){        
        for(i=0;i< comput_number_rec_task();i++) {
            fread(&task,sizeof(TASK),1,wp);
            if (strcmp(task.topic,tmp) == 0 && task.flag_del==0){
                timer->tm_year=task.date.tm_year;
                timer->tm_yday=task.date.tm_yday;
                timer->tm_wday=task.date.tm_wday;
                timer->tm_mon=task.date.tm_mon;
                timer->tm_mday=task.date.tm_mday;
                flag=i;
                break;
            }
        }
        fclose(wp);
    }
    return flag;
}
/**************************************/
int update_file_date_task(int rec_number,struct tm *timer){
    TASK tmp;
    FILE *ptr;
    system("cls");
    printf("Enter year : ");
    scanf("%d",&tmp.date.tm_year);
    printf("Enter month : ");
    scanf("%d",&tmp.date.tm_mon);
    printf("Enter day of month : ");
    scanf("%d",&tmp.date.tm_mday);
    strcpy(tmp.topic,task.topic);
    strcpy(tmp.comment,task.comment);
    tmp.task_is_done=task.task_is_done;
    ptr = fopen("calender_task.dat","r+b");
    if ( !ptr )  {
        return 0;
    }
    fseek(ptr,(rec_number-1)*sizeof(TASK),0);
    fwrite(&tmp,sizeof(TASK),1,ptr);
    fclose(ptr);
    return(1);
}
/**************************************/
int update_file_task_done(int rec_number,struct tm *timer){
    TASK tmp;
    FILE *ptr;
    tmp.date.tm_year = timer->tm_year;
    tmp.date.tm_mon = timer->tm_mon;
    tmp.date.tm_mday = timer->tm_mday;
    strcpy(tmp.topic,task.topic);
    strcpy(tmp.comment,task.comment);
    tmp.task_is_done='y';
    ptr = fopen("calender_task.dat","r+b");
    if ( !ptr )  {
        return 0;
    }
    fseek(ptr,(rec_number-1)*sizeof(TASK),0);
    fwrite(&tmp,sizeof(TASK),1,ptr);
    fclose(ptr);
    return(1);
}
/**************************************/
int update_file_task(int rec_number,struct tm *timer){
    TASK tmp;
    FILE *ptr;
    tmp.date.tm_year = timer->tm_year;
    tmp.date.tm_mon = timer->tm_mon;
    tmp.date.tm_mday = timer->tm_mday;
    gotoxy(25,7);
    gets(tmp.topic);
    gotoxy(25,15);
    gets(tmp.comment);
    gotoxy(60,35);
    tmp.task_is_done=getchar();
    ptr = fopen("calender_task.dat","r+b");
    if ( !ptr )  {
        return 0;
    }
    fseek(ptr,(rec_number-1)*sizeof(TASK),0);
    fwrite(&tmp,sizeof(TASK),1,ptr);
    fclose(ptr);
    return(1);
}
/*****************************************/
int delete_file_task(struct tm *timer){
    FILE *ptr;
    int i,flag=-1;
    if ((ptr=fopen("calender_task.dat","r+b")) != NULL){        
        for(i=0;i< comput_number_rec_task();i++) {
            fread(&task,sizeof(TASK),1,ptr);
            if (task.date.tm_year == timer->tm_year && task.date.tm_mon == timer->tm_mon && task.date.tm_mday == timer->tm_mday && task.flag_del==0){
                task.flag_del=1;
                flag=i;
                break;
            }
        }
        fseek(ptr,(i)*sizeof(TASK),0);
        fwrite( &task , sizeof(TASK), 1 , ptr );
        fclose(ptr);
    }
}
/***********************************************/
void event_menu (struct tm *timer){
    char menu_output,day_name_sh[15],month_name_sh[15],day_name_mi[15],month_name_mi[15];
    system("cls");
    line('v',0,1,0,40);
    line('v',70,1,70,40);
    line('h',0,0,72,0);
    line('h',0,5,72,5);
    line('h',0,39,72,39);
    printf("\n* Press E to create event\n");
    printf("* Press T to transport to other date\n");
    printf("* Press D to delete event\n");
    printf("* Press U for updating current event\n");
    printf("* Press B to move back to related event\n");
    printf("* Press M to show the menu");
    gotoxy(2,7);
    printf("* Topic of the event : ");
    gotoxy(2,15);
    printf("* More declaration : ");
    gotoxy(25,15);
    day_name_mon_name_sh(timer,day_name_sh,month_name_sh);
    gotoxy(30,1);
    printf("%d/%d/%d",timer->tm_year,timer->tm_mon,timer->tm_mday);
    gotoxy(60,1);
    printf("%s",day_name_sh);
    gotoxy(2,1);
    printf("%s",month_name_sh);
    gotoxy(30,3);
    sh_to_mi(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
    day_name_mon_name_mi(timer,day_name_mi,month_name_mi);
    printf("%d/%d/%d",timer->tm_year,timer->tm_mon,timer->tm_mday);
    gotoxy(60,3);
    printf("%s",day_name_mi);
    gotoxy(2,3);
    printf("%s",month_name_mi);
    mi_to_sh(&timer->tm_year,&timer->tm_mon,&timer->tm_mday);
    menu_output = getch();
    switch (menu_output){
        case 'e':
            edite_event_menu_write(timer);
            day_menu(timer);
            break;
        case 'd':
            delete_file_event(timer);
            day_menu(timer);
            break;
        case 'u':
            update_file_event(read_file_event(timer),timer);
            day_menu(timer);
            break;
        case 'b':
            day_menu(timer);
            break;
        case 'm':
            calender_menu();
            break;
        case 't':
            update_file_date_event(read_file_event(timer),timer);
            delete_file_event(timer);
            day_menu(timer);
            break;
        default:
            event_menu (timer);
            break;
    }
}
/*****************************************/
int edite_event_menu_write (struct tm*timer){
    FILE *ptr;
    event.date.tm_year = timer->tm_year;
    event.date.tm_mon = timer->tm_mon;
    event.date.tm_mday = timer->tm_mday;
    gotoxy(25,7);
    gets(event.topic);
    gotoxy(25,15);
    gets(event.comment);
    ptr = fopen("calender_event.dat","ab");
    if ( !ptr )  {
        return 0;
    }    
    fwrite( &event , sizeof(EVENT), 1 , ptr );
    fclose(ptr);
    return 1;
}
/****************************************/
int read_file_event(struct tm*timer){
    FILE *wp;
    int i,flag=-1;
    if ((wp=fopen("calender_event.dat","rb")) != NULL){        
        for(i=0;i< comput_number_rec_event();i++) {
            fread(&event,sizeof(EVENT),1,wp);
            if (event.date.tm_year == timer->tm_year && event.date.tm_mon == timer->tm_mon && event.date.tm_mday == timer->tm_mday && event.flag_del == 0){
                flag=i;
                break;
            }
        }
        fclose(wp);
    }
    return flag;
}
/*******************************************/
int read_file_event_topic(char tmp[20],struct tm*timer){
    FILE *wp;
    int i,flag=-1;
    if ((wp=fopen("calender_event.dat","rb")) != NULL){        
        for(i=0;i< comput_number_rec_event();i++) {
            fread(&event,sizeof(EVENT),1,wp);
            if (strcmp(event.topic,tmp) == 0 && event.flag_del==0){
                timer->tm_year=event.date.tm_year;
                timer->tm_yday=event.date.tm_yday;
                timer->tm_wday=event.date.tm_wday;
                timer->tm_mon=event.date.tm_mon;
                timer->tm_mday=event.date.tm_mday;
                flag=i;
                break;
            }
        }
        fclose(wp);
    }
    return flag;
}
/*******************************************/
int update_file_date_event(int rec_number,struct tm *timer){
    TASK tmp;
    FILE *ptr;
    system("cls");
    printf("Enter year : ");
    scanf("%d",&tmp.date.tm_year);
    printf("Enter month : ");
    scanf("%d",&tmp.date.tm_mon);
    printf("Enter day of month : ");
    scanf("%d",&tmp.date.tm_mday);
    strcpy(tmp.topic,task.topic);
    strcpy(tmp.comment,task.comment);
    ptr = fopen("calender_event.dat","r+b");
    if ( !ptr )  {
        return 0;
    }
    fseek(ptr,(rec_number-1)*sizeof(TASK),0);
    fwrite(&tmp,sizeof(TASK),1,ptr);
    fclose(ptr);
    return(1);
}
/**************************************/
int update_file_event(int rec_number,struct tm *timer)
{
    EVENT tmp;
    FILE *ptr;
    tmp.date.tm_year = timer->tm_year;
    tmp.date.tm_mon = timer->tm_mon;
    tmp.date.tm_mday = timer->tm_mday;
    gotoxy(25,7);
    gets(tmp.topic);
    gotoxy(25,15);
    gets(tmp.comment);
    ptr = fopen("calender_event.dat","r+b");
    if ( !ptr )  {
        return 0;
    }
    fseek(ptr,(rec_number-1)*sizeof(EVENT),0);
    fwrite(&tmp,sizeof(EVENT),1,ptr);
    fclose(ptr);
    return(1);
}
/*****************************************/
int delete_file_event(struct tm *timer){
    FILE *ptr;
    int i,flag=-1;
    if ((ptr=fopen("calender_event.dat","r+b")) != NULL){        
        for(i=0;i< comput_number_rec_task();i++) {
            fread(&event,sizeof(TASK),1,ptr);
            if (event.date.tm_year == timer->tm_year && event.date.tm_mon == timer->tm_mon && event.date.tm_mday == timer->tm_mday && event.flag_del==0){
                event.flag_del=1;
                flag=i;
                break;
            }
        }
        fseek(ptr,(i-1)*sizeof(EVENT),0);
        fwrite( &event , sizeof(EVENT), 1 , ptr );
        fclose(ptr);
    }
}
/*******************************************/
struct tm * time_console(void){    
    time_t timer=time(NULL);
    struct tm *timer_block;
    timer_block=localtime(&timer);
    timer_block->tm_year += 1900;
    timer_block->tm_mon++;
    return timer_block;
}
/*****************************************/
void day_name_mon_name_sh(struct tm*timer,char day_name[15],char month_name[15]){
    switch (timer->tm_wday){
        case 0:
            strcpy(day_name,"Shanbe");
            break;
        case 1:
            strcpy(day_name,"Yek Shanbe");
            break;
        case 2:
            strcpy(day_name,"Do Shanbe");
            break;
        case 3:
            strcpy(day_name,"Se Shanbe");
            break;
        case 4:
            strcpy(day_name,"Char Shanbe");
            break;
        case 5:
            strcpy(day_name,"Panj Shanbe");
            break;
        case 6:
            strcpy(day_name,"Jomee");
            break;
    }
    switch (timer->tm_mon){
        case 1:
            strcpy(month_name,"Farvardin");
            break;
        case 2:
            strcpy(month_name,"Ordibehesht");
            break;
        case 3:
            strcpy(month_name,"Khordad");
            break;
        case 4:
            strcpy(month_name,"Tir");
            break;
        case 5:
            strcpy(month_name,"Mordad");
            break;
        case 6:
            strcpy(month_name,"Shahrivar");
            break;
        case 7:
            strcpy(month_name,"Mehr");
            break;
        case 8:
            strcpy(month_name,"Aban");
            break;
        case 9:
            strcpy(month_name,"Azar");
            break;
        case 10:
            strcpy(month_name,"Dey");
            break;
        case 11:
            strcpy(month_name,"Bahman");
            break;
        case 12:
            strcpy(month_name,"Esfand");
            break;
    }
}
/***********************************************/
void day_name_mon_name_mi(struct tm*timer,char day_name[15],char month_name[15]){
    switch (timer->tm_wday){
        case 0:
            strcpy(day_name,"Saturday");
            break;
        case 1:
            strcpy(day_name,"Sunday");
            break;
        case 2:
            strcpy(day_name,"Monday");
            break;
        case 3:
            strcpy(day_name,"Teusday");
            break;
        case 4:
            strcpy(day_name,"Wednesday");
            break;
        case 5:
            strcpy(day_name,"Tursday");
            break;
        case 6:
            strcpy(day_name,"Friday");
            break;
    }
    switch (timer->tm_mon){
        case 1:
            strcpy(month_name,"January");
            break;
        case 2:
            strcpy(month_name,"February");
            break;
        case 3:
            strcpy(month_name,"March");
            break;
        case 4:
            strcpy(month_name,"April");
            break;
        case 5:
            strcpy(month_name,"May");
            break;
        case 6:
            strcpy(month_name,"June");
            break;
        case 7:
            strcpy(month_name,"July");
            break;
        case 8:
            strcpy(month_name,"August");
            break;
        case 9:
            strcpy(month_name,"September");
            break;
        case 10:
            strcpy(month_name,"October");
            break;
        case 11:
            strcpy(month_name,"November");
            break;
        case 12:
            strcpy(month_name,"December");
            break;
    }
}
/***********************************************/
void pre_next_day(struct tm*timer,char p_or_n){
    if(p_or_n == 'n'){
        timer->tm_yday++;
        timer->tm_mday++;
        if (timer->tm_mon >= 1 && timer->tm_mon <= 6){
            if (timer->tm_mday > 31){
                timer->tm_mday = 1;
                timer->tm_mon ++;
            }
        }if (timer->tm_mon>=7 && timer->tm_mon<=11){
            if (timer->tm_mday > 30){
                timer->tm_mday = 1;
                timer->tm_mon ++;
            }
        }if (timer->tm_mon == 12){
            if (timer->tm_year%4 == 1391%4 ){
                if (timer->tm_mday > 30){
                    timer->tm_yday = 1;
                    timer->tm_mday = 1;
                    timer->tm_mon = 1;
                    timer->tm_year ++;
                }
            }else{
                if (timer->tm_mday > 29){
                    timer->tm_yday = 1;
                    timer->tm_mday = 1;
                    timer->tm_mon = 1;
                    timer->tm_year ++;
                }
            }
        }   
        timer->tm_wday++;
        if (timer->tm_wday >= 7)
            timer->tm_wday=0;
    }if (p_or_n == 'p'){
        timer->tm_wday--;
            if (timer->tm_wday < 0)
                timer->tm_wday=6;
        timer->tm_yday--;
        timer->tm_mday--;
        if (timer->tm_mon >= 2 && timer->tm_mon <= 6){
            if (timer->tm_mday < 1){
                timer->tm_mday = 31;
                timer->tm_mon --;
            }
        }if (timer->tm_mon>=7 && timer->tm_mon<=12){
            if (timer->tm_mday < 1){
                timer->tm_mday = 30;
                timer->tm_mon --;
            }
        }if (timer->tm_mon == 1){
            if (timer->tm_year%4 == 1391%4 ){
                if (timer->tm_mday < 1){
                    timer->tm_yday = 366;
                    timer->tm_mday = 30;
                    timer->tm_mon = 12;
                    timer->tm_year --;
                }
            }else{
                if (timer->tm_mday < 1){
                    timer->tm_yday = 365;
                    timer->tm_mday = 29;
                    timer->tm_mon = 12;
                    timer->tm_year --;
                }
            }    
        }
    }
}
/***********************************************/
void mi_to_sh(int *year,int *month,int *day){                                                               
    int mi_table[6][12]={
        {20,19,19,19,20,20,21,21,21,21,20,20},
        {10,11,10,12,11,11,10,10,10,9,10,10},
        {19,18,20,20,21,21,22,22,22,22,21,21},
        {11,12,10,11,10,10,9,9,9,8,9,9},
        {20,19,20,20,21,21,22,22,22,22,21,21},
        {10,11,9,11,10,10,9,9,9,8,9,9}};
    int k,t1,t2;
    k=*year%4;
    if(k==3)
        k=2;
    k*=2;
    t1=mi_table[k][*month-1];
    t2=mi_table[k+1][*month-1];
    if(*month<3 || (*month==3 && *day<=mi_table[k][*month-1]))
        *year=*year+78;
    else               
        *year=*year+79;
    if (*day<=t1){
        *day=*day+t2;
        *month=(*month+8)%12+1;
    }else{ 
        *day=*day-t1;
        *month=(*month+9)%12+1; 
    }
    *year-=700;
    return ;      
}
/*****************************************/
void sh_to_mi(int *year,int *month,int *day){  
    int k,t1,t2;
    int sh_table[6][12]={
        {11,10,10,9,9,9,8,9,9,10,11,9},
        {20,20,21,21,22,22,22,22,21,21,20,19},
        {11,10,10,9,9,9,8,9,9,10,11,10},
        {20,20,21,21,22,22,22,22,21,21,20,19},
        {12,11,11,10,10,10,9,10,10,11,12,10},
        {19,19,20,20,21,21,21,21,20,20,19,18}};
    k=*year%4;
    if(k==0)
        k=2;
    else
        k=k+k;
    t1=sh_table[k-2][*month-1];    
    t2=sh_table[k-1][*month-1];
    if(*month<10 || (*month==10 && *day<=sh_table[k-2][*month-1]))
        *year=*year-79;
    else
        *year=*year-78;
    if(*day<=t1){
        *day=*day+t2;
        *month=(*month+1)%12+1;
    }else{
        *day=*day-t1;
        *month=(*month+2)%12+1;
    }
    *year+=700;
    return ;           
}
/*****************************************/
void gotoxy(int x,int y){
    COORD pos;
    HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );
    if (INVALID_HANDLE_VALUE != hConsole){
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition( hConsole, pos );
    }
}
/*****************************************/
void line(char type, int start_x, int start_y, int end_x, int end_y){
    gotoxy( start_x ,start_y );
    if (type == 'h'){   /*"h" is horizontal*/ 
        for (int i = 0; i < end_x - start_x ; i++){
            printf("-");
            gotoxy( start_x + i , start_y );
        }
    }else if (type == 'v'){ /*"v" is vertical*/ 
        for (int i = 0; i < end_y - start_y ; i++){
            printf("|");
            gotoxy( start_x , start_y + i );
        }
    } 
}
/*****************************************/
int comput_number_rec_task(void){
    int size;
    FILE *ptr=fopen("calender_task.dat","rb");
    fseek(ptr, 0, SEEK_END);
    size = ftell(ptr);
    return size/sizeof(TASK);
}
/*****************************************/
int comput_number_rec_event(void){
    int size;
    FILE *ptr=fopen("calender_event.dat","rb");
    fseek(ptr, 0, SEEK_END);
    size = ftell(ptr);
    return size/sizeof(EVENT);
}
