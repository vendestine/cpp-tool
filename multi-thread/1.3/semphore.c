/*
demo: 生产者消费者模型，信号量实现
其中如果信号量资源总和为1，不需要线程同步，直接可以当作互斥锁用
如果信号量资源总和>1，需要线程同步；
信号量资源初始化 就看 对应的工作线程 最多几个同时工作；
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// 创建生产者信号量 和 消费者信号量
sem_t semp;
sem_t semc;

// 创建互斥锁
pthread_mutex_t mutex;


// 任务队列里的资源个数
int num;

struct Node {
    int number;
    struct Node* next;
};

struct Node* head = NULL;

// 生产者线程
void * producer(void* arg) {
    while(1) {
        // 头插法 添加节点
        sem_wait(&semp);
        pthread_mutex_lock(&mutex);
        
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        num++;
        printf("producer, id: %ld, number: %d, source num: %d\n", pthread_self(), newNode->number, num);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&semc);
        sleep(rand() % 3);
    }
    return NULL;
}

// 消费者线程
void * consumer(void* arg) {
    while(1) {
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);

        struct Node* node = head;  //之后要释放头节点，所以先将以前的头节点存起来；
        head = head->next;
        num --;
        printf("consumer, id: %ld, number: %d, source num: %d\n", pthread_self(), node->number, num);
        free(node);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        sleep(rand() % 3);
    }
    
    return NULL;
}


int main() {
    // 初始化互斥锁；
    pthread_mutex_init(&mutex, NULL);

    // 初始化生产者信号量
    sem_init(&semp, 0, 5);
    sem_init(&semc, 0, 0);

    // 创建生产者线程 和 消费者线程
    pthread_t t1[5], t2[5];
    for (int i = 0; i < 5; i ++) {
        pthread_create(&t1[i], NULL, producer, NULL);
    }

    for (int i = 0; i < 5; i ++) {
        pthread_create(&t2[i], NULL, consumer, NULL);
    }


    // 阻塞，回收
    for (int i = 0; i < 5; i ++) {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }

    // 销毁信号量和互斥锁
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semp);
    sem_destroy(&semc);

    return 0;
}