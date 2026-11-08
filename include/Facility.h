//Defines facility records amd the facility database operations.
#ifndef FACILITY_H
#define FACILITY_H
#include "HashTable.h"

#include <cstddef>
#include <string>
#include <vector>

using namespace std;

struct Facility {
    string id;        // Always stored in uppercase (for example, F004).
    string name;
    string building;  // Uses spelling from the accepted buildings list.
    string category;
};

class FacilityManager {
public:
    FacilityManager();

    // Loads facilities from a pipe-delimited file. acceptedBuildings must contain
    // the building names already accepted from buildings.txt.
    // Returns false only if the file cannot be opened; it still prints a zero summary.
    bool loadFromFile(
        const string& filename,
        const vector<string>& acceptedBuildings
    );

    // Displays accepted facilities in their original file order.
    void displayAllFacilities() const;

    // Searches by normalized ID through the custom hash table.
    void searchByID(const string& id) const;

    // Final-submission features: exact complete-name/category matching, case-insensitive.
    void searchByName(const string& name) const;
    void searchByCategory(const string& category) const;
    void displayCategoryReport() const;

    const vector<Facility>& getFacilities() const;
    size_t getHashCapacity() const;
    size_t getHashEntryCount() const;
    size_t getHashCollisionCount() const;

private:
    vector<Facility> facilities_;
    HashTable idTable_;
};
#endif