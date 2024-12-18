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

void Server::start(MyQueue& q, InvertedIndex& in_index, FileFinder& file_f, MyQueue& q_index)
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

        //Task t(client_socket, [this,&in_index,&file_f](SOCKET soc) {this->handle_client(soc, in_index,file_f); });
        std::string str = std::to_string(client_socket);
        Task t(str, [this, &in_index, &file_f,&q_index](std::string str) {this->handle_client(str, in_index, file_f, q_index); });
        q.push(t);
    }
}

Server::~Server()
{
    closesocket(server_socket);
}

bool is_number(const std::string& str) {
    if (str.empty()) return false;
    size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    return std::all_of(str.begin() + start, str.end(), ::isdigit);
}

void Server::handle_client(std::string str, InvertedIndex& in_index, FileFinder& file_f, MyQueue& q_index)
{
    int client_socket = stoi(str);
    char buffer[2048] = { 0 };
    //int bytesReceived = recv(client_socket, buffer, buffer_size, 0);
    //if (bytesReceived > 0) {
    //    std::cout << "Obtained message: " << buffer << std::endl;
    //}
    //else {
    //    std::cerr << "Error obtaining data: " << WSAGetLastError() << std::endl;
    //}


    //// Отправка ответа клиенту
    //std::string  message = "Hello from server";
    //send(client_socket, message.c_str(), message.length(), 0);
    //std::cout << "Message sent to client" << std::endl;

    //for (size_t i = 0; i < sizeof(buffer); ++i) {
    //    buffer[i] = 0;
    //}

   /* std::vector<std::string> pathes{ "C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\test\\neg" };
    std::vector<std::string> files = file_f.find_files(pathes, 0, 1000, 0, 1000);
    in_index.create_index(files);*/

    while (true)
    {
        int bytes_recv = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_recv == 0) {
            // Клиент закрыл соединение
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        if (bytes_recv == SOCKET_ERROR) {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            break;
        }
        if (bytes_recv > 0) {
            buffer[bytes_recv] = '\0';
            std::cout << "Запрос клиента: " << buffer << std::endl;
        }

        std::string buffer_str(buffer);
        std::istringstream iss(buffer_str);
        std::string action;
        iss >> action;
        //std::cout << "action is " << action << std::endl;
        
        if (action == "quit") {
            std::cout << "Quit command received, end of connection" << std::endl;
            break;
        }

        else if (action == "find") {
            std::string word;
            iss >> word;
            std::cout << "word is " << word << std::endl;
            std::transform(word.begin(), word.end(), word.begin(), 
                [](unsigned char c) {return std::tolower(c);});

            std::vector<std::string> arr = in_index.find_index(word);
            std::string answer = "";
            for (int i = 0; i < arr.size();i ++) {
                answer += arr[i];
                if (i != arr.size() - 1) {
                    answer += ", ";
                }
            }
            /*std::cout << "answer is " << answer << std::endl;
            std::cout << "answer2 is " << answer.c_str() << std::endl;
            std::cout << "sizeof buffer: " << sizeof(answer.c_str()) <<"  "<<answer.length() << std::endl;
            */
            answer += "\n";
            send(client_socket, answer.c_str(),answer.length(),0);
        }

        else if (action == "update") {
            std::string folder, num1_str, num2_str;
            if (iss >> num1_str && !is_number(num1_str)) {
                folder = num1_str;
                num1_str = "0";  // Стандартное значение, если это не число
                num2_str = "1000";
            }
            else if (iss >> num2_str && !is_number(num2_str)) {
                num2_str = "1000";
                folder = num2_str;
            }
            else {
                iss >> folder;
            }

            int num1 = std::stoi(num1_str);
            int num2 = std::stoi(num2_str);
            std::vector<std::string> folders{folder};
            
            std::vector<std::string> files = file_f.find_files(folders, num1, num2, num1, num2);
            in_index.create_index1(files, q_index);
            std::string response = "Update is being carried out \n";
            send(client_socket, response.c_str(), response.length(), 0);
        }

        else {
            std::string response = "Unknowen command";
            send(client_socket, response.c_str(), sizeof(response), 0);
        }

        for (size_t i = 0; i < sizeof(buffer); ++i) {
            buffer[i] = 0;
        }

    }

    // Закрытие сокетов
    closesocket(client_socket);
}

