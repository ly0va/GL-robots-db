#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "database.h"
#include <string>
#include <json/json.h>

class DBConnection {
public:
    explicit DBConnection(Database& db): db(db) {}
    std::string process(const std::string& request);
private:
    Json::Value ping(const Json::Value& command);
    Json::Value add(const Json::Value& command);
    Json::Value remove(const Json::Value& command);
    Json::Value update(const Json::Value& command);
    Json::Value find(const Json::Value& command);
    Json::Value find_all(const Json::Value& command);
    Database& db;
};

#endif
