/*
demo: 生产者消费者模型，条件变量实现
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// 创建条件变量 互斥锁
pthread_cond_t cond;
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
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        num++;
        printf("producer, id: %ld, number: %d, source num: %d\n", pthread_self(), newNode->number, num);
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&cond);
        sleep(rand() % 3);
    }
    return NULL;
}

// 消费者线程
void * consumer(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while (head == NULL) {
            // 阻塞消费者线程
            pthread_cond_wait(&cond, &mutex);
        }
        struct Node* node = head;  //之后要释放头节点，所以先将以前的头节点存起来；
        head = head->next;
        num --;
        printf("consumer, id: %ld, number: %d, source num: %d\n", pthread_self(), node->number, num);
        free(node);
        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 3);
    }
    
    return NULL;
}


int main() {

    // 初始化条件变量 和 互斥锁
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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

    // 销毁条件变量和互斥锁
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}