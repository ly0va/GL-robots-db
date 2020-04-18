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

std::ostream& operator<<(std::ostream& os, const Entry& entry);
std::istream& operator>>(std::istream& is, Entry& entry);

#endif
