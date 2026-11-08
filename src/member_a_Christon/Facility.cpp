//implements faciility loading, validation, searching, and category reporting.
#include "Facility.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

string trim(const string& value) {
    size_t first = 0;
    while (first < value.size() &&
           isspace(static_cast<unsigned char>(value[first]))) {
        ++first;
    }

    size_t last = value.size();
    while (last > first &&
           isspace(static_cast<unsigned char>(value[last - 1]))) {
        --last;
    }

    return value.substr(first, last - first);
}

string toUpper(string value) {
    for (char& ch : value) {
        ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    }
    return value;
}

bool equalsIgnoreCase(const string& left, const string& right) {
    return toUpper(trim(left)) == toUpper(trim(right));
}

bool isValidFacilityID(const string& id) {
    if (id.size() != 4 || id[0] != 'F') {
        return false;
    }
    for (size_t i = 1; i < id.size(); ++i) {
        if (!isdigit(static_cast<unsigned char>(id[i]))) {
            return false;
        }
    }
    return true;
}

bool isIgnoredLine(const string& line) {
    const string cleaned = trim(line);
    return cleaned.empty() || cleaned[0] == '#';
}

// Splits on every pipe and preserves empty fields, including a trailing one.
vector<string> splitPipeLine(const string& line) {
    vector<string> fields;
    size_t start = 0;

    while (true) {
        const size_t delimiter = line.find('|', start);
        if (delimiter == string::npos) {
            fields.push_back(trim(line.substr(start)));
            break;
        }
        fields.push_back(trim(line.substr(start, delimiter - start)));
        start = delimiter + 1;
    }
    return fields;
}

string facilityToString(const Facility& facility) {
    return facility.id + " | " + facility.name + " | " +
           facility.building + " | " + facility.category;
}

bool findCanonicalBuilding(
    const string& requestedName,
    const vector<string>& acceptedBuildings,
    string& canonicalName
) {
    for (const string& building : acceptedBuildings) {
        if (equalsIgnoreCase(requestedName, building)) {
            canonicalName = trim(building);
            return true;
        }
    }
    return false;
}

// namespace

FacilityManager::FacilityManager() : facilities_(), idTable_() {
}

bool FacilityManager::loadFromFile(
    const string& filename,
    const vector<string>& acceptedBuildings
) {
    // Reset first so calling the loader again does not retain stale facilities.
    facilities_.clear();
    idTable_.clear();

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Error: Could not open file '" << filename
             << "'. Treating it as empty.\n";
        cout << filename << ": 0 accepted, 0 rejected"
             << " (hash capacity: " << idTable_.getCapacity()
             << ", entries: " << idTable_.getEntryCount()
             << ", collisions: " << idTable_.getCollisionCount() << ").\n";
        return false;
    }

    string line;
    size_t lineNumber = 0;
    size_t accepted = 0;
    size_t rejected = 0;

    while (getline(inputFile, line)) {
        ++lineNumber;
        if (isIgnoredLine(line)) {
            continue;
        }

        const vector<string> fields = splitPipeLine(line);

        // Validation order follows the project instructions.
        if (fields.size() != 4) {
            cout << "Warning: " << filename << " line " << lineNumber
                 << ": expected 4 fields, found " << fields.size() << ".\n";
            ++rejected;
            continue;
        }

        bool hasEmptyField = false;
        for (const string& field : fields) {
            if (field.empty()) {
                hasEmptyField = true;
                break;
            }
        }
        if (hasEmptyField) {
            cout << "Warning: " << filename << " line " << lineNumber
                 << ": one or more required fields are empty.\n";
            ++rejected;
            continue;
        }

        const string normalizedID = toUpper(fields[0]);
        if (!isValidFacilityID(normalizedID)) {
            cout << "Warning: " << filename << " line " << lineNumber
                 << ": invalid facility ID '" << fields[0]
                 << "' (expected F followed by exactly three digits).\n";
            ++rejected;
            continue;
        }

        string canonicalBuilding;
        if (!findCanonicalBuilding(fields[2], acceptedBuildings, canonicalBuilding)) {
            cout << "Warning: " << filename << " line " << lineNumber
                 << ": unknown building reference '" << fields[2] << "'.\n";
            ++rejected;
            continue;
        }

        if (idTable_.contains(normalizedID)) {
            cout << "Warning: " << filename << " line " << lineNumber
                 << ": duplicate facility ID '" << normalizedID
                 << "'; keeping the first accepted record.\n";
            ++rejected;
            continue;
        }

        Facility facility{normalizedID, fields[1], canonicalBuilding, fields[3]};
        const size_t facilityIndex = facilities_.size();

        // The hash table stores the facility's position in the ordered vector.
        if (!idTable_.insert(normalizedID, facilityIndex)) {
            // Defensive duplicate check; should not occur after contains().
            cout << "Warning: " << filename << " line " << lineNumber
                 << ": duplicate facility ID '" << normalizedID
                 << "'; keeping the first accepted record.\n";
            ++rejected;
            continue;
        }

        facilities_.push_back(facility);
        ++accepted;
    }

    cout << filename << ": " << accepted << " accepted, " << rejected
         << " rejected (hash capacity: " << idTable_.getCapacity()
         << ", entries: " << idTable_.getEntryCount()
         << ", collisions: " << idTable_.getCollisionCount() << ").\n";
    return true;
}

void FacilityManager::displayAllFacilities() const {
    if (facilities_.empty()) {
        cout << "No facilities are available.\nTotal facilities: 0\n";
        return;
    }

    cout << "\n--- All Facilities ---\n";
    for (const Facility& facility : facilities_) {
        cout << facilityToString(facility) << '\n';
    }
    cout << "Total facilities: " << facilities_.size() << '\n';
}

void FacilityManager::searchByID(const string& id) const {
    const string normalizedID = toUpper(trim(id));
    if (!isValidFacilityID(normalizedID)) {
        cout << "Invalid facility ID format: '" << trim(id)
             << "'. An ID must be F followed by exactly three digits.\n";
        return;
    }

    size_t facilityIndex = 0;
    if (!idTable_.find(normalizedID, facilityIndex)) {
        cout << "Not found: no facility with ID " << normalizedID << ".\n";
        return;
    }

    if (facilityIndex >= facilities_.size()) {
        cout << "Error: invalid facility index for ID " << normalizedID << ".\n";
        return;
    }
    cout << "Found: " << facilityToString(facilities_[facilityIndex]) << '\n';
}

void FacilityManager::searchByName(const string& name) const {
    const string requestedName = trim(name);
    if (requestedName.empty()) {
        cout << "Error: facility name cannot be blank.\nMatches: 0\n";
        return;
    }

    size_t matches = 0;
    for (const Facility& facility : facilities_) {
        if (equalsIgnoreCase(facility.name, requestedName)) {
            cout << facilityToString(facility) << '\n';
            ++matches;
        }
    }
    if (matches == 0) {
        cout << "No facilities found with name '" << requestedName << "'.\n";
    }
    cout << "Matches: " << matches << '\n';
}

void FacilityManager::searchByCategory(const string& category) const {
    const string requestedCategory = trim(category);
    if (requestedCategory.empty()) {
        cout << "Error: category cannot be blank.\nMatches: 0\n";
        return;
    }

    size_t matches = 0;
    for (const Facility& facility : facilities_) {
        if (equalsIgnoreCase(facility.category, requestedCategory)) {
            cout << facilityToString(facility) << '\n';
            ++matches;
        }
    }
    if (matches == 0) {
        cout << "No facilities found in category '" << requestedCategory << "'.\n";
    }
    cout << "Matches: " << matches << '\n';
}

void FacilityManager::displayCategoryReport() const {
    if (facilities_.empty()) {
        cout << "No facilities are available for the category report.\n"
             << "Total facilities: 0\n";
        return;
    }

    struct CategoryCount {
        string spelling;
        size_t count;
    };

    vector<CategoryCount> categories;
    for (const Facility& facility : facilities_) {
        bool found = false;
        for (CategoryCount& item : categories) {
            if (equalsIgnoreCase(item.spelling, facility.category)) {
                ++item.count;
                found = true;
                break;
            }
        }
        if (!found) {
            categories.push_back({facility.category, 1});
        }
    }

    cout << "\n--- Facility Category Report ---\n";
    cout << left << setw(28) << "Category"
         << right << setw(10) << "Count"
         << setw(14) << "Percentage" << '\n';

    for (const CategoryCount& item : categories) {
        const double percentage = 100.0 * static_cast<double>(item.count) /
                                  static_cast<double>(facilities_.size());
        cout << left << setw(28) << item.spelling
             << right << setw(10) << item.count
             << setw(13) << fixed << setprecision(1)
             << percentage << "%\n";
    }

    cout << left << setw(28) << "Total"
         << right << setw(10) << facilities_.size()
         << setw(14) << "100.0%" << '\n';
}

const vector<Facility>& FacilityManager::getFacilities() const {
    return facilities_;
}

size_t FacilityManager::getHashCapacity() const {
    return idTable_.getCapacity();
}

size_t FacilityManager::getHashEntryCount() const {
    return idTable_.getEntryCount();
}

size_t FacilityManager::getHashCollisionCount() const {
    return idTable_.getCollisionCount();
}
