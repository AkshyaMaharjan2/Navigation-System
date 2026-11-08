// libraries
#include "../../include/CampusGraph.h"

#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>


int CampusGraph::findBuildingIndex(string name) {

    for (int i = 0; i < buildings.size(); i++) {

        if (buildings[i].name == name) {
            return i;
        }
    }

    // Return -1 if the building does not exist
    return -1;
}


void CampusGraph::addBuilding(string name) {

    // Avoid adding duplicate buildings
    if (findBuildingIndex(name) != -1) {
        cout << "Building already exists: " << name << endl;
        return;
    }

    Building newBuilding;

    newBuilding.name = name;

    buildings.push_back(newBuilding);
}


void CampusGraph::addConnection(string name1, string name2) {

    int index1 = findBuildingIndex(name1);
    int index2 = findBuildingIndex(name2);

    if (index1 == -1 || index2 == -1) {
        cout << "Invalid building Name!" << endl;
        return;
    }

    // Avoid duplicate connections
    if (find(buildings[index1].neighbors.begin(),
             buildings[index1].neighbors.end(),
             index2) != buildings[index1].neighbors.end()) {
        return;
    }

    // Connect both buildings
    buildings[index1].neighbors.push_back(index2);
    buildings[index2].neighbors.push_back(index1);
}




// Loading building from file
void CampusGraph::loadBuildings(string filename) {

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open buildings file!" << endl;
        return;
    }

    string line;
    int count = 0;

    while (getline(file, line)) {

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Remove Windows line ending if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Use the building name as both ID and name
        addBuilding(line);

        count++;
    }

    file.close();

    cout << count << " buildings loaded successfully!" << endl;
}

// loading connection from file
void CampusGraph::loadConnections(string filename) {

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open connections file!" << endl;
        return;
    }

    string line;
    int count = 0;

    while (getline(file, line)) {

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        stringstream ss(line);

        string buildingA;
        string buildingB;

        // Split using pipe delimiter
        getline(ss, buildingA, '|');
        getline(ss, buildingB);

        if (!buildingA.empty() && !buildingB.empty()) {

            addConnection(buildingA, buildingB);

            count++;
        }
    }

    file.close();

    cout << count << " connections loaded successfully!" << endl;
}

// Displaying Buildings
void CampusGraph::displayBuildings() {

    cout << "\n===== CAMPUS BUILDINGS =====\n";

    for (int i = 0; i < buildings.size(); i++) {

        cout << "\nBuilding: " << buildings[i].name;
        cout << " (" << buildings[i].name << ")";

        cout << "\nConnected to: ";

        if (buildings[i].neighbors.empty()) {
            cout << "No connections";
        }

        else {

            for (int neighbor : buildings[i].neighbors) {

                cout << buildings[neighbor].name << " ";
            }
        }

        cout << endl;
    }
}

// Breadth First Search
void CampusGraph::BFS(string startID) {

    int start = findBuildingIndex(startID);

    if (start == -1) {
        cout << "Starting building not found!" << endl;
        return;
    }

    // Keep track of visited buildings
    vector<bool> visited(buildings.size(), false);

    // Queue stores buildings waiting to be visited
    queue<int> q;

    // Start from the selected building
    visited[start] = true;
    q.push(start);

    cout << "\nBFS Traversal:\n";

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        cout << buildings[current].name << endl;

        // Explore all neighbors
        for (int neighbor : buildings[current].neighbors) {

            if (!visited[neighbor]) {

                visited[neighbor] = true;

                q.push(neighbor);
            }
        }
    }
}


// Shortest Path
void CampusGraph::shortestPath(string startName, string endName) {

    int start = findBuildingIndex(startName);
    int destination = findBuildingIndex(endName);

    if (start == -1 || destination == -1) {
        cout << "Invalid building Name!" << endl;
        return;
    }

    vector<bool> visited(buildings.size(), false);

    // Stores the previous building in the route
    vector<int> parent(buildings.size(), -1);

    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        if (current == destination) {
            break;
        }

        for (int neighbor : buildings[current].neighbors) {

            if (!visited[neighbor]) {

                visited[neighbor] = true;

                parent[neighbor] = current;

                q.push(neighbor);
            }
        }
    }

    // Check if destination was reached
    if (!visited[destination]) {
        cout << "No path exists between these buildings.\n";
        return;
    }

    // Reconstruct the path
    vector<int> path;

    int current = destination;

    while (current != -1) {

        path.push_back(current);

        current = parent[current];
    }

    // Reverse the path to get start -> destination
    reverse(path.begin(), path.end());

    cout << "\nShortest Path:\n";

    for (int index : path) {

        cout << buildings[index].name;

        if (index != destination) {
            cout << " -> ";
        }
    }

    cout << "\nTotal connections: " << path.size() - 1 << endl;
}





// COnnectivity Report

void CampusGraph::connectivityReport() {

    vector<bool> visited(buildings.size(), false);

    int components = 0;

    cout << "\n===== CONNECTIVITY REPORT =====\n";

    // Find isolated buildings
    cout << "\nIsolated Buildings:\n";

    for (int i = 0; i < buildings.size(); i++) {

        if (buildings[i].neighbors.empty()) {
            cout << buildings[i].name << endl;
        }
    }

    // Count connected components
    for (int i = 0; i < buildings.size(); i++) {

        if (!visited[i]) {

            components++;

            queue<int> q;

            visited[i] = true;
            q.push(i);

            while (!q.empty()) {

                int current = q.front();
                q.pop();

                for (int neighbor : buildings[current].neighbors) {

                    if (!visited[neighbor]) {

                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    cout << "\nTotal Connected Components: "
         << components << endl;
}