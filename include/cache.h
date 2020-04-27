#ifndef _LRU_CACHE_H_
#define _LRU_CACHE_H_

#include <list>
#include <unordered_map>
#include <memory>
#include <optional>
#include "entry.h"

using ListIt = std::list<size_t>::iterator;

class LRUCache {
private:
    std::list<size_t> lru; // list of keys in lru order
    std::unordered_map<size_t, std::pair<Robot, ListIt>> cache;
    size_t capacity;
public:
    explicit LRUCache(size_t capacity): capacity(capacity) {}
    std::optional<Robot> find(size_t key);
    void put(size_t key, const Robot& value);
    void remove(size_t key);
    void clear();
};

#endif
