#include "gmi.h"

void Gmi::parseGmiString(std::string gmiString) {
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
void Gmi::addHeader(std::string headerString, int headerLevel) {
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
void Gmi::addHyperLink(std::string URI_adrres, std::string linkText) {
  std::string stringResult;
  stringResult = "<a href = \"" + URI_adrres + "\">" + // start tag
                 linkText +                            // text
                 "</a>";                               // end tag
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг начала цитаты
 */
void Gmi::addStartQuote() {
  std::string stringResult;
  stringResult = "<blockqoute>";
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг конца цитаты
 */
void Gmi::addEndQuote() {
  std::string stringResult;
  stringResult = "</blockqoute>";
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг начала списка
 */
void Gmi::addStartList() {
  std::string stringResult;
  stringResult = "<ul>"; // start tag
  htmlDoc.push_back(stringResult);
}

/** Методо добавляет в html документ тэг конца списка
 */
void Gmi::addEndList() {
  std::string stringResult;
  stringResult = "</ul>"; // start tag
  htmlDoc.push_back(stringResult);
}
/** Методо добавляет в список элемент списка
 * @param ListElement - текст жлемента списка
 */
void Gmi::addListElement(std::string ListElement) {
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
void Gmi::addLists(std::vector<std::string> ListElements) {
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
void Gmi::addStartPreText() {
    std::string stringResult;
    stringResult += "<pre>"; // start tag
    htmlDoc.push_back(stringResult);  
}

/**  Методо добавляет в html документ тэг конца перформатированного текста
 */
void Gmi::addEndPreText() {
    std::string stringResult;
    stringResult += "</pre>"; // start tag
    htmlDoc.push_back(stringResult);  
}


/** Методо добавляет в список html документ перформатированный текст
 * @param inputText - текст который должен быть выведен
 */
void Gmi::addPreText(std::string inputText) {
    std::string stringResult;
    stringResult += inputText + "<br>";                             // text
    htmlDoc.push_back(stringResult);  
}


/** Метод добавляет в список html документ элементов
 * @param ListElements - вектор строк элемнетов списка
 */
void Gmi::addPlainText(std::string inputText) { htmlDoc.push_back(inputText); }

std::string Gmi::toHtml(){
    std::string resultString;
    htmlDoc.clear(); //очистим на всякий случай
    htmlDoc.push_back("<HTML>");//начало html 
    htmlDoc.push_back("<BODY>"); 
        for (const auto &gmiStr:gmiDoc){
            parseGmiString(gmiStr);
        }    
    htmlDoc.push_back("</BODY>");
    htmlDoc.push_back("</HTML>"); // Завершение html
// преобразуем всё это в строку и выведем
    for (const auto &htmlStr : htmlDoc){
        resultString += htmlStr + "\n";
    }
    return resultString;
}

std::ostream& operator<<(std::ostream &out, const Gmi &gmi)
{
    for (const auto &item:gmi.gmiDoc){
    out << item << "\n";
    }
    return out;
} 

void operator>>(std::istream &in, Gmi &gmi){
    std::string temp;
    while (!in.eof()){
        std::getline(in, temp);
        gmi.gmiDoc.push_back(temp);
    }
    std::cout << temp;
 }