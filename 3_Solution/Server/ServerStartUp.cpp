#include "ServerStartUp.h"

Server* Server::instance = nullptr;

Server& Server::getInstance() {
    if (!Server::instance) {
        Server::instance = new Server();
    }
    return (*Server::instance);
}

void Server::destroyInstance() {
    if (Server::instance) {
        delete Server::instance;
        Server::instance = nullptr;
    }
}

void Server::clientHandler(SOCKET clientSocket, DAL db) {
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

        std::string protocol;
        protocol = strtok(buf, "*");

        if (protocol == "0")
        {
            username = strtok(NULL, "*");
            password = strtok(NULL, "*");

            //Create user
            //std::string protocol;
            // if(protocol == "1")

            checkCred = db.getUserByUsername(username, password); //verify if there is a user with the username selected and if the password is correct
            if (checkCred)
            {
                response = "You're logged in!";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
            else {
                response = "Wrong username or password!";
                send(clientSocket, "Wrong username or password!", response.size() + 1, 0);
            }
            //send(clientSocket, response.c_str(), response.size() + 1, 0);
        }

        else
        {
            checkCred = db.getUserByUsername(username, password);
            if (checkCred == FALSE)
            {
                username = strtok(NULL, "*");
                password = strtok(NULL, "*");

                User user;
                user.password = password;
                user.username = username;

                db.createUser(user);
            }

        }

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

void Server::AcceptingClients() {
    std::vector<std::thread> threads;
    std::string connection_string = "Driver={ODBC Driver 17 for SQL Server};Server=localhost;Database=POO; Trusted_Connection=yes;";

    DAL db(connection_string);
    while (true) {
        if (listen(Server::getInstance().serverSocket_, SOMAXCONN) == INVALID_SOCKET) {
            cout << "error " << WSAGetLastError() << endl;
            return;
        }
        sockaddr_in client;
        int clientSize = sizeof(client);
        SOCKET clientSocket;
        clientSocket = accept(serverSocket_, (sockaddr*)&client, &clientSize);

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

}

bool Server::startServer(const std::string& ipAddress, int port) {
    if (isRunning_) {
        std::cout << "Server is already running." << std::endl;
        return false;
    }

    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &wsData);
    if (wsResult != 0) {
        std::cerr << "Failed to initialize winsock." << std::endl;
        return false;
    }

    // Create socket
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return false;
    }

    // Bind the socket to an IP address and port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &(hint.sin_addr));

    ::bind(serverSocket_, (sockaddr*)&hint, sizeof(hint));

    // Start listening on the socket
    int listenResult = listen(serverSocket_, SOMAXCONN);
    if (listenResult == SOCKET_ERROR) {
        std::cerr << "Failed to start listening." << std::endl;
        closesocket(serverSocket_);
        WSACleanup();
        return false;
    }

    isRunning_ = true;
    return true;
}

Server::~Server() {
    if (!isRunning_) {
        std::cout << "Server is not running." << std::endl;
        return;
    }

    // Cleanup and close the server socket
    closesocket(serverSocket_);
    WSACleanup();

    isRunning_ = false;
}
