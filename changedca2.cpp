#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_STUDENTS 5
void *student(void *num);
void *ta(void *);
void randwait(int secs);
sem_t waitingRoom;
sem_t Chair;
sem_t teacherPillow;
sem_t sit;
int allDone = 0;
int main(int argc, char *argv[]) {
pthread_t taid;
pthread_t stid[MAX_STUDENTS];
long Rand;
int i, numStudents, numChairs;
int Number[MAX_STUDENTS];
printf("Enter the number of Students : ");
scanf("%d",&numStudents) ;
printf("Enter the number of Chairs : ");
scanf("%d",&numChairs);
if (numStudents > MAX_STUDENTS) {
printf("The maximum number of Students is %d.\n", MAX_STUDENTS);
exit(-1);
}
for (i=0; i<MAX_STUDENTS; i++) {
Number[i] = i;
}
sem_init(&waitingRoom, 0, numChairs);
sem_init(&Chair, 0, 1);
sem_init(&teacherPillow, 0, 0);
sem_init(&sit, 0, 0);
pthread_create(&taid, NULL, ta, NULL);
for (i=0; i<numStudents; i++) {
pthread_create(&stid[i], NULL, student, (void *)&Number[i]);
sleep(1);
}
for (i=0; i<numStudents; i++) {
pthread_join(stid[i],NULL);
sleep(1);
}
allDone = 1;
sem_post(&teacherPillow);
pthread_join(taid,NULL);
}
void *student(void *number) {
int num = *(int *)number;
printf("Student %d leaving for TA desk.\n", num);
randwait(2);
printf("Student %d arrived at TA desk.\n", num);
sem_wait(&waitingRoom);
printf("Student %d entering waiting room.\n", num);
sem_wait(&Chair);
sem_post(&waitingRoom);
printf("Student %d waking the Teaching Assistant.\n", num);
sem_post(&teacherPillow);
sem_wait(&sit);
sem_post(&Chair);
printf("Student %d leaving the desk.\n", num);
}
void *ta(void *junk) {
while (!allDone) {
printf("The Teaching assistant is sleeping\n");
sem_wait(&teacherPillow);
if (!allDone) {
printf("The Teaching Assistant is teaching\n");
randwait(2);
printf("The Teaching Assistant has finished teaching.\n");
sem_post(&sit);
}
/*else {
printf("The Teaching Assistant is going home.\n");
}*/
}
}
void randwait(int secs) {
int len;
len = (int) ((1 * secs) + 1);
sleep(len);
}
