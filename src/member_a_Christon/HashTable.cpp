// HashTable.cpp
// Implements the custom separate-chaining hash table.
#include <fstream>
#include <cstddef>
#include <string>
#include <vector>

#include "HashTable.h"

#include <cctype>
using namespace std;

HashTable::HashTable(size_t capacity)
    : entryCount_(0), collisionCount_(0)
{
    buckets_.resize(capacity == 0 ? 1 : capacity);
}

string HashTable::normalizeKey(const string& key) {
    size_t first = 0;
    while (first < key.size() && isspace(static_cast<unsigned char>(key[first]))) {
        ++first;
    }

    size_t last = key.size();
    while (last > first && isspace(static_cast<unsigned char>(key[last - 1]))) {
        --last;
    }

    string normalized = key.substr(first, last - first);
    for (char& ch : normalized) {
        ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    }
    return normalized;
}

size_t HashTable::hashFunction(const string& normalizedKey) const {
    if (buckets_.empty()) {
        return 0;
    }

    // Polynomial rolling hash: h = (h * 31 + character) mod capacity.
    size_t hashValue = 0;
    for (size_t i = 0; i < normalizedKey.size(); ++i) {
        const unsigned char ch = static_cast<unsigned char>(normalizedKey[i]);
        hashValue = (hashValue * 31 + ch) % buckets_.size();
    }
    return hashValue;
}

bool HashTable::insert(const string& id, size_t facilityIndex) {
    const string key = normalizeKey(id);
    const size_t bucketIndex = hashFunction(key);
    vector<Entry>& bucket = buckets_[bucketIndex];

    // Reject duplicate keys before counting collisions.
    for (vector<Entry>::const_iterator it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->key == key) {
            return false;
        }
    }

    // One collision is counted for each successful insertion into a bucket
    // that already contains at least one entry.
    if (!bucket.empty()) {
        ++collisionCount_;
    }

    bucket.push_back({key, facilityIndex});
    ++entryCount_;
    return true;
}

bool HashTable::find(const string& id, size_t& facilityIndex) const {
    const string key = normalizeKey(id);
    const size_t bucketIndex = hashFunction(key);

    for (const Entry& entry : buckets_[bucketIndex]) {
        if (entry.key == key) {
            facilityIndex = entry.facilityIndex;
            return true;
        }
    }
    return false;
}

bool HashTable::contains(const string& id) const {
    size_t unusedIndex = 0;
    return find(id, unusedIndex);
}

void HashTable::clear() {
    for (vector<Entry>& bucket : buckets_) {
        bucket.clear();
    }
    entryCount_ = 0;
    collisionCount_ = 0;
}

size_t HashTable::getCapacity() const {
    return buckets_.size();
}

size_t HashTable::getEntryCount() const {
    return entryCount_;
}

size_t HashTable::getCollisionCount() const {
    return collisionCount_;
}
