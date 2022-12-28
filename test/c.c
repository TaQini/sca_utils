#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "errno.h"
#include "unistd.h"


// 定义线程数目 为16；
#define PTHREAD_NUM 16

unsigned long sum = 0;

void *thread(void *arg){
    for (int i = 0;  i < 10000; i++)
        sum += 1;  // 注意，此处没有使用原子操作
}


int main(void){

    printf("before ..sum = %lu \n", sum);

    pthread_t  pthread[PTHREAD_NUM]; // 被创建线程的标识
    int ret;        // 接受返回值
    void *retval[PTHREAD_NUM];

    for(int i = 0; i < PTHREAD_NUM; i++){

        ret = pthread_create(&pthread[i], NULL, thread, NULL);
        if(ret != 0){
            perror("cause:");
            printf("cause pthread %d failed. \n", i+1);
        }
    }

    for( int i = 0; i < PTHREAD_NUM; i++)
        pthread_join(pthread[i], &retval[i]);

    printf("after.... sum = %lu \n", sum);

    return 0;

}
