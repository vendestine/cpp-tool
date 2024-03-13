/*
demo: tcp简单通信，多线程，服务端
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

// 信息结构体
struct SockInfo {
    struct sockaddr_in addr;
    int fd;
};

// 最多512个客户端同时连接
struct SockInfo infos[512];

// 函数声明
void* working(void* arg);


// 子线程和客户端进行通信
void* working(void* arg) {
    struct SockInfo* pinfo = (struct SockInfo*)arg;
    // 连接成功，输出客户端ip，port
    char ip[32];
    printf("Client IP: %s, Port: %d\n", inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(pinfo->addr.sin_port));
    
    // 5. 通信
    while (1) {
        // 接收数据
        char buff[1024];
        int len = recv(pinfo->fd, buff, sizeof(buff), 0);
        if (len > 0) {
            printf("tid: %ld, Client say: %s\n", pthread_self(), buff);
            send(pinfo->fd, buff, len, 0);
        }
        else if (len == 0) {
            printf("Client stop connecting ...\n");
            break;
        }
        else {
            perror("recv fail!");
            break;
        }
    }

    close(pinfo->fd);
    pinfo->fd = -1;

    return NULL;
}


int main() {
    // 1. 创建监听socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket fail!");
        return -1;
    }

    // 2. 监听socket绑定本地ip port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);   //建议使用5000以上
    saddr.sin_addr.s_addr = INADDR_ANY;    //0地址 网卡的实际ip地址
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind fail!");
        return -1;
    }

    // 3. 设置监听
    ret = listen(fd, 128);
    if (ret == -1) {
        perror("listen fail!");
        return -1;
    }

    int max = sizeof(infos) / sizeof(infos[0]);
    for (int i = 0; i < max; i ++) {
        bzero(&infos[i], sizeof(infos[i]));
        infos[i].fd = -1;
    }

    // 4. 阻塞并等待客户端连接
    int addrlen = sizeof(struct sockaddr_in);
    while (1) {
        struct SockInfo* pinfo;
        for (int i = 0; i < max; i ++) {
            if (infos[i].fd == -1) {
                pinfo = &infos[i];
                break;
            }
        }
        int cfd = accept(fd, (struct sockaddr*)&pinfo->addr, &addrlen);
        pinfo->fd = cfd;
        if (cfd == -1) {
            perror("accpet fail!");
            break;
        }
        // 创建子线程
        pthread_t tid;
        pthread_create(&tid, NULL, working, pinfo);
        // 分离 防止主线程多处阻塞
        pthread_detach(tid);
    }

    // 6. 断开
    close(fd);
    return 0;
}
