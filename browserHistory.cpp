#include <iostream>
#include <string>
#include <list>

using namespace std;

class BrowserHistory {
    public:
        BrowserHistory();
        void goBack();
        void goForward();
        void addNewPage(const string&);
        void removeCurrentPage();
        void removePageByPosition(int);
        void printCurrent();
        void printHistory();
        void printOptions();

    private:
        list<string> history;
        list<string>::iterator current;
};


BrowserHistory::BrowserHistory() {
    history.push_back("HomePage"); 
    current = history.begin();
}

void BrowserHistory::goBack() {
    if (current == history.begin())
        cout << "Already at the beginning.\n";
    else
        --current;
}

void BrowserHistory::goForward() {
    auto next = current;
    ++next;
    if(next == history.end())
        cout << "Already at the latest page.\n";
    else
        current = next;
}

void BrowserHistory::addNewPage(const string& page) {
     auto eraseFrom = current;
     eraseFrom++;
     history.erase(eraseFrom, history.end());
     history.push_back(page);
     ++current;
     cout << "Value '" << page << " added.\n";
}

void BrowserHistory::removeCurrentPage(){
    if (current == history.begin())
        cout << "Can not remove first page.\n";
    auto toErase = current;
    --current;
    history.erase(toErase);
}

void BrowserHistory::removePageByPosition(int pos){
    if (pos == 0)
        cout << "Can not remove first page.\n";
    if (pos < 0 || pos >= history.size()) {
        cout << "Invalid position.\n";
    }

    auto it = history.begin();
    int index = 0;

    while (index++ < pos && it++ != history.end());

    if (it == current) {
        auto toErase = current--;
        history.erase(toErase);
    }
    else
        history.erase(it);
}

void BrowserHistory::printCurrent(){
    int index = 0;
    for (auto it = history.begin() ; it != history.end() ; ++it, ++index) {
        if (it == current) {
            cout << "Current: " << *it <<"(" << index << ")\n";
            return;
        }
    }
}

void BrowserHistory::printHistory(){
    cout << "History:\n";
    int index = 0;
    for (auto it = history.begin() ; it != history.end() ; ++it, ++index) {
        cout << *it << "(" << index << ")";
        if (next(it) != history.end())
            cout << "->";
    }
    cout << endl;
}

void BrowserHistory::printOptions(){
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
  BrowserHistory browser;
  browser.printOptions();
  int option;

  while(true)
  {
        cout << "--------------------------------------------------\n";
        cout << "Pick an option: ";
        cin >> option;

        switch (option) {
            case 1:
                browser.goBack();
                break;

            case 2:
                browser.goForward();
                break;

            case 3: {
                cout << "Input the value: ";
                string page;
                cin.ignore();
                getline(cin, page);
                browser.addNewPage(page);
                break;
                    } 
            case 4:
                browser.removeCurrentPage();
                break;

            case 5: {
                cout << "Position: ";
                int pos;
                cin >> pos;
                browser.removePageByPosition(pos);
                break;
                    }
            case 6:
                browser.printCurrent();
                break;

            case 7:
                browser.printHistory();
                break;

            case 8:
                cout << "Closing...\n";
                return 0;

            case 9:
                browser.printOptions();
                break;

            default:
                cout << "Invalid option. Try again.\n";
        }
  }

  return 0;
}
