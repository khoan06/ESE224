#ifndef CAMPUS_NAVIGATOR_H
#define CAMPUS_NAVIGATOR_H

#include <string>
#include <vector>
#include <map>

// POI info
struct POI {
    std::string name, category, description;
};

// Campus node
struct Location {
    std::string name;
    std::vector<int> neighbors; // edges
    int poiIndex;               // -1 if none
};

class CampusNavigator {
private:
    std::vector<Location> locations;
    std::vector<POI> pois;
    std::map<std::string,int> locNameToIndex; // lower -> index

    std::string toLower(const std::string& s);
    void hardcodedMapInit(); // build data in code

public:
    CampusNavigator();
    bool loadMap(); // reload hardcoded data
    void searchLocation(const std::string& keyword);
    void findPath(const std::string& start, const std::string& end);
    void browsePOI(const std::string& category);
    void showPOIDetails(const std::string& name);
    void printHelp();
};

#endif