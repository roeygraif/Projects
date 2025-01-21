#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX_STUDENTS 10
#define MAX_NAME 100
#define INFO 2
#define INIT 100
#define ADD 200
#define DEL 300
#define SUCCESS 0
#define FAILURE -1

void init_db(int students_info[][INFO], char* students_names[], int n);
int read_students_info(int students_info[][INFO], char* students_names[], int n);
void add_student(int students_info[][INFO], char* students_names[], int n);
void delete_student(int students_info[][INFO], char* students_names[], int n);
void printall(int students_info[][INFO], char* students_names[], int n);
void clean_db(char* students_names[], int n);


/* Additional functions of your choice */


/* main Function */
int main()
{
    int students_info[MAX_STUDENTS][INFO];
    char* students_names[MAX_STUDENTS];
    init_db(students_info, students_names, MAX_STUDENTS);
    int op = INIT;
    while (scanf("%d", &op) != EOF)

    {

        switch (op) {
        case INIT: if (read_students_info(students_info, students_names, MAX_STUDENTS) != SUCCESS) { return FAILURE; } break;
        case ADD: add_student(students_info, students_names, MAX_STUDENTS); break;
        case DEL: delete_student(students_info, students_names, MAX_STUDENTS); break;
        default: break;
        }
    }
    clean_db(students_names, MAX_STUDENTS);

    return 0;
}
void init_db(int students_info[][INFO], char* students_names[], int n) {//start database
    for (int i = 0; i < n; i++)
    {
        students_info[i][0]=-1;
        students_info[i][1]=-1;
        students_names[i] = "-1";
    }
}
int read_students_info(int students_info[][INFO], char* students_names[], int n) {
    int id = 0 , time = 0,sizeName=0;
    char name[MAX_NAME + 1];
    int M = 0;
    scanf("%d",&M);
    if (M > n) {
        printf("Too many students!! Aborting!!");
        return -1;
    }
    for (int i = 0; i < M; i++)             //read students M times
    {
        scanf("%s", name);
        scanf("%d", &id);
        scanf("%d", &time);
        sizeName = strlen(name)+1;
        //printf("%d",sizeName);
        students_names[i] = malloc(sizeName*sizeof(char));
        if (students_names[i] == NULL)
            exit(0);
        strcpy(students_names[i], name);
        students_info[i][0] = id;
        students_info[i][1] = time;
    }
    printf("First %d students were added to the database!!\n", M);
   // printall(students_info, students_names, MAX_NAME);
    return 0;
}
void add_student(int students_info[][INFO], char* students_names[], int n) {    //adding one student
   // printf("add fucking shit\n");
    bool idIsFree=true,found=false;
    char name[MAX_NAME + 1];
    int id = 0, time = 0, sizeName = 0, freeplace = -1;
    scanf("%s", name);
    scanf("%d", &id);
    scanf("%d", &time);
    for (int i = 0; i < n; i++)
    {
        if (!found && students_info[i][0] == -1) {               //making sure there is free place
            freeplace = i;
            found = true;
        }
        if (students_info[i][0] == id) {                          //making sure id is not taken
            idIsFree = false;
            printf("Student %d already exists!!\n", id);
        }
    }
    if (idIsFree && freeplace != -1) {                  //id is -1
        sizeName = strlen(name) + 1;
       // printf("\nprint name %s\n",name);
        students_names[freeplace] = malloc(sizeName * sizeof(char));
        if (students_names[freeplace] == NULL)
            exit(0);
        strcpy(students_names[freeplace], name);
        students_info[freeplace][0] = id;
        students_info[freeplace][1] = time;
        printf("Student %d was successfully added to the system!!\n", students_info[freeplace][0]);
        /*printf("%d\n", i);*/
    }
    if (freeplace==-1)
        printf("Cannot add student %d, class already full!!\n",id);
  //  printf("adds    %s\n", students_names[0]);
  //  printf("adds    %s\n", students_names[1]);
   // printall(students_info, students_names, MAX_NAME);
}
void delete_student(int students_info[][INFO], char* students_names[], int n) {
    int idDelete = 0;
    bool found = false;
    scanf("%d",&idDelete);
    for (int i = 0; i < n; i++)
    {
        if (students_info[i][0] == idDelete) {
            students_info[i][0] = -1;
            students_info[i][1] = -1;
            free(students_names[i]);                           //free the user name data space
            students_names[i] = "-1";
            found = true;
            printf("Student %d information were deleted from the system!!!\n",idDelete);
            break;
        }
    }
    if (!found)         
        printf("Student %d does not exist!!!\n",idDelete);
   // printall(students_info, students_names, MAX_NAME);
}
void clean_db(char* students_names[], int n) {
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(students_names[i], "-1")) {        //free student database space
            free(students_names[i]);
            count++;
        }
    }
    printf("All %d entries were deleted!!!", count);
}
void printall(int students_info[][INFO], char* students_names[], int n) {   //testing only fuction
    for (int i = 0; i < 3; i++)
    {
        printf("%s    ", students_names[i]);
        printf("%d    ",students_info[i][0]);
        printf("%d\n", students_info[i][1]);
    }
    printf("%d\n",n);
    
}