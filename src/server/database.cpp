#include "database.h"
#include <fstream>
#include <iostream>

const auto BIN_APP = std::ofstream::binary | std::ofstream::app;
const auto BIN_IN = std::ofstream::binary | std::ofstream::in;

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
        std::clog << "Found " << total_entries << " existing entries\n";
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
    std::ofstream offsets(offset_file, BIN_IN);
    const uint8_t size = sizeof(size_t);
    offsets.seekp(index*size);
    offsets.write(reinterpret_cast<char*>(&offset), size);
    offsets.close();
}

size_t Database::get_total_entries() {
    return total_entries;
}

void Database::add(const Robot& robot) {
    std::ofstream entries(entries_file, BIN_APP);
    long offset = entries.tellp();
    Entry entry = {false, total_entries, robot};
    entries << entry;
    entries.close();
    write_offset(offset, total_entries++);
    std::clog << "Added a new robot\n";
}

Entry Database::find(size_t id) {
    // TODO: error-checking for out of range id and deleted entries
    size_t offset = get_offset(id);
    std::ifstream entries(entries_file, std::ifstream::binary);
    entries.seekg(offset);
    Entry entry;
    entries >> entry;
    entries.close();
    std::clog << "Found an entry\n";
    return entry;
}

void Database::remove(size_t id) {
    size_t offset = get_offset(id);
    std::ofstream entries(entries_file, BIN_IN);
    entries.seekp(offset);
    char deleted = 1;
    entries.write(&deleted, 1);
    entries.close();
    std::clog << "Removed an entry\n";
    // TODO: check if the entry is already deleted
    // TODO: check if id is out of range
    // notice, we DO NOT decrement total_entries
}

std::vector<Entry> Database::find_all(const Predicate& p) {
    std::ifstream entries(entries_file, std::ifstream::binary);
    entries.seekg(0, std::ios::end);
    long len = entries.tellg();
    entries.seekg(0, std::ios::beg);
    std::vector<Entry> result;
    while (entries.tellg() < len) {
        Entry entry;
        entries >> entry;
        if (!entry.deleted && p(entry.robot)) {
            result.push_back(entry);
        }
    }
    entries.close();
    std::clog << "Found " << result.size() << " entries\n";
    return result;
}

void Database::update(size_t id, const Robot& robot) {
    // TODO: check if id is deleted or out of range
    remove(id);
    std::ofstream entries(entries_file, BIN_APP);
    long offset = entries.tellp();
    Entry entry = {false, id, robot};
    entries << entry;
    entries.close();
    write_offset(offset, id);
    std::clog << "Updated an entry\n";
}
