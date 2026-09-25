#include "CampusNavigator.h"
#include <iostream>
using namespace std;

int main() {
    CampusNavigator nav;
    string cmd;

    cout << "Welcome to Campus Navigator! Type 'help' for commands.\n";

    while (true) {
        cout << "\nEnter command: ";
        getline(cin, cmd);
        while (!cmd.empty() && cmd.back() == ' ') cmd.pop_back();

        if (cmd == "help") nav.printHelp();
        else if (cmd == "exit") break;
        else if (cmd == "load_map") nav.loadMap();

        else if (cmd.rfind("search ", 0) == 0) {
            string kw = cmd.substr(7);
            if (!kw.empty()) nav.searchLocation(kw);
            else cout << "Usage: search <name_part>\n";
        }
        else if (cmd.rfind("find_path ", 0) == 0) {
            // split on first space after "find_path "
            size_t p = cmd.find(' ', 10);
            if (p != string::npos) {
                string a = cmd.substr(10, p-10);
                string b = cmd.substr(p+1);
                if (!a.empty() && !b.empty()) nav.findPath(a, b);
                else cout << "Usage: find_path <start> <end>\n";
            } else cout << "Usage: find_path <start> <end>\n";
        }
        else if (cmd.rfind("browse_poi ", 0) == 0) {
            string cat = cmd.substr(11);
            if (!cat.empty()) nav.browsePOI(cat);
            else cout << "Usage: browse_poi <category>\n";
        }
        else if (cmd.rfind("show_poi_details ", 0) == 0) {
            string name = cmd.substr(17);
            if (!name.empty()) nav.showPOIDetails(name);
            else cout << "Usage: show_poi_details <name>\n";
        }
        else if (cmd.empty()) continue;
        else cout << "Unknown command. Type 'help'.\n";
    }

    cout << "Goodbye!\n";
}