#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
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
#include "ServerStartUp.h"
//#include "ServerStartUp.h"
//#pragma comment (lib, "sqldb.lib")


using namespace std;



int main() {
    Server& server = Server::getInstance();
    if (server.startServer("127.0.0.1", 54001)) {
        std::cout << "Server started successfully." << std::endl;
        server.AcceptingClients();
    }

    server.destroyInstance();

    return 0;
}
