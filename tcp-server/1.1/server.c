/*
demo: tcp简单通信，单线程，服务端
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

    // 4. 阻塞并等待客户端连接
    struct sockaddr_in caddr;
    int addrlen = sizeof(caddr);
    int cfd = accept(fd, (struct sockaddr*)&caddr, &addrlen);
    if (cfd == -1) {
        perror("accpet fail!");
        return -1;
    }

    // 连接成功，输出客户端ip，port
    char ip[32];
    printf("Client IP: %s, Port: %d\n", inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(caddr.sin_port));

    // 5. 通信
    while (1) {
        // 接收数据
        char buff[1024];
        int len = recv(cfd, buff, sizeof(buff), 0);
        if (len > 0) {
            printf("Client say: %s\n", buff);
            send(cfd, buff, len, 0);
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

    // 6. 断开
    close(fd);
    close(cfd);


    return 0;
}