#include "include/CampusGraph.h"

int main() {

    CampusGraph campus;

    // Load input files
    campus.loadBuildings("data/buildings.txt");
    campus.loadConnections("data/connections.txt");

    int choice;

    do {

        cout << "\n===== CAMPUS NAVIGATION =====\n";

        cout << "1. Display Buildings\n";
        cout << "2. BFS Traversal\n";
        cout << "3. Find Shortest Path\n";
        cout << "4. Connectivity Report\n";
        cout << "0. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        string startName;
        string endName;

        switch (choice) {

            case 1:
                campus.displayBuildings();
                break;

            case 2:
                cout << "Enter starting building Name: ";
                cin >> startName;

                campus.BFS(startName);
                break;

            case 3:
                cout << "Enter starting building Name: ";
                cin >> startName;

                cout << "Enter destination building Name: ";
                cin >> endName;

                campus.shortestPath(startName, endName);
                break;

            case 4:
                campus.connectivityReport();
                break;

            case 0:
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}