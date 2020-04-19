#include "client.h"
#include <iostream>

static const char *HELP = R"(COMMANDS:
    ping       - check connection to the server
    add        - add a new robot to the database
    update     - update an existing robot in the database
    remove     - remove an existing robot in the database
    find       - find an existing robot by its ID
    find-all   - find all robots with matching fields
    info       - get information about the database
    quit       - quit the client
)";

Client::Client(std::string host, std::string port):
    context(1), socket(context, ZMQ_REQ) {
        std::string address = "tcp://" + host + ":" + port;
        std::cout << "Conntecting to " << address << "...\n";
        socket.connect(address);
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
        } else if (cmd == "remove") {
            remove();
        } else if (cmd != "" && cmd != "quit") {
            std::cerr << "Invalid command\n";
        }
    } while (cmd != "quit" && !std::cin.eof());
}

void Client::ping() {
    std::string request_str = "{\"command\":\"ping\"}";
    zmq::message_t request(request_str.size());
    memcpy(request.data(), request_str.c_str(), request_str.size());
    socket.send(request);
    zmq::message_t reply;
    socket.recv(&reply);
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    std::cout << reply_str << '\n';
}

void Client::find() {
    std::cout << "ID (int): ";
    std::string id_str;
    std::getline(std::cin, id_str);
    size_t id = std::stoull(id_str); // JANKY way to check for validity;
    std::string request_str = "{\"command\":\"find\",\"arg\":" 
                                    + std::to_string(id) + "}";
    zmq::message_t request(request_str.size());
    memcpy(request.data(), request_str.c_str(), request_str.size());
    socket.send(request);
    zmq::message_t reply;
    socket.recv(&reply);
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    std::cout << reply_str << '\n';
}

void Client::remove() {
    std::cout << "ID (int): ";
    std::string id_str;
    std::getline(std::cin, id_str);
    size_t id = std::stoull(id_str); // JANKY way to check for validity;
    std::string request_str = "{\"command\":\"remove\",\"arg\":" 
                                    + std::to_string(id) + "}";
    zmq::message_t request(request_str.size());
    memcpy(request.data(), request_str.c_str(), request_str.size());
    socket.send(request);
    zmq::message_t reply;
    socket.recv(&reply);
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    std::cout << reply_str << '\n';
}
