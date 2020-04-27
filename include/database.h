#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "entry.h"
#include "cache.h"
#include <inttypes.h>
#include <vector>
#include <functional>
#include <fstream>

using Predicate = std::function<bool(const Robot&)>;

class Database {
private:
    const char *o_filename;
    const char *e_filename;
    std::fstream offsets;
    std::fstream entries;
    size_t total_entries;
    LRUCache cache;
    size_t get_offset(size_t id);
    void write_offset(size_t offset, size_t index);
public:
    Database(const char *offset_file, const char *entries_file);
    ~Database();
    void add(const Robot& robot);
    int8_t remove(size_t id);
    int8_t update(size_t id, const Robot& robot);
    std::optional<Entry> find(size_t id);
    std::vector<Entry> find_all(const Predicate& p);
    void cleanup();
    size_t get_total_entries() const;
};

#endif
