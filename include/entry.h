#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <inttypes.h>
#include <string>

struct Robot {
    int32_t price;
    float weight;
    std::string name;
};

struct Entry {
    bool deleted;
    size_t id;
    Robot robot;
};

void dump(std::ostream& os, const Entry& entry);
Entry load(std::istream& is);

#endif
