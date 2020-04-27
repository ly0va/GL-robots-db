#include "database.h"
#include "cache.h"
#include <iostream>
#include <cassert>

const auto MODE  = std::ios::binary | std::ios::in  | std::ios::out;
const auto TOUCH = std::ios::binary | std::ios::app | std::ios::out;
const size_t CACHE_SIZE = 256;

Database::Database(const char *offset_file, const char *entries_file):
    o_filename(offset_file), e_filename(entries_file),
    offsets(offset_file, TOUCH), entries(entries_file, TOUCH), cache(CACHE_SIZE) {
        offsets.close();
        entries.close();
        offsets.open(offset_file, MODE);
        entries.open(entries_file, MODE);
        if (!offsets.is_open()) {
            std::cerr << "Unable to open offsets file\n";
            abort();
        }
        if (!entries.is_open()) {
            std::cerr << "Unable to open entries file\n";
            abort();
        }
        offsets.seekp(0, std::ios::end);
        total_entries = (size_t)offsets.tellp()/sizeof(size_t);
}

Database::~Database() {
    if (entries.is_open()) {
        entries.close();
    }
    if (offsets.is_open()) {
        offsets.close();
    }
}

size_t Database::get_offset(size_t id) {
    const uint8_t size = sizeof(size_t);
    offsets.seekg(id*size);
    char buffer[size];
    offsets.read(buffer, size);
    assert(!offsets.fail());
    return *reinterpret_cast<size_t*>(buffer);
}

void Database::write_offset(size_t offset, size_t index) {
    const uint8_t size = sizeof(size_t);
    offsets.seekp(index*size);
    offsets.write(reinterpret_cast<char*>(&offset), size);
    offsets.flush();
    assert(!offsets.fail());
}

size_t Database::get_total_entries() const {
    return total_entries;
}

void Database::add(const Robot& robot) {
    cache.put(total_entries, robot);
    entries.seekp(0, std::ios::end);
    long offset = entries.tellp();
    Entry entry = {false, total_entries, robot};
    entries << entry;
    assert(!entries.fail());
    write_offset(offset, total_entries++);
}

std::optional<Entry> Database::find(size_t id) {
    auto cached = cache.find(id);
    if (cached != std::nullopt) {
        return Entry{false, id, cached.value()};
    }
    if (id >= total_entries) {
        return {};
    }
    size_t offset = get_offset(id);
    entries.seekg(offset);
    Entry entry;
    entries >> entry;
    assert(!entries.fail());
    if (entry.deleted) {
        return {};
    }
    return entry;
}

// 0 is success
int8_t Database::remove(size_t id) {
    cache.remove(id);
    if (id >= total_entries) {
        return 1;
    }
    size_t offset = get_offset(id);
    char deleted = 0;
    entries.seekp(offset);
    entries.read(&deleted, 1);
    if (deleted) {
        return 1;
    }
    deleted = 1;
    entries.seekp(offset);
    entries.write(&deleted, 1);
    entries.flush();
    assert(!entries.fail());
    return 0;
    // notice, we DO NOT decrement total_entries
}

std::vector<Entry> Database::find_all(const Predicate& p) {
    entries.seekg(0, std::ios::end);
    long len = entries.tellg();
    entries.seekg(0, std::ios::beg);
    std::vector<Entry> result;
    result.reserve(total_entries);
    while (entries.tellg() < len) {
        Entry entry;
        entries >> entry;
        if (!entry.deleted && p(entry.robot)) {
            result.push_back(entry);
        }
    }
    assert(!entries.fail());
    return result;
}

// 0 is success
int8_t Database::update(size_t id, const Robot& robot) {
    if (remove(id) != 0) {
        return 1;
    }
    cache.put(id, robot);
    entries.seekp(0, std::ios::end);
    long offset = entries.tellp();
    Entry entry = {false, id, robot};
    entries << entry;
    assert(!entries.fail());
    write_offset(offset, id);
    return 0;
}

// costly operation, use rarely
// will renumerate all entries
void Database::cleanup() {
    cache.clear();
    std::vector<Entry> all_entries = find_all(
        [](const Robot& r){ return true; }
    );
    total_entries = all_entries.size();
    entries.close();
    offsets.close();
    entries.open(e_filename, std::ios::out | std::ios::binary);
    offsets.open(o_filename, std::ios::out | std::ios::binary);
    for (size_t id = 0; id < total_entries; id++) {
        write_offset(entries.tellp(), id);
        all_entries[id].id = id;
        entries << all_entries[id];
    }
    entries.close();
    offsets.close();
    entries.open(e_filename, MODE);
    offsets.open(o_filename, MODE);
}
