#include "client.h"
#include <iostream>

static const char *HELP = R"(COMMANDS:
    ping       - check connection to the server
    add        - add a new robot to the database
    update     - update an existing robot in the database
    remove     - remove an existing robot in the database
    find       - find an existing robot by its ID
    find-all   - find all robots with matching fields
    quit       - quit the client
)";

static std::string input(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

Client::Client(const std::string& host, const std::string& port):
    context(1), socket(context, ZMQ_REQ) {
        std::string address = "tcp://" + host + ":" + port;
        std::cout << "Conntecting to " << address << " ...\n";
        socket.connect(address);
}

Json::Value Client::send_recv(const std::string& request_str) {
    zmq::message_t request(request_str.size());
    memcpy(request.data(), request_str.c_str(), request_str.size());
    socket.send(request);
    zmq::message_t reply;
    socket.recv(&reply);
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    // Json::Value json; return reader.parse(reply_str, json);
    std::cout << reply_str;
    return Json::Value();
}

void Client::mainloop() {
    std::string cmd;
    std::cout << "Welcome to the RobotsDB!\n"
              << "Type a command ('help' for help)\n";
    do {
        std::cout << ">> ";
        std::getline(std::cin, cmd);
        if (cmd == "help") {
            std::cout << HELP;
        } else if (cmd == "ping") {
            ping();
        } else if (cmd == "find") {
            find();
        } else if (cmd == "update") {
            update();
        } else if (cmd == "find-all") {
            find_all();
        } else if (cmd == "add") {
            add();
        } else if (cmd == "remove") {
            remove();
        } else if (cmd != "" && cmd != "quit") {
            std::cerr << "Invalid command\n";
        }
    } while (cmd != "quit" && !std::cin.eof());
}

void Client::ping() {
    Json::Value request;
    request["command"] = "ping";
    send_recv(writer.write(request));
}

void Client::find() {
    Json::Value request;
    request["command"] = "find";
    request["arg"] = std::stoi(input("ID (int): "));
    send_recv(writer.write(request));
}

void Client::remove() {
    Json::Value request;
    request["command"] = "remove";
    request["arg"] = std::stoi(input("ID (int): "));
    send_recv(writer.write(request));
}

void Client::add() {
    Json::Value request;
    request["command"] = "add";
    request["arg"]["name"] = input("Name (string): ");
    request["arg"]["price"] = std::stoi(input("Price (int): "));
    request["arg"]["weight"] = std::stod(input("Weight (float): "));
    send_recv(writer.write(request));
}

void Client::update() {
    Json::Value request;
    request["command"] = "update";
    request["arg"]["id"] = std::stoi(input("ID (int): "));
    request["arg"]["name"] = input("New name (string): ");
    request["arg"]["price"] = std::stoi(input("New price (int): "));
    request["arg"]["weight"] = std::stod(input("New weight (float): "));
    send_recv(writer.write(request));
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
        // ERROR
    }
    send_recv(writer.write(request));
}
