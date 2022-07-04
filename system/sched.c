#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>

//获取线程优先级
int get_thread_policy(pthread_attr_t *attr)
{
    int policy;
    int ret = pthread_attr_getschedpolicy(attr, &policy);
    if (ret)
    {
        perror(strerror(errno));
    }

    switch (policy)
    {
    case SCHED_OTHER:
        printf("policy:SCHED_OTHER\n");
        break;
    case SCHED_FIFO:
        printf("policy:SCHED_FIFO\n");
        break;
    case SCHED_RR:
        printf("policy:SCHED_RR\n");
        break;
    default:
        printf("policy:UNKNOWN\n");
        break;
    }
    return policy;
}

void set_thread_policy(pthread_attr_t *attr, int policy)
{
    int ret = pthread_attr_setschedpolicy(attr, policy);
    if (ret)
    {
        perror(strerror(errno));
    }
}

//显示策略的优先级
void show_thread_priority(int policy)
{
    int priority = sched_get_priority_max(policy);
    if (priority == -1)
    {
        perror(strerror(errno));
    }
    printf("priority_max:%d\n", priority);
    priority = sched_get_priority_min(policy);
    if (priority == -1)
    {
        perror(strerror(errno));
    }
    printf("priority_min:%d\n", priority);
}

int get_thread_priority(pthread_attr_t *attr)
{
    struct sched_param param;
    int ret = pthread_attr_getschedparam(attr, &param);
    if (ret)
    {
        perror(strerror(errno));
    }
    printf("priority=%d", param.__sched_priority);
    return param.__sched_priority;
}

int main(int argc, char **argv)
{
    //线程属性
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if (ret)
    {
        perror(strerror(errno));
    }

    int policy = get_thread_policy(&attr);
    printf("Show current configuration of priority\n");
    show_thread_priority(policy);
    printf("show SCHED_FIFO of priority\n");
    show_thread_priority(SCHED_FIFO);
    printf("show SCHED_RR of priority\n");
    show_thread_priority(SCHED_RR);
    printf("show priority of current thread\n");
    int priority = get_thread_priority(&attr);

    printf("Set thread policy\n");
    printf("set SCHED_FIFO policy\n");
    set_thread_policy(&attr, SCHED_FIFO);
    printf("set SCHED_RR policy\n");
    set_thread_policy(&attr, SCHED_RR);
    printf("Restore current policy\n");
    set_thread_policy(&attr, policy);

    ret = pthread_attr_destroy(&attr);

    return 0;
}
