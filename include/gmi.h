#pragma once
#include <vector>
#include <filesystem>
#include <iostream>
#include <string>

class Gmi{

private:
  std::vector<std::string> htmlDoc;
  std::vector<std::string> GmiDoc;
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

public:
  void print();
  void toHtml();
  void parseGmiString(std::string gmiString);
};