#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CampusGraph {

private:

    // Information about one building
    struct Building {
        
        string name;
        vector<int> neighbors;
    };

    // Stores all campus buildings
    vector<Building> buildings;

    // Finds a building's position using its ID
    int findBuildingIndex(string id);


public:

    // Add a new building
    void addBuilding(string name);

    // Connect two buildings
    void addConnection(string id1, string id2);

    // Load information from text files
    void loadBuildings(string filename);
    void loadConnections(string filename);

    // Display buildings and their connections
    void displayBuildings();
    vector<string> getBuildingNames() const;

    // Find all reachable buildings using BFS
    void BFS(string startID);

    // Find the shortest path between two buildings
    void shortestPath(string startID, string endID);

    // Display isolated buildings and connected components
    void connectivityReport();

};

#endif