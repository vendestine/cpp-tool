/*
demo: 线程回收函数，以及回调函数里的参数怎么传递（利用pthread_create, pthread_exit, pthread_join);
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
    
    void* ptr;
    pthread_join(tid, &ptr);
    struct Test* p = (struct Test*) ptr;
    printf("num: %d, age: %d\n", p->num, p->age);
    
    return 0;
}