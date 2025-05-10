#include <iostream>
#include <list>

using namespace std;

class browserHistory {
    private:
        list<string> history;
        list<string>::iterator current;
    public:
        browserHistory();
        void goBack();
        void goForward();
        void addNewPage(string& newPage);
        void removeCurrentPage();
        void removePageUsingPosition(int pos);
        void printCurrentPage() const;
        void printHistory() const;
        void printOptions() const;
        void quit();
        void help();
};

browserHistory::browserHistory() {
    history.push_back("HomePage");
    current = history.begin();
}

void browserHistory::goBack() {
    if (current == history.begin()) {
        cout << "Already at the first page.\n";
        return;
    }
    --current;
}

void browserHistory::goForward() {
    auto next = current;
    next++;
    if (next == history.end())
        cout << "Already at the latest page.\n";
    ++current;
}

void browserHistory::addNewPage(string& newPage) {
    auto eraseFrom = current; 
    ++eraseFrom;
    history.erase(eraseFrom, history.end());
    history.push_back(newPage);
    ++current;
    cout << "Value '" << newPage << " added.\n";
}

void browserHistory::removeCurrentPage() {
    if (current == history.begin()) {
        cout << "Cannot remove the first page.\n";
        return;
    }
    auto toErase = current;
    --current;
    history.erase(toErase);
}

void browserHistory::removePageUsingPosition(int pos) {
    if (pos == 0) {
        cout << "Cannot remove the first page.\n";
        return; 
    }
    if (pos < 0 || pos >= history.size()) {
        cout << "Out of the range.\n";
        return; 
    }

    //  0     1       2       3 
    // home google youtube facebook
    
    auto index = 0;
    auto toErase = history.begin();
    while (index < pos) {
        ++toErase;
        ++index;
    }
    auto temp = toErase;
    current = --temp;
    history.erase(toErase, history.end());
}

void browserHistory::printCurrentPage() const {
    int index = 0; 
    auto temp = history.begin();
    while (temp++ != current) 
        ++index;
    cout << "Current: " << *current << "(" << index <<")\n"; 
}

void browserHistory::printHistory() const {
    cout << "History:\n";
    int index = 0;
    for (auto it = history.begin() ; it != history.end() ; ++it, ++index){
        cout << *it << "(" << index << ")";
        if (index != history.size() - 1)
          cout << "->";
    }
    cout << endl;
}

void browserHistory::printOptions() const {
    cout << "Options:\n";
    cout << "1. Go back\n";
    cout << "2. Go forward\n";
    cout << "3. Add new page\n";
    cout << "4. Remove current page\n";
    cout << "5. Remove page using position\n";
    cout << "6. Print current\n";
    cout << "7. Print history\n";
    cout << "8. Quit\n";
    cout << "9. Print options\n";
}


int main() {
  browserHistory broHis;
  int option;
  int position;
  string newPage;
  bool radi = true;

  broHis.printOptions(); 
  while (radi) {
      cout << "Pick an option: ";
      cin >> option;
      cout << "-------------------------------------\n";
      switch (option) { 
          case 1: 
              broHis.goBack(); 
              continue;
          case 2:
              broHis.goForward();
              continue;
          case 3:
              cout << "Input the value: ";
              cin >> newPage;
              broHis.addNewPage(newPage);
              continue;
          case 4:
              broHis.removeCurrentPage();
              continue;
          case 5:
              cout << "Position: ";
              cin >> position;
              broHis.removePageUsingPosition(position);
              continue;
          case 6:
              broHis.printCurrentPage();
              continue;
          case 7:
              broHis.printHistory();
              continue;
          case 8:
              radi = false;
              break;
          case 9:
              broHis.printOptions();
              continue;
          default:
              cout << "Wrong option number! \n";
      } 

  }
}
