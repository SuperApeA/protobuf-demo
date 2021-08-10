#include "../src/tcpsocket.h"
int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("./main.out ip port");
        return -1;
    }
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;
    CHECK(sock.Socket());
    CHECK(sock.Bind(ip, port));
    CHECK(sock.Listen());

    TcpSocket clisock;
    struct sockaddr_in cliaddr;
    CHECK(sock.Accept(clisock, &cliaddr));
    while (1) {
        string buf;
        CHECK(clisock.Recv(buf));
        cout << "client say:" << buf << endl;
        buf.clear();
        cout << "server say:";
        fflush(stdout);
        getline(cin, buf);
        CHECK(clisock.Send(buf));
    }
    sock.Close();
    return 0;
}