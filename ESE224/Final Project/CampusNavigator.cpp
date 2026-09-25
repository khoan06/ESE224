#include "CampusNavigator.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// lowercase helper
string CampusNavigator::toLower(const string& s) {
    string r = s;
    for (char& c : r) c = tolower(c);
    return r;
}

// hardcode map/paths/POIs
void CampusNavigator::hardcodedMapInit() {
    locations.clear(); pois.clear(); locNameToIndex.clear();

    // locations
    vector<string> names = {
        "MainGate","Library","ScienceBuilding","LectureHall1","CafeteriaA",
        "ParkingLotEntrance","SportsComplex","AdminOffice","StudentDorms","EngineeringAnnex"
    };
    for (auto& n : names) {
        locations.push_back({n, {}, -1});
        locNameToIndex[toLower(n)] = (int)locations.size()-1;
    }

    // connect helper
    auto connect = [&](const string& a, const string& b){
        int ia = locNameToIndex[toLower(a)];
        int ib = locNameToIndex[toLower(b)];
        locations[ia].neighbors.push_back(ib);
        locations[ib].neighbors.push_back(ia);
    };

    // paths (undirected)
    connect("MainGate","Library");
    connect("Library","ScienceBuilding");
    connect("ScienceBuilding","LectureHall1");
    connect("LectureHall1","CafeteriaA");
    connect("CafeteriaA","ParkingLotEntrance");
    connect("MainGate","ParkingLotEntrance");
    connect("Library","StudentDorms");
    connect("StudentDorms","SportsComplex");
    connect("SportsComplex","AdminOffice");
    connect("AdminOffice","EngineeringAnnex");
    connect("EngineeringAnnex","ScienceBuilding");

    // POIs
    vector<POI> init = {
        {"Library","Academic","Central resource for books and study."},
        {"CafeteriaA","Dining","Offers a variety of food options."},
        {"LectureHall1","Academic","Large hall for lectures."},
        {"SportsComplex","Recreational","Gym, pool, and sports fields."},
        {"AdminOffice","Administrative","University administration and registrar."}
    };
    for (int i = 0; i < (int)init.size(); ++i) {
        pois.push_back(init[i]);
        string k = toLower(init[i].name);
        if (locNameToIndex.count(k)) locations[locNameToIndex[k]].poiIndex = i;
    }

    cout << "Map loaded (" << locations.size() << " locations, "
         << pois.size() << " POIs).\n";
}

CampusNavigator::CampusNavigator() { hardcodedMapInit(); }
bool CampusNavigator::loadMap() { hardcodedMapInit(); return true; }

// search locations (partial, case-insensitive)
void CampusNavigator::searchLocation(const string& keyword) {
    string key = toLower(keyword);
    vector<int> hits;
    for (int i = 0; i < (int)locations.size(); ++i)
        if (toLower(locations[i].name).find(key) != string::npos) hits.push_back(i);

    if (hits.empty()) {
        cout << "No locations found matching '" << keyword << "'.\n";
        return;
    }
    cout << "Found " << hits.size() << " location(s):\n";
    for (int i : hits) {
        cout << "- " << locations[i].name;
        if (locations[i].poiIndex != -1) {
            POI& p = pois[locations[i].poiIndex];
            cout << " (" << p.category << ": " << p.description << ")";
        }
        cout << "\n";
    }
}

// shortest path (BFS)
void CampusNavigator::findPath(const string& start, const string& end) {
    auto itS = locNameToIndex.find(toLower(start));
    auto itT = locNameToIndex.find(toLower(end));
    if (itS == locNameToIndex.end() || itT == locNameToIndex.end()) {
        cout << "Invalid location name(s).\n"; return;
    }
    int s = itS->second, t = itT->second;
    vector<int> prev(locations.size(), -1);
    vector<char> vis(locations.size(), 0);
    queue<int> q; q.push(s); vis[s] = 1;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == t) break;
        for (int v : locations[u].neighbors) if (!vis[v]) {
            vis[v] = 1; prev[v] = u; q.push(v);
        }
    }
    if (!vis[t]) { cout << "No path found between " << start << " and " << end << ".\n"; return; }

    vector<int> path;
    for (int cur = t; cur != -1; cur = prev[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());

    cout << "Shortest path from " << locations[s].name << " to " << locations[t].name << ":\n";
    for (int i = 0; i < (int)path.size(); ++i) {
        cout << locations[path[i]].name << (i+1<(int)path.size() ? " -> " : "\n");
    }
}

// list POIs by category
void CampusNavigator::browsePOI(const string& category) {
    string cat = toLower(category);
    vector<int> hits;
    for (int i = 0; i < (int)pois.size(); ++i)
        if (toLower(pois[i].category) == cat) hits.push_back(i);

    if (hits.empty()) { cout << "No POIs in '" << category << "'.\n"; return; }
    cout << "POIs in '" << category << "':\n";
    for (int i : hits) cout << "- " << pois[i].name << ": " << pois[i].description << "\n";
}

// POI details by name
void CampusNavigator::showPOIDetails(const string& name) {
    string k = toLower(name);
    for (auto& p : pois) if (toLower(p.name) == k) {
        cout << "POI Details\nName: " << p.name
             << "\nCategory: " << p.category
             << "\nDescription: " << p.description << "\n";
        return;
    }
    cout << "POI '" << name << "' not found.\n";
}

// help text
void CampusNavigator::printHelp() {
    cout << "Commands:\n"
         << "  load_map\n"
         << "  search <name_part>\n"
         << "  find_path <start> <end>\n"
         << "  browse_poi <category>\n"
         << "  show_poi_details <name>\n"
         << "  help\n"
         << "  exit\n";
}