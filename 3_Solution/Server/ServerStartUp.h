#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>
#include <thread>
#include "DAL.h"
#pragma comment (lib, "ws2_32.lib")


class Server {
public:
    static Server& getInstance();
    static void destroyInstance();
    static void clientHandler(SOCKET, DAL);
    bool startServer(const std::string& ipAddress, int port);

    void AcceptingClients();


private:

    static Server* instance;
    const vector<string> AdminVect = { "admin1", "admin2" };
    Server() {};
    Server(const Server&) {};
    ~Server();
    bool isRunning_ = false;
    SOCKET serverSocket_;
};
