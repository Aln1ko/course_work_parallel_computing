#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "server.h"
#pragma comment(lib,"ws2_32.lib")


Server::Server(int port) : port(port) {
    setlocale(LC_ALL, "");

    // Инициализация библиотеки Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error initialization Winsock" << std::endl;
        return ;
    }

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Creating socket error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return ;
    }

    // Настройка адреса сервера
    //serverAddr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Привязка сокета к адресу и порту
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << " Binding ocket error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return ;
    }

    // Начало прослушивания входящих соединений
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return ;
    }

    std::cout << "Waiting for connection on port " << port << "..." << std::endl;
}

void Server::start() 
{
    while (true) {
        SOCKET client_socket;
        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);

        client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Error accepting client: " << WSAGetLastError() << std::endl;
            continue;
        }
        std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr)
            << ":" << ntohs(client_addr.sin_port) << std::endl;
        //
        std::thread clientThread(&Server::handleClient, this, client_socket);
        clientThread.detach();
        //threads.push_back(handleClient, client_socket);
        //

    }
}

Server::~Server()
{
    closesocket(server_socket);
}

void Server::handleClient(SOCKET client_socket)
{
    char buffer[1024] = { 0 };
    int bytesReceived = recv(client_socket, buffer, buffer_size, 0);
    if (bytesReceived > 0) {
        std::cout << "Получено сообщение: " << buffer << std::endl;
    }
    else {
        std::cerr << "Ошибка получения данных: " << WSAGetLastError() << std::endl;
    }

    // Отправка ответа клиенту
    const char* message = "Привет от сервера!";
    send(client_socket, message, strlen(message), 0);
    std::cout << "Сообщение отправлено клиенту" << std::endl;

    // Закрытие сокетов
    closesocket(client_socket);
}
