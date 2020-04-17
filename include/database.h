#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include <functional>

struct Robot {
    bool deleted;
    size_t id;
    int32_t price;
    float weight;
    std::string name;
};

typedef std::function<bool(const Robot&)> Predicate;

class Database {
private:
    const char *offset_file;
    const char *entries_file;
public:
    size_t get_offset(size_t id);
    void write_offset(size_t offset, size_t index, bool append=false);
    Database(const char *offset_file, const char *entries_file):
        offset_file(offset_file), entries_file(entries_file) {}
    void add(const Robot& robot);
    void remove(size_t id);
    void update(size_t id, const Robot& robot);
    Robot find(size_t id);
    std::vector<Robot> find_all(Predicate p);
};

#endif
