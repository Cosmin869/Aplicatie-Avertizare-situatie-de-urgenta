#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>
#include <thread>
#include "DAL.h"
#pragma comment (lib, "ws2_32.lib")

static void clientHandler(SOCKET clientSocket, DAL db) {
    char buf[4096];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    getpeername(clientSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    char clientHost[NI_MAXHOST];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientHost, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);

    // Welcome message to the client

    bool checkCred = false;
    int dataReceived;
    string response;// = "Introduceti numele si parola";
    //std::string welcomeMsg;
    do {
        //response.clear();
       /* welcomeMsg = "Welcome to the server! Introduce your username:";
        send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);*/
        //send(clientSocket, response.c_str(), response.size() + 1, 0);
        // Verify credentials
        string date;
        std::string username, password, login;
        ZeroMemory(buf, 4096);
        dataReceived = recv(clientSocket, buf, 4096, 0);
        if (dataReceived == SOCKET_ERROR) {
            std::cout << "Client disconnected: " << clientHost << ":" << clientPort << std::endl;
            return;
        }
        username = strtok(buf, "*");
        password = strtok(NULL, "*");

        checkCred = db.getUserByUsername(username, password); //verify if there is a user with the username selected and if the password is correct
        if (checkCred) {
            response = "You're logged in!";
            send(clientSocket, "You're logged in!", response.size() + 1, 0);
        }
        else {
            response = "Wrong username or password!";
            send(clientSocket, "Wrong username or password!", response.size() + 1, 0);
        }
        //send(clientSocket, response.c_str(), response.size() + 1, 0);
    } while (!checkCred);
    //send(clientSocket, response.c_str(), response.size() + 1, 0);
    while (true) {
        int bytesReceived;

        // Receive data from the client
        ZeroMemory(buf, 4096);
        bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cout << "Client disconnected: " << clientHost << ":" << clientPort << std::endl;
            break;
        }
        if (bytesReceived == 0) {
            std::cout << "Client disconnected: " << clientHost << ":" << clientPort << std::endl;
            break;
        }
        // Print received message to server console
        std::cout << "Client> " << std::string(buf, bytesReceived) << std::endl;

        // Echo received data back to the client
        send(clientSocket, buf, bytesReceived, 0);
    }

    // Close the client socket
    closesocket(clientSocket);
}

class Server {
public:
    static Server& getInstance();
    static void destroyInstance();
    static void clientHandler(SOCKET, DAL);
    bool startServer(const std::string& ipAddress, int port);

    void AcceptingClients();


private:
    
    static Server* instance;

    Server() {};
    Server(const Server&) {};
    ~Server();
    bool isRunning_ = false;
    SOCKET serverSocket_;
};
