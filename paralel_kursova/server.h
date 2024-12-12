#pragma once
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <vector>
#include "MyQueue.h"

class Server {
public:
	Server(int port);
	void start(MyQueue& q);
	~Server();
private:
	SOCKET server_socket;
	int port;
	const int buffer_size = 1024;
	sockaddr_in server_addr;
	void handle_client(SOCKET client_socket);

};
