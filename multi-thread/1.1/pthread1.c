/*
demo：线程创建 退出；
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// 回调函数 也叫任务函数
void* callback(void* arg) {
    for (int i = 0; i < 5; i ++) {
        printf("子线程：i = %d\n", i);
    }

    printf("子线程：%ld\n", pthread_self());
    return NULL;
}


int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);
    for (int i = 0; i < 5; i ++) {
        printf("主线程：i = %d\n", i);
    }
    printf("主线程：%ld\n", pthread_self());
    // 有可能主线程结束，子线程还没执行完，导致子线程未执行或者结果错误；
    // 解决方案 1. 让主线程阻塞，让子线程执行完； 2. 主线程使用退出而不是结束，这样主线程不影响子线程运行；
    
    // solution 1
    // sleep(1);
    // pthread_join(tid, NULL);

    //solution 2
    pthread_exit(NULL);

    return 0;
}