#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")  // Подключение библиотеки Winsock

#define SERVER_IP "127.0.0.1"  // Адрес сервера (локальный хост)
#define PORT 8080
#define BUFFER_SIZE 2048

int main() {
    // Инициализация библиотеки Winsock
    setlocale(LC_ALL, "");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock" << std::endl;
        return 1;
    }

    // Создание клиентского сокета
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // Подключение к серверу
    if (connect(client_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка подключения к серверу: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Подключение к серверу установлено" << std::endl;

    // Отправка сообщения серверу
    //const char* message = "hello from client";
    //if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
    //    std::cerr << "Ошибка отправки сообщения: " << WSAGetLastError() << std::endl;
    //    closesocket(client_socket);
    //    WSACleanup();
    //    return 1;
    //}
    //std::cout << "Сообщение отправлено серверу" << std::endl;

    //// Получение ответа от сервера
    char buffer[BUFFER_SIZE] = { 0 };
    int bytes_received;

    //bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    //if (bytes_received > 0) {
    //    buffer[bytes_received] = '\0';
    //    std::cout << "Ответ от сервера: " << buffer << std::endl;
    //}

    //for (size_t i = 0; i < sizeof(buffer); ++i) {
    //    buffer[i] = 0;
    //}

    while (true) {
        // Считывание пользовательского ввода
        std::cout << "Client request: ";
        std::string user_input;
        std::getline(std::cin, user_input);

         if (user_input == "update") {
             user_input += " C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\test\\neg";
         }

        // Отправка запроса серверу
        if (send(client_socket, user_input.c_str(), user_input.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Ошибка отправки сообщения: " << WSAGetLastError() << std::endl;
            break;
        }

        if (user_input == "quit") {
            std::cout << "Завершение соединения." << std::endl;
            break;
        }

        // Получение ответа от сервера
        std::string answer =  "";
        while (true) {
            bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                answer += buffer;
            }
            else if (bytes_received == 0) {
                std::cout << "Соединение закрыто сервером." << std::endl;
                break;
            }
            else {
                std::cerr << "Ошибка получения данных: " << WSAGetLastError() << std::endl;
                break;
            }

            if (answer.find("\n") != std::string::npos) {
                std::cout << "Ответ от сервера: " << answer << std::endl;
                for (size_t i = 0; i < sizeof(buffer); ++i) {
                    buffer[i] = 0;
                }
                break;
            }

            for (size_t i = 0; i < sizeof(buffer); ++i) {
                buffer[i] = 0;
            }
        }
        
    }
   

    // Закрытие сокета
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
