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
        std::cout << "Welcome to the RobotsDB!\nType a command ('help' for help)\n";
}

void Client::mainloop() {
    std::string cmd;
    std::cout << ">> ";
    std::getline(std::cin, cmd);
    if (cmd == "help") {
        std::cout << HELP;
    } else if (cmd == "ping") {
        ping();
    } else if (cmd == "quit" || std::cin.eof()) {
        exit(0);
    } else if (cmd != "") {
        std::cerr << "Invalid command\n";
    }
}

void Client::ping() {
    std::string request_str = "{\"command\": \"ping\"}";
    zmq::message_t request(request_str.size());
    memcpy(request.data(), request_str.c_str(), request_str.size());
    socket.send(request);
    zmq::message_t reply;
    socket.recv(&reply);
    std::string reply_str(static_cast<char*>(reply.data()), reply.size());
    std::cout << reply_str << '\n';
}
