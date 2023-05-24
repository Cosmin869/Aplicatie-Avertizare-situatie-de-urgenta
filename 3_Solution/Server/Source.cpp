#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WS2tcpip.h>
#include <thread>

#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>

#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "odbc32.lib")
#pragma comment (lib, "odbccp32.lib")
#include "DAL.h"
//#include "ServerStartUp.h"
//#pragma comment (lib, "sqldb.lib")


using namespace std;


// Function to handle each connected client
void clientHandler(SOCKET clientSocket, DAL db) {
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
    std::string welcomeMsg;
    do {
        welcomeMsg = "Welcome to the server! Introduce your username:";
        send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

        // Verify credentials
        std::string username, password, login, response;
        ZeroMemory(buf, 4096);
        dataReceived = recv(clientSocket, buf, 4096, 0);
        if (dataReceived == SOCKET_ERROR) {
            std::cout << "Client disconnected: " << clientHost << ":" << clientPort << std::endl;
            return;
        }
        username = buf;

        login = "Introduce your password: ";
        send(clientSocket, login.c_str(), login.size() + 1, 0);
        ZeroMemory(buf, 4096);
        dataReceived = recv(clientSocket, buf, 4096, 0);
        if (dataReceived == SOCKET_ERROR) {
            std::cout << "Client disconnected: " << clientHost << ":" << clientPort << std::endl;
            return;
        }
        password = buf;

        checkCred = db.getUserByUsername(username, password); //verify if there is a user with the username selected and if the password is correct
        if (checkCred) {
            response = "You're logged in!";
        }
        else {
            response = "Wrong username or password!";
        }
        send(clientSocket, response.c_str(), response.size() + 1, 0);
    } while (!checkCred);

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

int main() {

    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        std::cerr << "Can't initialize winsock! Quitting" << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        return 1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in hint{};
    //if (hint == INVALID_)
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54001);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Bind to any available network interface

    // Bind the socket to an IP address and port
    ::bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Listen for incoming connections


    std::cout << "Server started, waiting for connections..." << std::endl;

    std::vector<std::thread> threads; // Vector to hold threads for each connected client
    //connect database
    std::string connection_string = "Driver={ODBC Driver 17 for SQL Server};Server=localhost;Database=POO; Trusted_Connection=yes;";

    DAL db(connection_string);

    while (true) {
            if (listen(listening, SOMAXCONN) == INVALID_SOCKET) {
        cout << "error "<< WSAGetLastError() <<endl;
        return 1;
    }
        sockaddr_in client;
        int clientSize = sizeof(client);
        SOCKET clientSocket;
        clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Can't accept client connection! Error code: " << WSAGetLastError() << std::endl;
            break;
        }


        // Print client's IP address and port
        char clientHost[NI_MAXHOST];
        char clientService[NI_MAXSERV];
        ZeroMemory(clientHost, NI_MAXHOST);
        ZeroMemory(clientService, NI_MAXSERV);
        if (getnameinfo((sockaddr*)&client, sizeof(client), clientHost, NI_MAXHOST, clientService, NI_MAXSERV, 0) == 0) {
            std::cout << "Client connected: " << clientHost << " on port " << clientService << std::endl;
        }
        else {
            inet_ntop(AF_INET, &client.sin_addr, clientHost, NI_MAXHOST);
            std::cout << "Client connected: " << clientHost << " on port " << ntohs(client.sin_port) << std::endl;
        }

        // Create a new thread to handle the connected client
        std::thread t(clientHandler, clientSocket, db);
        threads.push_back(std::move(t));
    }

    // Close the listening socket
    closesocket(listening);
    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Cleanup
    WSACleanup();
    return 0;
}
