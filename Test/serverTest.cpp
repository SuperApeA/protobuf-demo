//
// Created by Aaj on 2021/8/9.
//
#include "../src/Cartoon.pb.h"
#include "../src/MyTcpSocket.h"

#include<iostream>
#include <fstream>
#include <string>

using namespace std;

void addCartoon(CartoonRequest cartoonRequest){
    fstream output("CartoonInf",ios::out | ios::binary | ios::app); //app代表追加方式写入
    CartoonList cartoonList = cartoonRequest.cartoon();
    if(!cartoonList.SerializePartialToOstream(&output)){
        cerr<<"无法写入CartoonInf\n";
        exit(-1);
    }
    return ;
}

CartoonList selectByCartoonId(int Id){
    CartoonList cartoonList;
    fstream input("CartoonInf",ios::in | ios::binary);
    if(input && !cartoonList.ParseFromIstream(&input)){
        cerr<<"无法读取CartoonInf\n";
        exit(-1);
    }


//    /*这里暂时就直接显示出来*/
//    cout<<"*selectByCartoonId*"<<endl;
//    for(int i=0;i<cartoonList.cartoonlist_size();++i){
//        Cartoon cartoon = cartoonList.cartoonlist(i);
//        if(cartoon.id() == cartoonRequest.selectbyid()){
//            cout<<"动画Id："<<cartoon.id()<<endl;
//            cout<<"动画名称："<<cartoon.name()<<endl;
//            cout<<"动画出品公司："<<cartoon.company()<<endl;
//            cout<<"***************************************"<<endl;
//            break;
//        }
//    }

    CartoonList res;
    for(int i=0;i<cartoonList.cartoonlist_size();++i){
        Cartoon cartoon = cartoonList.cartoonlist(i);
        if(cartoon.id() == Id){
            Cartoon* cartoontmp = res.add_cartoonlist();
            cartoontmp = &cartoon;
            break;
        }
    }
    return res;
}

CartoonList selectAllCartoon(){
    CartoonList cartoonList;
    fstream input("CartoonInf",ios::in | ios::binary);
    if(input && !cartoonList.ParseFromIstream(&input)) {
        cerr << "无法读取CartoonInf\n";
        exit(-1);
    }

//    /*这里暂时就直接显示出来*/
//    cout<<"*selectAllCartoon*"<<endl;
//    for(int i=0;i<cartoonResponse.mutable_cartoon()->cartoonlist_size();++i){
//        Cartoon cartoon = cartoonResponse.mutable_cartoon()->cartoonlist(i);
//        cout<<"动画Id："<<cartoon.id()<<endl;
//        cout<<"动画名称："<<cartoon.name()<<endl;
//        cout<<"动画出品公司："<<cartoon.company()<<endl;
//        cout<<"***************************************"<<endl;
//    }

    return cartoonList;
}

int main(){
    //如果没有CartoonInf则先创建一个
    if(!fopen("CartoonInf","r")){
        fopen("CartoonInf","w");
    }
    string ip = to_string(INADDR_ANY);
    uint16_t port = 9999;
//    MyTcpSicket service;
//    CHECK(service.Socket());
//    CHECK(service.Bind(ip,port));
//    CHECK(service.Listen());

    struct sockaddr_in server;
    struct sockaddr_in client;

    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr));

    listen(listenfd,10);

    while(1) {
        int sin_size = sizeof(struct sockaddr_in);

        int connectfd = accept(listenfd, (struct sockaddr *)&client, reinterpret_cast<socklen_t *>(&sin_size));

        char data[100];
        int len = recv(connectfd,data,10,0);
        cout<<data<<endl;
        send(connectfd,"server",10,0);
    }

//    while(1){
//        MyTcpSocket clisock;
//        struct sockaddr_in cliaddr;
//        CHECK(service.Accept(clisock,&cliaddr));
//        string data;
//        CartoonRequest cartoonRequest;
//        CHECK(clisock.Recv(data));
//        cartoonRequest.ParseFromString(data);
//        CartoonResponse cartoonResponse;
//        if(cartoonRequest.query() == 1){
//            addCartoon(cartoonRequest);
//            cartoonResponse.set_res("addCartoon success!");
//        }
//        else if(cartoonRequest.query() == 2){
//            CartoonList clist = selectAllCartoon();
//            cartoonResponse.set_allocated_cartoon(&clist);
//            cartoonResponse.set_res("selectByCartoonId success!");
//        }
//        else if(cartoonRequest.query() == 3){
//            CartoonList clist = selectByCartoonId(cartoonRequest.selectbyid());
//            cartoonResponse.set_allocated_cartoon(&clist);
//            cartoonResponse.set_res("selectAllCartoon success!");
//        }
//        else{
//            cartoonResponse.set_res("query error！");
//        }
//        cartoonResponse.SerializeToString(&data);
//        CHECK(clisock.Send(data));
//    }
//    service.Close();
    return 0;
}