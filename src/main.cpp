
#include <iostream>
#include <string>
#include "DepartmentTree.h"

using namespace std;

int main()
{
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

        cout << "\n--- Department Tree Management ---" << endl;
        cout << "9. Add Department" << endl;
        cout << "10. Search Department" << endl;
        cout << "11. Display Department Hierarchy" << endl;
        cout << "12. Preorder Traversal" << endl;
        cout << "13. Postorder Traversal" << endl;
        cout << "14. Level Order Traversal" << endl;
        cout << "15. Department Statistics" << endl;
        cout << "18. Exit" << endl;

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice)
        {
            // Add Department
            case 9:
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
            case 10:
            {
                string departmentName;

                cout << "Enter department name to search: ";
                getline(cin >> ws, departmentName);

                departmentTree.searchDepartment(departmentName);

                break;
            }

            // Display Hierarchy
            case 11:
                departmentTree.displayHierarchy();
                break;

            // Preorder Traversal
            case 12:
                departmentTree.preorderTraversal();
                break;

            // Postorder Traversal
            case 13:
                departmentTree.postorderTraversal();
                break;

            // Level Order Traversal
            case 14:
                departmentTree.levelOrderTraversal();
                break;

            // Department Statistics
            case 15:
                departmentTree.displayStatistics();
                break;

            // Exit
            case 18:
                cout << "Thank you for using the program!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 18);

    return 0;
}
