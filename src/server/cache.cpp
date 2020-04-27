#include "cache.h"

std::optional<Robot> LRUCache::find(size_t key) {
    if (!cache.count(key)) {
        return {};
    }
    const auto& p = cache[key];
    lru.erase(p.second);
    lru.push_back(key);
    cache[key] = {p.first, --lru.end()};
    return p.first;
}

void LRUCache::put(size_t key, const Robot& value) {
    if (cache.count(key)) {
        lru.erase(cache[key].second);
    }
    if (lru.size() == capacity) {
        cache.erase(*lru.begin());
        lru.erase(lru.begin());
    }
    lru.push_back(key);
    cache[key] = {value, --lru.end()};
}

void LRUCache::remove(size_t key) {
    if (!cache.count(key)) return;
    lru.erase(cache[key].second);
    cache.erase(key);
}

void LRUCache::clear() {
    lru.clear();
    cache.clear();
}
