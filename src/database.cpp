#include "database.h"
#include <fstream>

const auto BIN_APP = std::ofstream::binary | std::ofstream::app;

Database::Database(const char *offset_file, const char *entries_file):
    offset_file(offset_file), entries_file(entries_file) {
        std::ofstream offsets(offset_file, BIN_APP);
        std::ofstream entries(entries_file, BIN_APP);
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
    const uint8_t size = sizeof(size_t);
    offsets.seekg(id*size);
    char buffer[size];
    offsets.read(buffer, size);
    offsets.close();
    return *reinterpret_cast<size_t*>(buffer);
}

void Database::write_offset(size_t offset, size_t index) {
    std::ofstream offsets(offset_file, BIN_APP);
    const uint8_t size = sizeof(size_t);
    offsets.seekp(index*size);
    offsets.write(reinterpret_cast<char*>(&offset), size);
    offsets.close();
}

void Database::add(const Robot& robot) {
    std::ofstream entries(entries_file, BIN_APP);
    long offset = entries.tellp();
    Entry entry = {false, total_entries, robot};
    dump(entries, entry);
    entries.close();
    write_offset(offset, total_entries++);
}

Entry Database::find(size_t id) {
    // TODO: error-checking for out of range id and deleted entries
    size_t offset = get_offset(id);
    std::ifstream entries(entries_file, std::ifstream::binary);
    entries.seekg(offset);
    Entry entry = load(entries);
    entries.close();
    return entry;
}

void Database::remove(size_t id) {
    size_t offset = get_offset(id);
    std::ofstream entries(entries_file, BIN_APP);
    entries.seekp(offset);
    char deleted = true;
    entries.write(&deleted, 1);
    entries.close();
    // TODO: check if the entry is already deleted
    // notice, we DO NOT decrement total_entries
}

std::vector<Entry> Database::find_all(Predicate p) {
    std::ifstream entries(entries_file, std::ifstream::binary);
    std::vector<Entry> result;
    for (size_t id = 0; id < total_entries; id++) {
        Entry entry = load(entries);
        if (!entry.deleted && p(entry.robot)) {
            result.push_back(entry);
        }
    }
    entries.close();
    return result;
}
