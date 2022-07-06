#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t print_mutex;
pthread_cond_t print_cond;
pthread_mutex_t thread_mutex;
pthread_cond_t thread_cond;
int micro = 0;

void *printFunc(void *arg)
{
    int *micro = (int *)arg;

    while (1)
    {
        pthread_mutex_lock(&print_mutex);
        pthread_cond_wait(&print_cond, &print_mutex);
        printf("Sec/ten:%d\n", *micro);
        pthread_mutex_unlock(&print_mutex);
    }
    return NULL;
}

void *threadFunc(void *arg)
{
    bool *isSleep = (bool *)arg;

    pthread_t print;
    micro = 0;
    pthread_create(&print, NULL, printFunc, &micro);
    pthread_detach(print);
    pthread_cond_wait(&thread_cond, &thread_mutex);
    while (1)
    {
        if (!*isSleep)
        {

            pthread_cond_wait(&thread_cond, &thread_mutex);
        }
        usleep(1000 * 100);
        __sync_fetch_and_add(&micro, 1);
        pthread_cond_signal(&print_cond);
    }
    return NULL;
}

int main(int argc, char *argv)
{
    pthread_mutex_init(&thread_mutex, NULL);
    pthread_cond_init(&thread_cond, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    pthread_cond_init(&print_cond, NULL);

    pthread_t thread;
    bool isSleep = false;
    pthread_create(&thread, NULL, threadFunc, &isSleep);
    pthread_detach(thread);
    sleep(1);

    //秒
    printf("sleep\n");
    isSleep = true;
    micro = 0;
    pthread_cond_signal(&thread_cond);
    sleep(5);
    isSleep = false;

    sleep(1);
    //微秒
    printf("usleep\n");
    isSleep = true;
    micro = 0;
    pthread_cond_signal(&thread_cond);
    usleep(1000 * 900);
    isSleep = false;

    sleep(1);
    return 0;
}
