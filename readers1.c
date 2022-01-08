#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void *writer(void *wno)
{
    int d;
    sem_wait(&wrt);
    printf("\n\t\t\t\tEditor %d is trying to edit \n",(*((int *)wno)));

    printf("\n\t\t\t\tshared resource holds a counter whose current value is %d\n\t\t\t\tEnter new counter value : ",cnt);
    scanf("%d",&d);
    cnt = d;
    printf("\n\t\t\t\t=> Editor %d edited data to %d\n\n\t\t\t------------------------------------------------------------------------\n",(*((int *)wno)),cnt);
    sem_post(&wrt);

}
void *reader(void *rno)
{
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); // If this id the first reader, then it will block the writer
    }
    pthread_mutex_unlock(&mutex);
    // Reading Section
    printf("\n\t\t\t\tViewer %d is reading...\n\t\t\t\tCurrent counter value is : %d\n\n\t\t\t------------------------------------------------------------------------\n",*((int *)rno),cnt);

    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{

    pthread_t read[5],write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[5] = {1,2,3,4,5},x=0,y=0; //Just used for numbering the producer and consumer
    char b[10] = {'r','w','w','r','w','r','w','r','w','r'};

    for(int i = 0; i < 10; i++)
    {
        if (b[i] == 'r')
        {
            pthread_create(&read[x], NULL, (void *)reader, (void *)&a[x]);
            x++;
        }
        else if(b[i]=='w')
        {
            pthread_create(&write[y], NULL, (void *)writer, (void *)&a[y]);
            y++;
        }
    }


    for(int i = 0; i < 5; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    printf("\n\n\t\t\t\tAll viewers and editors are done reading and editing !!!\n\n\t\t\t\tFinal shared resource counter value is : %d\n\n\n\n",cnt);
    return 0;

}
