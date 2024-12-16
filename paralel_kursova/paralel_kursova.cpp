
#include "InvertedIndex.h"
#include "FileFinder.h"
#include "server.h"
#include "ThreadPool.h"
#include <chrono>


int main()
{
	std::vector<std::string> data_folders = { "C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\test\\neg",
						"C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\test\\pos",
						"C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\train\\neg",
						"C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\train\\pos",
						"C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\train\\unsup" };
	int start_index = 8000;
	int finish_index = 8250;
	int start_index1 = 32000;
	int finish_index1 = 33000;
	/*std::vector<std::string> data_folders = {"D:\\sasha\\4-course\\data\\t1",
		"D:\\sasha\\4-course\\data\\t2"};
	int start_index = 0;
	int finish_index = 10;*/
	
	auto start_time = std::chrono::high_resolution_clock::now();

	FileFinder file_f;
	std::vector<std::string> files = file_f.find_files(data_folders, start_index, finish_index, start_index1, finish_index1);

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end_time - start_time;
	std::cout << "Time execution: " << duration.count() << " seconds" << std::endl;

	start_time = std::chrono::high_resolution_clock::now();

	InvertedIndex in_index;
	in_index.create_index(files);

	end_time = std::chrono::high_resolution_clock::now();
	duration = end_time - start_time;
	std::cout << "Time execution: " << duration.count() << " seconds" << std::endl;

	MyQueue q;
	ThreadPool tp(4,q);
	tp.inizialize();
	Server s = Server(8080);
	s.start(q, in_index, file_f);
	

	return 0;
}

