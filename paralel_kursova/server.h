#pragma once
#include <winsock2.h>
#include <iostream>

class Server {
public:
	Server(int port);
	void start();
	~Server();
private:
	SOCKET server_socket;
	int port;
	sockaddr_in server_addr;
	void handleClient(SOCKET client_socket);

};
