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
    CHECK(sock.Connect(ip, port));

    while (1) {
        string buf;
        cout << "client say:";
        fflush(stdout);
        getline(cin, buf);
        CHECK(sock.Send(buf));
        buf.clear();
        CHECK(sock.Recv(buf));
        cout << "server say:" << buf << endl;
    }
    sock.Close();
    return 0;
}
