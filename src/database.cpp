#include "database.h"
#include <fstream>
#include <algorithm>

template<typename T> void write(std::ostream& os, T obj) {
    os.write(reinterpret_cast<char*>(&obj), sizeof(T));
}

void dump(std::ostream& os, const Robot& robot) {
    write(os, robot.deleted);
    write(os, robot.id);
    write(os, robot.price);
    write(os, robot.weight);
    write(os, robot.name.size());
    os << robot.name;
}

template<typename T> T read(std::ifstream& is) {
    char buffer[sizeof(T)];
    is.read(buffer, sizeof(T));
    return *reinterpret_cast<T*>(buffer);
}

template<> std::string read<std::string>(std::ifstream& is) {
    size_t len = read<size_t>(is);
    char *buffer = new char[len];
    is.read(buffer, len);
    std::string result(buffer, buffer+len);
    delete[] buffer;
    return result;
}

Robot load(std::ifstream& is) {
    Robot robot;
    robot.deleted = read<bool>(is);
    robot.id      = read<size_t>(is);
    robot.price   = read<int32_t>(is);
    robot.weight  = read<float>(is);
    robot.name    = read<std::string>(is);
    return robot;
}

size_t Database::get_offset(size_t id) {
    std::ifstream offsets(offset_file, std::ifstream::binary);
    if (!offsets.is_open()) {
        throw std::runtime_error("Unable to open offsets_file");
    } 
    const uint8_t size = sizeof(size_t);
    offsets.seekg(id*size);
    if ((size_t)offsets.tellg() != id*size) {
        throw std::runtime_error("Unable to find the offset");
    }
    char buffer[size];
    offsets.read(buffer, size);
    offsets.close();
    size_t *offset = reinterpret_cast<size_t*>(buffer);
    return *offset;
}

void Database::write_offset(size_t offset, size_t index, bool append) {
    std::ofstream offsets(offset_file, std::ofstream::binary);
    if (!offsets.is_open()) {
        throw std::runtime_error("Unable to open offsets_file");
    }
    const uint8_t size = sizeof(size_t);
    offsets.seekp(append ? offsets.end : index*size);
    if (!append && (size_t)offsets.tellp() != index*size) {
        throw std::runtime_error("Unable to find the offset");
    }
    offsets.write(reinterpret_cast<char*>(&offset), size);
    offsets.close();
}

void Database::add(const Robot& robot) {
    std::ofstream robots(entries_file, std::ofstream::app);
    if (!robots.is_open()) {
        throw std::runtime_error("Unable to open entries_file");
    }
    long offset = robots.tellp();
    dump(robots, robot);
    robots.close();
    write_offset(offset, 0, true);
}

Robot Database::find(size_t id) {
    size_t offset = get_offset(id);
    std::ifstream robots(entries_file, std::ifstream::binary);
    if (!robots.is_open()) {
        throw std::runtime_error("Unable to open entries_file");
    }
    robots.seekg(offset);
    Robot robot = load(robots);
    robots.close();
    return robot;
}

