//Defines the custom separate-chaining hash table used for facility ID lookup.
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <cstddef>
#include <string>
#include <vector>

using namespace std;
class HashTable {
public:
    // Default bucket count is intentionally centralized here.
    static constexpr size_t DEFAULT_CAPACITY = 11;

    explicit HashTable(size_t capacity = DEFAULT_CAPACITY);

    // Inserts a normalized facility ID and its index in FacilityManager's vector.
    // Returns false for a duplicate ID. Successful insertions into a nonempty
    // bucket increment the collision counter once.
    bool insert(const string& id, size_t facilityIndex);

    // Looks up an ID and writes its facility-vector index to facilityIndex.
    bool find(const string& id, size_t& facilityIndex) const;

    bool contains(const string& id) const;

    // Clears entries and diagnostics while keeping the current bucket capacity.
    void clear();

    size_t getCapacity() const;
    size_t getEntryCount() const;
    size_t getCollisionCount() const;

private:
    struct Entry {
        string key;
        size_t facilityIndex;
    };

    vector<vector<Entry>> buckets_;
    size_t entryCount_;
    size_t collisionCount_;

    // Custom polynomial string hash; does not use std::hash.
    size_t hashFunction(const string& normalizedKey) const;
    static string normalizeKey(const string& key);
};
#endif