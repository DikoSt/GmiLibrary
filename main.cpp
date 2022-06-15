#include "gmi.h"
#include <filesystem>
#include <fstream>

auto recursiveGetFileNamesByExtension(std::filesystem::path path,
                                      const std::string extension) {
  std::vector<std::string> list_filename;
  for (auto &p : std::filesystem::recursive_directory_iterator(path)) {
    if (p.is_regular_file() && !p.path().extension().compare(extension)) {
      list_filename.push_back(p.path().filename().string());
    }
  }
  return std::make_unique<std::vector<std::string>>(list_filename);
};


int main() {
Gmi gmi;
std::cout << "Start programm" << std::endl;
  
    std::cout << std::endl;
    std::fstream file;

    file.open("../testgmi.gmi", std::ios::in);

    if (file.is_open()){
        file >> gmi;
        file.close();
    }

    file.open("../htmlresult.html", std::ios::out);

    if (file.is_open()){
        file << gmi.toHtml();
        file.close();
    }


 return 0;
}