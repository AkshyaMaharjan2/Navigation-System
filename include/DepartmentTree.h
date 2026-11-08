#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class DepartmentTree
{
private:

    // Each node represents one department
    struct DepartmentNode
    {
        string name;
        DepartmentNode* parent;
        vector<DepartmentNode*> children;

        DepartmentNode(string departmentName,
                       DepartmentNode* parentNode = nullptr)
        {
            name = departmentName;
            parent = parentNode;
        }
    };

    DepartmentNode* root;

    // Helper functions
    string trim(const string& text) const;
    string toLowerCase(const string& text) const;

    DepartmentNode* findDepartment(const string& name) const;

    void deleteTree(DepartmentNode* node);

    void displayPreorder(DepartmentNode* node, int depth) const;
    void displayPreorderSequence(DepartmentNode* node) const;
    void displayPostorder(DepartmentNode* node) const;

    int countDepartments(DepartmentNode* node) const;
    int countLeaves(DepartmentNode* node) const;
    int calculateHeight(DepartmentNode* node) const;
    int calculateDepth(DepartmentNode* node) const;

public:

    // Constructor and destructor
    DepartmentTree();
    ~DepartmentTree();

    // Prevent copying of tree pointers
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // Department operations
    bool loadFromFile(const string& filename);

    bool addDepartment(const string& name,
                       const string& parentName = "");

    bool searchDepartment(const string& name) const;

    void displayHierarchy() const;

    // Tree traversals
    void preorderTraversal() const;
    void postorderTraversal() const;
    void levelOrderTraversal() const;

    // Department statistics
    void displayStatistics() const;

    // Check if tree is empty
    bool isEmpty() const;
};

#endif