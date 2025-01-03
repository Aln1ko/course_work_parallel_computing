#pragma once
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <vector>
#include "MyQueue.h"
#include "InvertedIndex.h"
#include "FileFinder.h"
#include <sstream>

class Server {
public:
	Server(int port);
	void start(MyQueue& q,InvertedIndex& in_index, FileFinder& file_f);
	~Server();
private:
	SOCKET server_socket;
	int port;
	const int buffer_size = 2048;
	sockaddr_in server_addr;
	void handle_client(std::string str, InvertedIndex& in_index, 
		FileFinder& file_f, MyQueue& q);
};
