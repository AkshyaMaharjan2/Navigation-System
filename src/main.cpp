
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

    // Load facilities only after the accepted building list is available.
    FacilityManager facilityManager;
    facilityManager.loadFromFile("data/facilities.txt", campus.getBuildingNames());
  
  
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

        

        cout << "1. Display All Facilities\n";
        cout << "2. Search Facility by ID\n";
        cout << "3. Search Facility by Name\n";
        cout << "4. Search Facility by Category\n";
        cout << "5. Display Facility Category Report\n";

      
         cout << "\n===== CAMPUS NAVIGATION =====\n";

        cout << "6. Display Buildings\n";
        cout << "7. BFS Traversal\n";
        cout << "8. Find Shortest Path\n";
        cout << "9. Connectivity Report\n";

        cout << "\n--- Department Tree Management ---" << endl;
        cout << "10. Add Department" << endl;
        cout << "11. Search Department" << endl;
        cout << "12. Display Department Hierarchy" << endl;
        cout << "13. Preorder Traversal" << endl;
        cout << "14. Postorder Traversal" << endl;
        cout << "15. Level Order Traversal" << endl;
        cout << "16. Department Statistics" << endl;

        cout << "\n--- Facility Management ---" << endl;
        cout << "17. View All Facilities" << endl;
        cout << "18. Search Facility by ID" << endl;
        cout << "19. Search Facility by Name" << endl;
        cout << "20. Search Facility by Category" << endl;
        cout << "21. Facility Category Report" << endl;
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
            facilityManager.displayAllFacilities();
            break;

            case 2: {
                string id;
                cout << "Enter Facility ID: ";
                cin >> id;

                facilityManager.searchByID(id);
                break;
            }

            case 3: {
                string name;
                cin.ignore(10000, '\n');

                cout << "Enter Facility Name: ";
                getline(cin, name);

                facilityManager.searchByName(name);
                break;
            }

            case 4: {
                string category;
                cin.ignore(10000, '\n');

                cout << "Enter Facility Category: ";
                getline(cin, category);

                facilityManager.searchByCategory(category);
                break;
            }

            case 5:
                facilityManager.displayCategoryReport();
                break;

            case 6:
                campus.displayBuildings();
                break;

            case 7:
                cout << "Enter starting building Name: ";
                cin >> startName;

                campus.BFS(startName);
                break;

            case 8:
                cout << "Enter starting building Name: ";
                cin >> startName;

                cout << "Enter destination building Name: ";
                cin >> endName;

                campus.shortestPath(startName, endName);
                break;

            case 9:
                campus.connectivityReport();
                break;
            // Add Department
            case 10:
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
            case 11:
            {
                string departmentName;

                cout << "Enter department name to search: ";
                getline(cin >> ws, departmentName);

                departmentTree.searchDepartment(departmentName);

                break;
            }

            // Display Hierarchy
            case 12:
                departmentTree.displayHierarchy();
                break;

            // Preorder Traversal
            case 13:
                departmentTree.preorderTraversal();
                break;

            // Postorder Traversal
            case 14:
                departmentTree.postorderTraversal();
                break;

            // Level Order Traversal
            case 15:
                departmentTree.levelOrderTraversal();
                break;

            // Department Statistics
            case 16:
                departmentTree.displayStatistics();
                break;

            case 17:
                facilityManager.displayAllFacilities();
                break;

            case 18:
            {
                string facilityID;
                cout << "Enter facility ID: ";
                getline(cin >> ws, facilityID);
                facilityManager.searchByID(facilityID);
                break;
            }

            case 19:
            {
                string facilityName;
                cout << "Enter facility name: ";
                getline(cin >> ws, facilityName);
                facilityManager.searchByName(facilityName);
                break;
            }

            case 20:
            {
                string category;
                cout << "Enter facility category: ";
                getline(cin >> ws, category);
                facilityManager.searchByCategory(category);
                break;
            }

            case 21:
                facilityManager.displayCategoryReport();
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
