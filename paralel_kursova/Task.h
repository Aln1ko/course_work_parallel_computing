#pragma once
#include <functional>
#include <winsock2.h>

class Task {
	int client_id;
	SOCKET client_socket;
	std::function<void(SOCKET client_socket)> f;
public:
	Task(int id, SOCKET client_socket, std::function<void(SOCKET client_socket)> fun) 
		: client_id(id), client_socket(client_socket), f(fun)  {	
	}
	void execute() {
		f(client_socket);
	}
};