#pragma once
#include <functional>
#include <winsock2.h>

//class Task {
//	SOCKET client_socket;
//	std::function<void(SOCKET)> f;
//public:
//	Task(SOCKET client_socket, std::function<void(SOCKET client_socket)> fun)
//		: client_socket(client_socket), f(fun)  {	
//	}
//	void execute() {
//		f(client_socket);
//	}
//};

class Task {
	std::string client_socket;
	std::function<void(std::string)> f;
public:
	Task(std::string client_socket, std::function<void(std::string client_socket)> fun)
		: client_socket(client_socket), f(fun) {
	}
	void execute() {
		f(client_socket);
	}
};