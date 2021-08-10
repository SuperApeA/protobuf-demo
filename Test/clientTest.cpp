//
// Created by Aaj on 2021/8/9.
//

#include "../src/Cartoon.pb.h"
#include "../src/MyTcpSocket.h"
#include <google/protobuf/util/time_util.h>

#include<cstdio>
#include<iostream>
#include<fstream>
#include<ctime>
#include<list>
#include <netdb.h>

using namespace std;
using google::protobuf::util::TimeUtil;

void addCartoon(CartoonRequest &cartoonRequest){
    while(true){
        Cartoon *cartoon = cartoonRequest.mutable_cartoon()->add_cartoonlist();
        cout<<"输入动画Id: ";
        int Id;
        cin>>Id;cout<<Id<<endl;
        cin.ignore(256,'\n');//忽略掉一个回车
        cartoon->set_id(Id);

        cout<<"输入动画名称: ";
        string name;
        getline(cin,name);cout<<name<<endl;
        cartoon->set_name(name);

        cout<<"输入动画出品公司: ";
        string company;
        getline(cin,company);cout<<company<<endl;
        cartoon->set_company(company);

        *cartoon->mutable_time() = TimeUtil::SecondsToTimestamp(time(NULL));

        string res;
        while(true){
            cout<<"输入 0 结束，1 继续输入\n";
            cin>>res;
            if(res == "0" || res == "1")
                break;
        }
        if(res == "0"){
//            /*这里暂时写成直接写入CartoonInf文件方式*/
//            fstream output("CartoonInf",ios::out | ios::binary | ios::app); //app代表追加方式写入
//            CartoonList cartoonList = cartoonRequest.cartoon();
//            if(!cartoonList.SerializePartialToOstream(&output)){
//                cerr<<"无法写入CartoonInf\n";
//                exit(-1);
//            }
            return ;
        }
    }
}
void selectById(CartoonRequest &cartoonRequest){
    cout<<"请输入Id: ";
    int Id;
    cin>>Id;
    cin.ignore(256,'\n');//忽略掉一个回车
    cartoonRequest.set_selectbyid(Id);
}

void selectAll(CartoonRequest &cartoonRequest){
    cartoonRequest.set_selectall(true);
}
int main(){
    //如果没有CartoonInf则先创建一个
    if(!fopen("CartoonInf","r")){
        fopen("CartoonInf","w");
    }

    string ip = "192.168.17.245";//本机
    uint16_t  port = 9999;
//    MyTcpSicket client;
//    CHECK(client.Socket());
//    CHECK(client.Connect(ip,port));

    int fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr));

    send(fd,"client",10,0);
    char data[100];
    int len = recv(fd, data, 10, 0);
    cout<<data<<endl;

//    while(1){
//        CartoonRequest cartoonRequest;
//        cout<<"输入请求码 1->add 2->selectById 3->selectAll，或者直接回车退出: ";
//        int code;
//        cin>>code;
//        cin.ignore(256,'\n');//忽略掉一个回车
//        cartoonRequest.set_query(code);
//        //add
//        if(code == 1){
//            addCartoon(cartoonRequest);
//            //break;
//        }
//            //selectById
//        else if(code == 2){
//            selectById(cartoonRequest);
//            //break;
//        }
//            //selectAll
//        else if(code == 3){
//            selectAll(cartoonRequest);
//            //break;
//        }
//        else{
//            cout<<"退出成功！\n";
//            break;
//            //cout<<"请求码非法，重新输入！\n";
//        }
//        string data;
//        cartoonRequest.SerializeToString(&data);
//        CHECK(client.Send(data));
//        data.clear();
//        CHECK(client.Recv(data));
//        CartoonResponse cartoonResponse;
//        cartoonResponse.ParseFromString(data);
//        cout<<cartoonResponse.res()<<endl;
//        if(code == 2 || code == 3){
//            for(int i=0;i<cartoonResponse.mutable_cartoon()->cartoonlist_size();++i){
//                Cartoon cartoon = cartoonResponse.mutable_cartoon()->cartoonlist(i);
//                cout<<"动画Id："<<cartoon.id()<<endl;
//                cout<<"动画名称："<<cartoon.name()<<endl;
//                cout<<"动画出品公司："<<cartoon.company()<<endl;
//                cout<<"***************************************"<<endl;
//            }
//        }
//    }
//    client.Close();
    return 0;
}