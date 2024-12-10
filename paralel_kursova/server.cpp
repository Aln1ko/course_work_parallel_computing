#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "server.h"

Server::Server(int port) : port(port) {
    setlocale(LC_ALL, "");

    // ������������� ���������� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error initialization Winsock" << std::endl;
        return ;
    }

    // �������� ������
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Creating socket error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return ;
    }

    // ��������� ������ �������
    //serverAddr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // �������� ������ � ������ � �����
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << " Binding ocket error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return ;
    }

    // ������ ������������� �������� ����������
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

    }
}

Server::~Server()
{
    closesocket(server_socket);
}

void Server::handleClient(SOCKET client_socket)
{

}
