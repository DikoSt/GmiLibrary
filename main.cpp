#include "gmi.h"

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

std::cout << "Start programm" << std::endl;
  //   auto res =
  //   recursiveGetFileNamesByExtension("c:/program1/ProffesionCPlusplus",
  //   ".cpp"); for (auto f_name=res->begin();f_name!=res->end();++f_name)
  //       std::cout << *f_name << " " << std::endl;

Gmi gmi;
  std::cout << std::endl;

  gmi.parseGmiString("# Heder 1");
  gmi.parseGmiString("## Heder 2");
  gmi.parseGmiString("### Heder 3");
  gmi.parseGmiString("* List Element");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("=> http://some-address.com Link 2");
  gmi.parseGmiString("=> http://some-address.com Link");
  gmi.parseGmiString("``` http://some-address.com Link");
  gmi.parseGmiString("=> http://some-address.com Link");
  gmi.parseGmiString("=> http://some-address.com Link");
  gmi.parseGmiString("=> http://some-address.com Link");
  gmi.parseGmiString("```");

  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString("> Quote 1");
  gmi.parseGmiString(" ");

  gmi.print();

 return 0;
}

/*

Файл с расширением .gmi содержит в себе текст в упрощенной разметке, которая
называется gemtext. Поддерживаются следующие возможности разметки:

# Заголовок первого уровня
## Заголовок второго уровня
### Заголовок третьего уровня

* Элемент списка

> Цитата

=> http://some-address.com Ссылка

Преформатированный текст
*/