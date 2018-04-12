/*Consider a computer system in which computer games can be played
by students only between 10 P.M. and 6 A.M. , by faculty members
between 5 P.M. and 8 A.M. , and by the computer center staff at all
times. Suggest a scheme for implementing this policy efficiently.*/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

// Shared Game Variable

char Game[] = "Playing Game";

// Student, Teacher, TechStaff and Game Semaphores respectively

sem_t stud, tech, staff, game;

// Current Time (Entered Manually)

int timeStart, timeEnd;

// Student Function

int studentCount = 0;
void *student(void *n)
{
    // Current Student No.
    int studentN = (int)n;
    // Locking Game and Student Semaphore
    sem_wait(&game);
    sem_wait(&stud);
    // Flag Variable to determine whether 1st student thread have locked both teacher and techstaff
    int flag = 0;
    if ((timeStart >= 6) && (timeEnd <= 22))
    {
        studentCount++;
        if (studentCount == 1)
        {
            // Locking Teacher and TechStaff threads
            sem_wait(&tech);
            sem_wait(&staff);
            flag = 1;
        }
        else
        {
            flag = 1;
        }
    }
    printf("Student %d Process Started\n", studentN);
    if (flag == 1)
    {
        //Critical Section Accessing Game
        printf("Student %d %s\n", studentN, Game);
        sleep(2);
        printf("Student %d Game Ended\n", studentN);
    }

    studentCount--;
    if (studentCount == 0)
    {
        // Releasing Locks on Teacher and TechStaff when Students done Playing
        sem_post(&tech);
        sem_post(&staff);
    }
    // Releasing Lock on Student and Game
    sem_post(&stud);
    sem_post(&game);
    pthread_exit(NULL);
}

// Teacher Function 

int teacherCount = 0;
void *teacher(void *n)
{
    // Current Teacher No.
    int teacherN = (int)n;
    // Locking Game and Teacher Semaphore
    sem_wait(&game);
    sem_wait(&tech);
    // Flag Variable to determine whether 1st teacher thread have locked both student and techstaff
    int flag = 0;
    if ((timeStart >= 8) && (timeEnd <= 17))
    {
        teacherCount++;
        if (teacherCount == 1)
        {
            // Locking Student and TechStaff threads
            sem_wait(&stud);
            sem_wait(&staff);
            flag = 1;
        }
        else
        {
            flag = 1;
        }
    }
    printf("Teacher %d Process Started\n", teacherN);
    if (flag == 1)
    {
        // Critical Section Accessing Game
        printf("Teacher %d %s\n", teacherN, Game);
        sleep(2);
        printf("Teacher %d Game Ended\n", teacherN);
    }
    teacherCount--;
    if (teacherCount == 0)
    {
        // Releasing Locks on Student and TechStaff
        sem_post(&stud);
        sem_post(&staff);
    }
    // Releasing Locks on Teacher and Game
    sem_post(&tech);
    sem_post(&game);
    pthread_exit(NULL);
}

// TechStaff Function

int techStaffCount = 0;
void *tech_staff(void *n)
{
    // Current TechStaff No.
    int techStaffN = (int)n;
    // Locking Game and TechStaff Semaphore
    sem_wait(&game);
    sem_wait(&staff);
    // Flag Variable to determine whether 1st TechStaff thread have locked both student and teacher
    int flag = 0;
    techStaffCount++;
    if (techStaffCount == 1)
    {
        // Locking Student and Teacher threads
        sem_wait(&stud);
        sem_wait(&tech);
        flag = 1;
    }
    else
    {

        flag = 1;
    }

    printf("TechStaff %d Process Started\n", techStaffN);
    if (flag == 1)
    {
        // Critical Section Accessing Game
        printf("TechStaff %d %s\n", techStaffN, Game);
        sleep(2);
        printf("TechStaff %d Game Ended\n", techStaffN);
    }

    techStaffCount--;
    if (techStaffCount == 0)
    {
        // Releasing Locks on Teacher and Student
        sem_post(&stud);
        sem_post(&tech);
    }
    // Releasing Locks on TechStaff and Game
    sem_post(&staff);
    sem_post(&game);
    pthread_exit(NULL);
}

int main()
{
    pthread_t st[100], th[100], sf[100];
    int no_of_students, no_of_teachers, no_of_techStaff, i;
    sem_init(&stud, 0, 1);
    sem_init(&tech, 0, 1);
    sem_init(&staff, 0, 1);
    sem_init(&game, 0, 1);

    // Entering No. of Student, Teacher and TechStaff threads respectively

    printf("Enter No. of Students: ");
    scanf("%d", &no_of_students);
    printf("Enter No. of Teachers: ");
    scanf("%d", &no_of_teachers);
    printf("Enter No. of TechStaff: ");
    scanf("%d", &no_of_techStaff);

    // Entering Time
    printf("Enter Start Time: ");
    scanf("%d", &timeStart);
    printf("Enter End Time: ");
    scanf("%d", &timeEnd);

    // Threads Creation
    for (i = 0; i < no_of_students; i++)
    {
        pthread_create(&st[i], NULL, student, (void *)i + 1);
    }

    for (i = 0; i < no_of_teachers; i++)
    {
        pthread_create(&th[i], NULL, teacher, (void *)i + 1);
    }

    for (i = 0; i < no_of_techStaff; i++)
    {
        pthread_create(&sf[i], NULL, tech_staff, (void *)i + 1);
    }

    // Threads Joining
    
    for (i = 0; i < no_of_students; i++)
    {
        pthread_join(st[i], NULL);
    }

    for (i = 0; i < no_of_teachers; i++)
    {
        pthread_join(th[i], NULL);
    }

    for (i = 0; i < no_of_techStaff; i++)
    {
        pthread_join(sf[i], NULL);
    }
}
