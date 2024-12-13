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

void Server::start(MyQueue& q, InvertedIndex& in_index)
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

        Task t(client_socket, [this,&in_index](SOCKET soc) {this->handle_client(soc, in_index); });
        q.push(t);
    }
}

Server::~Server()
{
    closesocket(server_socket);
}

void Server::handle_client(SOCKET client_socket, InvertedIndex& in_index)
{
    char buffer[2048] = { 0 };
    int bytesReceived = recv(client_socket, buffer, buffer_size, 0);
    if (bytesReceived > 0) {
        std::cout << "Obtained message: " << buffer << std::endl;
    }
    else {
        std::cerr << "Error obtaining data: " << WSAGetLastError() << std::endl;
    }

    std::vector<std::string> arr = in_index.find_index(buffer);
    std::string answer = "";
    for (auto& str : arr) {
        answer += str;
        answer += ", ";
    }
    // Отправка ответа клиенту
    //const char* message = answer.c_str();

    const char* message = "Hello from server";
    //char test[10000] = {  };
    send(client_socket, message, sizeof(message), 0);
    std::cout << "Message sent to client" << std::endl;


    while (true)
    {
        int bytes_recv = recv(client_socket,buffer,sizeof(buffer),0);
        if (bytes_recv == 0) {
            // Клиент закрыл соединение
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        if (bytes_recv == SOCKET_ERROR) {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            break;
        }
        buffer[bytes_recv] = '\0';

        std::string buffer_str(buffer);
        std::istringstream iss(buffer_str);
        std::string action;
        iss >> action;

        if (action == "quit") {
            std::cout << "Quit command received, end of connection" << std::endl;
            break;
        }
        else if (action == "find") {

        }
        else if (action == "update") {

        }
        else {
            std::string response = "Unknowen command";
            send(client_socket, response.c_str(), sizeof(response), 0);
        }

    }

    // Закрытие сокетов
    closesocket(client_socket);
}
