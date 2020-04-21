#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "entry.h"
#include <inttypes.h>
#include <vector>
#include <functional>
#include <fstream>

typedef std::function<bool(const Robot&)> Predicate;

class Database {
private:
    std::fstream offsets;
    std::fstream entries;
    size_t total_entries;
    size_t get_offset(size_t id);
    void write_offset(size_t offset, size_t index);
public:
    Database(const char *offset_file, const char *entries_file);
    ~Database();
    void add(const Robot& robot);
    void remove(size_t id);
    void update(size_t id, const Robot& robot);
    Entry find(size_t id);
    std::vector<Entry> find_all(const Predicate& p);
    size_t get_total_entries();
};

#endif
