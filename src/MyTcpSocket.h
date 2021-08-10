//
// Created by Aaj on 2021/8/6.
//

#ifndef MYPROTOCOLBUF_MYTCPSOCKET_H
#define MYPROTOCOLBUF_MYTCPSOCKET_H  1

#include<string.h>
#include<iostream>
#include<string>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>

using namespace std;

void CHECK(bool res){
    if(res == false){
        exit(-1);
    }
}

class MyTcpSocket{
private:
    int _sock;
public:
    MyTcpSocket() : _sock(-1){}
    void setNewSockFd(int newsockfd){
        _sock = newsockfd;
    }
    //1.创建套接字
    bool Socket(){
        //函数原型 int socket(int domain, int type, int protocol);
        //int domain（协议族） 参数为 AF_INET 表示 TCP/IP_IPv4
        //int type（套接字类型） 参数为SOCK_STREAM 表示 TPC流
        //int protocol 参数为0，因为 domain 和 type 已经确定
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if(_sock < 0){
            perror("socket error");
            return false;
        }
        return true;
    }

    //2.为套接字绑定地址信息
    bool Bind(string& ip,uint16_t& port) const{
        struct sockaddr_in addr;        //加上struct为c写法
        bzero(&addr, sizeof(addr));     //清零
        addr.sin_family = AF_INET;      //TCP
        addr.sin_port = htons(port);    //htons是将整型变量从主机字节顺序转变成网络字节顺序， 就是整数在地址空间存储方式变为高位字节存放在内存的低地址处。
        addr.sin_addr.s_addr = inet_addr(ip.c_str()); //将IP转为二进制形式
        size_t len = sizeof(struct sockaddr_in);
        int binding = bind(_sock,(struct sockaddr*)&addr, len);
        if(binding < 0){
            perror("bind error");
            return false;
        }
        return true;
    }

    //3.服务端监听
    bool Listen(int backlog = 5) const {
        int listening = listen(_sock,backlog);
        if(listening < 0){
            perror("listen error");
            return false;
        }
        return true;
    }

    //4.客户端请求连接
    bool Connect(string& ip,uint16_t& port) const {
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        size_t len = sizeof(struct sockaddr_in);
        int connecting = connect(_sock,(struct sockaddr*)& addr,len);
        if(connecting < 0){
            perror("connect error");
            return false;
        }
        return true;
    }

    //5.服务端接受客户端请求
    bool Accept(MyTcpSocket& cli_sock,struct sockaddr_in* cli_addr = NULL){
        MyTcpSocket clisock;
        size_t len = sizeof(struct MyTcpSocket);
        int newsockfd = accept(_sock, (struct sockaddr*)&clisock, (socklen_t *)&len);
        if(newsockfd < 0){
            perror("accept error");
            return false;
        }
        if (cli_addr != NULL) {
            memcpy(cli_addr, &clisock, len);
            cli_sock.setNewSockFd(newsockfd);
        }
        return true;
    }

    //6.发送数据
    bool Send(string& buf){
        size_t size = send(_sock, buf.c_str(),buf.size(), 0);
        if(size < 0){
            perror("send error");
            return false;
        }
        return true;
    }

    //7.接收数据
    bool Recv(string& buf){
        char buf_tmp[4096] = {0};
        size_t size = recv(_sock,buf_tmp, sizeof(buf_tmp),0);
        if(size < 0){
            perror("recv error");
            return false;
        }
        else if(size == 0){
            perror("peer shutdown");
            return false;
        }
        buf.assign(buf_tmp, size);
        return true;
    }

    void Close(){
        close(_sock);
        _sock = -1;
    }
};

#endif //MYPROTOCOLBUF_MYTCPSOCKET_H
