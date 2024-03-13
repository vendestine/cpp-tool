/*
demo: tcp简单通信，单线程，客户端
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // 1. 创建通信socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket fail!");
        return -1;
    }

    // 2. 连接服务器IP，Port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "192.168.174.128", &saddr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("connet fail!");
        return -1;
    }


    // 3. 通信
    int number = 0;
    while (1) {
        // 发送数据
        char buff[1024];
        sprintf(buff, "hello world, %d ... \n", number++);
        send(fd, buff, strlen(buff) + 1, 0);

        // 接收数据
        memset(buff, 0, sizeof(buff));
        int len = recv(fd, buff, sizeof(buff), 0);  //阻塞函数
        if (len > 0) {
            printf("Server say: %s\n", buff);
        }
        else if (len == 0) {
            printf("Server stop connecting ...\n");
            break;
        }
        else {
            perror("recv fail!");
            break;
        }
        sleep(1);
    }

    // 4. 断开
    close(fd);


    return 0;
}