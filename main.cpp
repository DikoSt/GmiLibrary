#include <iostream>
#include <filesystem>
#include <vector>
#include <string>


auto recursiveGetFileNamesByExtension (std::filesystem::path path, const std::string extension)
    {
        std::vector<std::string> list_filename;
        for(auto& p: std::filesystem::recursive_directory_iterator(path)) {
            if (p.is_regular_file() && !p.path().extension().compare(extension)){
                list_filename.push_back(p.path().filename().string());
            }
        }
        return std::make_unique<std::vector<std::string>>(list_filename);
    };


 void parseGmiString(std::string gmiString){

    }
//enum headerLevel{0, H1, H2, H3}
std::string addHeader(std::string headerString, int headerLevel){
        std::string stringResult;
        if (headerLevel < 1 || headerLevel > 3) {
            return headerString;
        }

        stringResult += "<h" + std::to_string(headerLevel) + ">" +  // start tag
                        headerString +                              // text
                        "</h" + std::to_string(headerLevel) + ">";  // end tag
        return stringResult;
    }

std::string addHyperLink(std::string URI_adrres, std::string linkText){
        std::string stringResult;
        stringResult = "<a href = \"" + URI_adrres + "\">" +  // start tag
                        linkText +                              // text
                        "</a>";  // end tag
        return stringResult;
    }

std::string addQuote(std::string textQuote){
        std::string stringResult;
        stringResult = "<blockqoute>" +  // start tag
//TODO многострочная цитата - подумать как реализовать (надо <br> завместо \n вставлять)
                        textQuote +                              // text
                        "</blockqoute>";  // end tag
        return stringResult;
}

std::string addLists(std::string ListElement){
        std::string stringResult;
        stringResult = "<ul><li>" +   // start tag
                        ListElement +                              // text
                        "</li></ul>";  // end tag
        return stringResult;
}

std::string addLists(std::vector<std::string> ListElements){
        std::string stringResult;
        stringResult = "<ul>";   // start tag
            for (auto const &elList:ListElements){
                stringResult += "<li>" + elList + "</li>";
                // if (printprety) {
                //     stringResult += "\n";
                // }
            }
        stringResult += "</ul>";  // end tag
        return stringResult;
}

std::string addPreformatedText(std::string inputText){
    return std::string();
}

std::string addPlainText(std::string inputText){
    return std::string();
}

int main() {
//   auto res = recursiveGetFileNamesByExtension("c:/program1/ProffesionCPlusplus", ".cpp");
//   for (auto f_name=res->begin();f_name!=res->end();++f_name)
//       std::cout << *f_name << " " << std::endl;

    std::cout << addHeader("Heder 1", 1)  << std::endl; 
    std::cout << addHeader("Heder 2", 2)  << std::endl; 
    std::cout << addHeader("Heder 3", 3)  << std::endl;
    std::cout << addHyperLink("http://google.com", "Google COM") << std::endl;
    std::cout << addQuote(" А этот обра кадабра тексат я попробую вывестик как нибудь") << std::endl;
    std::cout << addLists("Это будет простой однострочный список") << std::endl;
    std::cout << addLists({"Элемент 1","Element 2", "Element 3", "Element 4"}) << std::endl;
    return 0;
}

/*

Файл с расширением .gmi содержит в себе текст в упрощенной разметке, которая называется gemtext. Поддерживаются следующие возможности разметки:

# Заголовок первого уровня
## Заголовок второго уровня
### Заголовок третьего уровня

* Элемент списка

> Цитата

=> http://some-address.com Ссылка

Преформатированный текст
*/