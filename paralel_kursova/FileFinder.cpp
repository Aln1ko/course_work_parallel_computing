#include "FileFinder.h"

FileFinder::FileFinder(){}

std::vector<std::string> FileFinder::find_files(std::vector<std::string> data_folders, int start_index, int finish_index,
    int start_index1, int finish_index1)
{
    int start;
    int finish;
    std::vector<std::string> files;
    for (const std::string& folder : data_folders) {

        if (folder == "C:\\Users\\RT\\Desktop\\7semestr\\aclImdb\\train\\unsup") {
            start = start_index1;
            finish = finish_index1;
        }
        else {
            start = start_index;
            finish = finish_index;
        }

        for (const auto& entry : std::filesystem::directory_iterator(folder))
        {
            if (is_valid(entry.path().filename().string(), start, finish))
            {
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}
bool FileFinder::is_valid(std::string file_path, int start, int finish)
{
    std::stringstream ss(file_path);
    std::string number;
    char delimeter = '_';

    std::getline(ss, number, delimeter);
    try {
        int num = std::stoi(number);
        if (num >= start && num <= finish) {
            return true;
        }
        else {
            return false;
        }
    }
    catch (const std::invalid_argument& e) {
        return false;
    }
    catch (const std::out_of_range& e) {
        return false;
    }

}