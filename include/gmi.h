#pragma once
#include <vector>
#include <iostream>
#include <string>

class Gmi{

private:
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
    void parseGmiString(std::string gmiString);

public:
    std::string toHtml();
    
    friend std::ostream& operator<<(std::ostream &out, const Gmi &gmi);
    friend void operator>>(std::istream &in, Gmi &gmi);

};