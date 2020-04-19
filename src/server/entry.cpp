#include "entry.h"
#include <fstream>

template<typename T> void write(std::ostream& os, T obj) {
    os.write(reinterpret_cast<char*>(&obj), sizeof(T));
}

std::ostream& operator<<(std::ostream& os, const Entry& robot) {
    write(os, robot.deleted);
    write(os, robot.id);
    write(os, robot.robot.price);
    write(os, robot.robot.weight);
    write(os, robot.robot.name.size());
    return os << robot.robot.name;
}

template<typename T> T read(std::istream& is) {
    char buffer[sizeof(T)];
    is.read(buffer, sizeof(T));
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
