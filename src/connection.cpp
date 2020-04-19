#include "connection.h"
#include "database.h"
#include <cmath>
#include <json/json.h>

const double EPS = 1e-8;

Json::Value to_json(const Entry& entry) {
    Json::Value root;
    root["id"] = (int32_t)entry.id;
    root["price"] = entry.robot.price;
    root["weight"] = entry.robot.weight;
    root["name"] = entry.robot.name;
    return root;
}

Robot from_json(const Json::Value json) {
    Robot robot;
    robot.price =  json["price"].asInt();
    robot.weight = json["weight"].asFloat();
    robot.name =   json["name"].asString();
    return robot;
}

Json::Value DBConnection::ping(const Json::Value& argument) {
    Json::Value root;
    root["status"] = 200;
    return root;
}

Json::Value DBConnection::add(const Json::Value& argument) {
    Robot robot = from_json(argument);
    db.add(robot);
    Json::Value root;
    root["status"] = 200;
    return root;
}

Json::Value DBConnection::remove(const Json::Value& argument) { // TODO: catch errors
    size_t id = argument.asLargestUInt();
    db.remove(id);
    Json::Value root;
    root["status"] = 200;
    return root;
}


Json::Value DBConnection::update(const Json::Value& argument) { // TODO: catch errors
    size_t id = argument["id"].asLargestUInt();
    Robot robot = from_json(argument);
    db.update(id, robot);
    Json::Value root;
    root["status"] = 200;
    return root;
}

Json::Value DBConnection::find(const Json::Value& argument) { // TODO: catch errors
    size_t id = argument.asLargestUInt();
    Entry entry = db.find(id);
    Json::Value root;
    root["result"] = to_json(entry);
    root["status"] = 200;
    return root;
}

Json::Value DBConnection::find_all(const Json::Value& argument) {
    std::function<bool(const Robot&)> predicate;
    if (argument.isMember("price")) {
        predicate = [&](const Robot& r) {
            return r.price == argument["price"].asInt();
        };
    } else if (argument.isMember("name")) {
        predicate = [&](const Robot& r) {
            return r.name == argument["name"].asString();
        };
    } else if (argument.isMember("weight")) {
        predicate = [&](const Robot& r) {
            return fabs(r.weight - argument["weight"].asFloat()) < EPS;
        };
    }
    std::vector<Entry> found = db.find_all(predicate);
    Json::Value root;
    root["status"] = 200;
    root["result"] = Json::Value(Json::arrayValue);
    root["result"].resize(found.size());
    for (size_t e = 0; e < found.size(); e++) {
        root["result"][(int32_t)e] = to_json(found[e]);
    }
    return root;
}

std::string DBConnection::process(const std::string& request) {
    Json::Reader reader;
    Json::Value command;
    if (!reader.parse(request, command)) {
        return "{\"status\": 400}";
    }
    std::string command_type = command["command"].asString();
    Json::Value argument = command["arg"];
    Json::FastWriter writer;
    Json::Value response;
         if (command_type == "add")      response = add(argument);
    else if (command_type == "remove")   response = remove(argument);
    else if (command_type == "update")   response = update(argument);
    else if (command_type == "find")     response = find(argument);
    else if (command_type == "find_all") response = find_all(argument);
    else if (command_type == "ping")     response = ping(argument);
    else return "{\"status\": 400}";
    return writer.write(response);
}



