#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
char Game[] = "Playing Game";

sem_t stud, tech, staff, game;

int timeStart, timeEnd;
int studentCount = 0;
void *student(void *n)
{
    int studentN = (int)n;
       sem_wait(&game);
    sem_wait(&stud);
       int flag = 0;
    if ((timeStart >= 6) && (timeEnd <= 22))
    {
        studentCount++;
        if (studentCount == 1)
        {
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
        sem_post(&tech);
        sem_post(&staff);
    }
   
    sem_post(&stud);
    sem_post(&game);
    pthread_exit(NULL);
}



int teacherCount = 0;
void *teacher(void *n)
{
    
    int teacherN = (int)n;
   
    sem_wait(&game);
    sem_wait(&tech);
    int flag = 0;
    if ((timeStart >= 8) && (timeEnd <= 17))
    {
        teacherCount++;
        if (teacherCount == 1)
        {
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
        printf("Teacher %d %s\n", teacherN, Game);
        sleep(2);
        printf("Teacher %d Game Ended\n", teacherN);
    }
    teacherCount--;
    if (teacherCount == 0)
    {
        sem_post(&stud);
        sem_post(&staff);
    }
    sem_post(&tech);
    sem_post(&game);
    pthread_exit(NULL);
}



int techStaffCount = 0;
void *tech_staff(void *n)
{
   
    int techStaffN = (int)n;
    
    sem_wait(&game);
    sem_wait(&staff);
    
    int flag = 0;
    techStaffCount++;
    if (techStaffCount == 1)
    {
        
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
        
        printf("TechStaff %d %s\n", techStaffN, Game);
        sleep(2);
        printf("TechStaff %d Game Ended\n", techStaffN);
    }

    techStaffCount--;
    if (techStaffCount == 0)
    {
       
        sem_post(&stud);
        sem_post(&tech);
    }
    
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

   
    printf("Enter No. of Students: ");
    scanf("%d", &no_of_students);
    printf("Enter No. of Teachers: ");
    scanf("%d", &no_of_teachers);
    printf("Enter No. of TechStaff: ");
    scanf("%d", &no_of_techStaff);

 
    printf("Enter Start Time: ");
    scanf("%d", &timeStart);
    printf("Enter End Time: ");
    scanf("%d", &timeEnd);

    
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
