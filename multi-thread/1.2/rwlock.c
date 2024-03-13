/*
利用读写锁做线程同步；
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX 50
int number = 0;

pthread_rwlock_t rwlock;

void * read_num(void* arg) {
    for (int i = 0; i < MAX; i ++) {
        pthread_rwlock_rdlock(&rwlock);
        printf("Thread read, id = %lu, number = %d\n", pthread_self(), number);
        pthread_rwlock_unlock(&rwlock);
        // sleep，让出时间片，多个线程交替工作
        usleep(rand() % 5);
    }
    return NULL;
}

void * write_num(void* arg) {
    for (int i = 0; i < MAX; i ++) {
        pthread_rwlock_wrlock(&rwlock);
        int cur = number;
        cur++;
        number = cur;
        printf("Thread write, id = %lu, number = %d\n", pthread_self(), number);
        pthread_rwlock_unlock(&rwlock);
        usleep(rand() % 5);
    }
    return NULL;
}



int main() {
    // 初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);
    
    // 创建3个写线程和5个读线程
    pthread_t p1[5], p2[3];
    for (int i = 0; i < 5; i ++) {
        pthread_create(&p1[i], NULL, read_num, NULL);
    }

    for (int i = 0; i < 3; i ++) {
        pthread_create(&p2[i], NULL, write_num, NULL);
    }


    // 阻塞，回收所有线程
    for (int i = 0; i < 5; i ++) {
        pthread_join(p1[i], NULL);
    }

    for (int i = 0; i < 3; i ++) {
        pthread_join(p2[i], NULL);
    }

    //销毁读写锁
    pthread_rwlock_destroy(&rwlock);

    return 0;
}