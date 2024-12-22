#include "InvertedIndex.h"
#include <sstream>
#include <fstream>
#include <set>
#include <execution>
#include <regex>



InvertedIndex::InvertedIndex() {}

void InvertedIndex::create_index(std::vector<std::string> files) {
    std::for_each(std::execution::par, files.begin(), files.end(), 
        [this](const std::string& path) { add_index_1file(path); });
}

void InvertedIndex::create_index1(std::vector<std::string> files, MyQueue& q) {
    for (const auto& file : files) {
        Task t(file, [this](std::string file) { this->add_index_1file(file); });
        q.push(t);
    }
}


void InvertedIndex::add_index_1file(const std::string& path) {
    std::ifstream file(path);
   // std::cout << path << std::endl;
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    /*for (char& c : content) {
        if (std::ispunct(c)) {
            c = ' ';
        }
    }*/
    std::regex punctuation(R"([[:punct:]])");

    content = std::regex_replace(content, punctuation, "");
    /*std::transform(content.begin(), content.end(), content.begin(), [](unsigned char ch) {
        if (!std::isalpha(ch))
            ch = ' ';
        else
            ch = std::tolower(ch);
        return ch;
        });*/

    std::string word;
    std::istringstream iss(content);
    std::set<std::string> set_words;

    while (iss >> word) {
        // Привести слово к нижнему регистру
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
            return std::tolower(c);
            });
        set_words.insert(word);
    }

    hm_mutex.lock();
    for (const std::string& word : set_words) {
        hm[word].push_back(path);
    }
    hm_mutex.unlock();
}



void InvertedIndex::print_index() {
    for (const auto& pair : hm) {
        std::cout << "Key: " << pair.first << "\nValues: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}


std::vector<std::string> InvertedIndex::find_index(std::string word) {
    hm_mutex.lock_shared();
    std::vector<std::string> res = hm[word];
    hm_mutex.unlock_shared();
    return res;
}

void InvertedIndex::update_index(std::vector<std::string> files) {
    
}


std::vector<std::string> InvertedIndex::get_words() {
    return words; 
}


InvertedIndex::~InvertedIndex() {}
