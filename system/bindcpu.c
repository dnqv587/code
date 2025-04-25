#include <stdio.h>
#define __USE_GNU
#include <pthread.h>
//#include <sched.h>

int main(){
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);        // 清空集合
    CPU_SET(2, &cpuset);  // 添加目标核心到集合
    const int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if(rc != 0){
        perror("set cpuid error!\n");
    }
    printf("set cpuid is 2\n");
    
    printf("cpu is %d\n",sched_getcpu());
    
    return 0;
}
