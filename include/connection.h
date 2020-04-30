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
    static Json::Value ping();
    Json::Value add(const Json::Value& argument);
    Json::Value remove(const Json::Value& argument);
    Json::Value update(const Json::Value& argument);
    Json::Value find(const Json::Value& argument);
    Json::Value find_all(const Json::Value& argument);
    Json::Reader reader;
    Json::FastWriter writer;
    Database& db;
};

#endif
