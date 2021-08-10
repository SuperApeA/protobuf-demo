//
// Created by Aaj on 2021/8/9.
//

#ifndef MYPROTOCOLBUF_TCPSOCKET_H
#define MYPROTOCOLBUF_TCPSOCKET_H

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
using std::cout;
using std::endl;
using std::cin;
using std::string;
inline void CHECK(bool operation) {
    if (operation == false) {
        exit(-1);
    }
}
class TcpSocket {
public:
    TcpSocket() : _sock(-1) {}
    void setNewSockFd(int newsockfd) {
        _sock = newsockfd;
    }
    // 创建套接字
    bool Socket() {
        _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_sock < 0) {
            perror("socket error");
            return false;
        }
        return true;
    }
    // 为套接字绑定地址信息
    bool Bind(string& ip, uint16_t& port) const {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        size_t len = sizeof(struct sockaddr_in);
        int binding = bind(_sock, (struct sockaddr*)&addr, len);
        if (binding < 0) {
            perror("bind error");
            return false;
        }
        return true;
    }
    // 服务端监听
    bool Listen(int backlog = 5) const {
        int listening = listen(_sock, backlog);
        if (listening < 0) {
            perror("listen error");
            return false;
        }
        return true;
    }
    // 客户端请求连接
    bool Connect(string& ip, uint16_t& port) const {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        size_t len = sizeof(struct sockaddr_in);
        int connecting = connect(_sock, (struct sockaddr*)&addr, len);
        if (connecting < 0) {
            perror("connect error");
            return false;
        }
        return true;
    }
    // 服务端接受客户端请求
    bool Accept(TcpSocket& cli_sock, struct sockaddr_in* cli_addr = NULL) {
        TcpSocket clisock;
        size_t len = sizeof(struct TcpSocket);
        int newsockfd = accept(_sock, (struct sockaddr*)&clisock, (socklen_t *)&len);
        if (newsockfd < 0) {
            perror("accept error");
            return false;
        }
        if (cli_addr != NULL) {
            memcpy(cli_addr, &clisock, len);
            cli_sock.setNewSockFd(newsockfd);
        }
        return true;
    }
    // 发送数据
    bool Send(string& buf) {
        size_t size = send(_sock, buf.c_str(), buf.size(), 0);
        if (size < 0) {
            perror("send error");
            return false;
        }
        return true;
    }
    // 接收数据
    bool Recv(string& buf) {
        char buf_tmp[4096] = {0};
        size_t size = recv(_sock, buf_tmp, sizeof(buf_tmp), 0);
        if (size < 0) {
            perror("recv error");
            return false;
        } else if (size == 0) {
            perror("peer shutdown");
            return false;
        }
        buf.assign(buf_tmp, size);
        return true;
    }
    void Close() {
        close(_sock);
        _sock = -1;
    }
private:
    int _sock;
};

#endif //MYPROTOCOLBUF_TCPSOCKET_H
