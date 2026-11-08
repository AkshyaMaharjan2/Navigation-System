
#include <iostream>
#include <string>


#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "Facility.h"
#include "HashTable.h"

using namespace std;

int main()
{
  
    CampusGraph campus;

    // Load input files
    campus.loadBuildings("data/buildings.txt");
    campus.loadConnections("data/connections.txt");
  
  
    // Create Department Tree object
    DepartmentTree departmentTree;

    // Load departments from file
    cout << "Loading department information..." << endl;

    if (departmentTree.loadFromFile("data/departments.txt"))
    {
        cout << "Departments loaded successfully." << endl;
    }
    else
    {
        cout << "Error: Could not load departments file." << endl;
    }

    int choice = 0;

    // Main menu
    do
    {
        cout << "\n========================================" << endl;
        cout << "       SMART CAMPUS NAVIGATION SYSTEM" << endl;
        cout << "========================================" << endl;
      
         cout << "\n===== CAMPUS NAVIGATION =====\n";

        cout << "1. Display Buildings\n";
        cout << "2. BFS Traversal\n";
        cout << "3. Find Shortest Path\n";
        cout << "4. Connectivity Report\n";

        cout << "\n--- Department Tree Management ---" << endl;
        cout << "5. Add Department" << endl;
        cout << "6. Search Department" << endl;
        cout << "7. Display Department Hierarchy" << endl;
        cout << "8. Preorder Traversal" << endl;
        cout << "9. Postorder Traversal" << endl;
        cout << "10. Level Order Traversal" << endl;
        cout << "11. Department Statistics" << endl;
        cout << "0. Exit" << endl;

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
      
      
        string startName;
        string endName;

        switch (choice)
        {
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
            // Add Department
            case 5:
            {
                string departmentName;
                string parentName;

                cout << "Enter department name: ";
                getline(cin >> ws, departmentName);

                if (departmentTree.isEmpty())
                {
                    departmentTree.addDepartment(departmentName);
                }
                else
                {
                    cout << "Enter parent department name: ";
                    getline(cin, parentName);

                    departmentTree.addDepartment(
                        departmentName, parentName
                    );
                }

                break;
            }

            // Search Department
            case 6:
            {
                string departmentName;

                cout << "Enter department name to search: ";
                getline(cin >> ws, departmentName);

                departmentTree.searchDepartment(departmentName);

                break;
            }

            // Display Hierarchy
            case 7:
                departmentTree.displayHierarchy();
                break;

            // Preorder Traversal
            case 8:
                departmentTree.preorderTraversal();
                break;

            // Postorder Traversal
            case 9:
                departmentTree.postorderTraversal();
                break;

            // Level Order Traversal
            case 10:
                departmentTree.levelOrderTraversal();
                break;

            // Department Statistics
            case 11:
                departmentTree.displayStatistics();
                break;

            // Exit
            case 0:
                cout << "Thank you for using the program!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}
