//
// Created by Aaj on 2021/8/6.
//

#include"src/Cartoon.pb.h"
#include "src/MyTcpSocket.h"

#include<iostream>
#include <fstream>
#include <string>

using namespace std;

void addCartoon(CartoonRequest &cartoonRequest){
    fstream output("CartoonInf",ios::out | ios::binary | ios::app); //app代表追加方式写入
    CartoonList cartoonList = cartoonRequest.cartoon();
    if(!cartoonList.SerializePartialToOstream(&output)){
        cerr<<"无法写入CartoonInf"<<endl;
        exit(-1);
    }
    return ;
}

void selectByCartoonId(int Id,CartoonResponse &cartoonResponse){
    CartoonList cartoonList;
    fstream input("CartoonInf",ios::in | ios::binary);
    if(input && !cartoonList.ParseFromIstream(&input)){
        cerr<<"无法读取CartoonInf"<<endl;
        exit(-1);
    }


//    /*这里暂时就直接显示出来*/
//    cout<<"*selectByCartoonId*"<<endl;
//    for(int i=0;i<cartoonList.cartoonlist_size();++i){
//        Cartoon cartoon = cartoonList.cartoonlist(i);
//        cout<<"动画Id："<<cartoon.id()<<endl;
//        cout<<"动画名称："<<cartoon.name()<<endl;
//        cout<<"动画出品公司："<<cartoon.company()<<endl;
//        cout<<"***************************************"<<endl;
//    }

    for(int i=0;i<cartoonList.cartoonlist_size();++i){
        if(cartoonList.cartoonlist(i).id() == Id){
            Cartoon* cartoon = cartoonResponse.mutable_cartoon()->add_cartoonlist();
            cartoon->set_id(cartoonList.cartoonlist(i).id());
            cartoon->set_name(cartoonList.cartoonlist(i).name());
            cartoon->set_company(cartoonList.cartoonlist(i).company());
            break;
        }
    }
    return ;
}

void selectAllCartoon(CartoonResponse &cartoonResponse){
    CartoonList cartoonList;
    fstream input("CartoonInf",ios::in | ios::binary);
    if(input && !cartoonList.ParseFromIstream(&input)) {
        cerr << "无法读取CartoonInf"<<endl;
        exit(-1);
    }

//    /*这里暂时就直接显示出来*/
//    cout<<"*selectAllCartoon*"<<endl;
//    for(int i=0;i<cartoonList.cartoonlist_size();++i){
//        Cartoon cartoon = cartoonList.cartoonlist(i);
//        cout<<"动画Id："<<cartoon.id()<<endl;
//        cout<<"动画名称："<<cartoon.name()<<endl;
//        cout<<"动画出品公司："<<cartoon.company()<<endl;
//        cout<<"***************************************"<<endl;
//    }

    for(int i=0;i<cartoonList.cartoonlist_size();++i){
        Cartoon* cartoon = cartoonResponse.mutable_cartoon()->add_cartoonlist();
        cartoon->set_id(cartoonList.cartoonlist(i).id());
        cartoon->set_name(cartoonList.cartoonlist(i).name());
        cartoon->set_company(cartoonList.cartoonlist(i).company());
    }
    return ;
}

int main(){
    //如果没有CartoonInf则先创建一个
    if(!fopen("CartoonInf","r")){
        fopen("CartoonInf","w");
    }
    string ip = to_string(INADDR_ANY);
    uint16_t port = 9999;
    MyTcpSocket service;
    CHECK(service.Socket());
    CHECK(service.Bind(ip,port));
    CHECK(service.Listen());


    MyTcpSocket clisock;
    struct sockaddr_in cliaddr;
    CHECK(service.Accept(clisock, &cliaddr));
    //selectAllCartoon(); //测试用

    while(1){
        string data;
        CartoonRequest cartoonRequest;
        CHECK(clisock.Recv(data));
        cartoonRequest.ParseFromString(data);
        CartoonResponse cartoonResponse;
        if(cartoonRequest.query() == 1){
            cout<<"addCartoon"<<endl;
            addCartoon(cartoonRequest);
            cartoonResponse.set_res("addCartoon success!");
        }
        else if(cartoonRequest.query() == 2){
            cout<<"selectByCartoonId"<<endl;
            selectByCartoonId(cartoonRequest.selectbyid(),cartoonResponse);
            if(cartoonResponse.cartoon().cartoonlist_size()==0){
                cartoonResponse.set_res("Cartoons don't exist");
            }
            else{
                cartoonResponse.set_res("selectByCartoonId success!");
            }
        }
        else if(cartoonRequest.query() == 3){
            cout<<"selectAllCartoon"<<endl;
            selectAllCartoon(cartoonResponse);
            if(cartoonResponse.cartoon().cartoonlist_size()==0){
                cartoonResponse.set_res("Cartoons don't exist");
            }
            else{
                cartoonResponse.set_res("selectAllCartoon success!");
            }
        }
        else{
            cartoonResponse.set_res("query error！");
        }
        cartoonResponse.SerializeToString(&data);
        CHECK(clisock.Send(data));
    }
    service.Close();
    return 0;
}