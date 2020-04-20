#include "client.h"
#include <iostream>
#include <iomanip>

static const char *HELP = R"(COMMANDS:
    ping       - check connection to the server
    add        - add a new robot to the database
    update     - update an existing robot in the database
    remove     - remove an existing robot in the database
    find       - find an existing robot by its ID
    search     - search robots by field
    dump       - print all entries in the database
    help       - print this message
    quit       - quit the client
)";

static std::string input(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

static void dump_entry(const Json::Value& entry) {
    std::cout << "  ID: "
        << entry["id"]    << "\n  Name: "
        << entry["name"]  << "\n  Price: "
        << entry["price"] << "\n  Weight: ";
    std::cout << std::fixed << std::setprecision(4)
        << entry["weight"].asDouble() << '\n';
}

Client::Client(const std::string& host, const std::string& port):
    context(1), socket(context, ZMQ_REQ) {
        socket.setsockopt(ZMQ_RCVTIMEO, 5000);
        socket.setsockopt(ZMQ_SNDTIMEO, 5000);
        std::string address = "tcp://" + host + ":" + port;
        std::cout << "Connecting to " << address << " ...\n";
        socket.connect(address);
        ping();
}

Json::Value Client::send_recv(const Json::Value& request_json) {
    std::string request_str = writer.write(request_json);
    zmq::message_t request(request_str.size());
    memcpy(request.data(), request_str.c_str(), request_str.size());
    zmq::message_t reply;
    bool send_res = socket.send(request);
    bool recv_res = socket.recv(&reply);
    if (send_res == 0 || recv_res == 0) {
        std::cerr << "Connection timed out\n";
        exit(1);
    }
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    Json::Value json; 
    reader.parse(reply_str, json);
    if (json["status"].asInt() == 400) {
        throw std::runtime_error("Somehow a bad request was formed");
    }
    return json;
}

void Client::mainloop() {
    std::string cmd;
    std::cout << "Welcome to the RobotsDB!\n"
              << "Type a command ('help' for help)\n";
    do {
        std::cout << ">> ";
        std::getline(std::cin, cmd);
        try {
                 if (cmd == "help")     std::cout << HELP;
            else if (cmd == "ping")     ping();
            else if (cmd == "add")      add();
            else if (cmd == "find")     find();
            else if (cmd == "search")   find_all();
            else if (cmd == "update")   update();
            else if (cmd == "remove")   remove();
            else if (cmd == "dump")     dump();
            else if (cmd != "" && cmd != "quit") {
                std::cerr << "Invalid command\n";
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << '\n';
        }
    } while (cmd != "quit" && !std::cin.eof());
}

void Client::ping() {
    Json::Value request;
    request["command"] = "ping";
    Json::Value response = send_recv(request);
    if (response["status"].asInt() == 200) {
        std::cout << "Connection is OK\n";
    }
}

void Client::find() {
    Json::Value request;
    request["command"] = "find";
    request["arg"] = std::stoi(input("ID (int): "));
    Json::Value response = send_recv(request);
    int32_t status = response["status"].asInt();
    if (status == 404) {
        std::cout << "No entry with given ID\n";
    } else if (status == 200) {
        std::cout << "Found 1 entry:\n";
        dump_entry(response["result"]);
    }
}

void Client::remove() {
    Json::Value request;
    request["command"] = "remove";
    request["arg"] = std::stoi(input("ID (int): "));
    Json::Value response = send_recv(request);
    int32_t status = response["status"].asInt();
    if (status == 200) {
        std::cout << "Successfully removed an entry\n";
    } else if (status == 404) {
        std::cout << "Requested entry was not found\n";
    }
}

void Client::add() {
    Json::Value request;
    request["command"] = "add";
    request["arg"]["name"] = input("Name (string): ");
    request["arg"]["price"] = std::stoi(input("Price (int): "));
    request["arg"]["weight"] = std::stod(input("Weight (float): "));
    Json::Value response = send_recv(request);
    int32_t status = response["status"].asInt();
    if (status == 200) {
        std::cout << "New robot added with ID = " << response["result"] << '\n';
    }
}

void Client::update() {
    Json::Value request;
    request["command"] = "update";
    request["arg"]["id"] = std::stoi(input("ID (int): "));
    request["arg"]["name"] = input("New name (string): ");
    request["arg"]["price"] = std::stoi(input("New price (int): "));
    request["arg"]["weight"] = std::stod(input("New weight (float): "));
    Json::Value response = send_recv(request);
    int32_t status = response["status"].asInt();
    if (status == 200) {
        std::cout << "Successfully updated an entry\n";
    } else if (status == 404) {
        std::cout << "Requested entry was not found\n";
    }
}

void Client::find_all() {
    Json::Value request;
    request["command"] = "find_all";
    std::string field = input("Field to search by (name/price/weight): ");
    std::string match = input("Value of the field: ");
    if (field == "name") {
        request["arg"][field] = match;
    } else if (field == "price") {
        request["arg"][field] = std::stoi(match);
    } else if (field == "weight") {
        request["arg"][field] = std::stod(match);
    } else {
        std::cerr << "Invalid field name\n";
        return;
    }
    Json::Value response = send_recv(request);
    int32_t status = response["status"].asInt();
    if (status == 200) {
        std::cout << "Found " << response["result"].size() << " entries\n";
        for (const auto& entry: response["result"]) {
            dump_entry(entry);
            std::cout << '\n';
        }
    }
}

void Client::dump() {
    Json::Value request;
    request["command"] = "find_all";
    request["arg"] = Json::objectValue;
    Json::Value response = send_recv(request);
    int32_t status = response["status"].asInt();
    if (status == 200) {
        std::cout << "Found " << response["result"].size() << " entries\n";
        for (const auto& entry: response["result"]) {
            dump_entry(entry);
            std::cout << '\n';
        }
    }
}
