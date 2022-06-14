#include <filesystem>
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> htmlDoc;
std::vector<std::string> gmiDoc;

int startFlag = 0;
enum START_FLAG {
  START_QUOTE = 1 << 0,
  START_LIST = 1 << 1,
  START_PRETEXT = 1 << 2
};

void addHeader(std::string headerString, int headerLevel);
void addHyperLink(std::string URI_adrres, std::string linkText);
void addQuote(std::string textQuote);
void addPreformatedText(std::string inputText);
void addPlainText(std::string inputText);
void addStartQuote();
void addEndQuote();
void addStartList();
void addEndList();
void addListElement(std::string ListElement);
void addLists(std::vector<std::string> ListElements);
void addStartPreText();
void addEndPreText();
void addPreText(std::string);

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

void parseGmiString(std::string gmiString) {
  std::string command = gmiString.substr(0, gmiString.find(" "));
  std::string lastString =
      gmiString.substr(gmiString.find(" ") + 1, gmiString.size());

  // если в пердыдущей строке была команда начала цитаты, а в текущей нет
  // продолжения, то втсавляем знак конца блока цитаты и штатно обрабатываем
  // строку далее
  if (((startFlag & START_QUOTE) != 0) && command != ">") {
    addEndQuote();
    startFlag &= ~START_QUOTE;
  }

  // если в пердыдущей строке была команда начала списка, а в текущей нет
  // продолжения, то втсавляем знак конца списка и штатно обрабатываем строку
  // далее
  if (((startFlag & START_LIST) != 0) && command != "*") {
    addEndList();
    startFlag &= ~START_LIST;
  }

  // конец преформатированный текст закончится только тогда когда встретится второй знак ```
  //
  if ((startFlag & START_PRETEXT) != 0){ 
        if (command == "```"){
            addEndPreText();
            startFlag &= ~START_PRETEXT;
            return;
        }else{
            addPreText(command + " " + lastString);
            return;
        }
    }

// команда заголовка первого уровня
  if (command == "#") {
    addHeader(lastString, 1);
  }

// команда заголовка второго уровня
  else if (command == "##") {
    addHeader(lastString, 2);
  }
// команда заголовка третьего уровня
  else if (command == "###") {
    addHeader(lastString, 3);
  }

// команда url-ссылки
  else if (command == "=>") {
    std::string URIString = lastString.substr(0, lastString.find(" "));
    std::string linkString =
        lastString.substr(lastString.find(" "), lastString.size());
    addHyperLink(URIString, linkString);
  }

// команда цитаты
  else if (command == ">") {
    if ((startFlag & START_QUOTE) == 0) {
      addStartQuote();
      startFlag |= START_QUOTE;
    }
    addPlainText(lastString);
  }

// команда списка
  else if (command == "*") {
    if ((startFlag & START_LIST) == 0) {
      addStartList();
      startFlag |= START_LIST;
    }
    addListElement(lastString);
  }

// команда преформатированного текса
  else if (command == "```") {
    if ((startFlag & START_PRETEXT) == 0) {
      addStartPreText();
      startFlag |= START_PRETEXT;
    }
    //addPreText(lastString);
  }

  // если никакой команды не опознано, то текст вставлям как есть.
  else {
    addPlainText(command + " " + lastString);
    ;
  }
}

/** Методо добавляет в html документ строку-заголовок
 * @param headerString - текст заголовка
 * @param headerLevel - уровыень заголовка (1..3)
 */
// enum headerLevel{0, H1, H2, H3}
void addHeader(std::string headerString, int headerLevel) {
  std::string stringResult;
  if (headerLevel < 1 || headerLevel > 3) {
    addPlainText(headerString);
  }

  stringResult += "<h" + std::to_string(headerLevel) + ">" + // start tag
                  headerString +                             // text
                  "</h" + std::to_string(headerLevel) + ">"; // end tag
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ URL-ссылку
 * @param URI_adrres - URL адрес
 * @param linkText - текст в html-документе 
 */
void addHyperLink(std::string URI_adrres, std::string linkText) {
  std::string stringResult;
  stringResult = "<a href = \"" + URI_adrres + "\">" + // start tag
                 linkText +                            // text
                 "</a>";                               // end tag
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг начала цитаты
 */
void addStartQuote() {
  std::string stringResult;
  stringResult = "<blockqoute>";
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг конца цитаты
 */
void addEndQuote() {
  std::string stringResult;
  stringResult = "</blockqoute>";
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг начала списка
 */
void addStartList() {
  std::string stringResult;
  stringResult = "<ul>"; // start tag
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг конца списка
 */
void addEndList() {
  std::string stringResult;
  stringResult = "</ul>"; // start tag
  htmlDoc.push_back(stringResult);
}
/** Методо добавляет в список элемент списка
 * @param ListElement - текст жлемента списка
 */
void addListElement(std::string ListElement) {
    if((startFlag & START_LIST)==0){
        std::cerr << "WARNING!!! Start list absent!!!";
    }
  std::string stringResult;
  stringResult = "<li>" + ListElement + "</li>";
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в список html документ элементов
 * @param ListElementы - вектор строк элемнетов списка
 */
void addLists(std::vector<std::string> ListElements) {
  std::string stringResult;
  stringResult = "<ul>"; // start tag
  for (auto const &elList : ListElements) {
    stringResult += "<li>" + elList + "</li>";
  }
  stringResult += "</ul>"; // end tag
  htmlDoc.push_back(stringResult);
}


/**  Методо добавляет в html документ тэг начала перформатированного текста
 */
void addStartPreText() {
    std::string stringResult;
    stringResult += "<pre>"; // start tag
    htmlDoc.push_back(stringResult);  
}

/**  Методо добавляет в html документ тэг конца перформатированного текста
 */
void addEndPreText() {
    std::string stringResult;
    stringResult += "</pre>"; // start tag
    htmlDoc.push_back(stringResult);  
}


/** Методо добавляет в список html документ перформатированный текст
 * @param inputText - текст который должен быть выведен
 */
void addPreText(std::string inputText) {
    std::string stringResult;
    stringResult += inputText + "\n";                             // text
    htmlDoc.push_back(stringResult);  
}


/** Методо добавляет в список html документ элементов
 * @param ListElements - вектор строк элемнетов списка
 */
void addPlainText(std::string inputText) { htmlDoc.push_back(inputText); }

int main() {
  //   auto res =
  //   recursiveGetFileNamesByExtension("c:/program1/ProffesionCPlusplus",
  //   ".cpp"); for (auto f_name=res->begin();f_name!=res->end();++f_name)
  //       std::cout << *f_name << " " << std::endl;

  std::cout << std::endl;

  parseGmiString("# Heder 1");
  parseGmiString("## Heder 2");
  parseGmiString("### Heder 3");
  parseGmiString("* List Element");
  parseGmiString("> Quote 1");
  parseGmiString("> Quote 1");
  parseGmiString("> Quote 1");
  parseGmiString("> Quote 1");
  parseGmiString("=> http://some-address.com Link 2");
  parseGmiString("=> http://some-address.com Link");
  parseGmiString("``` http://some-address.com Link");
  parseGmiString("=> http://some-address.com Link");
  parseGmiString("=> http://some-address.com Link");
  parseGmiString("=> http://some-address.com Link");
  parseGmiString("```");

  parseGmiString("> Quote 1");
  parseGmiString("> Quote 1");
  parseGmiString("> Quote 1");
  parseGmiString("> Quote 1");
  parseGmiString(" ");


  for (const auto &htmlString : htmlDoc) {
    std::cout << htmlString << std::endl;
  }

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