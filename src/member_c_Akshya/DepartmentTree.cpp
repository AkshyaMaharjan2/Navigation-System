#include "DepartmentTree.h"

#include <fstream>
#include <queue>
#include <cctype>

// Constructor
DepartmentTree::DepartmentTree()
{
    root = nullptr;
}

// Destructor
DepartmentTree::~DepartmentTree()
{
    deleteTree(root);
}

// Remove spaces from the beginning and end of a string
string DepartmentTree::trim(const string& text) const
{
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos)
    {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}

// Convert text to lowercase
string DepartmentTree::toLowerCase(const string& text) const
{
    string result = text;

    for (size_t i = 0; i < result.length(); i++)
    {
        result[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(result[i]))
        );
    }

    return result;
}

// Find a department by name
DepartmentTree::DepartmentNode*
DepartmentTree::findDepartment(const string& name) const
{
    if (root == nullptr)
    {
        return nullptr;
    }

    queue<DepartmentNode*> departments;

    departments.push(root);

    string searchName = toLowerCase(trim(name));

    while (!departments.empty())
    {
        DepartmentNode* current = departments.front();

        departments.pop();

        if (toLowerCase(current->name) == searchName)
        {
            return current;
        }

        for (DepartmentNode* child : current->children)
        {
            departments.push(child);
        }
    }

    return nullptr;
}

// Delete all nodes and free memory
void DepartmentTree::deleteTree(DepartmentNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    for (DepartmentNode* child : node->children)
    {
        deleteTree(child);
    }

    delete node;
}

// Check whether the tree is empty
bool DepartmentTree::isEmpty() const
{
    return root == nullptr;
}

// Load departments from a text file
bool DepartmentTree::loadFromFile(const string& filename)
{
    ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        cout << "Error: Could not open file: "
             << filename << endl;

        return false;
    }

    // Clear the old tree before loading
    deleteTree(root);
    root = nullptr;

    string line;

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    while (getline(inputFile, line))
    {
        lineNumber++;

        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // Find the separator
        size_t separator = line.find('|');

        if (separator == string::npos ||
            line.find('|', separator + 1) != string::npos)
        {
            cout << "Invalid format on line "
                 << lineNumber << endl;

            rejected++;
            continue;
        }

        string departmentName =
            trim(line.substr(0, separator));

        string parentName =
            trim(line.substr(separator + 1));

        // Check for empty names
        if (departmentName.empty() || parentName.empty())
        {
            cout << "Empty department or parent name on line "
                 << lineNumber << endl;

            rejected++;
            continue;
        }

        // Check for duplicate department names
        if (findDepartment(departmentName) != nullptr)
        {
            cout << "Duplicate department rejected: "
                 << departmentName << endl;

            rejected++;
            continue;
        }

        // First accepted department must be the root
        if (root == nullptr)
        {
            if (toLowerCase(parentName) != "none")
            {
                cout << "First department must have NONE "
                     << "as its parent: "
                     << departmentName << endl;

                rejected++;
                continue;
            }

            root = new DepartmentNode(departmentName);

            accepted++;
        }
        else
        {
            // Parent must already exist
            DepartmentNode* parent =
                findDepartment(parentName);

            if (parent == nullptr)
            {
                cout << "Parent not found for department: "
                     << departmentName << endl;

                rejected++;
                continue;
            }

            DepartmentNode* newDepartment =
                new DepartmentNode(departmentName, parent);

            // Add the new department to its parent's children
            parent->children.push_back(newDepartment);

            accepted++;
        }
    }

    inputFile.close();

    cout << "\nDepartment file loading completed." << endl;
    cout << "Accepted departments: " << accepted << endl;
    cout << "Rejected departments: " << rejected << endl;

    return true;
}

// Add a new department
bool DepartmentTree::addDepartment(
    const string& name,
    const string& parentName)
{
    string departmentName = trim(name);
    string parent = trim(parentName);

    // Department name cannot be empty
    if (departmentName.empty())
    {
        cout << "Department name cannot be empty."
             << endl;

        return false;
    }

    // Check for duplicates
    if (findDepartment(departmentName) != nullptr)
    {
        cout << "Department already exists."
             << endl;

        return false;
    }

    // If the tree is empty, create the root
    if (root == nullptr)
    {
        root = new DepartmentNode(departmentName);

        cout << "Root department added successfully."
             << endl;

        return true;
    }

    // Other departments need an existing parent
    if (parent.empty())
    {
        cout << "Please enter an existing parent department."
             << endl;

        return false;
    }

    DepartmentNode* parentNode =
        findDepartment(parent);

    if (parentNode == nullptr)
    {
        cout << "Parent department does not exist."
             << endl;

        return false;
    }

    DepartmentNode* newDepartment =
        new DepartmentNode(departmentName, parentNode);

    parentNode->children.push_back(newDepartment);

    cout << "Department added successfully."
         << endl;

    return true;
}

// Calculate the depth of a department
int DepartmentTree::calculateDepth(DepartmentNode* node) const
{
    int depth = 0;

    while (node != nullptr && node->parent != nullptr)
    {
        depth++;

        node = node->parent;
    }

    return depth;
}

// Search for a department
bool DepartmentTree::searchDepartment(
    const string& name) const
{
    DepartmentNode* department =
        findDepartment(name);

    if (department == nullptr)
    {
        cout << "Department not found." << endl;

        return false;
    }

    cout << "\nDepartment Information" << endl;

    cout << "Name: "
         << department->name << endl;

    if (department->parent == nullptr)
    {
        cout << "Parent: None" << endl;
    }
    else
    {
        cout << "Parent: "
             << department->parent->name << endl;
    }

    cout << "Direct children: "
         << department->children.size() << endl;

    cout << "Depth: "
         << calculateDepth(department) << endl;

    return true;
}

// Display hierarchy with four spaces for each level
void DepartmentTree::displayPreorder(
    DepartmentNode* node,
    int depth) const
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        cout << "    ";
    }

    cout << node->name << endl;

    for (DepartmentNode* child : node->children)
    {
        displayPreorder(child, depth + 1);
    }
}

// Display the department hierarchy
void DepartmentTree::displayHierarchy() const
{
    if (root == nullptr)
    {
        cout << "The department tree is empty."
             << endl;

        return;
    }

    cout << "\nDepartment Hierarchy:" << endl;

    displayPreorder(root, 0);
}

// Preorder sequence without indentation
void DepartmentTree::displayPreorderSequence(
    DepartmentNode* node) const
{
    if (node == nullptr)
    {
        return;
    }

    cout << node->name << endl;

    for (DepartmentNode* child : node->children)
    {
        displayPreorderSequence(child);
    }
}

// Preorder traversal
void DepartmentTree::preorderTraversal() const
{
    if (root == nullptr)
    {
        cout << "The department tree is empty."
             << endl;

        return;
    }

    cout << "\nPreorder Traversal:" << endl;

    displayPreorderSequence(root);
}

// Postorder helper function
void DepartmentTree::displayPostorder(
    DepartmentNode* node) const
{
    if (node == nullptr)
    {
        return;
    }

    for (DepartmentNode* child : node->children)
    {
        displayPostorder(child);
    }

    cout << node->name << endl;
}

// Postorder traversal
void DepartmentTree::postorderTraversal() const
{
    if (root == nullptr)
    {
        cout << "The department tree is empty."
             << endl;

        return;
    }

    cout << "\nPostorder Traversal:" << endl;

    displayPostorder(root);
}

// Level-order traversal using a queue
void DepartmentTree::levelOrderTraversal() const
{
    if (root == nullptr)
    {
        cout << "The department tree is empty."
             << endl;

        return;
    }

    cout << "\nLevel-order Traversal:" << endl;

    queue<DepartmentNode*> departments;

    departments.push(root);

    while (!departments.empty())
    {
        DepartmentNode* current =
            departments.front();

        departments.pop();

        cout << current->name << endl;

        for (DepartmentNode* child : current->children)
        {
            departments.push(child);
        }
    }
}

// Count all departments
int DepartmentTree::countDepartments(
    DepartmentNode* node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    int total = 1;

    for (DepartmentNode* child : node->children)
    {
        total += countDepartments(child);
    }

    return total;
}

// Count departments without children
int DepartmentTree::countLeaves(
    DepartmentNode* node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    if (node->children.empty())
    {
        return 1;
    }

    int total = 0;

    for (DepartmentNode* child : node->children)
    {
        total += countLeaves(child);
    }

    return total;
}

// Calculate tree height
int DepartmentTree::calculateHeight(
    DepartmentNode* node) const
{
    if (node == nullptr)
    {
        return -1;
    }

    int maximumHeight = -1;

    for (DepartmentNode* child : node->children)
    {
        int childHeight =
            calculateHeight(child);

        if (childHeight > maximumHeight)
        {
            maximumHeight = childHeight;
        }
    }

    return maximumHeight + 1;
}

// Display department statistics
void DepartmentTree::displayStatistics() const
{
    cout << "\nDepartment Statistics:" << endl;

    if (root == nullptr)
    {
        cout << "Total departments: 0" << endl;
        cout << "Leaf departments: 0" << endl;
        cout << "Tree height: N/A" << endl;

        return;
    }

    cout << "Total departments: "
         << countDepartments(root) << endl;

    cout << "Leaf departments: "
         << countLeaves(root) << endl;

    cout << "Tree height: "
         << calculateHeight(root) << endl;
}