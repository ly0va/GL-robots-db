#include "entry.h"
#include "endian.h"
#include <fstream>
#include <algorithm>

template<typename T> void write(std::ostream& os, T obj) {
    char *buffer = reinterpret_cast<char*>(&obj);
    if (HOST_ORDER == BIG_ENDIAN) {
        std::reverse(buffer, buffer + sizeof(T));
    }
    os.write(buffer, sizeof(T));
}

std::ostream& operator<<(std::ostream& os, const Entry& entry) {
    write(os, entry.deleted);
    write(os, entry.id);
    write(os, entry.robot.price);
    write(os, entry.robot.weight);
    write(os, entry.robot.name.size());
    os.write(entry.robot.name.c_str(), entry.robot.name.size());
    os.flush();
    return os;
}

template<typename T> T read(std::istream& is) {
    char buffer[sizeof(T)];
    is.read(buffer, sizeof(T));
    if (HOST_ORDER == BIG_ENDIAN) {
        std::reverse(buffer, buffer+sizeof(T));
    }
    return *reinterpret_cast<T*>(buffer);
}

template<> std::string read<std::string>(std::istream& is) {
    size_t len = read<size_t>(is);
    char *buffer = new char[len];
    is.read(buffer, len);
    std::string result(buffer, buffer+len);
    delete[] buffer;
    return result;
}

std::istream& operator>>(std::istream& is, Entry& entry) {
    entry.deleted = read<bool>(is);
    entry.id = read<size_t>(is);
    entry.robot.price = read<int32_t>(is);
    entry.robot.weight = read<float>(is);
    entry.robot.name = read<std::string>(is);
    return is;
}
