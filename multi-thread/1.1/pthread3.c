/*
demo: 线程分离，一般配合pthread_exit使用；
一般多线程工作，需要满足1. 子线程执行完 2. 子线程资源被回收
两种方法 主线程里操作
1. pthread_join 阻塞满足执行完子线程，回收子线程资源
2. pthread_detach 分离，其他线程回收子线程资源 + pthread_exit() 线程退出，子线程不受主线程影响可以执行完；
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct Test {
    int num;
    int age;
};

void* callback(void* arg) {

    struct Test* t = (struct Test*) arg;
    t->num = 100;
    t->age = 6;

    printf("子线程：%ld\n", pthread_self());
    pthread_exit(t);
    
    return NULL;
}


int main() {
    pthread_t tid;
    
    struct Test t;
    pthread_create(&tid, NULL, callback, &t);

    printf("主线程：%ld\n", pthread_self());

    // 分离 达到回收的效果
    pthread_detach(tid);
    // 退出 子线程不受主线程影响，子线程可以执行完
    pthread_exit(NULL);
    return 0;
}