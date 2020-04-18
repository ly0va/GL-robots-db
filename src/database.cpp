#include "database.h"
#include <fstream>

Database::Database(const char *offset_file, const char *entries_file):
    offset_file(offset_file), entries_file(entries_file) {
        std::ofstream offsets(offset_file, std::ofstream::binary | std::ofstream::app);
        std::ofstream entries(entries_file, std::ofstream::binary | std::ofstream::app);
        if (!offsets.is_open()) {
            throw std::runtime_error("Unable to open offsets_file");
        }
        if (!entries.is_open()) {
            throw std::runtime_error("Unable to open entries_file");
        }
        total_entries = (size_t)offsets.tellp()/sizeof(size_t);
        offsets.close();
        entries.close();
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

void Database::write_offset(size_t offset, size_t index) {
    std::ofstream offsets(offset_file, std::ofstream::binary | std::ofstream::app);
    if (!offsets.is_open()) {
        throw std::runtime_error("Unable to open offsets_file");
    }
    const uint8_t size = sizeof(size_t);
    offsets.seekp(index*size);
    if ((size_t)offsets.tellp() != index*size) {
        throw std::runtime_error("Unable to find the offset");
    }
    offsets.write(reinterpret_cast<char*>(&offset), size);
    offsets.close();
}

void Database::add(const Robot& robot) {
    std::ofstream entries(entries_file, std::ofstream::binary | std::ofstream::app);
    if (!entries.is_open()) {
        throw std::runtime_error("Unable to open entries_file");
    }
    long offset = entries.tellp();
    Entry entry = {false, total_entries, robot};
    dump(entries, entry);
    entries.close();
    write_offset(offset, total_entries++);
}

Entry Database::find(size_t id) {
    size_t offset = get_offset(id);
    std::ifstream entries(entries_file, std::ifstream::binary);
    if (!entries.is_open()) {
        throw std::runtime_error("Unable to open entries_file");
    }
    entries.seekg(offset);
    Entry entry = load(entries);
    entries.close();
    return entry;
}

