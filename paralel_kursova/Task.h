#pragma once
#include <functional>
#include <winsock2.h>
#include "InvertedIndex.h"

class Task {
	SOCKET client_socket;
	std::function<void(SOCKET)> f;
public:
	Task(SOCKET client_socket, std::function<void(SOCKET client_socket)> fun)
		: client_socket(client_socket), f(fun)  {	
	}
	void execute() {
		f(client_socket);
	}
};