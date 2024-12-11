#pragma once
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <vector>

class Server {
public:
	Server(int port);
	void start();
	~Server();
private:
	SOCKET server_socket;
	int port;
	const int buffer_size = 1024;
	sockaddr_in server_addr;
	void handleClient(SOCKET client_socket);

};
